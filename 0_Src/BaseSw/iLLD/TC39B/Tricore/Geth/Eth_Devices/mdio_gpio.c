/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#include "configuration.h"
#if PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V31C || PROJECT == PROJECT_V3NA
#include "mdio_gpio.h"
#include <internal_fw_debug.h>

#define MDIO_GPIO_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)

#define c45     0
#define c45_adr 0
#define c45_wrt 1
#define c45_rdp 2
#define c45_rd  3
#define c22     1
#define c22_wrt 1
#define c22_rd  2

#define turnaround 2

static void set_mdc_output(void)
{
	IfxPort_setPinModeOutput(PHY_MDC.pin.port, PHY_MDC.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
}

static void set_mdc(uint8 val)
{
	if(val==0)
		IfxPort_setPinLow(PHY_MDC.pin.port, PHY_MDC.pin.pinIndex);
	else
		IfxPort_setPinHigh(PHY_MDC.pin.port, PHY_MDC.pin.pinIndex);
}


static void set_mdio_input(void)
{
	IfxPort_setPinModeInput(PHY_MDIO.pin.port, PHY_MDIO.pin.pinIndex,IfxPort_Mode_inputNoPullDevice);
}


static void set_mdio_output(void)
{
	IfxPort_setPinModeOutput(PHY_MDIO.pin.port, PHY_MDIO.pin.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);
}

static void set_mdio(uint8 val)
{
	if(val==0)
		IfxPort_setPinLow(PHY_MDIO.pin.port, PHY_MDIO.pin.pinIndex);
	else
		IfxPort_setPinHigh(PHY_MDIO.pin.port, PHY_MDIO.pin.pinIndex);
}


static uint8 read_mdio(void)
{
	if (IfxPort_getPinState(PHY_MDIO.pin.port, PHY_MDIO.pin.pinIndex) == TRUE)
		return 1;
	else
		return 0;
}


void init_MDIO(void)
{
	set_mdc_output();

	//digitalWrite(MDC,0);
	set_mdc(0);
	//TODO: should we change it to high ?
	set_mdio(1);
	set_mdio_input();
	MDIO_GPIO_ALWAYS_PRINTF(("enable GPIO simulating MDIO\r\n"));
}

void deinit_MDIO(void)
{
	IfxPort_setPinModeOutput(PHY_MDIO.pin.port, PHY_MDIO.pin.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);
	IfxPort_setPinModeOutput(PHY_MDC.pin.port, PHY_MDC.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_alt6);
	MDIO_GPIO_ALWAYS_PRINTF(("disable GPIO simulating MDIO\r\n"));
}

/* Generate single clock pulse */

static void toggle_MDC (void)
{
	//MDC_SET_PIN(MDC, LOW);
	set_mdc(0);
	waitTime(TimeConst[TIMER_INDEX_100NS]);	// this is just a placeholder value for delay
	//MDC_SET_PIN(MDC, HIGH);
	set_mdc(1);
	waitTime(TimeConst[TIMER_INDEX_100NS]);	// this is just a placeholder value for delay

}


/* Write preamble of 32 clock pulses */

static void mdcPreamble(void)
{
	uint8 count;
	for (count = 0; count < 32; count++)
		toggle_MDC();
}

/* Write a frame of 16 bits */
void writeFrame(uint16 frame)
{
	uint16 out;
	for (out=0x8000; out != 0; out=out>>1)
	{
		if (out & frame)
			set_mdio(1);
		else
			set_mdio(0);
		toggle_MDC();
	}
}

/* Write a frame of 14 bits and turn around MDIO for 2 clock cycles */
static void writeFrameTA(uint16 frame)
{
	uint16 out;
	for (out=0x8000; out > 2; out=out>>1)  //Shift only 14 bits through MDIO pin
	{
		if (out & frame)
			set_mdio(1);
		else
			set_mdio(0);
		toggle_MDC();
	}
	set_mdio_input();    //Change pin direction of MDIO
	toggle_MDC();
	toggle_MDC();
}

static uint16 readFrame()
{
	int count;
	uint8 mdio_val;
	uint16 data=0;

	//pinMode(MDIO,INPUT);
	set_mdio_input();    //Change pin direction of MDIO

	/* why do we need to toggle one more MDC clock in E3550 ? */
	//toggle_MDC();

	for (count=0; count<16; count++)
	{
		data=data<<1;
		mdio_val = read_mdio();
		//MDIO_READ(MDIO, &mdio_val);
		if (mdio_val)
			data|=1;
		toggle_MDC();
	}
	toggle_MDC();  // end frame

	return data;
}

/* Clause22 register write */

void clause22_write( uint8 phy, uint8 reg, uint16 data)
{
	MDIO_GPIO_ALWAYS_PRINTF(("%s\r\n", __func__));
	uint16 frame;
	frame= c22 <<14 |c22_wrt << 12 | phy << 7 | reg << 2 | turnaround ;
	mdcPreamble();
	set_mdio_output();
	writeFrame(frame);
	//Serial.println(frame,HEX);
	writeFrame(data);
	//Serial.println(data,HEX);
	set_mdio_input();
	toggle_MDC();  // end frame
	toggle_MDC();  // end frame
}

/* Clause22 register read */

uint16 clause22_read( uint8 phy, uint8 reg)
{
	MDIO_GPIO_ALWAYS_PRINTF(("%s\r\n", __func__));
	uint16 frame;
	frame= c22 <<14 |c22_rd << 12 | phy << 7 | reg << 2 | turnaround ;
	mdcPreamble();
	set_mdio_output();
	writeFrameTA(frame);
	//Serial.println(frame,HEX);
	// MDIO_PIN_MODE(MDIO,INPUT);
	frame=readFrame();
	toggle_MDC();
	toggle_MDC(); // end frame
	return frame;
}

void clause45_write (uint8 phy, uint8 dev,uint16 adr, uint16 data)
{
	MDIO_GPIO_ALWAYS_PRINTF(("%s\r\n", __func__));
	uint16 frame;
	frame= c45 <<14 |c45_adr << 12 | phy << 7 | dev << 2 | turnaround ;
	mdcPreamble();
	set_mdio_output();
	writeFrame(frame);
	//Serial.println(frame,HEX);
	writeFrame(adr);
	//Serial.println(adr,HEX);
	set_mdio_input();
	toggle_MDC(); // end frame

	frame= c45 <<14 |c45_wrt << 12 | phy << 7 | dev << 2 | turnaround ;
	mdcPreamble();
	set_mdio_output();
	writeFrame(frame);
	//Serial.println(frame,HEX);
	writeFrame(data);
	//Serial.println(data,HEX);
	set_mdio_input();
	toggle_MDC(); // end frame
}

uint16 clause45_read (uint8 phy, uint8 dev,uint16 adr)
{
	MDIO_GPIO_ALWAYS_PRINTF(("%s\r\n", __func__));
	uint16 frame;
	frame= c45 <<14 |c45_adr << 12 | phy << 7 | dev << 2 | turnaround ;
	mdcPreamble();
	set_mdio_output();

	writeFrame(frame);
	//Serial.println(frame,HEX);

	writeFrame(adr);
	//Serial.println(adr,HEX);

	set_mdio_input();
	toggle_MDC(); // end frame

	frame= c45 <<14 |c45_rd << 12 | phy << 7 | dev << 2 | turnaround ;
	mdcPreamble();
	set_mdio_output();
	writeFrameTA(frame);
	//Serial.println(frame,HEX);
	//pinMode(MDIO,INPUT);
	frame=readFrame();
	toggle_MDC();
	toggle_MDC(); // end frame
	return frame;
}

/* Read register from current address and increase address register*/

uint16 clause45_read_next (uint8 phy, uint8 dev)
{
	uint16 frame;
	frame= c45 <<14 |c45_rdp << 12 | phy << 7 | dev << 2 | turnaround ;
	mdcPreamble();
	set_mdio_output();
	writeFrameTA(frame);
	//pinMode(MDIO,INPUT);
	frame=readFrame();
	toggle_MDC();
	toggle_MDC(); // end frame

	return frame;
}

/* Start clause45 read Burst
Set first address , read register and increase address register */

uint16 clause45_read_burst (uint8 phy, uint8 dev,uint16 adr)
{
	uint16 frame;
	frame= c45 <<14 |c45_adr << 12 | phy << 7 | dev << 2 | turnaround ;
	mdcPreamble();
	set_mdio_output();
	writeFrame(frame);
	writeFrame(adr);
	set_mdio_input();
	toggle_MDC(); // end frame

	frame= c45 <<14 |c45_rdp << 12 | phy << 7 | dev << 2 | turnaround ;
	mdcPreamble();
	set_mdio_output();
	writeFrameTA(frame);
	//pinMode(MDIO,INPUT);
	frame=readFrame();
	toggle_MDC();
	toggle_MDC(); // end frame

	return frame;
}
#endif
