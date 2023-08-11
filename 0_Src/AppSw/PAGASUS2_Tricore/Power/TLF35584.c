 /* COPYRIGHT (C) 2019
 * QUANTA CORPORATION
 * ALL RIGHTS RESERVED

 * THE SOURCE CODE AND ITS RELATED DOCUMENTATION IS PROVIDED "AS IS". QUANTA
 * CORPORATION MAKES NO OTHER WARRANTY OF ANY KIND, WHETHER EXPRESS, IMPLIED OR,
 * STATUTORY AND DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * SATISFACTORY QUALITY, NON INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * THE SOURCE CODE AND DOCUMENTATION MAY INCLUDE ERRORS. QUANTA CORPORATION
 * RESERVES THE RIGHT TO INCORPORATE MODIFICATIONS TO THE SOURCE CODE IN LATER
 * REVISIONS OF IT, AND TO MAKE IMPROVEMENTS OR CHANGES IN THE DOCUMENTATION OR
 * THE PRODUCTS OR TECHNOLOGIES DESCRIBED THEREIN AT ANY TIME.
 *
 * QUANTA CORPORATION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGE OR LIABILITY ARISING FROM YOUR USE OF THE SOURCE CODE OR
 * ANY DOCUMENTATION, INCLUDING BUT NOT LIMITED TO, LOST REVENUES, DATA OR
 * PROFITS, DAMAGES OF ANY SPECIAL, INCIDENTAL OR CONSEQUENTIAL NATURE, PUNITIVE
 * DAMAGES, LOSS OF PROPERTY OR LOSS OF PROFITS ARISING OUT OF OR IN CONNECTION
 * WITH THIS AGREEMENT, OR BEING UNUSABLE, EVEN IF ADVISED OF THE POSSIBILITY OR
 * PROBABILITY OF SUCH DAMAGES AND WHETHER A CLAIM FOR SUCH DAMAGE IS BASED UPON
 * WARRANTY, CONTRACT, TORT, NEGLIGENCE OR OTHERWISE.
 */

#include "TLF35584.h"
#include "Configuration.h"
#include <Qspi/SpiMaster/IfxQspi_SpiMaster.h>
#include "AsclinShellInterface.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define TLF_BUFFER_SIZE 1   /**< \brief Tx/Rx Buffer size */
#define QSPI2_MAX_BAUDRATE          50000000  // maximum baudrate which is possible to get a small time quantum
//#define QSPI2_USE_DMA  // uncomment line for using DMA and define the DMA channels for this QSPI
//#define DMA_CH_QSPI2_TX
//#define DMA_CH_QSPI2_RX
//#define QSPI2_TRANSMIT_CALLBACK
//#define QSPI2_RECEIVE_CALLBACK

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/
IFX_EXTERN App_AsclinShellInterface g_AsclinShellInterface;
/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct
{
    uint32 spiTxBuffer[TLF_BUFFER_SIZE];                               /**< \brief Qspi Transmit buffer */
    uint32 spiRxBuffer[TLF_BUFFER_SIZE];                               /**< \brief Qspi receive buffer */
} AppQspi_TLF_Buffer;

/** \brief QspiCpu global data */
typedef struct
{
    AppQspi_TLF_Buffer qspiBuffer;                       /**< \brief Qspi buffer */
    struct
    {
        IfxQspi_SpiMaster         spiMaster;             /**< \brief Spi Master handle */
        IfxQspi_SpiMaster_Channel spiMasterChannel;      /**< \brief Spi Master Channel handle */
    }drivers;
}  App_Qspi_TLF_Cpu;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
#if ISR_PROVIDER_QSPI2 == 0
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
#elif ISR_PROVIDER_QSPI2 == 1
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
#elif ISR_PROVIDER_QSPI2 == 2
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
#error "Set ISR_PROVIDER_QSPI2 to a valid value!"
#endif

App_Qspi_TLF_Cpu g_Qspi_TLF_Cpu;
volatile uint32 TLF_B_Step; // if this is 0 then we are using an A-step device else B-step device
volatile boolean wwd_enable = FALSE;
volatile boolean eventFlag_10ms_wwd_service = FALSE;

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
#ifdef QSPI2_TRANSMIT_CALLBACK
    extern void QSPI2_TRANSMIT_CALLBACK(void);
#endif
#ifdef QSPI2_RECEIVE_CALLBACK
    extern void QSPI2_RECEIVE_CALLBACK(void);
