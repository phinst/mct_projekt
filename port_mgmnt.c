#include "port_mgmnt.h"
#include "p33FJ128GP802.h"
//Prozessorspezifische Headerdatei
#include "read_write.h"
#include "timer_mgmnt.h"

//nutzt Timer 5!!!

//Port A: 5 Pins (0-4), TRISA Register
//Eingang 1, Ausgang 0
//default: 1
//Port B: 16 Pins (0-15), TRISB Register
//Eingang 1, Ausgang 0
//default: 1

//Analog: AN0-5 AN9-12, AD1PCFGL Register
//digital 1, analog 0
//default 0

//Aus- und Eingänge von 16 Pins (RP0-15) können per Multiplexer verschiedenen Peripherienaus- und eingängen zugewiesen werden.
//Sicherhaltshalber muss während des Remappings das IOLOCK Bit des OSCCON Registers null gesetzt werden.

int cn_value[2] = {0, 0};
//CN Variable
//[0]: confirmed flag
//[1]: Pinnummer

void clr_ports() {
    //Analoge Pins digital setzten
    AD1PCFGL=0xFFFF;
    //Alle Pins als Ausgänge definieren
    TRISA=0;
    TRISB=0;
    //Ausschalten aller Pullups
    CNPU1=0;
    CNPU2=0;
}

