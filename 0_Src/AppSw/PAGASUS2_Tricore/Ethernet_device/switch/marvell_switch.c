/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#include "marvell_switch.h"
#ifdef TC397B
#include <Geth/Eth_Devices/IfxEth_Devices.h>
#else
#include <Eth/Eth_Devices/IfxEth_Devices.h>
#endif
#include <internal_fw_debug.h>
#include <Configuration.h>
#include "board.h"
#include "error_report.h"
#include "config_pg2_feasures.h"  
#if ETH_SWITCH_ENABLE == 1

#define MVR_SWITCH_DEBUG_ENABLE 1

#if MVR_SWITCH_DEBUG_ENABLE == 1
#define MVR_SWITCH_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define MVR_SWITCH_DEBUG_PRINTF(x)
#endif

extern boolean boot_msg_flag;
#define MVR_SWITCH_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)
#define PRINTF_BOOT(x)  ({if(boot_msg_flag){DEBUG_PRINTF_ALWAYS(x);}})


#define MDIO_WAIT_TIME_MS 200

static boolean isCpuMode1 = FALSE;
static boolean isCpuMode2 = FALSE;

 #if 1
static boolean Wait_For_Command_Read(uint32 slaveaddr)
{
    uint32 l_data = 0, count = 0;
    IfxEth_read_mdio_reg(slaveaddr, IFXETH_SW_88E88E6321_SMI_COMMAND_DEVICE_ADDR, IFXETH_SW_88E88E6321_SMI_COMMAND, &l_data);
    while((l_data & IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_BUSY) != 0)
    {
        count++;
        waitTime(TimeConst[TIMER_INDEX_1MS]);
        if(count > MDIO_WAIT_TIME_MS)
        {
            if(slaveaddr==0x10) MVR_SWITCH_ALWAYS_PRINTF(("Error: Time out waiting for MDIO 88E6321_1 Command busy bit\r\n"));
			else if(slaveaddr==0x12) MVR_SWITCH_ALWAYS_PRINTF(("Error: Time out waiting for MDIO 88E6321_2 Command busy bit\r\n"));
			else if(slaveaddr==0x11) MVR_SWITCH_ALWAYS_PRINTF(("Error: Time out waiting for MDIO 88Q5050 Command busy bit\r\n"));
			else MVR_SWITCH_ALWAYS_PRINTF(("Error: Time out waiting for MDIO others device Command busy bit\r\n"));
			
            return FALSE;
        }
        IfxEth_read_mdio_reg(slaveaddr, IFXETH_SW_88E88E6321_SMI_COMMAND_DEVICE_ADDR, IFXETH_SW_88E88E6321_SMI_COMMAND, &l_data);
    }

    return TRUE;
}

/* General function to support register read/write for Marvell switch 88E6321, 88Q5050
 * If in single chip mode, we can access register by clause 22 SMI directly
 * If in multi-chip mode, we need to use SMI Command/Data register to access register indirectly
 */

void IfxEth_Marvell_Switch_read_reg(uint32 slaveaddr, uint32 layeraddr, uint32 regaddr, uint32 *pdata)
{
#if SINGLE_CHIP_ADDRESS_MODE_ENABLE == 1
    MVR_SWITCH_DEBUG_PRINTF(("%s: Single chip mode\r\n", __func__));
    IfxEth_read_mdio_reg(slaveaddr, layeraddr, regaddr, pdata);
#else
    /*  layer address of command, data register is 0 */
    uint32 command=0, l_data=0;
//  MVR_SWITCH_DEBUG_PRINTF(("%s: Multi-chip mode: wait for SMI COMMAND busy bit to be ready (0) \r\n", __func__));
    /* Wait for busy bit to be 0 */
    if(!Wait_For_Command_Read(slaveaddr))
    { /* if commmand busy bit is not ready, then we stop it */
        return;
    }

//  MVR_SWITCH_DEBUG_PRINTF(("%s: write the command \r\n", __func__));
    command = IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_BUSY                |
              IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_MODE_CLAUSE_22      |
              IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_OP_RD               |
              IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_DEV_ADDR(layeraddr) |
              IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_REG_ADDR(regaddr);
    IfxEth_write_mdio_reg(slaveaddr, IFXETH_SW_88E88E6321_SMI_COMMAND_DEVICE_ADDR, IFXETH_SW_88E88E6321_SMI_COMMAND, command);


//  MVR_SWITCH_DEBUG_PRINTF(("%s: wait for SMI COMMAND busy bit to be ready (0) \r\n", __func__));
    /* Wait for busy bit to be 0 */
    if(!Wait_For_Command_Read(slaveaddr))
    { /* if commmand busy bit is not ready, then we stop it */
        return;
    }

    MVR_SWITCH_DEBUG_PRINTF(("%s: read data \r\n", __func__));
    IfxEth_read_mdio_reg(slaveaddr, IFXETH_SW_88E88E6321_SMI_COMMAND_DEVICE_ADDR, IFXETH_SW_88E88E6321_SMI_DATA, pdata);
#endif
}


void IfxEth_Marvell_Switch_write_reg(uint32 slaveaddr, uint32 layeraddr, uint32 regaddr, uint32 data)
{
#if SINGLE_CHIP_ADDRESS_MODE_ENABLE == 1
    MVR_SWITCH_DEBUG_PRINTF(("%s: Single chip mode\r\n", __func__));
    IfxEth_write_mdio_reg(slaveaddr, layeraddr, regaddr, data);
#else
    uint32 command=0, l_data=0;
//  MVR_SWITCH_DEBUG_PRINTF(("%s: Multi-chip mode: wait for SMI COMMAND busy bit to be ready (0) \r\n", __func__));
    /* Wait for busy bit to be 0 */
    if(!Wait_For_Command_Read(slaveaddr))
    { /* if commmand busy bit is not ready, then we stop it */
        return;
    }

    /* write the data to the SMI DATA register */
    IfxEth_write_mdio_reg(slaveaddr, IFXETH_SW_88E88E6321_SMI_COMMAND_DEVICE_ADDR, IFXETH_SW_88E88E6321_SMI_DATA, data);

//  MVR_SWITCH_DEBUG_PRINTF(("%s: write the command and data \r\n", __func__));
    command = IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_BUSY                |
              IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_MODE_CLAUSE_22      |
              IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_OP_WR               |
              IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_DEV_ADDR(layeraddr) |
              IFXETH_SW_88E88E6321_SMI_COMMAND_BIT_SMI_REG_ADDR(regaddr);
    IfxEth_write_mdio_reg(slaveaddr, IFXETH_SW_88E88E6321_SMI_COMMAND_DEVICE_ADDR, IFXETH_SW_88E88E6321_SMI_COMMAND, command);

//  MVR_SWITCH_DEBUG_PRINTF(("%s: wait for SMI COMMAND busy bit to be ready (0) \r\n", __func__));
    /* Wait for busy bit to be 0 */
    if(!Wait_For_Command_Read(slaveaddr))
    { /* if commmand busy bit is not ready, then we stop it */
        return;
    }
#endif
}


void IfxEth_Sw_88E88E6321_read_phy_reg(uint32 slaveaddr, uint8 port, uint8 phy_offset, uint32 *pdata)
{
    uint32 command = 0;
    uint32 data;
    command = (0x26 <<10) | (port << 5) | phy_offset;
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND,command);

    IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND, &data);//wait until smi is available
    while (data & IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND_BIT_SMIBUSY)
    {
        IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND, &data);
    }
    IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_DATA, &data);
    * pdata = data;
}


