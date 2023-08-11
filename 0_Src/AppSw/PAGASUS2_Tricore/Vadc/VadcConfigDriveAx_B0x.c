/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#include "Configuration.h"
#if PROJECT == PROJECT_PAGASUS2
#include "EvadcAutoScan.h"
#else
#include "VadcAutoScan.h"
#endif
#include "board.h"
#if PROJECT == PROJECT_PAGASUS2
#if PG2_PREEVT == 1

const IfxVadc_Pin IfxVadc_AN_P3570[] =
{
    //  {AN#,PU,PD,"name to display"}
    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "INPUTS:",0,0,0,0,0,""},
    {0, NO_GROUP_SHARED, 49.9,10,"KL30_VBAT1",1,0,0,1,0,"V"},
    {10, NO_GROUP_SHARED,49.9,10,"KL30_VBAT2",1,0,0,1,0,"V"},
    {1, NO_GROUP_SHARED, 49.9,10,"KL30_POWER",1,0,0,1,0,"V"},
    {2, NO_GROUP_SHARED, 49.9,10,"KL15_POWER",1,0,0,1,0,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "AURIX:",0,0,0,0,0,""},
    {7, NO_GROUP_SHARED, 0,1,"AURIX_1V3",1,0,0,1,0,"V"},
    {6, NO_GROUP_SHARED, 0,1,"AURIX_3V3",1,0,0,1,0,"V"},
    {3, NO_GROUP_SHARED, 4.99,10,"AURIX_5V",1,0,0,1,0,"V"},
    {4, NO_GROUP_SHARED, 4.99,10,"CAN1_5V",1,0,0,1,0,"V"},
    {8, NO_GROUP_SHARED, 4.99,10,"CAN2_5V",1,0,0,1,0,"V"},
    {5, NO_GROUP_SHARED, 4.99,10,"FR_5V",1,0,0,1,0,"V"},
    {9, NO_GROUP_SHARED, 0,1,"HRNS_GPI",1,0,2.35,1.06540,-0.16098,"V"},
    {11, NO_GROUP_SHARED,4.99,10,"AURIX2_CAN2_5V",1,0,0,1,0,"V"},
    {12, NO_GROUP_SHARED,4.99,10,"AURIX2_CAN1_5V",1,0,0,1,0,"V"},
    {13, NO_GROUP_SHARED,4.99,10,"AURIX2_5V",1,0,0,1,0,"V"},
    {14, NO_GROUP_SHARED,0,1,"AURIX2_3V3",1,0,0,1,0,"V"},
    {15, NO_GROUP_SHARED,0,1,"AURIX2_1V3",1,0,0,1,0,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "SYSTEM:",0,0,0,0,0,""},
    {20, NO_GROUP_SHARED, 49.9,10,"VBAT_SYS",1,0,0,1,0,"V"},
    {16, NO_GROUP_SHARED, 0,1,"VBATSYS_ISENSE",8.225,0,0,1,0,"A"},
    {32, NO_GROUP_SHARED, 0,1,"SYS_0V82",1,0,0,1,0,"V"},
    {40, 0x31, 6.81,10,"SYS_5V",1,0,0,1,0,"V"},
    {37, 0x31, 0,1,"SYS_0V85",1,0,0,1,0,"V"},
    {42, 0x31, 0,1,"SYS_0V92_1",1,0,0,1,0,"V"},
    {47, 0x31, 0,1,"SYS_0V92_2",1,0,0,1,0,"V"},
    {36, 0x31, 0,1,"SYS_1V0",1,0,0,1,0,"V"},
    {46, 0x31, 0,1,"SYS_1V1_2",1,0,0,1,0,"V"},
    {35, 0x31, 0,1,"SYS_1V2",1,0,0,1,0,"V"},
    {45, 0x31, 0,1,"SYS_1V5_2",1,0,0,1,0,"V"},
    {39, 0x31, 0,1,"SYS_1V8_1",1,0,0,1,0,"V"},
    {44, 0x31, 0,1,"SYS_1V8_2",1,0,0,1,0,"V"},
    {34, 0x31, 0,1,"SYS_2V1",1,0,0,1,0,"V"},
    {41, 0x31, 0,1,"SYS_2V5",1,0,0,1,0,"V"},
    {38, 0x31, 1,10,"SYS_3V3_1",1,0,0,1,0,"V"},
    {43, 0x31, 1,10,"SYS_3V3_2",1,0,0,1,0,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "Tegra A:",0,0,0,0,0,""},
    {21, NO_GROUP_SHARED, 49.9,10,"VBAT_TEG",1,0,0,1,0,"V"},
    {17, NO_GROUP_SHARED, 0,1,"VBATTEG_ISENSE",8.225,0,0,1,0,"A"},
    {24, NO_GROUP_SHARED, 20,5.11,"XA_PREREG",1,0,0,1,0,"V"},
    {25, NO_GROUP_SHARED, 6.81,10,"XA_5V",1,0,0,1,0,"V"},
    {26, NO_GROUP_SHARED, 6.81,10,"XA_5V_SW",1,0,0,1,0,"V"},
    {34, 0x01, 0,1,"X1_DGPU_PG",1,0,0,1,0,"V"},
    {35, 0x01, 0,1,"X1_DGPU_VMON_INT(A01)",1,0,0,1,0,"V"},
    {40, 0x01, 0,1,"XA_VDD_1V0",1,0,0,1,0,"V"},
    {36, 0x01, 0,1,"XA_VDD_1V8_AO",1,0,0,1,0,"V"},
    {37, 0x01, 0,1,"XA_VDD_1V8_HS",1,0,0,1,0,"V"},
    {38, 0x01, 0,1,"XA_VDD_1V8_LS",1,0,0,1,0,"V"},
    {45, 0x01, 0,1,"XA_VDD_CPU",1,0,0,1,0,"V"},
    {43, 0x01, 0,1,"XA_VDD_CV",1,0,0,1,0,"V"},
    {41, 0x01, 0,1,"XA_VDD_DDR2",1,0,0,1,0,"V"},
    {42, 0x01, 0,1,"XA_VDD_DDRQ",1,0,0,1,0,"V"},
    {39, 0x01, 0,1,"XA_VDD_DDR_1V1",1,0,0,1,0,"V"},
    {44, 0x01, 0,1,"XA_VDD_GPU",1,0,0,1,0,"V"},
    {46, 0x01, 0,1,"XA_VDD_SOC",1,0,0,1,0,"V"},
    {47, 0x01, 0,1,"X1_DGPU_THERM_ALERT_N",1,0,0,1,0,"V"},
    {46, 0x21, 0,1,"CAM_1V2(A01)",1,0,0,1,0,"V"},
    {47, 0x21, 0,1,"CAM_1V8(A01)",1,0,0,1,0,"V"},
    {22, NO_GROUP_SHARED, 49.9,10,"VBAT_SXMA",1,0,0,1,0,"V"},
    {18, NO_GROUP_SHARED, 0,1,"VBATSXMA_ISENSE",8.225,0,0,1,0,"A"},
    {27, NO_GROUP_SHARED, 20,5.11,"SXMA_PREREG",1,0,0,1,0,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "Tegra B:",0,0,0,0,0,""},
    {28, NO_GROUP_SHARED, 20,5.11,"XB_PREREG",1,0,0,1,0,"V"},
    {29, NO_GROUP_SHARED, 6.81,10,"XB_5V",1,0,0,1,0,"V"},
    {30, NO_GROUP_SHARED, 6.81,10,"XB_5V_SW",1,0,0,1,0,"V"},
    {34, 0x11, 0,1,"X2_DGPU_PG",1,0,0,1,0,"V"},
    {35, 0x11, 0,1,"X2_DGPU_VMON_INT(A01)",1,0,0,1,0,"V"},
    {40, 0x11, 0,1,"XB_VDD_1V0",1,0,0,1,0,"V"},
    {36, 0x11, 0,1,"XB_VDD_1V8_AO",1,0,0,1,0,"V"},
    {37, 0x11, 0,1,"XB_VDD_1V8_HS",1,0,0,1,0,"V"},
    {38, 0x11, 0,1,"XB_VDD_1V8_LS",1,0,0,1,0,"V"},
    {45, 0x11, 0,1,"XB_VDD_CPU",1,0,0,1,0,"V"},
    {43, 0x11, 0,1,"XB_VDD_CV",1,0,0,1,0,"V"},
    {41, 0x11, 0,1,"XB_VDD_DDR2",1,0,0,1,0,"V"},
    {42, 0x11, 0,1,"XB_VDD_DDRQ",1,0,0,1,0,"V"},
    {39, 0x11, 0,1,"XB_VDD_DDR_1V1",1,0,0,1,0,"V"},
    {44, 0x11, 0,1,"XB_VDD_GPU",1,0,0,1,0,"V"},
    {46, 0x11, 0,1,"XB_VDD_SOC",1,0,0,1,0,"V"},
    {47, 0x11, 0,1,"X2_DGPU_THERM_ALERT_N",1,0,0,1,0,"V"},
    {23, NO_GROUP_SHARED, 49.9,10,"VBAT_SXMB",1,0,0,1,0,"V"},
    {19, NO_GROUP_SHARED, 0,1,"VBATSXMB_ISENSE",8.225,0,0,1,0,"A"},
    {31, NO_GROUP_SHARED, 20,5.11,"SXMB_PREREG",1,0,0,1,0,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "B2B:",0,0,0,0,0,""},
    {34, 0x21, 0,1,"B2B_PRST_DET1_N",1,0,0,1,0,"V"},
    {35, 0x21, 0,1,"B2B_PRST_DET2_N",1,0,0,1,0,"V"},
    {36, 0x21, 0,1,"B2B_BRD_ID0_RX",1,0,0,1,0,"V"},
    {37, 0x21, 0,1,"B2B_BRD_ID1_RX",1,0,0,1,0,"V"},
    {38, 0x21, 0,1,"B2B_BRD_ID2_RX",1,0,0,1,0,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "A02:",0,0,0,0,0,""},
    {40, 0x21, 0,1,"VDD_3V3_UFS_1_1_ADC",1.1,0,0,1,0,"V"},
    {41, 0x21, 0,1,"VDD_3V3_UFS_2_ADC",1.1,0,0,1,0,"V"},
    {39, 0x21, 0,1,"VDD_A2B_POC_SENSE",1,0,0,1,0,"V"},
    {33, 0x21, 0,1,"VDD_PWRYNK_SHDN",1,0,0,1,0,"V"},
};

