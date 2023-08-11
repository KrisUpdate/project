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

#include "SysSe/Comm/Ifx_Shell.h"
#include "SysSe/Bsp/Bsp.h"
#include "Configuration.h"
#include "EthernetCmds.h"
#include "ethernet.h"
#include "Ifx_lwip.h"
#include "../Gpio/Gpio.h"
#include "resource.h"
//#include "tegraethcomm.h"
#if ETH_PHY_88Q2112_ENABLE == 1
#include "phy_88q2112.h"
#endif
#include "switch_sja1105.h"
#include "marvell_switch.h"
#include <internal_fw_debug.h>
#include <string.h>
#ifdef TC397B
#include <Geth/Eth_Devices/IfxEth_Devices.h>
#else
#include <Eth/Eth_Devices/IfxEth_Devices.h>
#endif
#include "board.h"
#include "config_pg2_pins_def.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

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

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/



boolean mdioinit(pchar args, void *data, IfxStdIf_DPipe *io)
{
#if PROJECT == PROJECT_PAGASUS2
    boolean enable = FALSE;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : mdioinit [gpio enable/disable]"ENDL);
        IfxStdIf_DPipe_print(io, "           : enable/disable gpio mdio"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "gpio") != FALSE)
        {
            if (Ifx_Shell_matchToken(&args, "enable") != FALSE)
            {
                enable = TRUE;
            }
            else if (Ifx_Shell_matchToken(&args, "disable") != FALSE)
            {
                enable = FALSE;
            }
            IfxEth_mdio_set_simulating(enable);
            return TRUE;
        }
        IfxEth_mdioinit();
        return TRUE;
    }
#else
    IfxStdIf_DPipe_print(io, "Error : invalid command "ENDL);
    return TRUE;
#endif
}


/** \brief Handle the 'mdioset' command
 *
 * \par Syntax
 *  - mdioset : set mdio control logic, it's for bring up debug purpose
 *    when we prove multi-chip mode works fine, we will remove it
 */
boolean mdioset(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 slaveaddr;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : mdioset slaveaddress"ENDL);
        IfxStdIf_DPipe_print(io, "           : set MDIO control logic to access device"ENDL);
    }
    else
    {
        // now we get the first argument (phy address)
        if (Ifx_Shell_parseUInt32(&args, &slaveaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid slave address %s"ENDL, args);
            return TRUE;
        }
        IfxStdIf_DPipe_print(io, "Set MDIO control logic to access slave address 0x%x"ENDL, slaveaddr);
        IfxEth_Cfg_Aurix_MDC(slaveaddr);
    }

}


/** \brief Handle the 'mdiord' command
 *
 * \par Syntax
 *  - mdiord : read MII Register of connected phy through MDIO/ interface
 */
boolean mdiord(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 phyaddr,regaddr,count,value;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : mdiord x y n"ENDL);
        IfxStdIf_DPipe_print(io, "           > read MII Register at smi(slave) address x offset y for n bytes"ENDL);
        IfxStdIf_DPipe_print(io, "           > in DAX, please use ethswrd to access switch register, "ENDL);
        IfxStdIf_DPipe_print(io, "           > use ethphyrd to access phy register, "ENDL);
    }
    else
    {
        // now we get the first argument (phy address)
        if (Ifx_Shell_parseUInt32(&args, &phyaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid smi address %s"ENDL, args);
            return TRUE;
        }
        if (phyaddr > 69905 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid smi address %s"ENDL, args);
            return TRUE;
        }
        //now we get the 2nd argument (mii reg address)
        if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid register address %s"ENDL, args);
            return TRUE;
        }
        // now we get the number of registers to read
        if (Ifx_Shell_parseUInt32(&args, &count, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid count %s"ENDL, args);
            return TRUE;
        }

        while (count)
        {
            IfxEth_read_mdio_reg(phyaddr, IFXETH_SW_88E88E6321_SMI_COMMAND_DEVICE_ADDR, regaddr,&value);

            IfxStdIf_DPipe_print(io, "0x%04x"ENDL,value);
            count --;
            regaddr++;
        }

    }
    return TRUE;
}
/** \brief Handle the 'mdiord' command
 *
 * \par Syntax
 *  - mdiowr : write value to MII Register of connected phy through MDIO/ interface
 */
boolean mdiowr(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 phyaddr,regaddr,value;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : mdiowr x y z"ENDL);
        IfxStdIf_DPipe_print(io, "           > write value z to SMI(slave) address x offset y"ENDL);
        IfxStdIf_DPipe_print(io, "           > in DAX, please use ethswwr to access switch register, "ENDL);
        IfxStdIf_DPipe_print(io, "           > use ethphywr to access phy register, "ENDL);
    }
    else
    {
        // now we get the first argument (phy address)
        if (Ifx_Shell_parseUInt32(&args, &phyaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid smi address %s"ENDL, args);
            return TRUE;
        }
        if (phyaddr > 69905 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid smi address %s"ENDL, args);
            return TRUE;
        }
        //now we get the 2nd argument (mii reg address)

        if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid register address %s"ENDL, args);
            return TRUE;
        }
        // now we get the second argument (value)
        if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
            return TRUE;
        }
        IfxEth_write_mdio_reg(phyaddr, IFXETH_SW_88E88E6321_SMI_COMMAND_DEVICE_ADDR, regaddr, value);
    }
    return TRUE;
}



#if ETH_SWITCH_ENABLE == 1
/** \brief Handle the 'rdswphy' command
 *
 * \par Syntax
 *  - rdswphy : read Ethernet Switch Phy Register
 */
