/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef ETHERNETCMDS_H
#define ETHERNETCMDS_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define AURIX_MAC_H_LOCATION  0XAF0B0014;
#define AURIX_MAC_L_LOCATION  0XAF0B0010;
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
IFX_EXTERN boolean mdioinit(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean mdioset(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean mdiord(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean mdiowr(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean readSWitchPhy(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean writeSWitchPhy(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean readSWitchAvb(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean writeSWitchAvb(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethernetinit(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethernetvlan(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethernetping(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethernetstat(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethernetpingdetect(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethernetiperf(pchar args, void *data, IfxStdIf_DPipe *io);
#if PROJECT == PROJECT_PAGASUS2
IFX_EXTERN boolean ethswitchread(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethswitchwrite(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethphy1512read(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethphy1512write(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethphy107read(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethphy107write(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethswitchloadsim(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethswitchexposeaurix(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean swpakcnt(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean nonVlanConfig(pchar args, void *data, IfxStdIf_DPipe *io);
#endif
#if ETH_PHY_88Q2112_ENABLE == 1
IFX_EXTERN boolean ethphyinit(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethphyread(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ethphywrite(pchar args, void *data, IfxStdIf_DPipe *io);
#endif //#if ETH_PHY_88Q2112_ENABLE == 1
#endif
