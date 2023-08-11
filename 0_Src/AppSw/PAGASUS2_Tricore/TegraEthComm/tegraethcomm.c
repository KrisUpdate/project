/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "tegraethcomm.h"
#include "Configuration.h"
#include <internal_fw_debug.h>
#include "EthernetCmds.h"
#include "udpapp.h"
#include "ethernet.h"
#include "protocol.h"
#include "AsclinShellInterface.h"
#include "resource_state_manage.h"
#include "CIM.h"
#include "resource.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define TEGRA_ETH_COMM_DEBUG_ENABLE 1

#if TEGRA_ETH_COMM_DEBUG_ENABLE == 1
#define COMM_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define COMM_DEBUG_PRINTF(x)
#endif

#define COMM_PRINTF_ALWAYS(x) DEBUG_PRINTF_ALWAYS(x)

#define HEART_BEAT_TIME_OUT_S    10
#define HEART_BEAT_TIME_OUT_MS   (HEART_BEAT_TIME_OUT_S * 1000)
#define TIME_TICK_MS             100

/* set 1 to enable separate Tegra detection, need protocol update */
#define TEGRA_SEPERATE_ENABLE    0

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
#if ISR_PROVIDER_TEGRA_ETH_COMM == 0
    #if defined(__GNUC__)
    #pragma section ".bss_cpu0" awc0
    #endif
    #if defined(__TASKING__)
    #pragma section farbss "bss_cpu0"
    #pragma section fardata "data_cpu0"
    #endif
    #if defined(__DCC__)
    #pragma section DATA ".data_cpu0" ".bss_cpu0" far-absolute RW
    #endif
#elif ISR_PROVIDER_TEGRA_ETH_COMM == 1
    #if defined(__GNUC__)
    #pragma section ".bss_cpu1" awc1
    #endif
    #if defined(__TASKING__)
    #pragma section farbss "bss_cpu1"
    #pragma section fardata "data_cpu1"
    #endif
    #if defined(__DCC__)
    #pragma section DATA ".data_cpu1" ".bss_cpu1" far-absolute RW
    #endif
#elif ISR_PROVIDER_TEGRA_ETH_COMM == 2
    #if defined(__GNUC__)
    #pragma section ".bss_cpu2" awc2
    #endif
    #if defined(__TASKING__)
    #pragma section farbss "bss_cpu2"
    #pragma section fardata "data_cpu2"
    #endif
    #if defined(__DCC__)
    #pragma section DATA ".data_cpu2" ".bss_cpu2" far-absolute RW
    #endif
#else
#error "Set ISR_PROVIDER_TEGRA_ETH_COMM to a valid value!"
#endif

#if defined(__GNUC__)
#pragma section
#endif
#if defined(__TASKING__)
#pragma section farbss restore
#pragma section fardata restore
#endif
#if defined(__DCC__)
#pragma section DATA RW
#endif

extern disable_udp_cmd_flag;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
static void TegraEthComm_UpdateStatus_V06(tPacket* pCommand, tPacket *spCommand);
static uint8 TegraEthComm_ExecuteCmd_V06(tPacket* pCommand, tPacket *spCommand);
static void TegraEthComm_UpdateStatus(tPacket* status);
static uint8 TegraEthComm_ExecuteCmd(tPacket* pCommand);
static void TegraEthComm_Receive(uint8* pData, uint8 len);
static void TegraEthComm_HandleCmd(tPacket* packet);
static char* get_command_string(uint8 cmd);
/* heart beat chekcing fucntion */
static void HeartBeatResetCnt(uint8 tegra);
static void HeartBeatTimeOut(uint8 tegra);
static void HeartBeatFeed(uint8 tegra);
static void HeartBeatDetection(void);
/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
static boolean isEthEnable = FALSE;
static boolean isResume = FALSE;

#define TEGRA_BOTH    0xFF

typedef struct
{
    boolean isRunning;
#if TEGRA_SEPERATE_ENABLE==1
    uint32  cnt_a;
    uint32  cnt_b;
#else
    uint32  cnt_both;
#endif
}t_heart_beat_detect;

static t_heart_beat_detect heart_beat =
{
        .isRunning = FALSE,
#if TEGRA_SEPERATE_ENABLE==1
        .cnt_a = 0,
        .cnt_b = 0,
#else
        .cnt_both = 0,
#endif
};
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
#if 1
static uint8 isV6SessionStarted[3] = {0, 0};


