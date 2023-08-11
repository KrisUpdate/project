/**
 * \file IfxMtu.c
 * \brief MTU  basic functionality
 *
 * \version iLLD_1_0_1_9_0
 * \copyright Copyright (c) 2018 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxMtu.h"

/******************************************************************************/
/*----------------------------------Macros------------------------------------*/
/******************************************************************************/

/** \brief Set Global safety endinit.\n
 * Note: IFXMTU_SET_GLOBAL_SAFETY_ENDINIT() function macros is added to avoid issues with tasking compiler       \n when higher optimization is enabled.
 */
#define IFXMTU_SET_GLOBAL_SAFETY_ENDINIT()                                                        \
    {                                                                                             \
        MODULE_SCU.SEICON0.U = (0xFFFCU << 16U) |                                                 \
                               ((MODULE_SCU.SEICON0.B.EPW ^ 0x003F) << IFX_SCU_SEICON0_EPW_OFF) | \
                               (1 << IFX_SCU_SEICON0_ENDINIT_OFF);                                \
        MODULE_SCU.SEICON0.U;                                                                     \
    }

/** \brief Clear Global safety endinit.\n
 * Note: IFXMTU_CLEAR_GLOBAL_SAFETY_ENDINIT() function macros is added to avoid issues with tasking compiler       \n when higher optimization is enabled.
 */
#define IFXMTU_CLEAR_GLOBAL_SAFETY_ENDINIT()                                                     \
    {                                                                                            \
        MODULE_SCU.SEICON0.U = (0xFFFCU << 16U) |                                                \
                               ((MODULE_SCU.SEICON0.B.EPW ^ 0x003F) << IFX_SCU_SEICON0_EPW_OFF); \
        MODULE_SCU.SEICON0.U;                                                                    \
    }

#define IFXMTU_CLEAR_SRAM_MCONTROL_CONFIG (0x4000 | (1U << IFX_MTU_MC_MCONTROL_SRAM_CLR_OFF))

/** \addtogroup IfxLld_Mtu_Std_Operative
 * \{ */

/******************************************************************************/
/*-----------------------Private Function Prototypes--------------------------*/
/******************************************************************************/

/** \brief
 * \param mbistConfig pointer to the configuration structure for the RAM test
 * \return None
 */
IFX_STATIC void IfxMtu_runMbistConfigMtest(const IfxMtu_MbistConfig *mbistConfig);

/** \brief
 * \param mbistConfig pointer to the configuration structure for the RAM test
 * \return None
 */
IFX_STATIC void IfxMtu_runMbistConfigRange(const IfxMtu_MbistConfig *mbistConfig);

/** \brief
 * \param mbistConfig pointer to the configuration structure for the RAM test
 * \return None
 */
IFX_STATIC void IfxMtu_runMbistClearRam(const IfxMtu_MbistConfig *mbistConfig);

/** \brief
 * \param mbistConfig pointer to the configuration structure for the RAM test
 * \return None
 */
IFX_STATIC void IfxMtu_runMbistConfigPatterns(const IfxMtu_MbistConfig *mbistConfig);

/** \brief
 * \param testsDone tests Done status
 * \return test done status
 */
IFX_STATIC boolean IfxMtu_runMbistEnd(uint32 testsDone);

/** \} */

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxMtu_clearErrorTracking(IfxMtu_MbistSel mbistSel)
{
    Ifx_MTU_MC *mc = &MODULE_MTU.MC[mbistSel];
    mc->ECCD.U |= (1 << IFX_MTU_MC_ECCD_TRC_OFF);
}


void IfxMtu_clearSram(IfxMtu_MbistSel mbistSel)
{
    uint8  isEndInitEnabled = 0;
    uint16 password         = 0;

    password = IfxScuWdt_getSafetyWatchdogPassword();

    /* Check if the Endinit is cleared by application. If not, then handle it internally inside teh function.*/
    if (IfxScuWdt_getSafetyWatchdogEndInit() == 1U)
    {
        /* Clear EndInit */
        IfxScuWdt_clearSafetyEndinit(password);
        isEndInitEnabled = 1;
    }

    IfxMtu_clearSramStart(mbistSel);

    /* Set EndInit Watchdog (to prevent Watchdog TO)*/
    IfxScuWdt_setSafetyEndinit(password);

    /* wait for the end of the fill operation */

    while (!IfxMtu_isMbistDone(mbistSel))
    {
        __nop();
    }

    /* Clear EndInit */
    IfxScuWdt_clearSafetyEndinit(password);

    IfxMtu_clearSramContinue(mbistSel);

    if (isEndInitEnabled == 1)
    {
        /* Set EndInit Watchdog (to prevent Watchdog TO)*/
        IfxScuWdt_setSafetyEndinit(password);
    }
}


void IfxMtu_clearSramContinue(IfxMtu_MbistSel mbistSel)
{
    /* Before clearing the ECC error flags we've to issue a dummy SRAM access to get a valid memory output */
    IfxMtu_readSramAddress(mbistSel, 0x0000);
    /* Note: a SMU alarm will be flagged HERE if the wrong ECC has been written! */
    IfxMtu_disableMbistShell(mbistSel);

    /* for auto-init memories: wait for the end of the clear operation */
    while (IfxMtu_isAutoInitRunning(mbistSel))
    {}
}


