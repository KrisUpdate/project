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
#include "Configuration.h"
#include "I2c.h"
#include <I2c/I2c/IfxI2c_I2c.h>
#include "internal_fw_debug.h"
#include "board.h"
#include "resource.h"


/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
/* data buffer size will be (I2C_BUFFER_SIZE-1)
 * as it include the i2c address byte
 * set data buffer to 16 to meet the I2C rom page size (16) *
 * */
#define I2C_BUFFER_SIZE 17   /**< \brief Tx/Rx Buffer size */




/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct
{
    uint8 i2cTxBuffer[I2C_BUFFER_SIZE];                  /**< \brief I2c Transmit buffer */
    uint8 i2cRxBuffer[I2C_BUFFER_SIZE];                  /**< \brief I2c receive buffer */
} AppI2c_Buffer;

/** \brief I2cCpu global data */
typedef struct
{
    AppI2c_Buffer i2cBuffer;                       /**< \brief I2c buffer */
    struct
    {
    	IfxI2c_I2c               i2cHandle;        /**< \brief I2c handle */
    	IfxI2c_I2c_Device        i2cDev;           /**< \brief I2c Device handles to I2C device  */
    }drivers;
} App_I2c_Cpu;


/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
#if ISR_PROVIDER_I2C == 0
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
#elif ISR_PROVIDER_I2C == 1
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
#elif ISR_PROVIDER_I2C == 2
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
#error "Set ISR_PROVIDER_I2C to a valid value!"
#endif

App_I2c_Cpu g_I2c_Cpu[NUM_I2C_MODULE]; /**< \brief I2c global data */

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

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
/* pin configuration */
const IfxI2c_Pins i2c_pins_e3550[] = {
								{&AURIX_I2C_SCL,                                 /* SCL */
								 &AURIX_I2C_SDA,                                 /* SDA */
                                 IfxPort_PadDriver_ttlSpeed1},		              /* pad driver mode */
 								{&AURIX_I2C1_SCL,                                 /* SCL */
 								 &AURIX_I2C1_SDA,                                 /* SDA */
                                  IfxPort_PadDriver_ttlSpeed1}		              /* pad driver mode */
};

const IfxI2c_Pins i2c_pins_p3479_A00[] = {
								{&AURIX_I2C_SCL,                                 /* SCL */
								 &AURIX_I2C_SDA,                                 /* SDA */
								 IfxPort_PadDriver_cmosAutomotiveSpeed3},		  /* pad driver mode */
 								{&AURIX_I2C1_SCL,                                 /* SCL */
 								 &AURIX_I2C1_SDA,                                 /* SDA */
								 IfxPort_PadDriver_cmosAutomotiveSpeed2}		  /* pad driver mode */
};

const IfxI2c_Pins i2c_pins_p3479_A01[] = {
								{&AURIX_I2C_SCL,                                 /* SCL */
								 &AURIX_I2C_SDA,                                 /* SDA */
								 IfxPort_PadDriver_cmosAutomotiveSpeed2},		  /* pad driver mode */
 								{&AURIX_I2C1_SCL,                                 /* SCL */
 								 &AURIX_I2C1_SDA,                                 /* SDA */
								 IfxPort_PadDriver_cmosAutomotiveSpeed2}		  /* pad driver mode */
};

//const IfxI2c_Pins i2c0_pins = {&AURIX_I2C_SCL,                                 /* SCL */
//								&AURIX_I2C_SDA,                                 /* SDA */
 //                               IfxPort_PadDriver_ttlSpeed1		              /* pad driver mode */
//};
//const IfxI2c_Pins i2c1_pins = {&AURIX_I2C1_SCL,                                 /* SCL */
//								&AURIX_I2C1_SDA,                                 /* SDA */
 //                               IfxPort_PadDriver_ttlSpeed1		              /* pad driver mode */
//};
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/** \brief I2C initialization
 *
 * This function initializes I2c in master mode.
 */

