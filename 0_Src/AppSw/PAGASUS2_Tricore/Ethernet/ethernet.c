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
#include "ethernet.h"
#include "Configuration.h"
#include "SysSe/Comm/Ifx_Shell.h"
#include "SysSe/Bsp/Bsp.h"
#include "Ifx_lwip.h"
#include <internal_fw_debug.h>
//#include "tegraethcomm.h"
#include "../I2c/I2cRom.h"
#include "i2crom_drv.h"

//#include "Flash/ota.h"
#ifdef TC397B
#include <Geth/Eth_Devices/IfxEth_Devices.h>
#else
#include <Eth/Eth_Devices/IfxEth_Devices.h>
#endif
#include <ethernet_device.h>
#include "error_report.h"
#include "board.h"
#include "config_pg2_feasures.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define AURIX_MAC_H_LOCATION  0XAF0B0014;
#define AURIX_MAC_L_LOCATION  0XAF0B0010;

#define ETH_APP_DEBUG_ENABLE 1

#if ETH_APP_DEBUG_ENABLE == 1
#define ETH_APP_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define ETH_APP_DEBUG_PRINTF(x)
#endif


extern boolean boot_msg_flag;
#define ETH_APP_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#define PRINTF_BOOT(x)       ({if(boot_msg_flag){DEBUG_PRINTF(x);}})
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct
{
    boolean is_resume;  /* if it's resume only */
    boolean is_on;      /* if ethernet is acitve */
    e_eth_mode mode;    /* if it's static ip or dhcp */
    t_ip_addr ip;
}t_eth_app_data;
/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
static void Ethernet_SetIP(t_ip_addr* ip_p);
static void Ethernet_ChangeMode(e_eth_mode mode);
static void Initial_MacAddr(eth_addr_t *ethAddr_p);
/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
static t_eth_app_data eth_app_data=
{
    .is_resume = FALSE,
    .is_on= FALSE,
    .mode = STATIC_IP,
};

/* maintain one copy of mac address here for inquiry from command line */
static uint8 mac_addr_copy[6];
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
static void get_default_ip(t_ip_addr* ip_p)
{
    ip_p->ipaddr1 = DEFAULT_IP1;
    ip_p->ipaddr2 = DEFAULT_IP2;
    ip_p->ipaddr3 = DEFAULT_IP3;
    ip_p->ipaddr4 = DEFAULT_IP4;
    ip_p->mask1 = DEFAULT_MASK1;
    ip_p->mask2 = DEFAULT_MASK2;
    ip_p->mask3 = DEFAULT_MASK3;
    ip_p->mask4 = DEFAULT_MASK4;
}

static void Ethernet_change_config(e_eth_mode eth_mode, t_ip_addr* ip_p)
{
    if((eth_mode == DHCP) && (eth_app_data.mode == STATIC_IP))
    { /* STATIC ip => DHCP */
        PRINTF_BOOT(("Change static IP to DHCP\n"));
        Ethernet_ChangeMode(DHCP);
    }
    else if((eth_mode == STATIC_IP) && (eth_app_data.mode == STATIC_IP))
    { /* STATIC ip => STATIC ip, change IP address only */
        PRINTF_BOOT(("Change static IP address\n"));
        Ethernet_SetIP(ip_p);
        Ethernet_ChangeMode(STATIC_IP);
    }
    else if((eth_mode == DHCP) && (eth_app_data.mode == DHCP))
    { /* DHCP => DHCP */
        PRINTF_BOOT(("Already in DHCP mode \n"));
        Ethernet_ChangeMode(DHCP);
    }
    else if((eth_mode == STATIC_IP) && (eth_app_data.mode == DHCP))
    { /* DHCP => STATIC ip */
        PRINTF_BOOT(("Change DHCP to static IP \n"));
        Ethernet_SetIP(ip_p);
        Ethernet_ChangeMode(STATIC_IP);
    }
}


// Read Master/Slave configuration from InfoROM
static uint8 Chk_InfoROM_88q5050_MS_by_Port(uint8 portid)
{
    uint8 bytesleft;
    T_ROM_ETHPHY_MODE_MS_DATA ethphy_mode_ms;

    if((portid < 1) || (portid > 4))
        bytesleft= INFOROM_ADDR_TBL_ETH_PHY_SLAVE;
    else
    {
        bytesleft= EthPHYModeMS_ReadValue(&ethphy_mode_ms.uB[0]);   // read 3 bytes data from I2C0 InfoROM
        if(!bytesleft)
        {
            //successful
            if(portid == 1)
                bytesleft= ethphy_mode_ms.detail.J10_1_M_S;
            else if(portid == 2)
                bytesleft= ethphy_mode_ms.detail.J10_2_M_S;
            else if(portid == 3)
                bytesleft= ethphy_mode_ms.detail.J10_3_M_S;
            else if(portid == 4)
                bytesleft= ethphy_mode_ms.detail.J10_4_M_S;
            else
                bytesleft= INFOROM_ADDR_TBL_ETH_PHY_SLAVE;
        }
        else
        {
            //failed
            bytesleft= INFOROM_ADDR_TBL_ETH_PHY_SLAVE;
        }
    }

    return bytesleft;
}