void IfxMtu_clearSramStart(IfxMtu_MbistSel mbistSel)
{
    Ifx_MTU_MC *mc = &MODULE_MTU.MC[mbistSel];

    IfxMtu_enableMbistShell(mbistSel);

    /* for auto-init memories: wait for the end of the clear operation */
    while (IfxMtu_isAutoInitRunning(mbistSel))
    {}

    /* start init operation */
    uint16 mcontrolMask = 0x4000;                                                                                   /* set USERED flag */
    mc->MCONTROL.U = mcontrolMask | (1 << IFX_MTU_MC_MCONTROL_SRAM_CLR_OFF) | (1 << IFX_MTU_MC_MCONTROL_START_OFF); /* START = SRAM_CLR = 1 */
    mc->MCONTROL.U = mcontrolMask | (1 << IFX_MTU_MC_MCONTROL_SRAM_CLR_OFF);
}


void IfxMtu_enableErrorTracking(IfxMtu_MbistSel mbistSel, boolean enable)
{
    Ifx_MTU_MC *mc = &MODULE_MTU.MC[mbistSel];

    if (enable == FALSE)
    {
        mc->ECCS.U &= ~(1 << IFX_MTU_MC_ECCS_TRE_OFF);
    }
    else
    {
        mc->ECCS.U |= (1 << IFX_MTU_MC_ECCS_TRE_OFF);
    }
}