void TegraEthComm_init(void)
{
    COMM_DEBUG_PRINTF(("Initializing the ethernet communication between Tegra and Aurix\r\n"));
    isEthEnable = TRUE;
}


void TegraEthComm_resume(void)
{
    COMM_DEBUG_PRINTF(("Resuming the ethernet communication between Tegra and Aurix\r\n"));
    isResume = TRUE;
}


void TegraEthComm_routine(void)
{
    if(!isEthEnable)
        return;
    static boolean initial = FALSE;
    if((!initial) && isResume)
    {
        COMM_PRINTF_ALWAYS(("TegraEthComm: it may initial the UDP again, please report it to MCU owner\n\r"));
    }
    if(!initial)
    {
         /* Checking the dhcp ip status and start the UDP after it's ready */
        if(Ethernet_Is_Ip_Bound())
        {
            uint32 ip = 0;
            if(Ethernet_get_ip(&ip))
            {
                COMM_DEBUG_PRINTF(("TegraEthComm: Get ip address is 0x%x\n\r", ip));
            }
            else
            {
                COMM_DEBUG_PRINTF(("TegraEthComm: Fail to get the bound ip\n\r"));
            }
            initial = TRUE;
            udp_app_init(ip);
            udp_app_register_rec_cb(TegraEthComm_Receive);
            COMM_DEBUG_PRINTF(("Tegra<->Aurix ethernet communication is ready\n\r"));
        }
    }
    /* we don't need initial again if it's a resume process */
    if(isResume)
    {
        if(Ethernet_Is_Ip_Bound())
        {
            COMM_DEBUG_PRINTF(("Tegra<->Aurix ethernet communication is ready\n\r"));
            isResume = FALSE;
        }
    }
    if(heart_beat.isRunning)
    {
        HeartBeatDetection();
    }

    IncrementCamGroupAlivePeriodAndHandleOwnerships();
}
#endif

void TegraEthComm_EnableHeartBeatDetect(void)
{
    heart_beat.isRunning = TRUE;
#if TEGRA_SEPERATE_ENABLE==1
    HeartBeatResetCnt(TEGRA_A);
    HeartBeatResetCnt(TEGRA_B);
#else
    HeartBeatResetCnt(TEGRA_BOTH);
#endif
}

void TegraEthComm_DisableHeartBeatDetect(void)
{
    heart_beat.isRunning = FALSE;
}


#if 1
static void HeartBeatResetCnt(uint8 tegra)
{
    if(heart_beat.isRunning)
    {
        switch(tegra)
        {
#if TEGRA_SEPERATE_ENABLE==1
        case TEGRA_A:
            heart_beat.cnt_a = HEART_BEAT_TIME_OUT_MS / TIME_TICK_MS;
            break;
        case TEGRA_B:
            heart_beat.cnt_b = HEART_BEAT_TIME_OUT_MS / TIME_TICK_MS;
            break;
#else
        case TEGRA_BOTH:
            heart_beat.cnt_both = HEART_BEAT_TIME_OUT_MS / TIME_TICK_MS;
            break;
#endif
        default:
            break;
        }
    }
}


static void HeartBeatTimeOut(uint8 tegra)
{
    switch(tegra)
    {
#if TEGRA_SEPERATE_ENABLE==1
    case TEGRA_A:
        COMM_PRINTF_ALWAYS(("Tegra A time out happen!\n\r"));
        /* set FAN to max speed */
        Resource_SetFanState(BAD_STATE);
        break;
    case TEGRA_B:
        COMM_PRINTF_ALWAYS(("Tegra B time out happen!\n\r"));
        /* set FAN to max speed */
        Resource_SetFanState(BAD_STATE);
        break;
#else
    case TEGRA_BOTH:
        COMM_PRINTF_ALWAYS(("Tegra time out happen! Set Fan to Maxim Speed\n\r"));
        /* set FAN to max speed */
        Resource_SetFanState(BAD_STATE);
        break;
#endif
    default:
        break;
    }
}


static void HeartBeatFeed(uint8 tegra)
{
    COMM_DEBUG_PRINTF(("Enter %s\n\r", __func__));
    HeartBeatResetCnt(tegra);
    switch(tegra)
    {
#if TEGRA_SEPERATE_ENABLE==1
    case TEGRA_A:
        COMM_DEBUG_PRINTF(("Receive Heart Beat from Tegra A\n\r"));
        break;
    case TEGRA_B:
        COMM_DEBUG_PRINTF(("Receive Heart Beat from Tegra B\n\r"));
        break;
#else
    case TEGRA_BOTH:
        COMM_DEBUG_PRINTF(("Receive Heart Beat from Tegra\n\r"));
        break;
#endif
    default:
        break;
    }
}


