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
#include "Tsensor.h"
#include "SysSe/Bsp/Bsp.h"
#include "Configuration.h"
#include <internal_fw_debug.h>
#include "board.h"
#include "gpioexp.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define TSENSOR_DEBUG_ENABLE 0

#if TSENSOR_DEBUG_ENABLE == 1
#define TSENSOR_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define TSENSOR_DEBUG_PRINTF(x)
#endif

#define TSENSOR_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)
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
static uint32 readStatus(t_i2c_device* dev_ptr);
/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
static void init_temp_sensor(t_i2c_device* dev_ptr)
{
    uint32 bytesleft;
    uint8 value;

    value = TMP411_LOCAL_TEMP_HIGH_LIMIT_VALUES_REG;
    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_TEMP_HIGH_LIMIT_HBYTE_WRITE, 1, &value);
#if PROJECT != PROJECT_PAGASUS2
    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_TEMP_HIGH_LIMIT_LBYTE, 1, 0x00);
#endif
    value = TMP411_LOCAL_TEMP_LOW_LIMIT_VALUES_REG;
    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_TEMP_LOW_LIMIT_HBYTE_WRITE, 1, &value);
#if PROJECT != PROJECT_PAGASUS2
    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_TEMP_LOW_LIMIT_LBYTE, 1, 0x00);
#endif
    value = TMP411_REMOTE_TEMP_HIGH_LIMIT_VALUES_REG;
    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_REMOTE_TEMP_HIGH_LIMIT_HBYTE_WRITE, 1, &value);
    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_REMOTE_TEMP_HIGH_LIMIT_LBYTE, 1, 0x00);

    value = TMP411_REMOTE_TEMP_LOW_LIMIT_VALUES_REG;
    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_REMOTE_TEMP_LOW_LIMIT_HBYTE_WRITE, 1, &value);
    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_REMOTE_TEMP_LOW_LIMIT_LBYTE, 1, 0x00);

    value = TMP411_LOCAL_TEMP_THERM_LIMIT_VALUES_REG;
    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_THERM_LIMIT, 1, &value);

    value = TMP411_REMOTE_TEMP_THERM_LIMIT_VALUES_REG;
    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_REMOTE_THERM_LIMIT, 1, &value);

    value = 0x4;
    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_CONFIGURATION_WRITE, 1, &value);

    /* enable timeout feature to fix SCL and SDA are hell low.
     * set SMBTO bit to 1 */
    value = 0x81;
    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_CONSECUTIVE_ALERT, 1, &value);

#if TMP411_WAR_ENABLE == 1
    //disable Remote Alert
    value = 0xc;
    bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, 0x1a, 1, &value);
    bytesleft = i2c_read_bytes(dev_ptr->bus,0xc, 0x0, 1, &value,0);
#endif


    waitTime(500*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay

}

void initTempsensors()
{
    init_temp_sensor(&i2c_tmp451_1);
    init_temp_sensor(&i2c_tmp451_2);
    init_temp_sensor(&i2c_tmp451_3);

    tempsensor_initialized = TRUE;
}

