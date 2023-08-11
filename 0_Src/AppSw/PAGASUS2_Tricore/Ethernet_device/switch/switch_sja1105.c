/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#include "switch_sja1105.h"
#include <NXP_SJA1105P_spi.h>
#include <NXP_SJA1105P_config.h>
#include <NXP_SJA1105P_switchCore.h>
#include <NXP_SJA1105P_clockGenerationUnit.h>
#include <NXP_SJA1105P_resetGenerationUnit.h>
#include <NXP_SJA1105P_auxiliaryConfigurationUnit.h>
#include <internal_fw_debug.h>
#include <TegraQspi.h>
#include <string.h>
#include "error_report.h"
#include "board.h"
#include "Configuration.h"


#define SJA1105_DEBUG_ENABLE 1

#if SJA1105_DEBUG_ENABLE == 1
#define SJA1105_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define SJA1105_DEBUG_PRINTF(x)
#endif

#define SJA1105_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)

#define SPI_TMP_BUFFER_SIZE  256

#define SJA1105_CONFIG_STATUS_REGISTER_OFFSET     1
#define SJA1105_CONFIG_STATUS_CONFIGS_BIT_OFFSET  31
#define SJA1105_DEVICE_ID  0xae00030e

typedef enum
{
    SJA1105P_SPI_RD_OP,
    SJA1105P_SPI_WR_OP,
    SJA1105P_SPI_MAX_OP
}eSja1105SpiOp;

#if 1
static uint32_t SJA1105P_getControlPhase(eSja1105SpiOp op, uint8_t wordCount, uint32_t registerAddress)
{
    uint32_t controlPhase = 0;
    if(op == SJA1105P_SPI_WR_OP)
    {
        controlPhase |= SJA1105_SPI_CONTROL_PHASE_BIT_OP_WR;
    }
    else
    {
        controlPhase |= SJA1105_SPI_CONTROL_PHASE_BIT_COUNT_OF_DWORD(wordCount);
    }
    controlPhase = controlPhase | SJA1105_SPI_CONTROL_PHASE_BIT_REG_ADDR(registerAddress);
    SJA1105_DEBUG_PRINTF(("controlPhase 0x%08x\r\n", controlPhase));
    return controlPhase;
}



/* the function used to perform read operations via SPI */
static uint8_t SJA1105P_spiRead32(uint8_t deviceSelect, uint8_t wordCount, uint32_t registerAddress, uint32_t *p_registerValue)
{
    uint8_t ret = 0;
    uint32 controlPhase;
    uint32 written_array[SPI_TMP_BUFFER_SIZE];
    uint32 read_array[SPI_TMP_BUFFER_SIZE];
    memset(written_array, 0, sizeof(written_array));
    memset(read_array, 0, sizeof(read_array));

    /* Transfer it to the required format */
    /* Step 1: get control phase */
    controlPhase = SJA1105P_getControlPhase(SJA1105P_SPI_RD_OP, wordCount, registerAddress);

    /* Step 2: transfer control phase + dummy data */
    written_array[0] = controlPhase;

    /* Step 3: write control phase, and then read data at next 32bit cycle */
    TegraQspiMaster_read_write(1, wordCount + 1, written_array, read_array);

    /* byteCount = wordCount*4 */
    memcpy(p_registerValue, &read_array[1], wordCount*4);

#if 0 //NEED?
    SJA1105_DEBUG_PRINTF(("%s: deviceSelect %d, wordCount %d, registerAddress 0x%x\r\n",
                __func__, deviceSelect, wordCount, registerAddress));
#if SJA1105_DEBUG_ENABLE == 1 && DEBUG_ENABLE == 1
    DEBUG_PRINTF_BUFFER_32BIT(p_registerValue, wordCount);
#endif
#endif
    return ret;
}
/* the function used to perform write operations via SPI */
static uint8_t SJA1105P_spiWrite32(uint8_t deviceSelect, uint8_t wordCount, uint32_t registerAddress, uint32_t *p_registerValue)
{
    uint8_t ret = 0;
    uint32 controlPhase;
    uint32 dummy_array[SPI_TMP_BUFFER_SIZE];
    uint32 written_array[SPI_TMP_BUFFER_SIZE];
#if 0 //NEED?
    SJA1105_DEBUG_PRINTF(("%s: deviceSelect %d, wordCount %d, registerAddress 0x%08x, value to write is:\r\n",
                    __func__, deviceSelect, wordCount, registerAddress));
#if SJA1105_DEBUG_ENABLE == 1 && DEBUG_ENABLE == 1
    DEBUG_PRINTF_BUFFER_32BIT(p_registerValue, wordCount);
#endif
#endif
    /* Transfer it to the required format */
    /* Step 1: get control phase */
    controlPhase = SJA1105P_getControlPhase(SJA1105P_SPI_WR_OP, wordCount, registerAddress);
    /* Step 2: transfer control phase + written data */
    written_array[0] = controlPhase;
    memcpy(&written_array[1], p_registerValue, wordCount*4);
    TegraQspiMaster_read_write(1, wordCount+1, written_array, dummy_array);
    return ret;
}
#endif

