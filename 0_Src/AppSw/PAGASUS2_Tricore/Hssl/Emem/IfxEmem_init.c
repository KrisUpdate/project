/*
 * IfxEmem_init.c
 *
 *  Created on: 14.06.2016
 */

 /*
 ******************************************************************************
 * Standard include files
 ******************************************************************************
 */
#include "IfxEmem_init.h"
#include "IfxEmem.h"
#include "IfxScuWdt.h"
#include "_Impl/IfxEmem_cfg.h"

/*
 ******************************************************************************
 * Project include files
 ******************************************************************************
 */


/*
 ******************************************************************************
 * Macro defines
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Types
 ******************************************************************************
 */


/*
 ******************************************************************************
 * Variable declarations
 ******************************************************************************
 */


/*
 ******************************************************************************
 * Routine prototypes
 ******************************************************************************
 */


/*
 ******************************************************************************
 * Routine implementation
 ******************************************************************************
 */

void IfxEmem_Init(){
	// Enable Emem
	while (IfxEmem_isModuleEnabled()==FALSE){
	   	IfxEmem_setClockEnableState(IfxEmem_State_enabled);
	}

	// Unlock Emem
	IfxEmem_setUnlockMode(&MODULE_EMEM);
	while (IfxEmem_getLockedState()==IfxEmem_LockedState_locked);

	IfxEmem_TileNumber tile;

	for (tile = IfxEmem_TileNumber_0; tile <= IfxEmem_TileNumber_15; tile++)
		IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_unusedMode, tile);

	for (tile = IfxEmem_TileNumber_0; tile <= IfxEmem_TileNumber_15; tile++)
		IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode, tile);


	// Disable ECC_ERR before initialization
	IfxEmem_disableEccErrorReporting(IfxEmem_MpuIndex_0);
	IfxEmem_disableEccErrorReporting(IfxEmem_MpuIndex_1);
	IfxEmem_disableEccErrorReporting(IfxEmem_MpuIndex_2);
	IfxEmem_disableEccErrorReporting(IfxEmem_MpuIndex_3);

	// Initialize 4MB EMEM
	uint32* pmem;
	uint32  value=0;
	uint32  i;
	pmem = (uint32 *)IFXEMEM_START_ADDR_CPU;
	for (i=0; i<(IFXEMEM_SIZE/4); i++){
		*(pmem++) = value++;
	}

	// Enable ECC_ERR after initialization
	IfxEmem_enableEccErrorReporting(IfxEmem_MpuIndex_0);
	IfxEmem_enableEccErrorReporting(IfxEmem_MpuIndex_1);
	IfxEmem_enableEccErrorReporting(IfxEmem_MpuIndex_2);
	IfxEmem_enableEccErrorReporting(IfxEmem_MpuIndex_3);

}
