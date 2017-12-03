#include "clk_mgmnt.h"
#include <p33FJ128GP802.h>
//Prozessorspezifische Headerdatei

void clk_reset(){
    //OSCCON HIGH, Primary Source + XT
    __builtin_write_OSCCONH(0x02);
    //OSCCON LOW
    __builtin_write_OSCCONL(0x1);
}

void clk_boost(int freq){
    switch(freq){
        case 80:
            //M = 38
            PLLFBDbits.PLLDIV  = 38; 
            //Postscaler = 0
            CLKDIVbits.PLLPOST = 0;
            //Prescaler = 0
            CLKDIVbits.PLLPRE  = 0; 
            //OSCCON HIGH, Primary Source + XT + PLL
            __builtin_write_OSCCONH(0x03);
            //OSCCON LOW
            __builtin_write_OSCCONL(0x1);
            break;
    }
    //wait for PLL
    while(OSCCONbits.COSC != 0b011);
    //wait for lock
    while(OSCCONbits.LOCK != 1);
}