uint32 IfxMtu_getSystemAddress(IfxMtu_MbistSel mbistSel, Ifx_MTU_MC_ETRR trackedSramAddress)
{
    uint32 sramAddress   = trackedSramAddress.B.ADDR;
    uint32 mbi           = trackedSramAddress.B.MBI;
    uint32 systemAddress = 0;

    switch (mbistSel)
    {
    case IfxMtu_MbistSel_cpu0Pspr:
        systemAddress = 0x70100000 | ((sramAddress << 5) | ((mbi & 1) << 4));
        break;

    case IfxMtu_MbistSel_cpu0Dspr:
        systemAddress = 0x70000000 | ((sramAddress << 4) | ((mbi & 3) << 2));
        break;
    case IfxMtu_MbistSel_cpu1Pspr:
        systemAddress = 0x60100000 | ((sramAddress << 5) | ((mbi & 1) << 4));
        break;

    case IfxMtu_MbistSel_cpu1Dspr:
        systemAddress = 0x60000000 | ((sramAddress << 4) | ((mbi & 3) << 2));
        break;
    case IfxMtu_MbistSel_cpu2Pspr:
        systemAddress = 0x50100000 | ((sramAddress << 5) | ((mbi & 1) << 4));
        break;

    case IfxMtu_MbistSel_cpu2Dspr:
        systemAddress = 0x50000000 | ((sramAddress << 4) | ((mbi & 3) << 2));
        break;
    case IfxMtu_MbistSel_cpu3Pspr:
        systemAddress = 0x40100000 | ((sramAddress << 5) | ((mbi & 1) << 4));
        break;
    case IfxMtu_MbistSel_cpu3Dspr:
        systemAddress = 0x40000000 | ((sramAddress << 4) | ((mbi & 3) << 2));
        break;
    case IfxMtu_MbistSel_cpu4Pspr:
        systemAddress = 0x30100000 | ((sramAddress << 5) | ((mbi & 1) << 4));
        break;

    case IfxMtu_MbistSel_cpu4Dspr:
        systemAddress = 0x30000000 | ((sramAddress << 4) | ((mbi & 3) << 2));
        break;

    case IfxMtu_MbistSel_cpu5Pspr:
        systemAddress = 0x10100000 | ((sramAddress << 5) | ((mbi & 1) << 4));
        break;

    case IfxMtu_MbistSel_cpu5Dspr:
        systemAddress = 0x10000000 | ((sramAddress << 4) | ((mbi & 3) << 2));
        break;
    case IfxMtu_MbistSel_lmu0:
        systemAddress = 0xb0040000 | ((sramAddress << 5) | ((mbi & 1) << 4));
        break;

    case IfxMtu_MbistSel_cpu0Dlmu:
        systemAddress = 0xb0000000 | (sramAddress << 4);
        break;
    case IfxMtu_MbistSel_cpu1Dlmu:
        systemAddress = 0xb0010000 | (sramAddress << 4);
        break;
    case IfxMtu_MbistSel_cpu2Dlmu:
        systemAddress = 0xb0020000 | (sramAddress << 4);
        break;
    case IfxMtu_MbistSel_cpu3Dlmu:
        systemAddress = 0xb0030000 | (sramAddress << 4);
        break;
    case IfxMtu_MbistSel_cpu4Dlmu:
        systemAddress = 0xb0140000 | (sramAddress << 4);
        break;

    case IfxMtu_MbistSel_cpu5Dlmu:
        systemAddress = 0xb0150000 | (sramAddress << 4);
        break;
    case IfxMtu_MbistSel_dma:
        systemAddress = 0xf0012000 | ((sramAddress << 5) | ((mbi & 3) << 3));
        break;
    case IfxMtu_MbistSel_mcds:
        systemAddress = 0xFA010000 | ((sramAddress << 2) | ((mbi & 3) << 2));
        break;
    case IfxMtu_MbistSel_ememUpper0:
        systemAddress = 0x99200000 | (sramAddress << 5);
        break;
    case IfxMtu_MbistSel_ememUpper1:
        systemAddress = 0x99300000 | (sramAddress << 5);
        break;
    case IfxMtu_MbistSel_ememXtm:
        systemAddress = 0x99400000 | (sramAddress << 5);
        break;
    case IfxMtu_MbistSel_spu0Config:
        systemAddress = 0xFAA00000 | ((sramAddress << 5) | ((mbi & 1) << 3));
        break;
    case IfxMtu_MbistSel_spu1Config:
        systemAddress = 0xFAE00000 | ((sramAddress << 5) | ((mbi & 1) << 3));
        break;
    case IfxMtu_MbistSel_gtmFifo:
        systemAddress = 0xf0119000 | (sramAddress << 2);
        break;
    case IfxMtu_MbistSel_gtmDpll1a:
        systemAddress = 0xf0128200 | (sramAddress << 2);
        break;
    case IfxMtu_MbistSel_gtmDpll1b:
        systemAddress = 0xf0128400 | (sramAddress << 2);
        break;
    case IfxMtu_MbistSel_gtmDpll2:
        systemAddress = 0xf012c000 | (sramAddress << 2);
        break;
    case IfxMtu_MbistSel_mcan0:
        systemAddress = 0xF0200000 | (sramAddress << 2);
        break;
    case IfxMtu_MbistSel_mcan1:
        systemAddress = 0xF0210000 | (sramAddress << 2);
        break;
    case IfxMtu_MbistSel_mcan2:
        systemAddress = 0xF0220000 | (sramAddress << 2);
        break;
    case IfxMtu_MbistSel_psi5:
        systemAddress = 0xf0005600 | (sramAddress << 2);
        break;
    case IfxMtu_MbistSel_scrXram:
        systemAddress = 0xF0240000 | (sramAddress << 2);
        break;
    case IfxMtu_MbistSel_scrIram:
    case IfxMtu_MbistSel_eray0Obf:
    case IfxMtu_MbistSel_eray0IbfTbf:
    case IfxMtu_MbistSel_eray0Mbf:
    case IfxMtu_MbistSel_eray1Obf:
    case IfxMtu_MbistSel_eray1IbfTbf:
    case IfxMtu_MbistSel_eray1Mbf:
    case IfxMtu_MbistSel_gtmMcs0Slow:
    case IfxMtu_MbistSel_gtmMcs0Fast:
    case IfxMtu_MbistSel_gtmMcs1Slow:
    case IfxMtu_MbistSel_gtmMcs1Fast:
    case IfxMtu_MbistSel_spu0Fft0:
    case IfxMtu_MbistSel_spu0Fft1:
    case IfxMtu_MbistSel_spu0Fft2:
    case IfxMtu_MbistSel_spu0Fft3:
    case IfxMtu_MbistSel_spu1Fft0:
    case IfxMtu_MbistSel_spu1Fft1:
    case IfxMtu_MbistSel_spu1Fft2:
    case IfxMtu_MbistSel_spu1Fft3:
    case IfxMtu_MbistSel_spu0Buffer:
    case IfxMtu_MbistSel_spu1Buffer:
    default:
        systemAddress = 0; /* unsupported address descrambling */
    }

    return systemAddress;
}


uint8 IfxMtu_getTrackedSramAddresses(IfxMtu_MbistSel mbistSel, Ifx_MTU_MC_ETRR *trackedSramAddresses)
{
    Ifx_MTU_MC *mc                  = &MODULE_MTU.MC[mbistSel];
    uint8       validFlags          = (mc->ECCD.U >> IFX_MTU_MC_ECCD_VAL_OFF) & IFX_MTU_MC_ECCD_VAL_MSK;
    uint8       numTrackedAddresses = 0;
    int         i;

#if IFX_MC_ECCD_VAL_LEN > IFXMTU_MAX_TRACKED_ADDRESSES
# error "Unexpected size of VAL mask"
#endif

    for (i = 0; i < IFXMTU_MAX_TRACKED_ADDRESSES; ++i)
    {
        if (validFlags & (1 << i))
        {
            trackedSramAddresses[numTrackedAddresses].U = mc->ETRR[i].U;
            ++numTrackedAddresses;
        }
    }

    return numTrackedAddresses;
}