static void Switch_SJA1105_Extra_Board_Initial(void)
{
    /* Extra step to set MII RX_er to internal pull down for port 0
     * This is required in P3479
     * And this setting has no effect on E3550 */
    uint32_t val = 0;
    Switch_SJA1105_Register_Read(1, 0x100801, &val);
    val |= 0x300;
    Switch_SJA1105_Register_Write(1, 0x100801, &val);

    //Disable VLAN 200
    val = 0x40000000;
    Switch_SJA1105_Register_Write(1, 0x2D, &val);
    val = 0x003FFF06;
    Switch_SJA1105_Register_Write(1, 0x2E, &val);
    val = 0xC0000000;
    Switch_SJA1105_Register_Write(1, 0x30, &val);

    //Enable VLAN 200 only between Xavier-A and Aurix Ports (P0 and P1)
    val = 0x40000000;
    Switch_SJA1105_Register_Write(1, 0x2D, &val);
    val = 0x00063106;
    Switch_SJA1105_Register_Write(1, 0x2E, &val);
    val = 0xC8000000;
    Switch_SJA1105_Register_Write(1, 0x30, &val);
}


#if 1
void Switch_SJA1105_Step2_Init(void)
{
    return;
    uint32_t val = 0;
    SJA1105_ALWAYS_PRINTF(("Reset port 0,3,4 Rxc delay line\r\n"));

    /* PORT 4 */
    /* 0x100814 : cfg_pad_mii4_id */
    Switch_SJA1105_Register_Read(1, 0x100814, &val);
    /* Set Bypass mode and power down the delay line */
    val |= 0x0300;
    Switch_SJA1105_Register_Write(1, 0x100814, &val);

    waitTime(TimeConst[TIMER_INDEX_1MS]);

    /* power on the delay line */
    val &= ~0x0100;
    Switch_SJA1105_Register_Write(1, 0x100814, &val);

    waitTime(TimeConst[TIMER_INDEX_1MS]);

    /* disable bypass mode after delay line is powered up */
    val &= ~0x0300;
    Switch_SJA1105_Register_Write(1, 0x100814, &val);

    /* PORT 3 */
    /* 0x100813 : cfg_pad_mii3_id */
    Switch_SJA1105_Register_Read(1, 0x100813, &val);
    /* Set Bypass mode and power down the delay line */
    val |= 0x0300;
    Switch_SJA1105_Register_Write(1, 0x100813, &val);

    waitTime(TimeConst[TIMER_INDEX_1MS]);

    /* power on the delay line */
    val &= ~0x0100;
    Switch_SJA1105_Register_Write(1, 0x100813, &val);

    waitTime(TimeConst[TIMER_INDEX_1MS]);

    /* disable bypass mode after delay line is powered up */
    val &= ~0x0300;
    Switch_SJA1105_Register_Write(1, 0x100813, &val);


    /* PORT 0 */
   /* 0x100810 : cfg_pad_mii0_id */
   Switch_SJA1105_Register_Read(1, 0x100810, &val);
   /* Set Bypass mode and power down the delay line */
   val |= 0x0300;
   Switch_SJA1105_Register_Write(1, 0x100810, &val);

   waitTime(TimeConst[TIMER_INDEX_1MS]);

   /* power on the delay line */
   val &= ~0x0100;
   Switch_SJA1105_Register_Write(1, 0x100810, &val);

   waitTime(TimeConst[TIMER_INDEX_1MS]);

   /* disable bypass mode after delay line is powered up */
   val &= ~0x0300;
   Switch_SJA1105_Register_Write(1, 0x100810, &val);
}