void IfxEth_Sw_88E88E6321_write_phy_reg(uint32 slaveaddr, uint8 port, uint8 phy_offset, uint32 newdata)
{
    uint32 command = 0;
    uint32 data;
    command = (0x25 <<10) | (port << 5) | phy_offset;

    IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND, &data);//wait until smi is available
    while (data & IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND_BIT_SMIBUSY)
    {
        IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND, &data);
    }
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_DATA, newdata);
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND,command);

    IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND, &data);//wait until smi is available
    while (data & IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND_BIT_SMIBUSY)
    {
        IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND, &data);
    }
}
#endif

void IfxEth_Sw_88E88E6321_read_avb_reg(uint32 slaveaddr, uint8 port, uint8 phy_offset, uint32 *pdata)
{
    uint32 command = 0;
    uint32 data;
    command = (1 << 15) | (0b100 << 12) | (port << 8) | (0 << 5) | phy_offset;

    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_COMMAND,command);

    IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_COMMAND, &data);//wait until smi is available
    while (data & IFXETH_SW_88E88E6321_GLOBAL_2_SMI_PHY_COMMAND_BIT_SMIBUSY)
    {
        IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_COMMAND, &data);
    }
    IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_DATA, &data);
    * pdata = data;
}

void IfxEth_Sw_88E88E6321_write_avb_reg(uint32 slaveaddr, uint8 port, uint8 phy_offset, uint32 newdata)
{
    uint32 command = 0;
    uint32 data;
    command = (1 << 15) | (0b011 << 12) | (port << 8) | (0 << 5) | phy_offset;

    IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_COMMAND, &data);//wait until smi is available
    while (data & IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_COMMAND_BIT_SMIBUSY)
    {
        IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_COMMAND, &data);
    }

    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_DATA, newdata);
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_COMMAND,command);

    IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_COMMAND, &data);//wait until smi is available
    while (data & IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_COMMAND_BIT_SMIBUSY)
    {
        IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_2_SMI_AVB_COMMAND, &data);
    }
}

#if 1
static boolean read_cpu_mode(void)
{
    uint32 val = 0;
    /* read the port control register, bit 1:0 port state. If it's disable, then it's CPU mode */
    IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x10, 0x04, &val);
    if((val & 0x3) == 0)
    {
        MVR_SWITCH_ALWAYS_PRINTF(("88E6321_1 is in CPU mode\r\n"));
        isCpuMode1 = TRUE;
    }
    else
    {
        MVR_SWITCH_ALWAYS_PRINTF(("88E6321_1 is in NO CPU mode\r\n"));
        isCpuMode1 = FALSE;
    }

    IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x10, 0x04, &val);
    if((val & 0x3) == 0)
    {
        MVR_SWITCH_ALWAYS_PRINTF(("88E6321_2 is in CPU mode\r\n"));
        isCpuMode2 = TRUE;
    }
    else
    {
        MVR_SWITCH_ALWAYS_PRINTF(("88E6321_2 is in NO CPU mode\r\n"));
        isCpuMode2 = FALSE;
    }
}


/* Enable the internal PHY for port 3, port 4 */
static void enable_phy(uint32 slaveaddr)
{
    uint32 val = 0;
    MVR_SWITCH_DEBUG_PRINTF(("Enabling phy in 88e6321 0x%x\r\n", slaveaddr));
    /* Enable PHY */
    IfxEth_Sw_88E88E6321_read_phy_reg(slaveaddr, 3, 0, &val);
    val &= (~0x800);
    IfxEth_Sw_88E88E6321_write_phy_reg(slaveaddr, 3, 0, val);

    IfxEth_Sw_88E88E6321_read_phy_reg(slaveaddr, 4, 0, &val);
    val &= (~0x800);
    IfxEth_Sw_88E88E6321_write_phy_reg(slaveaddr, 4, 0, val);
}


/* Enable the internal Serdes for port 0, port 1 */
static void enable_serdes(uint32 slaveaddr)
{
    uint32 val = 0;
    MVR_SWITCH_DEBUG_PRINTF(("Enabling SGMII serdes in 88e6321 0x%x\r\n", slaveaddr));
    /* Enable PHY */
    IfxEth_Sw_88E88E6321_read_phy_reg(slaveaddr, 0xc, 0, &val);
    val &= (~0x800);
    IfxEth_Sw_88E88E6321_write_phy_reg(slaveaddr, 0xc, 0, val);

    IfxEth_Sw_88E88E6321_read_phy_reg(slaveaddr, 0xd, 0, &val);
    val &= (~0x800);
    IfxEth_Sw_88E88E6321_write_phy_reg(slaveaddr, 0xd, 0, val);
}


static boolean Check_88E6321_Device_ID(uint32 slaveaddr)
{
    boolean ret = FALSE;
    uint32 value=0;
    IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x10, 0x03, &value);
    if((value & 0xfff0) == MARVELL_88E6321_DEVICE_ID)
    {
        return TRUE;
    }
    else
    {
        MVR_SWITCH_ALWAYS_PRINTF(("Device ID in 88E6321 slave address 0x%x is not expected\r\n", slaveaddr));
        MVR_SWITCH_ALWAYS_PRINTF(("Expected 0x3102, but reading back 0x%x\r\n", value));
        return FALSE;
    }

}

/* TODO: create a set of initial function for different project, and maintain them according project macro
 *
 *  IfxEth_Marvell_Sw_init_ACH
 *  IfxEth_Marvell_Sw_init_DDPX_A01
 *  IfxEth_Marvell_Sw_init_DDPX_B0X
 *
 *  The process follows the init process in bug http://nvbugs/200355368
 *  */

