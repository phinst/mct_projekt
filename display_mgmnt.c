#include "display_mgmnt.h"
#include <p33FJ128GP802.h>
//Prozessorspezifische Headerdatei
#include "i2c_mgmnt.h"
#include "timer_mgmnt.h"
#include "port_mgmnt.h"
//custom headers

/*I2C Backpack: PCF8574AT, K44001, 05, KNM00463
 *standard I2C Adresse des PCF8574AT ICs ist 0x3F
 * I2C Datenword hat 8 Bit, wovon die unteren vier Bit für die Steuerung 
 * des Displays parallel an das HD44780U weitergegeben werden. Die oberen
 * drei Bit sind das enable, register select und das r/w Bit!
 * 
 * Die Frage ist, wo am IO Port des Displays das LSB des seriellen Datenwortes ausgegeben wird.
 * Pins am Display:     x   x   x   RS  RW  E   DB0 DB1 DB2 DB3 DB4 DB5 DB6 DB7 x   x
 * Pins vom PCF8574AT:              P0  P1  P2                  P4 P5 P6 P7
 * ==> Datenwort muss wie folgt an den PCF8574AT Übertragen werden: (MSB) DB7 DB6 DB5 DB4 x E RW RS (LSB)
 *  
 * Display muss also im 4 Bit Mode betrieben werden:
 * Es werden nur DB7-DB4 genutzt, 8 Bit Daten werden als zwei mal 4 Bit übertragen,
 * wobei erst die oberen und dann die unteren Bits gesendet werden. Nach jedem Paar 4 Bit 
 * Daten muss die Busy Flag abgefragt werden.
 * 
 * Das Display wird anfangs durch den Internal Reset Circuit mit verschiedenen Parametern initialisiert.
 * Für unseren Zweck müssen wir ein paar dieser Parameter ändern (siehe display_init).
 * 
 * Grundlegende Befehle:
 * RS 0, RW 0: Instruction Register write as internal operation
 * RS 0, RW 1: Read Busy Flag and AC (DB0 bis DB7)
 * RS 1, RW 0: Data Register write as internal operation
 * RS 1, RW 1: Data Register read as internal operation
 *
 * function set Befehl:
 * RS   RW  DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
 * 0    0   0   0   1   DL  N   F   x   x
 * DL data/interface length: 1 8 Bit, 0 4 Bit
 * N number of display lines: 1 2 lines, 0 1 line
 * F character font: 1 5x10 dots, 0 5x8 dots
 * 
 * display control Befehl:
 * RS   RW  DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
 * 0    0   0   0   0   0   1   D   C   B
 * D display on/off
 * C cursor on/off
 * B blinking cursor on/off
 * 
 * entry mode Befehl:
 * RS   RW  DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
 * 0    0   0   0   0   0   0   1   I/D S
 * I/D: 0 decrement, 1 increment
 * S: 1 accompanies display shift
 * 
 */

#define BACKLIGHT 0x08
#define ENABLE 0x04
#define RW 0x02
#define RS 0x01

int waitfor = 1;
int timecount = 0;

void waitXus(int val){
    //Werte für das Warten mit Timer 4:
    //21500: 4,3 ms
    //600: 120us
    //250: 50us
    
    timecount = 0;
    set_timer4(0, val, 16);
    while (timecount == 0);
    timecount = 0;
    T4CONbits.TON = 0;
    //Disable Timer 4
}

void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void) {

    waitfor = 0;
    timecount++;

    //T4CONbits.TON = 0;
    //Timer 4 wieder ausschalten
    IFS1bits.T4IF = 0;
    //Clear Timer 4 Interrupt Flag
}

void send_8(char deviceid, unsigned int data, int type){
    //data: 8 Bit, MSB DB7 ... DB0 LSB
    //type: 0 instructions (RW 0, RS 0), 1 DDRAM (RW 0, RS 1)

    char select = 0;
    if(type) select = RS;
    //
    
    i2c_start();
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write((data & 0xF0) | ENABLE | BACKLIGHT | select);
    //übertragen der der oberen Bit mit E high
    
    waitXus(1);
    
    i2c_write((data & 0xF0) | BACKLIGHT | select);
    //übertragen der der oberen Bit mit E low
    
    waitXus(1);
    
    i2c_write((data << 4) | ENABLE | BACKLIGHT | select);
    //übertragen der der unteren Bit mit E high
    
    waitXus(1);
    
    i2c_write((data << 4) | BACKLIGHT | select);
    //übertragen der der unteren Bit mit E low
    i2c_stop();
    
    waitXus(250);
}

