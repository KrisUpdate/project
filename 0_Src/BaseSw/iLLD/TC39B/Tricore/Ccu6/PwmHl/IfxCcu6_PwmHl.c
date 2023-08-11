/**
 * \file IfxCcu6_PwmHl.c
 * \brief CCU6 PWMHL details
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
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxCcu6_PwmHl.h"
#include "_Utilities/Ifx_Assert.h"
#include "stddef.h"
#include "string.h"

/** \addtogroup IfxLld_Ccu6_PwmHl_StdIf_Functions
 * \{ */

/******************************************************************************/
/*-----------------------Private Function Prototypes--------------------------*/
/******************************************************************************/

/** \brief Turn-off the complementary PWM generation mode.
 *
 * \note The changes are not applied until Timer->applyUpdate() is called.
 * \note This function will set the ON time to ZERO, overriding previous call to
 * IfxCcu6_PwmHl_setOnTime().
 * \note This function will disable PWM in all channels, overriding previous call to
 * IfxCcu6_PwmHl_setupChannels().
 * \param driver CCU6 PWM driver
 * \return None
 */
IFX_STATIC void IfxCcu6_PwmHl_disable(IfxCcu6_PwmHl *driver);

/** \brief Turn-on the complementary PWM generation mode.
 *
 * \note The changes are not applied until Timer->applyUpdate() is called.
 * \note This function will disable PWM in all channels, overriding previous call to
 * IfxCcu6_PwmHl_setupChannels().
 * \param driver CCU6 PWM driver
 * \return None
 */
IFX_STATIC void IfxCcu6_PwmHl_enable(IfxCcu6_PwmHl *driver);

/** \brief Sets up the output pins
 * \param module Pointer to register base
 * \param config CCU6 : PWM HL configuration
 * \return None
 */
IFX_STATIC void IfxCcu6_PwmHl_setOutputPins(Ifx_CCU6 *module, const IfxCcu6_PwmHl_Config *config);

/** \brief Updates the x output duty cycle in center aligned and center aligned inverted modes
 * \param driver CCU6 PWM driver
 * \param tOn T on
 * \return None
 */
IFX_STATIC void IfxCcu6_PwmHl_updateCenterAligned(IfxCcu6_PwmHl *driver, Ifx_TimerValue *tOn);

/** \brief Updates the x output duty cycle in edge aligned modes (left and right aligned)
 * \param driver CCU6 PWM driver
 * \param tOn T on
 * \return None
 */
IFX_STATIC void IfxCcu6_PwmHl_updateEdgeAligned(IfxCcu6_PwmHl *driver, Ifx_TimerValue *tOn);

/** \brief Set the outputs to inactive
 * \param driver CCU6 PWM driver
 * \param tOn T on
 * \return None
 */
IFX_STATIC void IfxCcu6_PwmHl_updateOff(IfxCcu6_PwmHl *driver, Ifx_TimerValue *tOn);

/** \} */

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

IFX_STATIC IFX_CONST IfxCcu6_PwmHl_Mode IfxCcu6_PwmHl_modes[Ifx_Pwm_Mode_off + 1] = {
    {Ifx_Pwm_Mode_centerAligned,         FALSE, &IfxCcu6_PwmHl_updateCenterAligned},
    {Ifx_Pwm_Mode_centerAlignedInverted, TRUE,  &IfxCcu6_PwmHl_updateCenterAligned},
    {Ifx_Pwm_Mode_leftAligned,           FALSE, &IfxCcu6_PwmHl_updateEdgeAligned  },
    {Ifx_Pwm_Mode_rightAligned,          TRUE,  &IfxCcu6_PwmHl_updateEdgeAligned  },
    {Ifx_Pwm_Mode_off,                   FALSE, &IfxCcu6_PwmHl_updateOff          }
};

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

