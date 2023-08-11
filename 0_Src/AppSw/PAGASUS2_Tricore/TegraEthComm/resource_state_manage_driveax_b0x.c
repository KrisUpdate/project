/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#include "Configuration.h"
#include "Port/Std/IfxPort.h"
#include "resource_state_manage.h"
#include <internal_fw_debug.h>
#include "../Gpio_exp/gpioexp.h"
#include "CIM.h"
#include "board_I2C_devices.h"

#if (PROJECT == PROJECT_PAGASUS2) && (PG2_PREEVT == 1)
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define RESOURCE_STATE_DEBUG_ENABLE 1

#if RESOURCE_STATE_DEBUG_ENABLE == 1
#define RESOURCE_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define RESOURCE_DEBUG_PRINTF(x)
#endif

#define RESOURCE_PRINTF_ALWAYS(x) DEBUG_PRINTF_ALWAYS(x)
#define CAMGROUP_ALIVE_PERIOD_TIMEOUT 20

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
extern boolean disable_udp_cmd_flag;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/* resource state */
/* TBD: Even though Aurix console is single-instance,
 * the commands over TACP can potentially come in parallel
 * from multiple Tegras;
 * So, the Set and Get functions can potentially be getting
 * invoked in parallel, thus locking should be enabled for
 * these variables*/
static uint8 cam_unit_state[] = {CAMERA_OFF, CAMERA_OFF, CAMERA_OFF, CAMERA_OFF,
        CAMERA_OFF, CAMERA_OFF, CAMERA_OFF, CAMERA_OFF,
        CAMERA_OFF, CAMERA_OFF, CAMERA_OFF, CAMERA_OFF,
        CAMERA_OFF, CAMERA_OFF, CAMERA_OFF, CAMERA_OFF};
static uint8 cam_agg_state[] = {CAMERA_OFF, CAMERA_OFF, CAMERA_OFF, CAMERA_OFF};
static uint8 cam_frsync_state[] = {FRSYNC_OFF, FRSYNC_OFF, FRSYNC_OFF, FRSYNC_OFF};

static uint8 fan_state = INGORE;
static uint8 fsync_src = TEGRA_A;

//Registration status for each of the THREE Tegras and each of the FOUR Camera Groups
static uint8 iregistry[3][4] = { {NOT_REGISTERED, NOT_REGISTERED, NOT_REGISTERED, NOT_REGISTERED}, {NOT_REGISTERED, NOT_REGISTERED, NOT_REGISTERED, NOT_REGISTERED}, {NOT_REGISTERED, NOT_REGISTERED, NOT_REGISTERED, NOT_REGISTERED}};
static uint16 nvccpCamGroupVersion[3][4] = { {PROTOCOL_VERSION_V07,PROTOCOL_VERSION_V07,PROTOCOL_VERSION_V07,PROTOCOL_VERSION_V07}, {PROTOCOL_VERSION_V07,PROTOCOL_VERSION_V07,PROTOCOL_VERSION_V07,PROTOCOL_VERSION_V07}, {PROTOCOL_VERSION_V07,PROTOCOL_VERSION_V07,PROTOCOL_VERSION_V07,PROTOCOL_VERSION_V07}};

static uint16 iTimeStamp[3][4] = { {0U,0U,0U,0U}, {0U,0U,0U,0U}, {0U,0U,0U,0U}};

//State Machine for each of the cameras
static uint8 istate_cam[16] = { STATE_0, STATE_0, STATE_0, STATE_0, STATE_0, STATE_0, STATE_0, STATE_0,
                         STATE_0, STATE_0, STATE_0, STATE_0, STATE_0, STATE_0, STATE_0, STATE_0 };

//State Machine for each of the aggregator
static uint8 istate_agg[4] = { STATE_0, STATE_0, STATE_0, STATE_0 };

/* camera management */
/* set intial value to 1, so that we can turn it off at power up */
static uint8 master_cnt[] = {1, 1, 1, 1};

uint8 Resource_PowerOff(eCameraModule cam_module, uint8 cam_units, uint8 tegra);
#define VERIFY_PIN_DELAY_TIME  TIMER_INDEX_100US

/*Buffer for holding Slave Status for Aggregator Power Supply*/
uint8 slaveState_A[4][2] = {{0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}};
/*Buffer for holding Slave Status for Camera Power Supply*/
uint8 slaveState_C[16][2] = {{0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}};

/* Global Variable to store the status of CCP I2C Owner */
uint8 i2c_owner_registered = 0;

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void PopulateNvccpVersionForCamGroup(eCameraModule cam_module, uint8 tegra, uint8 version)
{
  uint8 current_req_tegra = tegra-1;
  nvccpCamGroupVersion[current_req_tegra][cam_module] = version;
}
uint16 ResetCamGroupAlivePeriod(eCameraModule cam_module, uint8 tegra)
{
  uint8 current_req_tegra = tegra-1;
   //Dont handle alive messages for a group which is of Nvccp Version 07. Should never reach here for Version 07
  if(nvccpCamGroupVersion[current_req_tegra][cam_module] == PROTOCOL_VERSION_V07)
      return CCP_STATUS_DENIED;
  if(iregistry[current_req_tegra][cam_module] == NOT_REGISTERED)
  {
    return CCP_STATUS_DENIED;
  } else {
#ifdef SHELL_PRINT
    RESOURCE_PRINTF_ALWAYS((" Alive message arrived for Camera Group: %d from Tegra: %d\n",cam_module,current_req_tegra));
#endif
    iTimeStamp[current_req_tegra][cam_module] = 0U;
  }
  return CCP_STATUS_OK;
}