static void HeartBeatDetection(void)
{
#if TEGRA_SEPERATE_ENABLE==1
    heart_beat.cnt_a--;
    if(heart_beat.cnt_a==0)
    {
        HeartBeatTimeOut(TEGRA_A);
        HeartBeatResetCnt(TEGRA_A);
    }

    heart_beat.cnt_b--;
    if(heart_beat.cnt_b==0)
    {
        HeartBeatTimeOut(TEGRA_B);
        HeartBeatResetCnt(TEGRA_B);
    }
#else
    heart_beat.cnt_both--;
    if(heart_beat.cnt_both==0)
    {
        HeartBeatTimeOut(TEGRA_BOTH);
        HeartBeatResetCnt(TEGRA_BOTH);
    }
#endif
}


static void TegraEthComm_UpdateStatus_V06(tPacket* cmd, tPacket* status)
{
    int i;
    status->v6.aggreg_fsync_status = 0;
    status->v6.power_status_0 = 0;
    status->v6.power_status_1 = 0;
    //Aggre State
    for(i = 0; i < CAMERA_MODULE_MAX; i++) //TBC: Avoid Hardcoding
    {
        uint8 flag = (Resource_GetCameraAggState(i) == CAMERA_ON);
        status->v6.aggreg_fsync_status |= (flag << i) << 4;
    }

    //Fsync_enable
    for(i = 0; i < CAMERA_MODULE_MAX; i++) //TBC: Avoid Hardcoding
    {
        uint8 flag = (Resource_GetFsyncEnState(i) == FRSYNC_ON);
        status->v6.aggreg_fsync_status |= (flag << i);
    }

    //Camera power
    for(i = 0; i < CAMERA_UNIT_MAX; i++) //TBC: Avoid Hardcoding
    {
        if(i < 8)
        {
            uint8 flag = (Resource_GetCameraState(i) == CAMERA_ON);
            status->v6.power_status_0 |= (flag << i);
        }
        else
        {
            uint8 flag = (Resource_GetCameraState(i) == CAMERA_ON);
            status->v6.power_status_1 |= (flag << (i-8));
        }
    }

    //Frsync Owner
    status->v6.cam_frsync_owner = (Resource_GetFsyncSrcState() & 0xF);

    //Fan status
    status->fan_state = Resource_GetFanState();

    //Need to check with Johnny - How we are sending version for OTA when
    /* we should return the Major version number,
     * but historical issue DPX2 doesn't have major version number */
#if PROJECT == PROJECT_DRIVE_PX2
    status->aurix_version = SW_REVISION;
#else
#if ETHERNET_OTA_FW_TYPE == _OTA_PROD_FW
    status->aurix_version = SW_MAJOR;
#if PROJECT == PROJECT_PAGASUS2
#else
    status->aurix_version = UPDATE_SW_MAJOR;
#endif
#endif
#endif
    status->fan_state = Resource_GetFanState();;
    status->cam_master = INGORE;

}


boolean HandleCamGroupAliveMessages(tPacket* pCommand,tPacket *spCommand)
{
  boolean isAliveMessage = FALSE;
  switch(pCommand->v6.reserved) {
    case 0x9:
      /*Alive message for camera group arrived. Reset timestamp*/
      if(pCommand->head == PROTOCOL_VERSION_V07) {
    	  COMM_PRINTF_ALWAYS(("Error ! Alive message arrived for NVCCP V07 ! Should not be the case ! \r\n"));
        break;
      }   
      spCommand->v6.message_return_status = ResetCamGroupAlivePeriod(pCommand->v6.cam_grp_id, (pCommand->cam_master & 0xF));
      isAliveMessage = TRUE;
      break;
    case 0xA:
      /*Simple ping.respond with OK */
      spCommand->v6.message_return_status = CCP_STATUS_OK;
      isAliveMessage = TRUE;
      break;
    default:
      isAliveMessage = FALSE;
      break;
  }
  return isAliveMessage;
}



