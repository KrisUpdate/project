/**
 * \file ConfigurationIsr.h
 * \brief Interrupts configuration.
 *
 *
 * \version disabled
 * \copyright Copyright (c) 2018 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * \defgroup IfxLld_Demo_GethBasic_SrcDoc_InterruptConfig Interrupt configuration
 * \ingroup IfxLld_Demo_GethBasic_SrcDoc
 */

#ifndef CONFIGURATIONISR_PAGASUS2_H
#define CONFIGURATIONISR_PAGASUS2_H
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \brief Build the ISR configuration object
 * \param no interrupt priority
 * \param cpu assign CPU number
 */
#define ISR_ASSIGN(no, cpu)  ((no << 8) + cpu)

/** \brief extract the priority out of the ISR object */
#define ISR_PRIORITY(no_cpu) (no_cpu >> 8)

/** \brief extract the service provider  out of the ISR object */
#define ISR_PROVIDER(no_cpu) (no_cpu % 8)
/**
 * \addtogroup IfxLld_Demo_AsclinShellInterface_SrcDoc_InterruptConfig
 * \{ */

/**
 * \addtogroup IfxLld_Demo_GethBasic_SrcDoc_InterruptConfig
 * \{ */

/**
 * \name Interrupt priority configuration.
 * The interrupt priority range is [1,255]
 * \{
 */
#define IFX_CFG_STM0_TICKS_PER_MS (100000) /**< \brief Number of STM0 ticks per millisecond */
#define IFX_CFG_STM1_TICKS_PER_MS (100000) /**< \brief Number of STM1 ticks per millisecond */
#define IFX_CFG_STM2_TICKS_PER_MS (100000) /**< \brief Number of STM2 ticks per millisecond */
#define IFX_CFG_STM3_TICKS_PER_MS (100000) /**< \brief Number of STM1 ticks per millisecond */
#define IFX_CFG_STM4_TICKS_PER_MS (100000) /**< \brief Number of STM2 ticks per millisecond */
#define IFX_CFG_STM5_TICKS_PER_MS (100000) /**< \brief Number of STM1 ticks per millisecond */


#define ISR_PRIORITY_ASC_SHELL_RX 4         /**< \brief Define the ASC0 receive interrupt priority.  */
#define ISR_PRIORITY_ASC_SHELL_TX 20         /**< \brief Define the ASC0 transmit interrupt priority.  */

#define ISR_PRIORITY_ASC_SHELL_EX 12        /**< \brief Define the ASC0 error interrupt priority.  */

#if 0
#define ISR_PRIORITY_TEGRA_UART_RX    25  /**< \brief Define the ASC SHELL receive interrupt priority.  */
#define ISR_PRIORITY_TEGRA_UART_TX    26  /**< \brief Define the ASC SHELL transmit interrupt priority.  */
#define ISR_PRIORITY_TEGRA_UART_EX    27  /**< \brief Define the ASC SHELL error interrupt priority.  */
#endif

#define ISR_PRIORITY_CAN_NODE0      14
#define ISR_PRIORITY_CAN_NODE1      15

#define ISR_PRIORITY_CAN_NODE2      16
#define ISR_PRIORITY_CAN_NODE3      17
#define ISR_PRIORITY_CAN_NODE4      18
#define ISR_PRIORITY_CAN_NODE5      19
 


#define ISR_PRIORITY_GtmTIM_1       31
#define ISR_PRIORITY_GtmTIM_2       32
#define ISR_PRIORITY_GtmTIM_3       33

#define ISR_PRIORITY_QSPI_0_ER       21  /**< \brief Define the Qspi0 error interrupt priority.  */
#define ISR_PRIORITY_QSPI_1_ER       22  /**< \brief Define the Qspi0 error interrupt priority.  */
#define ISR_PRIORITY_QSPI2_ER        23  /**< \brief Define the Qspi2 error interrupt priority.  */



#define ISR_PRIORITY_QSPI_4_ER       24
#define ISR_PRIORITY_QSPI_3_ER       28


#define ISR_PRIORITY_QSPI_0_TX      30  /**< \brief Define the Qspi0 transmit interrupt priority.  */
#define ISR_PRIORITY_QSPI_0_RX      36  /**< \brief Define the Qspi0 receive interrupt priority.  */

#define ISR_PRIORITY_QSPI_1_TX       40  /**< \brief Define the Qspi0 transmit interrupt priority.  */
#define ISR_PRIORITY_QSPI_1_RX       41  /**< \brief Define the Qspi0 receive interrupt priority.  */

#define ISR_PRIORITY_QSPI_4_TX	     42
#define ISR_PRIORITY_QSPI_4_RX	     43

#if 0
#define ISR_PRIORITY_DMA_QSPI_4_TX   44
#define ISR_PRIORITY_DMA_QSPI_4_RX   45
#endif

