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
#include "CIM.h"
#include "CIMcmds.h"
#include "Configuration.h"
#include "SysSe/Comm/Ifx_Shell.h"
#include <I2c.h>
#include "SysSe/Bsp/Bsp.h"
#include <internal_fw_debug.h>
#include "resource_state_manage.h"
#include "gpioexp.h"
#include "resource.h"
#if CAMERA_CIM_ENABLE == 1

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define CIM_CMD_DEBUG_ENABLE 1

#if CIM_CMD_DEBUG_ENABLE == 1
#define CIM_CMD_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define CIM_CMD_DEBUG_PRINTF(x)
#endif


/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
boolean disable_udp_cmd_flag = FALSE;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/


/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/


/** \brief Handle the 'cimpower' command
 *
 * \par Syntax
 *  - cimpower [on/off]: power enable/disable CIM and release RST of via CIM IO Expander
 *
 */

boolean CIM_Power_Control_Cmd(pchar args, void *data, IfxStdIf_DPipe *io)
{
    boolean powerEnable;
    boolean cim_sgood;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : cimpower [on/off]: "ENDL);
        IfxStdIf_DPipe_print(io, "           > power enable/disable CIM"ENDL);
        return TRUE;
    }
    else {
        // first we get the first argument
        if (Ifx_Shell_matchToken(&args, "on") != FALSE)
        {
            powerEnable = TRUE;
        }
        else if (Ifx_Shell_matchToken(&args, "off") != FALSE)
        {
            powerEnable = FALSE;
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid Camera Power state %s"ENDL, args);
            return TRUE;
        }
    }
    // Check if CIM is present and seated
    cim_sgood = CIM_Check_SGood_Status();
    if (cim_sgood)
    {
        CIM_Power_Control(powerEnable);
    }
    else
    {
        IfxStdIf_DPipe_print(io, "Error : Failed to power on/off CIM %s"ENDL, args);
    }

    return TRUE;
}

/** \brief Handle the 'cimcampower' command
 *
 * \par Syntax
 *  - cimcampower [a0/a1/a2/a3/b0/b1/b2/b3/c0/c1/c2/c3/d0/d1/d2/d3] [on/off]: control LSW power enable/disable
 */
boolean CIM_LSW_Control_Cmd(pchar args, void *data, IfxStdIf_DPipe *io)
{
    eCameraUnit cam_unit;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : cimcampower [a0/a1/a2/a3/b0/b1/b2/b3/c0/c1/c2/c3/d0/d1/d2/d3] [on/off]: "ENDL);
        IfxStdIf_DPipe_print(io, "           > power enable/disable CIM LSW channels"ENDL);
        IfxStdIf_DPipe_print(io, "           >    a0~a3 - Short View Camera Group(SVC)"ENDL);
        IfxStdIf_DPipe_print(io, "           >    b0~b3 - Cross View Camera Group(CVC)"ENDL);
        IfxStdIf_DPipe_print(io, "           >    c0~c3 - Long Range Camera, Mid Range Radar Group(LRC_MRR) "ENDL);
        IfxStdIf_DPipe_print(io, "           >    d0~d3 - Long Range Radar Group(LRR)"ENDL);
        return TRUE;
    }
    else
    {
#if CAMERA_A_ENABLE == 1
        if((Ifx_Shell_matchToken(&args, "a0") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "A0") != FALSE))
        {
            cam_unit = CAMERA_UNIT_A0;
        }
        else if((Ifx_Shell_matchToken(&args, "a1") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "A1") != FALSE))
        {
            cam_unit = CAMERA_UNIT_A1;
        }
        else if((Ifx_Shell_matchToken(&args, "a2") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "A2") != FALSE))
        {
            cam_unit = CAMERA_UNIT_A2;
        }
        else if((Ifx_Shell_matchToken(&args, "a3") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "A3") != FALSE))
        {
            cam_unit = CAMERA_UNIT_A3;
        }