static uint8 TegraEthComm_ExecuteCmd_V06(tPacket* pCommand, tPacket *spCommand)
{
    uint8 ret_cmd = CMD_RES_ACCEPTED;
    uint16 status = 0;
    if((pCommand->cam_master >= TEGRA_A) && (pCommand->cam_master < TEGRA_INVLD))
    {

    if(HandleCamGroupAliveMessages(pCommand,spCommand)){
      return CMD_RES_ACCEPTED;
    }

    /*Request from Valid Tegra A|B|C*/
    switch(pCommand->v6.cam_cmd)
    {
        case REQ_MASTER:
            spCommand->v6.message_return_status = Resource_GetOwnershipMaster(pCommand->v6.cam_grp_id, (pCommand->cam_master & 0xF) );
            PopulateNvccpVersionForCamGroup(pCommand->v6.cam_grp_id, (pCommand->cam_master & 0xF), pCommand->head);
            break;
        case REL_MASTER:
            spCommand->v6.message_return_status = Resource_GiveawayOwnershipMaster(pCommand->v6.cam_grp_id, (pCommand->cam_master & 0xF));
            break;
        case REQ_SLAVE:
            spCommand->v6.message_return_status = Resource_GetOwnershipSlave(pCommand->v6.cam_grp_id, (pCommand->cam_master & 0xF) );
            PopulateNvccpVersionForCamGroup(pCommand->v6.cam_grp_id, (pCommand->cam_master & 0xF), pCommand->head);
            break;
        case REL_SLAVE:
            spCommand->v6.message_return_status = Resource_GiveawayOwnershipSlave(pCommand->v6.cam_grp_id, (pCommand->cam_master & 0xF));
            break;
        case SET_POWER_ON:
            if (disable_udp_cmd_flag == FALSE){
                //DEBUG_PRINTF_ALWAYS(("Resource power on 1\r\n"));
                spCommand->v6.message_return_status = Resource_PowerOn(pCommand->v6.cam_grp_id, pCommand->v6.cam_units, (pCommand->cam_master & 0xF));
            }else if (disable_udp_cmd_flag == TRUE)
                spCommand->v6.message_return_status = 0;
            //COMM_PRINTF_ALWAYS(("on message return status : %d \n", spCommand->v6.message_return_status));
            break;
        case SET_POWER_OFF:
            if (disable_udp_cmd_flag == FALSE){
                //DEBUG_PRINTF_ALWAYS(("Resource power off 1\r\n"));
                spCommand->v6.message_return_status = Resource_PowerOff(pCommand->v6.cam_grp_id, pCommand->v6.cam_units, (pCommand->cam_master & 0xF));
            }else if (disable_udp_cmd_flag == TRUE)
                spCommand->v6.message_return_status = 0;
            //COMM_PRINTF_ALWAYS(("off message return status : %d \n", spCommand->v6.message_return_status));
            break;
        case SET_FSYNC_OWN:
            if(pCommand->v6.cam_frsync_owner == CAM_AGG_FRSYNC_SRC_TEGRA_A || pCommand->v6.cam_frsync_owner == CAM_AGG_FRSYNC_SRC_TEGRA_B ||
                    pCommand->v6.cam_frsync_owner == CAM_AGG_FRSYNC_SRC_TEGRA_C)
            {
                if(Resource_SetFsyncSrcState(pCommand->v6.cam_frsync_owner | 0x40))
                {

                    spCommand->v6.message_return_status = CCP_STATUS_OK;
                    spCommand->cmd = CMD_RES_ACCEPTED;
                }
                else
                {
                    spCommand->v6.message_return_status = CCP_STATUS_DENIED;
                    spCommand->cmd = CMD_RES_FAILED;
                }
            }
            else
            {
                spCommand->v6.message_return_status = CCP_STATUS_DENIED;
                spCommand->cmd = CMD_RES_INVALID;
            }
            break;
        case SET_FSYNC_EN:
            COMM_PRINTF_ALWAYS(("TegraEthComm: Calling Resource_SetFsyncEnState with cam_module : %d\n\r", pCommand->v6.cam_grp_id));
            if(Resource_SetFsyncEnState((((pCommand->v6.cam_grp_id)<<4) | ((pCommand->cam_master & 0xF))) , FRSYNC_ON))
            {
                spCommand->v6.message_return_status = CCP_STATUS_OK;
                spCommand->cmd = CMD_RES_ACCEPTED;
            }
            else
            {
                spCommand->v6.message_return_status = CCP_STATUS_DENIED;
                spCommand->cmd = CMD_RES_FAILED;
            }
            break;
        case SET_FSYNC_DIS:
            if(Resource_SetFsyncEnState((((pCommand->v6.cam_grp_id)<<4) | ((pCommand->cam_master & 0xF))), FRSYNC_OFF))
            {
                spCommand->v6.message_return_status = CCP_STATUS_OK;
                spCommand->cmd = CMD_RES_ACCEPTED;
            }
            else
            {
                spCommand->v6.message_return_status = CCP_STATUS_DENIED;
                spCommand->cmd = CMD_RES_FAILED;
            }
            break;
        default:
            ret_cmd = CMD_RES_FAILED;
            break;
    }

    TegraEthComm_UpdateStatus_V06(pCommand, spCommand);

    }
    else
    {
        ret_cmd = CMD_RES_FAILED;
    }
    return ret_cmd;
}


