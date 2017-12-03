/* I2C Register - Übersicht
 * http://ww1.microchip.com/downloads/en/DeviceDoc/70000195f.pdf
 * 
 * I2CxCON: I2Cx Control Register
 * I2CxSTAT: I2Cx Status Register
 * I2CxMSK: I2Cx Slave Mode Address Mask Register
 *      -> für mehrere Adressen
 * 
 * I2CxRCV: I2Cx Receive Buffer Register
 *      -> hier werden Daten gelesen
 * 
 * I2CxTRN: I2Cx Transmit Register
 *      -> hier werden Daten gesendet
 * 
 * I2CxADD: I2Cx Address Register
 *      -> Slave Adresse
 * 
 * I2CxBRG: I2Cx Baud Rate Generator Reload Register
 * 
 * ACKSTAT:
 *  Acknowledge Status bit 
 * 1 = NACK received from slave
 * 0 = ACK received from slave
 *  * 
 * TRSTAT: Transmit Status bit (I2C? Master mode transmit operation)
1 = Master transmit is in progress (8 bits + ACK)
0 = Master transmit is not in progress
 */

#include "i2c_mgmnt.h"
#include "p33FJ128GP802.h"

void i2c_init(void) {

    I2CCONbits.I2CEN = 1; // enable I2C functionality
    I2CCONbits.I2CSIDL = 0; // continue when idle
    I2CCONbits.SCLREL = 1; // used in master mode           (nicht 100% sicher)
    I2CCONbits.IPMIEN = 0; // don't use IPMI                (ein industriestandard, der dinge verumständlicht)
    I2CCONbits.A10M = 0; // use 7-bit addressing
    I2CCONbits.DISSLW = 1; // slew rate control disabled (nicht nötig in slow mode)
    I2CCONbits.SMEN = 0; // don't use SMBus thresholds
    I2CCONbits.GCEN = 0; //General call address disabled
    I2CCONbits.STREN = 0; //Disable Software or release clock 

    /*I2CBRG = [( 1/Fscl - PGD)*Fcy]-2 = 393
     * Fcy = 40MHz
     * Fscl = 100KHz
     * PGD = 130ns
     */
    I2C1BRG = 393; // set the baud rate
 
    __delay_ms(1);

}

int I2C_WriteReg(char dev_addr, char reg_addr, char value)
{
    char wr_dev_addr = dev_addr << 1; //first bis is ACK
    // Send I2C start condition
	I2CCONbits.SEN = 1;			
	while(I2CCONbits.SEN == 1);
	// Send I2C device address on the bus for write operation
	I2CTRN = wr_dev_addr;			
	while(I2CSTATbits.TRSTAT);			
	if (I2CSTATbits.ACKSTAT)				
	{								
		I2CCONbits.PEN = 1;
		while(I2CCONbits.PEN);			
		return I2C_ERROR;					
	}
    // Send register address on the bus
	I2CTRN = reg_addr;
	while(I2CSTATbits.TRSTAT);
	if (I2CSTATbits.ACKSTAT)
	{
		I2CCONbits.PEN = 1;
		while(I2CCONbits.PEN);
		return I2C_ERROR;
	}
	// Send register value on the bus    
	I2CTRN = value;
	while(I2CSTATbits.TRSTAT);
	if (I2CSTATbits.ACKSTAT)
	{
		I2CCONbits.PEN = 1;
		while(I2CCONbits.PEN);
		return I2C_ERROR;
	}
	/// Send I2C stop condition
	I2CCONbits.PEN = 1;
	while(I2CCONbits.PEN);
	return I2C_OK;
}