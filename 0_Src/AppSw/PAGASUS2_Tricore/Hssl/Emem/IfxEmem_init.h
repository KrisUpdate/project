/**
 * \file IfxEmem_init.h
 * \brief Emem Initialization
 *
 * Copyright (c) 2015 Infineon Technologies AG. All rights reserved.
 *
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
 * \defgroup Ifx_APP_EMEM Application Emem Specific Part
 * \ingroup Ifx_HSSL_HSCT_APP
 * \addtogroup Ifx_APP_EMEM
 * \{
 * The Application Demo Software is using parts of the EMEM for application. Therefore
 * it is necessary to enable, configure and initialize the EMEM of the Device.
 * \section Ifx_EMEM_INIT Emem initialization
 * \subsection Ifx_EMEM_INIT_INCLUDE Include Files
 * Following header files are included:
 * \code
 * #include "IfxEmem_init.h"
 * #include "IfxEmem.h"
 * #include "IfxScuWdt.h"
 * \endcode
 * \subsection Ifx_EMEM_INIT_FUNCTIONS Initialization functions:
 * The following function call is used to enable, configure and initialize the Emem
 * \code
 * IfxEmem_AdasInit(); // initialize the system timer 0 to issue an interrupt every 1 ms
 * \endcode
 *
 * The initialization of the Emem needs to be done only once in the application.
 *
 * \}
 *
 * \defgroup Ifx_APP_EMEM_Function Application Main Function
 * \ingroup Ifx_APP_EMEM
 *
 */

#ifndef IFXEMEM_INIT_H_
#define IFXEMEM_INIT_H_
/*
 ******************************************************************************
 * Standard include files
 ******************************************************************************
 */
#include <Cpu/Std/Ifx_Types.h>


/*
 ******************************************************************************
 * Project include files
 ******************************************************************************
 */


/*
 ******************************************************************************
 * Macro defines
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Types
 ******************************************************************************
 */


/*
 ******************************************************************************
 * Variable declarations
 ******************************************************************************
 */


/*
 ******************************************************************************
 * Routine prototypes
 ******************************************************************************
 */
/** \addtogroup Ifx_APP_EMEM_Function
 * \{ */
/** \fn void IfxEmem_AdasInit(void)
 * \brief Emem Initialization routine
 * \param	-
 * \return	None
 * */
IFX_EXTERN void IfxEmem_Init(void);
/** \} */



#endif /* IFXEMEM_INIT_H_ */
