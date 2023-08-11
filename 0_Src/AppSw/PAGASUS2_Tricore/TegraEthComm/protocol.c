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
#include "protocol.h"
#include "Configuration.h"
#include <internal_fw_debug.h>

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define PROTOCOL_DEBUG_ENABLE 1

#if PROTOCOL_DEBUG_ENABLE == 1
#define PROTOCOL_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define PROTOCOL_PRINTF(x)
#endif

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
static void initialize_default_packet(tPacket* cmd);
static void prepare_packet_checksum(tPacket* cmd);
static boolean verify_packet_checksum(tPacket* cmd);

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/


/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
#if 1
static void initialize_default_packet(tPacket* cmd)
{
    //cmd->head = SUPPORT_PROTOCOL_VERSION;
    cmd->cmd = 0xf;
    cmd->cam_pwr = 0xff;
    if ((cmd->head == PROTOCOL_VERSION_V06)||(cmd->head == PROTOCOL_VERSION_V07))
    {
        cmd->v6.cam_cmd = 0xf;
        cmd->v6.cam_grp_id = 0xf;

        cmd->v6.cam_units = 0xf;
        cmd->v6.cam_agg_frsync_en = 0x3;
        cmd->v6.cam_frsync_owner = CAM_AGG_FRSYNC_SRC_IGNORE;

        cmd->v6.power_status_0 = 0x0;
        cmd->v6.power_status_1 = 0x0;
        cmd->v6.aggreg_fsync_status = 0x0;

        cmd->v6.message_return_status = 0xf;
        cmd->v6.reserved = 0xf;

    }
    else if (cmd->head == PROTOCOL_VERSION_V04)
    {
        cmd->v4.cam_gmsl = 0xff;
        cmd->v4.tegra_gmsl = 0xff;
        cmd->v4.tegra_fpdl = 0xff;
        cmd->v4.tegra_a_hdmi = 0xff;
        cmd->v4.tegra_b_hdmi = 0xff;
        cmd->v4.tegra_pci = 0xff;
    }
    else if(cmd->head == PROTOCOL_VERSION_V05)
    {
        cmd->v5.cam_units = 0xffff;
        cmd->v5.cam_agg_pwdn = 0xff;
        cmd->v5.cam_frsync_owner = 0xff;
        cmd->v5.cam_agg_frsync_en = 0xff;
        cmd->v5.cam_agg_frsync_group = 0xff;
    }
    cmd->aurix_version = 0xff;
    cmd->fan_state = 0xff;
    cmd->cam_master = 0xff;
}
#endif

static void prepare_packet_checksum(tPacket* cmd)
{
    if (cmd->head == PROTOCOL_VERSION_V04)
    {
        cmd->checksum = (~(0xFF & (cmd->cam_pwr + cmd->v4.cam_gmsl + cmd->v4.tegra_gmsl +
            cmd->v4.tegra_fpdl + cmd->v4.tegra_a_hdmi + cmd->v4.tegra_b_hdmi + cmd->v4.tegra_pci +
            cmd->aurix_version + cmd->fan_state + cmd->cam_master)) + 1);
    }
    else if (cmd->head == PROTOCOL_VERSION_V05)
    {
        cmd->checksum = (~(0xFF & (cmd->cam_pwr + (cmd->v5.cam_units & 0xFF) + ((cmd->v5.cam_units>>8) & 0xFF)+ cmd->v5.cam_agg_pwdn +
            cmd->v5.cam_frsync_owner + cmd->v5.cam_agg_frsync_en + cmd->v5.cam_agg_frsync_group +
            cmd->aurix_version + cmd->fan_state + cmd->cam_master)) + 1);
    }
    else if ((cmd->head == PROTOCOL_VERSION_V06)||(cmd->head == PROTOCOL_VERSION_V07))
    {
        cmd->checksum = (~(0xFF & (cmd->cam_pwr + (cmd->v5.cam_units & 0xFF) + ((cmd->v5.cam_units>>8) & 0xFF)+ cmd->v5.cam_agg_pwdn +
            cmd->v5.cam_frsync_owner + cmd->v5.cam_agg_frsync_en + cmd->v5.cam_agg_frsync_group +
            cmd->aurix_version + cmd->fan_state + cmd->cam_master)) + 1);
    }
    else
    {
        PROTOCOL_PRINTF(("protocol version error: cmd->head =%x\n\r", cmd->head));

        return;
    }
    return;
}

