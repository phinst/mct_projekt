#include "pwm_mgmnt.h"
#include <p33FJ128GP802.h>
//Prozessorspezifische Headerdatei
#include "timer_mgmnt.h"
#include "port_mgmnt.h"
//custom headers

//Der dsPIC hat insgesamt 4 PWM Module (weil 4 OC Module)
//OC Module können nur mit Timer 2 & 3 als Zeitbasis verwendet werden.
//pwm1 & 2 nutzen Timer 2 als Zeitbasis, pwm 3 & 4 nutzen Timer 3

void pwm1_init(int pinnummer, int freq){
    OC1CONbits.OCM = 0b000;     
    //Disable Output Compare Module
    OC1R = 0;                 
    //Duty cycle first PWM pulse
    OC1RS = 0;                
    //Duty cycle of next PWM pulses
    OC1CONbits.OCTSEL = 0;      
    //select output compare base time
    //0: timer 2, 1: timer 3
    OC1CONbits.OCM = 0b110;     
    //Select output compare mode
    //6: pwm w/o fault detection
    if (T2CONbits.TON == 0) set_timer2(0, freq, 16);
    //Timer falls noch nicht geschehen aktivieren
    pin_cfg(pinnummer, 'd', 'o');
    //gewünschter Pin als digitaler Ausgang
    rpin_cfg(pinnummer, 18, 'o');
    //Schalten des OC Outputs auf den gewünschten Pin
}

void pwm2_init(int pinnummer, int freq){
    OC2CONbits.OCM = 0b000;     
    //Disable Output Compare Module
    OC2R = freq/2;                 
    //Duty cycle first PWM pulse
    OC2RS = 0x0;                
    //Duty cycle of next PWM pulses
    OC2CONbits.OCTSEL = 0;      
    //select output compare base time
    //0: timer 2, 1: timer 3
    OC2CONbits.OCM = 0b110;     
    //Select output compare mode
    //6: pwm w/o fault detection
    if (T2CONbits.TON == 0) set_timer2(0, freq, 16);
    //Timer falls noch nicht geschehen aktivieren
    pin_cfg(pinnummer, 'd', 'o');
    //gewünschter Pin als digitaler Ausgang
    rpin_cfg(pinnummer, 19, 'o');
    //Schalten des OC Outputs auf den gewünschten Pin
}

void pwm3_init(int pinnummer, int freq){
    OC3CONbits.OCM = 0b000;     
    //Disable Output Compare Module
    OC3R = freq/2;                 
    //Duty cycle first PWM pulse
    OC3RS = 0x0;                
    //Duty cycle of next PWM pulses
    OC3CONbits.OCTSEL = 0;      
    //select output compare base time
    //0: timer 2, 1: timer 3
    OC3CONbits.OCM = 0b110;     
    //Select output compare mode
    //6: pwm w/o fault detection
    if (T3CONbits.TON == 0) set_timer3(freq);
    //Timer falls noch nicht geschehen aktivieren
    pin_cfg(pinnummer, 'd', 'o');
    //gewünschter Pin als digitaler Ausgang
    rpin_cfg(pinnummer, 20, 'o');
    //Schalten des OC Outputs auf den gewünschten Pin
}

void pwm4_init(int pinnummer, int freq){
    OC4CONbits.OCM = 0b000;     
    //Disable Output Compare Module
    OC4R = freq/2;                 
    //Duty cycle first PWM pulse
    OC4RS = 0x0;                
    //Duty cycle of next PWM pulses
    OC4CONbits.OCTSEL = 0;      
    //select output compare base time
    //0: timer 2, 1: timer 3
    OC4CONbits.OCM = 0b110;     
    //Select output compare mode
    //6: pwm w/o fault detection
    if (T3CONbits.TON == 0) set_timer3(freq);
    //Timer falls noch nicht geschehen aktivieren
    pin_cfg(pinnummer, 'd', 'o');
    //gewünschter Pin als digitaler Ausgang
    rpin_cfg(pinnummer, 21, 'o');
    //Schalten des OC Outputs auf den gewünschten Pin
}

void pwm1_duty(int dutycycle){
    //duty cycle in %
    OC1RS = (PR2 * dutycycle)/100;                
    //duty cycle für nächste PWM Pulse
}

void pwm2_duty(int dutycycle){
    //duty cycle in %
    OC2RS = (PR2 * dutycycle)/100;                
    //duty cycle für nächste PWM Pulse
}

void pwm3_duty(int dutycycle){
    //duty cycle in %
    OC3RS = (PR3 * dutycycle)/100;                
    //duty cycle für nächste PWM Pulse
}

void pwm4_duty(int dutycycle){
    //duty cycle in %
    OC3RS = (PR3 * dutycycle)/100;       
    //duty cycle für nächste PWM Pulse
}