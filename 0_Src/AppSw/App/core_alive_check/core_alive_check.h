#ifndef CORE_ALIVE_CHECK_H 
#define CORE_ALIVE_CHECK_H

#include "Ifx_Types.h"

#define HANG_DECTION 5
typedef enum {
  CORE0 = 0,
  CORE1 = 1,
  CORE2 = 2,
  CORE3 = 3,
  CORE4 = 4,
  CORE5 = 5,
  CORES_MAX = 6,
} core_number_t;


void core_simulate_hang(unsigned int coreID, boolean action);
void core_shell_enable(unsigned int coreID);
void roll_call_cores_status_update(unsigned int coreID);
uint32 core_show_count(unsigned int coresID);
uint32 core_show_healthy(unsigned int coresID);
uint32 core_show_debug(unsigned int coresID);
uint32 roll_call_cores_status_check(unsigned int coreID);
boolean core_raise_count(unsigned int coresID);
boolean check_core_shell_enabled(unsigned int coreID);
void roll_call_cores_status_check_all(void);
#endif /* CORE_ALIVE_CHECK_H */