void pin_cfg(int pinnummer, char mode, char richtung){
    //Konfiguration eines Pins als digitalen oder analogen Ein- bzw. Ausgang
    //pinnummer: # des verwendeten Pins z.B. RA0/AN0 liegt auf Pin#2
    //mode: a analog, d digital
    //richtung: i in, o out
    if (mode == 'd'){
        if (richtung == 'o'){
            //Ausgang
            switch(pinnummer){
                //pin 1:    /reset (/mclr)
                case 2:     AD1PCFGLbits.PCFG0=1;
                            //Pin digital schalten
                            TRISAbits.TRISA0=0;
                            //Pin als Ausgang schalten
                            break;
                case 3:     AD1PCFGLbits.PCFG1=1;
                            TRISAbits.TRISA1=0;
                            break;
                case 4:     AD1PCFGLbits.PCFG2=1;
                            TRISBbits.TRISB0=0;
                            break;
                case 5:     AD1PCFGLbits.PCFG3=1;
                            TRISBbits.TRISB1=0;
                            break;
                case 6:     AD1PCFGLbits.PCFG4=1;
                            TRISBbits.TRISB2=0;
                            break;
                case 7:     AD1PCFGLbits.PCFG5=1;
                            TRISBbits.TRISB3=0;
                            break;
                //pin 8:    gnd (vss)
                case 9:     TRISAbits.TRISA2=0;
                            break;
                case 10:    TRISAbits.TRISA3=0;
                            break;
                case 11:    TRISBbits.TRISB4=0;
                            break;
                case 12:    TRISAbits.TRISA4=0;
                            break;
                //pin 13:   vdd (3.3V)
                case 14:    TRISBbits.TRISB5=0;
                            break;
                case 15:    TRISBbits.TRISB6=0;
                            break;
                case 16:    TRISBbits.TRISB7=0;
                            break;
                case 17:    TRISBbits.TRISB8=0;
                            break;
                case 18:    TRISBbits.TRISB9=0;
                            break;
                //pin 19:   gnd (vss2)
                //pin 20:   vcap
                case 21:    TRISBbits.TRISB10=0;
                            break;
                case 22:    TRISBbits.TRISB11=0;
                            break;
                case 23:    AD1PCFGLbits.PCFG12=1;
                            TRISBbits.TRISB12=0;
                            break;
                case 24:    AD1PCFGLbits.PCFG11=1;
                            TRISBbits.TRISB13=0;
                            break;
                case 25:    AD1PCFGLbits.PCFG10=1;
                            TRISBbits.TRISB14=0;
                            break;
                case 26:    AD1PCFGLbits.PCFG9=1;
                            TRISBbits.TRISB15=0;
                            break;
                //pin 27:   avss
                //pin 28:   avdd            
            }
        }
        else if (richtung == 'i'){
            //Eingang
            switch(pinnummer){
                //pin 1:    /reset (/mclr)
                case 2:     AD1PCFGLbits.PCFG0=1;
                            //Pin digital schalten
                            TRISAbits.TRISA0=1;
                            //Pin als Eingang schalten
                            break;
                case 3:     AD1PCFGLbits.PCFG1=1;
                            TRISAbits.TRISA1=1;
                            break;
                case 4:     AD1PCFGLbits.PCFG2=1;
                            TRISBbits.TRISB0=1;
                            break;
                case 5:     AD1PCFGLbits.PCFG3=1;
                            TRISBbits.TRISB1=1;
                            break;
                case 6:     AD1PCFGLbits.PCFG4=1;
                            TRISBbits.TRISB2=1;
                            break;
                case 7:     AD1PCFGLbits.PCFG5=1;
                            TRISBbits.TRISB3=1;
                            break;
                //pin 8:    gnd (vss)
                case 9:     TRISAbits.TRISA2=1;
                            break;
                case 10:    TRISAbits.TRISA3=1;
                            break;
                case 11:    TRISBbits.TRISB4=1;
                            break;
                case 12:    TRISAbits.TRISA4=1;
                            break;
                //pin 13:   vdd (3.3V)
                case 14:    TRISBbits.TRISB5=1;
                            break;
                case 15:    TRISBbits.TRISB6=1;
                            break;
                case 16:    TRISBbits.TRISB7=1;
                            break;
                case 17:    TRISBbits.TRISB8=1;
                            break;
                case 18:    TRISBbits.TRISB9=1;
                            break;
                //pin 19:   gnd (vss2)
                //pin 20:   vcap
                case 21:    TRISBbits.TRISB10=1;
                            break;
                case 22:    TRISBbits.TRISB11=1;
                            break;
                case 23:    AD1PCFGLbits.PCFG12=1;
                            TRISBbits.TRISB12=1;
                            break;
                case 24:    AD1PCFGLbits.PCFG11=1;
                            TRISBbits.TRISB13=1;
                            break;
                case 25:    AD1PCFGLbits.PCFG10=1;
                            TRISBbits.TRISB14=1;
                            break;
                case 26:    AD1PCFGLbits.PCFG9=1;
                            TRISBbits.TRISB15=1;
                            break;
                //pin 27:   avss
                //pin 28:   avdd            
            }
        }
    }
    else if (mode == 'a'){
        if (richtung == 'i'){
        //Konfiguration eines Pins als analogen Eingang
            switch(pinnummer){
                case 2:     AD1PCFGLbits.PCFG0=0;
                            //Pin analog schalten
                            TRISAbits.TRISA0=1;
                            //Pin als Eingang schalten
                            break;
                case 3:     AD1PCFGLbits.PCFG1=0;
                            TRISAbits.TRISA1=1;
                            break;
                case 4:     AD1PCFGLbits.PCFG2=0;
                            TRISBbits.TRISB0=1;
                            break;
                case 5:     AD1PCFGLbits.PCFG3=0;
                            TRISBbits.TRISB1=1;
                            break;
                case 6:     AD1PCFGLbits.PCFG4=0;
                            TRISBbits.TRISB2=1;
                            break;
                case 7:     AD1PCFGLbits.PCFG5=0;
                            TRISBbits.TRISB3=1;
                            break;
                case 23:    AD1PCFGLbits.PCFG12=0;
                            TRISBbits.TRISB12=1;
                            break;
                case 24:    AD1PCFGLbits.PCFG11=0;
                            TRISBbits.TRISB13=1;
                            break;
                case 25:    AD1PCFGLbits.PCFG10=0;
                            TRISBbits.TRISB14=1;
                            break;
                case 26:    AD1PCFGLbits.PCFG9=0;
                            TRISBbits.TRISB15=1;
                            break;
            }
        }
        else if (richtung == 'o'){
        //noch nicht konfiguriert, haben wir überhaupt einen dac?
        }
    }
}

