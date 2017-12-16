#include "display_mgmnt.h"
#include <p33FJ128GP802.h>
//Prozessorspezifische Headerdatei
#include "i2c_mgmnt.h"
//custom headers

/*I2C Backpack: PCF8574AT, K44001, 05, KNM00463
 *standard I2C Adresse des PCF8574AT ICs ist 0x3F
 * I2C Datenword hat 8 Bit, wovon die unteren vier Bit f�r die Steuerung 
 * des Displays parallel an das HD44780U weitergegeben werden. Die oberen
 * drei Bit sind das enable, register select und das r/w Bit!
 * 
 * Die Frage ist, wo am IO Port des Displays das LSB des seriellen Datenwortes ausgegeben wird.
 * Pins am Display:     x   x   x   RS  RW  E   DB0 DB1 DB2 DB3 DB4 DB5 DB6 DB7 x   x
 * Pins vom PCF8574AT:              P6  P5  P4                  P0  P1  P2  P3
 * ==> Datenwort muss wie folgt an den PCF8574AT �bertragen werden: (MSB) x RS RW E DB7 DB6 DB5 DB4 (LSB)
 *  
 * Display muss also im 4 Bit Mode betrieben werden:
 * Es werden nur DB7-DB4 genutzt, 8 Bit Daten werden als zwei mal 4 Bit �bertragen,
 * wobei erst die oberen und dann die unteren Bits gesendet werden. Nach jedem Paar 4 Bit 
 * Daten muss die Busy Flag abgefragt werden.
 * 
 * Das Display wird anfangs durch den Internal Reset Circuit mit verschiedenen Parametern initialisiert.
 * F�r unseren Zweck m�ssen wir ein paar dieser Parameter �ndern (siehe display_init).
 * 
 * Grundlegende Befehle:
 * RS 0, RW 0: Instruction Register write as internal operation
 * RS 0, RW 1: Read Busy Flag and AC (DB0 bis DB7)
 * RS 1, RW 0: Data Register write as internal operation
 * RS 1, RW 1: Data Register read as internal operation
 *
 */

int check_bf(char deviceid){
    //lie�t des Status des busy flags aus und �bergibt ihn
    
    unsigned char bf=0;
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0b00110000);
    //Schreiben der busy flag read instruction
    //setzen des enable bits gleichzeitig mit dem Rest. Eventuell fr�her n�tig?
    i2c_restart();
    //Neustart des Bus
    i2c_write((deviceid << 1) | 0x1);
    //ansprechen des Slaves mit Lesebefehl
    bf=i2c_read(8);
    //Funktion muss auch vor dem Einstellen der 4 Bit operation funktionieren
    //eigentlich nur die oberen 4 Bit interessant, deshalb nur einmaliges Lesen
    //zur�ckgegebene Bits entsprechen: x RS RW E DB7 DB6 DB5 DB4
    i2c_restart();
    //Neustart des Bus
    if(bf & 0x08) return 1;
    //busy flag up!
    else return 0;
    //no busy flag
}

int display_clear(char deviceid){
    //leert das gesamte Display 
}

int display_init(char deviceid){
    //initiiert das LCD und f�hrt einen clear aus
    
    i2c_start();
    //starten des i2c Bus
    while(check_bf);
    //wait for busy flag to clear
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0b00010010);
    //�bertragen einer Function set Anweisung
    //4 Bit Mode
    while(check_bf);
    //wait for busy flag to clear
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0b00010010);
    //�bertragen einer Function set Anweisung
    //4 Bit Mode
    i2c_write(0b00010000);
    //�bertragen einer Function set Anweisung
    //display parameter
    while(check_bf);
    //wait for busy flag to clear
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0b00010000);
    //�bertragen einer Function set Anweisung
    i2c_write(0b00011110);
    //�bertragen einer Function set Anweisung
    //cursor & display turn on
    while(check_bf);
    //wait for busy flag to clear
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    i2c_write(0b00010000);
    //�bertragen einer Function set Anweisung
    i2c_write(0b00010110);
    //�bertragen einer Function set Anweisung
    //entry mode set
    i2c_stop();
}

int display_write(char deviceid, char *text){
    //schreibt eine Abfolge von Zeichen sequentiell auf das Display
}