#endif

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
/* pin configuration */
const IfxQspi_SpiMaster_Pins qspi2_pins = {&AURIX_TLF_SPI_SCK, IfxPort_OutputMode_pushPull,   /* SCLK */
                                           &AURIX_TLF_SPI_MOSI,  IfxPort_OutputMode_pushPull, /* MTSR */
                                           &AURIX_TLF_SPI_MISO,  IfxPort_InputMode_pullDown,  /* MRST */
                                           IfxPort_PadDriver_ttlSpeed1		                  /* pad driver mode */
};

#ifndef TLF_USE_CHIPSELECT
/* our cs is not an SLSO, we disable the automatic, set to channel 4 and to selected port pin */
IfxQspi_Slso_Out TLF_USE_CHIPSELECT = {&TLF_QSPI_MODULE, TLF_QSPI_CHANNEL, {&TLF_CS_PORT_MODULE, TLF_CS_PORT_PIN},  IfxPort_OutputIdx_general};
#define NO_AUTOMATIC_CS
#endif
IFX_INTERRUPT(ISR_qspi2_Tx, 0, ISR_PRIORITY_QSPI2_TX);
IFX_INTERRUPT(ISR_qspi2_Rx, 0, ISR_PRIORITY_QSPI2_RX);
IFX_INTERRUPT(ISR_qspi2_Er, 0, ISR_PRIORITY_QSPI2_ER);

/** \brief Handle qspi2_Tx interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI2
 * \isrPriority \ref ISR_PRIORITY_QSPI2
 *
 */
void ISR_qspi2_Tx(void)
{
    IfxCpu_enableInterrupts();
#ifdef QSPI2_USE_DMA
    IfxQspi_SpiMaster_isrDmaTransmit(&g_Qspi_TLF_Cpu.drivers.spiMaster);
#else
    IfxQspi_SpiMaster_isrTransmit(&g_Qspi_TLF_Cpu.drivers.spiMaster);
#endif

#ifdef QSPI2_TRANSMIT_CALLBACK
    QSPI2_TRANSMIT_CALLBACK();
#endif
}

/** \brief Handle qspi2_Rx interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI2
 * \isrPriority \ref ISR_PRIORITY_QSPI2
 *
 */
void ISR_qspi2_Rx(void)
{
    IfxCpu_enableInterrupts();
#ifdef QSPI2_USE_DMA
    IfxQspi_SpiMaster_isrDmaReceive(&g_Qspi_TLF_Cpu.drivers.spiMaster);
#else
    IfxQspi_SpiMaster_isrReceive(&g_Qspi_TLF_Cpu.drivers.spiMaster);
#endif
#ifdef QSPI2_RECEIVE_CALLBACK
    QSPI2_RECEIVE_CALLBACK();
#endif
}

/** \brief Handle qspi2_Er interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI2
 * \isrPriority \ref ISR_PRIORITY_QSPI2
 *
 */
void ISR_qspi2_Er(void)
{
    IfxCpu_enableInterrupts();
    IfxQspi_SpiMaster_isrError(&g_Qspi_TLF_Cpu.drivers.spiMaster);
}

/** \brief TLF (QSPI) initialization
 *
 * This function initializes Qspix in master mode.
 */