IFX_STATIC void IfxCcu6_PwmHl_disable(IfxCcu6_PwmHl *driver)
{
    IfxCcu6_TimerWithTrigger *timer  = driver->timer;
    Ifx_CCU6                 *module = timer->ccu6;
    Ifx_CCU6_CMPSTAT          cmpstat;

    Ifx_TimerValue            tOn[3] = {0, 0, 0};
    IfxCcu6_PwmHl_setOnTime(driver, tOn); /* FIXME move to IfxCcu6_PwmHl_updateOff() */

    /** - All TOP and BOTTOM channel (COUTx) states equals CC6xST */
    cmpstat.U = 0;
    //cmpstat.B.T13IM = 0;
    //cmpstat.B.COUT63PS = 0;
    //cmpstat.B.CC60PS = 0;
    //cmpstat.B.CC61PS = 0;
    //cmpstat.B.CC62PS = 0;
    //cmpstat.B.COUT60PS = 0;
    //cmpstat.B.COUT61PS = 0;
    //cmpstat.B.COUT62PS = 0;
    module->CMPSTAT.U = cmpstat.U;

    {
        /* set all into stuck-states */
        //boolean active[6]       = {TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE};
        boolean active[6]     = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
        boolean stuckState[6] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
        IfxCcu6_PwmHl_setupChannels(driver, active, stuckState);
    }
}


IFX_STATIC void IfxCcu6_PwmHl_enable(IfxCcu6_PwmHl *driver)
{
    IfxCcu6_TimerWithTrigger *timer  = (IfxCcu6_TimerWithTrigger *)driver->timer;
    Ifx_CCU6                 *module = timer->ccu6;
    /** - All TOP and BOTTOM channel (COUTx) states equals CC6xST */
    Ifx_CCU6_CMPSTAT          cmpstat;
    cmpstat.U = 0;
    //cmpstat.B.T13IM = 0;
    //cmpstat.B.COUT63PS = 0;
    //cmpstat.B.CC60PS = 0;
    //cmpstat.B.CC61PS = 0;
    //cmpstat.B.CC62PS = 0;
    cmpstat.B.COUT60PS = 1;
    cmpstat.B.COUT61PS = 1;
    cmpstat.B.COUT62PS = 1;

    module->CMPSTAT.U  = cmpstat.U;
    {
        /* set all into non stuck-states */
        boolean active[6]     = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};
        boolean stuckState[6] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
        IfxCcu6_PwmHl_setupChannels(driver, active, stuckState);
    }
}


float32 IfxCcu6_PwmHl_getDeadtime(IfxCcu6_PwmHl *driver)
{
    return IfxStdIf_Timer_tickToS(driver->timer->base.clockFreq, driver->base.deadtime);
}


float32 IfxCcu6_PwmHl_getMinPulse(IfxCcu6_PwmHl *driver)
{
    return IfxStdIf_Timer_tickToS(driver->timer->base.clockFreq, driver->base.minPulse - driver->base.deadtime);
}


Ifx_Pwm_Mode IfxCcu6_PwmHl_getMode(IfxCcu6_PwmHl *driver)
{
    return driver->base.mode;
}


