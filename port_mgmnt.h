/* 
 * File:   port_mgmnt.h
 * Author: finst
 *
 * Created on 21. November 2017, 11:34
 */

#ifndef PORT_MGMNT_H
#define	PORT_MGMNT_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern void clr_ports();
    extern void pin_cfg(int pinnummer, char mode, char richtung);
    extern void pullup_cfg(int pinnummer, int state);
    extern void rpin_cfg(int pinnummer, int mode, char richtung);
    extern void cn_cfg(int pinnummer, int state);
    extern void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void);
    extern int cn_value[2];

#ifdef	__cplusplus
}
#endif

#endif	/* PORT_MGMNT_H */

