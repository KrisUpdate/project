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

#include "Phy_88q2112.h"
#ifdef TC397B
#include <Geth/Eth_Devices/IfxEth_Devices.h>
#else
#include <Eth/Eth_Devices/IfxEth_Devices.h>
#endif
#include <internal_fw_debug.h>
#include <Configuration.h>
#include "board.h"
#include "gpioexp.h"
#include "Phy_88q2112_API.h"
#include "string.h"
#include "config_pg2_feasures.h"
#include "i2crom_drv.h"

#if ETH_PHY_88Q2112_ENABLE == 1


extern boolean boot_msg_flag;
#define Q2112_PRINTF_ALWAYS(x)        DEBUG_PRINTF_ALWAYS(x)
#define PRINTF_BOOT(x)       ({if(boot_msg_flag){DEBUG_PRINTF_ALWAYS(x);}})


/******************************************************************************/
/*-----------------------Exported Variables/Constants-------------------------*/
/******************************************************************************/

static boolean Q2112_IsPhyInitDone[PHY_88Q2112_MAX_NUMBER] = {FALSE};

static const uint32 Q2112_SLAVE_ADDRESS_P3479_A01[] =
{
    IFXETH_PHY_88Q2112_1_ADDR_P3479,
    IFXETH_PHY_88Q2112_2_ADDR_P3479,
    IFXETH_PHY_88Q2112_3_ADDR_P3479,
    IFXETH_PHY_88Q2112_4_ADDR_P3479,
    IFXETH_PHY_88Q2112_5_ADDR_P3479,
    IFXETH_PHY_88Q2112_6_ADDR_P3479,
    IFXETH_PHY_88Q2112_7_ADDR_P3479,
    IFXETH_PHY_88Q2112_8_ADDR_P3479
};

static const uint32 Q2112_SLAVE_ADDRESS_P3479_A03[] =
{
    IFXETH_PHY_88Q2112_1_ADDR_P3479,
    IFXETH_PHY_88Q2112_2_ADDR_P3479,
    IFXETH_PHY_88Q2112_3_ADDR_P3479,
    IFXETH_PHY_88Q2112_4_ADDR_P3479,
    IFXETH_PHY_88Q2112_6_ADDR_P3479,
    IFXETH_PHY_88Q2112_7_ADDR_P3479,
    IFXETH_PHY_88Q2112_8_ADDR_P3479
};

static const uint32 Q2112_SLAVE_ADDRESS_E3550[] =
{
    IFXETH_PHY_88Q2112_1_ADDR_E3550,
    IFXETH_PHY_88Q2112_2_ADDR_E3550,
    IFXETH_PHY_88Q2112_3_ADDR_E3550,
    IFXETH_PHY_88Q2112_4_ADDR_E3550,
    IFXETH_PHY_88Q2112_5_ADDR_E3550,
    IFXETH_PHY_88Q2112_6_ADDR_E3550,
    IFXETH_PHY_88Q2112_7_ADDR_E3550
};

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
#if 1
static uint32* get_slave_address_pointer(void)
{
    if(board.project == P3479)
    {
        if ((board.pcb_spin == 'A') && (board.pcb_version >= 3))
        { // A03 and onwards
            return Q2112_SLAVE_ADDRESS_P3479_A03;
        }
        else
        { // A00, A01
            return Q2112_SLAVE_ADDRESS_P3479_A01;
        }
    }
    else
    {
        return Q2112_SLAVE_ADDRESS_E3550;
    }
}


