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
 * Acknowledge Status bit 
 * 1 = NACK received from slave
 * 0 = ACK received from slave
 * 
 * TRSTAT: Transmit Status bit (I2C? Master mode transmit operation)
 * 1 = Master transmit is in progress (8 bits + ACK)
 * 0 = Master transmit is not in progress
 * 
 * Beim Senden der Adresse muss an der Stelle Null des Datenwortes ein R/!W Bit eingefügt werden, 
 * sodass der Slave weiß, wer sendet und wer empfängt!
 * 0: master tx, slave rx
 * 1: master rx, slave tx
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
    //I2C1BRG = [( 1/Fscl - PGD)*Fcy/2]-2 = 195,4
    //Fcy = 40MHz
    //Fscl = 100KHz
    //PGD = 130ns
    pin_cfg(17, 'd', 'i');
    pin_cfg(18, 'd', 'i');
    //default Konfiguration der Pins, sonst läuft nix
    //SDA, SCL fest verbunden mit Pin 17 & 18
    I2C1BRG = 195; 
    //set baud rate, siehe oben
    I2C1CONbits.I2CEN = 1; 
    //enable I2C functionality
    while(I2C1CONbits.I2CEN == 0);
    //wait until enabled
}

int i2c_start(void){
    IFS1bits.MI2C1IF = 0;
    //löschen des Interrpt Flags//untere fünf Bit des Control Registers müssen null sein, sonst kein Start zulässig
    I2C1CONbits.SEN = 1;
    //send I2C start condition
    while(!IFS1bits.MI2C1IF == 1);
    //warten auf interrupt flag zur Bestätigung
    IFS1bits.MI2C1IF = 0;
    //IF der I2C  Master Events klären

    if (I2C1STATbits.ACKSTAT){
        //Fehler?											
        return 0;					
    }
    else return 1;
    //Start erfolgreich
}

int i2c_restart(void){
    IFS1bits.MI2C1IF = 0;
    //löschen des Interrpt Flags
    while(I2C1CON && 0x001F);
    //untere fünf Bit des Control Registers müssen null sein, sonst kein Restart zulässig
    I2C1CONbits.RSEN = 1;
    //send I2C restart condition
    while(!IFS1bits.MI2C1IF == 1);
    //warten auf interrupt flag zur Bestätigung
    IFS1bits.MI2C1IF = 0;
    //IF der I2C  Master Events klären

    if (I2C1STATbits.ACKSTAT){
        //Fehler?											
        return 0;					
    }
    else return 1;
    //Restart erfolgreich
}

int i2c_stop(void){
    IFS1bits.MI2C1IF = 0;
    //löschen des Interrpt Flags
    while(I2C1CON && 0x001F);
    //untere fünf Bit des Control Registers müssen null sein, sonst kein Stop zulässig
    I2C1CONbits.PEN = 1;
    //send I2C start condition
    while(!IFS1bits.MI2C1IF == 1);
    //warten auf interrupt flag zur Bestätigung
    IFS1bits.MI2C1IF = 0;
    //IF der I2C  Master Events klären

    if (I2C1STATbits.ACKSTAT){
        //Fehler?											
        return 0;					
    }
    else return 1;
    //Stop erfolgreich
}

int i2c_write(char data){
    IFS1bits.MI2C1IF = 0;
    //IF der I2C  Master Events klären
	I2C1TRN = data;	
    //Schreiben der Daten ins Transmit Register
	while(!IFS1bits.MI2C1IF == 1);
    //warten auf interrupt flag zur Bestätigung
    IFS1bits.MI2C1IF = 0;
    //IF der I2C  Master Events klären
				
	if (I2C1STATbits.ACKSTAT){
        //Fehler?
		return 0;					
	}
    else return 1;
    //Schreiben erfolgreich
}

void i2c_ack(int yn){
    //yn: 0 send ACK, 1 send NACK
    I2C1CONbits.ACKDT=yn;
    //entweder ACK oder NACK senden
    while(I2C1CON && 0x001F);
    //untere fünf Bit des Control Registers müssen null sein, sonst kein Lesen zulässig
    IFS1bits.MI2C1IF = 0;
    //IF der I2C  Master Events klären
    I2C1CONbits.ACKEN=1;
    //starten der acknowledge Sequenz
	while(!IFS1bits.MI2C1IF == 1);
    //warten auf interrupt flag zur Bestätigung
    IFS1bits.MI2C1IF = 0;
    //IF der I2C  Master Events klären
}

char i2c_read(char deviceid){
    char buffer=0;
    //Speicher für den Rückgabewert
    while(I2C1CON && 0x001F);
    //untere fünf Bit des Control Registers müssen null sein, sonst kein Lesen zulässig
    i2c_restart();
    //Neustart des Bus
    deviceid = (deviceid << 1) && 0x01;
    //R/!W Bit 1: master rx, slave tx
    i2c_write(deviceid);
    //Schreiben des Adressbytes mit geändertem R/!W Bit
    I2C1CONbits.RCEN=1;
    //ready to receive
    IFS1bits.MI2C1IF = 0;
    //IF der I2C  Master Events klären
	while(!IFS1bits.MI2C1IF == 1);
    //warten auf interrupt flag zur Bestätigung
    buffer = I2C1RCV;
    //speichern des empfangenen Bytes
    IFS1bits.MI2C1IF = 0;
    //IF der I2C  Master Events klären
    i2c_ack(0);
    //senden eines acknowledge
	return buffer;
    //Rückgabe des empfangenen Wertes
}