const IfxVadc_Pin IfxVadc_AN_P3570_AURIX2[] =
{
//  {AN#,PU,PD,"name to display"}
    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "INPUTS:",0,0,0,0,0,""},
    {0, NO_GROUP_SHARED, 49.9,10,"AURIX2_KL30",1,0,0,1,0,"V"},
    {5, NO_GROUP_SHARED, 4.99,10,"AURIX2_CAN3_5V",1,0,0,1,0,"V"},
};

const IfxVadc_Pin IfxVadc_AN_E3550[] =
{
    //  {AN#,PU,PD,"name to display"}
    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "INPUTS:",0,0,0,0,0,""},
    {0, NO_GROUP_SHARED, 49.9,10,"KL30_VBAT",1,0,0,1,0,"V"},
    {1, NO_GROUP_SHARED, 49.9,10,"KL30_POWER",1,0,0,1,0,"V"},
    {2, NO_GROUP_SHARED, 49.9,10,"KL15_POWER",1,0,0,1,0,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "AURIX:",0,0,0,0,0,""},
    {7, NO_GROUP_SHARED, 0,1,"AURIX_1V3",1,0,0,1,0,"V"},
    {6, NO_GROUP_SHARED, 0,1,"AURIX_3V3",1,0,0,1,0,"V"},
    {3, NO_GROUP_SHARED, 4.99,10,"AURIX_5V",1,0,0,1,0,"V"},
    {4, NO_GROUP_SHARED, 4.99,10,"CAN1_5V",1,0,0,1,0,"V"},
    {8, NO_GROUP_SHARED, 4.99,10,"CAN2_5V",1,0,0,1,0,"V"},
    {5, NO_GROUP_SHARED, 4.99,10,"FR_5V",1,0,0,1,0,"V"},
    {9, NO_GROUP_SHARED, 0,1,"HRNS_GPI",1,0,2.35,1.06540,-0.16098,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "SYSTEM:",0,0,0,0,0,""},
    {20, NO_GROUP_SHARED, 49.9,10,"VBAT_SYS",1,0,0,1,0,"V"},
    {16, NO_GROUP_SHARED, 0,1,"VBATSYS_ISENSE",8.225,0,0,1,0,"A"},
    {40, 0x31, 6.81,10,"SYS_5V",1,0,0,1,0,"V"},
    {37, 0x31, 0,1,"SYS_0V85",1,0,0,1,0,"V"},
    {42, 0x31, 0,1,"SYS_0V92_1",1,0,0,1,0,"V"},
    {47, 0x31, 0,1,"SYS_0V92_2",1,0,0,1,0,"V"},
    {36, 0x31, 0,1,"SYS_1V0",1,0,0,1,0,"V"},
    {46, 0x31, 0,1,"SYS_1V1_2",1,0,0,1,0,"V"},
    {35, 0x31, 0,1,"SYS_1V2",1,0,0,1,0,"V"},
    {45, 0x31, 0,1,"SYS_1V5_2",1,0,0,1,0,"V"},
    {39, 0x31, 0,1,"SYS_1V8_1",1,0,0,1,0,"V"},
    {44, 0x31, 0,1,"SYS_1V8_2",1,0,0,1,0,"V"},
    {34, 0x31, 0,1,"SYS_2V1",1,0,0,1,0,"V"},
    {41, 0x31, 0,1,"SYS_2V5",1,0,0,1,0,"V"},
    {38, 0x31, 1,10,"SYS_3V3_1",1,0,0,1,0,"V"},
    {43, 0x31, 1,10,"SYS_3V3_2",1,0,0,1,0,"V"},
    {12, NO_GROUP_SHARED, 20,5.11,"CE_PREREG",1,0,0,1,0,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "Tegra A:",0,0,0,0,0,""},
    {21, NO_GROUP_SHARED, 49.9,10,"VBAT_TEG",1,0,0,1,0,"V"},
    {17, NO_GROUP_SHARED, 0,1,"VBATTEG_ISENSE",8.225,0,0,1,0,"A"},
    {24, NO_GROUP_SHARED, 20,5.11,"XA_PREREG",1,0,0,1,0,"V"},
    {25, NO_GROUP_SHARED, 6.81,10,"XA_5V",1,0,0,1,0,"V"},
    {26, NO_GROUP_SHARED, 6.81,10,"XA_5V_SW",1,0,0,1,0,"V"},
    {40, 0x01, 0,1,"XA_VDD_1V0",1,0,0,1,0,"V"},
    {36, 0x01, 0,1,"XA_VDD_1V8_AO",1,0,0,1,0,"V"},
    {37, 0x01, 0,1,"XA_VDD_1V8_HS",1,0,0,1,0,"V"},
    {38, 0x01, 0,1,"XA_VDD_1V8_LS",1,0,0,1,0,"V"},
    {45, 0x01, 0,1,"XA_VDD_CPU",1,0,0,1,0,"V"},
    {43, 0x01, 0,1,"XA_VDD_CV",1,0,0,1,0,"V"},
    {41, 0x01, 0,1,"XA_VDD_DDR2",1,0,0,1,0,"V"},
    {42, 0x01, 0,1,"XA_VDD_DDRQ",1,0,0,1,0,"V"},
    {39, 0x01, 0,1,"XA_VDD_DDR_1V1",1,0,0,1,0,"V"},
    {44, 0x01, 0,1,"XA_VDD_GPU",1,0,0,1,0,"V"},
    {46, 0x01, 0,1,"XA_VDD_SOC",1,0,0,1,0,"V"},
    {47, 0x01, 0,1,"X1_DGPU_THERM_ALERT_N",1,0,0,1,0,"V"},
    {22, NO_GROUP_SHARED, 49.9,10,"VBAT_SXMA",1,0,0,1,0,"V"},
    {18, NO_GROUP_SHARED, 0,1,"VBATSXMA_ISENSE",8.225,0,0,1,0,"A"},
    {27, NO_GROUP_SHARED, 20,5.11,"SXMA_PREREG",1,0,0,1,0,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "Tegra B:",0,0,0,0,0,""},
    {28, NO_GROUP_SHARED, 20,5.11,"XB_PREREG",1,0,0,1,0,"V"},
    {29, NO_GROUP_SHARED, 6.81,10,"XB_5V",1,0,0,1,0,"V"},
    {30, NO_GROUP_SHARED, 6.81,10,"XB_5V_SW",1,0,0,1,0,"V"},
    {40, 0x11, 0,1,"XB_VDD_1V0",1,0,0,1,0,"V"},
    {36, 0x11, 0,1,"XB_VDD_1V8_AO",1,0,0,1,0,"V"},
    {37, 0x11, 0,1,"XB_VDD_1V8_HS",1,0,0,1,0,"V"},
    {38, 0x11, 0,1,"XB_VDD_1V8_LS",1,0,0,1,0,"V"},
    {45, 0x11, 0,1,"XB_VDD_CPU",1,0,0,1,0,"V"},
    {43, 0x11, 0,1,"XB_VDD_CV",1,0,0,1,0,"V"},
    {41, 0x11, 0,1,"XB_VDD_DDR2",1,0,0,1,0,"V"},
    {42, 0x11, 0,1,"XB_VDD_DDRQ",1,0,0,1,0,"V"},
    {39, 0x11, 0,1,"XB_VDD_DDR_1V1",1,0,0,1,0,"V"},
    {44, 0x11, 0,1,"XB_VDD_GPU",1,0,0,1,0,"V"},
    {46, 0x11, 0,1,"XB_VDD_SOC",1,0,0,1,0,"V"},
    {47, 0x11, 0,1,"X2_DGPU_THERM_ALERT_N",1,0,0,1,0,"V"},
    {23, NO_GROUP_SHARED, 49.9,10,"VBAT_SXMB",1,0,0,1,0,"V"},
    {19, NO_GROUP_SHARED, 0,1,"VBATSXMB_ISENSE",8.225,0,0,1,0,"A"},
    {31, NO_GROUP_SHARED, 20,5.11,"SXMB_PREREG",1,0,0,1,0,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "CVM:",0,0,0,0,0,""},
    {10, NO_GROUP_SHARED, 20,5.11,"P1_PREREG",1,0,0,1,0,"V"},
    {11, NO_GROUP_SHARED, 6.81,10,"P1_5V",1,0,0,1,0,"V"},
};

