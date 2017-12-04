#include "timer_mgmnt.h"
#include <p33FJ128GP802.h>
//Prozessorspezifische Headerdatei

//Der dsPIC hat 5 16Bit Timer, mit insgesamt drei Modi:
//Timer 1: Typ A
//Timer 2, 4: Typ B
//Timer 3, 5: Typ C
//Typ B & C lassen sich zu einem 32Bit Timer zusammenschalten.
//Jeder Timer hat ein Count Register TMRx, ein Period Register PRx und ein Comtrol Register TxCON
//Nützlich weiterhin: Interrupt Enable Control Bit TxIE, Interrupt Status Flag Bit TnIF, Interrupt Priority Control Bits TxIP<2:0>
//Timer 2 & 3 können als Timebase für OC Module genutzt werden!!!


void set_timer1(int PR1_value){
    T1CONbits.TON = 0;      
    //Disable Timer
    T1CONbits.TCS = 0; 
    //Select internal instruction cycle clock
    T1CONbits.TGATE = 0; 
    //Disable Gated Timer mode
    T1CONbits.TCKPS = 0b01; 
    //Select 1:8 Prescaler
    TMR1 = 0x00; 
    //Clear timer register
    PR1 = PR1_value; 
    //Zählt PR1+1 Schritte:
    //Load the period value (249 for 1kHz or 124 for 2kHz)
    IPC0bits.T1IP = 0x01;
    //Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF = 0; 
    //Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1; 
    //Enable Timer1 interrupt
    T1CONbits.TON = 1; 
    //Start Timer
}

void set_timer2(int upper16, int lower16, int width){
    //width 16: Timer 2 solo, 32: Timer 2+3 im 32 Bit Mode
    //width 32: lower Bits für Timer 2, upper Bits für Timer 3
    //width 16: nur lower Bits relevant!
    T2CONbits.TON = 0;      
    //Disable Timer 2
    if(width == 32){
        T3CONbits.TON = 0;      
        //Disable Timer 3
        T2CONbits.T32=1;
        //setzen des 32Bit Bits
        PR3=upper16;
        //upper Bits in das Timer 3 Periodenregister
        TMR3=0x00;
        //leeren des Zählregisters von Timer 3
    }
    else if(width == 16){
        T2CONbits.T32=0;
        //rücksetzen des 32Bit Bits
    }
    TMR2 = 0x00; 
    //Clear Timer 2 register
    PR2 = lower16; 
    //Load the period value
    T2CONbits.TCS = 0;      
    //Select internal instruction cycle clock
    T2CONbits.TGATE = 0; 
    //Disable Gated Timer mode
    T2CONbits.TCKPS = 0b01; 
    //Select 1:8 Prescaler
    IPC1bits.T2IP = 0x01; 
    //Set Timer 2 Interrupt Priority Level
    IFS0bits.T2IF = 0; 
    //Clear Timer2 Interrupt Flag
    IEC0bits.T2IE = 1; 
    //Enable Timer2 interrupt
    T2CONbits.TON = 1; 
    //Start Timer
}

void set_timer4(int upper16, int lower16, int width){
    //width 16: Timer 4 solo, 32: Timer 4+5 im 32 Bit Mode
    //width 32: lower Bits für Timer 4, upper Bits für Timer 5
    //width 16: nur lower Bits relevant!
    T4CONbits.TON = 0;      
    //Disable Timer 4
    if(width == 32){
        T5CONbits.TON = 0;      
        //Disable Timer 5
        T4CONbits.T32=1;
        //setzen des 32Bit Bits
        PR5=upper16;
        //upper Bits in das Timer 5 Periodenregister
        TMR5=0x00;
        //leeren des Zählregisters von Timer 5
    }
    else if(width == 16){
        T4CONbits.T32=0;
        //rücksetzen des 32Bit Bits
    }
    TMR4 = 0x00; 
    //Clear Timer 2 register
    PR4 = lower16; 
    //Load the period value
    T4CONbits.TCS = 0;      
    //Select internal instruction cycle clock
    T4CONbits.TGATE = 0; 
    //Disable Gated Timer mode
    T4CONbits.TCKPS = 0b01; 
    //Select 1:8 Prescaler
    IPC6bits.T4IP = 0x01; 
    //Set Timer 4 Interrupt Priority Level
    IFS1bits.T4IF = 0; 
    //Clear Timer 4 Interrupt Flag
    IEC1bits.T4IE = 1; 
    //Enable Timer 4 interrupt
    T4CONbits.TON = 1; 
    //Start Timer
}

void set_timer3(int PR_value){
    T3CONbits.TON = 0;      
    //Disable Timer
    T3CONbits.TCS = 0; 
    //Select internal instruction cycle clock
    T3CONbits.TGATE = 0; 
    //Disable Gated Timer mode
    T3CONbits.TCKPS = 0b01; 
    //Select 1:8 Prescaler
    TMR3 = 0x00; 
    //Clear Timer 3 register
    PR3 = PR_value; 
    //Load the period value
    T3CONbits.TCS = 0;      
    //Select internal instruction cycle clock
    T3CONbits.TGATE = 0; 
    //Disable Gated Timer mode
    T3CONbits.TCKPS = 0b01; 
    //Select 1:8 Prescaler
    IPC2bits.T3IP = 0x01; 
    //Set Timer 4 Interrupt Priority Level
    IFS0bits.T3IF = 0; 
    //Clear Timer 3 Interrupt Flag
    IEC0bits.T3IE = 1; 
    //Enable Timer 3 interrupt
    T3CONbits.TON = 1; 
    //Start Timer
}

void set_timer5(int PR_value){
    T5CONbits.TON = 0;      
    //Disable Timer
    T5CONbits.TCS = 0; 
    //Select internal instruction cycle clock
    T5CONbits.TGATE = 0; 
    //Disable Gated Timer mode
    T5CONbits.TCKPS = 0b10; 
    //Select Prescaler
    TMR5 = 0x00; 
    //Clear Timer 5 register
    PR5 = PR_value; 
    //Load the period value
    T5CONbits.TCS = 0;      
    //Select internal instruction cycle clock
    T5CONbits.TGATE = 0; 
    //Disable Gated Timer mode
    IPC7bits.T5IP = 0x01; 
    //Set Timer 5 Interrupt Priority Level
    IFS1bits.T5IF = 0; 
    //Clear Timer 5 Interrupt Flag
    IEC1bits.T5IE = 1; 
    //Enable Timer 5 interrupt
    T5CONbits.TON = 1; 
    //Start Timer
}