boolean IfxCcu6_PwmHl_init(IfxCcu6_PwmHl *driver, const IfxCcu6_PwmHl_Config *config)
{
    boolean                   result = TRUE;

    IfxCcu6_TimerWithTrigger *timer  = config->timer;
    Ifx_CCU6                 *module = timer->ccu6;

    driver->base.mode             = Ifx_Pwm_Mode_init;
    driver->timer                 = timer;
    driver->base.setMode          = 0;
    driver->base.inverted         = FALSE;
    driver->base.ccxActiveState   = config->base.ccxActiveState;
    driver->base.coutxActiveState = config->base.coutxActiveState;

    driver->base.channelCount     = __minu(config->base.channelCount, IFXCCU6_PWMHL_MAX_CHANNELS);

    IfxCcu6_PwmHl_setDeadtime(driver, config->base.deadtime);
    IfxCcu6_PwmHl_setMinPulse(driver, config->base.minPulse);

    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, config->base.channelCount == IFXCCU6_PWMHL_MAX_CHANNELS);
    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, config->timer->base.countDir == IfxStdIf_Timer_CountDir_upAndDown);

    unsigned int     passiveState;
    Ifx_CCU6_PSLR    pslr;
    Ifx_CCU6_MODCTR  modctr;
    Ifx_CCU6_MCMOUTS mcmouts;
    Ifx_CCU6_MCMCTR  mcmctr;

    /** - All T12 channels (0,1,2) are set in compare mode */

    IfxCcu6_setT12ChannelMode(module, IfxCcu6_T12Channel_0, IfxCcu6_T12ChannelMode_compareMode);
    IfxCcu6_setT12ChannelMode(module, IfxCcu6_T12Channel_1, IfxCcu6_T12ChannelMode_compareMode);
    IfxCcu6_setT12ChannelMode(module, IfxCcu6_T12Channel_2, IfxCcu6_T12ChannelMode_compareMode);

    /** - Enable T12 modulation mode */
    modctr.U          = module->MODCTR.U;
    modctr.B.MCMEN    = 1;
    modctr.B.T12MODEN = 0x3FU;
    module->MODCTR.U  = modctr.U;

    IfxCcu6_setMultiChannelSwitchingMode(module, IfxCcu6_MultiChannelSwitchingSelect_t12PeriodMatch);

    IfxCcu6_setMultiChannelSwitchingSync(module, IfxCcu6_MultiChannelSwitchingSync_t12ZeroMatch);

    mcmctr.U         = module->MCMCTR.U;
    mcmctr.B.STE12D  = FALSE;
    mcmctr.B.STE12U  = FALSE;
    mcmctr.B.STE13U  = FALSE;
    module->MCMCTR.U = mcmctr.U;

    IfxCcu6_enableDeadTime(module, IfxCcu6_T12Channel_0);
    IfxCcu6_enableDeadTime(module, IfxCcu6_T12Channel_1);
    IfxCcu6_enableDeadTime(module, IfxCcu6_T12Channel_2);

    /* Configure the passive state */
    pslr.B.PSL         = 0;
    passiveState       = (config->base.ccxActiveState == Ifx_ActiveState_high) ? 0 : 1;
    pslr.B.PSL        |= (passiveState << 0);
    pslr.B.PSL        |= (passiveState << 2);
    pslr.B.PSL        |= (passiveState << 4);

    passiveState       = (config->base.coutxActiveState == Ifx_ActiveState_high) ? 0 : 1;
    pslr.B.PSL        |= (passiveState << 1);
    pslr.B.PSL        |= (passiveState << 3);
    pslr.B.PSL        |= (passiveState << 5);

    module->PSLR.B.PSL = pslr.B.PSL;

    IfxCcu6_PwmHl_setMode(driver, Ifx_Pwm_Mode_off);

    Ifx_TimerValue tOn[IFXCCU6_PWMHL_MAX_CHANNELS] = {0};
    IfxCcu6_PwmHl_updateOff(driver, tOn); /* tOn do not need defined values */

    mcmouts.U         = module->MCMOUTS.U;
    mcmouts.B.MCMPS   = 0x3F;
    mcmouts.B.STRMCM  = 1;
    module->MCMOUTS.U = mcmouts.U;

    IfxCcu6_PwmHl_setOutputPins(module, config);

    //config->base.emergencyEnabled;

    /* Transfer the shadow registers */
    IfxCcu6_TimerWithTrigger_applyUpdate(driver->timer);

    return result;
}


void IfxCcu6_PwmHl_initConfig(IfxCcu6_PwmHl_Config *config)
{
    IfxStdIf_PwmHl_initConfig(&config->base);
    config->timer = NULL_PTR;
    config->cc0   = NULL_PTR;
    config->cc1   = NULL_PTR;
    config->cc2   = NULL_PTR;
    config->cout0 = NULL_PTR;
    config->cout1 = NULL_PTR;
    config->cout2 = NULL_PTR;
}


boolean IfxCcu6_PwmHl_setDeadtime(IfxCcu6_PwmHl *driver, float32 deadtime)
{
    Ifx_TimerValue value = IfxStdIf_Timer_sToTick(driver->timer->base.clockFreq, deadtime);

    /* FIXME warn if dead time in out of range: > 1/2 period, ... */
    driver->base.deadtime = __minu(__minu(value, driver->timer->base.period / 2), 255);

    IfxCcu6_setDeadTimeValue(driver->timer->ccu6, (uint8)driver->base.deadtime);
    return TRUE;
}


boolean IfxCcu6_PwmHl_setMinPulse(IfxCcu6_PwmHl *driver, float32 minPulse)
{
    Ifx_TimerValue value = IfxStdIf_Timer_sToTick(driver->timer->base.clockFreq, minPulse);

    driver->base.minPulse = value + driver->base.deadtime;
    driver->base.maxPulse = driver->timer->base.period - driver->base.minPulse;

    return TRUE;
}


