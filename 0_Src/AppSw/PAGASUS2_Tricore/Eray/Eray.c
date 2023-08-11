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
#include "Eray.h"
#include <internal_fw_debug.h>
#include <gpioexp.h>
#include "resource.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
#if ISR_PROVIDER_ERAY == 0
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
#elif ISR_PROVIDER_ERAY == 1
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
#elif ISR_PROVIDER_ERAY == 2
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
#error "Set ISR_PROVIDER_ERAY to a valid value!"
#endif

App_ErayTestMode g_ErayTestMode; /**< \brief Eray global data */

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

#define DEFAULT_FRAME_ID   32

#define ERAY_DEBUG_ENABLE 1

#if ERAY_DEBUG_ENABLE == 1
#define ERAY_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define ERAY_DEBUG_PRINTF(x)
#endif

#define ERAY_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)


/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
/* pin configuration */
/* channel A pins*/
const IfxEray_Eray_NodeA nodeAPins = {
    IfxPort_InputMode_pullDown,             IfxPort_OutputMode_pushPull,
    IfxPort_PadDriver_cmosAutomotiveSpeed1, IfxPort_OutputMode_pushPull,
    &FR_A_RXD,               &FR_A_TXD,
    &FR_A_TXEN
};
/* channel B pins*/
const IfxEray_Eray_NodeB nodeBPins = {
    IfxPort_InputMode_pullDown,             IfxPort_OutputMode_pushPull,
    IfxPort_PadDriver_cmosAutomotiveSpeed1, IfxPort_OutputMode_pushPull,
    &FR_B_RXD,               &FR_B_TXD,
    &FR_B_TXEN
};

const IfxEray_Eray_Pins  pinsEray = {(IfxEray_Eray_NodeA *)&nodeAPins, (IfxEray_Eray_NodeB *)&nodeBPins};

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

#if 0
IFX_INTERRUPT(ERAY_irqInt0Handler, 0, ISR_PRIORITY_ERAY_INT0);
IFX_INTERRUPT(ERAY_irqInt1Handler, 0, ISR_PRIORITY_ERAY_INT1);
IFX_INTERRUPT(ERAY_irqNdat1Handler, 0, ISR_PRIORITY_ERAY_NDAT1);

/** \brief Handle Eray INT 0 service line interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_ERAY_INT0
 * \isrPriority \ref ISR_PRIORITY_ERAY_INT0
 *
 */
void ERAY_irqInt0Handler(void)
{
	/*Get the error interrupt flags*/
    Ifx_ERAY_EIR ErrIntStat = IfxEray_Eray_getErrorInterrupts(&g_ErayTestMode.eray);

    /* Get the status interrupt flags*/
    Ifx_ERAY_SIR StatusIntStat = IfxEray_Eray_getStatusInterrupts(&g_ErayTestMode.eray);

    if (StatusIntStat.B.SDS)
    {
        /* SDS must be cleared for getting the next interrupt   */
        IfxEray_Eray_clearStatusFlag(&g_ErayTestMode.eray, IfxEray_ClearStatusFlag_sds);
    }

    if (StatusIntStat.B.CYCS)
    {
        /* CYCS must be cleared for getting the next interrupt   */
        IfxEray_Eray_clearStatusFlag(&g_ErayTestMode.eray, IfxEray_ClearStatusFlag_cycs);
    }

    if (StatusIntStat.B.SUCS)
    {
        /* SUCS must be cleared for getting the next interrupt   */
        IfxEray_Eray_clearStatusFlag(&g_ErayTestMode.eray, IfxEray_ClearStatusFlag_sucs);
    }

    if (StatusIntStat.B.TIBC)
    {
        /* TIBC must be cleared for getting the next interrupt   */
        IfxEray_Eray_clearStatusFlag(&g_ErayTestMode.eray, IfxEray_ClearStatusFlag_tibc);
    }

    if (StatusIntStat.B.TXI)
    {
        /* TXI must be cleared for getting the next interrupt   */
        IfxEray_Eray_clearStatusFlag(&g_ErayTestMode.eray, IfxEray_ClearStatusFlag_txi);
    }

    if (ErrIntStat.B.MHF)
    {
        g_ErayTestMode.errors++;
        /* clear flag MHF */
        IfxEray_Eray_clearErrorFlag(&g_ErayTestMode.eray, IfxEray_ClearErrorFlag_mhf);
    }
}


