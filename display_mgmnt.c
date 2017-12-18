#include "display_mgmnt.h"
#include <p33FJ128GP802.h>
//Prozessorspezifische Headerdatei
#include "i2c_mgmnt.h"
#include "timer_mgmnt.h"
#include "port_mgmnt.h"
//custom headers

int waitfor = 1;
int timecount = 0;

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

void display_send(unsigned int data){
    //data: 8 Bit, MSB DB7 ... DB0 LSB
    //nur instructions: RS 0, RW 0
    
    i2c_write((data & 0xF0) | 0x04);
    //übertragen der der oberen Bit mit E high
    set_timer4(0, 1, 16);
    while(timecount == 0);
    timecount = 0;
    T4CONbits.TON = 0;      
    //Disable Timer 4
    i2c_write(data & 0xF0);
    //übertragen der der oberen Bit mit E low
    set_timer4(0, 250, 16);
    while(timecount == 0);
    timecount = 0;
    T4CONbits.TON = 0;      
    //Disable Timer 4
    i2c_write((data<<4) | 0x04);
    //übertragen der der unteren Bit mit E high
    set_timer4(0, 1, 16);
    while(timecount == 0);
    timecount = 0;
    T4CONbits.TON = 0;      
    //Disable Timer 4
    i2c_write(data<<4);
    //übertragen der der unteren Bit mit E low
    set_timer4(0, 250, 16);
    while(timecount == 0);
    timecount = 0;
    T4CONbits.TON = 0;      
    //Disable Timer 4
}

int check_bf(char deviceid){
    //ließt des Status des busy flags aus und übergibt ihn
    
    unsigned char bf=0;
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0b00000110);
    //Schreiben der busy flag read instruction
    //setzen des enable bits gleichzeitig mit dem Rest. Eventuell früher nötig?
    i2c_restart();
    //Neustart des Bus
    i2c_write((deviceid << 1) | 0x1);
    //ansprechen des Slaves mit Lesebefehl
    bf=i2c_read(8);
    //Funktion muss auch vor dem Einstellen der 4 Bit operation funktionieren
    //eigentlich nur die oberen 4 Bit interessant, deshalb nur einmaliges Lesen
    //zurückgegebene Bits entsprechen: x RS RW E DB7 DB6 DB5 DB4
    i2c_restart();
    //Neustart des Bus
    if(bf & 0x80) return 1;
    //busy flag up!
    else return 0;
    //no busy flag
}

void display_clear(char deviceid){
    //leert das gesamte Display

    while(check_bf(deviceid));
    //wait for busy flag to clear
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0b00000100);
    //übertragen einer instruction set Anweisung Teil 1
    i2c_write(0b00010100);
    //übertragen einer instruction set Anweisung Teil 2
    //display clear
}

void display_init(char deviceid){
    //initiiert das LCD und führt einen clear aus
    //Werte für das Warten mit Timer 4:
    //25000 5 ms
    //600 120us
    //250 50us
    timecount = 0;
    
    set_timer4(0, 25000, 16);
    while(timecount < 10);
    timecount = 0;
    //40ms warten
    T4CONbits.TON = 0;      
    //Disable Timer 4
    i2c_start();
    //starten des i2c Bus
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    
    i2c_write(0b00110100);
    //übertragen einer function set Anweisung
    i2c_write(0b00110000);
    //übertragen einer function set Anweisung
    
    i2c_stop();
    set_timer4(0, 25000, 16);
    while(!timecount);
    timecount=0;
    //4,3ms warten
    T4CONbits.TON = 0;      
    //Disable Timer 4
    i2c_start();
    //starten des i2c Bus
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    
    i2c_write(0b00110100);
    //übertragen einer function set Anweisung
    i2c_write(0b00110000);
    //übertragen einer function set Anweisung
    
    i2c_stop();
    set_timer4(0, 600, 16);
    while(!timecount);
    timecount=0;
    //100us warten
    T4CONbits.TON = 0;      
    //Disable Timer 4
    
    
    i2c_start();
    //starten des i2c Bus
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    
    i2c_write(0b00110100);
    //übertragen einer function set Anweisung
    i2c_write(0b00110000);
    //übertragen einer function set Anweisung
    
    while(check_bf(deviceid));
    //wait for busy flag to clear
    
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    display_send(0b00111000);
    //übertragen der function set Anweisung
    //4 Bit mode, 2 lines, 5x8 dots
    while(check_bf(deviceid));
    //wait for busy flag to clear
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    display_send(0b00001000);
    //übertragen einer instruction set Anweisung
    //display & cursor off, blinking cursor off
    
    display_clear(deviceid);
    
    while(check_bf(deviceid));
    //wait for busy flag to clear
    
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    display_send(0b00000110);
    //übertragen einer instruction set Anweisung
    //entry mode set
    
    
    i2c_stop();
    set_timer4(0, 25000, 16);
    while(timecount < 10);
    timecount = 0;
    //40ms warten
    T4CONbits.TON = 0;  
}

void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void){
    
    waitfor=0;
    timecount++;
    
    //d_write(25, ~d_read(25));
    //T4CONbits.TON = 0;
    //Timer 4 wieder ausschalten
    IFS1bits.T4IF = 0;
    //Clear Timer 4 Interrupt Flag
}

int display_write(char deviceid, char *text){
    //schreibt eine Abfolge von Zeichen sequentiell auf das Display
    return 1;
}