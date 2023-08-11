#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/********************************************************************************
 * * msdDrvSwRegs.h
 * *
 * * DESCRIPTION:
 * *       definitions of the register map of Switch Device
 * *
 * * DEPENDENCIES:
 * *
 * * FILE REVISION NUMBER:
 * *
 * *******************************************************************************/
#ifndef __Willow_msdDrvSwRegs_H
#define __Willow_msdDrvSwRegs_H
#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/*																			*/
/* Block: Switch Registers													*/
/*																			*/
/****************************************************************************/

/* Define Switch SMI Device Address */
#define WILLOW_PORT_START_ADDR		0x0	    /* Start Port Address */
#define WILLOW_GLOBAL1_DEV_ADDR	0x1B	/* Global 1 Device Address */
#define WILLOW_GLOBAL2_DEV_ADDR	0x1C	/* Global 2 Device Address */
#define WILLOW_TCAM_DEV_ADDR		0x1F	/* TCAM Device Address */

/* Switch Per Port Registers */
#define WILLOW_QD_REG_PORT_STATUS          0x0	    /* Port Status Register */
#define WILLOW_QD_REG_PHY_CONTROL          0x1     /* Physical Control Register */
#define WILLOW_QD_REG_LIMIT_PAUSE_CONTROL  0x2     /* Flow Control Register */
#define WILLOW_QD_REG_SWITCH_ID            0x3		/* Switch Identifier Register */
#define WILLOW_QD_REG_PORT_CONTROL         0x4		/* Port Control Register */
#define WILLOW_QD_REG_PORT_CONTROL1        0x5		/* Port Control 1 Register */
#define WILLOW_QD_REG_PORT_VLAN_MAP        0x6		/* Port based VLAN Map Register */
#define WILLOW_QD_REG_PVID                 0x7		/* Port VLAN ID & Priority Register */
#define WILLOW_QD_REG_PORT_CONTROL2        0x8		/* Port Control 2 Register */
#define WILLOW_QD_REG_EGRESS_RATE_CTRL     0x9		/* Egress Rate Control Register */
#define WILLOW_QD_REG_EGRESS_RATE_CTRL2    0xA		/* Egress Rate Control 2 Register */
#define WILLOW_QD_REG_PAV                  0xB		/* Port Association Vector Register */
#define WILLOW_QD_REG_PORT_ATU_CONTROL     0xC		/* Port ATU Control Register */
#define WILLOW_QD_REG_PRI_OVERRIDE         0xD		/* Override Register */
#define WILLOW_QD_REG_POLICY_CONTROL       0xE		/* Policy Control Register */
#define WILLOW_QD_REG_PORT_ETH_TYPE        0xF		/* Port E Type Register */
#define WILLOW_QD_REG_LED_CONTROL          0x16	/* LED Control Register */
#define WILLOW_QD_REG_IP_PRI_MAPPING_TBL   0x17	/* IP Priority Mapping Table Register */
#define WILLOW_QD_REG_IEEE_PRI_MAPPING_TBL 0x18	/* IEEE Priority Mapping Table Register */
#define WILLOW_QD_REG_PORT_CONTROL3		0x19	/* Port Control 3 Register */
#define WILLOW_QD_REG_Q_COUNTER           0x1B		/* Queue Counter Register */
#define WILLOW_QD_REG_Q_CONTROL           0x1C		/* Port Queue Control Register */
#define WILLOW_QD_REG_CT_CONTROL          0x1E		/* Cut Through Control Register */
#define WILLOW_QD_REG_DBG_COUNTER         0x1F		/* Debug Counters Register */

