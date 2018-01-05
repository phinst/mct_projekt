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
//Pinnummern der Knöpfe

#define pin_out 20
//Pinnummer des Outputs

#define long_max 260
#define long_min 60
#define short_max 110
#define short_min 10
//Wertebereich der Pulse in ms

#define deviceid 0x3F

char p_long[] = "160ms";
char p_short[] = " 50ms";
//Anzeigedaten für das GUI
int int_long = 160;
int int_short = 60;

int mode=1;
int select=1;
//Statusvariablen für das GUI

void action(){
    //mögen die Pulse mit Euch sein
    d_write(pin_out, 1);
    if(mode == 2){
        wait_ms(int_short);
        d_write(pin_out, 0);
        //kurzer Puls
        wait_ms(300);
        //Pause
        d_write(pin_out, 1);        
    }
    wait_ms(int_long);
    d_write(pin_out, 0);
    //langer Puls
}

void convert(){
    //Konvertiert Int Variablen in anzeigbaren Text
    //erste drei Stellen interessant
    if(int_long < 100) p_long[0] = ' ';
    //Leerzeichen, falls erste Stelle nicht vorhanden
    else p_long[0]=(int_long/100)+'0';
    
    p_long[1]=((int_long/10)%10)+'0';
    p_long[2]=(int_long%10)+'0';
    
    if(int_short < 100) p_short[0] = ' ';
    //Leerzeichen, falls erste Stelle nicht vorhanden
    else p_short[0]=(int_short/100)+'0';
    
    p_short[1]=((int_short/10)%10)+'0';
    p_short[2]=(int_short%10)+'0';
}

void draw_update(){
    //Zeitwerte auf dem Display updaten
    convert();
    switch(mode){
        case 1:
            //Puls 1 in Modus 1 wurde geändert
            cursor_move(deviceid, 2, 12);
            display_write(deviceid, p_long);
            cursor_move(deviceid, 2, 15);
            //move cursor to 1st pulse
            break;
        case 2:
            if(select == 1){
                //Puls 1 in Modus 2 wurde geändert
                cursor_move(deviceid, 2, 12);
                display_write(deviceid, p_short);
                cursor_move(deviceid, 2, 15);
                //move cursor to 1st pulse
            }
            else if(select == 2){
                //Puls 2 in Modus 2 wurde geändert
                cursor_move(deviceid, 3, 12);
                display_write(deviceid, p_long);
                cursor_move(deviceid, 3, 15);
                //move cursor to 2nd pulse
            }
            break;
    }
}

void draw_mode(){
    //mode: 1 mode 1, 2 mode 2
    char m1_line1[] = "mode 1: single pulse";
    char m1_line2[] = "pulse 1:";
    char m1_line3[] = "                    ";
    
    char m2_line1[] = "mode 2: double pulse";
    char m2_line3[] = "pulse 2:";
    
    char menu_k[] = "mode   +    -   ok  ";
    //char menu_s[] = "mode   +    -  start";
    
    display_clear(deviceid);
    cursor_move(deviceid, 4, 1);
    display_write(deviceid, menu_k);
    cursor_move(deviceid, 2, 1);
    display_write(deviceid, m1_line2);
    cursor_move(deviceid, 1, 1);
    //common
    
    if(mode == 1){
        display_write(deviceid, m1_line1);
        cursor_move(deviceid, 2, 12);
        display_write(deviceid, p_long);
        cursor_move(deviceid, 3, 1);
        display_write(deviceid, m1_line3);
    }
    if(mode == 2){
        display_write(deviceid, m2_line1);
        cursor_move(deviceid, 2, 12);
        display_write(deviceid, p_short);
        cursor_move(deviceid, 3, 1);
        display_write(deviceid, m2_line3);
        cursor_move(deviceid, 3, 12);
        display_write(deviceid, p_long);
    }
    
    cursor_move(deviceid, 2, 15);
    //move cursor to 1st pulse
}

void interaction(void){
    if(cn_value[0] == 1){
        //Button gedrückt worden?
        switch(cn_value[1]){
            case pin_mode:
                if(mode == 1) mode=2;
                else mode=1;
                //aktualisieren des Modus
                display_clear(deviceid);
                display_clear(deviceid);
                draw_mode();
                //neuen Modus anzeigen
                break;
            case pin_plus:
                if(mode == 1 || (mode == 2 && select == 2)){
                    //Modus 1 oder Modus 2 langer Puls
                    int_long += 20;
                    //Erhöhen des Wertes
                    if(int_long > long_max) int_long = long_max;
                    //obere Wertegrenze
                }
                else{
                    //Modus 2 kurzer Puls
                    int_short += 10;
                    //Erhöhen des Wertes
                    if(int_short > short_max) int_short = short_max;
                    //obere Wertegrenze
                }
                draw_update();
                break;
            case pin_minus:
                if(mode == 1 || (mode == 2 && select == 2)){
                    //Modus 1 oder Modus 2 langer Puls
                    int_long -= 20;
                    //Senken des Wertes
                    if(int_long < long_min) int_long = long_min;
                    //untere Wertegrenze
                }
                else{
                    //Modus 2 kurzer Puls
                    int_short -= 10;
                    //Senken des Wertes
                    if(int_short < short_min) int_short = short_min;
                    //untere Wertegrenze
                }
                draw_update();
                break;
            case pin_k:
                //kurzer Druck zum umschalten des einzustellenden Wertes in Modus 2
                //langer Druck zum starten!
                wait_ms(250);
                if(d_read(pin_k)){
                    //Pin nach 250ms immer noch gedrückt?
                    action();
                }
                else{
                    if(mode == 2){
                        if(select == 1){
                            select=2;
                            cursor_move(deviceid, 3, 15);
                            //move cursor to 2nd pulse
                        }
                        else{
                            select=1;
                            cursor_move(deviceid, 2, 15);
                            //move cursor to 1st pulse
                        }
                    }
                }
                break;
        }
        cn_value[0] = 0;
    }
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
    pin_cfg(pin_out, 'd', 'o');
    d_write(pin_out, 0);
    //Ausgang
    cn_cfg(pin_mode, 1);
    cn_cfg(pin_plus, 1);
    cn_cfg(pin_minus, 1);
    cn_cfg(pin_k, 1);
    //CNs für Buttons
    //setup
    
    draw_mode();
    //anzeigen von Mode 1 auf Display
    
    while (1) {
        interaction();       
    }
}