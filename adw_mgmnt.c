#include "adw_mgmnt.h"
#include <p33FJ128GP802.h>
//Prozessorspezifische Headerdatei

//1 ADW
//12 Bit Modus: Sampling auf max. 1 Kanal
//10 Bit Modus: Sampling auf max. 4 Kanälen gleichzeitig
//Bei Benutzen der Funktionen nicht vergessen, die Portpins via pin_cfg als analoge Eingänge zu schalten!!!

void adw12_cfg(int eingangspin, char mode){
    AD1CON1bits.ADON = 0;       
    //AD-Wandler inaktiv: Kontrollbits nur bei deaktiviertem ADW schreiben!
    AD1CON1bits.AD12B = 1;
    //12bit Auflösung
    AD1CON2bits.CHPS = 0x0;
    //Festlegen der SH Kanäle
    //Bei 12 Bit nur CH0 möglich    
    AD1CHS0bits.CH0NA = 0;
    //Festlegen der negativen ADC Verstärker Spannung für CH0
    //0: neg. Verstärkereingang auf die interne Quelle legen 
    switch(eingangspin){
        //Festlegen, welcher Eingang auf den ADC geführt werden soll
        //AN0: 00000, AN1: 00001, AN2: 00010, AN3: 00011, AN4:00100, AN5: 00101
        //AN9: 01001, AN10: 01010, AN11: 01011, AN12: 01100
        case 2:     AD1CHS0bits.CH0SA = 0b00000;
                    //AN0
                    break;
        case 3:     AD1CHS0bits.CH0SA = 0b00001;
                    //AN1
                    break;
        case 4:     AD1CHS0bits.CH0SA = 0b00010;
                    //AN2
                    break;
        case 5:     AD1CHS0bits.CH0SA = 0b00011;
                    //AN3
                    break;
        case 6:     AD1CHS0bits.CH0SA = 0b00100;
                    //AN4
                    break;
        case 7:     AD1CHS0bits.CH0SA = 0b00101;
                    //AN5
                    break;
        case 23:     AD1CHS0bits.CH0SA = 0b01100;
                    //AN12
                    break;
        case 24:     AD1CHS0bits.CH0SA = 0b01011;
                    //AN11
                    break;
        case 25:     AD1CHS0bits.CH0SA = 0b01010;
                    //AN10
                    break;
        case 26:     AD1CHS0bits.CH0SA = 0b01001;
                    //AN9
                    break;
    }

    AD1CON3bits.ADRC = 1;
    //Taktquelle für die Abtastung
    //1: internal (Tad=Tadrc=250ns) 
    //0: external (Tad=Tcy(ADCS+1) mit AD1CON3.ADCS als Teiler des Systemtaktes
    if(mode == 'm') AD1CON1bits.ASAM = 0;
    //0: manuelle Abtastung
    else if(mode == 'a'){
        AD1CON1bits.ASAM = 1;
        //1: automatische Abtastung
        IPC3bits.AD1IP = 3;       
        // IRQ priority 3
        IFS0bits.AD1IF = 0;       
        // clear ADC Interrupt Flag
        IEC0bits.AD1IE = 1;       
        // enable the ADC Interrupt
    }
    AD1CON1bits.SSRC = 0b111;   
    //automatisches Triggern
    AD1CON1bits.FORM = 0b00;    
    //Einstellung des Ausgabeformats
    //00: unsignend int
    //10: signed int
    
    AD1CON1bits.ADON = 1;       
    // AD-Wandler aktiv
}

