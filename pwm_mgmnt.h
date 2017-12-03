/* 
 * File:   pwm_mgmnt.h
 * Author: finst
 *
 * Created on 25. November 2017, 21:23
 */

#ifndef PWM_MGMNT_H
#define	PWM_MGMNT_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern void pwm1_init(int pinnummer, int freq);
    extern void pwm2_init(int pinnummer, int freq);
    extern void pwm3_init(int pinnummer, int freq);
    extern void pwm4_init(int pinnummer, int freq);
    extern void pwm1_duty(int dutycycle);
    extern void pwm2_duty(int dutycycle);
    extern void pwm3_duty(int dutycycle);
    extern void pwm4_duty(int dutycycle);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_MGMNT_H */