static void IfxEth_Marvell_Sw_init_DDPX_B0X(uint32 slaveaddr)
{ /* Todo: we can add table to do it, and need to clean up to use macro
     but right now just go ahead for bring up */
    uint32 val, devaddr, count;
    static boolean is_cpu_mode_checked = FALSE;
    if(Check_88E6321_Device_ID(slaveaddr) == FALSE)
    {
        set_error_flag(INVAILD_88E6321_DEVICE_ERR);
        return;
    }
    if(!is_cpu_mode_checked)
    { /* For CPU mode, we only check once */
        is_cpu_mode_checked = TRUE;
        read_cpu_mode();
    }
/* if CPU mode is enable, we need to initial everything by ourselves */
#if MARVELL_CPU_MODE_ENABLE == 1
    if(slaveaddr == IFXETH_SWITCH_88E6321_1_ADDR)
    {
        MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_1\r\n"));
        /****************  Disable all ports ********************/
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val &= 0xFFFC;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }
        /* we don't enable it in bring up*/
#if MARVELL_MANAGEMENT_CONFIG_ENABLE == 1
        /****************  Multichip configuration ********************/
        // Set management link on P4 and P6 (port control[9:8] = 0b11 : EtherType DSA mode)
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x04, &val);
        val |= 0x0300;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x04, val);

        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x16, 0x04, &val);
        val |= 0x0300;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x04, val);

        // Set P6 as RMU port (GB1: global control 2[13:12] = 0b11 )
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x1C, &val);
        val |= 0x0300;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x1C, val);

        // Set P6 as CPU destination port (GB1: Monitor control [7:4] = 0b0110 )
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x1A, &val);
        val |= 0x0060;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x1A, val);

        /* Only for 6321-1: Create Trg_dev_addr --->  Trg_dev_port mapping to route
         * the from_cpu management packets (GB2: Device mapping table register)
         * Trg_dev_addr (0x12) ---> Trg_dev_port (P4)
         *
         * */
        val = 0x9204
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1C, 0x06, val);
#endif  //endof #if MARVELL_MANAGEMENT_CONFIG_ENABLE == 1
#if MARVELL_SWITCH_VLAN_ENABLE == 1
        /****************  VLAN configuration ********************/
        if((board.project == E3550) || (board.project == P3479))
        {
            MVR_SWITCH_ALWAYS_PRINTF(("E3550 Enable VLAN tagging for 2nd Aurix\r\n"));
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3333); // P0-P3 are not memeber
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3223); // P5 and P6 are memeber (egress tagged)
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x10C8); // 200 VID and valid bit[12] =1
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x00C8); // FID = VID
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program VTU
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x08, 0x2480); // Enable 802.1Q on P5 (mode Fallback)
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x08, 0x2480); // Enable 802.1Q on P6 (mode Fallback)
        }
        else if(board.project == P3570)
        {

            PRINTF_BOOT(("P3570 Enable VLAN tagging for 2nd Aurix\r\n"));
            //for 2 aurix board VLAN programming for Marvell switch 6321-1:
            if(board.pcb_version >= PG2_A01){
                MVR_SWITCH_ALWAYS_PRINTF(("A01 P3570 Enable VLAN tagging for 2nd Aurix\r\n"));

#if EXPOSE_AURIX == 1
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3331); // P0 (egress untagged),P1-P3 not member
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3211); // P6 is member (egress tagged), P5 (egress untagged), P4 (egress untagged)
#else
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3333); // P0-P3 not member
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3213); // P6 is member (egress tagged), P5 (egress untagged), P4 not member4
#endif
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x10C8); // 200 VID and valid bit[12] =1
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x00C8); // FID = VID
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program VTU
#if EXPOSE_AURIX == 1
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x08, 0x2480); // Enable 802.1Q on P5 (mode Fallback) //allow both tagged and un-tagged
#else
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x08, 0x2880); // Enable 802.1Q on P5 (mode check) //only allow tagged packet
#endif
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x08, 0x2480); // Enable 802.1Q on P6 (mode Fallback) //allow both tagged and un-tagged
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x07, 0x10C8); // P5 : Set default VID = 200 and force defaultVID =1
            }else{
                MVR_SWITCH_ALWAYS_PRINTF(("A00 P3570 Enable VLAN tagging for 2nd Aurix\r\n"));
#if EXPOSE_AURIX == 1
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3231); //  P0 (egress untagged), P1, P3 not member, P2 is member (egress tagged)
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3311); // P6 not member, P5 (egress untagged), P4 (egress untagged)
#else
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3233); // P0, P1, P3 not member, P2 is member (egress tagged)
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3313); // P6 not member, P5 (egress untagged), P4 not member
#endif
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x10C8); // 200 VID and valid bit[12] =1
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x00C8); // FID = VID
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program VTU
#if EXPOSE_AURIX == 1
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x08, 0x2480); // Enable 802.1Q on P5 (mode Fallback) //allow both tagged and un-tagged
#else
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x08, 0x2880); // Enable 802.1Q on P5 (mode check) //only allow tagged packet
#endif
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x08, 0x2480); // Enable 802.1Q on P2 (mode Fallback) //allow both tagged and un-tagged
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x07, 0x10C8); // P5 : Set default VID = 200 and force defaultVID =1
            }
            PRINTF_BOOT(("P3570 Enable VLAN tagging for 2nd Aurix -2\r\n"));
        }

        // Ensure VTU not busy
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x05, &val);
        count = 0;
        while(val & 0x8000)
        {
            waitTime(TimeConst[TIMER_INDEX_1MS]);
            IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x05, &val);
            count++;
            if(count>100)
            {
                MVR_SWITCH_ALWAYS_PRINTF(("Waiting for 88E6321 VTU not busy fail\r\n"));
                break;
            }
        }
#endif // endof #if MARVELL_SWITCH_VLAN_ENABLE == 1
        /****************  Loop avoidance  ********************/
#if MARVELL_SWITCH_FORWARDING_CONTROL_ENABLE == 1
        // Disable packet forwarding between P2 and P6
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x12, 0x06, &val);
        val = val & ~(1<<6);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x06, val);

        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x16, 0x06, &val);
        val = val & ~(1<<2);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x06, val);

        // Disable packet forwarding between P3 and P4
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x13, 0x06, &val);
        val = val & ~(1<<4);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x06, val);

        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x06, &val);
        val = val & ~(1<<3);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x06, val);
#endif

        /****************  Load balancing  ********************/
        /****************  Trunk co-lerining ********************/

        /****************  Enable RGMII delay ********************/
        // Note: ForceSpd is set to 0b11 in previous version, we may need to set it if it's not 0b11
        // set port 5 RGMII TX/RX delay
        MVR_SWITCH_DEBUG_PRINTF(("Initial 88e6321_1 RX/TX delay on port 5 (CVM)\r\n"));
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x15, 0X1, &val);
        val |= 0xc000; // bit 15, bit 14 set to 1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0X1, val);

        /****************  Enable all ports ********************/
        // Enable all the ports except P6 and P3 (Disable one link of trunks)
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {

            if(board.pcb_version >= PG2_A01){
                if((devaddr == 0x12) || (devaddr == 0x13))
                    continue;
            }else{
                if((devaddr == 0x16) || (devaddr == 0x13))
                    continue;
            }

            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val |= 0x0003;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }

        /****************  Enable Internal PHY, Serdes ********************/
        enable_phy(slaveaddr);
        enable_serdes(slaveaddr);

        /****************  Set port 1 in SGMII Mode ********************/
        if((board.project == E3550) || (board.project == P3479))
        { /* In P3570, it's direct SGMII connection, no need to enable PHY detection */
            MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_1 port 1 in SGMII Mode.\r\n"));
            IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x11, 0X0, &val);
            val |= 0x1000; //set PHYDetect
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x11, 0x0, val);
        }
    }
    else if(slaveaddr == IFXETH_SWITCH_88E6321_2_ADDR)
    {
        MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_2\r\n"));
        /****************  Disable all ports ********************/
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val &= 0xFFFC;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }
        /* we don't enable it in bring up*/
#if MARVELL_MANAGEMENT_CONFIG_ENABLE == 1
        /****************  Multichip configuration ********************/
        // Set management link on P4 (port control[9:8] = 0b11 : EtherType DSA mode)
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x04, &val);
        val |= 0x0300;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x04, val);

        // Set P4 as RMU port (GB1: global control 2[13:12] = 0b01 )
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x1C, &val);
        val |= 0x1000;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x1C, val);

        // Set P4 as CPU destination port (GB1: Monitor control [7:4] = 0b0100 )
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x1A, &val);
        val |= 0x0040;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x1A, val);
#endif //endof #if MARVELL_MANAGEMENT_CONFIG_ENABLE == 1
#if MARVELL_SWITCH_VLAN_ENABLE == 1
        /****************  VLAN configuration ********************/
        // TBD (post bring-up)
#endif
#if MARVELL_SWITCH_FORWARDING_CONTROL_ENABLE == 1
        /****************  Loop avoidance  ********************/
        // Disable packet forwarding between P3 and P4
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x13, 0x06, &val);
        val = val & ~(1<<4);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x06, val);

        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x06, &val);
        val = val & ~(1<<3);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x06, val);
#endif
        /****************  Load balancing  ********************/
        /****************  Trunk co-lerining ********************/

        /****************  Enable all ports ********************/
        // Enable all the ports except P3 (Disable one link of trunk)
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            if(devaddr == 0x13)
                continue;
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val |= 0x0003;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }

        /****************  Enable Internal PHY, Serdes ********************/
        enable_phy(slaveaddr);
        enable_serdes(slaveaddr);
    }