boolean readSWitchPhy(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 slaveaddr, phyaddr,regaddr,count,value;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : rdswphy x y z n"ENDL);
        IfxStdIf_DPipe_print(io, "           > read Ethernet Switch slave address x Phy address y offset z for n bytes"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &slaveaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid slave address %s"ENDL, args);
            return TRUE;
        }

        if (Ifx_Shell_parseUInt32(&args, &phyaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        if (phyaddr > 69905 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }

        if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid offset address %s"ENDL, args);
            return TRUE;
        }
        // now we get the number of registers to read
        if (Ifx_Shell_parseUInt32(&args, &count, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid count %s"ENDL, args);
            return TRUE;
        }

        while (count)
        {
            IfxEth_Sw_88E88E6321_read_phy_reg(slaveaddr, (uint8)phyaddr, (uint8)regaddr,&value);
            IfxStdIf_DPipe_print(io, "0x%04x"ENDL,value);
            count --;
            regaddr++;
        }

    }
    return TRUE;
}


/** \brief Handle the 'wrswphy' command
 *
 * \par Syntax
 *  - wrswphy : write value to Ethernet Switch Phy Register
 */
boolean writeSWitchPhy(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 slaveaddr, phyaddr,regaddr,count,value;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : wrswphy x y z v"ENDL);
        IfxStdIf_DPipe_print(io, "           > write value v to Ethernet Switch slave address x Phy address y offset z"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &slaveaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid slave address %s"ENDL, args);
            return TRUE;
        }

        if (Ifx_Shell_parseUInt32(&args, &phyaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        if (phyaddr > 69905 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        //now we get the 2nd argument register offset address)
        if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid offset address %s"ENDL, args);
            return TRUE;
        }
        // now we get the value to write
        if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
            return TRUE;
        }
        IfxEth_Sw_88E88E6321_write_phy_reg(slaveaddr, (uint8)phyaddr, (uint8)regaddr, value);

    }
    return TRUE;
}


/** \brief Handle the 'rdswavb' command
 *
 * \par Syntax
 *  - rdswavb : read Ethernet Switch Avb Register
 */
boolean readSWitchAvb(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 slaveaddr, portaddr,regaddr,count,value;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : rdswavb x y z n"ENDL);
        IfxStdIf_DPipe_print(io, "           > read Ethernet Switch slave address x, AVB port y offset z for n bytes"ENDL);
        IfxStdIf_DPipe_print(io, "           > port y=0xf to access to global PTP register"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &slaveaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid slave address %s"ENDL, args);
            return TRUE;
        }

        if (Ifx_Shell_parseUInt32(&args, &portaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port address %s"ENDL, args);
            return TRUE;
        }
        if (portaddr > 69905 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port address %s"ENDL, args);
            return TRUE;
        }
        //now we get the 2nd argument ( reg address)
        if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid offset address %s"ENDL, args);
            return TRUE;
        }
        // now we get the number of registers to read
        if (Ifx_Shell_parseUInt32(&args, &count, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid count %s"ENDL, args);
            return TRUE;
        }

        while (count)
        {
            IfxEth_Sw_88E88E6321_read_avb_reg(slaveaddr, (uint8)portaddr, (uint8)regaddr,&value);
            IfxStdIf_DPipe_print(io, "0x%04x"ENDL,value);
            count --;
            regaddr++;
        }

    }
    return TRUE;
}

/** \brief Handle the 'wrswavb' command
 *
 * \par Syntax
 *  - wrswavb : write value to Ethernet Switch Avb Register
 */
boolean writeSWitchAvb(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 slaveaddr, portaddr,regaddr,count,value;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : wrswavb x y z v"ENDL);
        IfxStdIf_DPipe_print(io, "           > write value v to Ethernet Switch slave address x Avb port y offset z"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &slaveaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid s address %s"ENDL, args);
            return TRUE;
        }
        if (Ifx_Shell_parseUInt32(&args, &portaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port address %s"ENDL, args);
            return TRUE;
        }
        if (portaddr > 69905 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port address %s"ENDL, args);
            return TRUE;
        }
        //now we get the 2nd argument register offset address)
        if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid offset address %s"ENDL, args);
            return TRUE;
        }
        // now we get the value to write
        if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
            return TRUE;
        }

        IfxEth_Sw_88E88E6321_write_avb_reg(slaveaddr, (uint8)portaddr, (uint8)regaddr, value);
    }
    return TRUE;
}
#endif //endof #if ETH_SWITCH_ENABLE == 1

    /** \brief Handle the 'ethinit' command
     *
     * \par Syntax
     *  - ethinit : initialize the ethernet modules and phy
     */
boolean ethernetinit(pchar args, void *data, IfxStdIf_DPipe *io)
{
    boolean dhcp_request = TRUE;
    uint32 ipaddr1, ipaddr2, ipaddr3, ipaddr4;
    uint32 mask1, mask2, mask3, mask4;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "           > ethinit [s ip1 ip2 ip3 ip4 nm1 nm2 nm3 nm4]"ENDL);
        IfxStdIf_DPipe_print(io, "           > Initialize the Ethernet module and the Phy with IP address assigned by DHCP (default) or with static assignment"ENDL);
        IfxStdIf_DPipe_print(io, "           > Below are all the format of this command"ENDL);
        IfxStdIf_DPipe_print(io, "           > ethinit:  get IP from DHCP server"ENDL);
        IfxStdIf_DPipe_print(io, "           > ethinit s ip1 ip2 ip3 ip4 nm1 nm2 nm3 nm4:  set static ip"ENDL);
        IfxStdIf_DPipe_print(io, "           > ethinit ip:  Read Aurix ip"ENDL);
        IfxStdIf_DPipe_print(io, "           > ethinit mac: Read Aurix mac address"ENDL);
    }
    else
    {
        if ((Ifx_Shell_matchToken(&args, "ip") != FALSE))
        {
            uint32 ipaddr=0;
            boolean ret = Ethernet_get_ip(&ipaddr);
            if(ret)
            {
                IfxStdIf_DPipe_print(io, "Aurix IP address is %d.%d.%d.%d"ENDL,
                        ipaddr & 0xFF, (ipaddr>>8) & 0xFF,
                            (ipaddr>>16) & 0xFF, (ipaddr>>24) & 0xFF);
            }
            else
            {
                IfxStdIf_DPipe_print(io, "Fail to get the IP address, it's not assigned yet"ENDL);
            }
            return TRUE;
        }

        if ((Ifx_Shell_matchToken(&args, "mac") != FALSE))
        {
            uint8 * mac_addr_ptr = Ethernet_get_mac();
            IfxStdIf_DPipe_print(io, "MAC Address: %02x:%02x:%02x:%02x:%02x:%02x"ENDL,
                    mac_addr_ptr[0], mac_addr_ptr[1], mac_addr_ptr[2],
                    mac_addr_ptr[3], mac_addr_ptr[4], mac_addr_ptr[5]);
            return TRUE;
        }
#if START_KIT_ENABLE != 1
        if(vdd3v3_off)
        {
            IfxStdIf_DPipe_print(io, "Error : Detect that power is off, can not execute the command"ENDL);
            return TRUE;
        }
#endif
        if ((Ifx_Shell_matchToken(&args, "s") != FALSE) || (Ifx_Shell_matchToken(&args, "S") != FALSE))
        {
            dhcp_request = FALSE;
        }

        if (!dhcp_request)
        {
            //IPADDR: 1
            if (Ifx_Shell_parseUInt32(&args, &ipaddr1, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %s"ENDL, args);
                return TRUE;
            }
            if (ipaddr1 > 255)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %d"ENDL, ipaddr1);
                return TRUE;
            }
            //IPADDR: 2
            if (Ifx_Shell_parseUInt32(&args, &ipaddr2, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %s"ENDL, args);
                return TRUE;
            }
            if (ipaddr2 > 255)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %d"ENDL, ipaddr2);
                return TRUE;
            }
            //IPADDR: 3
            if (Ifx_Shell_parseUInt32(&args, &ipaddr3, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %s"ENDL, args);
                return TRUE;
            }
            if (ipaddr3 > 255)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %d"ENDL, ipaddr3);
                return TRUE;
            }
            //IPADDR: 4
            if (Ifx_Shell_parseUInt32(&args, &ipaddr4, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %s"ENDL, args);
                return TRUE;
            }
            if (ipaddr4 > 255)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %d"ENDL, ipaddr4);
                return TRUE;
            }

            //MASK: 1
            if (Ifx_Shell_parseUInt32(&args, &mask1, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %s"ENDL, args);
                return TRUE;
            }
            if (mask1 > 255)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %d"ENDL, mask1);
                return TRUE;
            }
            //MASK: 2
            if (Ifx_Shell_parseUInt32(&args, &mask2, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %s"ENDL, args);
                return TRUE;
            }
            if (mask2 > 255)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %d"ENDL, mask2);
                return TRUE;
            }
            //MASK: 3
            if (Ifx_Shell_parseUInt32(&args, &mask3, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %s"ENDL, args);
                return TRUE;
            }
            if (mask3 > 255)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %d"ENDL, mask3);
                return TRUE;
            }
            //MASK: 4
            if (Ifx_Shell_parseUInt32(&args, &mask4, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %s"ENDL, args);
                return TRUE;
            }
            if (mask4 > 255)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %d"ENDL, mask4);
                return TRUE;
            }
            t_ip_addr ip;
            ip.ipaddr1 = ipaddr1;
            ip.ipaddr2 = ipaddr2;
            ip.ipaddr3 = ipaddr3;
            ip.ipaddr4 = ipaddr4;
            ip.mask1 = mask1;
            ip.mask2 = mask2;
            ip.mask3 = mask3;
            ip.mask4 = mask4;
            Ethernet_Init(STATIC_IP, &ip);
        }
        else
        {
            Ethernet_Init(DHCP, NULL);
        }
    }

    return TRUE;
}

