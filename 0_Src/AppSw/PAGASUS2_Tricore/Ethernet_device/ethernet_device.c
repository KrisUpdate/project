/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#include "./phy/phy.h"
#include "./switch/switch.h"
#include "ethernet_device.h"
#include "./phy/Phy_88q2112.h"
#include "./switch/marvell_switch.h"
#include <Configuration.h>
#include <internal_fw_debug.h>

#define ETHERNET_DEVICE_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)

typedef struct
{
uint32 phy_address;
uint32 switch_address;
uint32 switch_port_id;
uint32 speed;
e_role role;
char name[7];
}tHSDChannel;

static const tHSDChannel HSD_CHANNEL_CONFIG[]=
{
        {/* J11 */
            .phy_address = 5,
            .switch_address = 0x11                                       ,
            .switch_port_id = 8,
            .speed = 100,
            .role = HSD_MASTER,
            .name = "J11 A"
        },
        {/* J21 */
            .phy_address = 4,
            .switch_address = 0x12,
            .switch_port_id = 5,
            .speed = 100,
            .role = HSD_MASTER,
            .name = "J21 A"
        },
        {/* J21 */
            .phy_address = 6,
            .switch_address = 0x11,
            .switch_port_id = 6,
            .speed = 100,
            .role = HSD_MASTER,
            .name = "J21 B"
        },
        {/* J14 */
            .phy_address = 7,
            .switch_address = 0x12,
            .switch_port_id = 2,
            .speed = 100,
            .role = HSD_MASTER,
            .name = "J14 A"
        },
        {/* J14 */
            .phy_address = 3,
            .switch_address = 0x12,
            .switch_port_id = 6,
            .speed = 1000,
            .role = HSD_MASTER,
            .name = "J14 B"
        },
        {/* J12 */
            .phy_address = 2,
            .switch_address = 0x12,
            .switch_port_id = 0,
            .speed = 100,
            .role = HSD_MASTER,
            .name = "J12 A"
        },
        {/* J12 */
            .phy_address = 1,
            .switch_address = 0x12,
            .switch_port_id = 1,
            .speed = 100,
            .role = HSD_MASTER,
            .name = "J12 B"
        },

};

static void Ethernet_hsd_channel_config(void)
{
    uint32 i=0;
    uint8 ubTmp;

    for(i=0; i<(sizeof(HSD_CHANNEL_CONFIG)/sizeof(tHSDChannel)); i++)
    {
        IfxEth_Marvell_Sw_Set(HSD_CHANNEL_CONFIG[i].switch_address, HSD_CHANNEL_CONFIG[i].switch_port_id,
                HSD_CHANNEL_CONFIG[i].speed);

        ubTmp= Chk_InfoROM_88q2112_Mode_by_PHYID(i + 1);  // Read Legacy / Compliance mode setting
        IfxEth_Phy_88q2112_Set(HSD_CHANNEL_CONFIG[i].phy_address, HSD_CHANNEL_CONFIG[i].speed,
                HSD_CHANNEL_CONFIG[i].role, ubTmp);

        ETHERNET_DEVICE_PRINTF(("Initial channel %s: speed %d, HSD Role: %s\r\n",
                HSD_CHANNEL_CONFIG[i].name, HSD_CHANNEL_CONFIG[i].speed,
                (HSD_CHANNEL_CONFIG[i].role == HSD_MASTER ? "Master" : "Slave")));
    }
    ETHERNET_DEVICE_PRINTF(("Initial channel J9, J10, J11-B: speed 100, HSD Role: Master\r\n"));
    /* Extra config on switch level */
    IfxEth_Marvell_Hyperion_ExtraInit();
}


void Ethernet_Devices_Initial(uint32 devices)
{
#if ETH_SWITCH_ENABLE == 1
    Ethernet_Switch_Initial(devices);
#endif

#if ETH_PHY_ENABLE == 1
    Ethernet_Phy_Initial(devices);
#endif

    if((devices & default_config_init) != 0)
    {
        /* Initial HSD channel speed and role */
        Ethernet_hsd_channel_config();
    }
}

void Ethernet_Devices_DeInitial(uint32 devices)
{
#if ETH_PHY_ENABLE == 1
    Ethernet_Phy_DeInitial(devices);
#endif
    IfxEth_Cfg_Aurix_MDC_Reset();
}