#endif
#if CAMERA_B_ENABLE == 1
        else if((Ifx_Shell_matchToken(&args, "b0") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "B0") != FALSE))
        {
            cam_unit = CAMERA_UNIT_B0;
        }
        else if((Ifx_Shell_matchToken(&args, "b1") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "B1") != FALSE))
        {
            cam_unit = CAMERA_UNIT_B1;
        }
        else if((Ifx_Shell_matchToken(&args, "b2") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "B2") != FALSE))
        {
            cam_unit = CAMERA_UNIT_B2;
        }
        else if((Ifx_Shell_matchToken(&args, "b3") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "B3") != FALSE))
        {
            cam_unit = CAMERA_UNIT_B3;
        }
#endif
#if CAMERA_C_ENABLE == 1
        else if((Ifx_Shell_matchToken(&args, "c0") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "C0") != FALSE))
        {
            cam_unit = CAMERA_UNIT_C0;
        }
        else if((Ifx_Shell_matchToken(&args, "c1") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "C1") != FALSE))
        {
            cam_unit = CAMERA_UNIT_C1;
        }
        else if((Ifx_Shell_matchToken(&args, "c2") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "C2") != FALSE))
        {
            cam_unit = CAMERA_UNIT_C2;
        }
        else if((Ifx_Shell_matchToken(&args, "c3") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "C3") != FALSE))
        {
            cam_unit = CAMERA_UNIT_C3;
        }
#endif
#if CAMERA_D_ENABLE == 1
        else if((Ifx_Shell_matchToken(&args, "d0") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "D0") != FALSE))
        {
            cam_unit = CAMERA_UNIT_D0;
        }
        else if((Ifx_Shell_matchToken(&args, "d1") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "D1") != FALSE))
        {
            cam_unit = CAMERA_UNIT_D1;
        }
        else if((Ifx_Shell_matchToken(&args, "d2") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "D2") != FALSE))
        {
            cam_unit = CAMERA_UNIT_D2;
        }
        else if((Ifx_Shell_matchToken(&args, "d3") != FALSE) ||
            (Ifx_Shell_matchToken(&args, "D3") != FALSE))
        {
            cam_unit = CAMERA_UNIT_D3;
        }
#endif
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid camera %s"ENDL, args);
            return TRUE;
        }

        // get second argument        
        if (Ifx_Shell_matchToken(&args, "on") != FALSE)
        {
            Resource_SetCameraState(cam_unit, CAMERA_ON);
        }
        else if (Ifx_Shell_matchToken(&args, "off") != FALSE)
        {
            Resource_SetCameraState(cam_unit, CAMERA_OFF);
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid Camera Power state %s"ENDL, args);
            return TRUE;
        }
        IfxStdIf_DPipe_print(io, "Command Executed"ENDL);
    }

    return TRUE;
}

/** \brief Handle the 'cimaggpower' command
 *
 * \par Syntax
 *  - cimaggpower [a/b/c/d] [on/off]: control CIM aggregator power enable/disable
 */
