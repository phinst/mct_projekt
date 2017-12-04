#include <p33FJ128GP802.h>
//Prozessorspezifische Headerdatei
#include "port_mgmnt.h"
#include "adw_mgmnt.h"
#include "pwm_mgmnt.h"
#include "system_init.h"
#include "read_write.h"
#include "timer_mgmnt.h"
//custom headers


#define pin_ausgang 25
#define pin_eingang 26

void main(void) {
    clk_boost(80);
    clr_ports();
    
    pin_cfg(pin_eingang, 'd', 'i');
    pin_cfg(pin_ausgang, 'd', 'o');
    cn_cfg(pin_eingang, 1);
    while(1){
        if(cn_value[0] == 1){
            d_write(pin_ausgang, ~d_read(pin_ausgang));
            cn_value[0]=0;
        }
    }
}