boolean IfxMtu_isTestPass(const IfxMtu_MbistConfig *mbistConfig)
{
    boolean retVal = 1;
    uint32  sshConfigCount;

    for (sshConfigCount = 0; sshConfigCount < mbistConfig->numOfSshConfigurations; sshConfigCount++)
    {
        Ifx_MTU_MC *mc = &MODULE_MTU.MC[mbistConfig->sshConfigurations[sshConfigCount].sshSel];

        if (mc->ECCD.U & (uint32)((IFX_MTU_MC_ECCD_EOV_MSK << IFX_MTU_MC_ECCD_EOV_OFF) |   \
                                  (IFX_MTU_MC_ECCD_MERR_MSK << IFX_MTU_MC_ECCD_MERR_OFF) | \
                                  (IFX_MTU_MC_ECCD_UCERR_MSK << IFX_MTU_MC_ECCD_UCERR_OFF)))
        {
            retVal = 0;
        }
    }

    return retVal;
}


void IfxMtu_readSramAddress(IfxMtu_MbistSel mbistSel, uint16 sramAddress)
{
    Ifx_MTU_MC *mc = &MODULE_MTU.MC[mbistSel];

    /* configure MBIST for single read opeation */
    uint16      mcontrolMask = 0x4000;                                                               /* set USERED flag */
    mc->MCONTROL.U = mcontrolMask | (1 << IFX_MTU_MC_MCONTROL_DIR_OFF);
    mc->CONFIG0.U  = (1 << IFX_MTU_MC_CONFIG0_NUMACCS_OFF) | (1 << IFX_MTU_MC_CONFIG0_ACCSTYPE_OFF); /* 1 read access */
    mc->CONFIG1.U  = 0;                                                                              /* ensure that linear scrambling is used */

    /* Set the address to be read (RAEN = 0) */
    mc->RANGE.U = sramAddress;

    /* Start operation */
    mc->MCONTROL.U = mcontrolMask | (1 << IFX_MTU_MC_MCONTROL_DIR_OFF) | (1 << IFX_MTU_MC_MCONTROL_START_OFF);
    mc->MCONTROL.U = mcontrolMask | (1 << IFX_MTU_MC_MCONTROL_DIR_OFF);

    /* wait for the end of the fill operation */
    while (!IfxMtu_isMbistDone(mbistSel))
    {
        __nop();
    }
}


uint8 IfxMtu_runCheckerBoardTest(IfxMtu_MbistSel mbistSel, uint8 rangeSel, uint8 rangeAddrUp, uint8 rangeAddrLow, uint16 *errorAddr, uint32 numberRedundancyLines)
{
    /* Select MBIST Memory Controller:
     * Ifx_MC is a type describing structure of MBIST Memory Controller
     * registers defined in IfxMc_regdef.h file - MC object */
    Ifx_MTU_MC *mc                            = &MODULE_MTU.MC[mbistSel];
    uint32      configCheckerBoardSequence[4] = {
        0x08001000, //up /lin/w0
        0x08001001, //up /lin/r0
        0x00011000, //down/lin/w1
        0x00011001
    };              //down/lin/r1
    uint16      password         = 0;
    uint8       retVal           = 0U;
    uint8       testStep;
    uint8       isEndInitEnabled = 0;
    password = IfxScuWdt_getSafetyWatchdogPassword();

    /* Check if the Endinit is cleared by application. If not, then handle it internally inside teh function.*/
    if (IfxScuWdt_getSafetyWatchdogEndInit() == 1U)
    {
        /* Clear EndInit */
        IfxScuWdt_clearSafetyEndinit(password);
        isEndInitEnabled = 1;
    }

    /* Enable MBIST Memory Controller */
    IfxMtu_enableMbistShell(mbistSel);

    /* for auto-init memories: wait for the end of the clear operation */
    while (IfxMtu_isAutoInitRunning(mbistSel))
    {}

    /* Set the range register */
    mc->RANGE.U = (rangeSel << 15) | (rangeAddrUp << 7) | (rangeAddrLow << 0);

    /* Run the test */
    for (testStep = 0; testStep < 4; ++testStep)
    {
        mc->CONFIG0.U  = configCheckerBoardSequence[testStep] & 0x0000FFFF;
        mc->CONFIG1.U  = (configCheckerBoardSequence[testStep] & 0xFFFF0000) >> 16;
        mc->MCONTROL.U = numberRedundancyLines ? 0x40c9 : 0x00c9; // bit and row toggle
        mc->MCONTROL.U = numberRedundancyLines ? 0x40c8 : 0x00c8; // MCONTROL.B.START will generate a RMW which is too long for small SRAMs!

        /* Set EndInit Watchdog (to prevent Watchdog TO)*/
        IfxScuWdt_setSafetyEndinit(password);

        /* wait for the end of the fill operation */

        while (!IfxMtu_isMbistDone(mbistSel))
        {
            __nop();
        }

        /* Clear EndInit Again */
        IfxScuWdt_clearSafetyEndinit(password);

        /* Check the Fail Status */
        if (mc->MSTATUS.B.FAIL)
        {
            /* Test has failed, check if any un-correctable error */
            if (mc->ECCD.B.UCERR)
            {
                /* Read Error tracking register and return saying test failed */
                *errorAddr = mc->ETRR[0].U;
                retVal     = 1U;
                break;
            }
        }
    }

    /* Disable Memory Controller */

    IfxMtu_disableMbistShell(mbistSel);

    while (IfxMtu_isAutoInitRunning(mbistSel))
    {}

    if (isEndInitEnabled == 1)
    {
        /* Set EndInit Watchdog (to prevent Watchdog TO)*/
        IfxScuWdt_setSafetyEndinit(password);
    }

    return retVal;
}