/* Switch Global Registers */
#define WILLOW_QD_REG_GLOBAL_STATUS        0x0		/* Global Status Register */
#define WILLOW_QD_REG_ATU_FID_REG        	0x1		/* ATU FID Register */
#define WILLOW_QD_REG_VTU_FID_REG        	0x2		/* VTU FID Register */
#define WILLOW_QD_REG_STU_SID_REG        	0x3		/* VTU SID Register */
#define WILLOW_QD_REG_GLOBAL_CONTROL       0x4		/* Global Control Register */
#define WILLOW_QD_REG_VTU_OPERATION      	0x5		/* VTU Operation Register */
#define WILLOW_QD_REG_VTU_VID_REG        	0x6		/* VTU VID Register */
#define WILLOW_QD_REG_VTU_DATA1_REG      	0x7		/* VTU/STU Data Register Ports 0 to 3 */
#define WILLOW_QD_REG_VTU_DATA2_REG      	0x8		/* VTU/STU Data Register Ports 4 to 7 */
#define WILLOW_QD_REG_VTU_DATA3_REG      	0x9		/* VTU/STU Data Register Ports 8 and Priority */
#define WILLOW_QD_REG_ATU_CTRL_REG        	0xA		/* ATU Control Register */
#define WILLOW_QD_REG_ATU_OPERATION      	0xB		/* ATU Operation Register */
#define WILLOW_QD_REG_ATU_DATA_REG       	0xC		/* ATU Data Register */
#define WILLOW_QD_REG_ATU_MAC_BASE       	0xD		/* ATU MAC Address Register Base */
#define WILLOW_QD_REG_MONITOR_MGMT_CTRL    0x1A    /* Monitor & MGMT Control Register */
#define WILLOW_QD_REG_TOTAL_FREE_COUNTER   0x1B    /* Total Free Counter Register */
#define WILLOW_QD_REG_GLOBAL_CONTROL2      0x1C    /* Global Control 2 Register */
#define WILLOW_QD_REG_STATS_OPERATION    	0x1D    /* Stats Operation Register */
#define WILLOW_QD_REG_STATS_COUNTER3_2   	0x1E    /* Stats Counter Register Bytes 3 & 2 */
#define WILLOW_QD_REG_STATS_COUNTER1_0   	0x1F    /* Stats Counter Register Bytes 1 & 0 */

/* Switch Global 2 Registers */
#define WILLOW_QD_REG_INT_SOURCE     		0x0		/* Interrupt Source Register */
#define WILLOW_QD_REG_INT_MASK       		0x1		/* Interrupt Mask Register */
#define WILLOW_QD_REG_FLOWCTRL_DELAY    	0x4		/* Flow Control Delay Register */
#define WILLOW_QD_REG_MANAGEMENT        	0x5		/* Switch Management Register */
#define WILLOW_QD_REG_ROUTING_TBL       	0x6		/* Device Mapping Table Register */
#define WILLOW_QD_REG_TRUNK_MASK_TBL    	0x7		/* Trunk Mask Table Register */
#define WILLOW_QD_REG_TRUNK_ROUTING     	0x8		/* Trunk Mapping Register */	
#define WILLOW_QD_REG_IGR_RATE_COMMAND     0x9		/* Ingress Rate Command Register */	
#define WILLOW_QD_REG_IGR_RATE_DATA   		0xA		/* Ingress Rate Data Register */	
#define WILLOW_QD_REG_PVT_ADDR            	0xB		/* Cross Chip Port VLAN Addr Register */	
#define WILLOW_QD_REG_PVT_DATA            	0xC		/* Cross Chip Port VLAN Data Register */	
#define WILLOW_QD_REG_SWITCH_MAC          	0xD		/* Switch MAC/WoL/WoF Register */	
#define WILLOW_QD_REG_ATU_STATS           	0xE		/* ATU Stats Register */	
#define WILLOW_QD_REG_PRIORITY_OVERRIDE 	0xF		/* Priority Override Table Register */	
#define WILLOW_QD_REG_ENERGE_MNT        	0x12	/* Energy Management Register */
#define WILLOW_QD_REG_IMP_COMM_DBG			0x13	/* IMP Comm/Debug Register */
#define WILLOW_QD_REG_EEPROM_COMMAND    	0x14	/* EEPROM Command Register */	
#define WILLOW_QD_REG_EEPROM_DATA      	0x15	/* EEPROM Address Register */	
#define WILLOW_QD_REG_AVB_COMMAND     	  	0x16	/* AVB Command Register */	
#define WILLOW_QD_REG_AVB_DATA          	0x17	/* AVB Data Register */
#define WILLOW_QD_REG_SMI_PHY_CMD       	0x18	/* SMI PHY Command Register */	
#define WILLOW_QD_REG_SMI_PHY_DATA      	0x19	/* SMI PHY Data Register */	
#define WILLOW_QD_REG_SCRATCH_MISC      	0x1A	/* Scratch & Misc Register */	
#define WILLOW_QD_REG_WD_CONTROL        	0x1B	/* Watch Dog Control Register */	
#define WILLOW_QD_REG_QOS_WEIGHT        	0x1C	/* QoS Weights Register */	
#define WILLOW_QD_REG_MISC				    0x1D	/* Misc Register */	
#define WILLOW_QD_REG_CUT_THROU_CTRL    	0x1F	/* Cut Through Control Register */	

