#ifndef I2C_MGMNT_H
#define	I2C_MGMNT_H
  

#ifdef	__cplusplus
extern "C" {
#endif

extern void i2c_init(void);
extern int i2c_start(void);
extern int i2c_restart(void);
extern int i2c_stop(void);
extern int i2c_write(char data);
extern void i2c_ack(int yn);
extern char i2c_read(char deviceid);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* I2C_MGMNT_H */

