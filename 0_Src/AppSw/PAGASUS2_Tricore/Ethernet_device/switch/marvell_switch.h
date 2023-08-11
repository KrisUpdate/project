/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef MARVELL_SWITCH_H_
#define MARVELL_SWITCH_H_

#include <Configuration.h>

// Marvell Switch PHY ID
#define IFXETH_SW_88E88E6321_PHYID                          0

/* Marvell multi-chip address mode access registers */
#define IFXETH_SW_88E88E6321_SMI_COMMAND_DEVICE_ADDR        0
#define IFXETH_SW_88E88E6321_SMI_COMMAND                    0x0
#define IFXETH_SW_88E88E6321_SMI_DATA                       0x1

/* Bit Mask */
#define IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_BUSY           (0x1 << 15)
#define IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_MODE_CLAUSE_22 (0x1 << 12)
#define IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_OP_RD          (0x2 << 10)
#define IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_OP_WR          (0x1 << 10)
#define IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_DEV_ADDR(x)    (((x)&0x1F) << 5)
#define IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_REG_ADDR(x)    ((x)&0x1F)

//SMI/MDIO Addresses
#define IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR       0x1C
#define IFXETH_SW_88E88E6321_PORT_0_SMI_DEVICE_ADDR         0x10
#define IFXETH_SW_88E88E6321_PORT_1_SMI_DEVICE_ADDR         0x11
#define IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR         0x12
#define IFXETH_SW_88E88E6321_PORT_3_SMI_DEVICE_ADDR         0x13
#define IFXETH_SW_88E88E6321_PORT_4_SMI_DEVICE_ADDR         0x14
#define IFXETH_SW_88E88E6321_PORT_5_SMI_DEVICE_ADDR         0x15
#define IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR           0x1B

//Marvell Switch Registers
#define IFXETH_SW_88E88E6321_GLOBAL_1_VTU_FID           0X2
#define IFXETH_SW_88E88E6321_GLOBAL_1_VTU_OPERATION     0X5
#define IFXETH_SW_88E88E6321_GLOBAL_1_VTU_VID           0X6
#define IFXETH_SW_88E88E6321_GLOBAL_1_VTU_STU_DATA_0_3      0X7
#define IFXETH_SW_88E88E6321_GLOBAL_1_VTU_STU_DATA_4_6      0X8
#define IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND       0X18
#define IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_DATA          0X19

#define IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS               0X0
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL     0X1
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_CONTROL              0X4
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_CONTROL_1                0X5
#define IFXETH_SW_88E88E6321_PER_PORT_DEFAULT_VID_PRI               0X7
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_CONTROL_2                0X8
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_BASE_VLAN_MAP        0X6

//Marvell Switch Register Bit
#define IFXETH_SW_88E88E6321_GLOBAL_1_VTU_OPERATION_COMMAND_BIT_VTUBUSY         (0X1 << 15)
#define IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND_BIT_SMIBUSY       (0X1 << 15)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_FORCEDLINK      (0X1 << 4)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_LINKVALUE       (0X1 << 5)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_RGMII_TX_TIMING         (0X1 << 14)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_RGMII_RX_TIMING         (0X1 << 15)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_FORCESPD                (0X3 << 0)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_FORCESPD_10_Mbps        (0X0 << 0)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_FORCESPD_100_Mbps       (0X1 << 0)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_FORCESPD_1000_Mbps      (0X2 << 0)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_FORCESPD_NORMAL         (0X3 << 0)

#define IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_LINK                          (0x1 << 11)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_LINK_UP                       (0x1 << 11)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_LINK_DOWN                     (0x0 << 11)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_SPEED                         (0x3 << 8)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_SPEED_100Mbps                 (0x1 << 8)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_C_MODE                        (0x7 << 0)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_C_MODE_RGMII                  (0x7 << 0)
#define IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_C_MODE_DISABLED               (0x6 << 0)


//Marvell Switch Registers
#define IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_COMMAND       0X16
#define IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_DATA          0X17

#define IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_COMMAND_BIT_SMIBUSY       (0X1 << 15)

#define MARVELL_88E6321_DEVICE_ID  (0x310 << 4)

/*\brief Read register value from Marvell switch
 *\param:  slaveaddr         uint32    MDIO slave address
 *\param:  layeraddr         uint32    device address, AKA layer address
 *\param:  regaddr           uint32    register address
 *\param:  pdata             uint32*   the pointer to store the read back value
 *\return: void */
IFX_EXTERN void IfxEth_Marvell_Switch_read_reg(uint32 slaveaddr, uint32 layeraddr, uint32 regaddr, uint32 *pdata);

/*\brief Write register value to Marvell switch
 *\param:  slaveaddr         uint32    MDIO slave address
 *\param:  layeraddr         uint32    device address, AKA layer address
 *\param:  regaddr           uint32    register address
 *\param:  data              uint32    the value to write
 *\return: void */
IFX_EXTERN void IfxEth_Marvell_Switch_write_reg(uint32 slaveaddr, uint32 layeraddr, uint32 regaddr, uint32 data);

IFX_EXTERN void IfxEth_Sw_88E88E6321_read_phy_reg(uint32 slaveaddr, uint8 port, uint8 phy_offset, uint32 *pdata);
IFX_EXTERN void IfxEth_Sw_88E88E6321_write_phy_reg(uint32 slaveaddr, uint8 port, uint8 phy_offset, uint32 newdata);
IFX_EXTERN void IfxEth_Sw_88E88E6321_read_avb_reg(uint32 slaveaddr, uint8 port, uint8 phy_offset, uint32 *pdata);
IFX_EXTERN void IfxEth_Sw_88E88E6321_write_avb_reg(uint32 slaveaddr, uint8 port, uint8 phy_offset, uint32 newdata);

IFX_EXTERN void IfxEth_Marvell_Sw_Init(uint32 slaveaddr);
IFX_EXTERN void IfxEth_Marvell_Sw_Set(uint32 slaveaddr, uint32 port, uint32 speed);
IFX_EXTERN void IfxEth_Marvell_Hyperion_ExtraInit(void);
IFX_EXTERN void IfxEth_Marvell_Sw_Vlan_Enable(uint32 slaveaddr);
IFX_EXTERN void IfxEth_Marvell_Sw_Vlan_Disable(uint32 slaveaddr);
IFX_EXTERN void IfxEth_Marvell_Sw_Tag_Vlan_Enable(uint32 slaveaddr);
IFX_EXTERN boolean IfxEth_Marvell_Sw_Is_Link(uint32 slaveaddr);
IFX_EXTERN void IfxEth_Marvell_Sw_init_only_vlan200(void);
IFX_EXTERN void IfxEth_Marvell_Sw_Loadsim_Init(uint32 slaveaddr);
IFX_EXTERN void IfxEth_Marvell_Sw_ExposeAurix_Init(uint32 slaveaddr);
//Marvell PHY Registers

#endif /* MARVELL_SWITCH_H_ */