#else /* NO CPU MODE, we will only disable the looping port, and enable RMII delay on 88E6321_1 port 5 CVM */
// NO CPU MODE
    if(slaveaddr == IFXETH_SWITCH_88E6321_1_ADDR)
    {
        MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_1: Disable port 2, port 3\r\n"));
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x12, 0x04, &val);
        val &= 0xFFFC;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x04, val);

        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x13, 0x04, &val);
        val &= 0xFFFC;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x04, val);

        MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_1 RX/TX delay on port 5 (CVM)\r\n"));
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x15, 0X1, &val);
        val |= 0xc000; // bit 15, bit 14 set to 1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0X1, val);

    }
    else if(slaveaddr == IFXETH_SWITCH_88E6321_2_ADDR)
    {
        MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_2: Disable port 3\r\n"));
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x13, 0x04, &val);
        val &= 0xFFFC;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x04, val);
    }
#endif //#if MARVELL_CPU_MODE_ENABLE == 1
}
#endif

/* TODO: create a set of initial function for different project, and maintain them according project macro
 *
 *  IfxEth_Marvell_Sw_init_ACH
 *  IfxEth_Marvell_Sw_init_DDPX_A01
 *  IfxEth_Marvell_Sw_init_DDPX_B0X
 *
 *  The process follows the init process in bug http://nvbugs/200355368
 *  */
static void IfxEth_Marvell_Sw_init_DDPX_P3479(uint32 slaveaddr)
{ /* Todo: we can add table to do it, and need to clean up to use macro
     but right now just go ahead for bring up */
    uint32 val, devaddr, count;
    static boolean is_cpu_mode_checked = FALSE;
    if(Check_88E6321_Device_ID(slaveaddr) == FALSE)
    {
        set_error_flag(INVAILD_88E6321_DEVICE_ERR);
        return;
    }
    if(!is_cpu_mode_checked)
    { /* For CPU mode, we only check once */
        is_cpu_mode_checked = TRUE;
        read_cpu_mode();
    }
    if(slaveaddr == IFXETH_SWITCH_88E6321_1_ADDR)
    {
        MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_1\r\n"));
        /****************  Disable all ports ********************/
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val &= 0xFFFC;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }

        /****************  Enable all ports ********************/
        // Enable all the ports except P3 and P4 (Not used)
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            if((devaddr == 0x13) || (devaddr == 0x14))
                continue;
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val |= 0x0003;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }
        enable_serdes(slaveaddr);
    }
    else if(slaveaddr == IFXETH_SWITCH_88E6321_2_ADDR)
    {
        MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_2\r\n"));
        enable_serdes(slaveaddr);
        /****************  Disable all ports ********************/
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val &= 0xFFFC;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }

        /****************  Enable all ports ********************/
        // Enable all the ports except P3 and P4 (Not used)
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            if((devaddr == 0x13) || (devaddr == 0x14))
                    continue;
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val |= 0x0003;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }
    }
}

#if 1
void IfxEth_Marvell_Sw_Init(uint32 slaveaddr)
{
#if PROJECT == PROJECT_PAGASUS2 && DRIVE_AX_B0x == 1
    if(board.project == P3479)
        IfxEth_Marvell_Sw_init_DDPX_P3479(slaveaddr);
    else
        IfxEth_Marvell_Sw_init_DDPX_B0X(slaveaddr);
#else
/* Other project, include ACH, DDPX A01 */
#if PROJECT == PROJECT_DRIVE_PX2
    //Enable VLAN settings
    IfxEth_Marvell_Sw_Vlan_Enable(slaveaddr);
#endif

    //Enable tag based VLAN
    IfxEth_Marvell_Sw_Tag_Vlan_Enable(slaveaddr);

    MVR_SWITCH_DEBUG_PRINTF(("%s\n", __func__));
    /* Enable TX&RX delay timing for Tegra PORT 2 */
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL, 0xc003);

#if BROAD_REACH_PHY_ENABLE == 1
    /* this process is only needed for BROAD Reach PHY that connected to its port */
    uint32 newdata, data;
    //configure port 2 (BroadR-R Reach port)
    IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL, &data);

    newdata = (data & (~IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_LINKVALUE)) | IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_FORCEDLINK;
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL, newdata);  //disable link first

    while (1)
    {
        IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS, &data);  //wait for status = link down
        if ((data & IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_LINK) == IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_LINK_DOWN)
        {
            break;
        }
    }

    data = newdata;
    newdata = (data & (~IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_FORCESPD)) |
                IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_FORCESPD_100_Mbps  |
                IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_RGMII_TX_TIMING    |
                IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_RGMII_RX_TIMING;
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL, newdata);  //force speed to 100Mbps and add delay to GTXCLK  see nvbugs 200196695
    while (1)
    {
        IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS, &data);  //wait for status = 100 Mbps
        if ((data & IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_SPEED) == IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_SPEED_100Mbps)
        {
            break;
        }
    }

    data = newdata;
    newdata = data | IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_LINKVALUE | IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL_BIT_FORCEDLINK;
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL, newdata);  //enable link

    while (1)
    {
        IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS, &data);  //wait for status = link up
        if ((data & IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_LINK) == IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_LINK_UP)
        {
            break;
        }
    }

    //Enable RGMII
    newdata = 0 | IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_C_MODE_RGMII;
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS, newdata);  //enable link
#endif

#endif // #if PROJECT == PROJECT_DRIVE_AX && DRIVE_AX_B0x == 1
}


void IfxEth_Marvell_Sw_init_only_vlan200(void)
{
    uint32 val, count,devaddr;
    uint32 slaveaddr = 0x10;
    /****************  Disable all ports ********************/
    for(devaddr=0x10; devaddr<=0x16; devaddr++)
    {
        IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
        val &= 0xFFFC;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
    }
    //
    /****************  Loop avoidance  ********************/
        // Disable packet forwarding between P2 and P6
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x12, 0x06, &val);
        val = val & ~(1<<6);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x06, val);
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x16, 0x06, &val);
        val = val & ~(1<<2);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x06, val);

        // Disable packet forwarding between P3 and P4
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x13, 0x06, &val);
        val = val & ~(1<<4);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x06, val);
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x06, &val);
        val = val & ~(1<<3);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x06, val);
    //
    IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3233); // P0-P3 not member
    IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3213); // P6 is member (egress tagged), P5 (egress untagged), P4 not member4
    IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x10C8); // 200 VID and valid bit[12] =1
    IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x00C8); // FID = VID
    IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program VTU
    IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x08, 0x2880); // Enable 802.1Q on P5 (mode check) //only allow tagged packet
    IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x08, 0x2480); // Enable 802.1Q on P6 (mode Fallback) //allow both tagged and un-tagged
    IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x08, 0x2480);
    IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x07, 0x10C8); // P5 : Set default VID = 200 and force defaultVID =1
    // Ensure VTU not busy
    IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x05, &val);
    count = 0;
    while(val & 0x8000)
    {
        waitTime(TimeConst[TIMER_INDEX_1MS]);
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x05, &val);
        count++;
        if(count>100)
        {
            MVR_SWITCH_ALWAYS_PRINTF(("Waiting for 88E6321 VTU not busy fail\r\n"));
            break;
        }
    }
    /****************  Enable RGMII delay ********************/
    // Note: ForceSpd is set to 0b11 in previous version, we may need to set it if it's not 0b11
    // set port 5 RGMII TX/RX delay
    //MVR_SWITCH_DEBUG_PRINTF(("Initial 88e6321_1 RX/TX delay on port 5 (CVM)\r\n"));
    IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x15, 0X1, &val);
    val |= 0xc000; // bit 15, bit 14 set to 1
    IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0X1, val);

    /****************  Enable all ports ********************/
    // Enable all the ports except P6 and P3 (Disable one link of trunks)
    for(devaddr=0x10; devaddr<=0x16; devaddr++)
    {
        //if((devaddr == 0x14) || (devaddr == 0x16))
        //          continue;
        IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
        val |= 0x0003;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
    }

    /****************  Enable Internal PHY, Serdes ********************/
    enable_phy(slaveaddr);
    enable_serdes(slaveaddr);

    /****************  Set port 1 in SGMII Mode ********************/
    //if((board.project == E3550) || (board.project == P3479))
    //{ /* In P3570, it's direct SGMII connection, no need to enable PHY detection */
    //MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_1 port 1 in SGMII Mode.\r\n"));
    IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x11, 0X0, &val);
    val |= 0x1000; //set PHYDetect
    IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x11, 0x0, val);
    /* Enable TX&RX delay timing for Tegra PORT 2  <--- be careful it is not for 88E6321_1*/
    //IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_PHYSICAL_CONTROL, 0xc003);
    MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_1 Finish vlan200.\r\n"));
}

