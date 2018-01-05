/* 
 * File:   timer_mgmnt.h
 * Author: finst
 *
 * Created on 22. November 2017, 09:53
 */

#ifndef TIMER_MGMNT_H
#define	TIMER_MGMNT_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    extern void wait_ms(int ms);
    extern void wait_us(int us);
    extern void set_timer1(int PR1_value);
    extern void set_timer2(int upper16, int lower16, int width);
    extern void set_timer4(int upper16, int lower16, int width);
    extern void set_timer3(int PR_value);
    extern void set_timer5(int PR_value);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_MGMNT_H */