#if ETH_SWITCH_ENABLE == 1 &&  DEBUG_ENABLE == 1

/** \brief Handle the 'ethvlan' command
 *
 * \par Syntax
 *  - ethinit : initialize the ethernet modules and phy
 */
boolean ethernetvlan(pchar args, void *data, IfxStdIf_DPipe *io)
{
    /* TODO: this command is old and not used, we need a new VLAN command to support VLAN */
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "           > ethvlan on/off"ENDL);
        IfxStdIf_DPipe_print(io, "           > turn on/off the switch vlan feature"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "on") != FALSE)
        {
            IfxEth_Marvell_Sw_Vlan_Enable(IFXETH_SWITCH_88E6321_1_ADDR);
        }
        else if (Ifx_Shell_matchToken(&args, "off") != FALSE)
        {
            IfxEth_Marvell_Sw_Vlan_Disable(IFXETH_SWITCH_88E6321_1_ADDR);
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid parameter %s"ENDL, args);
            return TRUE;
        }

    }
    return TRUE;
}
#endif

extern void ping_send_now_nv(ip_addr_t *ping_target);

/** \brief Handle the 'ping' command
 *
 * \par Syntax
 *  - ping : send a ping to any address or to the gateway
 */


boolean ethernetping(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 datas[4];
    ip_addr_t ping_target;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : ping"ENDL);
        IfxStdIf_DPipe_print(io, "           > Send a ping to any address or to the gateway, format is"ENDL);
        IfxStdIf_DPipe_print(io, "           > ping ip1 ip2 ip3 ip4"ENDL);
        IfxStdIf_DPipe_print(io, "           > note: space is used between each IP byte"ENDL);
    }
    else
    {
        // we return directly if the MultiCan is not initialized
        if (g_Lwip.initialized == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Initialize Ethernet first with command ethinit"ENDL);
            return TRUE;
        }
        if (Ifx_Shell_parseUInt32(&args, &datas[0], FALSE) == FALSE)
        {
            ping_target = (netif_default?netif_default->gw:ip_addr_any);
            ping_send_now(&ping_target);
            return TRUE;
        }
        if (Ifx_Shell_parseUInt32(&args, &datas[1], FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid data %s"ENDL, args);
            return TRUE;
        }
        if (Ifx_Shell_parseUInt32(&args, &datas[2], FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid data %s"ENDL, args);
            return TRUE;
        }
        if (Ifx_Shell_parseUInt32(&args, &datas[3], FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid data %s"ENDL, args);
            return TRUE;
        }
        ping_target.addr = (datas[0] & 0xFF) | ((datas[1] & 0xFF)<<8) | ((datas[2] & 0xFF)<<16) | ((datas[3] & 0xFF)<<24);
        ping_send_now_nv(&ping_target);
    }

    return TRUE;
}


/** \brief Handle the 'netstat' command
 *
 * \par Syntax
 *  - netstat : show status of ethernet
 */
boolean ethernetstat(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 datas[4];
    ip_addr_t ping_target;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : netstat"ENDL);
        IfxStdIf_DPipe_print(io, "           > Show status of ethernet"ENDL);
    }
    else
    {
        // we return directly if the MultiCan is not initialized
        if (g_Lwip.initialized == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Initialize Ethernet first with command ethinit"ENDL);
            return TRUE;
        }
        IfxStdIf_DPipe_print(io, "LINK    : xmit %d    recv %d    err %d"ENDL, lwip_stats.link.xmit, lwip_stats.link.recv, lwip_stats.link.err);
        IfxStdIf_DPipe_print(io, "ETHARP  : xmit %d    recv %d    err %d"ENDL, lwip_stats.etharp.xmit, lwip_stats.etharp.recv, lwip_stats.etharp.err);
        IfxStdIf_DPipe_print(io, "IP_FRAG : xmit %d    recv %d    err %d"ENDL, lwip_stats.ip_frag.xmit, lwip_stats.ip_frag.recv, lwip_stats.ip_frag.err);
        IfxStdIf_DPipe_print(io, "IP      : xmit %d    recv %d    err %d"ENDL, lwip_stats.ip.xmit, lwip_stats.ip.recv, lwip_stats.ip.err);
        IfxStdIf_DPipe_print(io, "ICMP    : xmit %d    recv %d    err %d"ENDL, lwip_stats.icmp.xmit, lwip_stats.icmp.recv, lwip_stats.icmp.err);
        IfxStdIf_DPipe_print(io, "UDP     : xmit %d    recv %d    err %d"ENDL, lwip_stats.udp.xmit, lwip_stats.udp.recv, lwip_stats.udp.err);
        IfxStdIf_DPipe_print(io, "TCP     : xmit %d    recv %d    err %d"ENDL, lwip_stats.tcp.xmit, lwip_stats.tcp.recv, lwip_stats.tcp.err);
    }

    return TRUE;
}

boolean ethernetpingdetect(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : heartbeat on/off"ENDL);
        IfxStdIf_DPipe_print(io, "           > start/stop detecting Tegra heartbeat"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "on") != FALSE)
        {
            TegraEthComm_EnableHeartBeatDetect();
            IfxStdIf_DPipe_print(io, "Start detecting Tegra heartbeat"ENDL);
        }
        else if (Ifx_Shell_matchToken(&args, "off") != FALSE)
        {
            TegraEthComm_DisableHeartBeatDetect();
            IfxStdIf_DPipe_print(io, "Stop detecting Tegra heartbeat"ENDL);
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid parameter %s, only support \"on\" and \"off\""ENDL, args);
            return TRUE;
        }
    }
    return TRUE;
}

IFX_EXTERN boolean ethernetiperf(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : iperf tcp/udp [show]"ENDL);
        IfxStdIf_DPipe_print(io, "           > start iperf, or show the package it handles "ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "tcp") != FALSE)
        {
            IfxStdIf_DPipe_print(io, "Start iperf tcp test"ENDL);
            IfxStdIf_DPipe_print(io, "Please run iperf -s before this command"ENDL);
            iperf_bridge_tcp_init();
        }
        else if (Ifx_Shell_matchToken(&args, "udp") != FALSE)
        {
            IfxStdIf_DPipe_print(io, "Start iperf udp test"ENDL);
            iperf_bridge_udp_init();
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid parameter %s, only support \"tcp\" and \"udp\""ENDL, args);
        }
    }
    return TRUE;
}

/*
 * ethswrd  marvell  slave_address  device_address  register_offset  count
ethswwr  marvell  slave_address  device_address  register_offset  value

ethswrd  nxp  register_offset  count
ethswwr  nxp  register_offset  value
 *
 * */


#if PROJECT == PROJECT_PAGASUS2
#define VALUE_MAX_BUFFER_SIZE  256

