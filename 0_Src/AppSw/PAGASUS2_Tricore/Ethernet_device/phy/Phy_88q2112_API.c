/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */


#include "Phy_88q2112_API.h"
#ifdef TC397B
#include <Geth/Eth_Devices/IfxEth_Devices.h>
#else
#include <Eth/Eth_Devices/IfxEth_Devices.h>
#endif
#include "Phy_88q2112.h"
#include "internal_fw_debug.h"
//amy @1030
#include "config_pg2_feasures.h"
#include "i2crom_drv.h"

#define API_VERSION  "1.10"

// in Api Z1 1.11, it's recommended to use 500ms
// Based on the experience, we don't need that much delay here
#define Q2112_Z1_DELAY_MS   50
extern boolean boot_msg_flag;
#define Q2112_API_PRINTF_ALWAYS(x)  DEBUG_PRINTF_ALWAYS(x)
#define PRINTF_BOOT(x)       ({if(boot_msg_flag){DEBUG_PRINTF_ALWAYS(x);}})

//Store mode setting/id in memory or in file system
//static boolean saveOperateModeSetting(uint32 slaveaddr, uint32 modeId);
//Load mode setting/id from memory or from file system
//static boolean loadOperateModeSetting(uint32 slaveaddr, uint32 * modeId);
static boolean loadOperateModeSetting(uint32 * modeId, uint8 mode_sel);


//Store mode setting/id in memory or in file system
#if 0
static boolean saveOperateModeSetting(uint32 slaveaddr, uint32 modeId)
{
    boolean ret= TRUE;

    return ret;
}
#endif

#if 1
//Load mode setting/id from memory or from file system
//static boolean loadOperateModeSetting(uint32 slaveaddr, uint32 * modeId)
static boolean loadOperateModeSetting(uint32 * modeId, uint8 mode_sel)
{
    boolean ret= TRUE;

    #if 0
    if(mode_sel == INFOROM_ADDR_TBL_ETH_PHY_MODE_COMPLIANCE)
        // Set compliance (IEEE) mode
        *modeId= MRVL_88Q2112_MODE_DEFAULT;
    else
        // A2 default is Legacy mode
        *modeId= MRVL_88Q2112_MODE_LEGACY;

    #else

    // A2 default is Legacy mode
    *modeId= MRVL_88Q2112_MODE_LEGACY;
    #endif

    return ret;
}

static void softReset(uint32 slaveaddr)
{
    uint32 regData = 0;
    regData = getRevNum(slaveaddr);
    if(regData == MRVL_88Q2112_A1 || regData == MRVL_88Q2112_A2)
    {
        //A1 and A2 has the same software reset routine
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFDA1, 0x0003);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0900, 0x4000);

        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFE4, 0x070F);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0208);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0000);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0421);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0000);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0420);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0000);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E1);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0000);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E0);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0000);

        IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, 3, 0x0900, &regData);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x0900, regData | 0x8000);


        DelayMs(1);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0208);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0003);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E0);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0099);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E1);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0009);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0420);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x00CC);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0421);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x000C);

        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0900, 0x0000);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFDA1, 0x0000);
    }
    else{
        //A0 software reset routine
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E0);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0000);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E1);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0000);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0420);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0000);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0421);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0000);

        IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, 3, 0x0900, &regData);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x0900, regData | 0x8000);
        DelayMs(1);

        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E0);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0099);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E1);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0009);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0420);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x00cc);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0421);
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x000c);
    }
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFE4, 0x000C);
}


 
static boolean getAnegEnabled(uint32 slaveaddr)
{
    uint32 value = 0;
    IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, 7, 0x0200, &value);
    if((value & 0x1000) == 0x1000)
        return TRUE;
    else
        return FALSE;
}


static uint32 getSpeed(uint32 slaveaddr)
{
    uint32 value = 0;
    // check if AN enabled
    if (getAnegEnabled(slaveaddr))
    {
        IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, 7, 0x801a, &value);
        if((value & 0x4000) == 0x4000)
            return 1;
        else
            return 0;
    }
    else
    {
        IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, 1, 0x0834, &value);
        if((value & 0x0001) == 0x0001)
            return 1;
        else
            return 0;
    }
}