void IfxTLF35584_init(void)
{
    /* disable interrupts */
    boolean interruptState = IfxCpu_disableInterrupts();
    IfxQspi_SpiMaster_Config        spiMasterConfig; 

    /* create module config */
    IfxQspi_SpiMaster_initModuleConfig(&spiMasterConfig, &MODULE_QSPI2);

    /* set the maximum baudrate */
    spiMasterConfig.base.maximumBaudrate = QSPI2_MAX_BAUDRATE;

    /* ISR priorities and interrupt target */
    spiMasterConfig.base.txPriority  = ISR_PRIORITY_QSPI2_TX;
    spiMasterConfig.base.rxPriority  = ISR_PRIORITY_QSPI2_RX;
    spiMasterConfig.base.erPriority  = ISR_PRIORITY_QSPI2_ER;
    spiMasterConfig.base.isrProvider = ISR_PROVIDER_QSPI2;

#ifdef QSPI2_USE_DMA
    // DMA configuration
    spiMasterConfig.dma.txDmaChannelId = DMA_CH_QSPI2_TX;
    spiMasterConfig.dma.rxDmaChannelId = DMA_CH_QSPI2_RX;
    spiMasterConfig.dma.useDma = 1;
#endif
    spiMasterConfig.pins = &qspi2_pins;
        /* initialize module */
        IfxQspi_SpiMaster_initModule(&g_Qspi_TLF_Cpu.drivers.spiMaster, &spiMasterConfig);
    if (qspi2_pins.mrst != NULL_PTR)
        IfxPort_setPinPadDriver(qspi2_pins.mrst->pin.port, qspi2_pins.mrst->pin.pinIndex, qspi2_pins.pinDriver);

    IfxQspi_SpiMaster_ChannelConfig spiMasterChannelConfig;

    {
        /* create channel config */
#if defined(__DCC__)
   		// bug on DCC not all bits in mode are cleared
   		memset(&spiMasterChannelConfig, 0, sizeof(spiMasterChannelConfig));
#endif
        IfxQspi_SpiMaster_initChannelConfig(&spiMasterChannelConfig,
            &g_Qspi_TLF_Cpu.drivers.spiMaster);

        /* set the baudrate for this channel */
        spiMasterChannelConfig.base.baudrate = 5000000;

        /* set the transfer data width */
        spiMasterChannelConfig.base.mode.dataWidth = 16;

        spiMasterChannelConfig.base.mode.csTrailDelay = 2;
        spiMasterChannelConfig.base.mode.csInactiveDelay = 2;
        spiMasterChannelConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnTrailingEdge;


#ifdef NO_AUTOMATIC_CS
        spiMasterChannelConfig.base.mode.autoCS = 0;
#endif

        const IfxQspi_SpiMaster_Output slsOutput = {&TLF_USE_CHIPSELECT,
                                                    IfxPort_OutputMode_pushPull,
                                                    IfxPort_PadDriver_cmosAutomotiveSpeed1};

        spiMasterChannelConfig.sls.output.pin    = slsOutput.pin;
        spiMasterChannelConfig.sls.output.mode   = slsOutput.mode;
        spiMasterChannelConfig.sls.output.driver = slsOutput.driver;

        /* initialize channel */
        IfxQspi_SpiMaster_initChannel(&g_Qspi_TLF_Cpu.drivers.spiMasterChannel,
            &spiMasterChannelConfig);
    }


    /* init tx buffer area */
	g_Qspi_TLF_Cpu.qspiBuffer.spiTxBuffer[0] = 0;
    g_Qspi_TLF_Cpu.qspiBuffer.spiRxBuffer[0] = 0;


    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);


    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "[%s]"ENDL,__func__);
    /* first we check for A or B-step of TLF */
    /* we read the value from address 0x34 but ignore the returned value */
    IfxTLF35584_read_write(0x6801);  /* read address 0x34 */
    /* no we read the SPI status flags */
    TLF_B_Step = (IfxTLF35584_read_write(0x3E01)>>1) & 0xFF;  /* read address 0x1F */
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "TLF_B_STEP value:%d"ENDL,TLF_B_Step);

    if (TLF_B_Step & 0x4)
    {
    	/* there was an address error, this is the B-step */
    	/* we reset the ADDRE flag */
        IfxTLF35584_read_write(0xBE09);  /* SPISF = 0x04 */
    }
    else
        TLF_B_Step = 0;
}

void IfxTLF35584_unprotect_register(void)
{
    IfxTLF35584_read_write(0x8756);  /* PROTCFG = 0xAB */
    IfxTLF35584_read_write(0x87DE);  /* PROTCFG = 0xEF */
    IfxTLF35584_read_write(0x86AD);  /* PROTCFG = 0x56 */
    IfxTLF35584_read_write(0x8625);  /* PROTCFG = 0x12 */
}

void IfxTLF35584_protect_register(void)
{
	IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "[%s]"ENDL,__func__);
    IfxTLF35584_read_write(0x87BE);  /* PROTCFG = 0xDF */
    IfxTLF35584_read_write(0x8668);  /* PROTCFG = 0x34 */
    IfxTLF35584_read_write(0x877D);  /* PROTCFG = 0xBE */
    IfxTLF35584_read_write(0x8795);  /* PROTCFG = 0xCA */
}