#define ISR_PRIORITY_CAN0_RX        44 /**< \brief Define the CAN PORT0 RX interrupt priority.  */
#define ISR_PRIORITY_CAN1_RX        45 /**< \brief Define the CAN PORT1 RX interrupt priority.  */
#define ISR_PRIORITY_CAN2_RX        46 /**< \brief Define the CAN PORT2 RX interrupt priority.  */
#define ISR_PRIORITY_CAN3_RX        47 /**< \brief Define the CAN PORT3 RX interrupt priority.  */
#define ISR_PRIORITY_CAN4_RX        48 /**< \brief Define the CAN PORT4 RX interrupt priority.  */
#define ISR_PRIORITY_CAN5_RX        49 /**< \brief Define the CAN PORT5 RX interrupt priority.  */

#define ISR_PRIORITY_QSPI2_TX       50  /**< \brief Define the Qspi0 transmit interrupt priority.  */
#define ISR_PRIORITY_QSPI2_RX       51  /**< \brief Define the Qspi0 receive interrupt priority.  */

#define ISR_PRIORITY_QSPI_3_TX	     52
#define ISR_PRIORITY_QSPI_3_RX	     53

#if 0
#define ISR_PRIORITY_DMA_CH1        50 /**< \brief Define the Dma channel1 interrupt priority.  */
#define ISR_PRIORITY_DMA_CH2        51 /**< \brief Define the Dma channel2 interrupt priority.  */
#define ISR_PRIORITY_DMA_CH3        52 /**< \brief Define the Dma channel3 interrupt priority.  */
#define ISR_PRIORITY_DMA_CH4        53 /**< \brief Define the Dma channel4 interrupt priority.  */
#endif

#define ISR_PRIORITY_ERAY_NDAT1     55  /**< \brief Define the Eray New Data 1 interrupt priority.  */
#define ISR_PRIORITY_ERAY_INT1      56  /**< \brief Define the Eray Status and Error 1 interrupt priority.  */
#define ISR_PRIORITY_ERAY_INT0      57  /**< \brief Define the Eray Status and Error 0 interrupt priority.  */

#define ISR_PRIORITY_ETHERNET       60  /**< \brief Define the Ethernet interrupt priority.  */

//TC397B
#define ISR_PRIORITY_GETH_TX        59
#define ISR_PRIORITY_GETH_RX        60


#define IFX_INTPRIO_GETH_DMA_TX0 81
#define IFX_INTPRIO_GETH_DMA_TX1 82
#define IFX_INTPRIO_GETH_DMA_TX2 83
#define IFX_INTPRIO_GETH_DMA_TX3 84
#define IFX_INTPRIO_GETH_DMA_RX0 85
#define IFX_INTPRIO_GETH_DMA_RX1 86
#define IFX_INTPRIO_GETH_DMA_RX2 87
#define IFX_INTPRIO_GETH_DMA_RX3 88



#define ISR_PRIORITY_GTM_TIMER      61 /**< \brief Define the GTM interrupt priority.  */
#define ISR_PRIORITY_STM2_TICK      64  /**< \brief Define the STM2 interrupt priority.  */
#define ISR_PRIORITY_STM1_TICK      65  /**< \brief Define the STM1 interrupt priority.  */
#define ISR_PRIORITY_STM0_TICK      66  /**< \brief Define the STM0 interrupt priority.  */

#define ISR_PRIORITY_STM3_TICK      73  /**< \brief Define the STM3 interrupt priority.  */
#define ISR_PRIORITY_STM4_TICK      74  /**< \brief Define the STM4 interrupt priority.  */
#define ISR_PRIORITY_STM5_TICK      75  /**< \brief Define the STM5 interrupt priority.  */

#define ISR_PRIORITY_B2B_UART_RX    68
#define ISR_PRIORITY_B2B_UART_TX    69
#define ISR_PRIORITY_B2B_UART_ERR   70

#define ISR_PRIORITY_HSSL_RX	    71  /**< \brief Define the HSSL RX interrupt priority. */

#define ISR_PRIORITY_ADC_CHANNEL    67
#define ISR_PRIORITY_HSSL0_BASE_PRIO   72
#define ISR_PRIORITY_CH0_TRG   		76
#define ISR_PRIORITY_CH1_TRG  		80
#define ISR_PRIORITY_HSCT0 			90
#define ISR_PRIORITY_HSCT1          91   //Not used

//#define ISR_PRIORITY_PRINTF_ASC0_TX 5   /**< \brief Define the ASC0 transmit interrupt priority used by printf.c */