boolean IfxCcu6_PwmHl_setMode(IfxCcu6_PwmHl *driver, Ifx_Pwm_Mode mode)
{
    boolean             result = TRUE;
    IfxCcu6_PwmHl_Base *base   = &driver->base;

    if (base->mode != mode)
    {
        if ((mode > Ifx_Pwm_Mode_off) || (IfxCcu6_PwmHl_modes[mode].update == NULL_PTR))
        {
            mode   = Ifx_Pwm_Mode_off;
            result = FALSE;
        }

        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, mode == IfxCcu6_PwmHl_modes[mode].mode);

        base->mode     = mode;
        driver->update = IfxCcu6_PwmHl_modes[mode].update;

        if (base->mode == Ifx_Pwm_Mode_off)
        {
            IfxCcu6_PwmHl_disable(driver);
        }
        else
        {
            IfxCcu6_PwmHl_enable(driver);
        }
    }

    return result;
}


void IfxCcu6_PwmHl_setOnTime(IfxCcu6_PwmHl *driver, Ifx_TimerValue *tOn)
{
    // Period = 12
    // Ton    = 2
    // Center-aligned (the CCx equals CC6xST):
    //    T12PR  = (Period/2)-1
    //    T12    = 0 1 2 3 4 5 6 5 4 3 2  1  0 1 2 3
    //    CC6xR  = (Period - Ton) / 2 = 5
    //    CC6xST = 1 0 0 0 0 0 1 1 0 0 0 0 0 0 0 0
    // Right-aligned (the CCx equals CC6xST):
    //    T12PR  = (Period-1)
    //    T12    = 0 1 2 3 4 5 6 7 8 9 10 11 0 1 2 3
    //    CC6xR  = (Period - Ton) = 10
    //    CC6xST = 1 0 0 0 0 0 0 0 0 0 0  1  1 0 0 0
    // Left-aligned (the CCx equals ~CC6xST):
    //    T12PR  = (Period-1)
    //    T12    = 0 1 2 3 4 5 6 7 8 9 10 11 0 1 2 3
    //    CC6xR  = Ton = 2
    //    CC6xST = 1 0 0 1 1 1 1 1 1 1 1  1  1 0 0 0
    IfxCcu6_TimerWithTrigger *timer    = (IfxCcu6_TimerWithTrigger *)driver->timer;
    Ifx_CCU6                 *module   = timer->ccu6;
    int                       i;
    int                       channels = driver->base.channelCount;
    Ifx_TimerValue            period   = driver->timer->base.period;

    for (i = 0; i < channels; i++)
    {
        Ifx_TimerValue cmp;
        Ifx_TimerValue tOnX = tOn[i];

        if (tOnX < driver->base.minPulse)
        {
            tOnX = 0;
        }
        else if (tOnX > driver->base.maxPulse)
        {
            tOnX = period;
        }
        else
        {}

        cmp = (period - tOnX);
        cmp = cmp >> ((driver->base.mode == Ifx_Pwm_Mode_centerAligned) ? 1 : 0);
        IfxCcu6_setT12CompareValue(module, (IfxCcu6_T12Channel)i, (uint16)cmp);
    }
}


IFX_STATIC void IfxCcu6_PwmHl_setOutputPins(Ifx_CCU6 *module, const IfxCcu6_PwmHl_Config *config)
{
    if (config->cc0 != NULL_PTR)
    {
        IfxCcu6_initCc60OutPin(config->cc0, config->base.outputMode, config->base.outputDriver);
    }

    if (config->cc1 != NULL_PTR)
    {
        IfxCcu6_initCc61OutPin(config->cc1, config->base.outputMode, config->base.outputDriver);
    }

    if (config->cc2 != NULL_PTR)
    {
        IfxCcu6_initCc62OutPin(config->cc2, config->base.outputMode, config->base.outputDriver);
    }

    if (config->cout0 != NULL_PTR)
    {
        IfxCcu6_initCout60Pin(config->cout0, config->base.outputMode, config->base.outputDriver);
    }

    if (config->cout1 != NULL_PTR)
    {
        IfxCcu6_initCout61Pin(config->cout1, config->base.outputMode, config->base.outputDriver);
    }

    if (config->cout2 != NULL_PTR)
    {
        IfxCcu6_initCout62Pin(config->cout2, config->base.outputMode, config->base.outputDriver);
    }
}