/** \brief Handle Eray INT 1 service line interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_ERAY_INT1
 * \isrPriority \ref ISR_PRIORITY_ERAY_INT1
 *
 */
void ERAY_irqInt1Handler(void)
{
    /*Get the status interrupt flags*/
    Ifx_ERAY_SIR StatusIntStat = IfxEray_Eray_getStatusInterrupts(&g_ErayTestMode.eray);
    if (StatusIntStat.B.TOBC)
    {
        /* TOBC must be cleared for getting the next interrupt */
        IfxEray_Eray_clearStatusFlag(&g_ErayTestMode.eray, IfxEray_ClearStatusFlag_tobc);
    }
}


/** \brief Handle Eray New Data 1 service line interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_ERAY_NDAT1
 * \isrPriority \ref ISR_PRIORITY_ERAY_NDAT1
 *
 */
void ERAY_irqNdat1Handler(void)
{
    /* message received channel A */
    if (IfxEray_Eray_getNewDataInterruptStatus(&g_ErayTestMode.eray, RECEIVE_BUFFER_A_INDEX) == 1)
    {
        IfxEray_Eray_ReceiveControl config;
        config.headerReceived   = FALSE;
        config.dataReceived     = TRUE;
        config.receiveRequested = TRUE;
        config.swapRequested    = TRUE;
        config.bufferIndex      = RECEIVE_BUFFER_A_INDEX;
        IfxEray_Eray_receiveFrame(&g_ErayTestMode.eray, &config);
        IfxEray_Eray_readData(&g_ErayTestMode.eray, &g_ErayTestMode.receivedData_A[0], 2 * 2);
        g_ErayTestMode.ndat_A = TRUE;
    }
    /* message received channel B */
    if (IfxEray_Eray_getNewDataInterruptStatus(&g_ErayTestMode.eray, RECEIVE_BUFFER_B_INDEX) == 1)
    {
        IfxEray_Eray_ReceiveControl config;
        config.headerReceived   = FALSE;
        config.dataReceived     = TRUE;
        config.receiveRequested = TRUE;
        config.swapRequested    = TRUE;
        config.bufferIndex      = RECEIVE_BUFFER_B_INDEX;
        IfxEray_Eray_receiveFrame(&g_ErayTestMode.eray, &config);
        IfxEray_Eray_readData(&g_ErayTestMode.eray, &g_ErayTestMode.receivedData_B[0], 2 * 2);
        g_ErayTestMode.ndat_B = TRUE;
    }
}
#endif

/** \brief Eray init API
 *
 * This function is called from console to init the Eray
 */