void IncrementCamGroupAlivePeriodAndHandleOwnerships()
{
  uint8 cam_module = CAMERA_MODULE_A;
  uint8 tegraid = 0;
  for(cam_module = CAMERA_MODULE_A ; cam_module < CAMERA_MODULE_MAX; cam_module++)
  {
    for(tegraid = 0 ; tegraid < 3 ; tegraid++)
    {
      //Dont handle alive messages for a group which is of Nvccp Version 07
      if(nvccpCamGroupVersion[tegraid][cam_module] == PROTOCOL_VERSION_V07)
        continue;
      if(iregistry[tegraid][cam_module] != NOT_REGISTERED)
      {
        iTimeStamp[tegraid][cam_module]++;

        if(iTimeStamp[tegraid][cam_module] > CAMGROUP_ALIVE_PERIOD_TIMEOUT)
        {
          /*
          If it comes here..we assume a Tegra crash or nvmedia application crash which owns the camera group
          If this is a false alarm we might have an unexpected camera power off
          */
          RESOURCE_PRINTF_ALWAYS(("Releasing ownership for Camera Group: %d from Tegra %d\n",cam_module,tegraid));
          Resource_GiveawayOwnershipMaster(cam_module,(tegraid+1));
          Resource_GiveawayOwnershipSlave(cam_module,(tegraid+1));
          iTimeStamp[tegraid][cam_module] = 0U;

        }
      }
    }
  }
}

static boolean camera_inc(eCameraModule cam_module)
{
  boolean ret = TRUE;
  RESOURCE_DEBUG_PRINTF(("Resource: master_cnt[%d] = %d\n", cam_module, master_cnt[cam_module]));
  if(master_cnt[cam_module] == 0)
  {
    master_cnt[cam_module] ++;
    ret = TRUE;
  }
  else
  {
    ret = FALSE;
  }
  RESOURCE_DEBUG_PRINTF(("Resource: after turn on action, master_cnt[%d] = %d\n", cam_module, master_cnt[cam_module]));
  return ret;
}

static boolean camera_dec(eCameraModule cam_module)
{
  boolean ret = TRUE;
  RESOURCE_DEBUG_PRINTF(("Resource: master_cnt[%d] = %d\n", cam_module, master_cnt[cam_module]));
  if(master_cnt[cam_module] > 0)
  {
    master_cnt[cam_module] --;
    ret = TRUE;
  }
  else
  {
    ret = FALSE;
  }
  RESOURCE_DEBUG_PRINTF(("Resource: after turn off action, master_cnt[%d] = %d\n", cam_module, master_cnt[cam_module]));
  return ret;
}

static boolean IsCameraLogicPassed(eCameraModule cam_module, eCameraAction action)
{
  boolean ret = TRUE;
  if(action == CAMERA_ON)
  {
    if(!camera_inc(cam_module))
    {
      ret = FALSE;
    }
  }
  else if (action == CAMERA_OFF)
  {
    if(!camera_dec(cam_module))
    {
      ret = FALSE;
    }
  }
  return ret;
}

uint8 Resource_GetOwnershipMaster(eCameraModule cam_module, uint8 tegra)
{
  uint8 current_req_tegra = tegra-1;
  uint8 other_tegra1 = (current_req_tegra==0)?1:0;
  uint8 other_tegra2 = (current_req_tegra==2)?1:2;
  if((iregistry[other_tegra1][cam_module] != REGISTERED_AS_MASTER) && (iregistry[other_tegra2][cam_module] != REGISTERED_AS_MASTER) &&
     (iregistry[current_req_tegra][cam_module] == NOT_REGISTERED))
  {
    //Do the registration
    iregistry[current_req_tegra][cam_module] = REGISTERED_AS_MASTER;
    if(tegra  != (0xF & TEGRA_A))
    {
    if(FALSE == i2c_owner_registered)
    {
      /*Registration of I2C owner among TegraB and TegraC*/
      i2c_owner_registered = tegra;
      /*To Control MSTR_SEL[0] for i2c multiplexing between TegraB and TegraC*/
      Resource_SetCCPI2COwner(tegra);
    }
    else if(tegra != i2c_owner_registered)
    {
      /*Master request from non-I2C owner*/
      return CCP_STATUS_I2C_OWNERSHIP_DENIED;
    }
    }
    return CCP_STATUS_OK;
  }
  else if(iregistry[current_req_tegra][cam_module] == REGISTERED_AS_MASTER)
  {
    if((tegra != i2c_owner_registered) && (tegra  != (0xF & TEGRA_A)))
    {
    /* On Re-request of MasterOwnership from non I2C owner*/
    return CCP_STATUS_I2C_OWNERSHIP_DENIED;
    }
    return CCP_STATUS_OK;
  }
  return CCP_STATUS_DENIED;
}
uint8 Resource_GetOwnershipSlave(eCameraModule cam_module, uint8 tegra)
{
  uint8 current_req_tegra = tegra-1;
  uint8 other_tegra1 = (current_req_tegra==0)?1:0;
  uint8 other_tegra2 = (current_req_tegra==2)?1:2;
  if(((iregistry[other_tegra1][cam_module] != REGISTERED_AS_SLAVE) || (iregistry[other_tegra2][cam_module] != REGISTERED_AS_SLAVE)) &&
    (iregistry[current_req_tegra][cam_module] == NOT_REGISTERED))
  {
    //Do the registration
    iregistry[current_req_tegra][cam_module] = REGISTERED_AS_SLAVE;
    return CCP_STATUS_OK;
  }
  else if(iregistry[current_req_tegra][cam_module] == REGISTERED_AS_SLAVE)
  {
    return CCP_STATUS_OK;
  }
  return CCP_STATUS_DENIED;
}