void IfxTLF35584_disable_window_watchdog(void)
{
    uint32 uiWdcfg0;
	/* first we get the value of WDCFG0 to check the window watchdog */
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "[%s]"ENDL,__func__);
    if (TLF_B_Step)
        uiWdcfg0 = IfxTLF35584_read_write(0x1A00);  /* read RWDCFG0 (0x0D) */
    else
        uiWdcfg0 = IfxTLF35584_read_write(0x0C00);  /* read WDCFG0 (0x06) */
    /* if the WWDEN is cleared then return because the window watchdog is already disabled */
    if (!(uiWdcfg0 & 0x0010)) return;
	/* clear the WWDEN bit */
    uiWdcfg0 &= ~0x0010;
    /* correct the parity */
    if (uiWdcfg0 & 0x0001)
        uiWdcfg0 &= ~0x0001;
    else
        uiWdcfg0 |= 0x0001;
    /* set the write address to 0x06 */
    uiWdcfg0 |= (0x06<<9);
	/* Write back and disable the Window Watchdog */
    IfxTLF35584_read_write(uiWdcfg0);
}


void IfxTLF35584_enable_window_watchdog(void)
{
    uint32 uiWdcfg0;
	/* first we get the value of WDCFG0 to check the window watchdog */
    if (TLF_B_Step)
        uiWdcfg0 = IfxTLF35584_read_write(0x1A00);  /* read RWDCFG0 (0x0D) */
    else
        uiWdcfg0 = IfxTLF35584_read_write(0x0C00);  /* read WDCFG0 (0x06) */
    /* if the WWDEN is set then return because the window watchdog is already enabled */
    if (uiWdcfg0 & 0x0010) return;
	/* set the WWDEN bit */
    uiWdcfg0 |= 0x0010;
    /* correct the parity */
    if (uiWdcfg0 & 0x0001)
        uiWdcfg0 &= ~0x0001;
    else
        uiWdcfg0 |= 0x0001;
    /* set the write address to 0x06 */
    uiWdcfg0 |= (0x06<<9);
	/* Write back and enable the Window Watchdog */
    IfxTLF35584_read_write(uiWdcfg0);
}


void IfxTLF35584_disable_err_pin_monitor(void)
{
    uint32 uiSyspcfg;
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "[%s]"ENDL,__func__);
    if (TLF_B_Step)
    {
    	/* first we get the value of RSYSPCFG1 to check the error pin monitor */
    	uiSyspcfg = IfxTLF35584_read_write(0x1800);  /* read RSYSPCFG1 (0x0C) */
        /* if the ERREN is cleared then return because the error pin monitor is already disabled */
        if (!(uiSyspcfg & 0x0010)) return;
    	/* clear the ERREN bit */
        uiSyspcfg &= ~0x0010;
        /* set the write address to 0x05 */
        uiSyspcfg |= (0x05<<9);
        /* correct the parity */
        if (uiSyspcfg & 0x0001)
        	uiSyspcfg &= ~0x0001;
        else
        	uiSyspcfg |= 0x0001;
    }
    else
    {
    	/* first we get the value of SYSPCFG0 to check the error pin monitor */
    	uiSyspcfg = IfxTLF35584_read_write(0x0801);  /* read SYSPCFG0 (0x04) */
        /* if the ERREN is cleared then return because the error pin monitor is already disabled */
        if (!(uiSyspcfg & 0x0004)) return;
    	/* clear the ERREN bit */
        uiSyspcfg &= ~0x0004;
        /* set the write address to 0x04 */
        uiSyspcfg |= (0x04<<9);
        /* parity must not be corrected */
    }
	/* Write back and disable the error pin monitor */
    IfxTLF35584_read_write(uiSyspcfg);
}