void pullup_cfg(int pinnummer, int state ) {
    //Zu- bzw. Abschalten eines Pullups an einem bestimmten Pin
    //state: 0 disabled, 1 enabled
    if (state == 1) {
        switch(pinnummer){
            //pin 1:    /reset (/mclr)
            case 2:     CNPU1bits.CN2PUE=1;
                        break;
            case 3:     CNPU1bits.CN3PUE=1;
                        break;
            case 4:     CNPU1bits.CN4PUE=1;
                        break;
            case 5:     CNPU1bits.CN5PUE=1;
                        break;
            case 6:     CNPU1bits.CN6PUE=1;
                        break;
            case 7:     CNPU1bits.CN7PUE=1;
                        break;
            //pin 8:    gnd (vss)
            case 9:     CNPU2bits.CN30PUE=1;
                        break;
            case 10:    CNPU2bits.CN29PUE=1;
                        break;
            case 11:    CNPU1bits.CN1PUE=1;
                        break;
            case 12:    CNPU1bits.CN0PUE=1;
                        break;
            //pin 13:   vdd (3.3V)
            case 14:    CNPU2bits.CN27PUE=1;
                        break;
            case 15:    CNPU2bits.CN24PUE=1;
                        break;
            case 16:    CNPU2bits.CN23PUE=1;
                        break;
            case 17:    CNPU2bits.CN22PUE=1;
                        break;
            case 18:    CNPU2bits.CN21PUE=1;
                        break;
            //pin 19:   gnd (vss2)
            //pin 20:   vcap
            case 21:    CNPU2bits.CN16PUE=1;
                        break;
            case 22:    CNPU1bits.CN15PUE=1;
                        break;
            case 23:    CNPU1bits.CN14PUE=1;
                        break;
            case 24:    CNPU1bits.CN13PUE=1;
                        break;
            case 25:    CNPU1bits.CN12PUE=1;
                        break;
            case 26:    CNPU1bits.CN11PUE=1;
                        break;
            //pin 27:   avss
            //pin 28:   avdd            
        }
    }
    else if (state == 0) {
        switch(pinnummer){
            //pin 1:    /reset (/mclr)
            case 2:     CNPU1bits.CN2PUE=0;
                        break;
            case 3:     CNPU1bits.CN3PUE=0;
                        break;
            case 4:     CNPU1bits.CN4PUE=0;
                        break;
            case 5:     CNPU1bits.CN5PUE=0;
                        break;
            case 6:     CNPU1bits.CN6PUE=0;
                        break;
            case 7:     CNPU1bits.CN7PUE=0;
                        break;
            //pin 8:    gnd (vss)
            case 9:     CNPU2bits.CN30PUE=0;
                        break;
            case 10:    CNPU2bits.CN29PUE=0;
                        break;
            case 11:    CNPU1bits.CN1PUE=0;
                        break;
            case 12:    CNPU1bits.CN0PUE=0;
                        break;
            //pin 13:   vdd (3.3V)
            case 14:    CNPU2bits.CN27PUE=0;
                        break;
            case 15:    CNPU2bits.CN24PUE=0;
                        break;
            case 16:    CNPU2bits.CN23PUE=0;
                        break;
            case 17:    CNPU2bits.CN22PUE=0;
                        break;
            case 18:    CNPU2bits.CN21PUE=0;
                        break;
            //pin 19:   gnd (vss2)
            //pin 20:   vcap
            case 21:    CNPU2bits.CN16PUE=0;
                        break;
            case 22:    CNPU1bits.CN15PUE=0;
                        break;
            case 23:    CNPU1bits.CN14PUE=0;
                        break;
            case 24:    CNPU1bits.CN13PUE=0;
                        break;
            case 25:    CNPU1bits.CN12PUE=0;
                        break;
            case 26:    CNPU1bits.CN11PUE=0;
                        break;
            //pin 27:   avss
            //pin 28:   avdd            
        }
    }
}