uint8 Resource_GiveawayOwnershipMaster(eCameraModule cam_module, uint8 tegra)
{
  uint8 current_req_tegra = tegra-1;
  if(iregistry[current_req_tegra][cam_module] != NOT_REGISTERED)
  {
  if(iregistry[current_req_tegra][cam_module] == REGISTERED_AS_MASTER)
  {
    iTimeStamp[current_req_tegra][cam_module] = 0U;
    //Power OFF the Cameras and Aggregators
    RESOURCE_DEBUG_PRINTF(("Calling Resource_PowerOff for Cameras\n"));
    //DEBUG_PRINTF_ALWAYS(("Resource power off \r\n"));
    if (disable_udp_cmd_flag == FALSE)
        Resource_PowerOff(cam_module, 0xF, tegra); //Camera
    RESOURCE_DEBUG_PRINTF(("Calling Resource_PowerOff for Aggre\n"));
    Resource_PowerOff(cam_module, 0x0, tegra); //Aggre


    iregistry[current_req_tegra][cam_module] = NOT_REGISTERED;
    if((tegra != (0xF & TEGRA_A)) && (tegra == i2c_owner_registered))
    {
    if((iregistry[current_req_tegra][GROUP_ID_A] !=  REGISTERED_AS_MASTER) && (iregistry[current_req_tegra][GROUP_ID_B] !=  REGISTERED_AS_MASTER) &&
       (iregistry[current_req_tegra][GROUP_ID_C] !=  REGISTERED_AS_MASTER) && (iregistry[current_req_tegra][GROUP_ID_D] !=  REGISTERED_AS_MASTER))
    {
      /*Release of I2C Ownership State*/
      i2c_owner_registered = FALSE;
    }
    }
    return CCP_STATUS_OK;
  }
  else if(iregistry[current_req_tegra][cam_module] == REGISTERED_AS_SLAVE)
  {
    return CCP_STATUS_DENIED;
  }
  }
  else
  {
  return CCP_STATUS_NOT_REGISTERED;
  }
  return CCP_STATUS_OK;
}

uint8 Resource_GiveawayOwnershipSlave(eCameraModule cam_module, uint8 tegra)
{
  uint8 current_req_tegra = tegra-1;
  if(iregistry[current_req_tegra][cam_module] != NOT_REGISTERED)
  {
    if(iregistry[current_req_tegra][cam_module] == REGISTERED_AS_SLAVE)
    {
      iTimeStamp[current_req_tegra][cam_module] = 0U;
      /*Power OFF the Cameras and Aggregators*/
      Resource_PowerOff(cam_module, 0xF, tegra); /*Camera*/
      Resource_PowerOff(cam_module, 0x0, tegra); /*Aggregator*/

      iregistry[current_req_tegra][cam_module] = NOT_REGISTERED;
      return CCP_STATUS_OK;
    }
    else if(iregistry[current_req_tegra][cam_module] == REGISTERED_AS_MASTER)
    {
      return CCP_STATUS_DENIED;
    }
  }
  else
  {
    return CCP_STATUS_NOT_REGISTERED;
  }
  return CCP_STATUS_OK;
}