static boolean is_phy_id_valid(uint32 phyid)
{
    if(phyid > get_88q2112_number())
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
#endif


static boolean is_z1_silicon(uint32 slaveaddr)
{
    uint32 value;
    IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, 3, 0x3, &value);
    if(value == 0x0980)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


//static void IfxEth_Phy_88q2112_do_init(uint32 slaveaddr)
static void IfxEth_Phy_88q2112_do_init(uint32 slaveaddr, uint8 mode_sel, uint8 ms_sel)
{
    uint32 value = 0;
    boolean is_q2112_z1_version = FALSE;
    
//amy _ edit

        PRINTF_BOOT(("IfxEth_Phy_88q2112_do_init 1\r\n"));


#ifdef TC397B
    IFXGETH_PHY_WAIT_GMII_READY();
#else
    IFXETH_PHY_WAIT_GMII_READY();
#endif
    //Detect silicon version

//amy _ edit

           PRINTF_BOOT(("IfxEth_Phy_88q2112_do_init 2\r\n"));

    uint32 regData = getRevNum(slaveaddr);
    char version[10];
    switch(regData)
    {
    case MRVL_88Q2112_A2:
        strcpy(version, "A2");
		//DEBUG_PRINTF_ALWAYS(("PHY 2112 A2 \r\n"));
        break;
    case MRVL_88Q2112_A1:
        strcpy(version, "A1");
        break;
    case MRVL_88Q2112_A0:
        strcpy(version, "A0");
        break;
    case MRVL_88Q2112_Z1:
        strcpy(version, "Z1");
        break;
    default:
        strcpy(version, "UnKnown");
        break;
    }
    
PRINTF_BOOT(("IfxEth_Phy_88q2112_do_init 3\r\n"));
        
    if(board.project == P3479)
    {
//      Q2112_PRINTF_ALWAYS(("Initial 88Q2112 %s silicon slave address %d at group %d, API %s\r\n",version, GET_MDIO_SLAVE_ADDRESS(slaveaddr), GET_GROUP(slaveaddr), get_api_version()));
   }
   else
   {
//     Q2112_PRINTF_ALWAYS(("Initial 88Q2112 %s silicon slave address %d, API %s\r\n", version, slaveaddr, get_api_version()));
   }
  
PRINTF_BOOT(("IfxEth_Phy_88q2112_do_init 4\r\n"));
           
    IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_DEV_1_ADDR,
          2, &value);
    if(value != 0x2b)
    {
       Q2112_PRINTF_ALWAYS(("Error: Unexpected 88Q2112 Device ID Found!!\r\n"));
    }
PRINTF_BOOT(("IfxEth_Phy_88q2112_do_init 5\r\n"));
        

    
    /* Initial Q2112 as slave, 1G speed */
    IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_DEV_1_ADDR,
           IFXETH_PHY_88Q2112_PMA_PMD_CTRL_REG, &value);

    if(ms_sel == INFOROM_ADDR_TBL_ETH_PHY_MASTER)
    {
        // set bit 14 to 1, config PHY as master
        value |= IFXETH_PHY_88Q2112_MASTER_SLAVE_BIT_MASK;
    }
    else  // Be slave at most condition
    {
        // set bit 14 to 0, config PHY as slave
        value &= (~IFXETH_PHY_88Q2112_MASTER_SLAVE_BIT_MASK);
    }

    // set bit 0 to 1, config PHY on 1G
    value |= IFXETH_PHY_88Q2112_SPEED_BIT_MASK;
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_DEV_1_ADDR,
           IFXETH_PHY_88Q2112_PMA_PMD_CTRL_REG, value);
    
PRINTF_BOOT(("IfxEth_Phy_88q2112_do_init 6\r\n"));
        
    /* write the init sequence from Marvell */
    if(is_q2112_z1_version)
    {
       initQ2112Z1Ge(slaveaddr);
    }
    else
    {
#if 0
        // 1.34.10 patch 10
        // init sequence already contain reset for A0 silicon
        initQ2112Ge(slaveaddr, mode_sel);
#else
        // porting for B2B board's PHY
        if (slaveaddr == 0){
            PRINTF_BOOT(("IfxEth_Phy_88q2112_do_init : initQ2112Ge_B2B\r\n"));
            // init sequence already contain reset for A0 silicon
            initQ2112Ge_B2B(0, mode_sel);
        }else{
            // init sequence already contain reset for A0 silicon
            initQ2112Ge(slaveaddr, mode_sel);
        }
#endif
    }

    /* if we need to access the 88Q2112 after init,
     * we need to add 500ms delay to wait for the chip to be ready
     */
}