void rpin_cfg(int pinnummer, int mode, char richtung){
    __builtin_write_OSCCONL(OSCCONL & 0xBF);
    //HW Lock Bit null setzen um das Schreiben auf Remapping Register zu erlauben
    if(richtung == 'i'){
        //Remapping eines Eingangspins
        switch(pinnummer){
            //Pinnummer nicht gleich des Index von RPn
            case 4:     pinnummer=0;
                        break;
            case 5:     pinnummer=1;
                        break;
            case 6:     pinnummer=2;
                        break;
            case 7:     pinnummer=3;
                        break;
            case 11:    pinnummer=4;
                        break;
            case 14:    pinnummer=5;
                        break;
            case 15:    pinnummer=6;
                        break;
            case 16:    pinnummer=7;
                        break;
            case 17:    pinnummer=8;
                        break;
            case 18:    pinnummer=9;
                        break;
            case 21:    pinnummer=10;
                        break;
            case 22:    pinnummer=11;
                        break;
            case 23:    pinnummer=12;
                        break;
            case 24:    pinnummer=13;
                        break;
            case 25:    pinnummer=14;
                        break;
            case 26:    pinnummer=15;
                        break;
        }
        switch(mode){
            //mode:
            //1: INT1, 2: INT2
            //3: T2CK, 4: T3CK, 5: T4CK, 6: T5CK
            //7: IC1, 8: IC2, 9: IC7, 10: IC8
            //11: OCFA
            //12: U1RX, 13: !U1CTS, 14: U2RX, 15: !U2CTS
            //16: SDI1, 17: SCK1, 18: !SS1, 19: SDI2, 20: SCK2, 21: !SS2
            //22: CSDI, 23: CSCK, 24: COFS, 25: CIRX
            case 1:     RPINR0bits.INT1R = pinnummer;
                        break;
            case 2:     RPINR1bits.INT2R = pinnummer;
                        break;
            case 3:     RPINR3bits.T2CKR = pinnummer;
                        break;
            case 4:     RPINR3bits.T3CKR = pinnummer;
                        break;
            case 5:     RPINR4bits.T4CKR = pinnummer;
                        break;
            case 6:     RPINR4bits.T5CKR = pinnummer;
                        break;
            case 7:     RPINR7bits.IC1R = pinnummer;
                        break;
            case 8:     RPINR7bits.IC2R = pinnummer;
                        break;
            case 9:     RPINR10bits.IC7R = pinnummer;
                        break;
            case 10:    RPINR10bits.IC8R = pinnummer;
                        break;
            case 11:    RPINR11bits.OCFAR = pinnummer;
                        break;
            case 12:    RPINR18bits.U1RXR = pinnummer;
                        break;
            case 13:    RPINR18bits.U1CTSR = pinnummer;
                        break;
            case 14:    RPINR19bits.U2RXR = pinnummer;
                        break;
            case 15:    RPINR19bits.U2CTSR = pinnummer;
                        break;
            case 16:    RPINR20bits.SDI1R = pinnummer;
                        break;
            case 17:    RPINR20bits.SCK1R = pinnummer;
                        break;
            case 18:    RPINR21bits.SS1R = pinnummer;
                        break;
            case 19:    RPINR22bits.SDI2R = pinnummer;
                        break;
            case 20:    RPINR22bits.SCK2R = pinnummer;
                        break;
            case 21:    RPINR23bits.SS2R = pinnummer;
                        break;
            case 22:    RPINR24bits.CSDIR = pinnummer;
                        break;
            case 23:    RPINR24bits.CSCKR = pinnummer;
                        break;
            case 24:    RPINR25bits.COFSR = pinnummer;
                        break;
            case 25:    RPINR26bits.C1RXR = pinnummer;
                        break;
        }    
    }
    else if(richtung == 'o'){
        //Remapping eines Ausgangspins
        //mode:
        //0(00000): NULL
        //1(00001): C1OUT, 2(00010): C2OUT
        //3(00011): U1TX, 4(00100): !U1RTS, 5(00101): U1TX, 6(00110): !U2RTS
        //7(00111): SDO1, 8(01000): SCK1, 9(01001): !SS1, 10(01010): SDO2, 11(01011): SCK2, 12(01100): !SS2
        //13(01101): CSDO, 14(01110): CSCK, 15(01111): COFS
        //16(10000): C1TX,
        //17(10010): OC1, 18(10011): OC2, 19(10100): OC3, 20(10101: OC4)
        switch(pinnummer){
            case 4:     RPOR0bits.RP0R = mode;
                        break;
            case 5:     RPOR0bits.RP1R = mode;
                        break;
            case 6:     RPOR1bits.RP2R = mode;
                        break;
            case 7:     RPOR1bits.RP3R = mode;
                        break;
            case 11:    RPOR2bits.RP4R = mode;
                        break;
            case 14:    RPOR2bits.RP5R = mode;
                        break;
            case 15:    RPOR3bits.RP6R = mode;
                        break;
            case 16:    RPOR3bits.RP7R = mode;
                        break;
            case 17:    RPOR4bits.RP8R = mode;
                        break;
            case 18:    RPOR4bits.RP9R = mode;
                        break;
            case 21:    RPOR5bits.RP10R = mode;
                        break;
            case 22:    RPOR5bits.RP11R = mode;
                        break;
            case 23:    RPOR6bits.RP12R = mode;
                        break;
            case 24:    RPOR6bits.RP13R = mode;
                        break;
            case 25:    RPOR7bits.RP14R = mode;
                        break;
            case 26:    RPOR7bits.RP15R = mode;
                        break;
        }
    }
    __builtin_write_OSCCONL(OSCCONL | 0x40);
    //HW Lock Bit 1 setzen um das Schreiben auf Remapping Register zu verbieten
}