uint8 IfxMtu_runMarchUTest(IfxMtu_MbistSel mbistSel, uint8 rangeSel, uint8 rangeAddrUp, uint8 rangeAddrLow, uint16 *errorAddr)
{
    /* Select MBIST Memory Controller:
     * Ifx_MC is a type describing structure of MBIST Memory Controller
     * registers defined in IfxMc_regdef.h file - MC object */
    Ifx_MTU_MC *mc                      = &MODULE_MTU.MC[mbistSel];
    uint32      configMarchUSequence[6] = {
        0x08001000, //up /lin/w0
        0x08064005, //up /lin/r0->w1->r1->w0
        0x08022001, //up /lin/r0->w1
        0x00094005, //down/lin/r1->w0->r0->w1
        0x00012001, //down/lin/r1->w0
        0x00001001
    };              //down/lin/r0
    uint16      password         = 0;
    uint8       retVal           = 0U;
    uint8       testStep;
    uint8       isEndInitEnabled = 0;
    password = IfxScuWdt_getSafetyWatchdogPassword();

    /* Check if the Endinit is cleared by application. If not, then handle it internally inside teh function.*/
    if (IfxScuWdt_getSafetyWatchdogEndInit() == 1U)
    {
        /* Clear EndInit */
        IfxScuWdt_clearSafetyEndinit(password);
        isEndInitEnabled = 1;
    }

    /* Enable MBIST Memory Controller */
    IfxMtu_enableMbistShell(mbistSel);

    /* for auto-init memories: wait for the end of the clear operation */
    while (IfxMtu_isAutoInitRunning(mbistSel))
    {}

    /* Set the range register */
    mc->RANGE.U = (rangeSel << 15) | (rangeAddrUp << 7) | (rangeAddrLow << 0);

    /* Run the test */
    for (testStep = 0; testStep < 6; ++testStep)
    {
        mc->CONFIG0.U        = configMarchUSequence[testStep] & 0x0000FFFF;
        mc->CONFIG1.U        = (configMarchUSequence[testStep] & 0xFFFF0000) >> 16;
        mc->MCONTROL.U       = 0x4209;
        mc->MCONTROL.B.START = 0;

        /* Set EndInit Watchdog (to prevent Watchdog TO)*/
        IfxScuWdt_setSafetyEndinit(password);

        /* wait for the end of the fill operation */

        while (!IfxMtu_isMbistDone(mbistSel))
        {
            __nop();
        }

        /* Clear EndInit Again */
        IfxScuWdt_clearSafetyEndinit(password);

        /* Check the Fail Status */
        if (mc->MSTATUS.B.FAIL)
        {
            /* Test has failed, check if any un-correctable error */
            if (mc->ECCD.B.UCERR)
            {
                /* Read Error tracking register and return saying test failed */
                *errorAddr = mc->ETRR[0].U;
                retVal     = 1U;
                break;
            }
        }
    }

    /* Disable Memory Controller */
    IfxMtu_disableMbistShell(mbistSel);

    /* for auto-init memories: wait for the end of the clear operation */
    while (IfxMtu_isAutoInitRunning(mbistSel))
    {}

    /* Restore the endinit state */
    if (isEndInitEnabled == 1)
    {
        /* Set EndInit Watchdog (to prevent Watchdog TO)*/
        IfxScuWdt_setSafetyEndinit(password);
    }

    return retVal;
}


