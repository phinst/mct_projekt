#include <p33FJ128GP802.h>
//Prozessorspezifische Headerdatei
#include "port_mgmnt.h"
//#include "adw_mgmnt.h"
//#include "pwm_mgmnt.h"
//#include "system_init.h"
//#include "read_write.h"
//#include "timer_mgmnt.h"
#include "i2c_mgmnt.h"
#include "clk_mgmnt.h"
//custom headers

void main(void) {
    clk_boost(80);
    clr_ports();
    i2c_init();
    
    while(1){
        //I2C1CONbits.ACKDT=0;
        I2C_WriteReg(0b1101000, 0x75, 0xFF);
        //I2C_ReadReg(0b11001000, 0x75, &test);
        /*I2C1CONbits.SEN = 1;			
        while(I2C1CONbits.SEN == 1);
        //I2C1TRN = 0x56;*/
        
    }
}