/* TCAM Registers - Page 0 */
#define WILLOW_QD_REG_TCAM_OPERATION         0x0		/* TCAM Operation Register */
#define WILLOW_QD_REG_TCAM_P0_KEYS_1         0x2		/* TCAM Key Register 1 */
#define WILLOW_QD_REG_TCAM_P0_KEYS_2         0x3		/* TCAM Key Register 2 */
#define WILLOW_QD_REG_TCAM_P0_KEYS_3         0x4		/* TCAM Key Register 3 */
#define WILLOW_QD_REG_TCAM_P0_KEYS_4         0x5		/* TCAM Key Register 4 */
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_1   0x6		/* TCAM Match Data Register 1 */
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_2   0x7		/* TCAM Match Data Register 2 */
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_3   0x8
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_4   0x9
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_5   0xa
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_6   0xb
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_7   0xc
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_8   0xd
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_9   0xe
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_10  0xf
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_11  0x10
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_12  0x11
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_13  0x12
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_14  0x13
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_15  0x14
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_16  0x15
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_17  0x16
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_18  0x17
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_19  0x18
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_20  0x19
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_21  0x1a
#define WILLOW_QD_REG_TCAM_P0_MATCH_DATA_22  0x1b

/* TCAM Registers - Page 1 */
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_23   0x2		/* TCAM Match Data Register 23 */
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_24   0x3		/* TCAM Match Data Register 24 */
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_25   0x4
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_26   0x5
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_27   0x6
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_28   0x7
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_29   0x8
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_30   0x9
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_31   0xa
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_32   0xb
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_33   0xc
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_34   0xd
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_35   0xe
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_36   0xf
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_37   0x10
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_38   0x11
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_39   0x12
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_40   0x13
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_41   0x14
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_42   0x15
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_43   0x16
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_44   0x17
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_45   0x18
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_46   0x19
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_47   0x1a
#define WILLOW_QD_REG_TCAM_P1_MATCH_DATA_48   0x1b

/* TCAM Registers - Page 2 */
#define WILLOW_QD_REG_TCAM_P2_ACTION_1        0x2		/* TCAM Action 1 Register */
#define WILLOW_QD_REG_TCAM_P2_ACTION_2        0x3		/* TCAM Action 2 Register */
#define WILLOW_QD_REG_TCAM_P2_ACTION_3        0x4		/* TCAM Action 3 Register */
#define WILLOW_QD_REG_TCAM_P2_ACTION_5        0x6		/* TCAM Action 5 Register */
#define WILLOW_QD_REG_TCAM_P2_ACTION_6        0x7		/* TCAM Action 6 Register */
#define WILLOW_QD_REG_TCAM_P2_ACTION_7        0x8		/* TCAM Action 7 Register */
#define WILLOW_QD_REG_TCAM_P2_DEBUG_PORT      0x1C		/* TCAM Debug Register */
#define WILLOW_QD_REG_TCAM_P2_ALL_HIT         0x1F		/* TCAM Debug Register 2 */

/* TCAM Registers - Page 3 */
#define WILLOW_QD_REG_TCAM_EGR_PORT           0x1		/* Egress TCAM Port Register */
#define WILLOW_QD_REG_TCAM_EGR_ACTION_1       0x2		/* Egress TCAM Action 1 Register */
#define WILLOW_QD_REG_TCAM_EGR_ACTION_2       0x3		/* Egress TCAM Action 2 Register */
#define WILLOW_QD_REG_TCAM_EGR_ACTION_3       0x4		/* Egress TCAM Action 3 Register */


/****************************************************************************/
/*																			*/
/* Block: INTERNAL PHY Registers											*/
/*																			*/
/****************************************************************************/

/* Internal Copper PHY registers*/
#define WILLOW_QD_PHY_CONTROL_REG          0
#define WILLOW_QD_PHY_AUTONEGO_AD_REG      4
#define WILLOW_QD_PHY_AUTONEGO_1000AD_REG  9
#define WILLOW_QD_PHY_SPEC_CONTROL_REG		16
#define WILLOW_QD_PHY_PAGE_ANY_REG			22