void IfxEth_Marvell_Sw_ExposeAurix_Init(uint32 slaveaddr)
{
    uint32 val, devaddr, count;
    static boolean is_cpu_mode_checked = FALSE;
    if(Check_88E6321_Device_ID(slaveaddr) == FALSE)
    {
        set_error_flag(INVAILD_88E6321_DEVICE_ERR);
        return;
    }
    if(!is_cpu_mode_checked)
    { /* For CPU mode, we only check once */
        is_cpu_mode_checked = TRUE;
        read_cpu_mode();
    }
/* if CPU mode is enable, we need to initial everything by ourselves */
#if MARVELL_CPU_MODE_ENABLE == 1
    if(slaveaddr == IFXETH_SWITCH_88E6321_1_ADDR)
    {
        MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_1\r\n"));
        /****************  Disable all ports ********************/
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val &= 0xFFFC;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }
        /* we don't enable it in bring up*/
#if MARVELL_MANAGEMENT_CONFIG_ENABLE == 1
        /****************  Multichip configuration ********************/
        // Set management link on P4 and P6 (port control[9:8] = 0b11 : EtherType DSA mode)
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x04, &val);
        val |= 0x0300;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x04, val);

        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x16, 0x04, &val);
        val |= 0x0300;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x04, val);

        // Set P6 as RMU port (GB1: global control 2[13:12] = 0b11 )
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x1C, &val);
        val |= 0x0300;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x1C, val);

        // Set P6 as CPU destination port (GB1: Monitor control [7:4] = 0b0110 )
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x1A, &val);
        val |= 0x0060;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x1A, val);

        /* Only for 6321-1: Create Trg_dev_addr --->  Trg_dev_port mapping to route
         * the from_cpu management packets (GB2: Device mapping table register)
         * Trg_dev_addr (0x12) ---> Trg_dev_port (P4)
         *
         * */
        val = 0x9204
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1C, 0x06, val);
#endif  //endof #if MARVELL_MANAGEMENT_CONFIG_ENABLE == 1
#if MARVELL_SWITCH_VLAN_ENABLE == 1
        /****************  VLAN configuration ********************/
        if((board.project == E3550) || (board.project == P3479))
        {
            MVR_SWITCH_ALWAYS_PRINTF(("E3550 Enable VLAN tagging for 2nd Aurix\r\n"));
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3333); // P0-P3 are not memeber
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3223); // P5 and P6 are memeber (egress tagged)
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x10C8); // 200 VID and valid bit[12] =1
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x00C8); // FID = VID
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program VTU
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x08, 0x2480); // Enable 802.1Q on P5 (mode Fallback)
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x08, 0x2480); // Enable 802.1Q on P6 (mode Fallback)
        }
        else if(board.project == P3570)
        {

            PRINTF_BOOT(("P3570 Enable VLAN tagging for 2nd Aurix\r\n"));
            //for 2 aurix board VLAN programming for Marvell switch 6321-1:
            if(board.pcb_version >= PG2_A01){
                MVR_SWITCH_ALWAYS_PRINTF(("ExposeAurix A01 P3570 Enable VLAN tagging for 2nd Aurix\r\n"));
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3331); // P0 (egress untagged),P1-P3 not member
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3211); // P6 is member (egress tagged), P5 (egress untagged), P4 (egress untagged)
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x10C8); // 200 VID and valid bit[12] =1
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x00C8); // FID = VID
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program VTU
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x08, 0x2480); // Enable 802.1Q on P5 (mode Fallback) //allow both tagged and un-tagged
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x08, 0x2480); // Enable 802.1Q on P6 (mode Fallback) //allow both tagged and un-tagged
                IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x07, 0x10C8); // P5 : Set default VID = 200 and force defaultVID =1
            }else{
                MVR_SWITCH_ALWAYS_PRINTF(("ExposeAurix A00 P3570 Enable VLAN tagging for 2nd Aurix\r\n"));
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3231); //  P0 (egress untagged), P1, P3 not member, P2 is member (egress tagged)
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3311); // P6 not member, P5 (egress untagged), P4 (egress untagged)
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x10C8); // 200 VID and valid bit[12] =1
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x00C8); // FID = VID
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program VTU
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x08, 0x2480); // Enable 802.1Q on P5 (mode Fallback) //allow both tagged and un-tagged
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x08, 0x2480); // Enable 802.1Q on P2 (mode Fallback) //allow both tagged and un-tagged
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x07, 0x10C8); // P5 : Set default VID = 200 and force defaultVID =1
            }
            PRINTF_BOOT(("P3570 Enable VLAN tagging for 2nd Aurix -2\r\n"));
        }

        // Ensure VTU not busy
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x05, &val);
        count = 0;
        while(val & 0x8000)
        {
            waitTime(TimeConst[TIMER_INDEX_1MS]);
            IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x05, &val);
            count++;
            if(count>100)
            {
                MVR_SWITCH_ALWAYS_PRINTF(("Waiting for 88E6321 VTU not busy fail\r\n"));
                break;
            }
        }
#endif // endof #if MARVELL_SWITCH_VLAN_ENABLE == 1
        /****************  Loop avoidance  ********************/
#if MARVELL_SWITCH_FORWARDING_CONTROL_ENABLE == 1
        // Disable packet forwarding between P2 and P6
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x12, 0x06, &val);
        val = val & ~(1<<6);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x06, val);

        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x16, 0x06, &val);
        val = val & ~(1<<2);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x06, val);

        // Disable packet forwarding between P3 and P4
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x13, 0x06, &val);
        val = val & ~(1<<4);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x06, val);

        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x06, &val);
        val = val & ~(1<<3);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x06, val);
#endif

        /****************  Load balancing  ********************/
        /****************  Trunk co-lerining ********************/

        /****************  Enable RGMII delay ********************/
        // Note: ForceSpd is set to 0b11 in previous version, we may need to set it if it's not 0b11
        // set port 5 RGMII TX/RX delay
        MVR_SWITCH_DEBUG_PRINTF(("Initial 88e6321_1 RX/TX delay on port 5 (CVM)\r\n"));
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x15, 0X1, &val);
        val |= 0xc000; // bit 15, bit 14 set to 1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0X1, val);

        /****************  Enable all ports ********************/
        // Enable all the ports except P6 and P3 (Disable one link of trunks)
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {

            if(board.pcb_version >= PG2_A01){
                if((devaddr == 0x12) || (devaddr == 0x13))
                    continue;
            }else{
                if((devaddr == 0x16) || (devaddr == 0x13))
                    continue;
            }

            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val |= 0x0003;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }

        /****************  Enable Internal PHY, Serdes ********************/
        enable_phy(slaveaddr);
        enable_serdes(slaveaddr);

        /****************  Set port 1 in SGMII Mode ********************/
        if((board.project == E3550) || (board.project == P3479))
        { /* In P3570, it's direct SGMII connection, no need to enable PHY detection */
            MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_1 port 1 in SGMII Mode.\r\n"));
            IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x11, 0X0, &val);
            val |= 0x1000; //set PHYDetect
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x11, 0x0, val);
        }
    }
    else if(slaveaddr == IFXETH_SWITCH_88E6321_2_ADDR)
    {
        MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_2\r\n"));
        /****************  Disable all ports ********************/
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val &= 0xFFFC;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }
        /* we don't enable it in bring up*/