//static boolean applyOperateModeConfig(uint32 slaveaddr)
static boolean applyOperateModeConfig(uint32 slaveaddr, uint8 mode_sel)
{
    //Legacy/Compliance mode differentiation is not applicable in FE speed.
    boolean status = FALSE;

    if (getSpeed(slaveaddr) == 0)
        return status;

    uint32 op_mode = MRVL_88Q2112_MODE_DEFAULT;
    uint32 regData = getRevNum(slaveaddr);

    //status = loadOperateModeSetting(slaveaddr, &op_mode);
    status = loadOperateModeSetting(&op_mode, mode_sel);

    if (status){
        if (regData == MRVL_88Q2112_A2){
#if 0
           // 1.34.10 patch 10
            if (op_mode == MRVL_88Q2112_MODE_LEGACY)
            {
                // Enable 1000BT1 legacy mode support
                IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFDB8, 0x0001);
                IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFD3D, 0x0014);
                IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0902, op_mode |= 1 << 1);
                Q2112_API_PRINTF_ALWAYS(("Apply Legacy mode\r\n"));
            }
            else
            {
                // Set back to default compliant mode setting
                IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFDB8, 0x0000);
                IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFD3D, 0x0000);
                IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0902, op_mode |= 1 << 1);
                Q2112_API_PRINTF_ALWAYS(("Apply A2 mode\r\n"));
#else
            //porting for B2B board's PHY
            if (slaveaddr == 0){
                /*
                IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFDB8, 0x0000);
                                IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFD3D, 0x0000);
                                IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0902, op_mode |= 1 << 1);
                                Q2112_API_PRINTF_ALWAYS(("Force B2B use Apply A2 mode\r\n"));
                                */
                IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFDB8, 0x0001);
                                IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFD3D, 0x0014);
                                IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0902, op_mode |= 1 << 1);
                                Q2112_API_PRINTF_ALWAYS(("Force B2B  Apply Legacy mode\r\n"));

            }else{
				if (mode_sel == INFOROM_ADDR_TBL_ETH_PHY_MODE_LEGACY)	
                {
                    // Enable 1000BT1 legacy mode support
                    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFDB8, 0x0001);
                    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFD3D, 0x0014);
                    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0902, op_mode |= 1 << 1);
                    PRINTF_BOOT(("Apply Legacy mode\r\n"));
                    Q2112_API_PRINTF_ALWAYS(("Apply 88Q2112 %d in Legacy mode.\r\n",slaveaddr));
                }
				else if	(mode_sel == INFOROM_ADDR_TBL_ETH_PHY_MODE_COMPLIANCE)
                {
                    // Set back to default compliant mode setting
                    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFDB8, 0x0000);
                    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFD3D, 0x0000);
                    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0902, op_mode |= 1 << 1);
                    Q2112_API_PRINTF_ALWAYS(("Apply 88Q2112 %d in Compliance mode.\r\n",slaveaddr));    // A2 mode
                }
            }
#endif
        }
        else
        {
            IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0902, op_mode |= 1 << 1);
        }
    }

    return status;
}


char* get_api_version(void)
{
    return API_VERSION;
}


uint32 getRevNum(uint32 slaveaddr)
{
    uint32 devAddr = 1;
    uint32 regAddr = 0x0003;
    uint32 retData = 0;
    uint32 revNum = 0;
    PRINTF_BOOT(("getRevNum\r\n"));
    IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, devAddr, regAddr, &retData);
    revNum = (retData & 0xf);

    return revNum;
}
#endif

/* Q2112 A0 API 1.00
  Initial 100M speed */
void initQ2112Fe(uint32 slaveaddr)
{
  uint32 value = 0;
  DelayMs(1);

  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFA07, 0x0202);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x0200, 0x0000);
  // set speed to 100M
  IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, 1, 0x0834, &value);
  value = value & 0xFFF0;
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0834, value);
  DelayMs(5);

  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8000, 0x0000);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8100, 0x0200);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFA1E, 0x0002);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE5C, 0x2402);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFA12, 0x001F);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFA0C, 0x9E05);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xfbdd, 0x6862);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xfbde, 0x736e);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xfbdf, 0x7f79);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xfbe0, 0x8a85);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xfbe1, 0x9790);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xfbe3, 0xa39d);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xfbe4, 0xb0aa);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xfbe5, 0xb8);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFBFD, 0x0D0A);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFBFE, 0x0906);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8016, 0x0011);

  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x0900, 0x8000);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFA07, 0x0200);
}


