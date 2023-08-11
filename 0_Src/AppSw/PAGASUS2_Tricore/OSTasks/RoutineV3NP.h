#ifndef _ROUTINE_V3NP_H_
#define _ROUTINE_V3NP_H_

typedef enum{
    ROUTINE_V3NP_SELECT_AURIX_MAIN,
    ROUTINE_V3NP_SELECT_AURIX_SECOND
} routine_v3np_select_t;

void RoutineV3NP_init(routine_v3np_select_t select);

#endif /* _ROUTINE_V3NP_H_ */