static void TegraEthComm_UpdateStatus(tPacket* status)
{
    uint8 cam_dev;

    status->cam_pwr = CAM_ON;
    /* Update the camera status to bit mask */

    if (status->head == PROTOCOL_VERSION_V04)
    {
        status->v4.cam_gmsl = 0;

        for(cam_dev = CAMERA_MODULE_A; cam_dev < CAMERA_MODULE_MAX; cam_dev++)
        {
            if(Resource_GetCameraState(cam_dev) == CAM_ON)
            {
                status->v4.cam_gmsl |= (1<<cam_dev);
            }
        }
    }
    else
    {
        status->v5.cam_units = 0;

        for(cam_dev = CAMERA_UNIT_A0; cam_dev < CAMERA_UNIT_MAX; cam_dev++)
        {
            if(Resource_GetCameraState(cam_dev) == CAM_ON)
            {
                status->v5.cam_units |= (1<<cam_dev);
            }
        }
    }

    if (status->head == PROTOCOL_VERSION_V05)
    {
        status->v5.cam_agg_pwdn = 0;

        for(cam_dev = CAMERA_MODULE_A; cam_dev < CAMERA_MODULE_MAX; cam_dev++)
        {
            if(Resource_GetCameraAggState(cam_dev) == CAM_ON)
            {
                status->v5.cam_agg_pwdn |= (1<<cam_dev);
            }
        }
    }

    if (status->head == PROTOCOL_VERSION_V05)
    {
        // cam_frsync_owner
        status->v5.cam_frsync_owner = Resource_GetFsyncSrcState();
        // cam_agg_frsync_en
        status->v5.cam_agg_frsync_en = FRSYNC_ENABLE;
        // cam_agg_frsync_group
        status->v5.cam_agg_frsync_group = 0;
        for(cam_dev = CAMERA_MODULE_A; cam_dev < CAMERA_MODULE_MAX; cam_dev++)
        {
            if(Resource_GetFsyncEnState(cam_dev) == FRSYNC_ENABLE)
            {
                status->v5.cam_agg_frsync_group |= (1<<cam_dev);
            }
        }
    }
    else
    {
        #if SOURCE_MUX_SELECTION_ENABLE == 1
            status->v4.tegra_gmsl = Resource_GetGmslState();
            status->v4.tegra_fpdl = Resource_GetFpdlState();
            status->v4.tegra_a_hdmi = Resource_GetTegraAHdmiState();
            status->v4.tegra_b_hdmi = Resource_GetTegraBHdmiState();
            status->v4.tegra_pci = Resource_GetPciState();
        #else
            status->v4.tegra_gmsl = INGORE;
            status->v4.tegra_fpdl = INGORE;
            status->v4.tegra_a_hdmi = INGORE;
            status->v4.tegra_b_hdmi = INGORE;
            status->v4.tegra_pci = INGORE;
        #endif
    }

/* we should return the Major version number,
 * but historical issue DPX2 doesn't have major version number */
#if PROJECT == PROJECT_DRIVE_PX2
    status->aurix_version = SW_REVISION;
#else
#if ETHERNET_OTA_FW_TYPE == _OTA_PROD_FW
    status->aurix_version = SW_MAJOR;
#if PROJECT == PROJECT_PAGASUS2
#else
    status->aurix_version = UPDATE_SW_MAJOR;
#endif
#endif
#endif
    status->fan_state = Resource_GetFanState();;
   status->cam_master = INGORE;
}

/* execute the command
 * return uint8
 *   - CMD_RES_ACCEPTED         Command accepted
     - CMD_RES_FAILED           Command failed, fail to set the source
     - CMD_RES_DENIED           Command deny, happened when Tegra is camera slave and want to turn on/off camera.
 *  */