#if 1
/* Initial 1G speed */
//void initQ2112Ge(uint32 slaveaddr)
void initQ2112Ge(uint32 slaveaddr, uint8 mode_sel)
{
  uint32 value = 0;
  uint32 revData = 0;
  DelayMs(1);

  revData = getRevNum(slaveaddr);
  if(revData == MRVL_88Q2112_A1 || revData == MRVL_88Q2112_A2)
  {
    // A1 and A2 has the same software reset routine
    // Except A1 need to be set to legacy mode by default.
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0900, 0x4000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x0200, 0x0000);

    // set speed to 1G
    IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, 1, 0x0834, &value);
    value = (value & 0xFFF0) | 0x0001;
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0834, value);

    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFE4, 0x07B5);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFE4, 0x06B6);

    DelayMs(5);

    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFDE, 0x402F);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8032, 0x0064);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8031, 0x0A01);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8031, 0x0C01);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE0F, 0x0000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x800C, 0x0000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE2A, 0x3C3D);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x801D, 0x0800);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC00, 0x01C0);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC17, 0x0425);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC94, 0x5470);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC95, 0x0055);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC19, 0x08d8);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC1a, 0x0110);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC1b, 0x0a10);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC3A, 0x2725);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC61, 0x2627);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC3B, 0x1612);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC62, 0x1C12);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC9D, 0x6367);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC9E, 0x8060);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC00, 0x01C8);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8000, 0x0000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8016, 0x0011);

    if (revData == MRVL_88Q2112_A1)
    {
        //Set A1 to legacy mode
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFDB8, 0x0001);
    }
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE02, 0x00C0);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFDB, 0x0010);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFF3, 0x0020);
  }
  else
  {// A0
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFE4, 0x06B6);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x0200, 0x0000);

    // set speed to 1G
    IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, 1, 0x0834, &value);
    value = (value & 0xFFF0) | 0x0001;
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0834, value);
    DelayMs(5);

    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFDE, 0x402F);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8032, 0x0064);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8031, 0x0A01);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8031, 0x0C01);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE0F, 0x0000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x800C, 0x0000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE2A, 0x3C3D);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC00, 0x01C0);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC17, 0x0425);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC94, 0x5470);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC95, 0x0055);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC19, 0x08d8);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC1a, 0x0110);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC1b, 0x0a10);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC3A, 0x2725);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC61, 0x2627);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC3B, 0x1612);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC62, 0x1C12);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC9D, 0x6367);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC9E, 0x8060);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC00, 0x01C8);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8000, 0x0000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8016, 0x0011);
  }

  //Apply operating mode setting
  //applyOperateModeConfig(slaveaddr);
  applyOperateModeConfig(slaveaddr, mode_sel);
  softReset(slaveaddr);
}
#endif

//void initQ2112Ge_B2B(uint32 slaveaddr)
void initQ2112Ge_B2B(uint32 slaveaddr, uint8 mode_sel)
{
  uint32 value = 0;
  uint32 revData = 0;
  DelayMs(1);

  revData = getRevNum(slaveaddr);
  if(revData == MRVL_88Q2112_A1 || revData == MRVL_88Q2112_A2)
  {
    // A1 and A2 has the same software reset routine
    // Except A1 need to be set to legacy mode by default.
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0900, 0x4000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x0200, 0x0000);

    // set speed to 1G
    IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, 1, 0x0834, &value);
    Q2112_API_PRINTF_ALWAYS(("Force B2B  0xC001 to master\r\n"));
    value = (value & 0xFFF0) | 0xC001;
    //value = (value & 0xFFF0) | 0x0001;
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0834, value);

    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFE4, 0x07B5);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFE4, 0x06B6);

    DelayMs(5);

    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFDE, 0x402F);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8032, 0x0064);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8031, 0x0A01);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8031, 0x0C01);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE0F, 0x0000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x800C, 0x0000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE2A, 0x3C3D);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x801D, 0x0800);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC00, 0x01C0);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC17, 0x0425);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC94, 0x5470);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC95, 0x0055);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC19, 0x08d8);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC1a, 0x0110);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC1b, 0x0a10);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC3A, 0x2725);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC61, 0x2627);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC3B, 0x1612);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC62, 0x1C12);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC9D, 0x6367);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC9E, 0x8060);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC00, 0x01C8);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8000, 0x0000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8016, 0x0011);

    if (revData == MRVL_88Q2112_A1)
    {
        //Set A1 to legacy mode
        IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFDB8, 0x0001);
    }
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE02, 0x00C0);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFDB, 0x0010);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFF3, 0x0020);
  }
  else
  {// A0
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFE4, 0x06B6);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x0200, 0x0000);

    // set speed to 1G
    IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, 1, 0x0834, &value);
    value = (value & 0xFFF0) | 0x0001;
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0834, value);
    DelayMs(5);

    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFFDE, 0x402F);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8032, 0x0064);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8031, 0x0A01);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8031, 0x0C01);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE0F, 0x0000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x800C, 0x0000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE2A, 0x3C3D);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC00, 0x01C0);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC17, 0x0425);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC94, 0x5470);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC95, 0x0055);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC19, 0x08d8);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC1a, 0x0110);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC1b, 0x0a10);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC3A, 0x2725);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC61, 0x2627);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC3B, 0x1612);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC62, 0x1C12);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC9D, 0x6367);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC9E, 0x8060);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC00, 0x01C8);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8000, 0x0000);
    IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8016, 0x0011);
  }

  //Apply operating mode setting
  //applyOperateModeConfig(slaveaddr);
  applyOperateModeConfig(slaveaddr, mode_sel);
  softReset(slaveaddr);
}



