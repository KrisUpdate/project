/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef __PROTOCOL_COMMON_H_
#define __PROTOCOL_COMMON_H_

#include "Configuration.h"

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#if ((PROJECT == PROJECT_PAGASUS2) && (PG2_PREEVT == 1))
#define SUPPORT_PROTOCOL_VERSION    0x7
#else
#define SUPPORT_PROTOCOL_VERSION    0x4
#endif

#define PROTOCOL_VERSION_V07        0x7
#define PROTOCOL_VERSION_V06        0x6
#define PROTOCOL_VERSION_V05        0x5
#define PROTOCOL_VERSION_V04        0x4


/* cmd nibble */
#define CMD_PING            0x1
#define CMD_ACK             0x2
#define CMD_REQ_GET             0x3
#define CMD_REQ_SET         0x4
#define CMD_RES_ACCEPTED        0x7   //Command accepted
#define CMD_RES_FAILED          0x8   //Command failed, previous rejected
#define CMD_RES_DENIED          0x9   //Command denied
#define CMD_RES_INVALID         0xA   //Command invalid

/*ONLY FOR SPI*/
/*************************************/
#define CMD_STATE           0X05
#define CMD_ERROR           0X06
/*************************************/

/*  cam_pwr nibble */
#define CAM_ON              0x1
#define CAM_OFF             0x2

/*  cam_master nibble */
#define CAM_MASTER          0x01
#define CAM_SLAVE           0x02

/*  cam_gmsl or cam_agg_group or frsync_agg */

#define GMSL_A              0x01
#define GMSL_B              0x02
#define GMSL_C              0x04
#define GMSL_D              0x08
#if CAMERA_A_ENABLE && CAMERA_B_ENABLE && CAMERA_C_ENABLE && !CAMERA_D_ENABLE
#define GMSL_ALL            0x07
#elif CAMERA_A_ENABLE && CAMERA_B_ENABLE && CAMERA_C_ENABLE && CAMERA_D_ENABLE
#define GMSL_ALL            0x0F
#endif

/* V0.5: cam_id */
#define CAM_A0      0x01
#define CAM_A1      0x02
#define CAM_A2      0x04
#define CAM_A3      0x08
#define CAM_B0      0x10
#define CAM_B1      0x20
#define CAM_B2      0x40
#define CAM_B3      0x80
#define CAM_C0      0x0100
#define CAM_C1      0x0200
#define CAM_C2      0x0400
#define CAM_C3      0x0800
#define CAM_D0      0x1000
#define CAM_D1      0x2000
#define CAM_D2      0x4000
#define CAM_D3      0x8000
#define CAM_ALL     0xFFFF

/* V0.4: tegra_gmsl, tegra_fpdl, tegra_pci */
/* V0.5: frsync_owner */
#define TEGRA_A             0x41
#define TEGRA_B             0x42
#define TEGRA_C             0x43
#define TEGRA_INVLD         0x44

/* V0.4: tegra_a_hdmi, tegra_b_hdmi */
#define TEGRA               0x01
#define MXM                 0x02

/* V0.5: frsync_en */
#define FRSYNC_ENABLE        0x01
#define FRSYNC_DISABLE       0x02

/* FAN STATE */
#define GOOD_STATE          0x00
#define BAD_STATE           0x01
#define INGORE              0XFF


/* V0.6: cam_cmd */
#define REQ_MASTER     0x1
#define REL_MASTER     0x2
#define SET_POWER_ON   0x3
#define SET_POWER_OFF  0x4
#define SET_FSYNC_OWN  0x5
#define SET_FSYNC_EN   0x6
#define SET_FSYNC_DIS  0x7
#define GET_STATUS     0x8
#define REQ_SLAVE      0x9
#define REL_SLAVE      0xA

/* V0.6: cam_group_id */
#define GROUP_ID_A              0x00
#define GROUP_ID_B              0x01
#define GROUP_ID_C              0x02
#define GROUP_ID_D              0x03

/* V0.6: cam_units */
#define CAM_ID_0                0x01
#define CAM_ID_1                0x02
#define CAM_ID_2                0x04
#define CAM_ID_3                0x08

/* V0.6: cam_agg_frsync_en */
#define CAM_AGG_FRSYNC_ENABLE    0x1
#define CAM_AGG_FRSYNC_DISABLE   0x2
#define CAM_AGG_FRSYNC_IGNORE    0x0

