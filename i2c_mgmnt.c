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
 * 1 = Master transmit is in progress (8 bits + ACK)
 * 0 = Master transmit is not in progress
 * 
 * 
 * Generelle Funktionsstruktur nach Patz:
 * start - startbiz, deviceid, ACK y/n
 * writedata - data (reg addr / data), ACK y/n
 * stop
 * readdata - data, ACK y/n
 */

#include "i2c_mgmnt.h"
#include "p33FJ128GP802.h"

void i2c_init(void) { 
    /*I2C1BRG = [( 1/Fscl - PGD)*Fcy/2]-2 = 195,4
     *Fcy = 40MHz
     *Fscl = 100KHz
     *PGD = 130ns
     */
    pin_cfg(17, 'd', 'i');
    pin_cfg(18, 'd', 'i');
    //default Konfiguration der Pins
    I2C1BRG = 195; 
    //set baud rate
    I2C1CONbits.I2CEN = 1; 
    //enable I2C functionality
    while(I2C1CONbits.I2CEN == 0);
    //wait until enabled
}

int I2C_WriteReg(char dev_addr, char reg_addr, char value)
{
    char wr_dev_addr = dev_addr << 1; //first bit is 
    //IF der I2C  Master Events klären
    IFS1bits.MI2C1IF = 0;
    //send I2C start condition
	I2C1CONbits.SEN = 1;
    //warten auf interrupt flag zur Bestätigung
	while(!IFS1bits.MI2C1IF == 1);
    //IF der I2C  Master Events klären
    IFS1bits.MI2C1IF = 0;
    
	// Send I2C device address on the bus for write operation
	I2C1TRN = wr_dev_addr;	
    //warten auf interrupt flag zur Bestätigung
	while(!IFS1bits.MI2C1IF == 1);
    //IF der I2C  Master Events klären
    IFS1bits.MI2C1IF = 0;
	//Fehler?			
	if (I2C1STATbits.ACKSTAT){											
		return 0;					
	}
    
    // Send register address on the bus
	I2C1TRN = reg_addr;
	//warten auf interrupt flag zur Bestätigung
	while(!IFS1bits.MI2C1IF == 1);
    //IF der I2C  Master Events klären
    IFS1bits.MI2C1IF = 0;
	//Fehler?		
	if (I2C1STATbits.ACKSTAT){											
		return 0;					
	}
    
	// Send register value on the bus    
	I2C1TRN = value;
	//warten auf interrupt flag zur Bestätigung
	while(!IFS1bits.MI2C1IF == 1);
    //IF der I2C  Master Events klären
    IFS1bits.MI2C1IF = 0;
	//Fehler?		
	if (I2C1STATbits.ACKSTAT){										
		return 0;					
	}
    
	//Send I2C stop condition
	I2C1CONbits.PEN = 1;
	//warten auf interrupt flag zur Bestätigung
	while(!IFS1bits.MI2C1IF == 1);
    //IF der I2C  Master Events klären
    IFS1bits.MI2C1IF = 0;
	//Fehler?			
	if (I2C1STATbits.ACKSTAT)				
	{			
		return 0;					
	}
	return 1;
}

int i2c_start(char deviceid){
IFS1bits.MI2C1IF = 0;
//löschen des Interrpt Flags
I2C1CONbits.SEN = 1;
//send I2C start condition
while(!IFS1bits.MI2C1IF == 1);
//warten auf interrupt flag zur Bestätigung

IFS1bits.MI2C1IF = 0;
//IF der I2C  Master Events klären

// Send I2C device address on the bus for write operation
I2C1TRN = deviceid;	
//warten auf interrupt flag zur Bestätigung
while(!IFS1bits.MI2C1IF == 1);
//IF der I2C  Master Events klären
IFS1bits.MI2C1IF = 0;
			
if (I2C1STATbits.ACKSTAT){
    //Fehler?											
    return 0;					
}

return 1;
//Start erfolgreich
}
/*
int i2c_start(deviceid){
// Send I2C device address on the bus for write operation
	I2C1TRN = wr_dev_addr;	
    //warten auf interrupt flag zur Bestätigung
	while(!IFS1bits.MI2C1IF == 1);
    //IF der I2C  Master Events klären
    IFS1bits.MI2C1IF = 0;
	//Fehler?			
	if (I2C1STATbits.ACKSTAT){											
		return 0;					
	}
}*/