static uint8 TegraEthComm_ExecuteCmd(tPacket* pCommand)
{
    uint8 ret_cmd = CMD_RES_ACCEPTED;
    boolean status = TRUE;
    uint8 cam_unit_num = 0;
    uint16 cam_unit_select;

    COMM_PRINTF_ALWAYS(("TegraEthComm: executing %s command\n\r", get_command_string(pCommand->cmd)));
    if(pCommand->cam_pwr != INGORE)
    {
        if(pCommand->cam_master == CAM_MASTER)
        {
            if(pCommand->head == PROTOCOL_VERSION_V04)
            {
                if(pCommand->v4.cam_gmsl != INGORE)
                {
                    if((pCommand->v4.cam_gmsl & GMSL_A) != 0)
                    {
#if CAMERA_A_ENABLE == 1
                        status &= Resource_SetCameraState(CAMERA_MODULE_A, pCommand->cam_pwr);
#else
                        COMM_PRINTF_ALWAYS(("TegraEthComm: Camera module A is not enable in Aurix\n\r"));
#endif
                    }
                    if((pCommand->v4.cam_gmsl & GMSL_B) != 0)
                    {
#if CAMERA_B_ENABLE == 1
                        status &= Resource_SetCameraState(CAMERA_MODULE_B, pCommand->cam_pwr);
#else
                        COMM_PRINTF_ALWAYS(("TegraEthComm: Camera module B is not enable in Aurix\n\r"));
#endif
                    }
                    if((pCommand->v4.cam_gmsl & GMSL_C) != 0)
                    {
#if CAMERA_C_ENABLE == 1
                        status &= Resource_SetCameraState(CAMERA_MODULE_C, pCommand->cam_pwr);
#else
                        COMM_PRINTF_ALWAYS(("TegraEthComm: Camera module C is not enable in Aurix\n\r"));
#endif
                    }
                    if((pCommand->v4.cam_gmsl & GMSL_D) != 0)
                    {
#if CAMERA_D_ENABLE == 1
                        status &= Resource_SetCameraState(CAMERA_MODULE_D, pCommand->cam_pwr);
#else
                        COMM_PRINTF_ALWAYS(("TegraEthComm: Camera module D is not enable in Aurix\n\r"));
#endif
                    }
                }
            }
            else
            {

                if(pCommand->v5.cam_units != 0)
                {
                    for(cam_unit_num = CAMERA_UNIT_A0; cam_unit_num < CAMERA_UNIT_MAX; cam_unit_num++)
                    {
                        cam_unit_select = 1 << cam_unit_num;
                        if((pCommand->v5.cam_units & cam_unit_select) != 0)
                        {
                            status &= Resource_SetCameraState(cam_unit_num, pCommand->cam_pwr);
                        }
                    }
                }

                if(pCommand->v5.cam_agg_pwdn != INGORE)
                {
                    if((pCommand->v5.cam_agg_pwdn & GMSL_A) != 0)
                    {
#if CAMERA_A_ENABLE == 1
                        status &= Resource_SetCameraAggState(CAMERA_MODULE_A, pCommand->cam_pwr);
#else
                        COMM_PRINTF_ALWAYS(("TegraEthComm: Camera module A is not enable in Aurix\n\r"));
#endif
                    }
                    if((pCommand->v5.cam_agg_pwdn & GMSL_B) != 0)
                    {
#if CAMERA_B_ENABLE == 1
                        status &= Resource_SetCameraAggState(CAMERA_MODULE_B, pCommand->cam_pwr);
#else
                        COMM_PRINTF_ALWAYS(("TegraEthComm: Camera module B is not enable in Aurix\n\r"));
#endif
                    }
                    if((pCommand->v5.cam_agg_pwdn & GMSL_C) != 0)
                    {
#if CAMERA_C_ENABLE == 1
                        status &= Resource_SetCameraAggState(CAMERA_MODULE_C, pCommand->cam_pwr);
#else
                        COMM_PRINTF_ALWAYS(("TegraEthComm: Camera module C is not enable in Aurix\n\r"));
#endif
                    }
                    if((pCommand->v5.cam_agg_pwdn & GMSL_D) != 0)
                    {
#if CAMERA_D_ENABLE == 1
                        status &= Resource_SetCameraAggState(CAMERA_MODULE_D, pCommand->cam_pwr);
#else
                        COMM_PRINTF_ALWAYS(("TegraEthComm: Camera module D is not enable in Aurix\n\r"));
#endif
                    }
                }
            }
        }
        else
        {
            ret_cmd = CMD_RES_DENIED;
        }
    } //cam_pwr != INGORE

    if (pCommand->head == PROTOCOL_VERSION_V05)
    {
        // cam_frsync_owner
        if(pCommand->v5.cam_frsync_owner != INGORE)
        {
            status &= Resource_SetFsyncSrcState(pCommand->v5.cam_frsync_owner);
        }
        //frsync_en and frsync_sel
        if(pCommand->v5.cam_agg_frsync_en != INGORE)
        {
            if(pCommand->cam_master == CAM_MASTER)
            {
                if(pCommand->v5.cam_agg_frsync_group != INGORE)
                {
                    if((pCommand->v5.cam_agg_frsync_group & GMSL_A) != 0)
                    {
#if CAMERA_A_ENABLE == 1
                        status &= Resource_SetFsyncEnState(CAMERA_MODULE_A, pCommand->v5.cam_agg_frsync_en);
#else
                        COMM_PRINTF_ALWAYS(("TegraEthComm: Camera module A is not enable in Aurix\n\r"));
#endif
                    }
                    if((pCommand->v5.cam_agg_frsync_group & GMSL_B) != 0)
                    {
#if CAMERA_B_ENABLE == 1
                        status &= Resource_SetFsyncEnState(CAMERA_MODULE_B, pCommand->v5.cam_agg_frsync_en);
#else
                        COMM_PRINTF_ALWAYS(("TegraEthComm: Camera module B is not enable in Aurix\n\r"));
#endif
                    }
                    if((pCommand->v5.cam_agg_frsync_group & GMSL_C) != 0)
                    {
#if CAMERA_C_ENABLE == 1
                        status &= Resource_SetFsyncEnState(CAMERA_MODULE_C, pCommand->v5.cam_agg_frsync_en);
#else
                        COMM_PRINTF_ALWAYS(("TegraEthComm: Camera module C is not enable in Aurix\n\r"));
#endif
                    }
                    if((pCommand->v5.cam_agg_frsync_group & GMSL_D) != 0)
                    {
#if CAMERA_D_ENABLE == 1
                        status &= Resource_SetFsyncEnState(CAMERA_MODULE_D, pCommand->v5.cam_agg_frsync_en);
#else
                        COMM_PRINTF_ALWAYS(("TegraEthComm: Camera module D is not enable in Aurix\n\r"));
#endif
                    }
                }
            }
            else
            {
                ret_cmd = CMD_RES_DENIED;
            }
        }
    }
    else
    {

#if SOURCE_MUX_SELECTION_ENABLE == 1
        if(pCommand->v4.tegra_gmsl != INGORE)
        {
            status &= Resource_SetGmslState(pCommand->v4.tegra_gmsl);
        }
        if(pCommand->v4.tegra_fpdl != INGORE)
        {
            status &= Resource_SetFpdlState(pCommand->v4.tegra_fpdl);
        }
        if(pCommand->v4.tegra_a_hdmi != INGORE)
        {
            status &= Resource_SetTegraAHdmiState(pCommand->v4.tegra_a_hdmi);
        }
        if(pCommand->v4.tegra_b_hdmi != INGORE)
        {
            status &= Resource_SetTegraBHdmiState(pCommand->v4.tegra_b_hdmi);
        }
        if(pCommand->v4.tegra_pci != INGORE)
        {
            status &= Resource_SetPciState(pCommand->v4.tegra_pci);
        }
#endif
    }

    if(pCommand->fan_state != INGORE)
    {
        status &= Resource_SetFanState(pCommand->fan_state);
    }
    if(ret_cmd != CMD_RES_DENIED)
    {
        if(status)
        {
            ret_cmd = CMD_RES_ACCEPTED;
            COMM_PRINTF_ALWAYS(("TegraEthComm: execute command successfully\n\r"));
        }
        else
        {
            COMM_PRINTF_ALWAYS(("TegraEthComm: some resource is not executed successfully\n\r"));
            ret_cmd = CMD_RES_FAILED;
        }
    }
    return ret_cmd;
}
#endif
static void print_receive_buffer(uint8* pData, uint8 len)
{
    uint8* pData_l = pData;
    if(pData_l != NULL)
    {
      COMM_DEBUG_PRINTF(("TegraEthComm: data length is %d\n\r", len));

      COMM_DEBUG_PRINTF(("TegraEthComm: the data is \n\r"));
      uint8 i =0;
      for(i=0;i<len;i++)
      {
          COMM_DEBUG_PRINTF(("0x%x \n\r", (*pData_l)));
          pData_l++;
      }
    }
    else
    {
    }
}

