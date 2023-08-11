#include "Configuration.h"

#if (PROJECT == PROJECT_V31C)
#elif (PROJECT == PROJECT_TRIBOARD)
#elif (PROJECT == PROJECT_V3NA)
#elif (PROJECT == PROJECT_V3AA)
#elif (PROJECT == PROJECT_PAGASUS2)
#include "core_alive_check.h"
#include <string.h>

#define CORE_ACTIVE   1
#define CORE_INACTIVE 0 
#define CORE_MAX 6
//#define CODE_LAGACY 0

typedef enum {
  CORE_HEARTBEAT_NOW,
  CORE_HEARTBEAT_LAST,
  CORE_HEALTHY,
  CORE_STATUS
} core_state_t;

boolean core_asc_debug[CORES_MAX];
boolean core_asc_enable[CORES_MAX] = {1, 0 , 0, 0, 0, 0};
volatile uint32 core_heartbeat[CORES_MAX][CORE_STATUS];
static unsigned int count[CORES_MAX];

boolean core_raise_count(unsigned int coresID)
{
    boolean result = TRUE;
    if(coresID  >= CORES_MAX){
        result = FALSE;
    }
    else{ 
        if(core_asc_debug[coresID] == 0){
            core_heartbeat[coresID][CORE_HEARTBEAT_NOW]++;
        }
    }
    return result;
}

uint32 core_show_count(unsigned int coresID)
{
    if(coresID >= CORES_MAX){
        coresID = CORE0;
    }        
    return core_heartbeat[coresID][CORE_HEARTBEAT_NOW];
}

uint32 core_show_healthy(unsigned int coresID)
{
    if(coresID >= CORES_MAX){
        coresID = 0;
    }        
    return core_heartbeat[coresID][CORE_HEALTHY];
}

uint32 core_show_debug(unsigned int coreID)
{
    if(coreID >= CORES_MAX){
        coreID = 0;
    }        
    return core_asc_debug[coreID];
}

boolean check_core_shell_enabled(unsigned int coreID)
{
    if(coreID >= CORES_MAX){
        coreID = 0;
    }        
    return core_asc_enable[coreID];
}

void core_shell_enable(unsigned int coreID)
{
    if(coreID >= CORES_MAX){
        coreID = 5;
    }
    memset(core_asc_enable, 0, sizeof(core_asc_enable));
    core_asc_enable[coreID] = TRUE;
}


void core_simulate_hang(unsigned int coreID, boolean action)
{
    core_asc_debug[coreID] = action;
}

uint32 roll_call_cores_status_check(unsigned int  coresID)
{
/*
    if(core_heartbeat[coresID][CORE_HEARTBEAT_LAST] == 0){
        core_heartbeat[coresID][CORE_HEARTBEAT_LAST] = core_heartbeat[coresID][CORE_HEARTBEAT_NOW];
        count[coresID] = 0;
    }
*/
    if(core_heartbeat[coresID][CORE_HEARTBEAT_LAST] == core_heartbeat[coresID][CORE_HEARTBEAT_NOW] ){
        if(count[coresID] == HANG_DECTION){
            count[coresID] = HANG_DECTION;  
            core_heartbeat[coresID][CORE_HEALTHY] = CORE_INACTIVE;
        }else{
            count[coresID]++;
        }
    }
    else{
        core_heartbeat[coresID][CORE_HEARTBEAT_LAST] = core_heartbeat[coresID][CORE_HEARTBEAT_NOW];
        core_heartbeat[coresID][CORE_HEALTHY] = CORE_ACTIVE;
        count[coresID] = 0;
    }
    return count[coresID];
}

void roll_call_cores_status_check_all(void)
{
    unsigned int coreID;
    for(coreID = 0; coreID < CORES_MAX; coreID++){
        roll_call_cores_status_check(coreID);
    }
}

#if CODE_LAGACY
uint32 get_active_cores(void)
{
    int i = 0;
    uint32 result = 0;
    for(i = 0; i < CORES_MAX; i++){
        if(core_heartbeat[i][CORE_HEALTHY] == CORE_ACTIVE){
            result = (uint32)i;
            break;
        }          
    }
    return result;
}
#endif 

#else
#endif