#if 1
static void Ethernet_start(e_eth_mode eth_mode, eth_addr_t ethAddr, t_ip_addr* ip_p)
{
    switch(eth_mode)
    {
    case STATIC_IP:
        Ifx_Lwip_init_nv(eth_app_data.is_resume, ethAddr);
        Ethernet_SetIP(ip_p);
        Ethernet_ChangeMode(STATIC_IP);
        break;
    case DHCP:
        Ifx_Lwip_init_nv(eth_app_data.is_resume, ethAddr);
        Ethernet_ChangeMode(DHCP);
        break;
    case ROM_DEFAULT:
        if(setting_is_initial())
        { /* if ROM is initial, use ROM ethernet setting */
            e_eth_mode mode = *(e_eth_mode *)setting_get(ROMID_ETH_MODE);
            t_ip_addr ip = *(t_ip_addr*)setting_get(ROMID_IP_ADDR);
            if(mode >= ROM_DEFAULT)
            {
                ETH_APP_DEBUG_PRINTF(("Warning: read the ethernet mode %d from ROM, correct it to static mode\n", mode));
                mode = DEFAULT_MODE;
            }
            Ifx_Lwip_init_nv(eth_app_data.is_resume, ethAddr);
            ETH_APP_DEBUG_PRINTF(("use rom setting : eth mode %d, ip is %d.%d.%d.%d\n", mode,
                    ip.ipaddr1, ip.ipaddr2, ip.ipaddr3, ip.ipaddr4));
            if(mode != DHCP)
            {
                Ethernet_SetIP(&ip);
            }
            Ethernet_ChangeMode(mode);
        }
        else
        { /* if ROM is NOT initial, use default setting */
            ETH_APP_DEBUG_PRINTF(("Rom setting is not init, use static ip mode\n"));
            setting_init();
            Ifx_Lwip_init_nv(eth_app_data.is_resume, ethAddr);
            t_ip_addr ip;
            get_default_ip(&ip);
            Ethernet_SetIP(&ip);
            Ethernet_ChangeMode(DEFAULT_MODE);
        }
        break;
    default:
        break;
    }
}