sint32 i2c_init (uint8 i2c_module, uint32 baudrate)
{
    /* disable interrupts */
    if(i2c_module >= NUM_I2C_MODULE){
        //I2C_ALWAYS_PRINTF(("i2c_check_device: V31C has no bus %d\r\n",i2c_module));
        return FALSE;
    }

    uint8 i2c_sema_count=0;
    if(0 == i2c_module){
        while(sem_wait_until_get(SEM_I2C_0, __LINE__, __func__)==FALSE){
            waitTime(TimeConst[TIMER_INDEX_1MS]);
            i2c_sema_count++;
            if(i2c_sema_count >= I2C_GET_SEMA_RETRY_COUNT){
                I2C_ALWAYS_PRINTF(("i2c_init: retry %d times!! can not get SEM_I2C_BUS0\r\n", i2c_sema_count));
                return (0);
            }
        }
    }else{
        while(sem_wait_until_get(SEM_I2C_1, __LINE__, __func__)==FALSE){
            waitTime(TimeConst[TIMER_INDEX_1MS]);
            i2c_sema_count++;
            if(i2c_sema_count >= I2C_GET_SEMA_RETRY_COUNT){
                I2C_ALWAYS_PRINTF(("i2c_init: retry %d times!! can not get SEM_I2C_BUS1\r\n", i2c_sema_count));
                return (0);
            }
        }
    }
    boolean interruptState = IfxCpu_disableInterrupts();

    // create config structure
    IfxI2c_I2c_Config i2cConfig;
    // fill structure with default values and Module address
    if (i2c_module)
    {
    	IfxI2c_I2c_initConfig(&i2cConfig, &MODULE_I2C1);
    }
    else
    {
    	IfxI2c_I2c_initConfig(&i2cConfig, &MODULE_I2C0);
    }
    // configure pins
#if 0
    if(board.project == E3550)
    {
    I2C_ALWAYS_PRINTF(("E3550\r\n"));
    	i2cConfig.pins = &i2c_pins_e3550[i2c_module];
    }
    else
    {
		if ((board.pcb_spin == 'A') && (board.pcb_version == 0))
		{ // A00
		    I2C_ALWAYS_PRINTF(("E3550 A00\r\n"));
			i2cConfig.pins = &i2c_pins_p3479_A00[i2c_module];
		}
		else
		{ // A01
		    I2C_ALWAYS_PRINTF(("E3550 A01\r\n"));
			i2cConfig.pins = &i2c_pins_p3479_A01[i2c_module];
		}
    }
#else
    i2cConfig.pins = &i2c_pins_e3550[i2c_module];
#endif


    if(baudrate==0)
    {
    	baudrate = 400000;
    }
    I2C_DEBUG_PRINTF(("initialize i2c bus %d at baudrate %d\r\n", i2c_module, baudrate));
    i2cConfig.baudrate = baudrate; // 400 kHz
    // initialize module
    IfxI2c_I2c_initModule(&g_I2c_Cpu[i2c_module].drivers.i2cHandle, &i2cConfig);
    {
    	// create device config
    	IfxI2c_I2c_deviceConfig i2cDeviceConfig;
    	// fill structure with default values and i2c Handler
    	IfxI2c_I2c_initDeviceConfig(&i2cDeviceConfig, &g_I2c_Cpu[i2c_module].drivers.i2cHandle);
    	// initialize the i2c device handle
#if PROJECT == PROJECT_PAGASUS2
        IfxI2c_I2c_initDevice(&g_I2c_Cpu[i2c_module].drivers.i2cDev, &i2cDeviceConfig);
#else
        IfxI2c_I2c_initDevice(&i2cDeviceConfig, &g_I2c_Cpu[i2c_module].drivers.i2cDev);
#endif
    	// our actual device address is 0xFF, will be overwritten later
    }
    g_I2c_Cpu[i2c_module].i2cBuffer.i2cTxBuffer[0] = 0;
    g_I2c_Cpu[i2c_module].i2cBuffer.i2cRxBuffer[0] = 0;

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);

    if(0 == i2c_module){
        sem_release(SEM_I2C_0);
    }else{
        sem_release(SEM_I2C_1);
    }

    return (0);
}

void i2c_update_gr(void)
{
    // configure pins
    if(board.project == E3550)
    {
    	IfxPort_setPinPadDriver(AURIX_I2C_SCL.pin.port, AURIX_I2C_SCL.pin.pinIndex, i2c_pins_e3550[0].padDriver);
		IfxPort_setPinPadDriver(AURIX_I2C_SDA.pin.port, AURIX_I2C_SDA.pin.pinIndex, i2c_pins_e3550[0].padDriver);

		IfxPort_setPinPadDriver(AURIX_I2C1_SCL.pin.port, AURIX_I2C1_SCL.pin.pinIndex, i2c_pins_e3550[1].padDriver);
		IfxPort_setPinPadDriver(AURIX_I2C1_SDA.pin.port, AURIX_I2C1_SDA.pin.pinIndex, i2c_pins_e3550[1].padDriver);
		I2C_ALWAYS_PRINTF(("Initial I2C GR value for E3550 \r\n"));
    }
    else
	{
		if ((board.pcb_spin == 'A') && (board.pcb_version == 0))
		{ // A00
			IfxPort_setPinPadDriver(AURIX_I2C_SCL.pin.port, AURIX_I2C_SCL.pin.pinIndex, i2c_pins_p3479_A00[0].padDriver);
			IfxPort_setPinPadDriver(AURIX_I2C_SDA.pin.port, AURIX_I2C_SDA.pin.pinIndex, i2c_pins_p3479_A00[0].padDriver);

			IfxPort_setPinPadDriver(AURIX_I2C1_SCL.pin.port, AURIX_I2C1_SCL.pin.pinIndex, i2c_pins_p3479_A00[1].padDriver);
			IfxPort_setPinPadDriver(AURIX_I2C1_SDA.pin.port, AURIX_I2C1_SDA.pin.pinIndex, i2c_pins_p3479_A00[1].padDriver);
			I2C_ALWAYS_PRINTF(("Initial I2C GR value for p3479-a00 \r\n"));
		}
		else
		{ // A01
			IfxPort_setPinPadDriver(AURIX_I2C_SCL.pin.port, AURIX_I2C_SCL.pin.pinIndex, i2c_pins_p3479_A01[0].padDriver);
			IfxPort_setPinPadDriver(AURIX_I2C_SDA.pin.port, AURIX_I2C_SDA.pin.pinIndex, i2c_pins_p3479_A01[0].padDriver);

			IfxPort_setPinPadDriver(AURIX_I2C1_SCL.pin.port, AURIX_I2C1_SCL.pin.pinIndex, i2c_pins_p3479_A01[1].padDriver);
			IfxPort_setPinPadDriver(AURIX_I2C1_SDA.pin.port, AURIX_I2C1_SDA.pin.pinIndex, i2c_pins_p3479_A01[1].padDriver);
			I2C_ALWAYS_PRINTF(("Initial I2C GR value for p3479-a01 \r\n"));
		}
	}
}