boolean CIM_Aggregator_Control_Cmd(pchar args, void *data, IfxStdIf_DPipe *io)
{
    eCameraModule cam_module;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : cimaggpower [a/b/c/d] [on/off]: "ENDL);
        IfxStdIf_DPipe_print(io, "           > CIM Aggregator Power on/off "ENDL);
        IfxStdIf_DPipe_print(io, "           >    a - Short View Camera (SVC)"ENDL);
        IfxStdIf_DPipe_print(io, "           >    b - Cross View Camera (CVC)"ENDL);
        IfxStdIf_DPipe_print(io, "           >    c - Long Range Camera, Mid Range Radar (LRC_MRR) "ENDL);
        IfxStdIf_DPipe_print(io, "           >    d - Long Range Radar (LRR)"ENDL);

        return TRUE;
    }
    else
    {
        // first we get the device selection
        if ((Ifx_Shell_matchToken(&args, "a") != FALSE) || (Ifx_Shell_matchToken(&args, "A") != FALSE))
        {
#if CAMERA_A_ENABLE == 1
            cam_module = CAMERA_MODULE_A;
#else
            IfxStdIf_DPipe_print(io, "Error : invalid Group A"ENDL);
            return TRUE;
#endif
        }
        else if ((Ifx_Shell_matchToken(&args, "b") != FALSE) || (Ifx_Shell_matchToken(&args, "B") != FALSE))
        {
#if CAMERA_B_ENABLE == 1
            cam_module = CAMERA_MODULE_B;
#else
            IfxStdIf_DPipe_print(io, "Error : invalid Group B"ENDL);
            return TRUE;
#endif
        }
        else if ((Ifx_Shell_matchToken(&args, "c") != FALSE) || (Ifx_Shell_matchToken(&args, "C") != FALSE))
        {
#if CAMERA_C_ENABLE == 1
            cam_module = CAMERA_MODULE_C;
#else
            IfxStdIf_DPipe_print(io, "Error : invalid Group C"ENDL);
            return TRUE;
#endif

        }
        else if ((Ifx_Shell_matchToken(&args, "d") != FALSE) || (Ifx_Shell_matchToken(&args, "D") != FALSE))
        {
#if CAMERA_D_ENABLE == 1
            cam_module = CAMERA_MODULE_D;
#else
            IfxStdIf_DPipe_print(io, "Error : invalid Group C"ENDL);
            return TRUE;
#endif

        }
        else if ((Ifx_Shell_matchToken(&args, "e") != FALSE) || (Ifx_Shell_matchToken(&args, "E") != FALSE))
        {
#if CAMERA_E_ENABLE == 1
            cam_module = CAMERA_MODULE_E;
#else
            IfxStdIf_DPipe_print(io, "Error : invalid Group E"ENDL);
            return TRUE;
#endif
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid camera %s"ENDL, args);
            return TRUE;
        }

        // get second argument
        if (Ifx_Shell_matchToken(&args, "on") != FALSE)
        {
            Resource_SetCameraAggState(cam_module, CAMERA_ON);
        }
        else if (Ifx_Shell_matchToken(&args, "off") != FALSE)
        {
            Resource_SetCameraAggState(cam_module, CAMERA_OFF);
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid Aggregator Power state %s"ENDL, args);
            return TRUE;
        }
        IfxStdIf_DPipe_print(io, "Command Executed"ENDL);
    }

    return TRUE;
}

