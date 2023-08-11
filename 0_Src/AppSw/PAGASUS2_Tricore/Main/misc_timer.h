#ifndef MISC_TIMER_H_
#define MISC_TIMER_H_

#include "Ifx_Types.h"

#define timeafter(A, B) ((int)((B) - (A)) < 0)

uint64 get_local_time(void);

void waitfor(unsigned t);


/*!
 * Utility for keeping track of timeout periods
 */
typedef struct {
  uint64 timeout;
  unsigned int period;
  int active;
  int timeout_multiplier;
} avb_timer;

void init_avb_timer(avb_timer* tmr, int mult);
void start_avb_timer(avb_timer* tmr, unsigned int period_cs);
int avb_timer_expired(avb_timer* tmr);
void stop_avb_timer(avb_timer* tmr);
int is_tmr_active(avb_timer* tmr);

#endif /*MISC_TIMER_H_*/