#if MARVELL_MANAGEMENT_CONFIG_ENABLE == 1
        /****************  Multichip configuration ********************/
        // Set management link on P4 (port control[9:8] = 0b11 : EtherType DSA mode)
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x04, &val);
        val |= 0x0300;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x04, val);

        // Set P4 as RMU port (GB1: global control 2[13:12] = 0b01 )
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x1C, &val);
        val |= 0x1000;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x1C, val);

        // Set P4 as CPU destination port (GB1: Monitor control [7:4] = 0b0100 )
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x1A, &val);
        val |= 0x0040;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x1A, val);
#endif //endof #if MARVELL_MANAGEMENT_CONFIG_ENABLE == 1
#if MARVELL_SWITCH_VLAN_ENABLE == 1
        /****************  VLAN configuration ********************/
        // TBD (post bring-up)
#endif
#if MARVELL_SWITCH_FORWARDING_CONTROL_ENABLE == 1
        /****************  Loop avoidance  ********************/
        // Disable packet forwarding between P3 and P4
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x13, 0x06, &val);
        val = val & ~(1<<4);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x06, val);

        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x06, &val);
        val = val & ~(1<<3);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x06, val);
#endif
        /****************  Load balancing  ********************/
        /****************  Trunk co-lerining ********************/

        /****************  Enable all ports ********************/
        // Enable all the ports except P3 (Disable one link of trunk)
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            if(devaddr == 0x13)
                continue;
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val |= 0x0003;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }

        /****************  Enable Internal PHY, Serdes ********************/
        enable_phy(slaveaddr);
        enable_serdes(slaveaddr);
    }
#else /* NO CPU MODE, we will only disable the looping port, and enable RMII delay on 88E6321_1 port 5 CVM */
// NO CPU MODE
    if(slaveaddr == IFXETH_SWITCH_88E6321_1_ADDR)
    {
        MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_1: Disable port 2, port 3\r\n"));
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x12, 0x04, &val);
        val &= 0xFFFC;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x04, val);

        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x13, 0x04, &val);
        val &= 0xFFFC;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x04, val);

        MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_1 RX/TX delay on port 5 (CVM)\r\n"));
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x15, 0X1, &val);
        val |= 0xc000; // bit 15, bit 14 set to 1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0X1, val);

    }
    else if(slaveaddr == IFXETH_SWITCH_88E6321_2_ADDR)
    {
        MVR_SWITCH_ALWAYS_PRINTF(("Initial 88e6321_2: Disable port 3\r\n"));
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x13, 0x04, &val);
        val &= 0xFFFC;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x04, val);
    }
#endif //#if MARVELL_CPU_MODE_ENABLE == 1
}
void IfxEth_Marvell_Sw_Loadsim_Init(uint32 slaveaddr)
{
    uint32 val, devaddr, count;
    if(slaveaddr == IFXETH_SWITCH_88E6321_1_ADDR)
    {
        /****************  Disable all ports ********************/
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val &= 0xFFFC;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }
        //VLAN11, P2,P3 is member
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x1133); // P2,P3 is member
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3333);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x100B); // 11 VID and valid bit[12] =1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x000B); // FID = VID
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program VTU
        //IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x08, 0x2480); // Enable 802.1Q on P2 (mode Fallback) //allow both tagged and un-tagged
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x08, 0x2480); // Enable 802.1Q on P3 (mode Fallback) //allow both tagged and un-tagged
        //IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x08, 0x2480); // Enable 802.1Q on P4 (mode Fallback) //allow both tagged and un-tagged
        //IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x07, 0x100B); // P2 : Set default VID = 11 and force defaultVID =1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x07, 0x100B); // P3 : Set default VID = 11 and force defaultVID =1
        //IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x07, 0x100B); // P4 : Set default VID = 11 and force defaultVID =1
        // packet forwarding P5 : P2 and P6
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x15, 0x06, &val);
        val = 0x44;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x06, val);
        // packet forwarding P2 : P3 and P5
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x12, 0x06, &val);
        val = 0x28;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x06, val);
        // packet forwarding P3 : P2
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x13, 0x06, &val);
        val = 4;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x06, val);
        // Ensure VTU not busy
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x05, &val);
        count = 0;
        while(val & 0x8000)
        {
            waitTime(TimeConst[TIMER_INDEX_1MS]);
            IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x05, &val);
            count++;
            if(count>100)
            {
                MVR_SWITCH_ALWAYS_PRINTF(("Waiting for 88E6321 VTU not busy fail\r\n"));
                break;
            }
        }
        //
        //VLAN12, P0,P4,P6 is member
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3311);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3131); // P4,P6 member
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x100C); // 12 VID and valid bit[12] =1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x000C); // FID = VID
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program VTU
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x08, 0x2480); // Enable 802.1Q on P4 (mode Fallback) //allow both tagged and un-tagged
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x08, 0x2480); // Enable 802.1Q on P6 (mode Fallback) //allow both tagged and un-tagged
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x08, 0x2480); // Enable 802.1Q on P0 (mode Fallback) //allow both tagged and un-tagged
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x11, 0x08, 0x2480); // Enable 802.1Q on P0 (mode Fallback) //allow both tagged and un-tagged
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x07, 0x100C); // P4 : Set default VID = 12 and force defaultVID =1
        //IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x07, 0x100C); // P6 : Set default VID = 12 and force defaultVID =1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x07, 0x100C); // P0 : Set default VID = 12 and force defaultVID =1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x11, 0x07, 0x100C);
        // packet forwarding P4 : P6
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x06, &val);
        val = 0x40;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x06, val);
        // packet forwarding P6 : P5,P4,P0
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x16, 0x06, &val);
        val = 0x31;//0x11;//0x10;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x06, val);
        // packet forwarding P0 : P1,P6
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x10, 0x06, &val);
        val = 0x42;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x06, val);
        // packet forwarding P1 : P0
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x11, 0x06, &val);
        val = 0x1;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x11, 0x06, val);
        // Ensure VTU not busy
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x05, &val);
        count = 0;
        while(val & 0x8000)
        {
            waitTime(TimeConst[TIMER_INDEX_1MS]);
            IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x05, &val);
            count++;
            if(count>100)
            {
                MVR_SWITCH_ALWAYS_PRINTF(("Waiting for 88E6321 VTU not busy fail\r\n"));
                break;
            }
        }
        /****************  Enable all ports ********************/
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val |= 0x0003;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }
    }else if(slaveaddr == IFXETH_SWITCH_88E6321_2_ADDR)
    {
        /****************  Disable all ports ********************/
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val &= 0xFFFC;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }
        /**************** 88E6321_2 Loop avoidance  ********************/
        // Disable packet forwarding between P3 and P4
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x13, 0x06, &val);
        val = val & ~(1<<4);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x06, val);
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x06, &val);
        val = val & ~(1<<3);
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x06, val);
        //
        /**************** Set VLAN ****************************/
        //MVR_SWITCH_ALWAYS_PRINTF(("88E6321_2 vlan test\r\n"));
        //VLAN12, P1,P4 is member
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3313); // P1 is member
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3331); // P4 is member
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x100C); // 12 VID and valid bit[12] =1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x000C); // FID = VID
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program VTU
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x11, 0x08, 0x2480); // Enable 802.1Q on P1 (mode Fallback) //allow both tagged and un-tagged
        //IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x08, 0x2480); // Enable 802.1Q on P4 (mode Fallback) //allow both tagged and un-tagged
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x08, 0x2480); // Enable 802.1Q on P0 (mode Fallback) //allow both tagged and un-tagged
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x11, 0x07, 0x100C); // P1 : Set default VID = 12 and force defaultVID =1
        //IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x07, 0x100C); // P4 : Set default VID = 12 and force defaultVID =1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x07, 0x100C); // P0 : Set default VID = 12 and force defaultVID =1
        //VLAN11, P2,P6 is member
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3133); // P2 is member
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3133); // P6 is member
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x100B); // 11 VID and valid bit[12] =1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x000B); // FID = VID
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program VTU
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x08, 0x2480); // Enable 802.1Q on P2 (mode Fallback) //allow both tagged and un-tagged
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x08, 0x2480); // Enable 802.1Q on P6 (mode Fallback) //allow both tagged and un-tagged
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x07, 0x100B); // P2 : Set default VID = 11 and force defaultVID =1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x07, 0x100B); // P6 : Set default VID = 11 and force defaultVID =1
        //Here we wish to use P0,P3,P5 is member for red path
        //VLAN13, P3,P5 is member
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x1331); // P3 is member
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3313); // P5 is member
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x100D); // 13 VID and valid bit[12] =1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x000D); // FID = VID
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program VTU
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x08, 0x2480); // Enable 802.1Q on P3 (mode Fallback) //allow both tagged and un-tagged
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x08, 0x2480); // Enable 802.1Q on P5 (mode Fallback) //allow both tagged and un-tagged
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x08, 0x2480); // Enable 802.1Q on P0 (mode Fallback) //allow both tagged and un-tagged
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x07, 0x100D); // P3 : Set default VID = 13 and force defaultVID =1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x07, 0x100D); // P5 : Set default VID = 13 and force defaultVID =1
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x07, 0x100D); // P0 : Set default VID = 13 and force defaultVID =1
        // for red path packet forwarding between P0 : P3
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x10, 0x06, &val);
        val = 0x8;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x06, val);
        // packet forwarding P1: P4
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x11, 0x06, &val);
        val = 0x10;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x11, 0x06, val);
        // packet forwarding P4: P1
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x06, &val);
        val = 2;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x06, val);
        // packet forwarding P2: P6
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x12, 0x06, &val);
        val = 0x40;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x06, val);
        // packet forwarding P6: P2
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x16, 0x06, &val);
        val = 0x4;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x16, 0x06, val);
        // packet forwarding P5: P3
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x15, 0x06, &val);
        val = 0x8;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x15, 0x06, val);
        // packet forwarding P3: P5, for red path we add P0
        IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x13, 0x06, &val);
        val = 0x21;
        IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x06, val);
        //
        /****************  Enable all ports ********************/
        for(devaddr=0x10; devaddr<=0x16; devaddr++)
        {
            IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
            val |= 0x0003;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
        }
        enable_phy(slaveaddr);
        enable_serdes(slaveaddr);
    }
}
#endif