uint8 Resource_StateMachine_Aggre(eCameraModule cam_module, uint8 isMaster, uint8 input, uint8 slaveId)
{
  uint8 * current_state = &istate_agg[cam_module];

  RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d Current State is %d\n",cam_module, *current_state));
  switch(*current_state)
  {
  case STATE_0:
    if(input == I_OFF)
    {
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
      return CCP_STATUS_ALREADY_OFF;
    }
    else if(!isMaster && input == I_ON)
    {
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
      return CCP_STATUS_DENIED;
    }
    else if (isMaster && input == I_ON)
    {
      //Do the Power up  of Agggregator
      Resource_SetCameraAggState(cam_module, CAMERA_ON);

      //Move the State
      *current_state = STATE_1;
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
      return CCP_STATUS_OK;
    }
    break;
  case STATE_1:
    if(!isMaster && input == I_OFF)
    {
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
      return CCP_STATUS_ALREADY_OFF;
    }
    else if(isMaster && input == I_OFF)
    {
      //Do the Power Down  of Agggregator
      Resource_SetCameraAggState(cam_module, CAMERA_OFF);

      //Move the State
      *current_state = STATE_0;
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
      return CCP_STATUS_OK;
    }
    else if(!isMaster && input == I_ON)
    {
      //Move the State
      *current_state = STATE_2;
      slaveState_A[cam_module][slaveId] = TRUE;
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
      return CCP_STATUS_OK;
    }
    else if (isMaster && input == I_ON)
    {
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
      return CCP_STATUS_ALREADY_ON;
    }
    break;
  case STATE_2:
    if(!isMaster && input == I_OFF)
    {
      //Move the State
      slaveState_A[cam_module][slaveId] = FALSE;
      if(slaveState_A[cam_module][0] == slaveState_A[cam_module][1])
      {
      *current_state = STATE_1;
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
      return CCP_STATUS_OK;
    }
      return CCP_STATUS_SLAVE_RUNNING;
    }
    else if(isMaster && input == I_OFF)
    {
      //Move the State
      *current_state = STATE_3;
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
      return CCP_STATUS_OK;
    }
    else if(!isMaster && input == I_ON)
    {
      if(slaveState_A[cam_module][slaveId] == FALSE)
      {
        slaveState_A[cam_module][slaveId] = TRUE;
        return CCP_STATUS_OK;
      }
      else
      {
        return CCP_STATUS_ALREADY_ON;
      }
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
    }
    else if (isMaster && input == I_ON)
    {
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
      return CCP_STATUS_ALREADY_ON;
    }
    break;
  case STATE_3:
    if(!isMaster && input == I_OFF)
    {
      slaveState_A[cam_module][slaveId] = FALSE;
      if(slaveState_A[cam_module][0] == slaveState_A[cam_module][1])
      {
      //Do the Power Down  of Agggregator
      Resource_SetCameraAggState(cam_module, CAMERA_OFF);

      //Move the State
      *current_state = STATE_0;
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
      return CCP_STATUS_OK;
      }
      return CCP_STATUS_SLAVE_RUNNING;
    }
    else if(isMaster && input == I_OFF)
    {
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
      return CCP_STATUS_ALREADY_OFF;
    }
    else if(!isMaster && input == I_ON)
    {
      if(slaveState_A[cam_module][slaveId] == FALSE)
      {
        slaveState_A[cam_module][slaveId] = TRUE;
        return CCP_STATUS_OK;
      }
      else
      {
        return CCP_STATUS_ALREADY_ON;
      }
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
    }
    else if (isMaster && input == I_ON)
    {
      //Move the State
      *current_state = STATE_2;
      RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
      return CCP_STATUS_SLAVE_RUNNING;
    }
    break;
  default:
    RESOURCE_DEBUG_PRINTF(("Aggre Unit: %d New State is %d\n",cam_module, *current_state));
    return CCP_STATUS_DENIED;
    break;
  }

}

uint8 Resource_StateMachine_Camera(eCameraModule cam_module, uint8 cam_unit, uint8 isMaster, uint8 input, uint8 slaveId)
{
  uint8 * current_state = &istate_cam[cam_module*4+cam_unit];
  cam_unit = cam_module*4+cam_unit;

  //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and cam_module is %d\n", cam_module));
  //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and tegra is %d\n", tegra));
  //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and cam_unit is %d\n", cam_unit));
  //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and current stat is %d\n", *current_state));
  RESOURCE_DEBUG_PRINTF(("Camera Unit: %d Current State is %d\n",cam_unit, *current_state));

  switch(*current_state)
  {
  case STATE_0:
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and yes current stat is %d\n", STATE_0));
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and isMaster is %d\n", isMaster));
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and input is %d\n", input));

    if(input == I_OFF)
    {
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
      return CCP_STATUS_ALREADY_OFF;
    }
    else if(!isMaster && input == I_ON)
    {
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
      return CCP_STATUS_DENIED;
    }
    else if (isMaster && input == I_ON)
    {
      //Do the Power up  of Camera
      Resource_SetCameraState(cam_unit, CAMERA_ON);

      //Move the State
      *current_state = STATE_1;
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
      return CCP_STATUS_OK;
    }
    break;
  case STATE_1:
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and yes current stat is %d\n", STATE_1));
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and isMaster is %d\n", isMaster));
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and input is %d\n", input));
    if(!isMaster && input == I_OFF)
    {
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
      return CCP_STATUS_ALREADY_OFF;
    }
    else if(isMaster && input == I_OFF)
    {
      //Do the Power Down  of Camera
      Resource_SetCameraState(cam_unit, CAMERA_OFF);

      //Move the State
      *current_state = STATE_0;
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
      return CCP_STATUS_OK;
    }
    else if(!isMaster && input == I_ON)
    {
      //Move the State
      slaveState_C[cam_unit][slaveId]=TRUE;
      *current_state = STATE_2;
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
      return CCP_STATUS_OK;
    }
    else if (isMaster && input == I_ON)
    {
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
      return CCP_STATUS_ALREADY_ON;
    }
    break;
  case STATE_2:
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and yes current stat is %d\n", STATE_2));
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and isMaster is %d\n", isMaster));
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and input is %d\n", input));
    if(!isMaster && input == I_OFF)
    {
      //Move the State
      slaveState_C[cam_unit][slaveId]=FALSE;
      if(slaveState_C[cam_unit][0] == slaveState_C[cam_unit][1])
      {
      *current_state = STATE_1;
      return CCP_STATUS_OK;
      }
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
      return CCP_STATUS_SLAVE_RUNNING;
    }
    else if(isMaster && input == I_OFF)
    {
      //Move the State
      *current_state = STATE_3;
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
      return CCP_STATUS_OK;
    }
    else if(!isMaster && input == I_ON)
    {
      if(slaveState_C[cam_unit][slaveId] == FALSE)
      {
        slaveState_C[cam_unit][slaveId] = TRUE;
        return CCP_STATUS_OK;
      }
      else
      {
        return CCP_STATUS_ALREADY_ON;
      }
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
    }
    else if (isMaster && input == I_ON)
    {
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
      return CCP_STATUS_ALREADY_ON;
    }
    break;
  case STATE_3:
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and yes current stat is %d\n", STATE_3));
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and isMaster is %d\n", isMaster));
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and input is %d\n", input));
    if(!isMaster && input == I_OFF)
    {
      slaveState_C[cam_unit][slaveId] = FALSE;
      if(slaveState_C[cam_unit][0] == slaveState_C[cam_unit][1])
      {
      //Do the Power Down  of Camera
      Resource_SetCameraState(cam_unit,  CAMERA_OFF);

      //Move the State
      *current_state = STATE_0;
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
      return CCP_STATUS_OK;
      }
      return CCP_STATUS_SLAVE_RUNNING;
    }
    else if(isMaster && input == I_OFF)
    {
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
      return CCP_STATUS_ALREADY_OFF;
    }
    else if(!isMaster && input == I_ON)
    {
      if(slaveState_C[cam_unit][slaveId] == FALSE)
      {
        slaveState_C[cam_unit][slaveId] = TRUE;
        return CCP_STATUS_OK;
      }
      else
      {
        return CCP_STATUS_ALREADY_ON;
      }
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
    }
    else if (isMaster && input == I_ON)
    {
      //Move the State
      *current_state = STATE_2;
      RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
      return CCP_STATUS_SLAVE_RUNNING;
    }
    break;
  default:
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and yes current stat is %d\n", 255));
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and isMaster is %d\n", isMaster));
    //RESOURCE_PRINTF_ALWAYS(("Entered  Resource_StateMachine_Camera .. and input is %d\n", input));
    RESOURCE_DEBUG_PRINTF(("Camera Unit: %d New State is %d\n",cam_unit, *current_state));
    return CCP_STATUS_DENIED;
    break;
  }

}

