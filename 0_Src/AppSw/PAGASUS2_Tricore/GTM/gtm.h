/*
 * gtm.h
 *
 *  Created on: 2017Äê9ÔÂ5ÈÕ
 *      Author: bradf
 */

#ifndef _GTM_H_
#define _GTM_H_

/* GTM is tested in 39x only, in 29x we don't need that */
#if MCU_PLATFORM == _AURIX_TC39X_
#include <Gtm/Tom/Timer/IfxGtm_Tom_Timer.h>

/*\brief Initial the GTM in TOM mode (generating the 1ms timer interrupt)
 *\param: void
 *\return: void */
IFX_EXTERN void GtmTim_init(void);

/*\brief Call this function in runtime of main,
 *       To run the GTM background functions
 *\param: void
 *\return: void */
IFX_EXTERN boolean GtmTim_getFreq(uint8 channelIndex, uint32* freq_p);

#endif //endof #if MCU_PLATFORM == _AURIX_TC39X_

#endif /* _GTM_H_ */