void initAurix2Tempsensors()
{
    const uint8 alert_conf = 0x24;  // (bit5)alert default high, (bit2)range -64~191 C degree

    IfxPort_setPinModeInput(AURIX2_THERM_ALERT_1.port, AURIX2_THERM_ALERT_1.pinIndex, IfxPort_InputMode_noPullDevice);
    IfxPort_setPinModeInput(AURIX2_THERM_ALERT_2.port, AURIX2_THERM_ALERT_2.pinIndex, IfxPort_InputMode_noPullDevice);
    IfxPort_setPinModeInput(AURIX2_THERM_ALERT_3.port, AURIX2_THERM_ALERT_3.pinIndex, IfxPort_InputMode_noPullDevice);
    IfxPort_setPinModeInput(AURIX2_THERM_ALERT_4.port, AURIX2_THERM_ALERT_4.pinIndex, IfxPort_InputMode_noPullDevice);
    IfxPort_setPinModeInput(AURIX2_THERM_ALERT_5.port, AURIX2_THERM_ALERT_5.pinIndex, IfxPort_InputMode_noPullDevice);

    init_temp_sensor(&i2c_tmp451_1);
    init_temp_sensor(&i2c_tmp451_2);
    init_temp_sensor(&i2c_tmp451_3);
    init_temp_sensor(&i2c_tmp451_4);
    init_temp_sensor(&i2c_tmp451_5);

    (void) i2c_write_bytes(i2c_tmp451_1.bus, i2c_tmp451_1.address, TMP411_CONFIGURATION_WRITE, 1, &alert_conf);
    (void) i2c_write_bytes(i2c_tmp451_2.bus, i2c_tmp451_2.address, TMP411_CONFIGURATION_WRITE, 1, &alert_conf);
    (void) i2c_write_bytes(i2c_tmp451_3.bus, i2c_tmp451_3.address, TMP411_CONFIGURATION_WRITE, 1, &alert_conf);
    (void) i2c_write_bytes(i2c_tmp451_4.bus, i2c_tmp451_4.address, TMP411_CONFIGURATION_WRITE, 1, &alert_conf);
    (void) i2c_write_bytes(i2c_tmp451_5.bus, i2c_tmp451_5.address, TMP411_CONFIGURATION_WRITE, 1, &alert_conf);

    tempsensor_initialized = TRUE;
}

void clearTempsensors_status(void)
{
    readStatus(&i2c_tmp451_1);
    readStatus(&i2c_tmp451_2);
    readStatus(&i2c_tmp451_3);
}

void clearAurx2Tempsensors_status(void)
{
    readStatus(&i2c_tmp451_1);
    readStatus(&i2c_tmp451_2);
    readStatus(&i2c_tmp451_3);
    readStatus(&i2c_tmp451_4);
    readStatus(&i2c_tmp451_5);
}

float readTsensorLocal(t_i2c_device* dev_ptr)
{
    uint32 bytesleft;
    uint8 value, local_high, local_low;
    float localtemp;
    if (!tempsensor_initialized)
    {
        init_temp_sensor(dev_ptr);
    }

    bytesleft = i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_TEMP_HBYTE_READ, 1, &value,1);
    local_high = value;
    bytesleft = i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_TEMP_LBYTE_READ, 1, &value,1);
    local_low = value;
    localtemp = local_high + (((float)local_low)/16*0.0625)-64;

    return localtemp;
}

float readTsensorRemote(t_i2c_device* dev_ptr)
{
    uint32 bytesleft;
    uint8 value, remote_high, remote_low;
    float remotetemp;
    if (!tempsensor_initialized)
    {
        init_temp_sensor(dev_ptr);
    }

    bytesleft = i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_REMOTE_TEMP_HBYTE_READ, 1, &value,1);
    remote_high = value;
    bytesleft = i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_REMOTE_TEMP_LBYTE_READ, 1, &value,1);
    remote_low = value;
    remotetemp = remote_high + (((float)remote_low)/16*0.0625)-64;

    return remotetemp;
}

static uint32 readStatus(t_i2c_device* dev_ptr)
{
    uint8 value;
    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_STATUS_READ, 1, &value,1);
    return value;
}