uint8 IfxMtu_runNonDestructiveInversionTest(IfxMtu_MbistSel mbistSel, uint8 rangeSel, uint8 rangeAddrUp, uint8 rangeAddrLow, uint16 *errorAddr)
{
    /* Select MBIST Memory Controller:
     * Ifx_MC is a type describing structure of MBIST Memory Controller
     * registers defined in IfxMc_regdef.h file - MC object */
    Ifx_MTU_MC *mc               = &MODULE_MTU.MC[mbistSel];
    uint16      password         = 0;
    uint8       retVal           = 0U;
    uint8       isEndInitEnabled = 0;
    password = IfxScuWdt_getSafetyWatchdogPassword();

    /* Check if the Endinit is cleared by application. If not, then handle it internally inside teh function.*/
    if (IfxScuWdt_getSafetyWatchdogEndInit() == 1U)
    {
        /* Clear EndInit */
        IfxScuWdt_clearSafetyEndinit(password);
        isEndInitEnabled = 1;
    }

    /* Enable MBIST Memory Controller */
    IfxMtu_enableMbistShell(mbistSel);

    /* for auto-init memories: wait for the end of the clear operation */
    while (IfxMtu_isAutoInitRunning(mbistSel))
    {}

    /* Configure Non-destructive Inversion test */
    mc->CONFIG0.U        = 0x4005; //NUMACCS=4, ACCSTYPE=5
    mc->CONFIG1.U        = 0x5008; //AG_MOD=5
    /* Set the range register */
    mc->RANGE.U          = (rangeSel << 15) | (rangeAddrUp << 7) | (rangeAddrLow << 0);
    /* Run the tests */
    mc->MCONTROL.U       = 0x4009;
    mc->MCONTROL.B.START = 0;
    /* Set EndInit Watchdog (to prevent Watchdog TO)*/
    IfxScuWdt_setSafetyEndinit(password);

    /* wait for the end of the fill operation */

    while (!IfxMtu_isMbistDone(mbistSel))
    {
        __nop();
    }

    /* Clear EndInit Again */
    IfxScuWdt_clearSafetyEndinit(password);

    /* Check the Fail Status */
    if (IfxMtu_checkErrorFlags(mbistSel))
    {
        /* Read the Error tracking register and return saying test failed */
        *errorAddr = mc->ETRR[0].U;
        retVal     = 1U;
    }

    /* Disable Memory Controller */
    IfxMtu_disableMbistShell(mbistSel);

    /* for auto-init memories: wait for the end of the clear operation */
    while (IfxMtu_isAutoInitRunning(mbistSel))
    {}

    /* Restore the endinit state */
    if (isEndInitEnabled == 1)
    {
        /* Set EndInit Watchdog (to prevent Watchdog TO)*/
        IfxScuWdt_setSafetyEndinit(password);
    }

    return retVal;
}


void IfxMtu_writeSramAddress(IfxMtu_MbistSel mbistSel, uint16 sramAddress)
{
    Ifx_MTU_MC *mc               = &MODULE_MTU.MC[mbistSel];
    uint8       isEndInitEnabled = 0;
    uint16      password         = 0;
    password = IfxScuWdt_getSafetyWatchdogPassword();

    /* Check if the Endinit is cleared by application. If not, then handle it internally inside teh function.*/
    if (IfxScuWdt_getSafetyWatchdogEndInit() == 1U)
    {
        /* Clear EndInit */
        IfxScuWdt_clearSafetyEndinit(password);
        isEndInitEnabled = 1;
    }

    /* configure MBIST for single write opeation */
    uint16 mcontrolMask = 0x4000;                                                                    /* set USERED flag */
    mc->MCONTROL.U = mcontrolMask | (1 << IFX_MTU_MC_MCONTROL_DIR_OFF);
    mc->CONFIG0.U  = (1 << IFX_MTU_MC_CONFIG0_NUMACCS_OFF) | (0 << IFX_MTU_MC_CONFIG0_ACCSTYPE_OFF); /* 1 write access */
    mc->CONFIG1.U  = 0;                                                                              /* ensure that linear scrambling is used */

    /* Set the address to be written (RAEN = 0) */
    mc->RANGE.U = sramAddress;

    /* Start operation */
    mc->MCONTROL.U = mcontrolMask | (1 << IFX_MTU_MC_MCONTROL_DIR_OFF) | (1 << IFX_MTU_MC_MCONTROL_START_OFF);
    mc->MCONTROL.U = mcontrolMask | (1 << IFX_MTU_MC_MCONTROL_DIR_OFF);

    if (isEndInitEnabled == 1)
    {
        /* Set EndInit Watchdog (to prevent Watchdog TO)*/
        IfxScuWdt_setSafetyEndinit(password);
    }

    /* Wait for the end of the operation */

    while (!IfxMtu_isMbistDone(mbistSel))
    {
        __nop();
    }
}


boolean IfxMtu_runMbistAll(const IfxMtu_MbistConfig *const mbistConfig[])
{
    unsigned int gangConfigCount   = 0;
    boolean      mBistError        = 0;
    uint16       cpuWdtPassword    = IfxScuWdt_getCpuWatchdogPassword();
    uint16       safetyWdtPassword = IfxScuWdt_getSafetyWatchdogPassword();

    while (mbistConfig[gangConfigCount] != (void *)0)
    {
        if (IfxMtu_runMbist(mbistConfig[gangConfigCount]))
        {
            if (!IfxMtu_isTestPass(mbistConfig[gangConfigCount]))
            {
                mBistError = 1;
            }
        }
        else
        {
            mBistError = 1;
        }

        gangConfigCount++;
        IfxScuWdt_serviceCpuWatchdog(cpuWdtPassword);
        IfxScuWdt_serviceSafetyWatchdog(safetyWdtPassword);
    }

    return mBistError;
}


#if defined(__TASKING__)
#pragma optimize R
#elif defined(__HIGHTEC__)
#pragma GCC optimize ("-O1")
#endif
boolean IfxMtu_runMbist(const IfxMtu_MbistConfig *mbistConfig)
{
    uint16 endinitCpu_pw = IfxScuWdt_getCpuWatchdogPassword();

    if (IfxMtu_isModuleEnabled() == 0)
    {
        IfxScuWdt_clearCpuEndinit(endinitCpu_pw);
        IfxMtu_enableModule();                /* Enable MTU module */
        IfxScuWdt_setCpuEndinit(endinitCpu_pw);
    }

    __moveToAddrParam0(mbistConfig);
    __jumpToFunction((voidfuncvoid)IfxMtu_runMbistConfigMtest);

    return FALSE; /*dummy return, control shall not reach here*/
}


