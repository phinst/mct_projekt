#include "read_write.h"
#include "p33FJ128GP802.h"
//Prozessorspezifische Headerdatei
#include "adw_mgmnt.h"
//custom headers

unsigned int adw_read1 = 0;

void d_write(int pinnummer, int value){
    value = value & 0x0001;
    //Nur letztes Bit interessant, bei Invertierung werden alle invertiert
    switch(pinnummer){
        //pin 1:    /reset (/mclr)
        case 2:     PORTAbits.RA0=value;
                    break;
        case 3:     PORTAbits.RA1=value;
                    break;
        case 4:     PORTBbits.RB0=value;
                    break;
        case 5:     PORTBbits.RB1=value;
                    break;
        case 6:     PORTBbits.RB2=value;
                    break;
        case 7:     PORTBbits.RB3=value;
                    break;
        //pin 8:    gnd (vss)
        case 9:     PORTAbits.RA2=value;
                    break;
        case 10:    PORTAbits.RA3=value;
                    break;
        case 11:    PORTBbits.RB4=value;
                    break;
        case 12:    PORTAbits.RA4=value;
                    break;
        //pin 13:   vdd (3.3V)
        case 14:    PORTBbits.RB5=value;
                    break;
        case 15:    PORTBbits.RB6=value;
                    break;
        case 16:    PORTBbits.RB7=value;
                    break;
        case 17:    PORTBbits.RB8=value;
                    break;
        case 18:    PORTBbits.RB9=value;
                    break;
        //pin 19:   gnd (vss2)
        //pin 20:   vcap
        case 21:    PORTBbits.RB10=value;
                    break;
        case 22:    PORTBbits.RB11=value;
                    break;
        case 23:    PORTBbits.RB12=value;
                    break;
        case 24:    PORTBbits.RB13=value;
                    break;
        case 25:    PORTBbits.RB14=value;
                    break;
        case 26:    PORTBbits.RB15=value;
                    break;
        //pin 27:   avss
        //pin 28:   avdd            
    }
}

int d_read(int pinnummer){
    int value = 0;
    switch(pinnummer){
        //pin 1:    /reset (/mclr)
        case 2:     value = PORTAbits.RA0;
                    break;
        case 3:     value = PORTAbits.RA1;
                    break;
        case 4:     value = PORTBbits.RB0;
                    break;
        case 5:     value = PORTBbits.RB1;
                    break;
        case 6:     value = PORTBbits.RB2;
                    break;
        case 7:     value = PORTBbits.RB3;
                    break;
        //pin 8:    gnd (vss)
        case 9:     value = PORTAbits.RA2;
                    break;
        case 10:    value = PORTAbits.RA3;
                    break;
        case 11:    value = PORTBbits.RB4;
                    break;
        case 12:    value = PORTAbits.RA4;
                    break;
        //pin 13:   vdd (3.3V)
        case 14:    value = PORTBbits.RB5;
                    break;
        case 15:    value = PORTBbits.RB6;
                    break;
        case 16:    value = PORTBbits.RB7;
                    break;
        case 17:    value = PORTBbits.RB8;
                    break;
        case 18:    value = PORTBbits.RB9;
                    break;
        //pin 19:   gnd (vss2)
        //pin 20:   vcap
        case 21:    value = PORTBbits.RB10;
                    break;
        case 22:    value = PORTBbits.RB11;
                    break;
        case 23:    value = PORTBbits.RB12;
                    break;
        case 24:    value = PORTBbits.RB13;
                    break;
        case 25:    value = PORTBbits.RB14;
                    break;
        case 26:    value = PORTBbits.RB15;
                    break;
        //pin 27:   avss
        //pin 28:   avdd            
    }
    return value;
}

unsigned int analog_manual_read(){
    AD1CON1bits.SAMP = 1;
    //starten des Abtastvorgangs
    while(AD1CON1bits.DONE == 0);
    //warten bis Abtastung und Konvertierung fertig
    return ADC1BUF0;
}

void __attribute__((__interrupt__, no_auto_psv)) _ADC1Interrupt(void){
    adw_read1 = ADC1BUF0; 
    //Lesen des Ergebnisses und global speichern
    IFS0bits.AD1IF = 0; 
    // Clear AD1 Interrupt Flag
}

void a_write(int pinnummer){
    //dac?
}