#if 1
static boolean verify_packet_checksum(tPacket* cmd)
{
    unsigned int cal_checksum;

    if (cmd->head == PROTOCOL_VERSION_V04)
    {
        cal_checksum = (~(0xFF & (cmd->cam_pwr + cmd->v4.cam_gmsl + cmd->v4.tegra_gmsl +
            cmd->v4.tegra_fpdl + cmd->v4.tegra_a_hdmi + cmd->v4.tegra_b_hdmi + cmd->v4.tegra_pci +
            cmd->aurix_version + cmd->fan_state + cmd->cam_master)) + 1);
    }
    else if (cmd->head == PROTOCOL_VERSION_V05)
    {
        cal_checksum = (~(0xFF & (cmd->cam_pwr + (cmd->v5.cam_units & 0xFF) + ((cmd->v5.cam_units>>8) & 0xFF)+ cmd->v5.cam_agg_pwdn +
            cmd->v5.cam_frsync_owner + cmd->v5.cam_agg_frsync_en + cmd->v5.cam_agg_frsync_group +
            cmd->aurix_version + cmd->fan_state + cmd->cam_master)) + 1);
    }
    else if ((cmd->head == PROTOCOL_VERSION_V06)||(cmd->head == PROTOCOL_VERSION_V07))
    {
        cal_checksum = (~(0xFF & (cmd->cam_pwr + (cmd->v5.cam_units & 0xFF) + ((cmd->v5.cam_units>>8) & 0xFF)+ cmd->v5.cam_agg_pwdn +
            cmd->v5.cam_frsync_owner + cmd->v5.cam_agg_frsync_en + cmd->v5.cam_agg_frsync_group +
            cmd->aurix_version + cmd->fan_state + cmd->cam_master)) + 1);
    }
    else
    {
        PROTOCOL_PRINTF(("protocol version error: cmd->head =%x\n\r", cmd->head));
        return FALSE;
    }

    PROTOCOL_PRINTF(("cal_checksum =%x  cmd->checksum =%x\n\r", (cal_checksum & 0xFF), cmd->checksum));
    if ((cal_checksum & 0xFF) == cmd->checksum)
        return TRUE;
    else
        return FALSE;
}


boolean Protocol_GetPackage(uint8* data, uint8 size, tPacket* packet)
{
    boolean ret = FALSE;
    if(data != NULL)
    {
        if(size >= sizeof(tPacket))
        {
            memcpy(packet, data, sizeof(tPacket));
            //packet = (tPacket*)data;
            if(verify_packet_checksum(packet))
            {
                ret = TRUE;
                PROTOCOL_PRINTF(("Protocol: packet is received\n\r"));
            }
            else
            {
                PROTOCOL_PRINTF(("Protocol: Checksum fail\n\r"));
            }
        }
        else
        {
            PROTOCOL_PRINTF(("Protocol: size %d is not enough for packet\n\r", size));
        }
    }
    return ret;
}


boolean Protocol_PreparePackage(tPacket* packet)
{
    boolean ret = TRUE;
    prepare_packet_checksum(packet);
    return ret;
}


void Protocol_InitialPacket(tPacket* packet)
{
    initialize_default_packet(packet);
}


