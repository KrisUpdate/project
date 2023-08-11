/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef IFXETH_PHY_88q2112_H
#define IFXETH_PHY_88q2112_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

//#include "Eth/Std/IfxEth.h"
#include <Configuration.h>
/** \addtogroup IfxLld_Eth_Phy_88q2112_Functions
 * \{ */

/******************************************************************************/
/*----------------------------------Macros------------------------------------*/
/******************************************************************************/
#if DRIVE_AX_A01 == 1
#define IFXETH_PHY_88Q2112_1_PHYID                    1
#define IFXETH_PHY_88Q2112_2_PHYID                    2
#define IFXETH_PHY_88Q2112_3_PHYID                    3
#elif DRIVE_AX_B0x == 1
#define IFXETH_PHY_88Q2112_1_PHYID                    1  //I15, U178
#define IFXETH_PHY_88Q2112_2_PHYID                    2  //I30, U181
#define IFXETH_PHY_88Q2112_3_PHYID                    3  //I13, U183
#define IFXETH_PHY_88Q2112_4_PHYID                    4  //I19, U185
#define IFXETH_PHY_88Q2112_5_PHYID                    5  //I37, U160
#define IFXETH_PHY_88Q2112_6_PHYID                    6  //I38, U162
#define IFXETH_PHY_88Q2112_7_PHYID                    7  //I11, U176
#endif //endof #if DRIVE_AX_A01 == 1

#define IFXETH_PHY_88Q2112_DEV_1_ADDR               0x01
#define IFXETH_PHY_88Q2112_DEV_3_ADDR               0x03
#define IFXETH_PHY_88Q2112_REG_13_14_DEVICE_ADDR    0
#define IFXETH_PHY_88Q2112_REG_13_ADDR              13
#define IFXETH_PHY_88Q2112_REG_14_ADDR              14
#define IFXETH_PHY_88Q2112_1000BASET1_STA_REG_1   0x0901  /* Dev Addr 0x3 */
#define IFXETH_PHY_88Q2112_1000BASET1_STA_REG_2   0x0902  /* Dev Addr 0x3 */
#define IFXETH_PHY_88Q2112_PMA_PMD_CTRL_REG       0x0834  /* Dev Addr 0x1 */
#define IFXETH_PHY_88Q2112_MASTER_SLAVE_BIT_MASK  (0x1 << 14)
#define IFXETH_PHY_88Q2112_SPEED_BIT_MASK         0x1

typedef enum
{
    HSD_MASTER,
    HSD_SLAVE,
}e_role;

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

// Read Legacy/Compliance configuration from InfoROM (Q2112)
IFX_EXTERN uint8 Chk_InfoROM_88q2112_Mode_by_PHYID(uint8 phyid);

// Read Master/Slave configuration from InfoROM (Q2112)
IFX_EXTERN uint8 Chk_InfoROM_88q2112_MS_by_PHYID(uint8 phyid);

IFX_EXTERN void IfxEth_Phy_88q2112_read_mdio_reg(uint32 layeraddr, uint32 devaddr, uint32 regaddr, uint32 *pdata);


IFX_EXTERN void IfxEth_Phy_88q2112_write_mdio_reg(uint32 layeraddr, uint32 devaddr, uint32 regaddr, uint32 data);


IFX_EXTERN void IfxEth_Phy_88q2112_z1_fix();


/**
 * \return Status
 */
//IFX_EXTERN void IfxEth_Phy_88q2112_Set(uint32 slaveaddr, uint32 speed, e_role role);
IFX_EXTERN void IfxEth_Phy_88q2112_Set(uint32 slaveaddr, uint32 speed, e_role role, uint8 mode_sel);

/**
 * \return Status
 */
IFX_EXTERN boolean IfxEth_Phy_88q2112_init(uint32 phyid);

/**
 * \return Status
 */
IFX_EXTERN boolean IfxEth_Phy_88q2112_deinit(uint32 phyid);

/**
 * \return Link status
 */
IFX_EXTERN boolean IfxEth_Phy_88q2112_link(uint32 phyid);

/**
 * \return the number of 88Q2112
 */
IFX_EXTERN uint32 get_88q2112_number(void);

/******************************************************************************/
/*-------------------Global Exported Variables/Constants----------------------*/
/******************************************************************************/

IFX_EXTERN uint32 IfxEth_Phy_88q2112_1_iPhyInitDone;
IFX_EXTERN uint32 IfxEth_Phy_88q2112_2_iPhyInitDone;
IFX_EXTERN uint32 IfxEth_Phy_88q2112_3_iPhyInitDone;

#endif /* IFXETH_PHY_88q2112_H */
