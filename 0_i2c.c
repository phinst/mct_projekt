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
    char deviceid = 0x3F;

    clk_boost(80);
    clr_ports();
    i2c_init();
    //setup

    display_init(deviceid);

    while(1){
        while(check_bf(deviceid));

        char data = 0b11101101;

        send_8(deviceid, data, 1);
    }
}