// Read Legacy/Compliance configuration from InfoROM (Q2112)
// Q2112 phyid= 1 ~ 7
uint8 Chk_InfoROM_88q2112_Mode_by_PHYID(uint8 phyid)
{
    uint8 bytesleft;
    T_ROM_ETHPHY_MODE_MS_DATA ethphy_mode_ms;

    if((phyid < IFXETH_PHY_88Q2112_1_PHYID) || (phyid > IFXETH_PHY_88Q2112_7_PHYID))
        bytesleft= INFOROM_ADDR_TBL_ETH_PHY_MODE_LEGACY;
    else
    {
        bytesleft= EthPHYModeMS_ReadValue(&ethphy_mode_ms.uB[0]);   // read 3 bytes data from I2C0 InfoROM
        if(!bytesleft)
        {
            //successful
            if(phyid == IFXETH_PHY_88Q2112_1_PHYID)      //I15, U178
                bytesleft= ethphy_mode_ms.detail.J12_2_Mode;
            else if(phyid == IFXETH_PHY_88Q2112_2_PHYID) //I30, U181
                bytesleft= ethphy_mode_ms.detail.J12_1_Mode;
            else if(phyid == IFXETH_PHY_88Q2112_3_PHYID) //I13, U183
                bytesleft= ethphy_mode_ms.detail.J12_4_Mode;
            else if(phyid == IFXETH_PHY_88Q2112_4_PHYID) //I19, U185
                bytesleft= ethphy_mode_ms.detail.J11_3_Mode;
            else if(phyid == IFXETH_PHY_88Q2112_5_PHYID) //I37, U160
                bytesleft= ethphy_mode_ms.detail.J11_2_Mode;
            else if(phyid == IFXETH_PHY_88Q2112_6_PHYID) //I38, U162
                bytesleft= ethphy_mode_ms.detail.J11_4_Mode;
            else if(phyid == IFXETH_PHY_88Q2112_7_PHYID) //I11, U176
                bytesleft= ethphy_mode_ms.detail.J12_3_Mode;
        }
        else
        {
            //failed
            bytesleft= INFOROM_ADDR_TBL_ETH_PHY_MODE_LEGACY;
        }
    }

    return bytesleft;
}

// Read Master/Slave configuration from InfoROM (Q2112)
// Q2112 phyid= 1 ~ 7
uint8 Chk_InfoROM_88q2112_MS_by_PHYID(uint8 phyid)
{
    uint8 bytesleft;
    T_ROM_ETHPHY_MODE_MS_DATA ethphy_mode_ms;

    if((phyid < IFXETH_PHY_88Q2112_1_PHYID) || (phyid > IFXETH_PHY_88Q2112_7_PHYID))
        bytesleft= INFOROM_ADDR_TBL_ETH_PHY_SLAVE;
    else
    {
        bytesleft= EthPHYModeMS_ReadValue(&ethphy_mode_ms.uB[0]);   // read 3 bytes data from I2C0 InfoROM
        if(!bytesleft)
        {
            //successful
            if(phyid == IFXETH_PHY_88Q2112_1_PHYID)      //I15, U178
                bytesleft= ethphy_mode_ms.detail.J12_2_M_S;
            else if(phyid == IFXETH_PHY_88Q2112_2_PHYID) //I30, U181
                bytesleft= ethphy_mode_ms.detail.J12_1_M_S;
            else if(phyid == IFXETH_PHY_88Q2112_3_PHYID) //I13, U183
                bytesleft= ethphy_mode_ms.detail.J12_4_M_S;
            else if(phyid == IFXETH_PHY_88Q2112_4_PHYID) //I19, U185
                bytesleft= ethphy_mode_ms.detail.J11_3_M_S;
            else if(phyid == IFXETH_PHY_88Q2112_5_PHYID) //I37, U160
                bytesleft= ethphy_mode_ms.detail.J11_2_M_S;
            else if(phyid == IFXETH_PHY_88Q2112_6_PHYID) //I38, U162
                bytesleft= ethphy_mode_ms.detail.J11_4_M_S;
            else if(phyid == IFXETH_PHY_88Q2112_7_PHYID) //I11, U176
                bytesleft= ethphy_mode_ms.detail.J12_3_M_S;
        }
        else
        {
            //failed
            bytesleft= INFOROM_ADDR_TBL_ETH_PHY_SLAVE;
        }
    }

    return bytesleft;
}


void IfxEth_Phy_88q2112_read_mdio_reg(uint32 slaveaddr, uint32 devaddr, uint32 regaddr, uint32 *pdata)
{
    uint32 regdata = devaddr;
    uint32 value;
    IfxEth_write_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_REG_13_14_DEVICE_ADDR, IFXETH_PHY_88Q2112_REG_13_ADDR, regdata);

    regdata = regaddr;
    IfxEth_write_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_REG_13_14_DEVICE_ADDR, IFXETH_PHY_88Q2112_REG_14_ADDR, regdata);

    regdata = 0x4000 + devaddr;
    IfxEth_write_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_REG_13_14_DEVICE_ADDR, IFXETH_PHY_88Q2112_REG_13_ADDR, regdata);

    IfxEth_read_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_REG_13_14_DEVICE_ADDR, IFXETH_PHY_88Q2112_REG_14_ADDR, &value);

    *pdata = value;
}