IFX_EXTERN boolean ethswitchread(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 slaveaddr, devaddr, regaddr, count;
    uint32 value_array[VALUE_MAX_BUFFER_SIZE];
    memset(value_array, 0, VALUE_MAX_BUFFER_SIZE);
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : ethswrd mdio/spi x y z n"ENDL);
        IfxStdIf_DPipe_print(io, "           > read Ethernet switch register with two format"ENDL);
        IfxStdIf_DPipe_print(io, "           > ethswrd mdio x y z n"ENDL);
        IfxStdIf_DPipe_print(io, "           > read switch slave address x, device address y, register offset z for n bytes by mdio"ENDL);
        IfxStdIf_DPipe_print(io, "           > ethswrd spi z n"ENDL);
        IfxStdIf_DPipe_print(io, "           > read switch register offset z for n dword by spi"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "mdio") != FALSE)
        {
            if (Ifx_Shell_parseUInt32(&args, &slaveaddr, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid slave address %s"ENDL, args);
                return TRUE;
            }
            if (Ifx_Shell_parseUInt32(&args, &devaddr, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device address %s"ENDL, args);
                return TRUE;
            }
            if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid register address %s"ENDL, args);
                return TRUE;
            }
            if (Ifx_Shell_parseUInt32(&args, &count, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid number %s"ENDL, args);
                return TRUE;
            }
            if((count == 0) || (count > VALUE_MAX_BUFFER_SIZE))
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid number %s, it should be range 1 ~ %d"ENDL, VALUE_MAX_BUFFER_SIZE);
            }
            IfxStdIf_DPipe_print(io, "Read back value from MDIO switch slave address 0x%x, device address 0x%x, register address 0x%x are"ENDL,
                                            slaveaddr, devaddr, regaddr);
            uint8 i;
            for(i=0; i<count; i++)
            {
                IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, regaddr, &value_array[i]);
                IfxStdIf_DPipe_print(io, "0x%04x"ENDL,value_array[i]);
                regaddr++;
            }
        }
        else if (Ifx_Shell_matchToken(&args, "spi") != FALSE)
        {/* Right now we only have SJA1105 using SPI, so call its interface directly */
            if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid register address %s"ENDL, args);
                return TRUE;
            }
            if (Ifx_Shell_parseUInt32(&args, &count, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid number %s"ENDL, args);
                return TRUE;
            }
            if((count == 0) || (count > VALUE_MAX_BUFFER_SIZE))
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid number %s, it should be range 1 ~ %d"ENDL, VALUE_MAX_BUFFER_SIZE);
            }
            Switch_SJA1105_Register_Read(count, regaddr, value_array);
            IfxStdIf_DPipe_print(io, "Read back value from SPI switch SJA1105 are"ENDL);
            DEBUG_PRINTF_BUFFER_32BIT(value_array, count);
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid parameter %s, only support \"mdio\" and \"spi\""ENDL, args);
        }
    }
    return TRUE;
}

IFX_EXTERN boolean ethswitchwrite(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 slaveaddr, devaddr, regaddr, value;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : ethswwr mdio/spi x y z v"ENDL);
        IfxStdIf_DPipe_print(io, "           > write Ethernet switch register with two format"ENDL);
        IfxStdIf_DPipe_print(io, "           > ethswwr mdio x y z v"ENDL);
        IfxStdIf_DPipe_print(io, "           > write value v to switch slave address x, device address y, register offset z by mdio"ENDL);
        IfxStdIf_DPipe_print(io, "           > ethswwr spi z v"ENDL);
        IfxStdIf_DPipe_print(io, "           > write dword value v to switch register offset z by spi"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "mdio") != FALSE)
        {
            if (Ifx_Shell_parseUInt32(&args, &slaveaddr, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid slave address %s"ENDL, args);
                return TRUE;
            }
            if (Ifx_Shell_parseUInt32(&args, &devaddr, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device address %s"ENDL, args);
                return TRUE;
            }
            if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid register address %s"ENDL, args);
                return TRUE;
            }
            if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
                return TRUE;
            }
            IfxStdIf_DPipe_print(io, "Write value 0x%x into MDIO switch slave address 0x%x, device address 0x%x, register address 0x%x"ENDL,
                                value, slaveaddr, devaddr, regaddr);
            IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, regaddr, value);
        }
        else if (Ifx_Shell_matchToken(&args, "spi") != FALSE)
        {/* Right now we only have SJA1105 using SPI, so call its interface directly */
            if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid register address %s"ENDL, args);
                return TRUE;
            }
            if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
                return TRUE;
            }
            IfxStdIf_DPipe_print(io, "Write value 0x%x into SPI switch SJA1105 register address 0x%x"ENDL,
                    value, regaddr);
            Switch_SJA1105_Register_Write(1, regaddr, &value);
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid parameter %s, only support \"mdio\" and \"spi\""ENDL, args);
        }

    }
    return TRUE;
}

/** \brief Handle the 'swpakcnt' command
 *
 * \par Syntax
 *  - swpakcnt : count packet that each port send/receive of switch
 */
IFX_EXTERN boolean swpakcnt(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 dev_id=0;
    uint32 port_id=0;
    uint32 tx_num = 0;
    uint32 rx_num = 0;
    uint32 drop_array[VALUE_MAX_BUFFER_SIZE];
    memset(drop_array, 0, VALUE_MAX_BUFFER_SIZE);

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : swpakcnt [s1/] p"ENDL);
        IfxStdIf_DPipe_print(io, "           > [s1] catch switch 1 send/receive packet count"ENDL);
        IfxStdIf_DPipe_print(io, "           > p: port number. mapping via bits (e.g. 0x7 means p0,p1,p2 on. p3, p4 off) "ENDL);
        IfxStdIf_DPipe_print(io, "           > [hint]we only display the lower bits  "ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "s1") != FALSE)
        {
            dev_id = 0x1;
        }
        else
        {
            IfxStdIf_DPipe_print(io, "query all switch as default"ENDL);
            dev_id = 0x1;//default s1
        }

        if (Ifx_Shell_parseUInt32(&args, &port_id, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "query all port as default"ENDL);
            port_id=0x1f;
        }
        if (dev_id & 0x1 )
        {//Switch 1
            if(port_id & 0x1){//Switch 1 P0
                Switch_SJA1105_Device_Register_Read(0, 1, 0x402, &tx_num);
                Switch_SJA1105_Device_Register_Read(0, 1, 0x406, &rx_num);
                Switch_SJA1105_Device_Register_Read(0, 8, 0x408, drop_array);
                IfxStdIf_DPipe_print(io, "SJA1105S Switch_1 P0[tx:%d, rx:%d], drop:", tx_num, rx_num);
                DEBUG_PRINTF_BUFFER_32BIT(drop_array, 8);//list drop packet
            }
#if 1
            if(port_id & 0x2){//Switch 1 P1
                Switch_SJA1105_Device_Register_Read(0, 1, 0x412, &tx_num);
                Switch_SJA1105_Device_Register_Read(0, 1, 0x416, &rx_num);
                Switch_SJA1105_Device_Register_Read(0, 8, 0x418, drop_array);
                IfxStdIf_DPipe_print(io, "SJA1105S Switch_1 P1[tx:%d, rx:%d], drop:", tx_num, rx_num);
                DEBUG_PRINTF_BUFFER_32BIT(drop_array, 8);//list drop packet
            }
            if(port_id & 0x4){//Switch 1 P2
                Switch_SJA1105_Device_Register_Read(0, 1, 0x422, &tx_num);
                Switch_SJA1105_Device_Register_Read(0, 1, 0x426, &rx_num);
                Switch_SJA1105_Device_Register_Read(0, 8, 0x428, drop_array);
                IfxStdIf_DPipe_print(io, "SJA1105S Switch_1 P2[tx:%d, rx:%d], drop:", tx_num, rx_num);
                DEBUG_PRINTF_BUFFER_32BIT(drop_array, 8);//list drop packet
            }
            if(port_id & 0x8){//Switch 1 P3
                Switch_SJA1105_Device_Register_Read(0, 1, 0x432, &tx_num);
                Switch_SJA1105_Device_Register_Read(0, 1, 0x436, &rx_num);
                Switch_SJA1105_Device_Register_Read(0, 8, 0x438, drop_array);
                IfxStdIf_DPipe_print(io, "SJA1105S Switch_1 P3[tx:%d, rx:%d], drop:", tx_num, rx_num);
                DEBUG_PRINTF_BUFFER_32BIT(drop_array, 8);//list drop packet
            }
            if(port_id & 0x10){//Switch 1 P4
                Switch_SJA1105_Device_Register_Read(0, 1, 0x442, &tx_num);
                Switch_SJA1105_Device_Register_Read(0, 1, 0x446, &rx_num);
                Switch_SJA1105_Device_Register_Read(0, 8, 0x448, drop_array);
                IfxStdIf_DPipe_print(io, "SJA1105S Switch_1 P4[tx:%d, rx:%d], drop:", tx_num, rx_num);
                DEBUG_PRINTF_BUFFER_32BIT(drop_array, 8);//list drop packet
            }
#endif
        }
    }
    return TRUE;
}