/* Definitions for VCT registers */
#define WILLOW_QD_REG_MDI0_VCT_STATUS     16
#define WILLOW_QD_REG_MDI1_VCT_STATUS     17
#define WILLOW_QD_REG_MDI2_VCT_STATUS     18
#define WILLOW_QD_REG_MDI3_VCT_STATUS     19
#define WILLOW_QD_REG_ADV_VCT_CONTROL_5   23
#define WILLOW_QD_REG_ADV_VCT_SMPL_DIST   24
#define WILLOW_QD_REG_ADV_VCT_CONTROL_8   20
#define WILLOW_QD_REG_PAIR_SKEW_STATUS    20
#define WILLOW_QD_REG_PAIR_SWAP_STATUS    21

/* Bit Definition for QD_PHY_CONTROL_REG */
#define WILLOW_QD_PHY_RESET            0x8000
#define WILLOW_QD_PHY_LOOPBACK         0x4000
#define WILLOW_QD_PHY_SPEED            0x2000
#define WILLOW_QD_PHY_AUTONEGO         0x1000
#define WILLOW_QD_PHY_POWER            0x800
#define WILLOW_QD_PHY_ISOLATE          0x400
#define WILLOW_QD_PHY_RESTART_AUTONEGO 0x200
#define WILLOW_QD_PHY_DUPLEX           0x100
#define WILLOW_QD_PHY_SPEED_MSB        0x40

/* Bit Definition for QD_PHY_AUTONEGO_AD_REG */
#define WILLOW_QD_PHY_NEXTPAGE           0x8000
#define WILLOW_QD_PHY_REMOTEFAULT        0x4000
#define WILLOW_QD_PHY_PAUSE              0x400
#define WILLOW_QD_PHY_100_FULL           0x100
#define WILLOW_QD_PHY_100_HALF           0x80
#define WILLOW_QD_PHY_10_FULL            0x40
#define WILLOW_QD_PHY_10_HALF            0x20

#define WILLOW_QD_PHY_MODE_AUTO_AUTO    (WILLOW_QD_PHY_100_FULL | WILLOW_QD_PHY_100_HALF | WILLOW_QD_PHY_10_FULL | WILLOW_QD_PHY_10_HALF)
#define WILLOW_QD_PHY_MODE_100_AUTO     (WILLOW_QD_PHY_100_FULL | WILLOW_QD_PHY_100_HALF)
#define WILLOW_QD_PHY_MODE_10_AUTO      (WILLOW_QD_PHY_10_FULL | WILLOW_QD_PHY_10_HALF)
#define WILLOW_QD_PHY_MODE_AUTO_FULL    (WILLOW_QD_PHY_100_FULL | WILLOW_QD_PHY_10_FULL)
#define WILLOW_QD_PHY_MODE_AUTO_HALF    (WILLOW_QD_PHY_100_HALF | WILLOW_QD_PHY_10_HALF)

#define WILLOW_QD_PHY_MODE_100_FULL    WILLOW_QD_PHY_100_FULL
#define WILLOW_QD_PHY_MODE_100_HALF    WILLOW_QD_PHY_100_HALF
#define WILLOW_QD_PHY_MODE_10_FULL        WILLOW_QD_PHY_10_FULL    
#define WILLOW_QD_PHY_MODE_10_HALF        WILLOW_QD_PHY_10_HALF    

/* Gigabit Phy related definition */
#define WILLOW_QD_GIGPHY_1000X_FULL_CAP    0x8
#define WILLOW_QD_GIGPHY_1000X_HALF_CAP    0x4
#define WILLOW_QD_GIGPHY_1000T_FULL_CAP    0x2
#define WILLOW_QD_GIGPHY_1000T_HALF_CAP    0x1

#define WILLOW_QD_GIGPHY_1000X_CAP        (WILLOW_QD_GIGPHY_1000X_FULL_CAP|WILLOW_QD_GIGPHY_1000X_HALF_CAP)
#define WILLOW_QD_GIGPHY_1000T_CAP        (WILLOW_QD_GIGPHY_1000T_FULL_CAP|WILLOW_QD_GIGPHY_1000T_HALF_CAP)

#define WILLOW_QD_GIGPHY_1000X_FULL        0x20
#define WILLOW_QD_GIGPHY_1000X_HALF        0x40

#define WILLOW_QD_GIGPHY_1000T_FULL        0x200
#define WILLOW_QD_GIGPHY_1000T_HALF        0x100

#ifdef __cplusplus
}
#endif
#endif /* __Willow_msdDrvSwRegs_H */
#endif