uint8 Resource_StateMachine_Cameras(eCameraModule cam_module, uint8 cam_units, uint8 isMaster, uint8 input, uint8 slaveId)
{
  uint8 i;
  ccp_return_status ret_status = CCP_STATUS_DENIED;

  //RESOURCE_PRINTF_ALWAYS(("Entered Resource_StateMachine_Cameras "));
  for(i = 0; i < 4; i++)
    {

      if(cam_units&(1<<i))
        {
          //RESOURCE_PRINTF_ALWAYS(("Calling  Resource_StateMachine_Camera ..\n"));
      ret_status = Resource_StateMachine_Camera(cam_module, i, isMaster, input, slaveId);
          //RESOURCE_PRINTF_ALWAYS(("Resource_StateMachine_Camera return %d \n", ret_status));
        }
    }
  //RESOURCE_PRINTF_ALWAYS(("Calling  Resource_StateMachine_Camera ..\n"));
  return ret_status;
}

uint8 Resource_PowerOn(eCameraModule cam_module, uint8 cam_units, uint8 tegra)
{
  uint8 current_req_tegra = tegra-1;
  uint8 slaveId = 0;
  ccp_return_status ret_value = CCP_STATUS_OK;

  if(iregistry[current_req_tegra][cam_module] != NOT_REGISTERED)
  {
    uint8 isMaster = (iregistry[current_req_tegra][cam_module] == REGISTERED_AS_MASTER);
    if(!isMaster)
    {
      if(current_req_tegra == 1U)
      {
        slaveId = (iregistry[current_req_tegra + 1][cam_module] == REGISTERED_AS_MASTER);
      }
      else if((current_req_tegra == 2U))
      {
        slaveId = 1U;
      }
    }
    if(cam_units == 0x0)
    {
      //Then request is for Aggregator Only
      return Resource_StateMachine_Aggre(cam_module, isMaster, I_ON, slaveId);
    }
    else
    {

      /*Check if Aggregator is already Powered ON if not send status CCP_STATUS_DENIED*/
      if(cam_agg_state[cam_module] == CAMERA_ON )
      {
        return Resource_StateMachine_Cameras(cam_module, cam_units, isMaster, I_ON, slaveId);
      }
      else
      {
        return CCP_STATUS_DENIED;
      }
    }
  }
  else
  {
      return CCP_STATUS_NOT_REGISTERED;
  }
}

uint8 Resource_PowerOff(eCameraModule cam_module, uint8 cam_units, uint8 tegra)
{
  uint8 current_req_tegra = tegra-1;
  ccp_return_status ret_value = CCP_STATUS_OK;
  uint8 status;
  uint8 slaveId = 0;


  if(iregistry[current_req_tegra][cam_module] != NOT_REGISTERED)
  {
    //RESOURCE_PRINTF_ALWAYS(("Registered \n"));

    uint8 isMaster = (iregistry[current_req_tegra][cam_module] == REGISTERED_AS_MASTER);
    if(!isMaster)
    {
      if(current_req_tegra == 1U)
      {
        slaveId = (iregistry[current_req_tegra + 1][cam_module] == REGISTERED_AS_MASTER);
      }
      else if((current_req_tegra == 2U))
      {
        slaveId = 1U;
      }
    }
    if(cam_units == 0x0)
    {
      //RESOURCE_PRINTF_ALWAYS(("Calling  Resource_StateMachine_Aggre ..\n"));
      //Then request is for Aggregator Only
      status = Resource_StateMachine_Aggre(cam_module, isMaster, I_OFF, slaveId );
      //RESOURCE_PRINTF_ALWAYS(("Func  Resource_StateMachine_Aggre returned ..%d\n", status));
      return status;
    }
    else
    {
      //RESOURCE_PRINTF_ALWAYS(("Calling  Resource_StateMachine_Cameras ..\n"));

      //The request is for Cameras
      status =  Resource_StateMachine_Cameras(cam_module, cam_units, isMaster, I_OFF, slaveId);
      //RESOURCE_PRINTF_ALWAYS(("Func  Resource_StateMachine_Cameras returned ..%d\n", status));
      return status;
    }
  }
  else
  {
      return CCP_STATUS_NOT_REGISTERED;
  }

}