void IfxTLF35584_goto_standby_state(void)
{
	Ifx_SCU * scu_module;
	unsigned int read_val, new_value;
#if 0
    //now put Aurix into standby to avoid reset signal toggling several times
    uint16 password = IfxScuWdt_getSafetyWatchdogPassword();
    scu_module = &MODULE_SCU;
    IfxScuWdt_clearSafetyEndinit(password);
    scu_module->PMSWCR1.B.IRADIS = 0x1;

    scu_module->EVRUVMON.B.SWDUVVAL = 0x75;
    scu_module->EVRMONCTRL.U = 0x00212121;
    read_val = scu_module->PMSWCR1.U;
    new_value = read_val | 0x28000000;
    scu_module->PMSWCR1.U = new_value;
    //scu_module->PMSWCR1.B.CPUIDLSEL = 0x1; //CPU 0 control as master
    //scu_module->PMCSR[0].B.REQSLP = 0x3; //put Aurix to standby
    IfxScuWdt_setSafetyEndinit(password);
#endif
    IfxTLF35584_unprotect_register();
    IfxTLF35584_simplified_read_write(TRUE, 0x4, 0x00);
    IfxTLF35584_protect_register();
    if (TLF_B_Step)
    {
    	//IfxTLF35584_read_write(0x801C);  /* DEVCFG = 0x0e */
    	IfxTLF35584_simplified_read_write(TRUE, TLF_B_REG_DEVCTRL, 0xEC);
    	IfxTLF35584_simplified_read_write(TRUE, TLF_B_REG_DEVCTRLN, ~(0xEC));
    }
    else

        IfxTLF35584_read_write(0x9DD9);  /* DEVCTRL = 0xEC */
#if 0
    //now put Aurix into standby to avoid reset signal toggling several times
    uint16 password = IfxScuWdt_getSafetyWatchdogPassword();
    scu_module = &MODULE_SCU;
    IfxScuWdt_clearSafetyEndinit(password);
    scu_module->PMSWCR1.B.IRADIS = 0x1;

    scu_module->PMSWCR1.B.CPUSEL = 0x1; //CPU 0 control as master
    scu_module->PMCSR[0].B.REQSLP = 0x3; //put Aurix to standby
    IfxScuWdt_setSafetyEndinit(password);
#endif
}


void IfxTLF35584_goto_normal_state(void)
{
    /* Switch TLF to normal state */
    if (TLF_B_Step)
    {
    	IfxTLF35584_simplified_read_write(TRUE, TLF_B_REG_DEVCTRL, 0xEA);
    	IfxTLF35584_simplified_read_write(TRUE, TLF_B_REG_DEVCTRLN, ~(0xEA));
    }
    else
        IfxTLF35584_read_write(0x9DD5);  /* DEVCTRL = 0xEA */
}

uint32 IfxTLF35584_read_write(uint32 send_data)
{
    g_Qspi_TLF_Cpu.qspiBuffer.spiTxBuffer[0] = send_data;

    while (IfxQspi_SpiMaster_getStatus(&g_Qspi_TLF_Cpu.drivers.spiMasterChannel) == SpiIf_Status_busy)  {};

    IfxQspi_SpiMaster_exchange(&g_Qspi_TLF_Cpu.drivers.spiMasterChannel, &g_Qspi_TLF_Cpu.qspiBuffer.spiTxBuffer[0],
        &g_Qspi_TLF_Cpu.qspiBuffer.spiRxBuffer[0], TLF_BUFFER_SIZE);

    /* we wait until our values are read from Qspi */
    while (IfxQspi_SpiMaster_getStatus(&g_Qspi_TLF_Cpu.drivers.spiMasterChannel) == SpiIf_Status_busy)  {};

    return (g_Qspi_TLF_Cpu.qspiBuffer.spiRxBuffer[0]);

}

uint8 IfxTLF35584_simplified_read_write(boolean write_cmd, uint8 address, uint8 send_data)
{
    uint32 data_frame;
    uint8 parity, uiCnt;
    data_frame = 0xffff & ((((uint8)write_cmd) << 15) | (address << 9) | (send_data << 1));

    parity = 0;
    // we count our ones to calculate the parity
    for (uiCnt = 0; uiCnt<16; uiCnt++)
    {
    	if (data_frame & (0x1<<uiCnt))
    	{
    		if (parity) parity = 0;
    		else parity = 1;
    	}
    }
    data_frame |= parity;
    //return data_frame;

	g_Qspi_TLF_Cpu.qspiBuffer.spiTxBuffer[0] = data_frame;

    while (IfxQspi_SpiMaster_getStatus(&g_Qspi_TLF_Cpu.drivers.spiMasterChannel) == SpiIf_Status_busy)  {};

    IfxQspi_SpiMaster_exchange(&g_Qspi_TLF_Cpu.drivers.spiMasterChannel, &g_Qspi_TLF_Cpu.qspiBuffer.spiTxBuffer[0],
        &g_Qspi_TLF_Cpu.qspiBuffer.spiRxBuffer[0], TLF_BUFFER_SIZE);

    /* we wait until our values are read from Qspi */
    while (IfxQspi_SpiMaster_getStatus(&g_Qspi_TLF_Cpu.drivers.spiMasterChannel) == SpiIf_Status_busy)  {};

    return (uint8)((g_Qspi_TLF_Cpu.qspiBuffer.spiRxBuffer[0]>>1) & 0xff);

}