void IfxErayTestMode_init(boolean monitor, boolean loopback, uint32 frame_id, uint8 core)
{
    /* disable interrupts */
    boolean interruptState = IfxCpu_disableInterrupts();

    int     result         = 0;
    int     i;

    g_ErayTestMode.ndat_A    = FALSE;
    g_ErayTestMode.ndat_B    = FALSE;
    g_ErayTestMode.errors    = 0;

    //TODO: DAX: modify the SCU
#if PROJECT != PROJECT_PAGASUS2
    /*initialize Eray PLL*/
    {
        IfxScuCcu_ErayPllConfig ErayPllConfig;
        IfxScuCcu_initErayPllConfig(&ErayPllConfig);
        /* if we will use the loopback, we reduce the speed because the transceiver delay can't be compensated */
        if (loopback == TRUE) ErayPllConfig.pllInitialStep.k2Initial = (ErayPllConfig.pllInitialStep.k2Initial+1)*2-1;
        result |= IfxScuCcu_initErayPll(&ErayPllConfig);
    }
#endif
    /* create default module config*/
    IfxEray_Eray_initModuleConfig(&g_ErayTestMode.erayModuleConfig, &MODULE_ERAY0);

    /*Supplied Module Config*/
    if (loopback == FALSE)
    {
        /* in case of non loop back mode we setting up the interrupts */
    	g_ErayTestMode.erayModuleConfig.interrupt.int0Priority  = ISR_PRIORITY_ERAY_INT0;
        g_ErayTestMode.erayModuleConfig.interrupt.int1Priority  = ISR_PRIORITY_ERAY_INT1;
        g_ErayTestMode.erayModuleConfig.interrupt.ndat1Priority = ISR_PRIORITY_ERAY_NDAT1;

        g_ErayTestMode.erayModuleConfig.interrupt.int0IsrProvider  = core;
		g_ErayTestMode.erayModuleConfig.interrupt.int1IsrProvider  = core;
		g_ErayTestMode.erayModuleConfig.interrupt.tint0IsrProvider = core;
		g_ErayTestMode.erayModuleConfig.interrupt.tint1IsrProvider = core;
		g_ErayTestMode.erayModuleConfig.interrupt.ndat0IsrProvider = core;
		g_ErayTestMode.erayModuleConfig.interrupt.ndat1IsrProvider = core;
		g_ErayTestMode.erayModuleConfig.interrupt.mbsc0IsrProvider = core;
		g_ErayTestMode.erayModuleConfig.interrupt.mbsc1IsrProvider = core;
		g_ErayTestMode.erayModuleConfig.interrupt.ibusyIsrProvider = core;
		g_ErayTestMode.erayModuleConfig.interrupt.obusyIsrProvider = core;
    }
    else
    {
        /* in case of loop back mode we disable the (probably used) interrupts */
        volatile Ifx_SRC_SRCR *src;
        src = IfxEray_getInterruptLine0SrcPtr(g_ErayTestMode.erayModuleConfig.module);
        IfxSrc_disable(src);
        src = IfxEray_getInterruptLine1SrcPtr(g_ErayTestMode.erayModuleConfig.module);
        IfxSrc_disable(src);
        src = IfxEray_getNewDataInterrupt1SrcPtr(g_ErayTestMode.erayModuleConfig.module);
        IfxSrc_disable(src);
    }

    IfxEray_Eray_initModule(&g_ErayTestMode.eray, &g_ErayTestMode.erayModuleConfig);

    /*create ERAY node default configuration*/
    IfxEray_Eray_Node_initConfig(&g_ErayTestMode.nodeConfig);

    /* if we will use the loopback, we reduce the baudrate because the transceiver delay can't be compensated */
	if (loopback == TRUE) g_ErayTestMode.nodeConfig.controllerConfig.prtc1Control.baudrate = IfxEray_Baudrate_2_5;
    /* if we will use the loopback, we change the Strobe position for transmission more stable */
	if (loopback == TRUE) g_ErayTestMode.nodeConfig.controllerConfig.prtc1Control.strobePosition = IfxEray_StrobePosition_6;

    /* Messages*/
    g_ErayTestMode.nodeConfig.messageRAMConfig.numberOfMessageBuffers = 3;

    /*Frame header */
    IfxEray_Header     header[NO_OF_SLOTS] = {
        /* fid  cyc  cha    chb     buffer direction                   ppit   transmission mode          mbi    pl  dp   startup  sync*/
        {DEFAULT_FRAME_ID, 0, TRUE, TRUE, IfxEray_BufferDirection_transmit, TRUE,  IfxEray_TransmissionMode_continuous, TRUE, 4, 0x10, FALSE, FALSE},
        {DEFAULT_FRAME_ID, 0, TRUE, FALSE, IfxEray_BufferDirection_receive,  TRUE, IfxEray_TransmissionMode_continuous, TRUE, 4, 0x12, FALSE, FALSE},
        {DEFAULT_FRAME_ID, 0, FALSE, TRUE, IfxEray_BufferDirection_receive,  TRUE, IfxEray_TransmissionMode_continuous, TRUE, 4, 0x14, FALSE, FALSE},
    };

    /* if frame_id is set, we need to change it here */
    if((frame_id >= FRAME_ID_MIN) && (frame_id <= FRMAE_ID_MAX))
    {
    	uint8 i;
    	for(i=0; i<NO_OF_SLOTS; i++)
    		header[i].frameId = frame_id;
    }

    ERAY_DEBUG_PRINTF(("Erayinit with frame id 0x%x\r\n", header[0].frameId));

    /* slot buffer transmit configuration */
    IfxEray_SlotConfig slot[NO_OF_SLOTS] = {
        /* header data   stxrh bufferNum */
        {TRUE, TRUE,  TRUE,  0},
        {TRUE, FALSE, FALSE, 1},
        {TRUE, FALSE, FALSE, 2},
    };

    for (i = 0; i < NO_OF_SLOTS; i++)
    {
        g_ErayTestMode.nodeConfig.messageRAMConfig.header[i]      = &header[i];
        g_ErayTestMode.nodeConfig.messageRAMConfig.slotControl[i] = &slot[i];
        g_ErayTestMode.nodeConfig.messageRAMConfig.data[i]        = NULL_PTR;
    }

    g_ErayTestMode.nodeConfig.pins = (IfxEray_Eray_Pins *)&pinsEray;

    /*ERAY node initialisation with supplied configuration*/
    IfxEray_Eray_Node_init(&g_ErayTestMode.eray, &g_ErayTestMode.nodeConfig);

    IfxEray_initRxPin(pinsEray.nodeAPins->rxIn, pinsEray.nodeAPins->rxInMode);
    IfxEray_initTxPin(pinsEray.nodeAPins->txOut, pinsEray.nodeAPins->txOutMode, pinsEray.nodeAPins->pinDriver);
    IfxEray_initTxEnPin(pinsEray.nodeAPins->txEnOut, pinsEray.nodeAPins->txEnOutMode, pinsEray.nodeAPins->pinDriver);
    IfxEray_initRxPin(pinsEray.nodeBPins->rxIn, pinsEray.nodeBPins->rxInMode);
    IfxEray_initTxPin(pinsEray.nodeBPins->txOut, pinsEray.nodeBPins->txOutMode, pinsEray.nodeBPins->pinDriver);
    IfxEray_initTxEnPin(pinsEray.nodeBPins->txEnOut, pinsEray.nodeBPins->txEnOutMode, pinsEray.nodeBPins->pinDriver);

    IfxPort_setPinHigh(FR_A_EN.port, FR_A_EN.pinIndex);
#ifdef FR_A_STB
	IfxPort_setPinHigh(FR_A_STB.port, FR_A_STB.pinIndex);
#endif
#ifdef FR_A_STB_N_EXP
	SET_GPIOEXP_OUTPUT_HIGH(FR_A_STB_N_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(FR_A_STB_N_EXP);
#endif
	IfxPort_setPinHigh(FR_B_EN.port, FR_B_EN.pinIndex);
#ifdef FR_B_STB
	IfxPort_setPinHigh(FR_B_STB.port, FR_B_STB.pinIndex);
#endif
#ifdef FR_B_STB_N_EXP
	SET_GPIOEXP_OUTPUT_HIGH(FR_B_STB_N_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(FR_B_STB_N_EXP);
#endif
#ifdef FLEXRAY_ENABLE
	IfxPort_setPinLow(FLEXRAY_ENABLE.port, FLEXRAY_ENABLE.pinIndex);
#endif
#ifdef FLEXRAY_ENABLE_N
	IfxPort_setPinLow(FLEXRAY_ENABLE_N.port, FLEXRAY_ENABLE_N.pinIndex);
#endif

    /* we are here in ready state, switch back to config state */
	IfxEray_changePocState(g_ErayTestMode.eray.eray, IfxEray_PocCommand_config);

	if ((loopback == TRUE) || (monitor == FALSE))
	{
	    /* for atm and loop back we need WRTEN set */
	    g_ErayTestMode.eray.eray->LCK.B.TMK   = 0x75;
	    g_ErayTestMode.eray.eray->LCK.B.TMK   = 0x8A;
	    // set the WRTEN bit
	    g_ErayTestMode.eray.eray->TEST1.U = 0x1;
	    if (loopback == TRUE)
	    	// now switch to external Loopback
		    g_ErayTestMode.eray.eray->TEST1.U = 0x3;
	    // wait CC is busy
	    while (g_ErayTestMode.eray.eray->SUCC1.B.PBSY == 1)
	    {}
	}
    /* Ready unlock sequence */
    g_ErayTestMode.eray.eray->LCK.B.CLK   = 0xCE;
    g_ErayTestMode.eray.eray->LCK.B.CLK   = 0x31;

    /* in case of loopback we switch to loop_back mode */
	if (loopback == TRUE) g_ErayTestMode.eray.eray->SUCC1.U = 0x0C00000F;
	else
	    /* in case of no monitor we switch to asynchronous transmit mode */
		if (monitor == FALSE) g_ErayTestMode.eray.eray->SUCC1.U = 0x0C00000E;
	    /* otherwise we go to monitor mode */
		else IfxEray_changePocState(g_ErayTestMode.eray.eray, IfxEray_PocCommand_monitor);

 	if (monitor == FALSE) g_ErayTestMode.eray.eray->MTCCV.U = (30<<16) | 30;

 	if (loopback == FALSE)
 	{
 	    /*ALL NDAT interrupts are routed to SRC_ERAYNDAT1 interrupt service line*/
 	    IfxEray_Eray_setNewDataInterruptDestination(&g_ErayTestMode.eray, 1, 1); // New Data #1 -> NDAT1
 	    IfxEray_Eray_setNewDataInterruptDestination(&g_ErayTestMode.eray, 2, 1); // New Data #2 -> NDAT1
 	}

	g_ErayTestMode.initialized = TRUE;

    IfxCpu_restoreInterrupts(interruptState);
}

void send_eray_frame(uint32 *pData, uint32 wordlength)
{
	IfxEray_Eray_writeData(&g_ErayTestMode.eray, pData, wordlength*2);
	IfxEray_Eray_TransmitControl config;
	config.headerTransfered  = FALSE;
	config.dataTransfered    = TRUE;
	config.transferRequested = TRUE;
	config.bufferIndex       = 0;
	IfxEray_Eray_transmitFrame(&g_ErayTestMode.eray, &config);
}

uint32 testcnt=0;
#define NUMBER_OF_SENDED_32BIT_VALUES  2

uint32 testeray(uint8 mode)
{
	volatile uint32 timeout;
	uint32 uiCnt;
	uint32 uiSendData[NUMBER_OF_SENDED_32BIT_VALUES];
	uint32 uiRecDataA[NUMBER_OF_SENDED_32BIT_VALUES];
	uint32 uiRecDataB[NUMBER_OF_SENDED_32BIT_VALUES];
	IfxEray_Eray_TransmitControl tr_config;
    IfxEray_Eray_ReceiveControl rec_config;
    uint32 actK2Div;
    uint16 endinitSfty_pw;
    /* We need this for TC397 drive AX to lower the PPL1 when doing the Eray test */
#if PROJECT == PROJECT_PAGASUS2
	/*get the watch-dog passwords for usage with this function*/
	endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPasswordInline();
	actK2Div = SCU_PERPLLCON1.B.K2DIV;
#endif

#if PROJECT == PROJECT_PAGASUS2
    /* Reset Safety ENDINIT, SCU_PERPLLCON register is protected*/
    IfxScuWdt_clearSafetyEndinitInline(endinitSfty_pw);
    SCU_PERPLLCON1.B.K2DIV = 2*(actK2Div+1)-1;
    IfxScuWdt_setSafetyEndinitInline(endinitSfty_pw);
#endif
    for (uiCnt = 0; uiCnt < NUMBER_OF_SENDED_32BIT_VALUES; uiCnt++)
    {
    	if (uiCnt & 1) uiSendData[uiCnt] = 0x44556677+testcnt+uiCnt;
    	else uiSendData[uiCnt] = 0x00112233+testcnt+uiCnt;
    }

	if (mode)
	{
		// sending B to A we disable the TXENA
		IfxPort_setPinHigh(pinsEray.nodeAPins->txEnOut->pin.port, pinsEray.nodeAPins->txEnOut->pin.pinIndex);
		IfxPort_setPinModeOutput(pinsEray.nodeAPins->txEnOut->pin.port, pinsEray.nodeAPins->txEnOut->pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	}
	else
	{
		// sending A to B we disable the TXENB
		IfxPort_setPinHigh(pinsEray.nodeBPins->txEnOut->pin.port, pinsEray.nodeBPins->txEnOut->pin.pinIndex);
		IfxPort_setPinModeOutput(pinsEray.nodeBPins->txEnOut->pin.port, pinsEray.nodeBPins->txEnOut->pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	}

	timeout = 5000000/5;
	while(timeout) timeout--;	// wait for stable pin

	IfxEray_Eray_writeData(&g_ErayTestMode.eray, &uiSendData[0], NUMBER_OF_SENDED_32BIT_VALUES*2);
	tr_config.headerTransfered  = FALSE;
	tr_config.dataTransfered    = TRUE;
	tr_config.transferRequested = TRUE;
	tr_config.bufferIndex       = 0;
	IfxEray_Eray_transmitFrame(&g_ErayTestMode.eray, &tr_config);

	timeout = 5000000;
	while((g_ErayTestMode.eray.eray->NDAT1.U != 0x6) && timeout) timeout--;	// wait for new data
	if (!timeout) return 1;

	rec_config.headerReceived   = FALSE;
	rec_config.dataReceived     = TRUE;
	rec_config.receiveRequested = TRUE;
	rec_config.swapRequested    = TRUE;
	rec_config.bufferIndex      = 1;
    IfxEray_Eray_receiveFrame(&g_ErayTestMode.eray, &rec_config);

    IfxEray_Eray_readData(&g_ErayTestMode.eray, &uiRecDataA[0], NUMBER_OF_SENDED_32BIT_VALUES*2);

    for (uiCnt = 0; uiCnt < NUMBER_OF_SENDED_32BIT_VALUES; uiCnt++)
    {
        if (uiRecDataA[uiCnt] != uiSendData[uiCnt]) return (uiCnt | (1<<30));
    }
	rec_config.bufferIndex      = 2;
    IfxEray_Eray_receiveFrame(&g_ErayTestMode.eray, &rec_config);

    IfxEray_Eray_readData(&g_ErayTestMode.eray, &uiRecDataB[0], NUMBER_OF_SENDED_32BIT_VALUES*2);

    for (uiCnt = 0; uiCnt < NUMBER_OF_SENDED_32BIT_VALUES; uiCnt++)
    {
        if (uiRecDataB[uiCnt] != uiSendData[uiCnt]) return (uiCnt | (1<<30));
    }
	if (mode)
	{
		// setback TXENA to normal use
	    IfxEray_initTxEnPin(pinsEray.nodeAPins->txEnOut, pinsEray.nodeAPins->txEnOutMode, pinsEray.nodeAPins->pinDriver);
	}
	else
	{
		// setback TXENB to normal use
	    IfxEray_initTxEnPin(pinsEray.nodeBPins->txEnOut, pinsEray.nodeBPins->txEnOutMode, pinsEray.nodeBPins->pinDriver);
	}

    testcnt++;
#if PROJECT == PROJECT_PAGASUS2
	/* Reset Safety ENDINIT, SCU_PERPLLCON register is protected*/
	IfxScuWdt_clearSafetyEndinitInline(endinitSfty_pw);
	SCU_PERPLLCON1.B.K2DIV = actK2Div;
	IfxScuWdt_setSafetyEndinitInline(endinitSfty_pw);
#endif
    return 0;
}

static boolean is_eray_stress_log_printed = FALSE;

void run_eraystress_test()
{
	uint32 bytesleft;
	static uint32 datas[2] = {0x55555555, 0x55555555};

	if(!is_eray_stress_log_printed)
	{/* Print the log once only*/
		is_eray_stress_log_printed = TRUE;
		ERAY_ALWAYS_PRINTF(("Stressing Eray at background, run \"aurixreset\" to reboot Aurix if you don't need it\r\n"));
	}

	if(!eraystress_initialized)
	{
		//use default frame id for stress
		IfxErayTestMode_init(FALSE, FALSE, FRMAE_ID_INGORE, IfxSrc_Tos_cpu2);
	}
   	send_eray_frame(&datas[0], 2);
}

