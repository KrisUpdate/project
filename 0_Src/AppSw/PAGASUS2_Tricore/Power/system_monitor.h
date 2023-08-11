/*
 * system_monitor.h
 *
 *  Created on: Feb 13, 2019
 *      Author: bradf
 */

#ifndef _SYSTEM_MONITOR_H_
#define _SYSTEM_MONITOR_H_

#include "Configuration.h"
#include "SysSe/Comm/Ifx_Shell.h"

// 0: no backup power
// 1: backup power
// 2: already did the return-on
typedef enum
{
	NO_BACKUP_POWER,
	BACKUP_POWER,
	RETURNONED,
}backup_power_state;

boolean System_Monitor_Config_Cmd(pchar args, void *data, IfxStdIf_DPipe *io);
void System_Monitor_KL30_Init(void);
void System_Monitor_KL30_Routine(void);
unsigned char check_backup_power(void);
void set_backup_power(unsigned char flag);
#if USE_WC_PATTERN == 1
IFX_EXTERN boolean Start_WC_Pattern(pchar args, void *data, IfxStdIf_DPipe *io);
#endif
void booting_system(void);
#endif /* _SYSTEM_MONITOR_H_ */
