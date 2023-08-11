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

#include "AppAscUart.h"
#include "AsclinShellInterface.h"
#include <internal_fw_debug.h>
#include "Configuration.h"
#include "Board2board.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define APP_UART_DEBUG_ENABLE 1

#if APP_UART_DEBUG_ENABLE == 1
#define APP_UART_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define APP_UART_DEBUG_PRINTF(x)
#endif

#define APP_UART_PRINTF_ALWAYS(x) DEBUG_PRINTF_ALWAYS(x)

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
boolean UartCmd_Init(pchar args, void *data, IfxStdIf_DPipe *io);
boolean UartCmd_Send(pchar args, void *data, IfxStdIf_DPipe *io);

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
#define B2B_UART_BUF_SIZE  1024
#define UART_BUF_SIZE_WITH_HEADER (B2B_UART_BUF_SIZE + sizeof(Ifx_Fifo) + 8) // See \note for Ifx_Fifo_init().

//static uint8 uart_rx[B2B_UART_BUF_SIZE];
//static uint8 uart_tx[B2B_UART_BUF_SIZE];
static uint8 uart_rx[UART_BUF_SIZE_WITH_HEADER];
static uint8 uart_tx[UART_BUF_SIZE_WITH_HEADER];

#define B2B_UART_RX_BUF 32
#define B2B_UART_TX_BUF 32

static uint8 uart_rx_buffer[B2B_UART_RX_BUF];
static uint8 uart_tx_buffer[B2B_UART_TX_BUF];

IFX_INTERRUPT(ISR_UART_rx, 0, ISR_PRIORITY_B2B_UART_RX);
IFX_INTERRUPT(ISR_UART_tx, 0, ISR_PRIORITY_B2B_UART_TX);
IFX_INTERRUPT(ISR_UART_err, 0, ISR_PRIORITY_B2B_UART_ERR);

volatile boolean isLoopback = FALSE;
volatile boolean isDebugLogEnable = FALSE;

