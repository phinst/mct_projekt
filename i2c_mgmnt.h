
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef I2C_MGMNT_H
#define	I2C_MGMNT_H

//#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef FCY
#define FCY             (unsigned long)10000000 //default FCY 10MHz
#endif
//#include <libpic30.h>
#ifndef I2C_BAUDRATE
#define I2C_BAUDRATE    (unsigned long)100000 //default baud rate 100kHz
#endif
#define I2C_ERROR           -1
#define I2C_OK              1

extern void i2c_init(void);
extern int setBaudRate(void);
extern int I2C_WriteReg(char dev_addr, char reg_addr, char value);
extern int I2C_ReadReg(char dev_addr, char reg_addr, char *value);
    

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

