/**
 * \file Ifx_Cfg.h
 * \brief Configuration.
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
 * \author Naveen Gururaj <Naveen.Gururaj@Infineon.com>
 *
 * \defgroup IfxLld_Demo_GethBasic_SrcDoc_IlldConfig iLLD configuration
 * \ingroup IfxLld_Demo_GethBasic_SrcDoc
 */

#ifndef IFX_CFG_H
#define IFX_CFG_H

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Demo_AsclinShellInterface_SrcDoc_IlldConfig
 * \{ */

/** \addtogroup IfxLld_Demo_GethBasic_SrcDoc_IlldConfig
 * \{ */

/*______________________________________________________________________________
** Configuration for IfxScu_cfg.h
**____________________________________________________________________________*/
/**
 * \name Frequency configuration
 * \{
 */
#define IFX_CFG_SCU_XTAL_FREQUENCY (20000000)                       /**< \copydoc IFX_CFG_SCU_XTAL_FREQUENCY */
#define IFX_CFG_SCU_PLL_FREQUENCY		(300000000) /**< default supported: 300000000, 200000000, 160000000, 133000000, 80000000 */
#define IFX_CFG_SCU_PLL1_FREQUENCY		(320000000) /**< default supported: 320000000, 160000000 */
#define IFX_CFG_SCU_PLL2_FREQUENCY		(200000000) /**< default supported: 200000000 */
/** \} */

/*______________________________________________________________________________
** Configuration for Ifx_Assert.h
**____________________________________________________________________________*/
/**
 * \name Assert configuration
 * \{
 */

#define IFX_CFG_ASSERT_VERBOSE_LEVEL_DEFAULT (IFX_VERBOSE_LEVEL_INFO)   /**< \copydoc IFX_CFG_ASSERT_VERBOSE_LEVEL_DEFAULT */
#define IFX_CFG_ASSERT_STDIO                 (1)                        /**< \copydoc IFX_CFG_ASSERT_STDIO */
#define IFX_CFG_ASSERT_INCLUDE               "SysSe/Bsp/Assert.h"       /**< \copydoc IFX_CFG_ASSERT_INCLUDE */

/** \} */

/** \} */

#endif /* IFX_CFG_H */