IFX_STATIC void IfxMtu_runMbistConfigMtest(const IfxMtu_MbistConfig *mbistConfig)
{
    uint32 memTest0 = 0;
    uint32 memTest1 = 0;
    uint32 memTest2 = 0;
    sint32 count;

    /*prepare the MTEST registers with local variables*/
    for (count = 0; count < mbistConfig->numOfSshConfigurations; count++)
    {
        /*Set MEMTESTx registers for each SSH cell configured*/
        if (mbistConfig->sshConfigurations[count].sshSel < 32)
        {
            memTest0 |= (uint32)1U << (mbistConfig->sshConfigurations[count].sshSel & 0x1FU);
        }
        else if (mbistConfig->sshConfigurations[count].sshSel < 64)
        {
            memTest1 |= (uint32)1U << (mbistConfig->sshConfigurations[count].sshSel & 0x1FU);
        }
        else
        {
            memTest2 |= (uint32)1U << (mbistConfig->sshConfigurations[count].sshSel & 0x1FU);
        }
    }

    /*Write to MEMTESTx registers*/
    IFXMTU_CLEAR_GLOBAL_SAFETY_ENDINIT();

    /*The code below this point, we shall not use any function call*/
    MTU_MEMTEST0.U = memTest0;
    MTU_MEMTEST1.U = memTest1;
    MTU_MEMTEST2.U = memTest2;

    IFXMTU_SET_GLOBAL_SAFETY_ENDINIT();

    count = IFXMTU_WAIT_TIMEOUTCOUNT;

    /* Wait until MEMTEST bits are reset */
    while ((MTU_MEMTEST0.U != memTest0) || (MTU_MEMTEST1.U != memTest1) || (MTU_MEMTEST2.U != memTest2))
    {
        if ((--count) <= 0)
        {
            count = 0;
            /*When there is some error, reset the MBISTTEST registers*/
            __moveToDataParam0(count);
            __jumpToFunction((voidfuncvoid)IfxMtu_runMbistEnd);
            // break;
        }
    }

    __moveToAddrParam0(mbistConfig);
    __jumpToFunction((voidfuncvoid)IfxMtu_runMbistConfigRange);
}


IFX_STATIC void IfxMtu_runMbistConfigRange(const IfxMtu_MbistConfig *mbistConfig)
{
    uint32 sshConfigCount;

    IFXMTU_CLEAR_GLOBAL_SAFETY_ENDINIT();

    for (sshConfigCount = 0; sshConfigCount < mbistConfig->numOfSshConfigurations; sshConfigCount++)
    {
        Ifx_MTU_MC *mc = &MODULE_MTU.MC[mbistConfig->sshConfigurations[sshConfigCount].sshSel];

        while (IfxMtu_isAutoInitRunning(mbistConfig->sshConfigurations[sshConfigCount].sshSel))
        {}

        mc->RANGE.U = (1 << 15) | mbistConfig->sshConfigurations[sshConfigCount].highAddress << 7 |
                      mbistConfig->sshConfigurations[sshConfigCount].lowAddress;

        /* Clear the alarms, which are set during bootup, applicable for 39xB, 38x, 37x and 35x devices only */
        mc->ALMSRCS.B.MISCE = 0U;
        mc->ALMSRCS.B.OPENE = 0U;
        /*Clear all the error bits*/
        mc->ECCD.U          = 0x10;
        /*Enable Error tracking*/
        mc->ECCS.U          = 0x001F;
    }

    IFXMTU_SET_GLOBAL_SAFETY_ENDINIT();

    __moveToAddrParam0(mbistConfig);
    __jumpToFunction((voidfuncvoid)IfxMtu_runMbistClearRam);
}