boolean Resource_SetCameraState(eCameraUnit cam_unit, eCameraAction action)
{
  t_i2c_device* dev_ptr;
  uint8 camLswChannel;
  uint8 cam_module;
  char camera_str[20];
  boolean ret = TRUE;

  if(cam_unit >= CAMERA_UNIT_MAX)
  {
    RESOURCE_PRINTF_ALWAYS(("Resource: unknown camera unit %d\n", cam_unit));
    return FALSE;
  }

  if(cam_unit >= CAMERA_UNIT_A0 && cam_unit <= CAMERA_UNIT_A3)
  {
    cam_module = CAMERA_MODULE_A;
  }
  else if(cam_unit >= CAMERA_UNIT_B0 && cam_unit <= CAMERA_UNIT_B3)
  {
    cam_module = CAMERA_MODULE_B;
  }
  else if(cam_unit >= CAMERA_UNIT_C0 && cam_unit <= CAMERA_UNIT_C3)
  {
    cam_module = CAMERA_MODULE_C;
  }
#if CAMERA_D_ENABLE == 1
  else if(cam_unit >= CAMERA_UNIT_D0 && cam_unit <= CAMERA_UNIT_D3)
  {
    cam_module = CAMERA_MODULE_D;
  }
#endif

#if CAMERA_CIM_ENABLE == 1
  switch(cam_module)
  {
#if CAMERA_A_ENABLE == 1
  case CAMERA_MODULE_A:
    //camLswAddr = CIM_LSW_SVC_I2C_ADDR;
	dev_ptr = &i2c_cim_lsw_svc;
    strcpy(camera_str, "CAMERA_A: SVC");
    break;
#endif
#if CAMERA_B_ENABLE == 1
    case CAMERA_MODULE_B:
    //camLswAddr = CIM_LSW_CVC_I2C_ADDR;
    dev_ptr = &i2c_cim_lsw_cvc;
    strcpy(camera_str, "CAMERA_B: CVC");
    break;
#endif
#if CAMERA_C_ENABLE == 1
  case CAMERA_MODULE_C:
    //camLswAddr = CIM_LSW_LRC_MRR_I2C_ADDR;
	dev_ptr = &i2c_cim_lsw_lrc_mrr;
    strcpy(camera_str, "CAMERA_C: LRC_MRR");
    break;
#endif
#if CAMERA_D_ENABLE == 1
  case CAMERA_MODULE_D:
    //camLswAddr = CIM_LSW_LRR_ADDR;
    dev_ptr = &i2c_cim_lsw_lrr;
    strcpy(camera_str, "CAMERA_D: LRR");
    break;
#endif
#if CAMERA_E_ENABLE == 1
  case CAMERA_MODULE_E:
    strcpy(camera_str, "CAMERA_E");
    break;
#endif
  default:
    RESOURCE_PRINTF_ALWAYS(("Resource: unknown camera module %d\n", cam_module));
    /* leave the function */
    return FALSE;
    break;
  }

  camLswChannel = cam_unit;
  camLswChannel = camLswChannel % CAMERA_UNITS_PER_MODULE;
  camLswChannel = 1 << camLswChannel;

  switch(action)
  {
  case CAMERA_ON:
    ret = CIM_LSW_Control(dev_ptr, camLswChannel, TRUE);
    if(ret)
    {
      RESOURCE_DEBUG_PRINTF(("Resource: Turn on %s\n", camera_str));
    }
    else
    {
      RESOURCE_PRINTF_ALWAYS(("Resource: Fail to turn on %s\n", camera_str));
      return FALSE;
    }
    break;
  case CAMERA_OFF:
    ret = CIM_LSW_Control(dev_ptr, camLswChannel, FALSE);
    if(ret)
    {
      RESOURCE_DEBUG_PRINTF(("Resource: Turn off %s\n", camera_str));
    }
    else
    {
      RESOURCE_PRINTF_ALWAYS(("Resource: Fail to turn off %s\n", camera_str));
      return FALSE;
    }
    break;
  default:
    RESOURCE_DEBUG_PRINTF(("Resource: unknown camera power command %d\n", action));
    /* leave the function */
    return FALSE;
    break;
  }
#endif //CAMERA_CIM_ENABLE == 1
  cam_unit_state[cam_unit] = action;
  return ret;
}


uint8 Resource_GetCameraState(eCameraUnit cam_unit)
{
  return cam_unit_state[cam_unit];
}