/* Initial 100M speed */
void initQ2112Z1Fe(uint32 slaveaddr)
{
  uint32 value = 0;
  // por init.
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x0000, 0x0000);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0900, 0x0000);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x800d, 0x0000);

  // Link LED
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8000, 0x0000);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8016, 0x0011);

  // disable AN
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x0200, 0x0000);

  //set speed
  IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, 1, 0x0834, &value);
  value = value & 0xFFF0;
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0834, value);
  DelayMs(Q2112_Z1_DELAY_MS);

  // Init code
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8000, 0x0000);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE05, 0x3DAA);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE07, 0x6BFF);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFB95, 0x52F0);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE5D, 0x171C);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8016, 0x0011);

  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x0900, 0x8000);
  DelayMs(Q2112_Z1_DELAY_MS); // delay 500ms
}

/* Q2112 Z1 API 1.10
  Initial 100M speed */
void initQ2112Z1Ge(uint32 slaveaddr)
{
  uint32 value = 0;
  // por init.
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x0000, 0x0000);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0900, 0x0000);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x800d, 0x0000);

  // Link LED
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8000, 0x0000);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8016, 0x0011);

  // disable AN
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x0200, 0x0000);


  // restore default from 100M
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE05, 0x0000);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE07, 0x6A10);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFB95, 0x5720);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE5D, 0x175C);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x8016, 0x0071);

 // set speed to 1G
  IfxEth_Phy_88q2112_read_mdio_reg(slaveaddr, 1, 0x0834, &value);
  value = (value & 0xFFF0) | 0x0001;
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0834, value);
  DelayMs(Q2112_Z1_DELAY_MS);

  // Init code (version J)
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 1, 0x0902, 0x0002);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE12, 0x000E);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE05, 0x05AA);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE04, 0x0016);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE07, 0x681F);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE5D, 0x045C);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE7C, 0x001E);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC00, 0x01C0);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8032, 0x0020);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8031, 0x0012);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8031, 0x0A12);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8032, 0x003C);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8031, 0x0001);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 7, 0x8031, 0x0A01);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC10, 0xD870);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC11, 0x1522);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC12, 0x07FA);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC13, 0x010B);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC15, 0x35A4);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC2D, 0x3C34);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC2E, 0x104B);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC2F, 0x1C15);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC30, 0x3C3C);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC31, 0x3C3C);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC3A, 0x2A2A);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC61, 0x2829);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC3B, 0x0E0E);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC62, 0x1C12);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC32, 0x03D2);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC46, 0x0200);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC86, 0x0401);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC4E, 0x1820);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC9C, 0x0101);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC95, 0x007A);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC3E, 0x221F);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC3F, 0x0A08);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x020E);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0077);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0210);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0088);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0215);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x00AA);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x01D5);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x00AA);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0216);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x00AB);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x01D6);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x00AB);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0213);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x00A0);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x01D3);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x00A0);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0214);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x00AB);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x01D4);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x00AB);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x046B);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x00FA);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x046C);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x01F4);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x046E);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x01F4);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0455);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0320);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0416);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0323);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x0004);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0000);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03CC);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0055);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03CD);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0055);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03CE);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0022);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03CF);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0022);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03D0);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0022);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03D1);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0022);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E4);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0055);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E5);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0055);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E6);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0022);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E7);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0022);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E8);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0022);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x03E9);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0022);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC03, 0x040C);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC04, 0x0033);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC5D, 0x06BF);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC89, 0x0003);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC5C, 0x007F);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC69, 0x383A);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC6A, 0x383A);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC6B, 0x0082);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC6F, 0x888F);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC70, 0x0D1A);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC71, 0x0505);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC72, 0x090C);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC73, 0x0C0F);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC74, 0x0400);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC75, 0x0103);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC7A, 0x081E);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC8C, 0xBC40);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC8D, 0x9830);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC91, 0x0000);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC63, 0x4440);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC64, 0x3C3F);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC65, 0x783C);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC66, 0x0002);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC7B, 0x7818);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC7C, 0xC440);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC7D, 0x5360);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC5F, 0x4034);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC60, 0x7858);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC7E, 0x003F);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC8E, 0x0003);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC57, 0x1820);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC00, 0x01C8);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC93, 0x141C);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC9B, 0x0091);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFC94, 0x6D88);
  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0xFE4A, 0x5653);

  IfxEth_Phy_88q2112_write_mdio_reg(slaveaddr, 3, 0x0900, 0x8000);
  DelayMs(Q2112_Z1_DELAY_MS);
}