#undef VALUE_MAX_BUFFER_SIZE


IFX_EXTERN boolean ethswitchexposeaurix(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 slaveaddr, devaddr, regaddr, value;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : exposeaurix set/clear"ENDL);
        IfxStdIf_DPipe_print(io, "           > set/clear expose aurix"ENDL);
        IfxStdIf_DPipe_print(io, "           > exposeaurix set"ENDL);
        IfxStdIf_DPipe_print(io, "           > set expose aurix"ENDL);
        IfxStdIf_DPipe_print(io, "           > exposeaurix clear"ENDL);
        IfxStdIf_DPipe_print(io, "           > clear expose aurix"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "set") != FALSE)
        {
            IfxStdIf_DPipe_print(io, "Set ExposeAurix"ENDL);
            //Reset SJA1105_RSTN
            uint32 phy_number = 7;
            uint8 i;
            //Reset SJA1105_RSTN
            //IfxStdIf_DPipe_print(io, "Reset SJA1105_RSTN\r\n");
            IfxPort_setPinLow(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex);
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            IfxPort_setPinHigh(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex);
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            //IfxStdIf_DPipe_print(io, "Switch_SJA1105_LoadSim_Initial\r\n");
            Switch_SJA1105_ExposeAurix_Initial();
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            //IfxStdIf_DPipe_print(io,"Switch_SJA1105_Step2_Init\r\n");
            Switch_SJA1105_Step2_Init();
            //IfxStdIf_DPipe_print(io,"LWIP REINIT\r\n");
            Ethernet_Stop();
            //IfxStdIf_DPipe_print(io,"Ethernet_Stop\r\n");
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            Ifx_Lwip_deinit();
            //IfxStdIf_DPipe_print(io,"Ifx_Lwip_deinit\r\n");
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            //Reset GRP1 and GRP2
            SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
            SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
            SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
            SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
            waitTime(100*TimeConst[TIMER_INDEX_1MS]);
            SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
            SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
            // allow the chip to be ready for reading after hw reset
            waitTime(200*TimeConst[TIMER_INDEX_1MS]);
            t_ip_addr ip;
            ip.ipaddr1 = 10;
            ip.ipaddr2 = 42;
            ip.ipaddr3 = 0;
            ip.ipaddr4 = 146;
            ip.mask1 = 255;
            ip.mask2 = 255;
            ip.mask3 = 255;
            ip.mask4 = 0;
            //IfxStdIf_DPipe_print(io,"Geth Reinit\r\n");
            geth_init_loadsim(&ip);

            IfxEth_Marvell_Sw_init_only_vlan200();

            IfxEth_Marvell_Sw_ExposeAurix_Init(IFXETH_SWITCH_88E6321_1_ADDR);
            //IfxStdIf_DPipe_print(io,"IfxEth_Marvell_Sw_Loadsim_Init IFXETH_SWITCH_88E6321_2_ADDR\r\n");
            IfxEth_Marvell_Sw_ExposeAurix_Init(IFXETH_SWITCH_88E6321_2_ADDR);
            // Init 88Q2112 PHY
            IfxEth_Phy_88q2112_init(1);
            IfxEth_Phy_88q2112_init(2);
            IfxEth_Phy_88q2112_init(3);
            IfxEth_Phy_88q2112_init(4);
            IfxEth_Phy_88q2112_init(5);
            IfxEth_Phy_88q2112_init(6);
            IfxEth_Phy_88q2112_init(7);
            //Reset 5050
            uint32 val=0;
            uint32 port=1;
            // Set 88Q5050 forwarding P8 ---> P1,P2,P3,P4,P6,P7
            port = 8;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0xDE);
            // Set 88Q5050 forwarding P6 ---> P1,P2,P3,P4,P7,P8
            port = 6;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x19E);
            //  Set 88Q5050 forwarding P7 ---> P1,P2,P3,P4,P6,P8
            port = 7;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x15E);
            //  Set 88Q5050 forwarding P3 ---> P1,P2,P4,P6,P7,P8
            port = 3;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x1D6);
            //  Set 88Q5050 forwarding P4 ---> P1,P2,P3,P6,P7,P8
            port = 4;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x1CE);
            //  Set 88Q5050 forwarding P1 ---> P2,P3,P4,P6,P7,P8
            port = 1;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x1DC);
            //  Set 88Q5050 forwarding P2 ---> P1,P3,P4,P6,P7,P8
            port = 2;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x1DA);
            // Set P2Phy as Slave 0x8000
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x0834);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8041);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x8000);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8441);
            return TRUE;
        }
        else if (Ifx_Shell_matchToken(&args, "clear") != FALSE)
        {
            IfxStdIf_DPipe_print(io, "Clear ExposeAurix"ENDL);
            IfxPort_setPinLow(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex);
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            IfxPort_setPinHigh(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex);
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            Switch_SJA1105_Initial();
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            Switch_SJA1105_Step2_Init();
            //Do we need reset 88E6321?
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            Ifx_Lwip_deinit();
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
            SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
            SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
            SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
            waitTime(100*TimeConst[TIMER_INDEX_1MS]);
            SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
            SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
            // allow the chip to be ready for reading after hw reset
            waitTime(200*TimeConst[TIMER_INDEX_1MS]);
            t_ip_addr ip;
            // main Aurix 10.42.0.146
            ip.ipaddr1 = 10;
            ip.ipaddr2 = 42;
            ip.ipaddr3 = 0;
            ip.ipaddr4 = 146;
            ip.mask1 = 255;
            ip.mask2 = 255;
            ip.mask3 = 255;
            ip.mask4 = 0;
            geth_init_loadsim(&ip);
            IfxEth_Marvell_Sw_Init(IFXETH_SWITCH_88E6321_1_ADDR);
            IfxEth_Marvell_Sw_Init(IFXETH_SWITCH_88E6321_2_ADDR);
            uint32 phy_number = get_88q2112_number();
            uint8 i;
            for(i = 1; i <= phy_number; i++)
            {
                if ((i == 7) || (i == 4) || (i == 2)){
                    IfxStdIf_DPipe_print(io,"Force PHY %d set 0x0001 to slave\r\n",i);
                    IfxEth_Phy_88q2112_read_mdio_reg(i, 1, 0x0834, &value);
                    value = (value & 0xFFF0) | 0x0001;
                    IfxEth_Phy_88q2112_write_mdio_reg(i, 1, 0x0834, value);
                }
            }
            IfxEth_Phy_88q2112_init(1);
            IfxEth_Phy_88q2112_init(2);
            IfxEth_Phy_88q2112_init(3);
            IfxEth_Phy_88q2112_init(4);
            IfxEth_Phy_88q2112_init(5);
            IfxEth_Phy_88q2112_init(6);
            IfxEth_Phy_88q2112_init(7);
            //Reset 5050
            uint32 val=0;
            uint32 port=1;
            // Set 88Q5050 forwarding P8 ---> P1,P2,P3,P4,P6,P7
            port = 8;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0xDE);
            // Set 88Q5050 forwarding P6 ---> P1,P2,P3,P4,P7,P8
            port = 6;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x19E);
            //  Set 88Q5050 forwarding P7 ---> P1,P2,P3,P4,P6,P8
            port = 7;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x15E);
            //  Set 88Q5050 forwarding P3 ---> P1,P2,P4,P6,P7,P8
            port = 3;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x1D6);
            //  Set 88Q5050 forwarding P4 ---> P1,P2,P3,P6,P7,P8
            port = 4;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x1CE);
            //  Set 88Q5050 forwarding P1 ---> P2,P3,P4,P6,P7,P8
            port = 1;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x1DC);
            //  Set 88Q5050 forwarding P2 ---> P1,P3,P4,P6,P7,P8
            port = 2;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x1DA);
            // Set P2Phy as Slave 0x8000
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x0834);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8041);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x8000);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8441);
            return TRUE;
        }
        else
        {
        IfxStdIf_DPipe_print(io, "Error : invalid parameter %s, only support \"set\" and \"clear\""ENDL, args);
        }
    }
}