boolean Resource_SetCameraAggState(eCameraModule cam_module, eCameraAction action)
{
  IoExpPort_Pin pPort;
  char camera_str[20];
  boolean gpio_ret = TRUE;
  if(cam_module >= CAMERA_MODULE_MAX)
  {
    RESOURCE_PRINTF_ALWAYS(("Resource: unknown camera module %d\n", cam_module));
    return FALSE;
  }

  switch(cam_module)
  {
#if CAMERA_A_ENABLE == 1
  case CAMERA_MODULE_A:
    pPort = (SV_PWRDN_N);
    strcpy(camera_str, "CAMERA_A: SVC");
    break;
#endif
#if CAMERA_B_ENABLE == 1
  case CAMERA_MODULE_B:
    pPort = (CV_PWRDN_N);
    strcpy(camera_str, "CAMERA_B: CVC");
    break;
#endif
#if CAMERA_C_ENABLE == 1
  case CAMERA_MODULE_C:
    pPort = (LRC_PWRDN_N);
    strcpy(camera_str, "CAMERA_C: LRC_MRR");
    break;
#endif
#if CAMERA_D_ENABLE == 1
  case CAMERA_MODULE_D:
    pPort = (LRR_PWRDN_N);
    strcpy(camera_str, "CAMERA_D: LRR");
    break;
#endif
#if CAMERA_E_ENABLE == 1
  case CAMERA_MODULE_E:
    strcpy(camera_str, "CAMERA_E");
    break;
#endif
  default:
    RESOURCE_PRINTF_ALWAYS(("Resource: unknown camera module %d\n", cam_module));
    /* leave the function */
    return FALSE;
    break;
  }
  switch(action)
  {
  case CAMERA_ON:
    SET_GPIOEXP_OUTPUT_HIGH(pPort);
    SET_GPIOEXP_CONFIG_OUTPUT(pPort);
    gpio_ret = READ_GPIOEXP_STATE(pPort);
    /* if read back high (true) */
    if(gpio_ret)
    {
      RESOURCE_DEBUG_PRINTF(("Resource: Release aggregator reset%s\n", camera_str));
    }
    else
    {
      RESOURCE_PRINTF_ALWAYS(("Resource: Fail to release aggregator reset %s\n", camera_str));
      return FALSE;
    }
    break;
  case CAMERA_OFF:
    SET_GPIOEXP_OUTPUT_LOW(pPort);
    SET_GPIOEXP_CONFIG_OUTPUT(pPort);
    gpio_ret = READ_GPIOEXP_STATE(pPort);
    /* if read back low (false) */
    if(!gpio_ret)
    {
      RESOURCE_DEBUG_PRINTF(("Resource: Reset aggregator %s\n", camera_str));
    }
    else
    {
      RESOURCE_PRINTF_ALWAYS(("Resource: Fail to reset aggregator %s\n", camera_str));
      return FALSE;
    }
    break;
  default:
    RESOURCE_DEBUG_PRINTF(("Resource: unknown aggregator power command %d\n", action));
    /* leave the function */
    return FALSE;
    break;
  }
  cam_agg_state[cam_module] = action;
  return TRUE;
}


uint8 Resource_GetCameraAggState(eCameraModule cam_module)
{
  return cam_agg_state[cam_module];
}

boolean Resource_SetFsyncSrcState(uint8 tegra)
{
  boolean ret = FALSE;
  if(((Resource_GetFsyncEnState(CAMERA_MODULE_A)) == FRSYNC_OFF) && ((Resource_GetFsyncEnState(CAMERA_MODULE_B)) == FRSYNC_OFF)  &&\
      ((Resource_GetFsyncEnState(CAMERA_MODULE_C)) == FRSYNC_OFF) && ((Resource_GetFsyncEnState(CAMERA_MODULE_D)) == FRSYNC_OFF))
  {

  if(tegra == TEGRA_A)
  {
    SET_GPIOEXP_OUTPUT_LOW(FRSYNC_SEL);
    SET_GPIOEXP_CONFIG_OUTPUT(FRSYNC_SEL);
    fsync_src = tegra;
    RESOURCE_DEBUG_PRINTF(("Resource: Set FRSYNC_SRC source to Tegra A\n\r"));
    ret = TRUE;
  }
  else if(tegra == TEGRA_B)
  {
    SET_GPIOEXP_OUTPUT_HIGH(FRSYNC_SEL);
    SET_GPIOEXP_CONFIG_OUTPUT(FRSYNC_SEL);
    SET_GPIOEXP_OUTPUT_LOW(TEGRA_SEL);
    SET_GPIOEXP_CONFIG_OUTPUT(TEGRA_SEL);
    fsync_src = tegra;
    RESOURCE_DEBUG_PRINTF(("Resource: Set FRSYNC_SRC source to Tegra B\n\r"));
    ret = TRUE;
  }
  else if(tegra == TEGRA_C)
  {
    SET_GPIOEXP_OUTPUT_HIGH(FRSYNC_SEL);
    SET_GPIOEXP_CONFIG_OUTPUT(FRSYNC_SEL);
    SET_GPIOEXP_OUTPUT_HIGH(TEGRA_SEL);
    SET_GPIOEXP_CONFIG_OUTPUT(TEGRA_SEL);
    fsync_src = tegra;
    RESOURCE_DEBUG_PRINTF(("Resource: Set FRSYNC_SRC source to Tegra C\n\r"));
    ret = TRUE;
  }
  else
  {
    RESOURCE_DEBUG_PRINTF(("Resource: Invalid tegra source for FSYNC_SRC\n\r", tegra));
  }
  }
  return ret;

}

uint8 Resource_GetFsyncSrcState()
{
  return fsync_src;
}


