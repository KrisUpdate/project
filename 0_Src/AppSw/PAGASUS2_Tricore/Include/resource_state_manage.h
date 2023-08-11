/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef __RESOURCE_STATE_MANAGE_H_
#define __RESOURCE_STATE_MANAGE_H_

#include <Cpu/Std/Ifx_Types.h>
#include "protocol_common.h"
#include "Configuration.h"

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/* gmsl_cable */
//TODO we may need to add this for Ethernet Protocol
#define GMSL_CABLE_COAX                 0x41
#define GMSL_CABLE_DIFF                 0x42
#define GMSL_CABLE_IGNORE               0xFF

typedef enum
{
#if CAMERA_A_ENABLE == 1
    CAMERA_MODULE_A = 0,
#endif
#if CAMERA_B_ENABLE == 1
    CAMERA_MODULE_B,
#endif
#if CAMERA_C_ENABLE == 1
    CAMERA_MODULE_C,
#endif
#if CAMERA_D_ENABLE == 1
    CAMERA_MODULE_D,
#endif
#if CAMERA_E_ENABLE == 1
    CAMERA_MODULE_E,
#endif
    CAMERA_MODULE_MAX,
}eCameraModule;

typedef enum
{
    CAMERA_UNIT_A0 = 0,
    CAMERA_UNIT_A1,
    CAMERA_UNIT_A2,
    CAMERA_UNIT_A3,
    CAMERA_UNIT_B0,
    CAMERA_UNIT_B1,
    CAMERA_UNIT_B2,
    CAMERA_UNIT_B3,
    CAMERA_UNIT_C0,
    CAMERA_UNIT_C1,
    CAMERA_UNIT_C2,
    CAMERA_UNIT_C3,
    CAMERA_UNIT_D0,
    CAMERA_UNIT_D1,
    CAMERA_UNIT_D2,
    CAMERA_UNIT_D3,
    CAMERA_UNIT_MAX,
}eCameraUnit;

#define CAMERA_UNITS_PER_MODULE 4

typedef enum
{
    CAMERA_ON = 1,
    CAMERA_OFF,
    CAMERA_ACTION_MAX,
}eCameraAction;

typedef enum
{
    FRSYNC_ON = 1,
    FRSYNC_OFF,
    FRSYNC_ACTION_MAX,
}eCamFrsyncAction;


typedef enum  {NOT_REGISTERED = 0, REGISTERED_AS_MASTER, REGISTERED_AS_SLAVE}registration_status;
typedef enum  {STATE_0 = 0, STATE_1, STATE_2, STATE_3}state_machine_states;
typedef enum  {I_OFF = 0, I_ON}power;

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
#if (SUPPORT_PROTOCOL_VERSION == 0x06)

#endif

#if (SUPPORT_PROTOCOL_VERSION == 0x05) || (SUPPORT_PROTOCOL_VERSION == 0x06) || (SUPPORT_PROTOCOL_VERSION == 0x07)
void ResetOwnershipOnTegraReset(uint8 tegra);
uint8 Resource_GetOwnershipMaster(eCameraModule cam_module, uint8 tegra);
uint8 Resource_GiveawayOwnershipMaster(eCameraModule cam_module, uint8 tegra);
uint8 Resource_GetOwnershipSlave(eCameraModule cam_module, uint8 tegra);
uint8 Resource_GiveawayOwnershipSlave(eCameraModule cam_module, uint8 tegra);
uint8 Resource_PowerOn(eCameraModule cam_module, uint8 cam_units, uint8 tegra);
uint8 Resource_PowerOff(eCameraModule cam_module, uint8 cam_units, uint8 tegra);
uint8 Resource_StateMachine_Aggre(eCameraModule cam_module, uint8 isMaster, uint8 act, uint8 slaveId);
uint8 Resource_StateMachine_Cameras(eCameraModule cam_module, uint8 cam_units, uint8 isMaster, uint8 act, uint8 slaveId);
uint8 Resource_StateMachine_Camera(eCameraModule cam_module, uint8 cam_unit, uint8 isMaster, uint8 input, uint8 slaveId);
/** \brief Set the Camera state
 * \param  cam_select camera units
 * \param  action on/off
 * \return true if successful
 */
boolean Resource_SetCameraState(eCameraUnit cam_unit, eCameraAction action);

/** \brief Get the Camera state
 * \param  cam_select camera units
 * \return on/off
 */
uint8 Resource_GetCameraState(eCameraUnit cam_unit);

/** \brief Set the Camera Aggregator state
 * \param  cam_module camera module
 * \param  action on/off
 * \return true if successful
 */
boolean Resource_SetCameraAggState(eCameraModule cam_module, eCameraAction action);

/** \brief Get the Camera Aggregator state
 * \param  cam_module camera module
 * \return on/off
 */
