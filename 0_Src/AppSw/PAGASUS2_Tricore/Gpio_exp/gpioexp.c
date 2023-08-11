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
#include "gpioexp.h"
#include "SysSe/Bsp/Bsp.h"
#include <internal_fw_debug.h>
#include "resource_state_manage.h"
#include "configuration.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define GPIO_EXP_DEBUG_ENABLE 0

#if GPIO_EXP_DEBUG_ENABLE == 1
#define GPIO_EXP_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define GPIO_EXP_DEBUG_PRINTF(x)
#endif

#define GPIO_EXP_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)

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

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
boolean readConfig(uint8 i2cbus, uint8 devAddr, uint8 group, uint8 port)
{
/* If GPIO expander is not enalbed */
#if GPIO_EXPANDER_ENABLE == 0
	GPIO_EXP_ALWAYS_PRINTF(("IO expander is not enabled\n"));
	return FALSE;
#endif
/*
	GPIO_EXP_DEBUG_PRINTF(("%s: i2cbus %d, devAddr 0x%x, group %d, port%d\r\n",
			__func__, i2cbus, devAddr, group, port));
*/
	uint32 bytesleft;
	uint8 offset, value;
	if (group)
	{
		offset = GPIO_EXP_CONFIGURATION_REGISTER_HI;
	}
	else
	{
		offset = GPIO_EXP_CONFIGURATION_REGISTER_LO;
	}
	bytesleft = i2c_read_bytes(i2cbus,devAddr, offset, 1, &value,1);
	return (boolean)((value & (1<<port)) >>port);
}

boolean readState(uint8 i2cbus, uint8 devAddr, uint8 group, uint8 port)
{
/* If GPIO expander is not enalbed */
#if GPIO_EXPANDER_ENABLE == 0
	GPIO_EXP_ALWAYS_PRINTF(("IO expander is not enabled\n"));
	return FALSE;
#endif
/*
	GPIO_EXP_DEBUG_PRINTF(("%s: i2cbus %d, devAddr 0x%x, group %d, port %d\r\n",
				__func__, i2cbus, devAddr, group, port));
*/
	uint32 bytesleft;
	uint8 offset, value;
	if (group)
	{
		offset = GPIO_EXP_INPUT_REGISTER_HI;
	}
	else
	{
		offset = GPIO_EXP_INPUT_REGISTER_LO;
	}
	bytesleft = i2c_read_bytes(i2cbus,devAddr, offset, 1, &value,1);
	return (boolean)((value & (1<<port)) >>port);
}

void setConfig(uint8 i2cbus, uint8 devAddr, uint8 group, uint8 port, boolean setInput)
{
/* If GPIO expander is not enalbed */
#if GPIO_EXPANDER_ENABLE == 0
	GPIO_EXP_ALWAYS_PRINTF(("IO expander is not enabled\n"));
	return;
#endif
	GPIO_EXP_DEBUG_PRINTF(("%s: i2cbus %d, devAddr 0x%x, group %d, port %d, setInput %d\r\n",
				__func__, i2cbus, devAddr, group, port, setInput));
	uint32 bytesleft;
	uint8 newvalue, value, offset;
	if (group)
	{
		offset = GPIO_EXP_CONFIGURATION_REGISTER_HI;
	}
	else
	{
		offset = GPIO_EXP_CONFIGURATION_REGISTER_LO;
	}
	bytesleft = i2c_read_bytes(i2cbus,devAddr, offset, 1, &value,1);
	if(setInput)
	{
		newvalue = value | (1<<port);
	}
	else
	{
		newvalue = value & (~(1<<port));
	}
	bytesleft = i2c_write_bytes(i2cbus,devAddr, offset, 1, &newvalue);
}

void setOutputState(uint8 i2cbus, uint8 devAddr, uint8 group, uint8 port, boolean state)
{
/* If GPIO expander is not enalbed */
#if GPIO_EXPANDER_ENABLE == 0
	GPIO_EXP_ALWAYS_PRINTF(("IO expander is not enabled\n"));
	return;
#endif
	GPIO_EXP_DEBUG_PRINTF(("%s: i2cbus %d, devAddr 0x%x, group %d, port %d, state %d\r\n",
					__func__, i2cbus, devAddr, group, port, state));
	uint32 bytesleft;
	uint8 newvalue, value, offset;
	if (group)
	{
		offset = GPIO_EXP_OUTPUT_REGISTER_HI;
	}
	else
	{
		offset = GPIO_EXP_OUTPUT_REGISTER_LO;
	}
	bytesleft = i2c_read_bytes(i2cbus,devAddr, offset, 1, &value,1);
	if(state)
	{
		newvalue = value | (1<<port);
	}
	else
	{
		newvalue = value & (~(1<<port));
	}
	bytesleft = i2c_write_bytes(i2cbus, devAddr, offset, 1, &newvalue);
}

typedef enum
{
	OUTPUT = 0,
	INPUT,
	INGORE_PIN
}e_direction;

typedef struct
{
	uint8 address;
	uint8 port;
	uint8 pin;
	e_direction dir;
	uint8 value;
}t_exp_gpio_table;

#define OUTPUT_HIGH  1
#define OUTPUT_LOW   0
#define NOT_CARE  0xFF