void IfxEth_Phy_88q2112_write_mdio_reg(uint32 slaveaddr, uint32 devaddr, uint32 regaddr, uint32 data)
{
    uint32 regdata = devaddr;

    IfxEth_write_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_REG_13_14_DEVICE_ADDR, IFXETH_PHY_88Q2112_REG_13_ADDR, regdata);

    regdata = regaddr;
    IfxEth_write_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_REG_13_14_DEVICE_ADDR, IFXETH_PHY_88Q2112_REG_14_ADDR, regdata);

    regdata = 0x4000 + devaddr;
    IfxEth_write_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_REG_13_14_DEVICE_ADDR, IFXETH_PHY_88Q2112_REG_13_ADDR, regdata);

    IfxEth_write_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_REG_13_14_DEVICE_ADDR, IFXETH_PHY_88Q2112_REG_14_ADDR, data);

}


void IfxEth_Phy_88q2112_z1_fix()
{
    boolean is_z1_found = FALSE;
    uint8 i;
    uint32 value;
    uint32 phy_number = get_88q2112_number();
    uint32* slave_address = get_slave_address_pointer();

    for(i = IFXETH_PHY_88Q2112_1_PHYID; i <= phy_number; i++)
    { /* check if there's any Z1 silicon */
        IfxEth_Phy_88q2112_read_mdio_reg(slave_address[i-1], 3, 0x3, &value);
        if(value == 0x0980)
        { /* found Z1 silicon*/
            is_z1_found = TRUE;
            break;
        }
    }
    if(!is_z1_found)
    { /* if there's no z1 silicon, we don't need this */
        return;
    }
    Q2112_PRINTF_ALWAYS(("WAR Fix on Z1 low power mode issue\r\n"));
    for(i = IFXETH_PHY_88Q2112_1_PHYID; i <= phy_number; i++)
    {
        IfxEth_Phy_88q2112_write_mdio_reg(slave_address[i-1], 3,0x800d,0);
    }
    SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
    SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
    SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
    SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
    waitTime(100*TimeConst[TIMER_INDEX_1MS]);
    SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
    SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);

    // allow the chip to be ready for reading after hw reset
    waitTime(200*TimeConst[TIMER_INDEX_1MS]);
}



//void IfxEth_Phy_88q2112_Set(uint32 slaveaddr, uint32 speed, e_role role)
void IfxEth_Phy_88q2112_Set(uint32 slaveaddr, uint32 speed, e_role role, uint8 mode_sel)
{
    uint32 value;

    if(is_z1_silicon(slaveaddr) == TRUE)
    { /* Z1 */
        if(speed == 1000)
        { /* 1G */
            initQ2112Z1Ge(slaveaddr);
        }
        else
        { /* 100M */
            initQ2112Z1Fe(slaveaddr);
        }
    }
    else
    { /* A0 */
        if(speed == 1000)
        { /* 1G */
            initQ2112Ge(slaveaddr, mode_sel);
        }
        else
        { /* 100M */
            initQ2112Fe(slaveaddr);

        }
    }

    if(role == HSD_MASTER)
    { /* Master */
        IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_DEV_1_ADDR,
               IFXETH_PHY_88Q2112_PMA_PMD_CTRL_REG, &value);
        value |= IFXETH_PHY_88Q2112_MASTER_SLAVE_BIT_MASK;
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_DEV_1_ADDR,
               IFXETH_PHY_88Q2112_PMA_PMD_CTRL_REG, value);
    }
    else
    { /* Slave */
        IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_DEV_1_ADDR,
               IFXETH_PHY_88Q2112_PMA_PMD_CTRL_REG, &value);
        value &= (~IFXETH_PHY_88Q2112_MASTER_SLAVE_BIT_MASK);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, IFXETH_PHY_88Q2112_DEV_1_ADDR,
               IFXETH_PHY_88Q2112_PMA_PMD_CTRL_REG, value);
    }
}

#if 1
boolean IfxEth_Phy_88q2112_init(uint32 phyid)
{
    uint32* slave_address = get_slave_address_pointer();
    uint8 ubTmp1;
    uint8 ubTmp2;

    if(!is_phy_id_valid(phyid))
        return FALSE;

    if(!Q2112_IsPhyInitDone[phyid-1])
    {
        PRINTF_BOOT(("IfxEth_Phy_88q2112_init\r\n"));

        #if 1
        // Read InfoROM configuration
        ubTmp1= Chk_InfoROM_88q2112_Mode_by_PHYID(phyid); // Read Legacy / Compliance mode setting
        //DEBUG_PRINTF_ALWAYS(("Read IfxEth_Phy_88q2112_init [%d] Mode : %d \r\n",phyid,ubTmp1));
        ubTmp2= Chk_InfoROM_88q2112_MS_by_PHYID(phyid);   // Read Master / Slave setting

        //IfxEth_Phy_88q2112_do_init(slave_address[phyid-1]);
        IfxEth_Phy_88q2112_do_init(slave_address[phyid-1], ubTmp1, ubTmp2);

        #else

        IfxEth_Phy_88q2112_do_init(slave_address[phyid-1], INFOROM_ADDR_TBL_ETH_PHY_SLAVE, INFOROM_ADDR_TBL_ETH_PHY_MODE_LEGACY);
        #endif

        //Q2112_IsPhyInitDone[phyid-1] = TRUE;
    }

    return TRUE;
}