void dump_packet(tPacket* cmd)
{
    PROTOCOL_PRINTF(("packet is\n\r"));
    PROTOCOL_PRINTF(("cmd->head = %x\n\r", cmd->head));
    PROTOCOL_PRINTF(("cmd->cmd = %x\n\r", cmd->cmd));
    PROTOCOL_PRINTF(("cmd->cam_pwr= %x\n\r", cmd->cam_pwr));
    if (cmd->head == PROTOCOL_VERSION_V04)
    {
        PROTOCOL_PRINTF(("cmd->v4.cam_gmsl= %x\n\r", cmd->v4.cam_gmsl));
        PROTOCOL_PRINTF(("cmd->v4.tegra_gmsl = %x\n\r", cmd->v4.tegra_gmsl));
        PROTOCOL_PRINTF(("cmd->v4.tegra_fpdl = %x\n\r",  cmd->v4.tegra_fpdl));
        PROTOCOL_PRINTF(("cmd->v4.tegra_a_hdmi = %x\n\r", cmd->v4.tegra_a_hdmi));
        PROTOCOL_PRINTF(("cmd->v4.tegra_b_hdmi = %x\n\r", cmd->v4.tegra_b_hdmi));
        PROTOCOL_PRINTF(("cmd->v4.tegra_pci = %x\n\r", cmd->v4.tegra_pci));
    }
    else if (cmd->head == PROTOCOL_VERSION_V05)
    {
        PROTOCOL_PRINTF(("cmd->v5.cam_units= %x\n\r", cmd->v5.cam_units));
        PROTOCOL_PRINTF(("cmd->v5.cam_agg_pwdn = %x\n\r", cmd->v5.cam_agg_pwdn));
        PROTOCOL_PRINTF(("cmd->v5.cam_frsync_owner = %x\n\r",  cmd->v5.cam_frsync_owner));
        PROTOCOL_PRINTF(("cmd->v5.cam_agg_frsync_en = %x\n\r", cmd->v5.cam_agg_frsync_en));
        PROTOCOL_PRINTF(("cmd->v5.cam_agg_frsync_group = %x\n\r", cmd->v5.cam_agg_frsync_group));
    }
    else
    {
        PROTOCOL_PRINTF(("cmd->v6.cam_cmd= %x\n\r", cmd->v6.cam_cmd));
        PROTOCOL_PRINTF(("cmd->v6.cam_grp_id = %x\n\r", cmd->v6.cam_grp_id));
        PROTOCOL_PRINTF(("cmd->v6.cam_units = %x\n\r",  cmd->v6.cam_units));
        PROTOCOL_PRINTF(("cmd->v6.cam_agg_frsync_en = %x\n\r", cmd->v6.cam_agg_frsync_en));
        PROTOCOL_PRINTF(("cmd->v6.cam_frsync_owner = %x\n\r", cmd->v6.cam_frsync_owner));
        PROTOCOL_PRINTF(("cmd->cam_master = %x\n\r", cmd->cam_master));

    }
    PROTOCOL_PRINTF(("cmd->aurix_version = %x\n\r", cmd->aurix_version));
    PROTOCOL_PRINTF(("cmd->fan_state = %x\n\r", cmd->fan_state));
    PROTOCOL_PRINTF(("cmd->cam_master = %x\n\r", cmd->cam_master));
}
#endif

#if 0

int main(){
    int udpSocket, nBytes, ret;
    struct sockaddr_in serverAddr, clientAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size, client_addr_size;
    tPacket *cmd_packet;
    int i;
    printf("SERVER:\n\r");
    cmd_packet = (tPacket*) malloc(sizeof(tPacket));
    /*Create UDP socket*/
    udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr("12.0.0.10");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*Bind socket with address struct*/
    if (bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr))) {
        printf("bind failed\n\r");
        return 0;
    }

    /*Initialize size variable to be used later on*/
    addr_size = sizeof serverStorage;

    while(1){
        /* Try to receive any incoming UDP datagram. Address and port of
           requesting client will be stored on serverStorage variable */
        nBytes = recvfrom(udpSocket, cmd_packet, 1024, 0,(struct sockaddr *)&serverStorage, &addr_size);

        ret = verify_packet_checksum(cmd_packet);
        if (ret)
            printf("SERVER: Checksum verification fails for recieved packet\n\r");
        else {
            printf("SERVER: parse request\n\r");
#if debug
            dump_packet(cmd_packet);
#endif
            parse_request(cmd_packet);
        }
        prepare_response(cmd_packet);

        /*Send response*/
        sendto(udpSocket, cmd_packet, nBytes, 0, (struct sockaddr *)&serverStorage,addr_size);
    }

    return 0;
}

#endif
