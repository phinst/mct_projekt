#include "display_mgmnt.h"
#include <p33FJ128GP802.h>
//Prozessorspezifische Headerdatei
#include "i2c_mgmnt.h"
//custom headers

/*I2C Backpack: PCF8574AT, K44001, 05, KNM00463
 *standard I2C Adresse des PCF8574AT ICs ist 0x3F
 * I2C Datenword hat 8 Bit, wovon die unteren vier Bit für die Steuerung 
 * des Displays parallel an das HD44780U weitergegeben werden. Die oberen
 * drei Bit sind das enable, register select und das r/w Bit!
 * 
 * Die Frage ist, wo am IO Port des Displays das LSB des seriellen Datenwortes ausgegeben wird.
 * Pins am Display:     x   x   x   RS  RW  E   DB0 DB1 DB2 DB3 DB4 DB5 DB6 DB7 x   x
 * Pins vom PCF8574AT:              P6  P5  P4                  P0  P1  P2  P3
 * ==> Datenwort muss wie folgt an den PCF8574AT Übertragen werden: (MSB) x RS RW E DB7 DB6 DB5 DB4 (LSB)
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
 * C cursor on/offRS
 * B blinking cursor on/off
 * 
 * entry mode Befehl:
 * RS   RW  DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
 * 0    0   0   0   0   0   0   1   I/D S
 * I/D: 0 decrement, 1 increment
 * S: 1 accompanies display shift
 * 
 */

int check_bf(char deviceid){
    //ließt des Status des busy flags aus und übergibt ihn
    
    unsigned char bf=0;
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0b00110000);
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
    if(bf & 0x08) return 1;
    //busy flag up!
    else return 0;
    //no busy flag
}

int display_clear(char deviceid){
    //leert das gesamte Display

    while(check_bf);
    //wait for busy flag to clear
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0b00010000);
    //übertragen einer instruction set Anweisung Teil 1
    i2c_write(0b00010001);
    //übertragen einer instruction set Anweisung Teil 2
    //display clear
}

int display_init(char deviceid){
    //initiiert das LCD und führt einen clear aus
    
    i2c_start();
    //starten des i2c Bus
    while(check_bf);
    //wait for busy flag to clear
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0b00010010);
    //übertragen einer function set Anweisung
    //4 Bit Mode
    while(check_bf);
    //wait for busy flag to clear
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0b00010010);
    //übertragen der function set Anweisung Teil 1
    i2c_write(0b00011000);
    //übertragen der function set Anweisung Teil 2
    //4 Bit mode, 2 lines, 5x8 dots
    while(check_bf);
    //wait for busy flag to clear
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0b00010000);
    //übertragen einer instruction set Anweisung Teil 1
    i2c_write(0b00011110);
    //übertragen einer instruction set Anweisung Teil 2
    //display & cursor on, blinking cursor off
    while(check_bf);
    //wait for busy flag to clear 
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0b00010000);
    //übertragen einer instruction set Anweisung Teil 1
    i2c_write(0b00010110);
    //übertragen einer instruction set Anweisung Teil 2
    //entry mode set

    display_clear(deviceid);
    //execute display clear
    i2c_stop();
}

int display_write(char deviceid, char *text){
    //schreibt eine Abfolge von Zeichen sequentiell auf das Display
}