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
#include <Eray/Eray/IfxEray_Eray.h>

#ifndef _ERAY_H
#define _ERAY_H

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define NO_OF_SLOTS             3   /* Number of slots configured (1xtransmit+2xreceive */

#define TRANSMIT_BUFFER_INDEX   0   /* Number of buffer used for transmit */
#define RECEIVE_BUFFER_A_INDEX  1   /* Number of buffer used for receive on A */
#define RECEIVE_BUFFER_B_INDEX  2   /* Number of buffer used for receive on A */

#define FRAME_ID_MIN       (0)
#define FRMAE_ID_MAX       (0x7FF)
#define FRMAE_ID_INGORE    (0xFFFF)

#define ERAY_NONE	0
#define ERAY_MASTER	1
#define ERAY_SLAVE	2

#define ERAY_RECV_CHA	1
#define ERAY_RECV_CHB	2
#define ERAY_RECV_CHAB	3
#define ERAY_NORMAL_RECV	0x10

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct
{
    IfxEray_Eray            eray;                                     /**< \brief Eray handle */
    IfxEray_Eray_Config     erayModuleConfig;                         /**< \brief Eray module Configuration structure */
    IfxEray_Eray_NodeConfig nodeConfig;                               /**< \brief Eray Node Configuration structure */
    boolean                 ndat_A;                                   /**< \brief received data (new data) on channel A */
    boolean                 ndat_B;                                   /**< \brief received data (new data) on channel B */
    volatile unsigned       errors;                                   /**< \brief error interrupt counter */
    uint32                  receivedData_A[2];                        /**< \brief received data on channel A */
    uint32                  receivedData_B[2];                        /**< \brief received data on channel B */
    boolean                 initialized;                              /**< \brief Eray Initialized if set */
} App_ErayTestMode;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN App_ErayTestMode g_ErayTestMode;
static boolean eraystress_initialized = FALSE;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void IfxErayTestMode_init(boolean monitor, boolean loopback, uint32 frame_id, uint8 core);
IFX_EXTERN void send_eray_frame(uint32 *pData, uint32 wordlength);
IFX_EXTERN uint32 testeray(uint8 mode);
IFX_EXTERN void run_eraystress_test();
#endif /* _ERAY_H */