#if 1
static char* get_command_string(uint8 cmd)
{
    static const char* command_string[]=
    {
            "invalid",
            "PING",
            "ACK",
            "REQ_GET",
            "REQ_SET",
            "invalid",
            "invalid",
            "RES_ACCEPTED",
            "RES_FAILED",
            "RES_DENIED",
            "RES_INVALID"
    };

    if((cmd < CMD_RES_INVALID) && (cmd >= CMD_PING)   )
    {
        return command_string[cmd];
    }
    else
    {
        return "invalid";
    }

}


static void TegraEthComm_Receive(uint8* pData, uint8 len)
{
    tPacket packet;
    COMM_DEBUG_PRINTF(("TegraEthComm: receive Data \n\r"));
    if(Protocol_GetPackage(pData, len, &packet))
    {
        TegraEthComm_HandleCmd(&packet);
    }
}


static void TegraEthComm_HandleCmd(tPacket* packet)
{
    tPacket resp_packet;
    resp_packet.head = packet->head;
    Protocol_InitialPacket(&resp_packet);
    dump_packet(packet);
    COMM_DEBUG_PRINTF(("TegraEthComm: Receive %s command\n\r", get_command_string(packet->cmd)));

    //Just to ensure we are sending same protocol packet back
    resp_packet.head = packet->head;

    if((packet->head == PROTOCOL_VERSION_V06) || (packet->head == PROTOCOL_VERSION_V07))
    {
        isV6SessionStarted[0] = 1;
        if(isV6SessionStarted[1])
            COMM_PRINTF_ALWAYS(("CAUTIONS: Messages of different versions of the TACP protocol are flowing...the behavior of Camera Power can be as expected. \n\r"));
        //COMM_PRINTF_ALWAYS(("TegraEthComm: received V6 packet \n"));
        switch(packet->cmd)
        {
            case CMD_REQ_GET:
                resp_packet.cmd = CMD_RES_ACCEPTED;
                //COMM_PRINTF_ALWAYS(("TegraEthComm: calling TegraEthComm_UpdateStatus_V06... \n"));
                TegraEthComm_UpdateStatus_V06(packet, &resp_packet);
                //Return status
                resp_packet.v6.message_return_status = CCP_STATUS_OK;

                break;
            case CMD_REQ_SET:
                //COMM_PRINTF_ALWAYS(("TegraEthComm: calling TegraEthComm_ExecuteCmd_V06... \n"));
                resp_packet.cmd = TegraEthComm_ExecuteCmd_V06(packet, &resp_packet);
                break;
            default:
                resp_packet.cmd = CMD_RES_INVALID;
                break;
        }
    }
    else if (packet->head == PROTOCOL_VERSION_V04 || packet->head == PROTOCOL_VERSION_V05)
    {
        isV6SessionStarted[1] = 1;
        if(isV6SessionStarted[0])
            COMM_PRINTF_ALWAYS(("TegraEthComm: Messages of different versions of the TACP protocol are flowing...the behavior of Camera Power can be as expected. \n\r"));
        switch(packet->cmd)
        {
            case CMD_PING:
                resp_packet.cmd  = CMD_ACK;
                TegraEthComm_UpdateStatus(&resp_packet);
                HeartBeatFeed(TEGRA_BOTH);
                break;
            case CMD_REQ_GET:
                resp_packet.cmd = CMD_RES_ACCEPTED;
                TegraEthComm_UpdateStatus(&resp_packet);
                break;
            case CMD_REQ_SET:
                resp_packet.cmd = TegraEthComm_ExecuteCmd(packet);
                TegraEthComm_UpdateStatus(&resp_packet);
                break;
            default:
                resp_packet.cmd = CMD_RES_INVALID;
                break;
        }
    }
    else
    {
        /* unsupport protocol version */
        /* matching with received protocol version */
        resp_packet.head = packet->head;
        resp_packet.cmd  = CMD_RES_INVALID;
    }

    Protocol_PreparePackage(&resp_packet);
    COMM_DEBUG_PRINTF(("TegraEthComm: Response %s to Tegra\n\r", get_command_string(resp_packet.cmd)));
    dump_packet(&resp_packet);
    udp_app_send((uint8*)&resp_packet, sizeof(tPacket));
}
#endif
