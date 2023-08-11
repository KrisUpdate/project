#include "os.h"
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"
#include "lwip/err.h"
#include "Cpu_Main.h"
#include "Ifx_Console.h"
#define printf Ifx_Console_print

/** Set this to 1 to use a mutex for SYS_ARCH_PROTECT() critical regions.
 * Default is 0 and locks interrupts/scheduler for SYS_ARCH_PROTECT().
 */
#ifndef LWIP_EE_SYS_ARCH_PROTECT_USES_MUTEX
#define LWIP_EE_SYS_ARCH_PROTECT_USES_MUTEX     0
#endif

/** Set this to 1 to include a sanity check that SYS_ARCH_PROTECT() and
 * SYS_ARCH_UNPROTECT() are called matching.
 */
#ifndef LWIP_EE_SYS_ARCH_PROTECT_SANITY_CHECK
#define LWIP_EE_SYS_ARCH_PROTECT_SANITY_CHECK   0
#endif

#if SYS_LIGHTWEIGHT_PROT

sys_prot_t
sys_arch_protect(void)
{
#if LWIP_FREERTOS_SYS_ARCH_PROTECT_USES_MUTEX
  BaseType_t ret;
  LWIP_ASSERT("sys_arch_protect_mutex != NULL", sys_arch_protect_mutex != NULL);

  ret = xSemaphoreTakeRecursive(sys_arch_protect_mutex, portMAX_DELAY);
  LWIP_ASSERT("sys_arch_protect failed to take the mutex", ret == pdTRUE);
#else /* LWIP_FREERTOS_SYS_ARCH_PROTECT_USES_MUTEX */
  DisableAllInterrupts();

#endif /* LWIP_FREERTOS_SYS_ARCH_PROTECT_USES_MUTEX */
#if LWIP_FREERTOS_SYS_ARCH_PROTECT_SANITY_CHECK
  {
    /* every nested call to sys_arch_protect() returns an increased number */
    sys_prot_t ret = sys_arch_protect_nesting;
    sys_arch_protect_nesting++;
    LWIP_ASSERT("sys_arch_protect overflow", sys_arch_protect_nesting > ret);
    return ret;
  }
#else
  return 1;
#endif
}

void
sys_arch_unprotect(sys_prot_t pval)
{
#if LWIP_FREERTOS_SYS_ARCH_PROTECT_USES_MUTEX
  BaseType_t ret;
#endif
#if LWIP_FREERTOS_SYS_ARCH_PROTECT_SANITY_CHECK
  LWIP_ASSERT("unexpected sys_arch_protect_nesting", sys_arch_protect_nesting > 0);
  sys_arch_protect_nesting--;
  LWIP_ASSERT("unexpected sys_arch_protect_nesting", sys_arch_protect_nesting == pval);
#endif

#if LWIP_FREERTOS_SYS_ARCH_PROTECT_USES_MUTEX
  LWIP_ASSERT("sys_arch_protect_mutex != NULL", sys_arch_protect_mutex != NULL);

  ret = xSemaphoreGiveRecursive(sys_arch_protect_mutex);
  LWIP_ASSERT("sys_arch_unprotect failed to give the mutex", ret == pdTRUE);
#else /* LWIP_FREERTOS_SYS_ARCH_PROTECT_USES_MUTEX */
  EnableAllInterrupts();
#endif /* LWIP_FREERTOS_SYS_ARCH_PROTECT_USES_MUTEX */
  LWIP_UNUSED_ARG(pval);
}

extern uint32 Ifx_OSTask_1ms_Count;
extern App_Cpu g_AppCpu0; /* CPU 0 global data, needed for sys_now */

u32_t sys_now(void)
{
	//return Ifx_OSTask_1ms_Count;
	return g_AppCpu0.TickCount_1ms;

}

#endif /* SYS_LIGHTWEIGHT_PROT */
#if (NO_SYS==0)
err_t sys_sem_new(sys_sem_t *sem, u8_t initial_count)
{
  LWIP_ASSERT("sem != NULL", sem != NULL);
  LWIP_ASSERT("initial_count invalid (not 0 or 1)",
    (initial_count == 0) || (initial_count == 1));
  SemRefType tmpSem = sem;

  InitSem(tmpSem, initial_count);
  if(sem == NULL) {
    SYS_STATS_INC(sem.err);
    return ERR_MEM;
  }
  SYS_STATS_INC_USED(sem);

  if(initial_count == 1) {
	  StatusType ret = PostSem(sem);
    LWIP_ASSERT("sys_sem_new: initial give failed", ret == E_OK);
  }
  return ERR_OK;
}
void sys_sem_signal(sys_sem_t *sem)
{
  StatusType ret;
  LWIP_ASSERT("sem != NULL", sem != NULL);
  SemRefType tmpSem = sem;

  ret = PostSem(tmpSem);
  /* queue full is OK, this is a signal only... */
  LWIP_ASSERT("sys_sem_signal: sane return value",    (ret == E_OK));
}
#endif
void sys_arch_msleep(u32_t delay_ms)
{
#if 0 //this is EE os
	uint32 orig_tick = Ifx_OSTask_1ms_Count;
	while((Ifx_OSTask_1ms_Count-orig_tick)<delay_ms);

#else
	uint32 orig_tick = g_AppCpu0.TickCount_1ms;
	while((g_AppCpu0.TickCount_1ms - orig_tick) < delay_ms)
	{
		__nop();
		//printf("tick 1 %d tick2 %d\n\r",g_AppCpu0.TickCount_1ms,orig_tick);
	}
#endif

}