boolean Resource_SetFsyncEnState(eCameraModule cam_module, eCamFrsyncAction action)
{
  boolean ret = FALSE;
  char camera_str[20];
  IoExpPort_Pin pPort;
  if((cam_module >> 4) >= CAMERA_MODULE_MAX)
  {
    RESOURCE_PRINTF_ALWAYS(("Resource: unknown camera module %d\n", cam_module));
    return FALSE;
  }
  if((Resource_GetFsyncSrcState() & 0x0F)==(cam_module & 0x0F))
  {
  switch((cam_module >> 4))
  {
#if CAMERA_A_ENABLE == 1
  case CAMERA_MODULE_A:
    pPort = SV_FRSYNC_EN_N;
    strcpy(camera_str, "CAMERA_A: SVC");
    break;
#endif
#if CAMERA_B_ENABLE == 1
  case CAMERA_MODULE_B:
    pPort = CV_FRSYNC_EN_N;
    strcpy(camera_str, "CAMERA_B: CVC");
    break;
#endif
#if CAMERA_C_ENABLE == 1
  case CAMERA_MODULE_C:
    pPort = LRC_FRSYNC_EN_N;
    strcpy(camera_str, "CAMERA_C: LRC_MRR");
    break;
#endif
#if CAMERA_D_ENABLE == 1
  case CAMERA_MODULE_D:
    pPort = LRR_FRSYNC_EN_N;
    strcpy(camera_str, "CAMERA_D: LRR");
    break;
#endif
  default:
//    RESOURCE_PRINTF_ALWAYS(("Resource: unknown camera module %d\n", cam_module));
    return FALSE;
    break;
  }
  switch(action)
  {
  case FRSYNC_ON:
    SET_GPIOEXP_OUTPUT_LOW(pPort);
    SET_GPIOEXP_CONFIG_OUTPUT(pPort);
//    RESOURCE_DEBUG_PRINTF(("Resource: Enable Frame Sync for %s\n", camera_str));
    ret = TRUE;
    break;
  case FRSYNC_OFF:
    SET_GPIOEXP_OUTPUT_HIGH(pPort);
    SET_GPIOEXP_CONFIG_OUTPUT(pPort);
//    RESOURCE_DEBUG_PRINTF(("Resource: Disable Frame Sync for %s\n", camera_str));
    ret = TRUE;
    break;
  default:
//    RESOURCE_DEBUG_PRINTF(("Resource: unknown camera frame sync argument %d\n", action));
    return FALSE;
    break;
  }
    cam_frsync_state[(cam_module >> 4)] = action;
  }
  return ret;
}

uint8 Resource_GetFsyncEnState(eCameraModule cam_module)
{
  return cam_frsync_state[cam_module];
}

/* FAN state */
boolean Resource_InitFanState(uint8 state)
{
  boolean ret = FALSE;
  if(Resource_SetFanState(state))
  {
#if FAN_A_ENABLE == 1
    IfxPort_setPinModeOutput(AURIX_X1_FANCTL.port, AURIX_X1_FANCTL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
#if FAN_B_ENABLE == 1
    IfxPort_setPinModeOutput(AURIX_X2_FANCTL.port, AURIX_X2_FANCTL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
    ret = TRUE;
  }
  return ret;
}

boolean Resource_SetFanState(uint8 state)
{
  boolean ret = FALSE;
  if(state == GOOD_STATE)
  {
#if FAN_A_ENABLE == 1
    IfxPort_setPinLow(AURIX_X1_FANCTL.port, AURIX_X1_FANCTL.pinIndex);
#endif
#if FAN_B_ENABLE == 1
    IfxPort_setPinLow(AURIX_X2_FANCTL.port, AURIX_X2_FANCTL.pinIndex);
#endif
    fan_state = state;
    RESOURCE_DEBUG_PRINTF(("Resource: Fan in good state, set them to be controlled by Tegra \n"));
    ret = TRUE;
  }
  else if(state == BAD_STATE)
  {
#if FAN_A_ENABLE == 1
    IfxPort_setPinHigh(AURIX_X1_FANCTL.port, AURIX_X1_FANCTL.pinIndex);
#endif
#if FAN_B_ENABLE == 1
    IfxPort_setPinHigh(AURIX_X2_FANCTL.port, AURIX_X2_FANCTL.pinIndex);
#endif
    fan_state = state;
    RESOURCE_DEBUG_PRINTF(("Resource: Fan in bad state, set them to Maxim Speed\n"));
    ret = TRUE;
  }
  else
  {
    RESOURCE_DEBUG_PRINTF(("Resource: Invalid state for FAN\n", state));
  }
  return ret;
}

uint8 Resource_GetFanState()
{
  return fan_state;
}

/*
 * Function to set IO state to for multiplexing of I2C lines of TegraB and TegraC
 */
void Resource_SetCCPI2COwner(uint8 tegra)
{
  if((TEGRA_B & 0x0F) == tegra)
  {
    SET_GPIOEXP_OUTPUT_LOW(TEGRA_SEL);
    SET_GPIOEXP_CONFIG_OUTPUT(TEGRA_SEL);
  }
  else if((TEGRA_C & 0x0F) == tegra)
  {
    SET_GPIOEXP_OUTPUT_HIGH(TEGRA_SEL);
    SET_GPIOEXP_CONFIG_OUTPUT(TEGRA_SEL);
  }
  else
  {

  }
}

#endif //#if (PROJECT == PROJECT_DRIVE_AX) && (DRIVE_AX_B0x == 1)