void Ethernet_Init(e_eth_mode eth_mode, t_ip_addr* ip_p)
{
    ETH_APP_DEBUG_PRINTF(("%s\r\n", __func__));
    if(eth_app_data.is_on)
    { /* ETH is active, need to change config */
        Ethernet_change_config(eth_mode, ip_p);
    }
    else
    { /* ETH is NOT active, active it */
        if(board.aurix_index == MAIN_AURIX)
        {
            /* Initial SJA1105 to get the MII clock first */
            Ethernet_Devices_Initial(switch_sja1105);

            /* wait for the clock to be up */
            waitTime(10*TimeConst[TIMER_INDEX_1MS]);

            eth_addr_t ethAddr;
            Initial_MacAddr(&ethAddr);
            PRINTF_BOOT(("MAC Address programmed to this device: %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                        ethAddr.addr[0],ethAddr.addr[1],ethAddr.addr[2],ethAddr.addr[3],ethAddr.addr[4],ethAddr.addr[5]));
            // then we activate the PHY
            // we wait for 10ms
            waitTime(10*TimeConst[TIMER_INDEX_1MS]);

            Ethernet_start(eth_mode, ethAddr, ip_p);

            Ethernet_Devices_Initial(phy_88q2112);

            waitTime(200*TimeConst[TIMER_INDEX_1MS]);

            Ethernet_Devices_Initial(switch_88e6321);

            waitTime(100*TimeConst[TIMER_INDEX_1MS]);   // this is just a placeholder value for delay

            Ethernet_Devices_Initial(switch_sja1105_step2);

    #ifdef HYPERION
            Ethernet_Devices_Initial(default_config_init);
    #endif

            // Initialize 88Q5050 P1~P4 mode to Master @ 2020_0917
            // Set P1Phy
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0x0834);       // Assign Register Address from Q1010 datasheet
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x18, 0x8021);       // Device1, 0x0834(Q1010) @ Port 1(Q5050)
            if(Chk_InfoROM_88q5050_MS_by_Port(1) == INFOROM_ADDR_TBL_ETH_PHY_MASTER)
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0xC000);   // Write data to Register Address just assigned from Q1010 datasheet, bit14= 1= Master(Q1010) 
            else
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0x8000);   // Write data to Register Address just assigned from Q1010 datasheet, bit14= 0= Slave(Q1010)
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x18, 0x8421);       

            // Set P2Phy
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0x0834);       // Assign Register Address from Q1010 datasheet
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x18, 0x8041);       // Device1, 0x0834(Q1010) @ Port 2(Q5050)
            if(Chk_InfoROM_88q5050_MS_by_Port(2) == INFOROM_ADDR_TBL_ETH_PHY_MASTER)
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0xC000);   // Write data to Register Address just assigned from Q1010 datasheet, bit14= 1= Master(Q1010)
            else
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0x8000);   // Write data to Register Address just assigned from Q1010 datasheet, bit14= 0= Slave(Q1010)
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x18, 0x8441);       

            // Set P3Phy
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0x0834);
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x18, 0x8061);
            if(Chk_InfoROM_88q5050_MS_by_Port(3) == INFOROM_ADDR_TBL_ETH_PHY_MASTER)
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0xC000);
            else
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0x8000);
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x18, 0x8461);

            // Set P4Phy
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0x0834);
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x18, 0x8081);
            if(Chk_InfoROM_88q5050_MS_by_Port(4) == INFOROM_ADDR_TBL_ETH_PHY_MASTER)
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0xC000);
            else
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0x8000);
            
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x18, 0x8481);

        }
        else
        { // 2nd Aurix, only initial the network, no need to initial Switches and PHYs
            eth_addr_t ethAddr;
            Initial_MacAddr(&ethAddr);
            PRINTF_BOOT(("MAC Address programmed to this device: %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                        ethAddr.addr[0],ethAddr.addr[1],ethAddr.addr[2],ethAddr.addr[3],ethAddr.addr[4],ethAddr.addr[5]));
            // then we activate the PHY
            // we wait for 10ms
            waitTime(10*TimeConst[TIMER_INDEX_1MS]);
            Ethernet_start(eth_mode, ethAddr, ip_p);
        }
        if(eth_app_data.is_resume)
        {
            TegraEthComm_resume();
//#if ETHERNET_OTA_ENABLE == 1
            OTA_TegraEthComm_resume();
//#endif

        }
        else
        {
            TegraEthComm_init();
//#if ETHERNET_OTA_ENABLE == 1
            OTA_TegraEthComm_init();
//#endif
//#if ETHERNET_UDP_CMD_ENABLE == 1
            tegra_console_init();
//#endif
            UdpMonitor_init();
        }
        eth_app_data.is_resume = TRUE;
        eth_app_data.is_on = TRUE;
    }
}
#endif

void Ethernet_Stop(void)
{
    ETH_APP_DEBUG_PRINTF(("%s\n", __func__));
    if(!eth_app_data.is_on)
    {
        return;
    }
    if(eth_app_data.mode == DHCP)
    {
        Ifx_Lwip_deinit_dhcp();
    }
    //NEED
    Ifx_Lwip_deinit(TRUE);
    Ethernet_Devices_DeInitial(switch_sja1105 | switch_88e6321 | phy_88q2112);
    eth_app_data.is_on = FALSE;
}

boolean Ethernet_Is_Ip_Bound(void)
{
    if(eth_app_data.mode == STATIC_IP)
    {
        return TRUE;
    }
    else
    {
#if 0
        return Ifx_Lwip_IsDhcpBound();
#else
        return TRUE;
#endif
    }
}

boolean Ethernet_get_ip(uint32* p_ipaddr)
{
    ETH_APP_DEBUG_PRINTF(("%s\r\n", __func__));
    if(eth_app_data.mode == STATIC_IP)
    {
        *p_ipaddr = (eth_app_data.ip.ipaddr4 << 24) |
                    (eth_app_data.ip.ipaddr3 << 16) |
                    (eth_app_data.ip.ipaddr2 << 8) |
                    (eth_app_data.ip.ipaddr1);
        return TRUE;
    }
    else
    {
#if 0 //not support yet
        return Ifx_Lwip_GetBoundIp(p_ipaddr);
#else
        return 1;
#endif
    }
}

uint8* Ethernet_get_mac(void)
{
    return mac_addr_copy;
}