boolean CIM_FrameSync_Control_Cmd(pchar args, void *data, IfxStdIf_DPipe *io)
{
    eCameraModule cam_module;
    eCamFrsyncAction action;
    uint8 tegra;
    uint8 prev_fsync_state[4];
    uint8 fsync_src;
    uint8 index;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : cimfsync select/set"ENDL);
        IfxStdIf_DPipe_print(io, "           > CIM frame sync control, possible format is "ENDL);
        IfxStdIf_DPipe_print(io, "           > cimfsync select x1/x2/cvm"ENDL);
        IfxStdIf_DPipe_print(io, "           > select the source for frame sync "ENDL);
        IfxStdIf_DPipe_print(io, "           > cimfsync set [a/b/c/d] [on/off]"ENDL);
        IfxStdIf_DPipe_print(io, "           > CIM frame sync enable group"ENDL);
        IfxStdIf_DPipe_print(io, "           >    a - Short View Camera (SVC)"ENDL);
        IfxStdIf_DPipe_print(io, "           >    b - Cross View Camera (CVC)"ENDL);
        IfxStdIf_DPipe_print(io, "           >    c - Long Range Camera, Mid Range Radar (LRC_MRR) "ENDL);
        IfxStdIf_DPipe_print(io, "           >    d - Long Range Radar (LRR)"ENDL);
        return TRUE;
    }
    else
    {
        if((Ifx_Shell_matchToken(&args, "select") != FALSE) || (Ifx_Shell_matchToken(&args, "SELECT") != FALSE))
        {
            if((Ifx_Shell_matchToken(&args, "x1") != FALSE) || (Ifx_Shell_matchToken(&args, "X1") != FALSE))
            {
                tegra = TEGRA_A;
            }
            else if((Ifx_Shell_matchToken(&args, "x2") != FALSE) || (Ifx_Shell_matchToken(&args, "X2") != FALSE))
            {
                tegra = TEGRA_B;
            }
            else if((Ifx_Shell_matchToken(&args, "cvm") != FALSE) || (Ifx_Shell_matchToken(&args, "CVM") != FALSE))
            {
                tegra = TEGRA_C;
            }
            else
            {
                IfxStdIf_DPipe_print(io, "Error : invalid tegra %s, type cimfsync ? to see help"ENDL, args);
                return TRUE;
            }
            fsync_src = Resource_GetFsyncSrcState();
            /*Get frsync state before FrSyncSrc Change */
            for(index=0;index<4;index++)
            {
                prev_fsync_state[index] = Resource_GetFsyncEnState(index);
                Resource_SetFsyncEnState((index<<4)|(fsync_src & 0xF),FRSYNC_OFF);
            }
            Resource_SetFsyncSrcState(tegra);
            /*restore frsync state for Aggregators after FrSyncSrc Change*/
            for(index=0;index<4;index++)
            {
                Resource_SetFsyncEnState((index<<4)|(tegra & 0xF), prev_fsync_state[index]);
            }
            IfxStdIf_DPipe_print(io, "Command Executed"ENDL);
        }
        else if((Ifx_Shell_matchToken(&args, "set") != FALSE) || (Ifx_Shell_matchToken(&args, "SET") != FALSE))
        {
            if((Ifx_Shell_matchToken(&args, "a") != FALSE) || (Ifx_Shell_matchToken(&args, "A") != FALSE))
            {
                cam_module = CAMERA_MODULE_A;
            }
            else if((Ifx_Shell_matchToken(&args, "b") != FALSE) || (Ifx_Shell_matchToken(&args, "B") != FALSE))
            {
                cam_module = CAMERA_MODULE_B;
            }
            else if((Ifx_Shell_matchToken(&args, "c") != FALSE) || (Ifx_Shell_matchToken(&args, "C") != FALSE))
            {
                cam_module = CAMERA_MODULE_C;
            }
            else if((Ifx_Shell_matchToken(&args, "d") != FALSE) || (Ifx_Shell_matchToken(&args, "D") != FALSE))
            {
                cam_module = CAMERA_MODULE_D;
            }
            else
            {
                IfxStdIf_DPipe_print(io, "Error : invalid group %s, type cimfsync ? to see help"ENDL, args);
                return TRUE;
            }
            if((Ifx_Shell_matchToken(&args, "on") != FALSE) || (Ifx_Shell_matchToken(&args, "ON") != FALSE))
            {
                action = FRSYNC_ON;
            }
            else if((Ifx_Shell_matchToken(&args, "off") != FALSE) || (Ifx_Shell_matchToken(&args, "OFF") != FALSE))
            {
                action = FRSYNC_OFF;
            }
            else
            {
                IfxStdIf_DPipe_print(io, "Error : invalid operation %s, type cimfsync ? to see help"ENDL, args);
                return TRUE;
            }
            fsync_src = Resource_GetFsyncSrcState(); /*Enable/Disable expects request from frsync source*/
            Resource_SetFsyncEnState((cam_module << 4)|(fsync_src & 0xF), action);
            IfxStdIf_DPipe_print(io, "Command Executed"ENDL);
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid parameter %s, type cimfsync ? to see help"ENDL, args);
            return TRUE;
        }
    }

}

