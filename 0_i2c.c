#include <p33FJ128GP802.h>
//Prozessorspezifische Headerdatei

#include "port_mgmnt.h"
//#include "adw_mgmnt.h"
//#include "pwm_mgmnt.h"
#include "system_init.h"
//#include "read_write.h"
#include "timer_mgmnt.h"
#include "i2c_mgmnt.h"
#include "clk_mgmnt.h"
#include "display_mgmnt.h"
//custom headers



void main(void) {
    char deviceid=0x3F; 
    
    
    clk_boost(80);
    clr_ports();
    i2c_init();
    //setup
    
    display_init(deviceid);
    
    //pin_cfg(25, 'd', 'o');
    
    //set_timer4(0, 250, 16);
    
    i2c_start();
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    display_send(0b00001111);
    check_bf(deviceid);
    
    int data = 0b01000001;
    
    i2c_write(deviceid << 1);
    //ansprechen des Slaves mit Schreibbefehl
    
    i2c_write((data & 0xF0) | 0x05);
    //übertragen der der oberen Bit mit E high
    set_timer4(0, 1, 16);
    while(timecount == 0);
    timecount = 0;
    T4CONbits.TON = 0;      
    //Disable Timer 4
    i2c_write((data & 0xF0) | 0x01);
    //übertragen der der oberen Bit mit E low
    set_timer4(0, 250, 16);
    while(timecount == 0);
    timecount = 0;
    T4CONbits.TON = 0;      
    //Disable Timer 4
    i2c_write((data<<4) | 0x05);
    //übertragen der der unteren Bit mit E high
    set_timer4(0, 1, 16);
    while(timecount == 0);
    timecount = 0;
    T4CONbits.TON = 0;      
    //Disable Timer 4
    i2c_write((data<<4) | 0x01);
    //übertragen der der unteren Bit mit E low
    set_timer4(0, 250, 16);
    while(timecount == 0);
    timecount = 0;
    T4CONbits.TON = 0;      
    //Disable Timer 4
    
    i2c_write(0b00001000);
    
    i2c_stop();
    
    
    while(1){

        

    }
}