IFX_EXTERN boolean ethswitchloadsim(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 slaveaddr, devaddr, regaddr, value;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : loadsim set/clear"ENDL);
        IfxStdIf_DPipe_print(io, "           > set/clear vlan external loopback load simulator"ENDL);
        IfxStdIf_DPipe_print(io, "           > loadsim set"ENDL);
        IfxStdIf_DPipe_print(io, "           > set vlan external loopback load simulator"ENDL);
        IfxStdIf_DPipe_print(io, "           > loadsim clear"ENDL);
        IfxStdIf_DPipe_print(io, "           > clear vlan external loopback load simulator"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "set") != FALSE)
        {
            IfxStdIf_DPipe_print(io, "Set LoadSim"ENDL);
            uint32 phy_number = 7;
            uint8 i;
            //Reset SJA1105_RSTN
            //IfxStdIf_DPipe_print(io, "Reset SJA1105_RSTN\r\n");
            IfxPort_setPinLow(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex);
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            IfxPort_setPinHigh(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex);
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            //IfxStdIf_DPipe_print(io, "Switch_SJA1105_LoadSim_Initial\r\n");
            Switch_SJA1105_LoadSim_Initial();
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            //IfxStdIf_DPipe_print(io,"Switch_SJA1105_Step2_Init\r\n");
            Switch_SJA1105_Step2_Init();
            //IfxStdIf_DPipe_print(io,"LWIP REINIT\r\n");
            Ethernet_Stop();
            //IfxStdIf_DPipe_print(io,"Ethernet_Stop\r\n");
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            Ifx_Lwip_deinit();
            //IfxStdIf_DPipe_print(io,"Ifx_Lwip_deinit\r\n");
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            //Reset GRP1 and GRP2
            SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
            SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
            SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
            SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
            waitTime(100*TimeConst[TIMER_INDEX_1MS]);
            SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
            SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
            // allow the chip to be ready for reading after hw reset
            waitTime(200*TimeConst[TIMER_INDEX_1MS]);
            t_ip_addr ip;
            ip.ipaddr1 = 10;
            ip.ipaddr2 = 42;
            ip.ipaddr3 = 0;
            ip.ipaddr4 = 146;
            ip.mask1 = 255;
            ip.mask2 = 255;
            ip.mask3 = 255;
            ip.mask4 = 0;
            //IfxStdIf_DPipe_print(io,"Geth Reinit\r\n");
            geth_init_loadsim(&ip);

            IfxEth_Marvell_Sw_init_only_vlan200();
            //IfxStdIf_DPipe_print(io,"IfxEth_Marvell_Sw_Loadsim_Init IFXETH_SWITCH_88E6321_1_ADDR\r\n");
            IfxEth_Marvell_Sw_Loadsim_Init(IFXETH_SWITCH_88E6321_1_ADDR);
            //IfxStdIf_DPipe_print(io,"IfxEth_Marvell_Sw_Loadsim_Init IFXETH_SWITCH_88E6321_2_ADDR\r\n");
            IfxEth_Marvell_Sw_Loadsim_Init(IFXETH_SWITCH_88E6321_2_ADDR);
            // Init 88Q2112 PHY
            IfxEth_Phy_88q2112_init(1);
            IfxEth_Phy_88q2112_init(2);
            IfxEth_Phy_88q2112_init(3);
            IfxEth_Phy_88q2112_init(4);
            IfxEth_Phy_88q2112_init(5);
            IfxEth_Phy_88q2112_init(6);
            IfxEth_Phy_88q2112_init(7);
            for(i = 1; i <= phy_number; i++)
            {
                if ((i == 7) || (i == 4) || (i == 2)){
                    IfxStdIf_DPipe_print(io, "Force PHY %d set 0xC001 to master\r\n",i);
                    IfxEth_Phy_88q2112_read_mdio_reg(i, 1, 0x0834, &value);
                    value = (value & 0xFFF0) | 0xC001;
                    IfxEth_Phy_88q2112_write_mdio_reg(i, 1, 0x0834, value);
                }
            }
            //88Q5050
            //uint32 val=0;
            uint32 port=1;
            // Set 88Q5050 forwarding P8 ---> P6
            port = 8;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x40);
            // Set 88Q5050 forwarding P6 ---> P8
            port = 6;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x100);
            //  Set 88Q5050 forwarding P7 ---> P3,P4
            port = 7;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x18);
            //  Set 88Q5050 forwarding P3 ---> P7
            port = 3;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x80);
            //  Set 88Q5050 forwarding P4 ---> P7
            port = 4;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x80);
            //  Set 88Q5050 forwarding P1 ---> P2
            port = 1;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x4);
            //  Set 88Q5050 forwarding P2 ---> P1
            port = 2;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x2);
            // Set P2Phy as Master 0xC000
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0x0834);
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x18, 0x8041);
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x19, 0xC000);
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1c, 0x18, 0x8441);
            //Init B2B Phy
            IfxEth_B2BPhy_88q2112_init();
            //Ifx_Console_print("88Q5050 LOADSIM Setup\r\n");
            return TRUE;
        }
        else if (Ifx_Shell_matchToken(&args, "clear") != FALSE)
        {
            IfxStdIf_DPipe_print(io, "Clear LoadSim"ENDL);
            IfxPort_setPinLow(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex);
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            IfxPort_setPinHigh(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex);
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            Switch_SJA1105_Initial();
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            Switch_SJA1105_Step2_Init();
            //Do we need reset 88E6321?
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            Ifx_Lwip_deinit();
            waitTime(50*TimeConst[TIMER_INDEX_1MS]);
            SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
            SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
            SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
            SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
            waitTime(100*TimeConst[TIMER_INDEX_1MS]);
            SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
            SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
            // allow the chip to be ready for reading after hw reset
            waitTime(200*TimeConst[TIMER_INDEX_1MS]);
            t_ip_addr ip;
            // main Aurix 10.42.0.146
            ip.ipaddr1 = 10;
            ip.ipaddr2 = 42;
            ip.ipaddr3 = 0;
            ip.ipaddr4 = 146;
            ip.mask1 = 255;
            ip.mask2 = 255;
            ip.mask3 = 255;
            ip.mask4 = 0;
            geth_init_loadsim(&ip);
            IfxEth_Marvell_Sw_Init(IFXETH_SWITCH_88E6321_1_ADDR);
            IfxEth_Marvell_Sw_Init(IFXETH_SWITCH_88E6321_2_ADDR);
            uint32 phy_number = get_88q2112_number();
            uint8 i;
            for(i = 1; i <= phy_number; i++)
            {
                if ((i == 7) || (i == 4) || (i == 2)){
                    IfxStdIf_DPipe_print(io,"Force PHY %d set 0x0001 to slave\r\n",i);
                    IfxEth_Phy_88q2112_read_mdio_reg(i, 1, 0x0834, &value);
                    value = (value & 0xFFF0) | 0x0001;
                    IfxEth_Phy_88q2112_write_mdio_reg(i, 1, 0x0834, value);
                }
            }
            IfxEth_Phy_88q2112_init(1);
            IfxEth_Phy_88q2112_init(2);
            IfxEth_Phy_88q2112_init(3);
            IfxEth_Phy_88q2112_init(4);
            IfxEth_Phy_88q2112_init(5);
            IfxEth_Phy_88q2112_init(6);
            IfxEth_Phy_88q2112_init(7);
            //Reset 5050
            uint32 val=0;
            uint32 port=1;
            // Set 88Q5050 forwarding P8 ---> P1,P2,P3,P4,P6,P7
            port = 8;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0xDE);
            // Set 88Q5050 forwarding P6 ---> P1,P2,P3,P4,P7,P8
            port = 6;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x19E);
            //  Set 88Q5050 forwarding P7 ---> P1,P2,P3,P4,P6,P8
            port = 7;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x15E);
            //  Set 88Q5050 forwarding P3 ---> P1,P2,P4,P6,P7,P8
            port = 3;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x1D6);
            //  Set 88Q5050 forwarding P4 ---> P1,P2,P3,P6,P7,P8
            port = 4;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x1CE);
            //  Set 88Q5050 forwarding P1 ---> P2,P3,P4,P6,P7,P8
            port = 1;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x1DC);
            //  Set 88Q5050 forwarding P2 ---> P1,P3,P4,P6,P7,P8
            port = 2;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x1DA);
            // Set P2Phy as Slave 0x8000
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x0834);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8041);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x8000);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8441);
            return TRUE;
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid parameter %s, only support \"set\" and \"clear\""ENDL, args);
        }
    }
}
#endif