const IfxVadc_Pin IfxVadc_AN_P3479[] =
{
    //  {AN#,PU,PD,"name to display"}
    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "INPUTS:",0,0,0,0,0,""},
    {0, NO_GROUP_SHARED, 49.9,10,"KL30_VBAT",1,0,0,1,0,"V"},
    {1, NO_GROUP_SHARED, 49.9,10,"KL30_POWER",1,0,0,1,0,"V"},
    {2, NO_GROUP_SHARED, 49.9,10,"KL15_POWER",1,0,0,1,0,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "AURIX:",0,0,0,0,0,""},
    {7, NO_GROUP_SHARED, 0,1,"AURIX_1V3",1,0,0,1,0,"V"},
    {6, NO_GROUP_SHARED, 0,1,"AURIX_3V3",1,0,0,1,0,"V"},
    {3, NO_GROUP_SHARED, 4.99,10,"AURIX_5V",1,0,0,1,0,"V"},
    {4, NO_GROUP_SHARED, 4.99,10,"CAN1_5V",1,0,0,1,0,"V"},
    {8, NO_GROUP_SHARED, 4.99,10,"CAN2_5V",1,0,0,1,0,"V"},
    {5, NO_GROUP_SHARED, 4.99,10,"FR_5V",1,0,0,1,0,"V"},
    {9, NO_GROUP_SHARED, 0,1,"HRNS_GPI",1,0,2.55,1.08790,-0.22803,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "SYSTEM:",0,0,0,0,0,""},
    {20, NO_GROUP_SHARED, 49.9,10,"VBAT_SYS",1,0,0,1,0,"V"},
    {16, NO_GROUP_SHARED, 0,1,"VBATSYS_ISENSE",8.225,0,0,1,0,"A"},
    {40, NO_GROUP_SHARED, 6.81,10,"SYS_5V",1,0,0,1,0,"V"},
    {37, NO_GROUP_SHARED, 0,1,"SYS_0V85",1,0,0,1,0,"V"},
    {42, NO_GROUP_SHARED, 0,1,"SNN",1,0,0,1,0,"V"},
    {47, NO_GROUP_SHARED, 0,1,"SYS_0V92",1,0,0,1,0,"V"},
    {36, NO_GROUP_SHARED, 0,1,"SNN",1,0,0,1,0,"V"},
    {46, NO_GROUP_SHARED, 0,1,"SYS_1V1",1,0,0,1,0,"V"},
    {35, NO_GROUP_SHARED, 0,1,"SYS_1V2",1,0,0,1,0,"V"},
    {45, NO_GROUP_SHARED, 0,1,"SYS_1V5",1,0,0,1,0,"V"},
    {39, NO_GROUP_SHARED, 0,1,"SNN",1,0,0,1,0,"V"},
    {44, NO_GROUP_SHARED, 0,1,"SYS_1V8",1,0,0,1,0,"V"},
    {34, NO_GROUP_SHARED, 0,1,"SYS_2V1",1,0,0,1,0,"V"},
    {41, NO_GROUP_SHARED, 0,1,"SNN",1,0,0,1,0,"V"},
    {38, NO_GROUP_SHARED, 1,10,"SNN",1,0,0,1,0,"V"},
    {43, NO_GROUP_SHARED, 1,10,"SYS_3V3",1,0,0,1,0,"V"},
    {12, NO_GROUP_SHARED, 20,5.11,"SNN",1,0,0,1,0,"V"},

    {DISPLAY_ONLY_CHANNEL, NO_GROUP_SHARED, 0, 0, "Tegra A:",0,0,0,0,0,""},
    {21, NO_GROUP_SHARED, 49.9,10,"VBAT_TEG",1,0,0,1,0,"V"},
    {17, NO_GROUP_SHARED, 0,1,"VBATTEG_ISENSE",8.225,0,0,1,0,"A"},
    {24, NO_GROUP_SHARED, 20,5.11,"XA_PREREG",1,0,0,1,0,"V"},
    {25, NO_GROUP_SHARED, 6.81,10,"XA_5V",1,0,0,1,0,"V"},
    {26, NO_GROUP_SHARED, 6.81,10,"XA_5V_SW",1,0,0,1,0,"V"},
};


const IfxVadc_Pin* IfxVadc_AN = IfxVadc_AN_P3570;

uint32 VadcAutoScan_getPinNumber(void)
{
//    board.aurix_index = MAIN_AURIX;
    if (board.aurix_index == MAIN_AURIX)
    {
        return ARRAY_SIZE(IfxVadc_AN_P3570);
    }
    else
    {
        return ARRAY_SIZE(IfxVadc_AN_P3570_AURIX2);
    }
}

void VadcAutoScan_initConfig(void)
{
    if (board.aurix_index == MAIN_AURIX)
    {
        IfxVadc_AN = IfxVadc_AN_P3570;
    }
    else
    {
        IfxVadc_AN = IfxVadc_AN_P3570_AURIX2;
    }
}

#endif //endof #if DRIVE_AX_B0x == 1

#endif //endof #if PROJECT == PROJECT_DRIVE_AX

