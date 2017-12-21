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
    extern void display_write(char deviceid, char *pointer, int cnt);
    extern int timecount;
    extern void send_8(char deviceid, unsigned int data, int type);
    extern int check_bf(char deviceid);

#ifdef	__cplusplus
}
#endif

#endif	/* DISPLAY_MGMNT_H */

