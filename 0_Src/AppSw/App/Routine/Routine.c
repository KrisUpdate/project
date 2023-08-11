#include "Routine.h"
#include "Platform_Types.h"

// those event flags are set from ISR for corresponding routine process
extern  volatile boolean eventFlag_1ms_stm_0;
extern  volatile boolean eventFlag_10ms_stm_0;
extern  volatile boolean eventFlag_100ms_stm_0;
extern  volatile boolean eventFlag_1s_stm_0;

static routine_func_set_t _func_set = {(void*)0, (void*)0, (void*)0, (void*)0, (void*)0};

void Routine_bind(routine_func_set_t func_set)
{
    _func_set = func_set;
}

void Routine_do(void)
{
    if(_func_set.always)
        _func_set.always();

    if(_func_set.every_1ms && eventFlag_1ms_stm_0){
        eventFlag_1ms_stm_0 = FALSE;
        _func_set.every_1ms();
    }
    if(_func_set.every_10ms && eventFlag_10ms_stm_0){
        eventFlag_10ms_stm_0 = FALSE;
        _func_set.every_10ms();
    }
    if(_func_set.every_100ms && eventFlag_100ms_stm_0){
        eventFlag_100ms_stm_0 = FALSE;
        _func_set.every_100ms();
    }
    if(_func_set.every_1s && eventFlag_1s_stm_0){
        eventFlag_1s_stm_0 = FALSE;
        _func_set.every_1s();
    }
}