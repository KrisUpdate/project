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
#include "Configuration.h"
#include "SysSe/Bsp/Bsp.h"
#include <internal_fw_debug.h>
#include <I2c.h>
#include "resource_state_manage.h"
#include "Port/Std/IfxPort.h"
#include "SysSe/Bsp/Bsp.h"
#include "ethernet.h"
#include "I2cRom.h"
#include "gpioexp_pinmap.h"
#include "gpioexp.h"
#include "board.h"

#if CAMERA_CIM_ENABLE == 1


/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define CIM_DEBUG_ENABLE 0

#if CIM_DEBUG_ENABLE == 1
#define CIM_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define CIM_DEBUG_PRINTF(x)
#endif

#define CIM_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)


#define CAMMONITOR_DEBUG_ENABLE 1

#if CAMMONITOR_DEBUG_ENABLE == 1
#define CAMMONITOR_DEBUG_PRINTF(x)   DEBUG_PRINTF(x)
#else
#define CAMMONITOR_DEBUG_PRINTF(x)
#endif

#define CAMMONITOR_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)


/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
extern boolean g_PowerDown_flag;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/


/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

static boolean CIM_LSW_TurnOffAllPower(t_i2c_device* dev_ptr)
{
    uint8 w_value, r_value;
    boolean ret = FALSE;
    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, CIM_LSW_MAX20087_CONFIG_REG, 1, &w_value, 1);
    w_value &= (~CIM_LSW_MAX20087_CONFIG_EN_MASK);
    i2c_write_bytes(dev_ptr->bus, dev_ptr->address, CIM_LSW_MAX20087_CONFIG_REG, 1, &w_value);

    waitTime(TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
    /* verify the result */
    // we need to manually clear value, as if i2cread fail, the value of "value" will not change
    r_value = 0;
    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, CIM_LSW_MAX20087_CONFIG_REG, 1, &r_value, 1);

    if(r_value == w_value)
    {
        ret = TRUE;
    }
    return ret;
}

static void CIM_LSW_Initia(void)
{
    boolean ret = TRUE;

    ret &= CIM_LSW_TurnOffAllPower(&i2c_cim_lsw_svc);
    ret &= CIM_LSW_TurnOffAllPower(&i2c_cim_lsw_cvc);
    ret &= CIM_LSW_TurnOffAllPower(&i2c_cim_lsw_lrc_mrr);

//	if((board.project == E3550) && (board.project == P3570))
//	{
		ret &= CIM_LSW_TurnOffAllPower(&i2c_cim_lsw_lrr);
//	}

    if(!ret)
        CIM_ALWAYS_PRINTF(("Fail to initial the LSW \r\n"));

}

static void CIM_Set_Io_Expander_Pins_Low(void)
{
	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P00);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P00);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P01);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P01);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P02);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P02);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P03);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P03);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P04);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P04);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P05);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P05);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P06);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P06);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P07);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P07);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P10);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P10);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P11);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P11);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P12);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P12);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P13);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P13);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P14);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P14);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P15);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P15);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P16);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P16);

	SET_GPIOEXP_OUTPUT_LOW(IoExpPort_CIM_A74_P17);
	SET_GPIOEXP_CONFIG_OUTPUT(IoExpPort_CIM_A74_P17);

}

/** \brief Enable/Disable power for CIM board
 *  \param powerEnable : 0 - disable power, 1 - enable power
 *
 */