IfxAsclin_Asc b2b_uart_asc;
boolean is_b2b_test_status = FALSE;
extern boolean is_b2b_x86_v3nc;
extern boolean is_b2b_uart_debug;
//extern boolean is_b2b_uart_send;
#if 0
void ISR_UART_rx(void)
{
    static uint8 cnt = 0;
    uint8 chksum;
    IfxCpu_enableInterrupts();
    IfxAsclin_Asc_isrReceive(&b2b_uart_asc);
    uint8 data = IfxAsclin_Asc_blockingRead(&b2b_uart_asc);
    DEBUG_PRINTF_ALWAYS(("Uart: Receive 0x%x\n\r", data));
    if(is_b2b_x86_v3nc){
        //Header
        if(data == UART_PACKET_HEADER1_V3NC&&cnt!=5){
            cnt = 0;
            uart_rx_buffer[cnt] = data;
            cnt++;
            DEBUG_PRINTF_ALWAYS(("Uart: Header \n\r"));
        //End
        }else if(data == UART_PACKET_HEADER2_V3NC){
            uart_rx_buffer[cnt] = data;
            DEBUG_PRINTF_ALWAYS(("Uart: End %d\n\r", cnt));
            if(uart_rx_buffer[0]==UART_PACKET_HEADER1_V3NC&&uart_rx_buffer[7]==UART_PACKET_HEADER2_V3NC){
                APP_UART_DEBUG_PRINTF(("Uart: Receive Correct Packet \n\r"));
                cnt = 0;
                chksum=uart_rx_buffer[1]+uart_rx_buffer[2]+uart_rx_buffer[3]+uart_rx_buffer[4]+uart_rx_buffer[5];
                if(chksum==uart_rx_buffer[6]){
                    DEBUG_PRINTF_ALWAYS(("Uart: V3NC Receive Check Sum Correct \n\r"));
                }else{
                    DEBUG_PRINTF_ALWAYS(("Uart: V3NC Receive Check Sum Fail \n\r"));
                    memset(uart_rx_buffer,0,sizeof(uart_rx_buffer));
                }
            }
        }else{
            uart_rx_buffer[cnt] = data;
            cnt++;
            if(cnt>B2B_UART_VERIFY_LENGTH_V3NC) cnt = 0;
        }
    }else{
        //Header 0x23 is #
        if(data == 0x23){
            cnt = 0;
            uart_rx_buffer[cnt] = data;
            cnt++;
            DEBUG_PRINTF_ALWAYS(("Uart: Header \n\r"));
        //End 0x40 is @
        }else if(data == 0x40){
            uart_rx_buffer[cnt] = data;
            DEBUG_PRINTF_ALWAYS(("Uart: End %d\n\r", cnt));
            if(uart_rx_buffer[0]==0x23&&uart_rx_buffer[4]==0x40){
                DEBUG_PRINTF_ALWAYS(("Uart: Receive Correct Packet \n\r"));
                cnt = 0;
            }
        }else{
            uart_rx_buffer[cnt] = data;
            cnt++;
            if(cnt>B2B_UART_VERIFY_LENGTH) cnt = 0;
        }
    }
}
#else
void ISR_UART_rx(void)
{
    static uint8 cnt = 0;
    uint8 chksum=0;
    uint8 locate_n;
    IfxCpu_enableInterrupts();
    IfxAsclin_Asc_isrReceive(&b2b_uart_asc);
    uint8 data = IfxAsclin_Asc_blockingRead(&b2b_uart_asc);

    if(is_b2b_uart_debug)
        DEBUG_PRINTF_ALWAYS(("Uart: Receive 0x%x\n\r", data));

    if(is_b2b_x86_v3nc){
        //Header
        if(data == UART_PACKET_HEADER1_V3NC&&cnt==0){
            cnt = 0;
            uart_rx_buffer[cnt] = data;
            cnt++;
            //DEBUG_PRINTF_ALWAYS(("Uart: Header \n\r"));
        //End
        }else if(data == UART_PACKET_HEADER2_V3NC&&cnt>=7){
            uart_rx_buffer[cnt] = data;
            //DEBUG_PRINTF_ALWAYS(("Uart: End %d\n\r", cnt));
            locate_n=uart_rx_buffer[2]+6;
            if(uart_rx_buffer[0]==UART_PACKET_HEADER1_V3NC&&uart_rx_buffer[locate_n]==UART_PACKET_HEADER2_V3NC){
                //APP_UART_DEBUG_PRINTF(("Uart: Receive Correct Packet \n\r"));
                //is_b2b_uart_send = FALSE;
                cnt = 0;
                locate_n=uart_rx_buffer[2]+4;
                for(int i=1; i<=locate_n;i++)
                    chksum+=uart_rx_buffer[i];
                locate_n=locate_n+1;
                if(chksum==uart_rx_buffer[locate_n]){
                    //DEBUG_PRINTF_ALWAYS(("Uart: V3NC Receive Check Sum Correct \n\r"));
                }else{
                    DEBUG_PRINTF_ALWAYS(("Uart: V3NC Receive Check Sum Fail,%d \n\r",chksum));
                    memset(uart_rx_buffer,0,sizeof(uart_rx_buffer));
                }
            }
        }else{
            uart_rx_buffer[cnt] = data;
            cnt++;
            if(cnt>B2B_UART_VERIFY_LENGTH_V3NC) cnt = 0;
        }
    }else{
        //Header 0x23 is #
        if(data == 0x23){
            cnt = 0;
            uart_rx_buffer[cnt] = data;
            cnt++;
            DEBUG_PRINTF_ALWAYS(("Uart: Header \n\r"));
        //End 0x40 is @
        }else if(data == 0x40){
            uart_rx_buffer[cnt] = data;
            DEBUG_PRINTF_ALWAYS(("Uart: End %d\n\r", cnt));
            if(uart_rx_buffer[0]==0x23&&uart_rx_buffer[4]==0x40){
                DEBUG_PRINTF_ALWAYS(("Uart: Receive Correct Packet \n\r"));
                cnt = 0;
            }
        }else{
            uart_rx_buffer[cnt] = data;
            cnt++;
            if(cnt>B2B_UART_VERIFY_LENGTH) cnt = 0;
        }
    }
}
#endif
uint8 ReadB2BTest(uint8 v)
{
    return uart_rx_buffer[v];
}

void ClearB2BTest(void)
{
    memset(uart_rx_buffer,0,sizeof(uart_rx_buffer));
    //APP_UART_PRINTF_ALWAYS(("Uart: Clear Rev \r\n"));
}

void ISR_UART_tx(void)
{
    IfxCpu_enableInterrupts();
    IfxAsclin_Asc_isrTransmit(&b2b_uart_asc);
}

void ISR_UART_err(void)
{
    IfxCpu_enableInterrupts();
    IfxAsclin_Asc_isrError(&b2b_uart_asc);
}

