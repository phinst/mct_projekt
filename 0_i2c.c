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

void waitfunc() {
    timecount = 0;
    set_timer4(0, 25000, 16);
    while (timecount < 100);
    //warte auf betriebsspannung ~50ms
    T4CONbits.TON = 0;
    //Disable Timer 4
    timecount = 0;
}

void main(void) {
    char deviceid = 0x3F;

    clk_boost(80);
    clr_ports();
    i2c_init();
    display_init(deviceid);
    //setup

    char array[] = "1234 56789";
    char test[] = "1";
    int cnt = sizeof (array) / sizeof (char);
    display_clear(deviceid);
    while (1) {
        send_8(deviceid,60, 0);
        send_8(deviceid,'w', 1);
        //display_write(deviceid, test, cnt);
        waitfunc();



    }
}