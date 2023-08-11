/**
 * \file IfxGtm_Tim_Timer.h
 * \brief GTM TIMER details
 * \ingroup IfxLld_Gtm
 *
 * \version iLLD_1_0_1_9_0
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
 * \defgroup IfxLld_Gtm_Tim_Timer TIM Timer Interface
 * \ingroup IfxLld_Gtm_Tim
 */

#ifndef IFXGTM_TIM_TIMER_H
#define IFXGTM_TIM_TIMER_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Gtm/Std/IfxGtm_Tim.h"
#include "Gtm/Std/IfxGtm_Cmu.h"
#include "_Impl/IfxGtm_cfg.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \brief Tim handle data structure
 */
typedef struct
{
    Ifx_GTM      *gtm;            /**< \brief Pointer to GTM module */
    Ifx_GTM_TIM  *tim;            /**< \brief Pointer to TIM module */
    IfxGtm_Tim    timIndex;       /**< \brief Specifies the  TIM module no */
    IfxGtm_Tim_Ch channel;        /**< \brief Tim channel no */
} IfxGtm_Tim_Timer;

/** \brief Tim configuration data structure
 */
typedef struct
{
    IfxGtm_Tim                   timIndex;                         /**< \brief Specifies the TIM instance number */
    IfxGtm_Tim_Ch                channel;                          /**< \brief Tim channel no */
    IfxGtm_Tim_ChannelControl    channelControl;                   /**< \brief Control configuration */
    uint32                       raisingEdgeFilterTime;            /**< \brief Filter Raising Edge parameter */
    uint32                       fallingEdgeFilterTime;            /**< \brief Filter Falling Edge parameter */
    uint32                       shadowCounter;                    /**< \brief Shadow counter value */
    boolean                      irqEnable;                        /**< \brief enable interrupt notification */
    IfxGtm_IrqMode               irqMode;                          /**< \brief interrupt mode (level/pulse/pulse notify/single pulse) */
    IfxGtm_Tim_IrqType           irqType;                          /**< \brief interrupt type (new value/ ecnt overflow/ gpr overflow/ cnt overflow / TO detect / glitch detect) */
    IfxGtm_Tim_InputSourceSelect inputSourceSelect;                /**< \brief input source selection (mode , Value) */
    boolean                      enableAuxInputSourceSelect;       /**< \brief enable aurilliary input source selection */
} IfxGtm_Tim_Timer_Config;

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initialises the TIM channel object
 * \param driver TOM Timer interface Handle
 * \param config Configuration structure for TIM
 * \return None
 */
IFX_EXTERN void IfxGtm_Tim_Timer_initChannel(IfxGtm_Tim_Timer *driver, IfxGtm_Tim_Timer_Config *config);

/** \brief Initialise the config struct with default TIM channel configuration
 * \param config pointer to the TIM Channel configuration (it will be initialized by this function)
 * \param gtm Pointer to GTM module
 * \return None
 */
IFX_EXTERN void IfxGtm_Tim_Timer_initChannelConfig(IfxGtm_Tim_Timer_Config *config, Ifx_GTM *gtm);

#endif /* IFXGTM_TIM_TIMER_H */
