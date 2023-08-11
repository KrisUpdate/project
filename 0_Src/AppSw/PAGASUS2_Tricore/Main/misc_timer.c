#include "misc_timer.h"
#include "IfxStm_reg.h"
#include "Ifx_Types.h"
#include "IfxStm.h"

#define TICKS_PER_CENTISECOND (portTICK_PERIOD_MS * 10)

uint64 get_local_time(void)
{
	uint64 t;

	t = IfxStm_get(&MODULE_STM0);
	return t;
}

void waitfor(unsigned t)
{
	IfxStm_waitTicks(&MODULE_STM0, IfxStm_getTicksFromMicroseconds(&MODULE_STM0,t));
}

int is_tmr_active(avb_timer* tmr)
{
    return (tmr->active != 0);
}
void init_avb_timer(avb_timer* tmr, int mult)
{
  tmr->active = 0;
  tmr->timeout_multiplier = mult;
}

void start_avb_timer(avb_timer* tmr, unsigned int period_cs)
{
  tmr->period = (period_cs * IfxStm_getTicksFromMicroseconds(&MODULE_STM0,10000));
  tmr->timeout = get_local_time() + (period_cs * IfxStm_getTicksFromMicroseconds(&MODULE_STM0,10000));
  tmr->active = tmr->timeout_multiplier;
}

int avb_timer_expired(avb_timer* tmr)
{
  uint64 now = get_local_time();
  if (!tmr->active)
    return 0;

  if (timeafter(now, tmr->timeout)) {
    tmr->active--;
    tmr->timeout = now + tmr->period;
  }

  return (tmr->active == 0);
}

void stop_avb_timer(avb_timer* tmr)
{
  tmr->active = 0;
}
