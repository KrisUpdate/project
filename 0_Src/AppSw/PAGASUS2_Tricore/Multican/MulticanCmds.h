/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef MULTICANCMDS_H
#define MULTICANCMDS_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "SysSe/Bsp/Bsp.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef enum
{
    CAN_CMD_TEST_STRESS,
//    CAN_CMD_TEST_LOOP,
//    CAN_CMD_TEST_SINGLE,
    CAN_CMD_TEST_TP,
    CAN_CMD_SHOWLOG,
} App_Multican_CmdTestMode;

typedef struct
{
    App_Multican_CmdTestMode Mode;
    boolean  flag;
    uint32   txNode;
    uint32   rxNode;
    uint32   LoopCount;
    uint32   DataLength;
    uint32   CanData[16];
    uint32   StartTime;
    uint32   EndTime;
    uint32   LimitTime;
    uint32   Cnt;
} App_Multican_CmdTest;
/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
extern volatile boolean canstress_test;
extern App_Multican_CmdTest CmdTest;

extern uint32 can_cnt_print;
extern boolean broadcast_mode;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
void Multican_CmdTestInitial(void);
IFX_EXTERN boolean multicaninit(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean cansetup(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean show_cansetup(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean multicansend(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean send_CAN_Wakeup(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean canlp(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean CANWakeTest(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean setudpip(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean cantoudp(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN void Print_MulticanMsg(IfxStdIf_DPipe *io);
IFX_EXTERN void Print_CAN_Wakeup_detection(IfxStdIf_DPipe *io);
IFX_EXTERN boolean canstress(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean canstress_H(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean cancnt(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN void canstress_run(void);
IFX_EXTERN boolean canstress_H(pchar args, void *data, IfxStdIf_DPipe *io);
boolean canecho(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean canfilter(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean multicansend_test(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN void cansend_conti_do(void);
IFX_EXTERN boolean set_canbus_rx_show(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean can_measure(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean canclear(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean canthroughput(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN void Multican_CmdTest(void);
IFX_EXTERN void Multican_printTpResult(void);
IFX_EXTERN void Multican_setCanCmdTestMode(App_Multican_CmdTestMode mode);
IFX_EXTERN boolean cantestcase(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN void cpu2_can_test(void);
IFX_EXTERN void cpu1_can_test(void); 
#endif