#if ETH_PHY_88Q2112_ENABLE == 1
/** \brief Handle the 'ethphyinit' command
 *
 * \par Syntax
 *  - ethphyinit : init various ethernet phy
 */
boolean ethphyinit(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 phyid;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : ethphyinit x "ENDL);
        IfxStdIf_DPipe_print(io, "           > Init the Etherent Phy, x means phyid: "ENDL);
            IfxStdIf_DPipe_print(io, "           > 1 - 88Q2112 Phy 1 (I15, U178, addr 1)"ENDL);
            IfxStdIf_DPipe_print(io, "           > 2 - 88Q2112 Phy 2 (I30, U181, addr 2)"ENDL);
            IfxStdIf_DPipe_print(io, "           > 3 - 88Q2112 Phy 3 (I13, U183, addr 3)"ENDL);
            IfxStdIf_DPipe_print(io, "           > 4 - 88Q2112 Phy 4 (I19, U185, addr 4)"ENDL);
            IfxStdIf_DPipe_print(io, "           > 5 - 88Q2112 Phy 5 (I37, U160, addr 5)"ENDL);
            IfxStdIf_DPipe_print(io, "           > 6 - 88Q2112 Phy 6 (I38, U162, addr 6)"ENDL);
            IfxStdIf_DPipe_print(io, "           > 7 - 88Q2112 Phy 7 (I11, U176, addr 7)"ENDL);
    }
    else
    {
        // now we get the first argument (phy id)
        if (Ifx_Shell_parseUInt32(&args, &phyid, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid Phy Id %s"ENDL, args);
            return TRUE;
        }
        if ((phyid > get_88q2112_number()) || (phyid == 0))
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid Phy Id %s"ENDL, args);
            return TRUE;
        }
        IfxEth_Phy_88q2112_init(phyid);
    }
    return TRUE;
}

/** \brief Handle the 'ethphyrd' command
 *
 * \par Syntax
 *  - ethphyrd : read Ethernet Phy Register
 */
