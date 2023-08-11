/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef GPIO_H
#define GPIO_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Configuration.h"
#include <Cpu/Std/Ifx_Types.h>
#include "gpioexp.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
extern volatile boolean pmic_a_on;
extern volatile boolean pmic_b_on;
extern volatile boolean whole_board_off;
extern volatile boolean brd_alert;
extern volatile boolean brd_alert_2;
extern volatile boolean brd_alert_3;
extern volatile boolean tegra_a_alert;
extern volatile boolean tegra_b_alert;
extern volatile boolean brd_therm;
extern volatile boolean tegra_a_therm;
extern volatile boolean tegra_b_therm;

//#if PROJECT == PROJECT_DRIVE_AX && DRIVE_AX_B0x
extern volatile boolean pmic_c_on;
extern volatile boolean tegra_c_alert;
extern volatile boolean tegra_c_therm;
extern volatile boolean dgpu_a_on;
extern volatile boolean dgpu_a_alert;
extern volatile boolean dgpu_b_on;
extern volatile boolean dgpu_b_alert;
extern volatile boolean ovr_cpu_thwn;
extern volatile boolean ovr_gpu_thwn;
extern volatile boolean ovr_soc_thwn;
extern volatile boolean ovr_cv_thwn;
extern volatile boolean xa_ovr_thwn;
extern volatile boolean xb_ovr_thwn;
//#endif

extern volatile boolean vdd3v3_off;

extern char pcb_variant;
extern uint8 pcb_version;
extern char pcb_spin;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
void SetOutputs_to_System_PowerOff_State(void);
void SetOutputs_to_System_Inactive_State(void);
//#if (PROJECT == PROJECT_DRIVE_AX) && (DRIVE_AX_B0x == 1)
#if 1
/*\brief Power up the whole board
 *\param: in: boolean isForcePoweron
			  True: force powering up the board without checking PGOOD signals
			  False: power up the board with checking PGOOD signals
 *\return: boolean: return True if power up successfully, FALSE if power up fail */
boolean System_PowerOn(boolean isForcePoweron);
boolean check_pin_status(void);
/*\brief Power down the whole board
 *\param: void
 *\return: boolean: return True if power down successfully, FALSE if power down fail */
boolean System_PowerOff();

/*\brief Initial Aurix itself (GPIO, Ethernet etc), it's used only by second Aurix
 *\param: void
 *\return: boolean: return True if initialization successfully, FALSE if fail */
boolean Second_Aurix_Boot(void);
#else
void System_PowerOn(boolean pmic_a, boolean pmic_b);
void System_PowerOff(boolean pmic_a, boolean pmic_b);
#endif
void Monitor_Therm_Status(void);
void Monitor_Aurix2_Therm_Status(void);
void Monitor_System_Status(void);
void execute_thermal_shutdown(void);
void toggle_levelShifter(void);
boolean is_PowerOff(void);
void Initial_Sku_Setting(void);


#endif  // GPIO_H