void Switch_SJA1105_Initial(void)
{
    uint32_t id;
    uint32_t val = 0;

    SJA1105_DEBUG_PRINTF(("%s\r\n", __func__));
    /* Register SPI drivers */
    SJA1105P_registerSpiRead32CB(SJA1105P_spiRead32);
    SJA1105P_registerSpiWrite32CB(SJA1105P_spiWrite32);

    /* Initial SJA1105 */
    SJA1105P_initSwitch();

    if(board.project == P3479)
        Switch_SJA1105_Extra_Board_Initial();

    SJA1105P_getDeviceId(&id, 0);

    if(id != SJA1105_DEVICE_ID)
    {
        SJA1105_ALWAYS_PRINTF(("SJA1105 id is not correct\r\n"));
        set_error_flag(INVALID_SJA1105_DEVICE_ERR);
    }
    else
    {
        // read config status flag register 0ffset 1, bit 31
        Switch_SJA1105_Register_Read(1, SJA1105_CONFIG_STATUS_REGISTER_OFFSET, &val);
        if((val & (0x1 << SJA1105_CONFIG_STATUS_CONFIGS_BIT_OFFSET)) != 0)
        {
            SJA1105_ALWAYS_PRINTF(("Init SJA1105 sucessfully\r\n"));
        }
        else
        {
            SJA1105_ALWAYS_PRINTF(("Fail to Init SJA1105!!\r\n"));
            set_error_flag(SJA1105_INIT_FAIL_ERR);
        }
    }
}

void Switch_SJA1105_ExposeAurix_Initial(void)
{
    uint32_t id;
    uint32_t val = 0;

    SJA1105_DEBUG_PRINTF(("%s\r\n", __func__));
    /* Register SPI drivers */
    SJA1105P_registerSpiRead32CB(SJA1105P_spiRead32);
    SJA1105P_registerSpiWrite32CB(SJA1105P_spiWrite32);
    /* Initial SJA1105 */
    SJA1105P_exposeAurix_initSwitch();

    SJA1105P_getDeviceId(&id, 0);

    if(id != SJA1105_DEVICE_ID)
    {
        SJA1105_ALWAYS_PRINTF(("SJA1105 id is not correct\r\n"));
        set_error_flag(INVALID_SJA1105_DEVICE_ERR);
    }
    else
    {
        // read config status flag register 0ffset 1, bit 31
        Switch_SJA1105_Register_Read(1, SJA1105_CONFIG_STATUS_REGISTER_OFFSET, &val);
        if((val & (0x1 << SJA1105_CONFIG_STATUS_CONFIGS_BIT_OFFSET)) != 0)
        {
            SJA1105_ALWAYS_PRINTF(("Init SJA1105 ExposeAurix sucessfully\r\n"));
        }
        else
        {
            SJA1105_ALWAYS_PRINTF(("Fail to Init SJA1105!!\r\n"));
            set_error_flag(SJA1105_INIT_FAIL_ERR);
            }
        }
}

void Switch_SJA1105_LoadSim_Initial(void)
{
    uint32_t id;
    uint32_t val = 0;

    SJA1105_DEBUG_PRINTF(("%s\r\n", __func__));
    /* Register SPI drivers */
    SJA1105P_registerSpiRead32CB(SJA1105P_spiRead32);
    SJA1105P_registerSpiWrite32CB(SJA1105P_spiWrite32);
    /* Initial SJA1105 */
    SJA1105P_loadsim_initSwitch();

    SJA1105P_getDeviceId(&id, 0);

    if(id != SJA1105_DEVICE_ID)
    {
        SJA1105_ALWAYS_PRINTF(("SJA1105 id is not correct\r\n"));
        set_error_flag(INVALID_SJA1105_DEVICE_ERR);
    }
    else
    {
        // read config status flag register 0ffset 1, bit 31
        Switch_SJA1105_Register_Read(1, SJA1105_CONFIG_STATUS_REGISTER_OFFSET, &val);
        if((val & (0x1 << SJA1105_CONFIG_STATUS_CONFIGS_BIT_OFFSET)) != 0)
        {
            SJA1105_ALWAYS_PRINTF(("Init SJA1105 Loadsim sucessfully\r\n"));
        }
        else
        {
            SJA1105_ALWAYS_PRINTF(("Fail to Init SJA1105!!\r\n"));
            set_error_flag(SJA1105_INIT_FAIL_ERR);
            }
        }
}

void Switch_SJA1105_Register_Read(uint8 wordCount, uint32 registerAddress, uint32 *p_registerValue)
{
    SJA1105P_spiRead32(0, wordCount, registerAddress, p_registerValue);
}

void Switch_SJA1105_Register_Write(uint8 wordCount, uint32 registerAddress, uint32 *p_registerValue)
{
    SJA1105P_spiWrite32(0, wordCount, registerAddress, p_registerValue);
}


void Switch_SJA1105_Device_Register_Read(uint8 devId,uint8 wordCount, uint32 registerAddress, uint32 *p_registerValue){
    SJA1105P_spiRead32(devId, wordCount, registerAddress, p_registerValue);
}

void Switch_SJA1105_Device_Register_Write(uint8 devId,uint8 wordCount, uint32 registerAddress, uint32 *p_registerValue){
    SJA1105P_spiWrite32(devId, wordCount, registerAddress, p_registerValue);
}

#endif
