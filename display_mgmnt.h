/* 
 * File:   display_mgmnt.h
 * Author: finst
 *
 * Created on 13. Dezember 2017, 20:00
 */

#ifndef DISPLAY_MGMNT_H
#define	DISPLAY_MGMNT_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern void display_clear(char deviceid);
    extern void display_init(char deviceid);
    extern void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void);
    extern int timecount;
    extern void display_send(unsigned int data);
    extern int check_bf(char deviceid);


#ifdef	__cplusplus
}
#endif

#endif	/* DISPLAY_MGMNT_H */