/*!
 * \brief   check that a device exist on bus
 *          return TRUE if exist
 * ...
 */
boolean i2c_check_device(uint8 i2c_module, uint8 device_address)
{
	IfxI2c_I2c_Status status;

    if(i2c_module >= NUM_I2C_MODULE){
        //I2C_ALWAYS_PRINTF(("i2c_check_device: V31C has no bus %d\r\n",i2c_module));
        return FALSE;
    }

    uint8 i2c_sema_count=0;
    if(0 == i2c_module){
        while(sem_wait_until_get(SEM_I2C_0, __LINE__, __func__)==FALSE){
            waitTime(TimeConst[TIMER_INDEX_1MS]);
            i2c_sema_count++;
            if(i2c_sema_count >= I2C_GET_SEMA_RETRY_COUNT){
                I2C_ALWAYS_PRINTF(("i2c_init: retry %d times!! can not get SEM_I2C_BUS0\r\n", i2c_sema_count));
                return (0);
            }
        }
    }else{
        while(sem_wait_until_get(SEM_I2C_1, __LINE__, __func__)==FALSE){
            waitTime(TimeConst[TIMER_INDEX_1MS]);
            i2c_sema_count++;
            if(i2c_sema_count >= I2C_GET_SEMA_RETRY_COUNT){
                I2C_ALWAYS_PRINTF(("i2c_init: retry %d times!! can not get SEM_I2C_BUS1\r\n", i2c_sema_count));
                return (0);
            }
        }
    }

	g_I2c_Cpu[i2c_module].drivers.i2cDev.deviceAddress = device_address<<1;
	// we read any register
    g_I2c_Cpu[i2c_module].i2cBuffer.i2cTxBuffer[0] =  0;   //set address to 0
	// write data to device as soon as it is ready
   	status = IfxI2c_I2c_write(&g_I2c_Cpu[i2c_module].drivers.i2cDev, &g_I2c_Cpu[i2c_module].i2cBuffer.i2cTxBuffer[0], 1);

    if(0 == i2c_module){
        sem_release(SEM_I2C_0);
    }else{
        sem_release(SEM_I2C_1);
    }

    if (status != IfxI2c_I2c_Status_ok) return FALSE;
    // device exist
    return TRUE;
}

/*!
 * \brief   read a bytes from i2c
 *          return the number of bytes which are not read (0 means all requested bytes are read)
 * ...
 */
