#include <p33FJ128GP802.h>
//Prozessorspezifische Headerdatei
#include "port_mgmnt.h"
#include "adw_mgmnt.h"
#include "pwm_mgmnt.h"
#include "system_init.h"
#include "read_write.h"
#include "timer_mgmnt.h"
#include "i2c_mgmnt.h"
#include "clk_mgmnt.h"
#include "display_mgmnt.h"
//custom headers


#define pin_mode 26
#define pin_plus 25
#define pin_minus 24
#define pin_k 23

#define deviceid 0x3F

char caption_m1[] = "mode 1: single pulse";
char caption_m2[] = "mode 2: double pulse";
char content_m1[] = "pulse 1:";
char content_m2[] = "pulse 2:";
char delete_line[] = "                    ";
char menu_k[] = "mode   +    -   ok  ";
char menu_s[] = "mode   +    -  start";
char p_norm[] = "160ms";
char p_short[] = " 50ms";
int norm_int = 160;
int short_int = 50;

void convert(){
    //erste drei Stellen interessant
    p_norm[0]=(norm_int/100)+'0';
    p_norm[1]=((norm_int/10)%10)+'0';
    p_norm[2]=(norm_int%10)+'0';
    
    p_short[1]=(short_int/100)+'0';
    p_short[2]=((short_int/10)%10)+'0';
}

void waitfunc() {
    timecount = 0;
    set_timer4(0, 25000, 16);
    while (timecount < 100);
    //warte auf betriebsspannung ~50ms
    T4CONbits.TON = 0;
    //Disable Timer 4
    timecount = 0;
}

void mode_1(){
    display_clear(deviceid);
    cursor_move(deviceid, 1, 1);
    display_write(deviceid, caption_m1);
    cursor_move(deviceid, 2, 1);
    display_write(deviceid, content_m1);
    cursor_move(deviceid, 2, 12);
    display_write(deviceid, p_norm);
    cursor_move(deviceid, 3, 1);
    display_write(deviceid, delete_line);
    cursor_move(deviceid, 4, 1);
    display_write(deviceid, menu_k);
}

void mode_2(){
    display_clear(deviceid);
    cursor_move(deviceid, 1, 1);
    display_write(deviceid, caption_m2);
    cursor_move(deviceid, 2, 1);
    display_write(deviceid, content_m1);
    cursor_move(deviceid, 2, 12);
    display_write(deviceid, p_short);
    cursor_move(deviceid, 3, 1);
    display_write(deviceid, content_m2);
    cursor_move(deviceid, 3, 12);
    display_write(deviceid, p_norm);
    cursor_move(deviceid, 4, 1);
    display_write(deviceid, menu_k);
}

void main(void) {

    clk_boost(80);
    clr_ports();
    i2c_init();
    display_init(deviceid);
    
    pin_cfg(pin_mode, 'd', 'i');
    pin_cfg(pin_plus, 'd', 'i');
    pin_cfg(pin_minus, 'd', 'i');
    pin_cfg(pin_k, 'd', 'i');
    //Eingänge für Buttons
    cn_cfg(pin_mode, 1);
    cn_cfg(pin_plus, 1);
    cn_cfg(pin_minus, 1);
    cn_cfg(pin_k, 1);
    //CNs für Buttons
    
    mode_1();
    int mode=1;
    int p_select=1;
    
    while (1) {
        
        if(cn_value[0] == 1){
            //Button gedrückt worden?
            switch(cn_value[1]){
                case pin_mode:
                    if(mode == 1){
                        display_clear(deviceid);
                        display_clear(deviceid);
                        mode_2();
                        mode=2;
                    }
                    else{
                        display_clear(deviceid);
                        display_clear(deviceid);
                        mode_1();
                        mode=1;
                    }
                    break;
                case pin_plus:
                    if(mode == 1){
                        norm_int += 20;
                        if(norm_int > 260) norm_int = 260;
                        convert();
                        cursor_move(deviceid, 2, 12);
                        display_write(deviceid, p_norm);
                    }
                    break;
                case pin_minus:
                    if(mode == 1){
                        norm_int -= 20;
                        if(norm_int < 40) norm_int = 40;
                        convert();
                        cursor_move(deviceid, 2, 12);
                        display_write(deviceid, p_norm);
                    }
                    break;
                case pin_k:
                    break;
            }
            cn_value[0] = 0;
        }
    }
}

/*
    while(1){
        if(cn_value[0] == 1){
            d_write(pin_ausgang, ~d_read(pin_ausgang));
            cn_value[0]=0;
        }
    }*/