IFX_STATIC void IfxMtu_runMbistClearRam(const IfxMtu_MbistConfig *mbistConfig)
{
    uint32      patternCount;
    uint32      sshConfigCount;
    uint32      status = 0;
    sint32      count;

    Ifx_MTU_MC *mc;

    /*Loop through each configuration pattern*/
    for (patternCount = 0; patternCount < mbistConfig->numOfConfigPatterns; patternCount++)
    {
        /** Setup Configuration pattern for each SSH CONFIGx*/
        IFXMTU_CLEAR_GLOBAL_SAFETY_ENDINIT();

        for (sshConfigCount = 0; sshConfigCount < mbistConfig->numOfSshConfigurations; sshConfigCount++)
        {
            if (mbistConfig->sshConfigurations[sshConfigCount].preClearEnable)
            {
                mc             = &MODULE_MTU.MC[mbistConfig->sshConfigurations[sshConfigCount].sshSel];
                mc->MCONTROL.U = IFXMTU_CLEAR_SRAM_MCONTROL_CONFIG | (1 << IFX_MTU_MC_MCONTROL_START_OFF); // Configure the MCONTROL and set Start bit
                mc->MCONTROL.U = IFXMTU_CLEAR_SRAM_MCONTROL_CONFIG;                                        //Reset the start bit
            }
        }

        IFXMTU_SET_GLOBAL_SAFETY_ENDINIT();

        count = IFXMTU_WAIT_TIMEOUTCOUNT;

        do
        {
            status = 1;

            for (sshConfigCount = 0; sshConfigCount < mbistConfig->numOfSshConfigurations; sshConfigCount++)
            {
                if (mbistConfig->sshConfigurations[sshConfigCount].preClearEnable)
                {
                    Ifx_MTU_MC *mc = &MODULE_MTU.MC[mbistConfig->sshConfigurations[sshConfigCount].sshSel];
                    status &= (mc->MSTATUS.B.DONE & (!IfxMtu_isAutoInitRunning(mbistConfig->sshConfigurations[sshConfigCount].sshSel)));
                }
            }

            if ((--count) <= 0)
            {
                __moveToDataParam0(status);
                __jumpToFunction((voidfuncvoid)IfxMtu_runMbistEnd);
            }
        } while (status == 0);
    }

    __moveToAddrParam0(mbistConfig);
    __jumpToFunction((voidfuncvoid)IfxMtu_runMbistConfigPatterns);
}


IFX_STATIC void IfxMtu_runMbistConfigPatterns(const IfxMtu_MbistConfig *mbistConfig)
{
    uint32      patternCount;
    uint32      sshConfigCount;
    uint32      status = 0;
    sint32      count;

    Ifx_MTU_MC *mc;

    /*Loop through each configuration pattern*/
    for (patternCount = 0; patternCount < mbistConfig->numOfConfigPatterns; patternCount++)
    {
        /** Setup Configuration pattern for each SSH CONFIGx*/
        IFXMTU_CLEAR_GLOBAL_SAFETY_ENDINIT();

        for (sshConfigCount = 0; sshConfigCount < mbistConfig->numOfSshConfigurations; sshConfigCount++)
        {
            mc            = &MODULE_MTU.MC[mbistConfig->sshConfigurations[sshConfigCount].sshSel];
            mc->CONFIG0.U = mbistConfig->configPatterns[patternCount].config0.U; /* Set the CONFIG0 register */
            mc->CONFIG1.U = mbistConfig->configPatterns[patternCount].config1.U; /* Set the CONFIG1 register */
        }

        /* Start memory test for each SSH CONFIGx*/

        for (sshConfigCount = 0; sshConfigCount < mbistConfig->numOfSshConfigurations; sshConfigCount++)
        {
            mc             = &MODULE_MTU.MC[mbistConfig->sshConfigurations[sshConfigCount].sshSel];
            mc->MCONTROL.U = mbistConfig->mcontrol.U | 1; // Configure the MCONTROL and set Start bit

            mc->MCONTROL.U = mbistConfig->mcontrol.U;     //Reset the start bit
        }

        IFXMTU_SET_GLOBAL_SAFETY_ENDINIT();

        count = IFXMTU_WAIT_TIMEOUTCOUNT;

        do
        {
            status = 1;

            for (sshConfigCount = 0; sshConfigCount < mbistConfig->numOfSshConfigurations; sshConfigCount++)
            {
                Ifx_MTU_MC *mc = &MODULE_MTU.MC[mbistConfig->sshConfigurations[sshConfigCount].sshSel];
                status &= ((mc->MSTATUS.B.DONE | mc->MSTATUS.B.FAIL) & (!IfxMtu_isAutoInitRunning(mbistConfig->sshConfigurations[sshConfigCount].sshSel)));
            }

            if ((--count) <= 0)
            {
                __moveToDataParam0(status);
                __jumpToFunction((voidfuncvoid)IfxMtu_runMbistEnd);
            }
        } while (status == 0);
    }

    __moveToDataParam0(status);
    __jumpToFunction((voidfuncvoid)IfxMtu_runMbistEnd);
}


IFX_STATIC boolean IfxMtu_runMbistEnd(uint32 testsDone)
{
    sint32 timeoutCount = IFXMTU_WAIT_TIMEOUTCOUNT;

    /*ReSet MEMTESTx registers for each SSH cell configured*/
    IFXMTU_CLEAR_GLOBAL_SAFETY_ENDINIT();

    MTU_MEMTEST0.U = 0;
    MTU_MEMTEST1.U = 0;
    MTU_MEMTEST2.U = 0;

    IFXMTU_SET_GLOBAL_SAFETY_ENDINIT();

    /*Check if the MTEST is actually reset*/
    {
        while ((MTU_MEMTEST0.U > 0) || (MTU_MEMTEST1.U > 0) || (MTU_MEMTEST2.U > 0)) /* Wait until the MEMTEST bits are still set */
        {
            if ((--timeoutCount) <= 0)
            {
                testsDone = 0;
                break;
            }
        }
    }

    return (boolean)testsDone;
}
#if defined(__TASKING__)
#pragma endoptimize
#elif defined(__HIGHTEC__)
#pragma GCC reset_options
#endif