uint8 i2c_read_bytes(uint8 i2c_module, uint8 device_address, uint8 address, uint32 cnt, uint8 *pData, boolean use_reg_address)
{
	IfxI2c_I2c_Status status;
	uint32 uiCnt, uiCnt2;

    uint8 i2c_sema_count=0;
    if(0 == i2c_module){
        while(sem_wait_until_get(SEM_I2C_0, __LINE__, __func__)==FALSE){
            waitTime(TimeConst[TIMER_INDEX_1MS]);
            i2c_sema_count++;
            if(i2c_sema_count >= I2C_GET_SEMA_RETRY_COUNT){
                I2C_ALWAYS_PRINTF(("i2c_init: retry %d times!! can not get SEM_I2C_BUS0\r\n", i2c_sema_count));
                return (0);
            }
        }
    }else{
        while(sem_wait_until_get(SEM_I2C_1, __LINE__, __func__)==FALSE){
            waitTime(TimeConst[TIMER_INDEX_1MS]);
            i2c_sema_count++;
            if(i2c_sema_count >= I2C_GET_SEMA_RETRY_COUNT){
                I2C_ALWAYS_PRINTF(("i2c_init: retry %d times!! can not get SEM_I2C_BUS1\r\n", i2c_sema_count));
                return (0);
            }
        }
    }

	g_I2c_Cpu[i2c_module].drivers.i2cDev.deviceAddress = device_address<<1;
	if (use_reg_address)
	{
		// we read register
		g_I2c_Cpu[i2c_module].i2cBuffer.i2cTxBuffer[0] =  address;   //set address
		// write data to device as soon as it is ready
		status = IfxI2c_I2c_write(&g_I2c_Cpu[i2c_module].drivers.i2cDev, &g_I2c_Cpu[i2c_module].i2cBuffer.i2cTxBuffer[0], 1);
		if (status != IfxI2c_I2c_Status_ok) return cnt;
	}
    while (cnt)
    {
        uiCnt = cnt;
        if (uiCnt > I2C_BUFFER_SIZE) uiCnt = I2C_BUFFER_SIZE;
    	// read the values
       	status = IfxI2c_I2c_read(&g_I2c_Cpu[i2c_module].drivers.i2cDev, &g_I2c_Cpu[i2c_module].i2cBuffer.i2cRxBuffer[0], uiCnt);
        if (status != IfxI2c_I2c_Status_ok) return cnt;
    	// write the data
    	for (uiCnt2=0; uiCnt2<uiCnt; uiCnt2++)
    		*pData++ = g_I2c_Cpu[i2c_module].i2cBuffer.i2cRxBuffer[uiCnt2];
       	cnt -= uiCnt;
    }

    if(0 == i2c_module){
        sem_release(SEM_I2C_0);
    }else{
        sem_release(SEM_I2C_1);
    }
    return cnt;
}

/*!
 * \brief   write a bytes to i2c device
 *          return the number of bytes which are not written (0 means all requested bytes are written)
 * ...
 */
uint8 i2c_write_bytes(uint8 i2c_module, uint8 device_address, uint8 address, uint32 cnt, uint8 *pData)
{
	IfxI2c_I2c_Status status;
	uint32 uiCnt, uiCnt2;
	// we read any register

    uint8 i2c_sema_count=0;
    if(0 == i2c_module){
        while(sem_wait_until_get(SEM_I2C_0, __LINE__, __func__)==FALSE){
            waitTime(TimeConst[TIMER_INDEX_1MS]);
            i2c_sema_count++;
            if(i2c_sema_count >= I2C_GET_SEMA_RETRY_COUNT){
                I2C_ALWAYS_PRINTF(("i2c_init: retry %d times!! can not get SEM_I2C_BUS0\r\n", i2c_sema_count));
                return (0);
            }
        }
    }else{
        while(sem_wait_until_get(SEM_I2C_1, __LINE__, __func__)==FALSE){
            waitTime(TimeConst[TIMER_INDEX_1MS]);
            i2c_sema_count++;
            if(i2c_sema_count >= I2C_GET_SEMA_RETRY_COUNT){
                I2C_ALWAYS_PRINTF(("i2c_init: retry %d times!! can not get SEM_I2C_BUS1\r\n", i2c_sema_count));
                return (0);
            }
        }
    }

	g_I2c_Cpu[i2c_module].drivers.i2cDev.deviceAddress = device_address<<1;
    g_I2c_Cpu[i2c_module].i2cBuffer.i2cTxBuffer[0] =  address;   //set address
    I2C_DEBUG_PRINTF(("I2C driver: WRITE bytes is called, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x"ENDL, i2c_module, device_address, address, cnt, *pData));
    while (cnt)
    {
		g_I2c_Cpu[i2c_module].i2cBuffer.i2cTxBuffer[0] = address;
        uiCnt = cnt;
        if (uiCnt > (I2C_BUFFER_SIZE-1)) uiCnt = I2C_BUFFER_SIZE-1;
    	// write the data
    	for (uiCnt2=0; uiCnt2<uiCnt; uiCnt2++)
    		g_I2c_Cpu[i2c_module].i2cBuffer.i2cTxBuffer[uiCnt2+1] = *pData++;
    	// read the values
       	status = IfxI2c_I2c_write(&g_I2c_Cpu[i2c_module].drivers.i2cDev, &g_I2c_Cpu[i2c_module].i2cBuffer.i2cTxBuffer[0], uiCnt+1);
        if (status != IfxI2c_I2c_Status_ok) return cnt;
       	cnt -= uiCnt;
       	address += uiCnt;
    }

    if(0 == i2c_module){
        sem_release(SEM_I2C_0);
    }else{
        sem_release(SEM_I2C_1);
    }

    return cnt;
}


