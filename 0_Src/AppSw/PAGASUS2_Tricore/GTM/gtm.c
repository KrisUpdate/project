/*
 * gtm.c
 *
 *  Created on: 2017Äê9ÔÂ5ÈÕ
 *      Author: bradf
 */

#include <Configuration.h>
#include "gtm.h"
#include <internal_fw_debug.h>
/* TIM */
#include "Gtm/Std/IfxGtm.h"
#include "Gtm/Tim/In/IfxGtm_Tim_In.h"
#include "Cpu/Std/IfxCpu.h"
#include "Cpu/Irq/IfxCpu_Irq.h"
#include "fan.h"

/* GTM is tested in 39x only, in 29x we don't need that */
#if MCU_PLATFORM == _AURIX_TC39X_

#define GTM_DEBUG_ENABLE 1

#if GTM_DEBUG_ENABLE == 1
#define GTM_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define GTM_DEBUG_PRINTF(x)
#endif

#define GTM_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)

static IfxGtm_Tim_In TimForFan_Handler[Max_Tach_Pin_Number];
typedef struct
{
	uint8 timIndex;
	uint8 channelIndex;
	uint8 interrupt_priority;
	IfxGtm_Tim_TinMap  *inputPin;

}t_TimConfig;

static const t_TimConfig timConfig[Max_Tach_Pin_Number]=
{
		{
				0,
				5,
				ISR_PRIORITY_GtmTIM_1,
				&IfxGtm_TIM0_5_TIN40_P32_4_IN
		},
		{
				3,
				5,
				ISR_PRIORITY_GtmTIM_2,
				&IfxGtm_TIM3_5_TIN140_P32_5_IN
		},
		{
				0,
				7,
				ISR_PRIORITY_GtmTIM_3,
				&IfxGtm_TIM0_7_TIN118_P02_11_IN
		},

};

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
static void TimMeasurementInit(uint8 channelIndex); /* Tim configuration for measurement */

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/* Declare the ISR function */
IFX_INTERRUPT(GtmTim_ISR_1, 0, ISR_PRIORITY_GtmTIM_1);
IFX_INTERRUPT(GtmTim_ISR_2, 0, ISR_PRIORITY_GtmTIM_2);
IFX_INTERRUPT(GtmTim_ISR_3, 0, ISR_PRIORITY_GtmTIM_3);
void GtmTim_ISR_1(void)
{
	IfxCpu_enableInterrupts();
	IfxGtm_Tim_In_onIsr(&TimForFan_Handler[0]);
}
void GtmTim_ISR_2(void)
{
	IfxCpu_enableInterrupts();
	IfxGtm_Tim_In_onIsr(&TimForFan_Handler[1]);
}
void GtmTim_ISR_3(void)
{
	IfxCpu_enableInterrupts();
	IfxGtm_Tim_In_onIsr(&TimForFan_Handler[2]);
}

void GtmTim_init(void)
{

	/* enable GTM before anything else */
	 IfxGtm_enable(&MODULE_GTM);

	 uint8 i;
	 for(i=0; i<Max_Tach_Pin_Number; i++)
	 {
		/* Initialize the TIM  for measurement */
		TimMeasurementInit(i);
	 }
}

/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
boolean GtmTim_getFreq(uint8 channelIndex, uint32* freq_p)
{
    IfxGtm_Tim_In *handler = &TimForFan_Handler[channelIndex]; /* point to the handler of TIM channel */
    float frequency_hz = 0;
    /* Read into the information variable */
    static boolean isOverFlowErrorPrinted = FALSE;
    static boolean isDataLostErrorPrinted = FALSE;
    if(handler->overflowCnt == TRUE)
    {
    	if(!isOverFlowErrorPrinted)
    	{
    		GTM_ALWAYS_PRINTF(("FAN RPM reading error: voerflow flag is set\r\n"));
    		isOverFlowErrorPrinted = TRUE;
    	}
    	handler->overflowCnt = FALSE;
    }
    if(handler->dataLost == TRUE)
    {
    	if(!isDataLostErrorPrinted)
    	{
    		GTM_ALWAYS_PRINTF(("FAN RPM reading error: dataLost flag is set\r\n"));
    		isDataLostErrorPrinted = TRUE;
    	}
       	handler->dataLost = FALSE;
     }

    if (handler->newData == TRUE) /* If New data is present */
    {
    	handler->newData = FALSE;
    	if (handler->dataCoherent == TRUE) /* If data is coherent */
    	{
    		handler->dataCoherent = FALSE;
    	}
		/* Calculate Frequency in Hz */
		frequency_hz = handler->captureClockFrequency / handler->periodTick;
		(*freq_p) = (uint32)frequency_hz;
		return TRUE;
    }

    return FALSE;
}



/* TIM measurement initialization */
static void TimMeasurementInit(uint8 channelIndex)
{
	IfxGtm_Tim_In_Config Tim_In_Config;
	/* enable interrupts */
	IfxCpu_enableInterrupts();

	/* Initialize the Config Structure */
    IfxGtm_Tim_In_Config *config = &Tim_In_Config;
    IfxGtm_Tim_In_initConfig(config, &MODULE_GTM);

    /* Configure the Channel used */
    config->gtm                          = &MODULE_GTM;
    config->timIndex                     = timConfig[channelIndex].timIndex;
    config->channelIndex                 = timConfig[channelIndex].channelIndex;


    /* Configure Interrupt parameters */
    config->irqMode                      = IfxGtm_IrqMode_pulseNotify;
    config->isrProvider                  = IfxSrc_Tos_cpu0;
    config->isrPriority                  = timConfig[channelIndex].interrupt_priority;

    /* Enable the CMU clock for TIM */
    MODULE_GTM.CMU.CLK_EN.B.EN_CLK0 = 0x2;     /* enable the CMU clock 0 now */

    /* Configure the Capture Configurations */
    config->capture.irqOnNewVal          = TRUE; /* give me an interrupt on NEWVAL */
    config->capture.irqOnCntOverflow     = FALSE;
    config->capture.irqOnEcntOverflow    = FALSE;
    config->capture.irqOnDatalost        = FALSE;
    config->capture.clock                = IfxGtm_Cmu_Clk_0;
    config->capture.mode                 = Ifx_Pwm_Mode_leftAligned;

    /* Configure Timeout */
    config->timeout.irqOnTimeout         = FALSE;
    config->timeout.clock                = IfxGtm_Cmu_Clk_0;
    config->timeout.timeout              = 0.0;

    /* Configure the Input */
    config->filter.input                 = IfxGtm_Tim_In_Input_currentChannel;
    config->filter.inputPin              = timConfig[channelIndex].inputPin;
    config->filter.inputPinMode          = IfxPort_InputMode_noPullDevice;

    /* Configure Filter parameters */
    config->filter.risingEdgeMode        = IfxGtm_Tim_In_ConfigFilterMode_none;
    config->filter.fallingEdgeMode       = IfxGtm_Tim_In_ConfigFilterMode_none;
    config->filter.risingEdgeFilterTime  = 0;
    config->filter.fallingEdgeFilterTime = 0;
    config->filter.clock                 = IfxGtm_Cmu_Tim_Filter_Clk_0;

    /* Initialize the TIM Channel Now */
    IfxGtm_Tim_In_init(&TimForFan_Handler[channelIndex], config);
}


#endif // endof #if MCU_PLATFORM == _AURIX_TC39X_
