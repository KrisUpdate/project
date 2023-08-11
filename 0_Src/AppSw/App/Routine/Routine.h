#ifndef _ROUTINE_H_
#define _ROUTINE_H_

typedef void (*routine_func_t)(void);

typedef struct{
    routine_func_t always;
    routine_func_t every_1ms;
    routine_func_t every_10ms;
    routine_func_t every_100ms;
    routine_func_t every_1s;
} routine_func_set_t;

void Routine_bind(routine_func_set_t func_set);
void Routine_do(void);

#endif /* _ROUTINE_H_ */