void adw10_cfg(int eingangspin_ch0, int eingangspin_ch1, char mode){
    AD1CON1bits.ADON = 0;       
    //AD-Wandler inaktiv: Kontrollbits nur bei deaktiviertem ADW schreiben!
    AD1CON1bits.AD12B = 0;
    //10bit Auflösung
    AD1CON2bits.CHPS = 0x2;
    //Festlegen der SH Kanäle
    //Bei 10 Bit: 00 CH0, 01 CH0-CH1, 1x CH0-CH3
    AD1CHS0bits.CH0NA = 0;
    //Festlegen der negativen ADC Verstärker Spannung für CH0
    //0: neg. Verstärkereingang auf die interne Quelle legen 
    AD1CHS123bits.CH123NA = 0;
    //Festlegen der negativen ADC Verstärker Spannung für CH1-3
    //0: neg. Verstärkereingänge auf die interne Quelle legen
    switch(eingangspin_ch0){
        //Festlegen, welcher Eingang auf den ADC Kanal 0 geführt werden soll
        //Register AD1CHS0, Bits CH0SA
        //AN0: 00000, AN1: 00001, AN2: 00010, AN3: 00011, AN4:00100, AN5: 00101
        //AN9: 01001, AN10: 01010, AN11: 01011, AN12: 01100
        case 2:     AD1CHS0bits.CH0SA = 0b00000;
                    //AN0
                    break;
        case 3:     AD1CHS0bits.CH0SA = 0b00001;
                    //AN1
                    break;
        case 4:     AD1CHS0bits.CH0SA = 0b00010;
                    //AN2
                    break;
        case 5:     AD1CHS0bits.CH0SA = 0b00011;
                    //AN3
                    break;
        case 6:     AD1CHS0bits.CH0SA = 0b00100;
                    //AN4
                    break;
        case 7:     AD1CHS0bits.CH0SA = 0b00101;
                    //AN5
                    break;
        case 23:     AD1CHS0bits.CH0SA = 0b01100;
                    //AN12
                    break;
        case 24:     AD1CHS0bits.CH0SA = 0b01011;
                    //AN11
                    break;
        case 25:     AD1CHS0bits.CH0SA = 0b01010;
                    //AN10
                    break;
        case 26:     AD1CHS0bits.CH0SA = 0b01001;
                    //AN9
                    break;
    }
    if(eingangspin_ch0 == 2 || eingangspin_ch0 == 3 || eingangspin_ch0 == 4) eingangspin_ch1 = 5;
    if(eingangspin_ch0 == 5 || eingangspin_ch0 == 6 || eingangspin_ch0 == 7) eingangspin_ch1 = 2;
    //Kanäle 1-3 können entweder auf pin 2-4 (AN0-2) oder pin 5-7 (AN3-5) gelegt werden.
    //Falls Kanal 0 auf pin 2,3 oder 4 gelegt wurde dürfen Kanäle 1-3 nur auf pin 5-7 (AN3-5) gelegt werden!
    //Falls Kanal 0 auf pin 5,6 oder 7 gelegt wurde dürfen Kanäle 1-3 nur auf pin 2-4 (AN0-2) gelegt werden!
    switch(eingangspin_ch1){
        //Festlegen, welche Eingänge auf die ADC Kanäle 1-3 geführt werden sollen
        //Register AD1CHS123, Bits CH123SA
        //AN0-AN2: 0
        //AN3-AN5: 1 
        case 2:     AD1CHS123bits.CH123SA = 0;
                    //AN0-AN2
                    break;
        case 5:     AD1CHS123bits.CH123SA = 1;
                    //AN3-AN5
                    break;
    }        

    AD1CON3bits.ADRC = 1;
    //Taktquelle für die Abtastung
    //1: internal (Tad=Tadrc=250ns) 
    //0: external (Tad=Tcy(ADCS+1) mit AD1CON3.ADCS als Teiler des Systemtaktes
    AD1CON1bits.SIMSAM = 0;
    //1: simultanes Sampling, 0: sequentielles Sampling
    if(mode == 'm') AD1CON1bits.ASAM = 0;
    //0: manuelle Abtastung
    else if(mode == 'a'){
        AD1CON1bits.ASAM = 1;
        //1: automatische Abtastung
        IPC3bits.AD1IP = 3;       
        // IRQ priority 3
        IFS0bits.AD1IF = 0;       
        // clear ADC Interrupt Flag
        IEC0bits.AD1IE = 1;       
        // enable the ADC Interrupt
    }
    AD1CON1bits.SSRC = 0b111;   
    //automatisches Konvertieren
    AD1CON1bits.FORM = 0b00;    
    //Einstellung des Ausgabeformats
    //00: unsignend int
    //10: signed int

    AD1CON1bits.ADON = 1;       
    // AD-Wandler aktiv
}