static boolean testTsensorAlert(t_i2c_device* dev_ptr)
{
    uint8 value;
    boolean ret = FALSE;
    readStatus(dev_ptr);
    IoExpPort_Pin* ptr;

    if(dev_ptr == (&i2c_tmp451_1))
    {
        ptr = &P3570_BRD_THERM_ALERT_N_EXP_1;
    }
    if(dev_ptr == (&i2c_tmp451_2))
    {
        ptr = &P3570_BRD_THERM_ALERT_N_EXP_2;
    }
    if(dev_ptr == (&i2c_tmp451_3))
    {
        ptr = &P3570_BRD_THERM_ALERT_N_EXP_3;
    }

    if(!READ_GPIOEXP_STATE((*ptr)))
    {
        TSENSOR_ALWAYS_PRINTF(("Alert pin is low before test, Alert feature test fail!\r\n"));
        return FALSE;
    }

    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_CONFIGURATION_READ, 1, &value,1);
    value = ((value | TMP411_CONFIGURATION_RANGE_BIT_MASK) & (~TMP411_CONFIGURATION_ALERT_BIT_MASK));
    TSENSOR_DEBUG_PRINTF(("set configuration value 0x%x\r\n", value));
    i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_CONFIGURATION_WRITE, 1, &value);

    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_TEMP_HBYTE_READ, 1, &value,1);
    TSENSOR_ALWAYS_PRINTF(("local temp high byte is 0x%x\r\n", value));

    value--;
    i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_TEMP_HIGH_LIMIT_HBYTE_WRITE, 1, &value);
    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_TEMP_HIGH_LIMIT_HBYTE_READ, 1, &value,1);
    TSENSOR_ALWAYS_PRINTF(("set local temp limit high byte to 0x%x\r\n", value));

    waitTime(100*TimeConst[TIMER_INDEX_1MS]);

    if(!READ_GPIOEXP_STATE((*ptr)))
    {
        TSENSOR_ALWAYS_PRINTF(("Alert pin change to LOW, Alert feature test PASS\r\n"));
        ret = TRUE;
    }
    else
    {
        TSENSOR_ALWAYS_PRINTF(("Alert pin is still HIGH, Alert feature test FAIL\r\n"));
        ret = FALSE;
    }

    readStatus(dev_ptr);
    return ret;
}

static boolean testAurix2TsensorAlert(t_i2c_device* dev_ptr)
{
    uint8 value;
    boolean ret = FALSE;
    readStatus(dev_ptr);
    IfxPort_Pin* a2_alert_pin;

    if(dev_ptr == (&i2c_tmp451_1))
    {
        a2_alert_pin = &AURIX2_THERM_ALERT_1;
    }
    if(dev_ptr == (&i2c_tmp451_2))
    {
        a2_alert_pin = &AURIX2_THERM_ALERT_2;
    }
    if(dev_ptr == (&i2c_tmp451_3))
    {
        a2_alert_pin = &AURIX2_THERM_ALERT_3;
    }
    if(dev_ptr == (&i2c_tmp451_4))
    {
        a2_alert_pin = &AURIX2_THERM_ALERT_4;
    }
    if(dev_ptr == (&i2c_tmp451_5))
    {
        a2_alert_pin = &AURIX2_THERM_ALERT_5;
    }

    if(!IfxPort_getPinState(a2_alert_pin->port, a2_alert_pin->pinIndex))
    {
        TSENSOR_ALWAYS_PRINTF(("A2 Alert pin is low before test, Alert feature test fail!\r\n"));
        return FALSE;
    }

    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_CONFIGURATION_READ, 1, &value,1);
    value = ((value | TMP411_CONFIGURATION_RANGE_BIT_MASK) & (~TMP411_CONFIGURATION_ALERT_BIT_MASK));
    TSENSOR_DEBUG_PRINTF(("set configuration value 0x%x\r\n", value));
    i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_CONFIGURATION_WRITE, 1, &value);

    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_TEMP_HBYTE_READ, 1, &value,1);
    TSENSOR_ALWAYS_PRINTF(("local temp high byte is 0x%x\r\n", value));

    value--;
    i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_TEMP_HIGH_LIMIT_HBYTE_WRITE, 1, &value);
    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_TEMP_HIGH_LIMIT_HBYTE_READ, 1, &value,1);
    TSENSOR_ALWAYS_PRINTF(("set local temp limit high byte to 0x%x\r\n", value));

    waitTime(100*TimeConst[TIMER_INDEX_1MS]);

    if(!IfxPort_getPinState(a2_alert_pin->port, a2_alert_pin->pinIndex))
    {
        TSENSOR_ALWAYS_PRINTF(("Aurix2 Alert pin change to LOW, Alert feature test PASS\r\n"));
        ret = TRUE;
    }
    else
    {
        TSENSOR_ALWAYS_PRINTF(("Aurix2 Alert pin is still HIGH, Alert feature test FAIL\r\n"));
        ret = FALSE;
    }

    readStatus(dev_ptr);
    return ret;
}