boolean CIM_Power_Test_Cmd(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : cim power test command "ENDL);
        IfxStdIf_DPipe_print(io, "           > turn on/off all CIM power "ENDL);
        return TRUE;
    }else if(Ifx_Shell_matchToken(&args, "on") != FALSE) {
        disable_udp_cmd_flag=TRUE;
        Resource_SetCameraAggState(CAMERA_MODULE_A, CAMERA_ON);
        Resource_SetCameraAggState(CAMERA_MODULE_B, CAMERA_ON);
        Resource_SetCameraAggState(CAMERA_MODULE_C, CAMERA_ON);
        Resource_SetCameraAggState(CAMERA_MODULE_D, CAMERA_ON);

        Resource_SetCameraState(CAMERA_UNIT_A0, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_A1, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_A2, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_A3, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_B0, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_B1, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_B2, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_B3, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_C0, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_C1, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_C2, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_C3, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_D0, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_D1, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_D2, CAMERA_ON);
        Resource_SetCameraState(CAMERA_UNIT_D3, CAMERA_ON);
    }else if(Ifx_Shell_matchToken(&args, "off") != FALSE) {
        disable_udp_cmd_flag=FALSE;
        Resource_SetCameraAggState(CAMERA_MODULE_A, CAMERA_OFF);
        Resource_SetCameraAggState(CAMERA_MODULE_B, CAMERA_OFF);
        Resource_SetCameraAggState(CAMERA_MODULE_C, CAMERA_OFF);
        Resource_SetCameraAggState(CAMERA_MODULE_D, CAMERA_OFF);

        Resource_SetCameraState(CAMERA_UNIT_A0, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_A1, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_A2, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_A3, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_B0, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_B1, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_B2, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_B3, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_C0, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_C1, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_C2, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_C3, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_D0, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_D1, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_D2, CAMERA_OFF);
        Resource_SetCameraState(CAMERA_UNIT_D3, CAMERA_OFF);
    }

    return TRUE;
}

boolean CIM_SGood_Status_Cmd(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : cimseatgood "ENDL);
        IfxStdIf_DPipe_print(io, "           > Show CIM presence detection and seat good status "ENDL);
        return TRUE;
    }
    CIM_Check_SGood_Status();
    return TRUE;
}

boolean ReadCamVolt(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 cam_monitor;
    float camData[NUM_ADC_UNITS];

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : readcamvolt x "ENDL);
        IfxStdIf_DPipe_print(io, "           > read camera voltage in device x "ENDL);
        IfxStdIf_DPipe_print(io, "           > x = 0 for address 0x2c (a0~a3)"ENDL);
        IfxStdIf_DPipe_print(io, "           > x = 1 for address 0x2d (b0~b3)"ENDL);
        IfxStdIf_DPipe_print(io, "           > x = 2 for address 0x2b (c0~c3)"ENDL);
        IfxStdIf_DPipe_print(io, "           > x = 3 for address 0x2a (d0~d3)"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &cam_monitor, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c address %s"ENDL, args);
            return TRUE;
        }
        switch (cam_monitor)
        {
            case 0:
                readCamVoltage(&i2c_cim_lsw_svc, camData);
                break;

            case 1:
                readCamVoltage(&i2c_cim_lsw_cvc, camData);
                break;

            case 2:
                readCamVoltage(&i2c_cim_lsw_lrc_mrr, camData);
                break;

            case 3:
                readCamVoltage(&i2c_cim_lsw_lrr, camData);
                break;

            default:
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid camera monitor %d"ENDL, cam_monitor);
                return FALSE;
        }
        IfxStdIf_DPipe_print(io, "Voltage:%f:%f:%f:%f V"ENDL,camData[0],camData[1],camData[2],camData[3]);
    }
    return TRUE;
}