#define ISR_PROVIDER_QSPI_TEGRA     0     /**< \brief Define the Qspi0 interrupt provider.  */
#define ISR_PROVIDER_QSPI2          0     /**< \brief Define the Qspi2 interrupt provider.  */
#define ISR_PROVIDER_ASC_SHELL      0
#define ISR_PROVIDER_VADC           0     /**< \brief Define the VADC interrupt provider. This select also the CPU which initialize and start the VADC. */
#define ISR_PROVIDER_I2C            2     /**< \brief Define the I2c interrupt provider.  */
#if 0
#define ISR_PROVIDER_LIN            1     /**< \brief Define the AsclinLin interrupt provider.  */
#endif
#define ISR_PROVIDER_ERAY           1     /**< \brief Define the Eray interrupt provider.  */
#define ISR_PROVIDER_ETHERNET       0     /**< \brief Define the Ethernet interrupt provider.  */
#define ISR_PROVIDER_MULTICAN       2
#define ISR_PROVIDER_HSCT 			0
/** \} */

#define CFG_ASC_SHELL_BAUDRATE    (115200.0)                   /**< \brief Define the Baudrate */
#define ASC_SHELL_TX_PIN          AURIX_DBG_UART_TX
#define ASC_SHELL_RX_PIN          AURIX_DBG_UART_RX
 
#define CFG_ASC_RX_BUFFER_SIZE    (512)                        /**< \brief Define the Rx buffer size in byte. */
#define CFG_ASC_TX_BUFFER_SIZE    (6 * 1024)                   /**< \brief Define the Tx buffer size in byte. */

#if 0
#define ISR_PROVIDER_QSPI0       IfxSrc_Tos_cpu0      /**< \brief Define the Qspi0 interrupt provider.  */

#define ISR_PROVIDER_QSPI2       IfxSrc_Tos_cpu0      /**< \brief Define the Qspi2 interrupt provider.  */
#endif
#if 0
#define ISR_PROVIDER_DMA_CH1        IfxSrc_Tos_cpu0     /**< \brief Define the Dma Channel1 interrupt provider.  */
#define ISR_PROVIDER_DMA_CH2        IfxSrc_Tos_cpu0     /**< \brief Define the Dma Channel2 interrupt provider.  */
#define ISR_PROVIDER_DMA_CH3        IfxSrc_Tos_cpu0     /**< \brief Define the Dma Channel3 interrupt provider.  */
#define ISR_PROVIDER_DMA_CH4        IfxSrc_Tos_cpu0     /**< \brief Define the Dma Channel4 interrupt provider.  */
#endif
/** \} */

/**
 * \name Interrupt configuration.
 * \{ */
#if 0
#define INTERRUPT_QSPI0_TX          ISR_ASSIGN(ISR_PRIORITY_QSPI_0_TX, ISR_PROVIDER_QSPI0)             /**< \brief Define the QSPI0 transmit interrupt priority.  */
#define INTERRUPT_QSPI0_RX          ISR_ASSIGN(ISR_PRIORITY_QSPI_0_RX, ISR_PROVIDER_QSPI0)             /**< \brief Define the QSPI0 receive interrupt priority.  */
#define INTERRUPT_QSPI0_ER          ISR_ASSIGN(ISR_PRIORITY_QSPI_0_ER, ISR_PROVIDER_QSPI0)             /**< \brief Define the QSPI0 error interrupt priority.  */

#define INTERRUPT_QSPI2_TX          ISR_ASSIGN(ISR_PRIORITY_QSPI2_TX, ISR_PROVIDER_QSPI2)             /**< \brief Define the QSPI2 transmit interrupt priority.  */
#define INTERRUPT_QSPI2_RX          ISR_ASSIGN(ISR_PRIORITY_QSPI2_RX, ISR_PROVIDER_QSPI2)             /**< \brief Define the QSPI2 receive interrupt priority.  */
#define INTERRUPT_QSPI2_ER          ISR_ASSIGN(ISR_PRIORITY_QSPI2_ER, ISR_PROVIDER_QSPI2)             /**< \brief Define the QSPI2 error interrupt priority.  */
#endif

#if 0
#define INTERRUPT_DMA_CH1           ISR_ASSIGN(ISR_PRIORITY_DMA_CH1, ISR_PROVIDER_DMA_CH1)               /**< \brief Define the Dma channel1 interrupt priority.  */
#define INTERRUPT_DMA_CH2           ISR_ASSIGN(ISR_PRIORITY_DMA_CH2, ISR_PROVIDER_DMA_CH2)               /**< \brief Define the Dma channel2 interrupt priority.  */
#define INTERRUPT_DMA_CH3           ISR_ASSIGN(ISR_PRIORITY_DMA_CH3, ISR_PROVIDER_DMA_CH3)               /**< \brief Define the Dma channel3 interrupt priority.  */
#define INTERRUPT_DMA_CH4           ISR_ASSIGN(ISR_PRIORITY_DMA_CH4, ISR_PROVIDER_DMA_CH4)               /**< \brief Define the Dma channel4 interrupt priority.  */
#endif
/** \} */

/** \} */
//------------------------------------------------------------------------------
#endif