void cn_cfg(int pinnummer, int state){
    //CN Register: Handbuch S.44 Table 4-2
    //state: 0 disable, 1 enable
    if(state == 0 || state == 1){
        switch(pinnummer){
            //pin 1:    /reset (/mclr)
            case 2:     CNEN1bits.CN2IE=state;
                        break;
            case 3:     CNEN1bits.CN3IE=state;
                        break;
            case 4:     CNEN1bits.CN4IE=state;
                        break;
            case 5:     CNEN1bits.CN5IE=state;
                        break;
            case 6:     CNEN1bits.CN6IE=state;
                        break;
            case 7:     CNEN1bits.CN7IE=state;
                        break;
            //pin 8:    gnd (vss)
            case 9:     CNEN2bits.CN30IE=state;
                        break;
            case 10:    CNEN2bits.CN29IE=state;
                        break;
            case 11:    CNEN1bits.CN1IE=state;
                        break;
            case 12:    CNEN1bits.CN0IE=state;
                        break;
            //pin 13:   vdd (3.3V)
            case 14:    CNEN2bits.CN27IE=state;
                        break;
            case 15:    CNEN2bits.CN24IE=state;
                        break;
            case 16:    CNEN2bits.CN23IE=state;
                        break;
            case 17:    CNEN2bits.CN22IE=state;
                        break;
            case 18:    CNEN2bits.CN21IE=state;
                        break;
            //pin 19:   gnd (vss2)
            //pin 20:   vcap
            case 21:    CNEN2bits.CN16IE=state;
                        break;
            case 22:    CNEN1bits.CN15IE=state;
                        break;
            case 23:    CNEN1bits.CN14IE=state;
                        break;
            case 24:    CNEN1bits.CN13IE=state;
                        break;
            case 25:    CNEN1bits.CN12IE=state;
                        break;
            case 26:    CNEN1bits.CN11IE=state;
                        break;
            //pin 27:   avss
            //pin 28:   avdd            
        }
        if(!CNEN1 && !CNEN2){
            //Falls kein CN bit gesetzt kann CN deaktiviert werden
            IEC1bits.CNIE=0;
            //CN Interrupt Enable bit IEC1.CNIE: 0 disabled, 1 enabled
            IPC4bits.CNIP=0x0;
            //CN Interrupt Priority bits IPC4.CNIP: 000 disabled, 001 priority 1, ..., 111 priority 7 (highest)IFS1bits.CNIF = 0;
        }
        else if(CNEN1 || CNEN2){
            //Falls CN genutzt werden soll muss CN aktiviert sein
            if(!IEC1bits.CNIE) IEC1bits.CNIE=1;
            //CN Interrupt Enable bit IEC1.CNIE: 0 disabled, 1 enabled
            if(!IPC4bits.CNIP) IPC4bits.CNIP=0x1;
            //CN Interrupt Priority bits IPC4.CNIP: 000 disabled, 001 priority 1, ..., 111 priority 7 (highest)IFS1bits.CNIF = 0;
            IFS1bits.CNIF = 0;
            //Interrupt Flag freigeben
        }
    }
}

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void){
    int pinnummer=0;
    if(CNEN1){
        //nur Pins interessant, an denen CN aktiviert ist
        //prüfen des ersten CN Enable Registers
        if(CNEN1 & 0x00FF){
            if(CNEN1 & 0x000F){
                if(CNEN1bits.CN0IE){
                    if(d_read(12)) pinnummer=12;
                }
                if(CNEN1bits.CN1IE){
                    if(d_read(11)) pinnummer=11;
                }
                if(CNEN1bits.CN2IE){
                    if(d_read(2)) pinnummer=2;
                }
                if(CNEN1bits.CN3IE){
                    if(d_read(3)) pinnummer=3;
                }
            }
            if(CNEN1 & 0x00F0){
                if(CNEN1bits.CN4IE){
                    if(d_read(4)) pinnummer=4;
                }
                if(CNEN1bits.CN5IE){
                    if(d_read(5)) pinnummer=5;
                }
                if(CNEN1bits.CN6IE){
                    if(d_read(6)) pinnummer=6;
                }
                if(CNEN1bits.CN7IE){
                    if(d_read(7)) pinnummer=7;
                }
            }
        }
        else{
            if(CNEN1 & 0x0F00){
                if(CNEN1bits.CN11IE){
                    if(d_read(26)) pinnummer=26;
                }
            }
            if(CNEN1 & 0xF000){
                if(CNEN1bits.CN12IE){
                    if(d_read(25)) pinnummer=25;
                }
                if(CNEN1bits.CN13IE){
                    if(d_read(24)) pinnummer=24;
                }
                if(CNEN1bits.CN14IE){
                    if(d_read(23)) pinnummer=23;
                }
                if(CNEN1bits.CN15IE){
                    if(d_read(22)) pinnummer=22;
                }
            }
        }
    }
    if(CNEN2){
        //prüfen des zweiten CN Enable Registers
        if(CNEN2 & 0x00FF){
            if(CNEN2 & 0x000F){
                if(CNEN2bits.CN16IE){
                    if(d_read(21)) pinnummer=21;
                }
            }
            if(CNEN2 & 0x00F0){
                if(CNEN2bits.CN21IE){
                    if(d_read(18)) pinnummer=18;
                }
                if(CNEN2bits.CN22IE){
                    if(d_read(17)) pinnummer=17;
                }
                if(CNEN2bits.CN23IE){
                    if(d_read(16)) pinnummer=16;
                }
            }
        }
        if(CNEN2 & 0xFF00){
            if(CNEN2 & 0x0F00){
                if(CNEN2bits.CN24IE){
                    if(d_read(15)) pinnummer=15;
                }
                if(CNEN2bits.CN27IE){
                    if(d_read(14)) pinnummer=14;
                }
            }
            if(CNEN2 & 0xF000){
                if(CNEN2bits.CN29IE){
                    if(d_read(10)) pinnummer=10;
                }
                if(CNEN2bits.CN30IE){
                    if(d_read(9)) pinnummer=9;
                }
            }
        }
    }
    //herausfinden, welcher Pin die CN ausgelöst hat
    cn_value[1]=pinnummer;
    //globales speichern der Pinnummer
    if(!T5CONbits.TON) set_timer5(60000);
    //Timer mit bestimmtem Wert starten z.B. 500ns bzw. zurücksetzen
    IFS1bits.CNIF = 0;
    //CN Interrupt Flag freigeben
}

void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void){
    if(d_read(cn_value[1])) cn_value[0]=1;
    //cn_value confirm flag setzen, falls auch nach Ablauf des Timers ein high Pegel anliegt
    T5CONbits.TON=0;
    //Timer 5 wieder ausschalten
    IFS1bits.T5IF = 0;
    //Clear Timer 5 Interrupt Flag
}