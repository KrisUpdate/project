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

#ifndef CONFIGURATIONISR_V31C_H
#define CONFIGURATIONISR_V31C_H
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
#define IFX_CFG_STM3_TICKS_PER_MS (100000) /**< \brief Number of STM3 ticks per millisecond */
#define IFX_CFG_STM4_TICKS_PER_MS (100000) /**< \brief Number of STM4 ticks per millisecond */
#define IFX_CFG_STM5_TICKS_PER_MS (100000) /**< \brief Number of STM5 ticks per millisecond */


#define ISR_PRIORITY_ASC_SHELL_RX 4         /**< \brief Define the ASC0 receive interrupt priority.  */
#define ISR_PRIORITY_ASC_SHELL_TX 20        /**< \brief Define the ASC0 transmit interrupt priority.  */
#define ISR_PRIORITY_ASC_SHELL_EX 12        /**< \brief Define the ASC0 error interrupt priority.  */

#define ISR_PRIORITY_TEGRA_UART_RX    25  /**< \brief Define the ASC SHELL receive interrupt priority.  */
#define ISR_PRIORITY_TEGRA_UART_TX    26  /**< \brief Define the ASC SHELL transmit interrupt priority.  */
#define ISR_PRIORITY_TEGRA_UART_EX    27  /**< \brief Define the ASC SHELL error interrupt priority.  */


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

#define ISR_PRIORITY_DMA_QSPI_4_TX   44
#define ISR_PRIORITY_DMA_QSPI_4_RX   45


#define ISR_PRIORITY_QSPI2_TX       50  /**< \brief Define the Qspi0 transmit interrupt priority.  */
#define ISR_PRIORITY_QSPI2_RX       51  /**< \brief Define the Qspi0 receive interrupt priority.  */

#define ISR_PRIORITY_QSPI_3_TX	     52
#define ISR_PRIORITY_QSPI_3_RX	     53


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
#define ISR_PRIORITY_STM5_TICK      61  /**< \brief Define the STM5 interrupt priority.  */
#define ISR_PRIORITY_STM4_TICK      62  /**< \brief Define the STM4 interrupt priority.  */
#define ISR_PRIORITY_STM3_TICK      63  /**< \brief Define the STM3 interrupt priority.  */
#define ISR_PRIORITY_STM2_TICK      64  /**< \brief Define the STM2 interrupt priority.  */
#define ISR_PRIORITY_STM1_TICK      65  /**< \brief Define the STM1 interrupt priority.  */
#define ISR_PRIORITY_STM0_TICK      66  /**< \brief Define the STM0 interrupt priority.  */
#define ISR_PRIORITY_ADC_CHANNEL    67


#define ISR_PROVIDER_QSPI_TEGRA     0     /**< \brief Define the Qspi0 interrupt provider.  */
#define ISR_PROVIDER_QSPI2          0     /**< \brief Define the Qspi2 interrupt provider.  */
#define ISR_PROVIDER_QSPI4          0     /*TODO: Confirm value for provider */ /**< \brief Define the Qspi4 interrupt provider.  */
#define ISR_PROVIDER_ASC_SHELL      0
#define ISR_PROVIDER_VADC           0     /**< \brief Define the VADC interrupt provider. This select also the CPU which initialize and start the VADC. */
#define ISR_PROVIDER_I2C            2     /**< \brief Define the I2c interrupt provider.  */
#define ISR_PROVIDER_LIN            1     /**< \brief Define the AsclinLin interrupt provider.  */
#define ISR_PROVIDER_ERAY           1     /**< \brief Define the Eray interrupt provider.  */
#define ISR_PROVIDER_ETHERNET       0     /**< \brief Define the Ethernet interrupt provider.  */
#define ISR_PROVIDER_MULTICAN       2


#define CFG_ASC_SHELL_BAUDRATE    (115200.0)                   /**< \brief Define the Baudrate */
#define ASC_SHELL_TX_PIN          AURIX_DBG_UART_TX
#define ASC_SHELL_RX_PIN          AURIX_DBG_UART_RX
 
#define CFG_ASC_RX_BUFFER_SIZE    (512)                        /**< \brief Define the Rx buffer size in byte. */
#define CFG_ASC_TX_BUFFER_SIZE    (6 * 1024)                   /**< \brief Define the Tx buffer size in byte. */

//------------------------------------------------------------------------------
#endif
