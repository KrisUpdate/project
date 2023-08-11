/**
 * \file AsclinShellInterface.h
 * \brief ASCLIN Shell interface demo
 *
 * \version disabled
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
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
 * \defgroup IfxLld_Demo_AsclinShellInterface_SrcDoc_Main Demo Source
 * \ingroup IfxLld_Demo_AsclinShellInterface_SrcDoc
 * \defgroup IfxLld_Demo_AsclinShellInterface_SrcDoc_Main_Interrupt Interrupts
 * \ingroup IfxLld_Demo_AsclinShellInterface_SrcDoc_Main
 */

#ifndef ASCLINSHELLINTERFACE_H
#define ASCLINSHELLINTERFACE_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Configuration.h"
#include "SysSe/Bsp/Bsp.h"

#include "SysSe/Comm/Ifx_Console.h"
#include "SysSe/Comm/Ifx_Shell.h"
#include "Asclin/Asc/IfxAsclin_Asc.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

#if defined(__TASKING__)
#  define SW_COMPILER_VERSION ((COMPILER_VERSION / 1000) << 16) | ((COMPILER_VERSION % 10) << 8) | (COMPILER_REVISION)
#endif

// Definition of user type, lower value means higher level
#define USER_TYPE_ADMINISTRATOR                            0x00   // Administrator / Supervisor
#define USER_TYPE_ADV_USER                                 0x01   // Advanced users like supplier
#define USER_TYPE_USER                                     0x02   // Typical users like customer
#define USER_TYPE_DEFAULT                                  USER_TYPE_USER

typedef enum {
    PORT_0              =0x01,
    PORT_1              =0x02,
    PORT_2              =0x03,
    PORT_3              =0x04,
    PORT_4              =0x05,
    PORT_5              =0x06,
    PORT_6              =0x07,
    PORT_7              =0x08,
    PORT_8              =0x09
} marvell_5050_port_t;

typedef enum {
    INCOUNTER              =0x00,
    OUTCOUNTER           =0x01,
    INUNICAST              =0x02,
    INBROCAST              =0x03,
    INMULTICAST            =0x04,
    OUTUNICAST              =0x05,
    OUTBROCAST              =0x06,
    OUTMULTICAST            =0x07,
    INDISCARDS                 =0x08,
    OUTDROPPS                 =0x09,
    OUTDISCARDS                 =0x0A,
    INUNDERSIZE               =0x0B,
    INFRAGMENT               =0x0C,
    INOVERSIZE                =0x0D,
    INJABBER                    =0x0E
} marvell_5050_data_t;

typedef enum {
    INCOUNTER_6321              =0x00,
    OUTCOUNTER_6321           =0x01,
    INUNICAST_6321              =0x02,
    INBROCAST_6321              =0x03,
    INMULTICAST_6321            =0x04,
    OUTUNICAST_6321             =0x05,
    OUTBROCAST_6321            =0x06,
    OUTMULTICAST_6321         =0x07,
    INDISCARDS_6321             =0x08,
    INRXERR_6321                   =0x09,
    INFCSERR_6321                 =0x0A,
    OUTFCSERR_6321               =0x0B,
    INUNDERSIZE_6321            =0x0C,
    INFRAGMENT_6321             =0x0D,
    INOVERSIZE_6321              =0x0E,
    INJABBER_6321                 =0x10
} marvell_6321_data_t;
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

typedef struct
{
    uint8 tx[CFG_ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
    uint8 rx[CFG_ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
} AppAscBuffer;

/** \brief Application information */
typedef struct
{
    struct
    {
        sint32 srcRev;                 /**< \brief Project source code revision number */
        sint32 srcRevDate;             /**< \brief Project source code revision Date */
        sint32 compilerVer;            /**< \brief Compiler version */
    }info;
    AppAscBuffer ascBuffer;            /**< \brief ASC interface buffer */
    struct
    {
        IfxAsclin_Asc asc;             /**< \brief ASC interface */
    }drivers;
    Ifx_Shell shell;                   /**< \brief Shell object */
    struct
    {
        IfxStdIf_DPipe asc;
    }stdIf;

    uint8 user_type;                   // User type for menu items, features, etc.
     
} App_AsclinShellInterface;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

IFX_EXTERN App_AsclinShellInterface g_AsclinShellInterface;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

IFX_EXTERN void AsclinShellInterface_init(void);
IFX_EXTERN void AsclinShellInterface_run(void);
IFX_EXTERN void AsclinShellInterface_execute_cmd(pchar commandLine, pchar arg);
/*\brief Print the message after
 *\param:  void
 *\return: void */
IFX_EXTERN void AsclinShellInterface_Print_Delay_Message(void);
IFX_EXTERN void AsclinShellInterface_printf(const char *format, ...);
IFX_EXTERN void AsclinShellInterface_printf_test(const char *format, ...);
IFX_EXTERN void AsclinShellInterface_printbuffer(uint8* buffer, uint16 size);
#endif