void send_4(char deviceid, unsigned int data, int type) {
    //data: 4 Bit, MSB DB7 ... DB5 LSB
    //type: 0 instructions (RW 0, RS 0), 1 DDRAM (RW 0, RS 1)
    
    char select = 0;
    if(type) select = RS;
    
    i2c_start();
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write((data << 4) | ENABLE | BACKLIGHT | select);
    //übertragen der der oberen Bit mit E high
    
    waitXus(1);
    
    i2c_write((data << 4) | BACKLIGHT | select);
    //übertragen der der oberen Bit mit E low
    i2c_stop();
    
    waitXus(250);
}

int check_bf(char deviceid) {
    //ließt des Status des busy flags aus und übergibt ihn

    unsigned char bf = 0;
    
    i2c_start();
    //starten des Bus
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0x00 | ENABLE | RW);
    //Schreiben der busy flag read instruction mit enable high
    
    waitXus(1);
    
    i2c_write(0x00 | RW);
    //Schreiben der busy flag read instruction mit enable low
    
    waitXus(1);
    
    i2c_restart();
    //Neustart des Bus
    i2c_write((deviceid << 1) | 0x01);
    //ansprechen des Slaves mit Lesebefehl
    bf = i2c_read(8);
    //Funktion muss auch vor dem Einstellen der 4 Bit operation funktionieren
    //eigentlich nur die oberen 4 Bit interessant, deshalb nur einmaliges Lesen
    //zurückgegebene Bits entsprechen: x RS RW E DB7 DB6 DB5 DB4
    i2c_stop();
    //stoppen des Bus
    
    waitXus(250);
    
    if (bf & 0x80) return 1;
        //busy flag up!
    else return 0;
    //no busy flag
}

void display_clear(char deviceid) {
    //leert das gesamte Display
    while(check_bf(deviceid)); 
    //wait for busy flag to clear
    send_8(deviceid, 0x01, 0);
}

void display_init(char deviceid) {
    //initiiert das LCD und führt einen clear aus
    
    timecount = 0; 
    set_timer4(0, 25000, 16);
    while (timecount < 10);
    //warte auf betriebsspannung ~50ms
    T4CONbits.TON = 0;
    //Disable Timer 4
    timecount = 0;

    send_4(deviceid, 0b0011, 0);       
    waitXus(21500);
    //warte auf Verarbeitung
    
    send_4(deviceid, 0b0011, 0);     
    waitXus(21500);
    //warte auf Verarbeitung
    
    send_4(deviceid, 0b0011, 0);  
    waitXus(21500);
    //warte auf Verarbeitung

    send_4(deviceid, 0b0011, 0);
    waitXus(21500);
    //warte auf Verarbeitung
    
    send_4(deviceid, 0b0010, 0);
    waitXus(21500);
    //warte auf Verarbeitung
    
    while(check_bf(deviceid));
    //wait for busy flag to clear

    send_8(deviceid, 0b00111000, 0);
    //übertragen der function set Anweisung
    //4 Bit mode, 2 lines, 5x8 dots
    
    while(check_bf(deviceid));
    //wait for busy flag to clear
    
    send_8(deviceid, 0b00001111, 0);
    //übertragen einer instruction set Anweisung
    //display & cursor on, blinking cursor on

    display_clear(deviceid);

    while(check_bf(deviceid));
    //wait for busy flag to clear

    send_8(deviceid, 0b00000111, 0);
    //übertragen einer instruction set Anweisung
    //entry mode set

    waitXus(21500);
}

int display_write(char deviceid, char *text) {
    //schreibt eine Abfolge von Zeichen sequentiell auf das Display
    return 1;
}