static boolean testTsensorShutdown(t_i2c_device* dev_ptr)
{
    uint8 value;
    boolean ret = FALSE;
    readStatus(dev_ptr);

    if(!IfxPort_getPinState(BRD_THERM_SHDN_N.port, BRD_THERM_SHDN_N.pinIndex))
    {
        TSENSOR_ALWAYS_PRINTF(("Shutdown pin is low before test, Shutdown feature test fail!\r\n"));
        return FALSE;
    }

    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_CONFIGURATION_READ, 1, &value,1);
    value = ((value | TMP411_CONFIGURATION_RANGE_BIT_MASK) & (~TMP411_CONFIGURATION_ALERT_BIT_MASK));
    TSENSOR_DEBUG_PRINTF(("set configuration value 0x%x\r\n", value));
    i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_CONFIGURATION_WRITE, 1, &value);


    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_TEMP_HBYTE_READ, 1, &value,1);
    TSENSOR_ALWAYS_PRINTF(("local temp high byte is 0x%x\r\n", value));

    value--;
    i2c_write_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_THERM_LIMIT, 1, &value);
    i2c_read_bytes(dev_ptr->bus, dev_ptr->address, TMP411_LOCAL_THERM_LIMIT, 1, &value,1);
    TSENSOR_ALWAYS_PRINTF(("set local temp limit high byte to 0x%x\r\n", value));

    waitTime(100*TimeConst[TIMER_INDEX_1MS]);

    if(!IfxPort_getPinState(BRD_THERM_SHDN_N.port, BRD_THERM_SHDN_N.pinIndex))
    {
        TSENSOR_ALWAYS_PRINTF(("Shutdown pin change to LOW, shutdown feature test PASS\r\n"));
        ret = TRUE;
    }
    else
    {
        TSENSOR_ALWAYS_PRINTF(("Shutdown pin is still HIGH, shutdown feature test FAIL\r\n"));
        ret = FALSE;
    }

    readStatus(dev_ptr);
    return ret;
}

void testTsensor(t_i2c_device* dev_ptr, uint32 loops)
{
    boolean ret = TRUE;
    while(loops > 0)
    {
        TSENSOR_ALWAYS_PRINTF(("Start test thermal sensor at loop %d\r\n", loops));
        loops--;
        ret = testTsensorAlert(dev_ptr) && ret;
        ret = testTsensorShutdown(dev_ptr) && ret;
        init_temp_sensor(dev_ptr);
        readStatus(dev_ptr);
    }
    if(ret)
    {
        TSENSOR_ALWAYS_PRINTF(("\nThermal Sensor Test Pass!\r\n"));
    }
    else
    {
        TSENSOR_ALWAYS_PRINTF(("\nThermal Sensor Test Fail!\r\n"));
    }
}

void testAurix2Tsensor(t_i2c_device* dev_ptr, uint32 loops)
{
    boolean ret = TRUE;
    while(loops > 0)
    {
        TSENSOR_ALWAYS_PRINTF(("Start test thermal sensor at loop %d\r\n", loops));
        loops--;
        ret = testAurix2TsensorAlert(dev_ptr) && ret;
        init_temp_sensor(dev_ptr);
        readStatus(dev_ptr);
    }
    if(ret)
    {
        TSENSOR_ALWAYS_PRINTF(("\nThermal Sensor Test Pass!\r\n"));
    }
    else
    {
        TSENSOR_ALWAYS_PRINTF(("\nThermal Sensor Test Fail!\r\n"));
    }
}