boolean CIM_Power_Control(boolean powerEnable)
{
    /* Power enable/disable CIM and
     * Release RST of CIM IO Expander
     */

	//TODO: do we still need this command for P3570?
	// there's no power control signal in P3570
    if(powerEnable)
    {
//    	if(board.project == E3550)
//        {
//    		SET_GPIOEXP_OUTPUT_HIGH(E3550_CIM_PWR_ON_EXP);
//    		SET_GPIOEXP_CONFIG_OUTPUT(E3550_CIM_PWR_ON_EXP);
//        }


        SET_GPIOEXP_OUTPUT_HIGH(CIM_RST_N_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CIM_RST_N_EXP);

        waitTime(5*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
        CIM_LSW_Initia();
        CIM_ALWAYS_PRINTF(("CIM powered on\r\n"));

        /**
         * \req: On Aurix Boot:  Xavier-1 is ALWAYS chosen as Frame Sync source
         */

        Resource_SetFsyncSrcState(TEGRA_A);
        CIM_ALWAYS_PRINTF(("Frame Sync source assigned to Tegra-A\r\n"));
        /**
         * \req: On Aurix Boot:  All 4 FRSYNC buffers are to be enabled by default
         */
        Resource_SetFsyncEnState(((CAMERA_MODULE_A<<4)|CAM_AGG_FRSYNC_SRC_TEGRA_A), FRSYNC_ON);
        Resource_SetFsyncEnState(((CAMERA_MODULE_B<<4)|CAM_AGG_FRSYNC_SRC_TEGRA_A), FRSYNC_ON);
        Resource_SetFsyncEnState(((CAMERA_MODULE_C<<4)|CAM_AGG_FRSYNC_SRC_TEGRA_A), FRSYNC_ON);
        Resource_SetFsyncEnState(((CAMERA_MODULE_D<<4)|CAM_AGG_FRSYNC_SRC_TEGRA_A), FRSYNC_ON);
        CIM_ALWAYS_PRINTF(("Frame Sync Enabled for all 4 Aggregators\r\n"));
    }
    else
    {
        //for VW80000 test
        if(g_PowerDown_flag==FALSE){ 
            CIM_LSW_Initia();
            g_PowerDown_flag=TRUE;
       }
		
        CIM_ALWAYS_PRINTF(("CIM set Io exp low \r\n"));

        /* Set IO expander in CIM module all pins to low */
        CIM_Set_Io_Expander_Pins_Low();

//        if(board.project == E3550)
//        {
//			SET_GPIOEXP_OUTPUT_LOW(E3550_CIM_PWR_ON_EXP);
//			SET_GPIOEXP_CONFIG_OUTPUT(E3550_CIM_PWR_ON_EXP);
//        }

        SET_GPIOEXP_OUTPUT_LOW(CIM_RST_N_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CIM_RST_N_EXP);
        CIM_ALWAYS_PRINTF(("CIM powered off\r\n"));
    }
}

/** \brief Enable/Disable LSW power channels
 *  \param devAddr : I2c address of LSW device
 *  \param devChannel : Channel of Quad Load Switch
 *  \param powerEnable : 0 - disable power, 1 - enable power
 *
 */
boolean CIM_LSW_Control(t_i2c_device* dev_ptr, uint8 devChannel, boolean powerEnable)
{
    boolean ret;
    uint32 bytesleft;
    uint8 value;

    bytesleft = i2c_read_bytes(dev_ptr->bus, dev_ptr->address, CIM_LSW_MAX20087_CONFIG_REG, 1, &value, 1);
    if (powerEnable)
    {
        value |= (CIM_LSW_MAX20087_CONFIG_EN_MASK & devChannel);
    }
    else
    {
        value &= (~(CIM_LSW_MAX20087_CONFIG_EN_MASK & devChannel));
    }

    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, CIM_LSW_MAX20087_CONFIG_REG, 1, &value);

    waitTime(5*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay

    /* verify the result */
    // we need to manually clear value, as if i2cread fail, the value of "value" will not change
    value = 0;
    bytesleft = i2c_read_bytes(dev_ptr->bus, dev_ptr->address, CIM_LSW_MAX20087_CONFIG_REG, 1, &value, 1);
    value &= CIM_LSW_MAX20087_CONFIG_EN_MASK;

    if (powerEnable)
    {
        if(value & devChannel)
            ret = TRUE;
        else
            ret = FALSE;
    }
    else
    {
        if(value & devChannel)
            ret = FALSE;
        else
            ret = TRUE;
    }

    return ret;
}

boolean CIM_Check_SGood_Status()
{
    return TRUE;
}

void readCamVoltage(t_i2c_device* dev_ptr, float* camData)
{
    uint8 value;

    value = VOLTAGE_CONFIG_SETTING_COMMAND; 
    i2c_write_bytes(dev_ptr->bus, dev_ptr->address, CONFIG_REGISTER_COMMAND, 1, &value);
    DelayMs(5);

    for(int i = 0; i < NUM_ADC_UNITS; i++)
    {
        CAMMONITOR_DEBUG_PRINTF(("Voltage: BUS --> %d ; ADDR --> 0x%02x ; command --> 0x%02x.\r\n", dev_ptr->bus, dev_ptr->address, (ADC1_REGISTER_COMMAND + i)));
	    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, (ADC1_REGISTER_COMMAND + i), 1, &value, 1);
	    CAMMONITOR_DEBUG_PRINTF(("Voltage: ADC %d raw data is 0x%02x.\r\n", i, value));
	    camData[i] = (float)value*70/1000;
    }
}

void readCamCurrent(t_i2c_device* dev_ptr, float* camData)
{
    uint8 value;

    value = CURRENT_CONFIG_SETTING_COMMAND; 
    i2c_write_bytes(dev_ptr->bus, dev_ptr->address, CONFIG_REGISTER_COMMAND, 1, &value);
    DelayMs(5);

    for(int i = 0; i < NUM_ADC_UNITS; i++)
    {
	    CAMMONITOR_DEBUG_PRINTF(("Current: BUS --> %d ; ADDR --> 0x%02x ; command --> 0x%02x.\r\n", dev_ptr->bus, dev_ptr->address, (ADC1_REGISTER_COMMAND + i)));
	    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, (ADC1_REGISTER_COMMAND + i), 1, &value, 1);
	    CAMMONITOR_DEBUG_PRINTF(("Current: ADC %d raw data is 0x%02x.\r\n", i, value));
	    camData[i] = ((float)value*3)/1000;
    }
}

#endif //CAMERA_CIM_ENABLE == 1
