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
 * Bit 6: RS, Bit 5: RW, Bit 4: E, Bit 3: D7, Bit 2: D6, Bit 1: D5, Bit 0: D4
 * 
 */
int display_clear(char deviceid){
    //leert das gesamte Display 
}

int display_init(char deviceid){
    //initiiert das LCD und führt einen clear aus  
}

int display_write(char deviceid, char *text){
    //schreibt eine Abfolge von Zeichen sequentiell auf das Display
}