#include <p33FJ128GP802.h>
//Prozessorspezifische Headerdatei

#include "port_mgmnt.h"
//#include "adw_mgmnt.h"
//#include "pwm_mgmnt.h"
#include "system_init.h"
//#include "read_write.h"
//#include "timer_mgmnt.h"
#include "i2c_mgmnt.h"
#include "clk_mgmnt.h"
//custom headers

void main(void) {
    //clk_boost(80);
    clr_ports();
    i2c_init();
    //setup
    
    char deviceid=0x3F;
    char bf_out=0;
    
    unsigned int vierkommaeins=3500,i,j,hundertmikro=67;
    
    while(1){
        i2c_start();
        i2c_write(deviceid<<1);
        i2c_write(0b00110000);
        i2c_write(0b00001110);
        i2c_write(0b00000110);
        i2c_restart();
        i2c_write(deviceid<<1);
        i2c_write(0b01001000);
        
        /*i2c_start();
        i2c_write(deviceid<<1);
        i2c_write(0b00111000);
        //8 Bit operation
        for(j=0; j<=3000; j++);
        
        i2c_write(0b00111000);
        //8 Bit operation 
        for(j=0; j<=3000; j++);
        
        i2c_write(0b00111000);
        //8 Bit operation
        
        while(bf_out=i2c_read(deviceid)&0b10000000);
        i2c_restart();
        i2c_write(deviceid<<1);
        
        i2c_write(0b00111110);
        //Display off
        i2c_write(0b00001000);
        //Display clear
        i2c_write(0b00000111);
        //entry mode
        i2c_stop();*/
 
        /*
        bf_out=i2c_read(deviceid);
        i2c_stop();
        b=1;
        a=1;*/
        
    }
}