#if PROJECT == PROJECT_PAGASUS2
static const t_exp_gpio_table EXP_GPIO_INACTIVE_TABLE[]=
{
	/* 0x74 U55 */
	{GPIO_EXP0_I2C_ADDR, 0, 0, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 0, 1, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 0, 2, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 0, 3, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 0, 4, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 0, 5, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 0, 6, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 0, 7, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 0, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 1, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 2, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 3, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 4, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 5, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 6, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 7, INPUT, NOT_CARE},
};
#else
static const t_exp_gpio_table EXP_GPIO_INACTIVE_TABLE[]=
{
	/* 0x74 U55 */
	{GPIO_EXP0_I2C_ADDR, 0, 0, OUTPUT, OUTPUT_LOW},
	{GPIO_EXP0_I2C_ADDR, 0, 1, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 0, 2, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 0, 3, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 0, 4, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 0, 5, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 0, 6, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 0, 7, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 0, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 1, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 2, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 3, INPUT, NOT_CARE},
	/* It's control by Resource management, we ignore it */
	{GPIO_EXP0_I2C_ADDR, 1, 4, INGORE_PIN, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 5, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 6, INPUT, NOT_CARE},
	{GPIO_EXP0_I2C_ADDR, 1, 7, OUTPUT, OUTPUT_HIGH},

	/* 0x77 U205 */
	{GPIO_EXP1_I2C_ADDR, 0, 0, OUTPUT, OUTPUT_LOW},
	{GPIO_EXP1_I2C_ADDR, 0, 1, OUTPUT, OUTPUT_LOW},
	{GPIO_EXP1_I2C_ADDR, 0, 2, OUTPUT, OUTPUT_LOW},
	{GPIO_EXP1_I2C_ADDR, 0, 3, INPUT, NOT_CARE},
	{GPIO_EXP1_I2C_ADDR, 0, 4, INPUT, NOT_CARE},
	{GPIO_EXP1_I2C_ADDR, 0, 5, INPUT, NOT_CARE},
	{GPIO_EXP1_I2C_ADDR, 0, 6, INPUT, NOT_CARE},
	{GPIO_EXP1_I2C_ADDR, 0, 7, INPUT, NOT_CARE},
	{GPIO_EXP1_I2C_ADDR, 1, 0, OUTPUT, OUTPUT_LOW},
	{GPIO_EXP1_I2C_ADDR, 1, 1, OUTPUT, OUTPUT_LOW},
	{GPIO_EXP1_I2C_ADDR, 1, 2, OUTPUT, OUTPUT_LOW},
	/* It's control by power on sequence, we ignore it */
	{GPIO_EXP1_I2C_ADDR, 1, 3, INGORE_PIN, NOT_CARE},
	{GPIO_EXP1_I2C_ADDR, 1, 4, INGORE_PIN, NOT_CARE},
	{GPIO_EXP1_I2C_ADDR, 1, 5, INPUT, NOT_CARE},
	{GPIO_EXP1_I2C_ADDR, 1, 6, INPUT, NOT_CARE},
	{GPIO_EXP1_I2C_ADDR, 1, 7, INPUT, NOT_CARE},

};
#endif  //#if PROJECT == PROJECT_DRIVE_AX

static void SetExpPin(const t_exp_gpio_table* gpio_table_p)
{
	if(gpio_table_p->dir == INGORE_PIN)
	{
		GPIO_EXP_DEBUG_PRINTF(("EXP 0x%x port %d pin %d, ignore\n",
				gpio_table_p->address, gpio_table_p->port, gpio_table_p->pin));
	}
	else if(gpio_table_p->dir == INPUT)
	{
		setConfig(GPIO_EXP_I2C_BUS, gpio_table_p->address,
				gpio_table_p->port, gpio_table_p->pin, 1);
		GPIO_EXP_DEBUG_PRINTF(("EXP 0x%x port %d pin %d, set to input\n",
				gpio_table_p->address, gpio_table_p->port, gpio_table_p->pin));
	}
	else
	{
		setOutputState(GPIO_EXP_I2C_BUS, gpio_table_p->address,
				gpio_table_p->port, gpio_table_p->pin,  gpio_table_p->value);
		setConfig(GPIO_EXP_I2C_BUS, gpio_table_p->address,
						gpio_table_p->port, gpio_table_p->pin, 0);
		GPIO_EXP_DEBUG_PRINTF(("EXP 0x%x port %d pin %d, set to output %d\n",
						gpio_table_p->address, gpio_table_p->port, gpio_table_p->pin, gpio_table_p->value));
	}
}

void SetExpOutputs_to_System_PowerOff_State(void)
{
#if GPIO_EXPANDER_ENABLE == 1
	GPIO_EXP_DEBUG_PRINTF(("%s\n", __func__));
#endif
}

void SetExpOutputs_to_System_Inactive_State(void)
{
#if GPIO_EXPANDER_ENABLE == 1
	GPIO_EXP_DEBUG_PRINTF(("%s\n", __func__));
	uint8 i;
	for(i=0; i<ARRAY_SIZE(EXP_GPIO_INACTIVE_TABLE); i++)
	{
		SetExpPin(&EXP_GPIO_INACTIVE_TABLE[i]);
	}
#if PROJECT == PROJECT_DRIVE_PX2
	Resource_SetFpdlState(TEGRA_B);

	//TODO: need to make it easier to config
	/* input: 1, output: 0 */
    setConfig(GPIO_EXP_I2C_BUS, GPIO_EXP1_I2C_ADDR, 0, 3, 1);
    setConfig(GPIO_EXP_I2C_BUS, GPIO_EXP1_I2C_ADDR, 0, 4, 1);
#endif
#if DEBUG_ENABLE == 1
	/* need a delay to print the log out */
	waitTime(500 * TimeConst[TIMER_INDEX_1MS]);
#endif
#endif
}
