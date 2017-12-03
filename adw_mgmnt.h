/* 
 * File:   adw_mgmnt.h
 * Author: finst
 *
 * Created on 22. November 2017, 20:26
 */

#ifndef ADW_MGMNT_H
#define	ADW_MGMNT_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern void adw12_cfg(int eingangspin, char mode);
    extern void adw10_cfg(int eingangspin_ch0, int eingangspin_ch1, char mode);


#ifdef	__cplusplus
}
#endif

#endif	/* ADW_MGMNT_H */