void B2BUartService(void){
    if(is_b2b_test_status){
        //#xxx@
        if(uart_rx_buffer[0]==0x23&&uart_rx_buffer[4]==0x40){
            DEBUG_PRINTF_ALWAYS(("Uart Rev from B2B x86: Test Mode\r\n"));
            UartApp_BlockSend(uart_rx_buffer[0]);
            UartApp_BlockSend(uart_rx_buffer[1]);
            UartApp_BlockSend(uart_rx_buffer[2]);
            UartApp_BlockSend(uart_rx_buffer[3]);
            UartApp_BlockSend(uart_rx_buffer[4]);
            waitTime(5*TimeConst[TIMER_INDEX_1MS]);
            ClearB2BTest();
            UartApp_Init(UART_BAUD_RATE_115200);
        }
    }else{
        if(is_b2b_x86_v3nc){
            b2b_decode_uart_v3nc(&uart_rx_buffer);
            b2b_v3nc_polling_cmd();
        }else{
            //#xxx@
            if(uart_rx_buffer[0]==0x23&&uart_rx_buffer[4]==0x40){
                uart_tx_buffer[0]=0x23;    uart_tx_buffer[4]=0x40;
                switch (uart_rx_buffer[1]){
                    case 0x78:
                        if(uart_rx_buffer[2]==0x30&&uart_rx_buffer[3]==0x31){
                            DEBUG_PRINTF_ALWAYS(("Uart Rev from B2B x01 command\r\n"));
                            uart_tx_buffer[1]=0x61;    uart_tx_buffer[2]=0x30;    uart_tx_buffer[3]=0x30;
                            UartApp_BlockSend(uart_tx_buffer[0]);
                            UartApp_BlockSend(uart_tx_buffer[1]);
                            UartApp_BlockSend(uart_tx_buffer[2]);
                            UartApp_BlockSend(uart_tx_buffer[3]);
                            UartApp_BlockSend(uart_tx_buffer[4]);
                        }else{
                            DEBUG_PRINTF_ALWAYS(("Uart Rev from B2B x86: not support this commad\r\n"));
                        }
                        break;
                    default:
                        DEBUG_PRINTF_ALWAYS(("Uart Rev from B2B x86: not support this commad\r\n"));
                        break;
                }
                waitTime(5*TimeConst[TIMER_INDEX_1MS]);
                ClearB2BTest();
                UartApp_Init(UART_BAUD_RATE_115200);
            }
        }
    }
}

void UartApp_Init(uint32 baudrate)
{
    /* disable interrupts */
    boolean interruptState = IfxCpu_disableInterrupts();

    IfxAsclin_Asc_Config config;
    IfxAsclin_Asc_initModuleConfig(&config, P3570_AURIX_B2B_UART_TX.module);
    config.baudrate.baudrate             = baudrate;
    config.baudrate.oversampling         = IfxAsclin_OversamplingFactor_16;
    config.bitTiming.medianFilter        = IfxAsclin_SamplesPerBit_three;
    config.bitTiming.samplePointPosition = IfxAsclin_SamplePointPosition_8;
    config.interrupt.rxPriority          = ISR_PRIORITY_B2B_UART_RX;
    config.interrupt.txPriority          = ISR_PRIORITY_B2B_UART_TX;
    config.interrupt.erPriority          = ISR_PRIORITY_B2B_UART_ERR;
    config.interrupt.typeOfService       = IfxSrc_Tos_cpu0;
    IfxAsclin_Asc_Pins ascPins = {
        .cts       = NULL_PTR,
        .ctsMode   = IfxPort_InputMode_pullUp,
        .rx        = &P3570_AURIX_B2B_UART_RX,
        .rxMode    = IfxPort_InputMode_pullUp,
        .rts       = NULL_PTR,
        .rtsMode   = IfxPort_OutputMode_pushPull,
        .tx        = &P3570_AURIX_B2B_UART_TX,
        .txMode    = IfxPort_OutputMode_pushPull,
        .pinDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    config.pins         = &ascPins;
    config.rxBuffer     = uart_rx;
    config.txBuffer     = uart_tx;
    config.txBufferSize = B2B_UART_BUF_SIZE;
    config.rxBufferSize = B2B_UART_BUF_SIZE;
    IfxAsclin_Asc_initModule(&b2b_uart_asc, &config);

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);
    //DEBUG_PRINTF_ALWAYS(("Initial the B2B UART: %d\n\r",baudrate));
}

void UartApp_BlockSend(uint8 byte)
{
    IfxAsclin_Asc_blockingWrite(&b2b_uart_asc, byte);
    if(is_b2b_uart_debug)
        APP_UART_PRINTF_ALWAYS(("Sent byte 0x%x through UART\n\r", byte));
}