static void Ethernet_SetIP(t_ip_addr* ip_p)
{
    ETH_APP_DEBUG_PRINTF(("%s\r\n", __func__));
    if(ip_p != NULL)
    {
        memcpy(&eth_app_data.ip, ip_p, sizeof(t_ip_addr));
         PRINTF_BOOT(("Set IP address to %d.%d.%d.%d\r\n",
                eth_app_data.ip.ipaddr1, eth_app_data.ip.ipaddr2,
                eth_app_data.ip.ipaddr3, eth_app_data.ip.ipaddr4));
        Ifx_Lwip_set_ip(eth_app_data.ip.ipaddr1, eth_app_data.ip.ipaddr2,
                eth_app_data.ip.ipaddr3, eth_app_data.ip.ipaddr4,
                eth_app_data.ip.mask1, eth_app_data.ip.mask2,
                eth_app_data.ip.mask3, eth_app_data.ip.mask4);
        /* save to ROM */
        //NEED marked temp
//      setting_set(ROMID_IP_ADDR, &eth_app_data.ip);
        ETH_APP_DEBUG_PRINTF(("Save to rom: ip is %d.%d.%d.%d\r\n", ip_p->ipaddr1,
                              ip_p->ipaddr2, ip_p->ipaddr3, ip_p->ipaddr4));
    }
}


static void Ethernet_ChangeMode(e_eth_mode mode)
{
    if(mode >= MAX_ETH_MODE)
    {
        return;
    }
    if(mode == STATIC_IP)
    { /* DHCP => STATIC */
        //NEED marked temp
        Ifx_Lwip_deinit_dhcp();
    }
    else if(mode == DHCP)
    { /* STATIC => DHCP */
        //NEED marked temp
        Ifx_Lwip_init_dhcp();
    }
    eth_app_data.mode = mode;
    //NEED marked temp  
//  setting_set(ROMID_ETH_MODE, &eth_app_data.mode);
    ETH_APP_DEBUG_PRINTF(("Save to rom: mode %d \r\n", eth_app_data.mode));
}

static void Initial_MacAddr(eth_addr_t *ethAddr_p)
{
    uint32 mac_address_h = *(uint32*)AURIX_MAC_H_LOCATION;
    uint32 mac_address_l = *(uint32*)AURIX_MAC_L_LOCATION;
    uint8 buffer[10];
    e_AurixIndex get_I2cRom_Mac_index;
    get_I2cRom_Mac_index = detect_Aurix_index();

    if(i2crom_getEthMac(buffer, get_I2cRom_Mac_index))
    {
        ethAddr_p->addr[0] = buffer[5];
        ethAddr_p->addr[1] = buffer[4];
        ethAddr_p->addr[2] = buffer[3];
        ethAddr_p->addr[3] = buffer[2];
        ethAddr_p->addr[4] = buffer[1];
        ethAddr_p->addr[5] = buffer[0];
        PRINTF_BOOT(("Read valid mac address from I2C ROM\r\n"));
    }
    else
    {
        ethAddr_p->addr[0]= (mac_address_h & 0x0000ff00) >>8;
        ethAddr_p->addr[1]= (mac_address_h & 0x000000ff) >>0;
        ethAddr_p->addr[2]= (mac_address_l & 0xff000000) >>24;
        ethAddr_p->addr[3]= (mac_address_l & 0x00ff0000) >>16;
        ethAddr_p->addr[4]= (mac_address_l & 0x0000ff00) >>8;
        ethAddr_p->addr[5]= (mac_address_l & 0x000000ff) >>0;
        PRINTF_BOOT(("No valid mac address from I2C ROM, get it from Aurix DFLASH\r\n"));
    }
    if((ethAddr_p->addr[0] == 0) &&
            (ethAddr_p->addr[1] == 0) &&
            (ethAddr_p->addr[2] == 0) &&
            (ethAddr_p->addr[3] == 0) &&
            (ethAddr_p->addr[4] == 0) &&
            (ethAddr_p->addr[5] == 0))
    {
        PRINTF_BOOT(("Aurix Mac address is all 0, set it to temporary address\r\n"));

        ethAddr_p->addr[0]= 0x11;
        ethAddr_p->addr[1]= 0x12;
        ethAddr_p->addr[2]= 0x13;
        ethAddr_p->addr[3]= 0x14;
        ethAddr_p->addr[4]= 0x15;
        if(board.aurix_index == MAIN_AURIX)
            ethAddr_p->addr[5]= 0x16;
        else
            ethAddr_p->addr[5]= 0x17;
        //set_error_flag(INVAILD_MAC_ADDRESS_ERR);
    }

#if MAC_ADDRESS_MANUAL_SET == 1
    ethAddr_p->addr[0]= 0x11;
    ethAddr_p->addr[1]= 0x12;
    ethAddr_p->addr[2]= 0x13;
    ethAddr_p->addr[3]= 0x14;
    ethAddr_p->addr[4]= 0x15;
    ethAddr_p->addr[5]= 0x16;
#endif

    /* save one copy of the mac address */
    memcpy(mac_addr_copy, ethAddr_p->addr, 6);
}

void geth_init_loadsim(t_ip_addr* ip)
{
    eth_addr_t ethAddr;
    Initial_MacAddr(&ethAddr);
    waitTime(10*TimeConst[TIMER_INDEX_1MS]);
    Ethernet_start(STATIC_IP, ethAddr, ip);

}