uint8 Resource_GetCameraAggState(eCameraModule cam_module);

/**\brief  Set frsync_src state
 * \param  tegra TEGRA_A/TEGRA_B
 * \return true if successful
 */
boolean Resource_SetFsyncSrcState(uint8 tegra);

/**\brief  Get frsync_src state
 * \return TEGRA_A/TEGRA_B
 */
uint8 Resource_GetFsyncSrcState();

/**\brief  Set frsync state
 * \param  cam_module camera module
 * \param  action frame sync on/off
 * \return true if successful
 */
boolean Resource_SetFsyncEnState(eCameraModule cam_module, eCamFrsyncAction action);

/**\brief  Get frsync state
 * \param  cam_module camera module
 * \return on/off
 */
uint8 Resource_GetFsyncEnState(eCameraModule cam_module);

#else //(SUPPORT_PROTOCOL_VERSION == 0x05)

/** \brief Set the Camera state
 * \param  cam_module camera module
 * \param  action on/off
 * \return true if successful
 */
boolean Resource_SetCameraState(eCameraModule cam_module, eCameraAction action);

/** \brief Get the Camera state
 * \param  cam_module camera module
 * \return on/off
 */
uint8 Resource_GetCameraState(eCameraModule cam_module);

/**\brief Initial GMSL state, it will config GPIO direction
 * \param  tegra TEGRA_A/TEGRA_B
 * \return true if successful
 */
boolean Resource_InitGmslState(uint8 tegra);

/**\brief  Set GMSL state
 * \param  tegra TEGRA_A/TEGRA_B
 * \return true if successful
 */
boolean Resource_SetGmslState(uint8 tegra);

/**\brief  Get GMSL state
 * \return TEGRA_A/TEGRA_B
 */
uint8 Resource_GetGmslState();


/**\brief Initial GMSL Cable state, it will config GPIO direction
 * \param  mode GMSL_CABLE_COAX/GMSL_CABLE_DIFF
 * \return true if successful
 */
boolean Resource_InitGmslCableState(uint8 mode);

/**\brief Set GMSL Cable state
 * \param  mode GMSL_CABLE_COAX/GMSL_CABLE_DIFF
 * \return true if successful
 */
boolean Resource_SetGmslCableState(uint8 mode);

/**\brief Get GMSL Cable state
 * \return  mode GMSL_CABLE_COAX/GMSL_CABLE_DIFF
 */
uint8 Resource_GetGmslCableState();

/**\brief Set FPDL state
 * \param  tegra TEGRA_A/TEGRA_B
 * \return true if successful
 */
boolean Resource_SetFpdlState(uint8 tegra);
/**\brief  Get FPDL state
 * \return TEGRA_A/TEGRA_B
 */
uint8 Resource_GetFpdlState();

/**\brief Set PCI state
 * \param  tegra TEGRA_A/TEGRA_B
 * \return true if successful
 */
boolean Resource_SetPciState(uint8 tegra);
/**\brief  Get PCI state
 * \return TEGRA_A/TEGRA_B
 */
uint8 Resource_GetPciState();

/**\brief Init TegraAHdmi state
 * \param  tegra TEGRA/MXM
 * \return true if successful
 */
boolean Resource_InitTegraAHdmiState(uint8 source);
/**\brief Set TegraAHdmi state
 * \param  tegra TEGRA/MXM
 * \return true if successful
 */
boolean Resource_SetTegraAHdmiState(uint8 source);
/**\brief  Get TegraAHdmi state
 * \return  tegra TEGRA/MXM
 */
uint8 Resource_GetTegraAHdmiState();

/**\brief Init TegraBHdmi state
 * \param  tegra TEGRA/MXM
 * \return true if successful
 */
boolean Resource_InitTegraBHdmiState(uint8 source);
/**\brief Set TegraBHdmi state
 * \param  tegra TEGRA/MXM
 * \return true if successful
 */
boolean Resource_SetTegraBHdmiState(uint8 source);
/**\brief  Get TegraBHdmi state
 * \return  tegra TEGRA/MXM
 */
uint8 Resource_GetTegraBHdmiState();

#endif //(SUPPORT_PROTOCOL_VERSION == 0x05)

/**\brief Init FAN state
 * \param  tegra GOOD_STATE/BAD_STATE
 * \return true if successful
 */
boolean Resource_InitFanState(uint8 state);
/**\brief Set FAN state
 * \param  tegra GOOD_STATE/BAD_STATE
 * \return true if successful
 */
boolean Resource_SetFanState(uint8 state);
/**\brief Get FAN state
 * \return  tegra GOOD_STATE/BAD_STATE
 */
uint8 Resource_GetFanState();


#endif  // __RESOURCE_STATE_MANAGE_H_