boolean ethphyread(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 phyaddr, devaddr, regaddr,count,value, group_id;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        if(board.project == P3479)
        {
            IfxStdIf_DPipe_print(io, "Syntax     : ethphyrd [group_1/2] x y z n"ENDL);
            IfxStdIf_DPipe_print(io, "           > read Ethernet Phy address x at group_1/2, device address y offset z for n bytes"ENDL);
            IfxStdIf_DPipe_print(io, "           > group_1/2 is optional, if not given it's by default group 1"ENDL);
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Syntax     : ethphyrd x y z n"ENDL);
            IfxStdIf_DPipe_print(io, "           > read Ethernet Phy address x device address y offset z for n bytes"ENDL);
        }
    }
    else
    {

        if(board.project == P3479)
        {
            if (Ifx_Shell_matchToken(&args, "group_1") != FALSE)
            {
                group_id = 1;
            }
            else if (Ifx_Shell_matchToken(&args, "group_2") != FALSE)
            {
                group_id = 2;
            }
            else
            {
                group_id = 1;
            }
        }
        // now we get the first argument (phy address)
        if (Ifx_Shell_parseUInt32(&args, &phyaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        if (phyaddr > 69905 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        //now we get the 2nd argument ( dev address)
        if (Ifx_Shell_parseUInt32(&args, &devaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device address %s"ENDL, args);
            return TRUE;
        }

        //now we get the 3rd argument ( reg address)
        if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid offset address %s"ENDL, args);
            return TRUE;
        }


        // now we get the number of registers to read
        if (Ifx_Shell_parseUInt32(&args, &count, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid count %s"ENDL, args);
            return TRUE;
        }

        while (count)
        {
            if(board.project == P3479)
            {
                SET_GROUP(phyaddr, group_id);
                IfxStdIf_DPipe_print(io, "after binding group, the slave hex is 0x%x"ENDL, phyaddr);
            }
            IfxEth_Phy_88q2112_read_mdio_reg(phyaddr, devaddr, regaddr, &value);
            IfxStdIf_DPipe_print(io, "0x%04x"ENDL,value);
            count --;
            regaddr++;
        }

    }
    return TRUE;
}


/** \brief Handle the 'ethphywr' command
 *
 * \par Syntax
 *  - ethphywr : write value to Ethernet Phy Register
 */
boolean ethphywrite(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 phyaddr, devaddr, regaddr,count,value, group_id;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        if(board.project == P3479)
        {
            IfxStdIf_DPipe_print(io, "Syntax     : ethphywr [group_1/2] x y z v"ENDL);
            IfxStdIf_DPipe_print(io, "           > write value v to Ethernet Phy address x at group_1/2,"
                    " device address y, offset z"ENDL);
            IfxStdIf_DPipe_print(io, "           > group_1/2 is optional, if not given it's by default group 1"ENDL);
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Syntax     : ethphywr x y z v"ENDL);
            IfxStdIf_DPipe_print(io, "           > write value v to Ethernet Phy address x,"
                    " device address y, offset z"ENDL);
        }
    }
    else
    {
        if(board.project == P3479)
        {
            if (Ifx_Shell_matchToken(&args, "group_1") != FALSE)
            {
                group_id = 1;
            }
            else if (Ifx_Shell_matchToken(&args, "group_2") != FALSE)
            {
                group_id = 2;
            }
            else
            {
                group_id = 1;
            }
        }
        // now we get the first argument (phy address)
        if (Ifx_Shell_parseUInt32(&args, &phyaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        if (phyaddr > 69905 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        //now we get the 2nd argument register offset address)
        if (Ifx_Shell_parseUInt32(&args, &devaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device address %s"ENDL, args);
            return TRUE;
        }

        //now we get the 3rd argument register offset address)
        if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid offset address %s"ENDL, args);
            return TRUE;
        }

        // now we get the value to write
        if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
            return TRUE;
        }
        if(board.project == P3479)
        {
            SET_GROUP(phyaddr, group_id);
            IfxStdIf_DPipe_print(io, "after binding group, the slave hex is 0x%x"ENDL, phyaddr);
        }
        IfxEth_Phy_88q2112_write_mdio_reg(phyaddr, devaddr, regaddr, value);
    }
    return TRUE;
}

#if PROJECT == PROJECT_PAGASUS2

boolean ethphy1512read(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 phyaddr, devaddr, regaddr,count,value;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : ethphy1512rd x y z n"ENDL);
        IfxStdIf_DPipe_print(io, "           > read Ethernet Phy 1512 address x device address y offset z for n bytes"ENDL);
        IfxStdIf_DPipe_print(io, "Note: This is only applicable if you have SPC-3 RWK for 1512 phy (mdio control from Aurix)"ENDL);
    }
    else
    {
        // now we get the first argument (phy address)
        if (Ifx_Shell_parseUInt32(&args, &phyaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        if (phyaddr > 69905 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        //now we get the 2nd argument ( dev address)
        if (Ifx_Shell_parseUInt32(&args, &devaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device address %s"ENDL, args);
            return TRUE;
        }

        //now we get the 3rd argument ( reg address)
        if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid offset address %s"ENDL, args);
            return TRUE;
        }


        // now we get the number of registers to read
        if (Ifx_Shell_parseUInt32(&args, &count, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid count %s"ENDL, args);
            return TRUE;
        }

        while (count)
        {
            // Config the MDC control logic (5050 MDC)
            IfxEth_Cfg_Aurix_MDC_5050();
            // Execute mdio operation
            IfxEth_read_mdio_clause_22(phyaddr, regaddr, &value);
            IfxStdIf_DPipe_print(io, "0x%04x"ENDL,value);
            count --;
            regaddr++;
        }
    }
    return TRUE;
}


boolean ethphy1512write(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 phyaddr, devaddr, regaddr,count,value;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : ethphy1512wr x y z v"ENDL);
        IfxStdIf_DPipe_print(io, "           > write value v to Ethernet Phy address x,"
                " device address y, offset z"ENDL);
        IfxStdIf_DPipe_print(io, "Note: This is only applicable if you have SPC-3 RWK for 1512 phy (mdio control from Aurix)"ENDL);
    }
    else
    {
        // now we get the first argument (phy address)
        if (Ifx_Shell_parseUInt32(&args, &phyaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        if (phyaddr > 69905 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        //now we get the 2nd argument register offset address)
        if (Ifx_Shell_parseUInt32(&args, &devaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device address %s"ENDL, args);
            return TRUE;
        }

        //now we get the 3rd argument register offset address)
        if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid offset address %s"ENDL, args);
            return TRUE;
        }

        // now we get the value to write
        if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
            return TRUE;
        }

        // Config the MDC control logic (5050 MDC)
        IfxEth_Cfg_Aurix_MDC_5050();

        // Execute mdio operation
        IfxEth_write_mdio_clause_22(phyaddr, regaddr, value);
    }
    return TRUE;
}


boolean ethphy107read(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 phyaddr, devaddr, regaddr,count,value;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : ethphy107rd x y z n"ENDL);
        IfxStdIf_DPipe_print(io, "           > read Ethernet Phy AQR107 address x device address y offset z for n bytes"ENDL);
    }
    else
    {
        // now we get the first argument (phy address)
        if (Ifx_Shell_parseUInt32(&args, &phyaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        if (phyaddr > 69905 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        //now we get the 2nd argument ( dev address)
        if (Ifx_Shell_parseUInt32(&args, &devaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device address %s"ENDL, args);
            return TRUE;
        }

        //now we get the 3rd argument ( reg address)
        if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid offset address %s"ENDL, args);
            return TRUE;
        }


        // now we get the number of registers to read
        if (Ifx_Shell_parseUInt32(&args, &count, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid count %s"ENDL, args);
            return TRUE;
        }
        IfxEth_mdio_set_simulating(TRUE);
        while (count)
        {
            // Config the MDC control logic
            IfxEth_Cfg_Aurix_MDC_107();
            // Execute mdio operation
            IfxEth_read_mdio_clause_45(phyaddr, devaddr, regaddr, &value);
            IfxStdIf_DPipe_print(io, "0x%04x"ENDL,value);
            count --;
            regaddr++;
        }
        IfxEth_mdio_set_simulating(FALSE);
    }
    return TRUE;
}


boolean ethphy107write(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 phyaddr, devaddr, regaddr,count,value;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : ethphy107wr x y z v"ENDL);
        IfxStdIf_DPipe_print(io, "           > write value v to Ethernet Phy AQR 107 address x,"
                " device address y, offset z"ENDL);
    }
    else
    {
        // now we get the first argument (phy address)
        if (Ifx_Shell_parseUInt32(&args, &phyaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        if (phyaddr > 69905 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid phy address %s"ENDL, args);
            return TRUE;
        }
        //now we get the 2nd argument register offset address)
        if (Ifx_Shell_parseUInt32(&args, &devaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device address %s"ENDL, args);
            return TRUE;
        }

        //now we get the 3rd argument register offset address)
        if (Ifx_Shell_parseUInt32(&args, &regaddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid offset address %s"ENDL, args);
            return TRUE;
        }

        // now we get the value to write
        if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
            return TRUE;
        }

        IfxEth_mdio_set_simulating(TRUE);
        // Config the MDC control logic (5050 MDC)
        IfxEth_Cfg_Aurix_MDC_107();

        // Execute mdio operation
        IfxEth_write_mdio_clause_45(phyaddr, devaddr, regaddr, value);
        IfxEth_mdio_set_simulating(FALSE);
    }
    return TRUE;
}


IFX_EXTERN boolean nonVlanConfig(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : nonvlanconfig"ENDL);
        IfxStdIf_DPipe_print(io, "           > Enable non VLAN tegra-Aurix communication and disable VLAN#200 on aurix port"ENDL);
    }
    else
    {
        AsclinShellInterface_execute_cmd("ethswwr", "spi 0x53 0x80000000");
        AsclinShellInterface_execute_cmd("ethswwr", "spi 0x4c 0xfe08ac03");
        AsclinShellInterface_execute_cmd("ethswwr", "spi 0x53 0xA0000000");
        IfxStdIf_DPipe_print(io, "Enabled non VLAN tegra-Aurix communication and Disabled VLAN#200 on aurix port"ENDL);
        IfxStdIf_DPipe_print(io, "Note: This is a transient setting and reboot will wipe-out this setting"ENDL);

    }
    return TRUE;
}
#endif //#if PROJECT == PROJECT_DRIVE_AX

#endif //#if ETH_PHY_88Q2112_ENABLE == 1