/* V0.6: cam_frsync_owner */
#define CAM_AGG_FRSYNC_SRC_TEGRA_A    0x1
#define CAM_AGG_FRSYNC_SRC_TEGRA_B    0x2
#define CAM_AGG_FRSYNC_SRC_TEGRA_C    0x3
#define CAM_AGG_FRSYNC_SRC_IGNORE     0x0

/* V0.6: message_return_status */
typedef enum {CCP_STATUS_OK = 0x0, CCP_STATUS_DENIED, CCP_STATUS_NOT_REGISTERED, CCP_STATUS_ALREADY_ON, CCP_STATUS_ALREADY_OFF, CCP_STATUS_SLAVE_RUNNING, CCP_STATUS_I2C_OWNERSHIP_DENIED}ccp_return_status ;




/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

typedef struct packet
{
    unsigned int head:4;            // protocl version minor no
    unsigned int cmd:4;         // CMD
    unsigned int cam_pwr:8;         // CAM ON/OFF
    union {
        struct protocol_v4 {
            unsigned int cam_gmsl:8;        // GMSL A/B/C/D or CAM_SEL A/B/C/D
            unsigned int tegra_gmsl:8;      // gmsl disaply owner
            unsigned int tegra_fpdl:8;      // FPDL disaply owner
            unsigned int tegra_a_hdmi:8;        // V0.4 HDMI A owner
            unsigned int tegra_b_hdmi:8;        // HDMI B owner
            unsigned int tegra_pci:8;       // PEX clk/rst/i2c owner
        }__attribute__((packed)) v4;
        struct protocol_v5 {
            unsigned int cam_units:16;      // CAM_A ~ CAM_P
            unsigned int cam_agg_pwdn:8;        // Camera aggregator pwdn A/B/C/D
            unsigned int cam_frsync_owner:8;    // Camera FRSYNC_SRC
            unsigned int cam_agg_frsync_en:8;   // Camera FRSYNC_EN
            unsigned int cam_agg_frsync_group:8;    // Camera FRSYNC_SEL
        }__attribute__((packed)) v5;
        struct protocol_v6 {
            unsigned int cam_cmd:4;            // Commands:    0x1 - Req_Master, 0x2 - Rel_Master, 0x3 - Set_Power_On, 0x4 - Set_Power_Off, 0x5 -  Set_Fsync_Owner, 0x6 -  Set_Fsync_Enable,  0x7 -  Set_Fsync_Disable,  0x8 - Get_pwr_fsync_status
                                               // Commands:    0x9 - Req_Slave , 0xA - Rel_Slave
            unsigned int cam_grp_id:4;        // Camera Aggregator Group A/B/C/D  - Not a BitMask;  0x0 -A; 0x1 -B; 0x2 -C; 0x3 -D; 0xF - IGNORE
                                               // This field would be used for Master, Camera/Aggre Power and also FSYNC ENABLE

            unsigned int cam_units:4;          // Is a Bitmask within the Aggregator group
            unsigned int cam_agg_frsync_en:2;  // Camera FRSYNC_EN  : 0x3 - IGNORE,  0x1 - ON,      0x2 - OFF
            unsigned int cam_frsync_owner:2;   // Camera FRSYNC_SRC : 0x1 = TEGRA_A, 0x2 = TEGRA_B, 0x3 = TEGRA_C, 0x0 = IGNORE


            unsigned int power_status_0:8; //Camera#7-Camera#0(LSB)
            unsigned int power_status_1:8; //Camera#15-Camera#8(LSB)
            unsigned int aggreg_fsync_status:8; //bit7-bit4 : agrreg power status; bit3-bit0(LSB): fsync_enable status

            unsigned int message_return_status:4; //      CCP_STATUS_OK = 0,  CCP_STATUS_DENIED, CCP_STATUS_ALREADY_ON, CCP_STATUS_ALREADY_OFF, CCP_STATUS_SLAVE_RUNNING,
            unsigned int reserved:4; // Reserved for now
        }__attribute__((packed)) v6;
    };
    unsigned int aurix_version:8;       // Aurix FW version
    unsigned int fan_state:8;       // FAN state
    unsigned int cam_master:8;      // reserved >> cam_master  >> can use this for TegraId reading the DT then send 0x41 - TegraA and 0x42 - TegraB
    unsigned int checksum:8;        // checksum
}__attribute__((packed)) tPacket;

#endif  // __PROTOCOL_H_