void IfxEth_Marvell_Sw_Set(uint32 slaveaddr, uint32 port, uint32 speed)
{
    uint32 val;
    if(speed == 1000)
    { /* 1G */
        if((slaveaddr == IFXETH_SWITCH_88E6321_1_ADDR) ||
               (slaveaddr == IFXETH_SWITCH_88E6321_2_ADDR))
        {
            if((port==0) || (port == 1))
            {
                IfxEth_Marvell_Switch_read_reg(slaveaddr, port+0x10, 0X0, &val);
                val |= 0x1000; //set PHYDetect
                IfxEth_Marvell_Switch_write_reg(slaveaddr, port+0x10, 0x0, val);

                /* Forse Serdes to work at 1G */
                IfxEth_Sw_88E88E6321_write_phy_reg(slaveaddr, 0xc + port, 0, 0x0140);
            }
            /* force link down */
            IfxEth_Marvell_Switch_write_reg(slaveaddr, port+0x10, 0x1, 0x0012);
            /* force link up, 1G */
            IfxEth_Marvell_Switch_write_reg(slaveaddr, port+0x10, 0x1, 0x003E);

        }
        else if(slaveaddr == IFXETH_SWITCH_88Q5050_ADDR)
        {
            IfxEth_Marvell_Switch_read_reg(slaveaddr, port, 0x0, &val);
            val |= 0x1000; //set PHYDetect
            IfxEth_Marvell_Switch_write_reg(slaveaddr, port, 0x0, val);

            /* force link down */
            IfxEth_Marvell_Switch_write_reg(slaveaddr, port, 0x1, 0x2012);
            /* force link up, 100M */
            IfxEth_Marvell_Switch_write_reg(slaveaddr, port, 0x1, 0x203E);
        }
    }
    else
    { /* 100M */
        if((slaveaddr == IFXETH_SWITCH_88E6321_1_ADDR) ||
           (slaveaddr == IFXETH_SWITCH_88E6321_2_ADDR))
        {
            if((port==0) || (port == 1))
            {
                IfxEth_Marvell_Switch_read_reg(slaveaddr, port+0x10, 0X0, &val);
                val |= 0x1000; //set PHYDetect
                IfxEth_Marvell_Switch_write_reg(slaveaddr, port+0x10, 0x0, val);

                /* Forse Serdes to work at 100M */
                IfxEth_Sw_88E88E6321_write_phy_reg(slaveaddr, 0xc + port, 0, 0x2100);
            }
            /* force link down */
            IfxEth_Marvell_Switch_write_reg(slaveaddr, port+0x10, 0x1, 0x0011);
            /* force link up, 100M */
            IfxEth_Marvell_Switch_write_reg(slaveaddr, port+0x10, 0x1, 0x003D);
        }
        else if(slaveaddr == IFXETH_SWITCH_88Q5050_ADDR)
        {
            //IfxEth_Marvell_Switch_read_reg(slaveaddr, port, 0x0, &val);
            //val |= 0x1000; //set PHYDetect
            //IfxEth_Marvell_Switch_write_reg(slaveaddr, port, 0x0, val);


            /* force link down */
            IfxEth_Marvell_Switch_write_reg(slaveaddr, port, 0x1, 0x2011);
            /* force link up, 100M */
            IfxEth_Marvell_Switch_write_reg(slaveaddr, port, 0x1, 0x203D);
        }
    }
}

/* force all the 100M ports (internal PHY inside 88Q5050) to Master here */
void IfxEth_Marvell_Hyperion_ExtraInit(void)
{
    uint32 i;
    for(i=1; i<6; i++)
    { /* For port 1 to port 5, we set them to master role */
        IfxEth_Sw_88E88E6321_write_phy_reg(0x11, i, 0xD, 0x0001);
        IfxEth_Sw_88E88E6321_write_phy_reg(0x11, i, 0xE, 0x0834);
        IfxEth_Sw_88E88E6321_write_phy_reg(0x11, i, 0xD, 0x4001);
        IfxEth_Sw_88E88E6321_write_phy_reg(0x11, i, 0xE, 0xC000);
    }
    MVR_SWITCH_ALWAYS_PRINTF(("Config switch to use VLAN to reduce interference\r\n"));
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1, 0x6, 0x0081);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x2, 0x6, 0x0081);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x3, 0x6, 0x0081);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x4, 0x6, 0x0081);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x5, 0x6, 0x0081);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x6, 0x6, 0x0081);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x8, 0x6, 0x0081);
    IfxEth_Marvell_Switch_write_reg(0x12, 0x10, 0x6, 0x0018);
    IfxEth_Marvell_Switch_write_reg(0x12, 0x11, 0x6, 0x0018);
    IfxEth_Marvell_Switch_write_reg(0x12, 0x12, 0x6, 0x0018);
    IfxEth_Marvell_Switch_write_reg(0x12, 0x15, 0x6, 0x0018);
    IfxEth_Marvell_Switch_write_reg(0x12, 0x16, 0x6, 0x0018);
    IfxEth_Marvell_Switch_write_reg(0x10, 0x10, 0x6, 0x66);
    IfxEth_Marvell_Switch_write_reg(0x10, 0x13, 0x6, 0x66);
    IfxEth_Marvell_Switch_write_reg(0x10, 0x14, 0x6, 0x66);
}