boolean IfxEth_Phy_88q2112_b2b_re_init(uint32 phyid)
{
    uint32* slave_address = get_slave_address_pointer();
    uint8 ubTmp1;
    uint8 ubTmp2;

    if(!is_phy_id_valid(phyid))
        return FALSE;
    
    Q2112_PRINTF_ALWAYS(("IfxEth_Phy_88q2112_init\r\n"));

    #if 1
    // Read InfoROM configuration
    ubTmp1= Chk_InfoROM_88q2112_Mode_by_PHYID(phyid); // Read Legacy / Compliance mode setting
    ubTmp2= Chk_InfoROM_88q2112_MS_by_PHYID(phyid);   // Read Master / Slave setting

    IfxEth_Phy_88q2112_do_init(slave_address[phyid-1], ubTmp1, ubTmp2);

    #else

    IfxEth_Phy_88q2112_do_init(slave_address[phyid-1], INFOROM_ADDR_TBL_ETH_PHY_SLAVE, INFOROM_ADDR_TBL_ETH_PHY_MODE_LEGACY);
    #endif
    
  return TRUE;
}

boolean IfxEth_B2BPhy_88q2112_init(void)
{
    uint8 ubTmp1;
    uint8 ubTmp2;

    //IfxEth_Cfg_Aurix_MDC_E3550(0);
    //IfxPort_setPinHigh(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
    //IfxPort_setPinModeOutput(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    //SET_GPIOEXP_OUTPUT_HIGH(ARX_ETH_2112_GRP2_MDC_CTRL_EXP);
    //SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_2112_GRP2_MDC_CTRL_EXP);

    //IfxEth_Cfg_Aurix_MDC(0);
    IfxEth_Cfg_Aurix_MDC_5050();

    #if 1
    // Read InfoROM configuration
    ubTmp1= Chk_InfoROM_88q2112_Mode_by_PHYID(0); // Read Legacy / Compliance mode setting
    ubTmp2= Chk_InfoROM_88q2112_MS_by_PHYID(0);   // Read Master / Slave setting

    IfxEth_Phy_88q2112_do_init(0, ubTmp1, ubTmp2);

    #else

    IfxEth_Phy_88q2112_do_init(0, INFOROM_ADDR_TBL_ETH_PHY_SLAVE, INFOROM_ADDR_TBL_ETH_PHY_MODE_LEGACY);
    #endif

    return TRUE;
}

boolean IfxEth_Phy_88q2112_deinit(uint32 phyid)
{
    if(!is_phy_id_valid(phyid))
        return FALSE;
    Q2112_IsPhyInitDone[phyid-1] = FALSE;
    return TRUE;
}
#endif

boolean IfxEth_Phy_88q2112_link(uint32 phyid)
{
    boolean linkEstablished = FALSE;
    uint32 value;
    uint32* slave_address = get_slave_address_pointer();
    if(!is_phy_id_valid(phyid))
        return FALSE;
    if(Q2112_IsPhyInitDone[phyid-1])
    {
        IfxEth_Phy_88q2112_read_mdio_reg(slave_address[phyid-1], IFXETH_PHY_88Q2112_DEV_3_ADDR, IFXETH_PHY_88Q2112_1000BASET1_STA_REG_1, &value);
        linkEstablished = (value & 0x4) >> 2;
    }
    return linkEstablished;
}


#if 1
uint32 get_88q2112_number(void)
{
    if(board.project == P3479)
    {
        if ((board.pcb_spin == 'A') && (board.pcb_version >= 3))
        { // A03 and onwards, only have 7 88Q2112
            return IFXETH_PHY_88Q2112_NUMBER_P3479_A03;
        }
        else
        { // A00, A01 has 8 88Q2112
            return IFXETH_PHY_88Q2112_NUMBER_P3479_A01;
        }
    }
    else
    {
        return IFXETH_PHY_88Q2112_NUMBER_E3550;
    }
}
#endif

#endif //endof #if ETH_PHY_88Q2112_ENABLE == 1