void IfxCcu6_PwmHl_setupChannels(IfxCcu6_PwmHl *driver, boolean *activeCh, boolean *stuckSt)
{
    IfxCcu6_TimerWithTrigger *timer  = driver->timer;
    Ifx_CCU6                 *module = timer->ccu6;
    {
        uint8 pattern;

        pattern  = 0;
        pattern |= ((activeCh[0] != FALSE) ? 1 : 0) << 0;
        pattern |= ((activeCh[1] != FALSE) ? 1 : 0) << 1;
        pattern |= ((activeCh[2] != FALSE) ? 1 : 0) << 2;
        pattern |= ((activeCh[3] != FALSE) ? 1 : 0) << 3;
        pattern |= ((activeCh[4] != FALSE) ? 1 : 0) << 4;
        pattern |= ((activeCh[5] != FALSE) ? 1 : 0) << 5;

        IfxCcu6_setMultiChannelPwmPattern(module, pattern);
    }
    {
        Ifx_CCU6_PSLR pslr;
        pslr.U     = module->PSLR.U;
        pslr.B.PSL = 0;

        // top:
        pslr.B.PSL |= ((activeCh[0] == FALSE) ? ((stuckSt[0] == FALSE) ? 0 : 1) : 0) << 0;
        pslr.B.PSL |= ((activeCh[2] == FALSE) ? ((stuckSt[2] == FALSE) ? 0 : 1) : 0) << 2;
        pslr.B.PSL |= ((activeCh[4] == FALSE) ? ((stuckSt[4] == FALSE) ? 0 : 1) : 0) << 4;

        // bottom:
        pslr.B.PSL    |= ((activeCh[1] == FALSE) ? ((stuckSt[1] == FALSE) ? 0 : 1) : 0) << 1;
        pslr.B.PSL    |= ((activeCh[3] == FALSE) ? ((stuckSt[3] == FALSE) ? 0 : 1) : 0) << 3;
        pslr.B.PSL    |= ((activeCh[5] == FALSE) ? ((stuckSt[5] == FALSE) ? 0 : 1) : 0) << 5;
        module->PSLR.U = pslr.U;
    }
}


boolean IfxCcu6_PwmHl_stdIfPwmHlInit(IfxStdIf_PwmHl *stdif, IfxCcu6_PwmHl *driver)
{
    /* Ensure the stdif is reset to zeros */
    memset(stdif, 0, sizeof(IfxStdIf_PwmHl));

    /* *INDENT-OFF* Note: this file was indented manually by the author. */
    /* Set the API link */
    stdif->driver               = driver;
    stdif->setDeadtime          = (IfxStdIf_PwmHl_SetDeadtime  )&IfxCcu6_PwmHl_setDeadtime;
    stdif->getDeadtime          = (IfxStdIf_PwmHl_GetDeadtime  )&IfxCcu6_PwmHl_getDeadtime;
    stdif->setMinPulse          = (IfxStdIf_PwmHl_SetMinPulse  )&IfxCcu6_PwmHl_setMinPulse;
    stdif->getMinPulse          = (IfxStdIf_PwmHl_GetMinPulse  )&IfxCcu6_PwmHl_getMinPulse;
    stdif->getMode              = (IfxStdIf_PwmHl_GetMode      )&IfxCcu6_PwmHl_getMode;
    stdif->setMode              = (IfxStdIf_PwmHl_SetMode      )&IfxCcu6_PwmHl_setMode;
    stdif->setOnTime            = (IfxStdIf_PwmHl_SetOnTime    )&IfxCcu6_PwmHl_setOnTime;
    stdif->setupChannels        = (IfxStdIf_PwmHl_SetupChannels)&IfxCcu6_PwmHl_setupChannels;
    IfxCcu6_TimerWithTrigger_stdIfTimerInit(&stdif->timer, driver->timer);
    /* *INDENT-ON* */

    return TRUE;
}


IFX_STATIC void IfxCcu6_PwmHl_updateCenterAligned(IfxCcu6_PwmHl *driver, Ifx_TimerValue *tOn)
{
    IfxCcu6_PwmHl_setOnTime(driver, tOn);
}


IFX_STATIC void IfxCcu6_PwmHl_updateEdgeAligned(IfxCcu6_PwmHl *driver, Ifx_TimerValue *tOn)
{
    IfxCcu6_PwmHl_setOnTime(driver, tOn);
}


IFX_STATIC void IfxCcu6_PwmHl_updateOff(IfxCcu6_PwmHl *driver, Ifx_TimerValue *tOn)
{
    /* Nothing to do */
}