#define MAX_PORT  7

static void Set_Vlan(uint32 slaveaddr, uint8 sourceport, uint8 destport_bitmask)
{
    uint32 layeraddr;
    uint32 regdata;

    if(sourceport >= MAX_PORT)
    {
        return;
    }

    layeraddr = IFXETH_SW_88E88E6321_PORT_0_SMI_DEVICE_ADDR + sourceport;
    IfxEth_Marvell_Switch_read_reg(slaveaddr, layeraddr, IFXETH_SW_88E88E6321_PER_PORT_PORT_BASE_VLAN_MAP, &regdata);
    MVR_SWITCH_DEBUG_PRINTF(("Read data 0x%x from address 0x%x\n", regdata, layeraddr));

    regdata &= (~0x7f);
    regdata |= destport_bitmask;

    IfxEth_Marvell_Switch_write_reg(slaveaddr, layeraddr, IFXETH_SW_88E88E6321_PER_PORT_PORT_BASE_VLAN_MAP, regdata);
    MVR_SWITCH_DEBUG_PRINTF(("Write data 0x%x to address 0x%x\n", regdata, layeraddr));
}

void IfxEth_Marvell_Sw_Vlan_Enable(uint32 slaveaddr)
{
    Set_Vlan(slaveaddr, 0, 0x7E); //TA
    Set_Vlan(slaveaddr, 1, 0x7D); //TB
    Set_Vlan(slaveaddr, 2, 0x23); //Broad-R
    Set_Vlan(slaveaddr, 3, 0x03); //Harness
    Set_Vlan(slaveaddr, 4, 0x03); //Debug
    Set_Vlan(slaveaddr, 5, 0x07); //Aurix
}

void IfxEth_Marvell_Sw_Vlan_Disable(uint32 slaveaddr)
{
    Set_Vlan(slaveaddr, 0, 0x7E); //TA
    Set_Vlan(slaveaddr, 1, 0x7D); //TB
    Set_Vlan(slaveaddr, 2, 0x7B); //Broad-R
    Set_Vlan(slaveaddr, 3, 0x77); //Harness
    Set_Vlan(slaveaddr, 4, 0x6F); //Debug
    Set_Vlan(slaveaddr, 5, 0x5F); //Aurix
}

void IfxEth_Marvell_Sw_Tag_Vlan_Enable(uint32 slaveaddr)
{
    uint16 regdata;
#if PROJECT == PROJECT_DRIVE_PX2
    //program Membertag P0[1:0] P1[5:4] P2[9:8] P5[13:12] across reg 0x7 and 0x8
    IfxEth_write_mdio_reg(IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_STU_DATA_0_3, 0x3122); //P0 and P1 member egress tagged
    IfxEth_write_mdio_reg(IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_STU_DATA_4_6, 0x0313); //P2 and P5 untagged
    IfxEth_write_mdio_reg(IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_VID, 0x10C8); // VID = 200 and valid bit[12] =1
    IfxEth_write_mdio_reg(IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_FID, 0x00C8); // FID = VID

    // program VTU
    IfxEth_write_mdio_reg(IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_OPERATION, 0xB000); // opcode = Load and VTBusy =1

    // enable 802.1Q on P0 , P1 and P5
    IfxEth_write_mdio_reg(IFXETH_SW_88E88E6321_PORT_0_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_CONTROL_2, 0x2480); // P0: allow both (tagged and untagged)
    IfxEth_write_mdio_reg(IFXETH_SW_88E88E6321_PORT_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_CONTROL_2, 0x2480); // P1 :allow both (tagged and untagged)
    IfxEth_write_mdio_reg(IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_CONTROL_2, 0x2880); // P2 :only allow tagged pkt with VID 200
    IfxEth_write_mdio_reg(IFXETH_SW_88E88E6321_PORT_5_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_CONTROL_2, 0x2880); // P5 :only allow tagged pkt with VID 200
    IfxEth_write_mdio_reg(IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_DEFAULT_VID_PRI, 0x10C8); // P2 :set defaultVID = 200 and force defaultVID = 1
    IfxEth_write_mdio_reg(IFXETH_SW_88E88E6321_PORT_5_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_DEFAULT_VID_PRI, 0x10C8); // P5 :set defaultVID = 200 and force defaultVID = 1

    // wait for VTU busy bit to be clear
    IfxEth_read_mdio_reg(IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_OPERATION, &regdata);

    while (regdata & IFXETH_SW_88E88E6321_GLOBAL_1_VTU_OPERATION_COMMAND_BIT_VTUBUSY)
    {
        IfxEth_read_mdio_reg(IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_OPERATION, &regdata);
    }
#elif PROJECT == PROJECT_PAGASUS2 && PG2_PREEVT == 1

    //program Membertag P0[1:0] P1[5:4] P2[9:8] P4[13:12] and so on across reg 0x7 and 0x8
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_STU_DATA_0_3, 0x3233); //P2 member egress tagged
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_STU_DATA_4_6, 0x0313); //P5 (Aurix) untagged

    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_VID, 0x10C8); // VID = 200 and valid bit[12] =1
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_FID, 0x00C8); // FID = VID
    // program VTU
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_OPERATION, 0xB000); // opcode = Load and VTBusy =1

    // enable 802.1Q on P2 and P5
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_2_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_CONTROL_2, 0x2480); // P2 :allow both (tagged and untagged)

    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_5_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_CONTROL_2, 0x2880); // P5 :only allow tagged pkt with VID 200
    IfxEth_Marvell_Switch_write_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_5_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_DEFAULT_VID_PRI, 0x10C8); // P5 :set defaultVID = 200 and force defaultVID = 1

    // wait for VTU busy bit to be clear
    IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_OPERATION, &regdata);

    while (regdata & IFXETH_SW_88E88E6321_GLOBAL_1_VTU_OPERATION_COMMAND_BIT_VTUBUSY)
    {
          IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_GLOBAL_1_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_GLOBAL_1_VTU_OPERATION, &regdata);
    }
#endif
}

boolean IfxEth_Marvell_Sw_Is_Link(uint32 slaveaddr)
{
#if MARVELL_SWITCH_DEBUG_ENABLE == 1
    return TRUE;
#else
#if (PROJECT == PROJECT_PAGASUS2) && (DRIVE_AX_B0x == 1)
    //TODO: we need to read port status in SJA1105 in DDPX B00, return TRUE for now
    return TRUE;
#else
    uint32 value;
    boolean linkEstablished = FALSE;
    IfxEth_Marvell_Switch_read_reg(slaveaddr, IFXETH_SW_88E88E6321_PORT_5_SMI_DEVICE_ADDR, IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS, &value);  //check if link is up
    linkEstablished = ((value & IFXETH_SW_88E88E6321_PER_PORT_PORT_STATUS_BIT_LINK) !=0) ? TRUE : FALSE;
    return linkEstablished;
#endif
#endif
}

#endif //endof #if ETH_SWITCH_ENABLE == 1