boolean ReadCamCurr(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 cam_monitor;
    float camData[NUM_ADC_UNITS];

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : readcamcurr x"ENDL);
        IfxStdIf_DPipe_print(io, "           > read camera current in device x "ENDL);
        IfxStdIf_DPipe_print(io, "           > x = 0 for address 0x2c (a0~a3)"ENDL);
        IfxStdIf_DPipe_print(io, "           > x = 1 for address 0x2d (b0~b3)"ENDL);
        IfxStdIf_DPipe_print(io, "           > x = 2 for address 0x2b (c0~c3)"ENDL);
        IfxStdIf_DPipe_print(io, "           > x = 3 for address 0x2a (d0~d3)"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &cam_monitor, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c address %s"ENDL, args);
            return TRUE;
        }
        switch (cam_monitor)
        {
            case 0:
                readCamCurrent(&i2c_cim_lsw_svc, camData);
                break;

            case 1:
                readCamCurrent(&i2c_cim_lsw_cvc, camData);	
                break;

            case 2:
                readCamCurrent(&i2c_cim_lsw_lrc_mrr, camData);	
                break;

            case 3:
                readCamCurrent(&i2c_cim_lsw_lrr, camData);
                break;

            default:
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid camera monitor %d"ENDL, cam_monitor);
                return FALSE;
        }
        /*
        for(int i = 0; i < NUM_ADC_UNITS; i++)
        {
            IfxStdIf_DPipe_print(io, "Current: ADC %d is %f A."ENDL, i, camData[i]);
        }
        */
        IfxStdIf_DPipe_print(io, "Current:%f:%f:%f:%f A"ENDL,camData[0],camData[1],camData[2],camData[3]);
    }
    return TRUE;
}

boolean ReadCamVolCurr(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 cam_monitor;
    float camvolData[NUM_ADC_UNITS];
    float camcurrData[NUM_ADC_UNITS];
    uint8 devicename[5];
    char* name = devicename;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : readcamvoltcurr x"ENDL);
        IfxStdIf_DPipe_print(io, "           > read camera voltage current in device x "ENDL);
        IfxStdIf_DPipe_print(io, "           > x = 0 for address 0x2c (a0~a3)"ENDL);
        IfxStdIf_DPipe_print(io, "           > x = 1 for address 0x2d (b0~b3)"ENDL);
        IfxStdIf_DPipe_print(io, "           > x = 2 for address 0x2b (c0~c3)"ENDL);
        IfxStdIf_DPipe_print(io, "           > x = 3 for address 0x2a (d0~d3)"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &cam_monitor, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c address %s"ENDL, args);
            return TRUE;
        }
        switch (cam_monitor)
        {
            case 0://U1AA
                readCamCurrent(&i2c_cim_lsw_svc, camcurrData);
                readCamVoltage(&i2c_cim_lsw_svc, camvolData);
                name = "U1AA";
                break;

            case 1://U1AB
                readCamCurrent(&i2c_cim_lsw_cvc, camcurrData);
                readCamVoltage(&i2c_cim_lsw_cvc, camvolData);
                name = "U1AB";
                break;

            case 2://U1AC
                readCamCurrent(&i2c_cim_lsw_lrc_mrr, camcurrData);
                readCamVoltage(&i2c_cim_lsw_lrc_mrr, camvolData);
                name = "U1AC";
                break;

            case 3://U1AD
                readCamCurrent(&i2c_cim_lsw_lrr, camcurrData);
                readCamVoltage(&i2c_cim_lsw_lrc_mrr, camvolData);
                name = "U1AD";
                break;

            default:
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid camera monitor %d"ENDL, cam_monitor);
                return FALSE;
        }
        IfxStdIf_DPipe_print(io, "POC %s:%f V,%f A:%f V,%f A:%f V,%f A:%f V,%f A"ENDL,name, 
 camvolData[0],camcurrData[0],camvolData[1],camcurrData[1],camvolData[2],camcurrData[2],camvolData[3],camcurrData[3]);
    }
    return TRUE;
}
#endif //CAMERA_CIM_ENABLE == 1
