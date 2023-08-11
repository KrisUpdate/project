/**
 * \file IfxSpu.c
 * \brief SPU  basic functionality
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

#include "IfxSpu.h"

/******************************************************************************/
/*----------------------------------Macros------------------------------------*/
/******************************************************************************/

#define IFXSPU_DEBUG IFX_VERBOSE_LEVEL_ERROR

/******************************************************************************/
/*-----------------------Private Function Prototypes--------------------------*/
/******************************************************************************/

/**
 */
IFX_STATIC uint32 IfxSpu_crc32single(uint32 data, uint32 crc);

/**
 */
IFX_STATIC uint32 IfxSpu_crc32final(uint32 crc);

/**
 */
IFX_STATIC uint32 IfxSpu_crc32calculate(void *data, uint32 count);

/******************************************************************************/
/*-----------------------Exported Variables/Constants-------------------------*/
/******************************************************************************/

IFX_CONST uint32 IfxSpu_crcTable[] =
{0x00000000UL, 0x04C11DB7UL, 0x09823B6EUL, 0x0D4326D9UL, 0x130476DCUL, 0x17C56B6BUL, 0x1A864DB2UL, 0x1E475005UL,
 0x2608EDB8UL, 0x22C9F00FUL, 0x2F8AD6D6UL, 0x2B4BCB61UL, 0x350C9B64UL, 0x31CD86D3UL, 0x3C8EA00AUL, 0x384FBDBDUL,
 0x4C11DB70UL, 0x48D0C6C7UL, 0x4593E01EUL, 0x4152FDA9UL, 0x5F15ADACUL, 0x5BD4B01BUL, 0x569796C2UL, 0x52568B75UL,
 0x6A1936C8UL, 0x6ED82B7FUL, 0x639B0DA6UL, 0x675A1011UL, 0x791D4014UL, 0x7DDC5DA3UL, 0x709F7B7AUL, 0x745E66CDUL,
 0x9823B6E0UL, 0x9CE2AB57UL, 0x91A18D8EUL, 0x95609039UL, 0x8B27C03CUL, 0x8FE6DD8BUL, 0x82A5FB52UL, 0x8664E6E5UL,
 0xBE2B5B58UL, 0xBAEA46EFUL, 0xB7A96036UL, 0xB3687D81UL, 0xAD2F2D84UL, 0xA9EE3033UL, 0xA4AD16EAUL, 0xA06C0B5DUL,
 0xD4326D90UL, 0xD0F37027UL, 0xDDB056FEUL, 0xD9714B49UL, 0xC7361B4CUL, 0xC3F706FBUL, 0xCEB42022UL, 0xCA753D95UL,
 0xF23A8028UL, 0xF6FB9D9FUL, 0xFBB8BB46UL, 0xFF79A6F1UL, 0xE13EF6F4UL, 0xE5FFEB43UL, 0xE8BCCD9AUL, 0xEC7DD02DUL,
 0x34867077UL, 0x30476DC0UL, 0x3D044B19UL, 0x39C556AEUL, 0x278206ABUL, 0x23431B1CUL, 0x2E003DC5UL, 0x2AC12072UL,
 0x128E9DCFUL, 0x164F8078UL, 0x1B0CA6A1UL, 0x1FCDBB16UL, 0x018AEB13UL, 0x054BF6A4UL, 0x0808D07DUL, 0x0CC9CDCAUL,
 0x7897AB07UL, 0x7C56B6B0UL, 0x71159069UL, 0x75D48DDEUL, 0x6B93DDDBUL, 0x6F52C06CUL, 0x6211E6B5UL, 0x66D0FB02UL,
 0x5E9F46BFUL, 0x5A5E5B08UL, 0x571D7DD1UL, 0x53DC6066UL, 0x4D9B3063UL, 0x495A2DD4UL, 0x44190B0DUL, 0x40D816BAUL,
 0xACA5C697UL, 0xA864DB20UL, 0xA527FDF9UL, 0xA1E6E04EUL, 0xBFA1B04BUL, 0xBB60ADFCUL, 0xB6238B25UL, 0xB2E29692UL,
 0x8AAD2B2FUL, 0x8E6C3698UL, 0x832F1041UL, 0x87EE0DF6UL, 0x99A95DF3UL, 0x9D684044UL, 0x902B669DUL, 0x94EA7B2AUL,
 0xE0B41DE7UL, 0xE4750050UL, 0xE9362689UL, 0xEDF73B3EUL, 0xF3B06B3BUL, 0xF771768CUL, 0xFA325055UL, 0xFEF34DE2UL,
 0xC6BCF05FUL, 0xC27DEDE8UL, 0xCF3ECB31UL, 0xCBFFD686UL, 0xD5B88683UL, 0xD1799B34UL, 0xDC3ABDEDUL, 0xD8FBA05AUL,
 0x690CE0EEUL, 0x6DCDFD59UL, 0x608EDB80UL, 0x644FC637UL, 0x7A089632UL, 0x7EC98B85UL, 0x738AAD5CUL, 0x774BB0EBUL,
 0x4F040D56UL, 0x4BC510E1UL, 0x46863638UL, 0x42472B8FUL, 0x5C007B8AUL, 0x58C1663DUL, 0x558240E4UL, 0x51435D53UL,
 0x251D3B9EUL, 0x21DC2629UL, 0x2C9F00F0UL, 0x285E1D47UL, 0x36194D42UL, 0x32D850F5UL, 0x3F9B762CUL, 0x3B5A6B9BUL,
 0x0315D626UL, 0x07D4CB91UL, 0x0A97ED48UL, 0x0E56F0FFUL, 0x1011A0FAUL, 0x14D0BD4DUL, 0x19939B94UL, 0x1D528623UL,
 0xF12F560EUL, 0xF5EE4BB9UL, 0xF8AD6D60UL, 0xFC6C70D7UL, 0xE22B20D2UL, 0xE6EA3D65UL, 0xEBA91BBCUL, 0xEF68060BUL,
 0xD727BBB6UL, 0xD3E6A601UL, 0xDEA580D8UL, 0xDA649D6FUL, 0xC423CD6AUL, 0xC0E2D0DDUL, 0xCDA1F604UL, 0xC960EBB3UL,
 0xBD3E8D7EUL, 0xB9FF90C9UL, 0xB4BCB610UL, 0xB07DABA7UL, 0xAE3AFBA2UL, 0xAAFBE615UL, 0xA7B8C0CCUL, 0xA379DD7BUL,
 0x9B3660C6UL, 0x9FF77D71UL, 0x92B45BA8UL, 0x9675461FUL, 0x8832161AUL, 0x8CF30BADUL, 0x81B02D74UL, 0x857130C3UL,
 0x5D8A9099UL, 0x594B8D2EUL, 0x5408ABF7UL, 0x50C9B640UL, 0x4E8EE645UL, 0x4A4FFBF2UL, 0x470CDD2BUL, 0x43CDC09CUL,
 0x7B827D21UL, 0x7F436096UL, 0x7200464FUL, 0x76C15BF8UL, 0x68860BFDUL, 0x6C47164AUL, 0x61043093UL, 0x65C52D24UL,
 0x119B4BE9UL, 0x155A565EUL, 0x18197087UL, 0x1CD86D30UL, 0x029F3D35UL, 0x065E2082UL, 0x0B1D065BUL, 0x0FDC1BECUL,
 0x3793A651UL, 0x3352BBE6UL, 0x3E119D3FUL, 0x3AD08088UL, 0x2497D08DUL, 0x2056CD3AUL, 0x2D15EBE3UL, 0x29D4F654UL,
 0xC5A92679UL, 0xC1683BCEUL, 0xCC2B1D17UL, 0xC8EA00A0UL, 0xD6AD50A5UL, 0xD26C4D12UL, 0xDF2F6BCBUL, 0xDBEE767CUL,
 0xE3A1CBC1UL, 0xE760D676UL, 0xEA23F0AFUL, 0xEEE2ED18UL, 0xF0A5BD1DUL, 0xF464A0AAUL, 0xF9278673UL, 0xFDE69BC4UL,
 0x89B8FD09UL, 0x8D79E0BEUL, 0x803AC667UL, 0x84FBDBD0UL, 0x9ABC8BD5UL, 0x9E7D9662UL, 0x933EB0BBUL, 0x97FFAD0CUL,
 0xAFB010B1UL, 0xAB710D06UL, 0xA6322BDFUL, 0xA2F33668UL, 0xBCB4666DUL, 0xB8757BDAUL, 0xB5365D03UL, 0xB1F740B4UL};

IFX_CONST uint8  IfxSpu_reflect8[] =
{0x00U, 0x80U, 0x40U, 0xC0U, 0x20U, 0xA0U, 0x60U, 0xE0U, 0x10U, 0x90U, 0x50U, 0xD0U, 0x30U, 0xB0U, 0x70U, 0xF0U,
 0x08U, 0x88U, 0x48U, 0xC8U, 0x28U, 0xA8U, 0x68U, 0xE8U, 0x18U, 0x98U, 0x58U, 0xD8U, 0x38U, 0xB8U, 0x78U, 0xF8U,
 0x04U, 0x84U, 0x44U, 0xC4U, 0x24U, 0xA4U, 0x64U, 0xE4U, 0x14U, 0x94U, 0x54U, 0xD4U, 0x34U, 0xB4U, 0x74U, 0xF4U,
 0x0CU, 0x8CU, 0x4CU, 0xCCU, 0x2CU, 0xACU, 0x6CU, 0xECU, 0x1CU, 0x9CU, 0x5CU, 0xDCU, 0x3CU, 0xBCU, 0x7CU, 0xFCU,
 0x02U, 0x82U, 0x42U, 0xC2U, 0x22U, 0xA2U, 0x62U, 0xE2U, 0x12U, 0x92U, 0x52U, 0xD2U, 0x32U, 0xB2U, 0x72U, 0xF2U,
 0x0AU, 0x8AU, 0x4AU, 0xCAU, 0x2AU, 0xAAU, 0x6AU, 0xEAU, 0x1AU, 0x9AU, 0x5AU, 0xDAU, 0x3AU, 0xBAU, 0x7AU, 0xFAU,
 0x06U, 0x86U, 0x46U, 0xC6U, 0x26U, 0xA6U, 0x66U, 0xE6U, 0x16U, 0x96U, 0x56U, 0xD6U, 0x36U, 0xB6U, 0x76U, 0xF6U,
 0x0EU, 0x8EU, 0x4EU, 0xCEU, 0x2EU, 0xAEU, 0x6EU, 0xEEU, 0x1EU, 0x9EU, 0x5EU, 0xDEU, 0x3EU, 0xBEU, 0x7EU, 0xFEU,
 0x01U, 0x81U, 0x41U, 0xC1U, 0x21U, 0xA1U, 0x61U, 0xE1U, 0x11U, 0x91U, 0x51U, 0xD1U, 0x31U, 0xB1U, 0x71U, 0xF1U,
 0x09U, 0x89U, 0x49U, 0xC9U, 0x29U, 0xA9U, 0x69U, 0xE9U, 0x19U, 0x99U, 0x59U, 0xD9U, 0x39U, 0xB9U, 0x79U, 0xF9U,
 0x05U, 0x85U, 0x45U, 0xC5U, 0x25U, 0xA5U, 0x65U, 0xE5U, 0x15U, 0x95U, 0x55U, 0xD5U, 0x35U, 0xB5U, 0x75U, 0xF5U,
 0x0DU, 0x8DU, 0x4DU, 0xCDU, 0x2DU, 0xADU, 0x6DU, 0xEDU, 0x1DU, 0x9DU, 0x5DU, 0xDDU, 0x3DU, 0xBDU, 0x7DU, 0xFDU,
 0x03U, 0x83U, 0x43U, 0xC3U, 0x23U, 0xA3U, 0x63U, 0xE3U, 0x13U, 0x93U, 0x53U, 0xD3U, 0x33U, 0xB3U, 0x73U, 0xF3U,
 0x0BU, 0x8BU, 0x4BU, 0xCBU, 0x2BU, 0xABU, 0x6BU, 0xEBU, 0x1BU, 0x9BU, 0x5BU, 0xDBU, 0x3BU, 0xBBU, 0x7BU, 0xFBU,
 0x07U, 0x87U, 0x47U, 0xC7U, 0x27U, 0xA7U, 0x67U, 0xE7U, 0x17U, 0x97U, 0x57U, 0xD7U, 0x37U, 0xB7U, 0x77U, 0xF7U,
 0x0FU, 0x8FU, 0x4FU, 0xCFU, 0x2FU, 0xAFU, 0x6FU, 0xEFU, 0x1FU, 0x9FU, 0x5FU, 0xDFU, 0x3FU, 0xBFU, 0x7FU, 0xFFU};

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxSpu_checkConfigSlot(IfxSpu_ConfigSlot *slot, const char *caller, uint32 line)
{
    IFX_ASSERT(IFXSPU_DEBUG, slot->BE[0].LDR_CONF.B.DRPL == slot->BE[1].LDR_CONF.B.DRPL);
    IFX_ASSERT(IFXSPU_DEBUG, slot->BE[0].LDR_CONF.B.DRPF == slot->BE[1].LDR_CONF.B.DRPF);
    IFX_ASSERT(IFXSPU_DEBUG, slot->BE[0].LDR_CONF.B.EXPNT == slot->BE[1].LDR_CONF.B.EXPNT);
    IFX_ASSERT(IFXSPU_DEBUG, slot->BE[0].LDR_CONF.B.SIZE == slot->BE[1].LDR_CONF.B.SIZE);
    IFX_ASSERT(IFXSPU_DEBUG, slot->BE[0].LDR_CONF.B.FFTBYPS == slot->BE[1].LDR_CONF.B.FFTBYPS);
    IFX_ASSERT(IFXSPU_DEBUG, slot->BE[0].LDR_CONF2.B.PADF == slot->BE[1].LDR_CONF2.B.PADF);
    IFX_ASSERT(IFXSPU_DEBUG, slot->BE[0].UNLDR_CONF.B.FORMAT == slot->BE[1].UNLDR_CONF.B.FORMAT);
    IFX_ASSERT(IFXSPU_DEBUG, slot->BE[0].UNLDR_CONF.B.EXPNT == slot->BE[1].UNLDR_CONF.B.EXPNT);
    IFX_ASSERT(IFXSPU_DEBUG, slot->BE[0].LDR_CONF.B.IFFT == slot->BE[1].LDR_CONF.B.IFFT);
}


void IfxSpu_disableModule(Ifx_SPU *spu)
{
    uint16 password = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(password);
    spu->CLC.B.DISR = 1U;
    IfxScuWdt_setCpuEndinit(password);
}


Ifx_SPU *IfxSpu_getAddress(IfxSpu_Index spu)
{
    Ifx_SPU *module;

    if (spu < IFXSPU_NUM_MODULES)
    {
        module = (Ifx_SPU *)IfxSpu_cfg_indexMap[spu].module;
    }
    else
    {
        module = NULL_PTR;
    }

    return module;
}


IfxSpu_ConfigSlot *IfxSpu_getConfigSlot(Ifx_SPU *spu, uint32 slotNr)
{
    IfxSpu_ConfigSlot *slot = NULL_PTR;

    if (slotNr == 0)
    {
        slot = (IfxSpu_ConfigSlot *)&(spu->ID.CONF);
    }
    else
    {
        if (slotNr <= IFXSPU_CONFIG_SLOT_MAX)
        {
            if (spu == &MODULE_SPU0)
            {
                slot = (IfxSpu_ConfigSlot *)IFXSPU_CONFIG_RAM0_BASE;
                slot = &(slot[slotNr - 1]);
            }

#if IFXSPU_NUM_MODULES > 1
            else if (spu == &MODULE_SPU1)
            {
                slot = (IfxSpu_ConfigSlot *)IFXSPU_CONFIG_RAM1_BASE;
                slot = &(slot[slotNr - 1]);
            }
#endif
        }
    }

    return slot;
}


IfxSpu_DataSource IfxSpu_getDataSource(IfxSpu_ConfigSlot *slot)
{
    Ifx_SPU_ID_CONF idConf;
    idConf.U = slot->ID.CONF.U;
    return (IfxSpu_DataSource)idConf.B.SRC;
}


IfxSpu_Index IfxSpu_getIndex(Ifx_SPU *spu)
{
    uint32       index;
    IfxSpu_Index result;

    result = IfxSpu_Index_none;

    for (index = 0; index < IFXSPU_NUM_MODULES; index++)
    {
        if (IfxSpu_cfg_indexMap[index].module == spu)
        {
            result = (IfxSpu_Index)IfxSpu_cfg_indexMap[index].index;
            break;
        }
    }

    return result;
}


uint16 IfxSpu_getNumConnectedAntennae(IfxSpu_ConfigSlot *slot)
{
    uint16          result = 0;
    Ifx_SPU_ID_CONF idConf;
    idConf.U = slot->ID.CONF.U;

    if (IfxSpu_getDataSource(slot) != IfxSpu_DataSource_emem)
    {
        result = (uint16)(idConf.B.ANT + 1);

        if (IfxSpu_getDataSource(slot) == IfxSpu_DataSource_rifBoth)
        {
            result = result * 2;
        }
    }

    return result;
}


uint16 IfxSpu_getNumInputSamples(IfxSpu_ConfigSlot *slot)
{
    uint16            result;
    Ifx_SPU_ID_CONF   idConf;
    Ifx_SPU_ID_RM_BLR idRmBlr;

    idConf.U  = slot->ID.CONF.U;
    idRmBlr.U = slot->ID.RM_BLR.U;

    if (IfxSpu_getDataSource(slot) != IfxSpu_DataSource_emem)
    {
        result = idConf.B.SMPLCNT + 1;
    }
    else
    {
        result = idRmBlr.B.BLR + 1;
    }

    return result;
}


uint16 IfxSpu_getNumRamps(IfxSpu_ConfigSlot *slot)
{
    uint16          result = 0;
    Ifx_SPU_ID_CONF idConf;
    idConf.U = slot->ID.CONF.U;

    if (IfxSpu_getDataSource(slot) != IfxSpu_DataSource_emem)
    {
        result = idConf.B.RAMPS + 1;
    }

    return result;
}


void IfxSpu_initModule(Ifx_SPU *spu, const IfxSpu_ModuleConfig *config)
{
    /* enable module if not enabled already */
    if (spu->CLC.B.DISS != 0)
    {
        spu->CLC.B.DISR = 0;

        while (spu->CLC.B.DISS != 0)
        {}
    }

    {
        /* set up the clock divider ratio */
        Ifx_SPU_CTRL ctrl;
        ctrl.U      = spu->CTRL.U;
        ctrl.B.DIV  = config->clockDivisionRatio;
        spu->CTRL.U = ctrl.U;
    }
    {
        Ifx_SPU_STAT stat;
        stat.U = spu->STAT.U;

        /* enable the selected Attension request sources */
        stat.B.INTMSK = (uint8)((config->interrupt.anyStatus << 5) |
                                (config->interrupt.pactrTrigger << 4) |
                                (config->interrupt.linkedListEnd << 3) |
                                (config->interrupt.inputDmaUnit << 2) |
                                (config->interrupt.complexDataWrite << 1) |
                                (config->interrupt.powerDataWrite));

        /* enable the selected Error Flags */
        stat.B.ERRMSK = (uint8)((config->interrupt.math2UnitOverrun << 5) |
                                (config->interrupt.pactrError << 4) |
                                (config->interrupt.fifoOverrun << 3) |
                                (config->interrupt.outputDmaEngineError << 2) |
                                (config->interrupt.inputDmaWriteError << 1) |
                                (config->interrupt.inputDmaReadError));

        spu->STAT.U = stat.U;
    }
    {
        Ifx_SPU_PACTR pactr;
        pactr.U = spu->PACTR.U;

        /* enable preacquisition counter attention request and error flag sources */
        if (config->interrupt.pactrTrigger)
        {
            pactr.B.ATTN = TRUE;
        }

        if (config->interrupt.pactrError)
        {
            pactr.B.ERR = TRUE;
        }

        spu->PACTR.U = pactr.U;
    }
}


void IfxSpu_initModuleConfig(IfxSpu_ModuleConfig *config)
{
    config->clockDivisionRatio             = IfxSpu_ClockDivisionRatio_unity;

    config->interrupt.anyStatus            = FALSE;
    config->interrupt.pactrTrigger         = FALSE;
    config->interrupt.linkedListEnd        = FALSE;
    config->interrupt.inputDmaUnit         = FALSE;
    config->interrupt.complexDataWrite     = FALSE;
    config->interrupt.powerDataWrite       = FALSE;

    config->interrupt.math2UnitOverrun     = FALSE;
    config->interrupt.pactrError           = FALSE;
    config->interrupt.fifoOverrun          = FALSE;
    config->interrupt.outputDmaEngineError = FALSE;
    config->interrupt.inputDmaWriteError   = FALSE;
    config->interrupt.inputDmaReadError    = FALSE;
}


void IfxSpu_loadConfigRAM(Ifx_SPU *spu, const void *dataPtr, uint32 offset32, uint32 count32)
{
    uint32  i;
    uint32 *destPtr = NULL_PTR;

    if (spu == &MODULE_SPU0)
    {
        destPtr = (uint32 *)IFXSPU_CONFIG_RAM0_BASE;
    }
    else if (spu == &MODULE_SPU1)
    {
        destPtr = (uint32 *)IFXSPU_CONFIG_RAM1_BASE;
    }
    else /*if (spu module not present)*/
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    destPtr = &destPtr[offset32];

    for (i = 0; i < count32; i++)
    {
        *destPtr++ = ((uint32 *)dataPtr)[i];
    }
}


void IfxSpu_reloadConfig(Ifx_SPU *spu, uint32 slotNr)
{
    /* slotNr 0 is not allowed */
    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, slotNr > 0);

    Ifx_SPU_CTRL ctrl;
    ctrl.U = 0;

    /* set the trigger mode as reload*/
    ctrl.B.MODE = IfxSpu_TriggerMode_reload;

    /* set pointer to the configuration */
    ctrl.B.NXT_CONF = (slotNr - 1) * sizeof(IfxSpu_ConfigSlot);

    /* write 32 bit at once */
    spu->CTRL.U = ctrl.U;
}


void IfxSpu_resetNciWeights(IfxSpu_ConfigSlot *slot)
{
    slot->NCI.SCALAR0.U = 0xFFFFFFFFu;
    slot->NCI.SCALAR1.U = 0xFFFFFFFFu;
    slot->NCI.SCALAR2.U = 0xFFFFFFFFu;
    slot->NCI.SCALAR3.U = 0xFFFFFFFFu;
}


void IfxSpu_setPassMode(IfxSpu_ConfigSlot *slot, IfxSpu_PassMode passMode)
{
    Ifx_SPU_DPASS_CONF dpassConf;
    dpassConf.U = slot->DPASS_CONF.U;

    switch (passMode)
    {
    case IfxSpu_PassMode_double:
        dpassConf.B.EN     = TRUE;
        dpassConf.B.SWITCH = FALSE;
        break;
    case IfxSpu_PassMode_alternate:
        dpassConf.B.EN     = TRUE;
        dpassConf.B.SWITCH = TRUE;
        break;
    default:
        dpassConf.B.EN     = FALSE;
        dpassConf.B.SWITCH = FALSE;
        break;
    }

    slot->DPASS_CONF.U = dpassConf.U;
}


void IfxSpu_setupBinRejection(IfxSpu_ConfigSlot *slot, const IfxSpu_BinRejectionConfig *config)
{
    uint32 ix, jx;

    if (config != NULL_PTR)
    {
        {
            Ifx_SPU_BINREJCTRL binrejctrl;
            binrejctrl.U = slot->BINREJCTRL.U;

            /* set the Bin Rejection mode */
            binrejctrl.B.RMODE = config->mode;

            /* Enable Zeroing with Threshold if selected */
            binrejctrl.B.ZMODE = config->thresholdEnabled;

            /* set Zeroing threshold value */
            binrejctrl.B.VALUE = config->thresholdValue;

            /* set left justify value */
            binrejctrl.B.LJUST = config->leftJustify;

            slot->BINREJCTRL.U = binrejctrl.U;
        }

        /* set bin rejection registers */
        /* Notify in case config->numAllowedBins > 2048*/
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, config->numAllowedBins <= 2048);
        /*In case config->numAllowedBins > 2048, cap it to 2048 as that is the max FFT length available*/
        uint32 numAllowedBins = (config->numAllowedBins < IFXSPU_MAX_FFT_LENGTH) ? config->numAllowedBins : IFXSPU_MAX_FFT_LENGTH;
        uint32 count32        = numAllowedBins / 32;

/* Precautionary check for bin rejection being disabled i.e all data passed.Data length is decided by numAllowedBins */
        if (config->mode != IfxSpu_BinRejection_Mode_off)
        {
            for (ix = 0; ix < count32; ix++)
            {
                slot->BIN_REJ[ix].U = 0xFFFFFFFFU;
            }

            slot->BIN_REJ[ix].U = 0;
            uint32 remainder = config->numAllowedBins % 32;

            for (jx = 0; jx < remainder; jx++)
            {
                slot->BIN_REJ[ix].U |= (1u << jx);
            }

            for (ix = ix + 1; ix < 64; ix++)
            {
                slot->BIN_REJ[ix].U = 0;
            }
        }

/* select the CFAR engine */

        Ifx_SPU_CFAR_CFG1 cfarCfg1;
        cfarCfg1.U         = slot->CFAR.CFG1.U;
        cfarCfg1.B.CFARSEL = config->inlineCfarSelect;
        slot->CFAR.CFG1.U  = cfarCfg1.U;
    }
    else
    {
        {
            Ifx_SPU_BINREJCTRL binrejctrl;
            binrejctrl.U       = 0;
            binrejctrl.B.VALUE = 0xFFFFu;

            slot->BINREJCTRL.U = binrejctrl.U;
        }

        for (ix = 0; ix < 64; ix++)
        {
            slot->BIN_REJ[ix].U = 0xFFFFFFFFu;
        }

        Ifx_SPU_CFAR_CFG1 cfarCfg1;
        cfarCfg1.U         = slot->CFAR.CFG1.U;
        cfarCfg1.B.CFARSEL = IfxSpu_CFAR_Engine_auto;
        slot->CFAR.CFG1.U  = cfarCfg1.U;
    }
}


void IfxSpu_setupCfar(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_CfarConfig *config)
{
    Ifx_SPU_BE_CFARCTRL cfarctrl;

    cfarctrl.U = slot->BE[passId].CFARCTRL.U;

    if (config != NULL_PTR)
    {
        /* set the CFAR mode of operation */
        cfarctrl.B.CFAREN = config->mode;

        /* enable the CFAR engine */
        cfarctrl.B.CFAR_CAE  = config->caCfarEnabled;
        cfarctrl.B.CFAR_GOSE = config->gosCfarEnabled;

        /* set CFAR base address (converted from byte address) */
        cfarctrl.B.BASE = config->baseAddress / 32;

        /* select spectrum extension */
        cfarctrl.B.EXTNSN = config->spectrumExtension;

        /* set spectrum extension window size */
        cfarctrl.B.SEWIN = config->spectrumExtensionSize;
    }
    else
    {
        cfarctrl.U = 0;
    }

    slot->BE[passId].CFARCTRL.U = cfarctrl.U;
}


void IfxSpu_setupCfarCA(IfxSpu_ConfigSlot *slot, const IfxSpu_CfarCAConfig *config)
{
    Ifx_SPU_CFAR_CFG1 cfarcfg;
    cfarcfg.U = slot->CFAR.CFG1.U;

    if (config != NULL_PTR)
    {
        /* select the CA-CFAR algorithm */
        cfarcfg.B.CAALGO = config->algorithm;

        /* set number of guard cells in CA-CFAR */
        cfarcfg.B.CAGUARD = config->guardCells;

        /* set number of active cells to be averaged in CA-CFAR */
        cfarcfg.B.CAWINCELL = config->windowCellsExponent;

        /* set CA-CFAR beta */
        cfarcfg.B.CABETA = config->beta;
    }
    else
    {
        cfarcfg.B.CAALGO    = IfxSpu_CFAR_CA_Algorithm_cash;
        cfarcfg.B.CAGUARD   = 0;
        cfarcfg.B.CAWINCELL = 0;
        cfarcfg.B.CABETA    = 0;
    }

    slot->CFAR.CFG1.U = cfarcfg.U;

    {
        Ifx_SPU_CFAR_CFG2 cfarcfg2;
        cfarcfg2.U = slot->CFAR.CFG2.U;

        if (config == NULL_PTR)
        {
            cfarcfg2.B.CASHWIN = 0;
        }
        else if (config->algorithm == IfxSpu_CFAR_CA_Algorithm_cash)
        {
            cfarcfg2.B.CASHWIN = config->cashSubWindowExponent;
        }
        else
        {
            cfarcfg2.B.CASHWIN = config->windowCellsExponent;
        }

        slot->CFAR.CFG2.U = cfarcfg2.U;
    }
}


void IfxSpu_setupCfarGOS(IfxSpu_ConfigSlot *slot, const IfxSpu_CfarGOSConfig *config)
{
    if (config != NULL_PTR)
    {
        {
            Ifx_SPU_CFAR_CFG1 cfarcfg1;
            cfarcfg1.U = slot->CFAR.CFG1.U;

            /* select the GOS-CFAR algorithm */
            cfarcfg1.B.GOSALGO = config->algorithm;

            slot->CFAR.CFG1.U  = cfarcfg1.U;
        }

        {
            Ifx_SPU_CFAR_CFG2 cfarcfg2;
            cfarcfg2.U = slot->CFAR.CFG2.U;

            /* set number of guard cells in GOS-CFAR */
            cfarcfg2.B.GOSGUARD = config->guardCells;

            /* set leading index in GOS-CFAR */
            cfarcfg2.B.IDXLD = config->indexLead;

            /* set lagging index in GOS-CFAR */
            cfarcfg2.B.IDXLG = config->indexLag;

            /* set number of active cells to be averaged in GOS-CFAR */
            cfarcfg2.B.GOSWINCELL = config->windowCells;

            slot->CFAR.CFG2.U     = cfarcfg2.U;
        }

        {
            Ifx_SPU_CFAR_CFG3 cfarcfg3;
            cfarcfg3.U = slot->CFAR.CFG3.U;

            /* set GOS-CFAR beta */
            cfarcfg3.B.GOSBETA    = config->beta;
            cfarcfg3.B.CHAN5OFFST = config->addressOffset / 32;

            slot->CFAR.CFG3.U     = cfarcfg3.U;
        }
    }
    else
    {
        {
            Ifx_SPU_CFAR_CFG1 cfarcfg1;
            cfarcfg1.U         = slot->CFAR.CFG1.U;
            cfarcfg1.B.GOSALGO = IfxSpu_CFAR_GOS_Algorithm_gos;
            slot->CFAR.CFG1.U  = cfarcfg1.U;
        }

        slot->CFAR.CFG2.U = 0;
        slot->CFAR.CFG3.U = 0;
    }
}


void IfxSpu_setupConfigSequence(Ifx_SPU *spu, uint32 slotNr, uint32 nextSlotNr, IfxSpu_TriggerMode mode, IfxSpu_ClockDivisionRatio clkDiv)
{
    /* get pointer to SFR base address of selected SPU */
    IfxSpu_ConfigSlot *slot     = IfxSpu_getConfigSlot(spu, slotNr);

    boolean            lastSlot = (nextSlotNr == 0);
    uint32             offset;

    if (lastSlot == FALSE)
    {
        /* set pointer to next config */
        offset = (uint32)IfxSpu_getConfigSlot(spu, nextSlotNr);
        offset = offset - (uint32)IfxSpu_getConfigSlot(spu, 1);
    }
    else
    {
        offset = 0;
    }

    {
        Ifx_SPU_CTRL ctrl;
        ctrl.U      = slot->CTRL.U;

        ctrl.B.MODE = mode;

        /* set the trigger mode */
        if (mode == IfxSpu_TriggerMode_software)
        {
            ctrl.B.TRIG = 1;
        }
        else
        {
            ctrl.B.TRIG = 0;
        }

        /* set the Clock division */
        ctrl.B.DIV      = clkDiv;

        ctrl.B.NXT_CONF = offset;
        ctrl.B.LAST     = lastSlot;

        slot->CTRL.U    = ctrl.U;
    }
}


void IfxSpu_setupFft(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_FftConfig *config)
{
    if (config != NULL_PTR)
    {
        {
            Ifx_SPU_BE_LDR_CONF ldrConf;
            ldrConf.U = slot->BE[passId].LDR_CONF.U;

            /* set FFT bypass */
            ldrConf.B.FFTBYPS = config->enabled ? 0 : 1;

            /* set inverse FFT */
            ldrConf.B.IFFT = config->inversed;

            /* set length of the FFT */
            ldrConf.B.SIZE              = config->size - IfxSpu_FftSize_8;

            slot->BE[passId].LDR_CONF.U = ldrConf.U;
        }

        {
            Ifx_SPU_BE_UNLDR_CONF unldrConf;
            unldrConf.U = slot->BE[passId].UNLDR_CONF.U;

            /* set up unloader configuration */
            unldrConf.B.FORMAT            = config->dataFormat;
            unldrConf.B.EXPNT             = config->exponent;

            slot->BE[passId].UNLDR_CONF.U = unldrConf.U;
        }

        {
            /* set up force to real operation if selected */
            Ifx_SPU_BE_ODP_CONF odpConf;
            odpConf.U                   = slot->BE[passId].ODP_CONF.U;
            odpConf.B.FTR               = config->forceToReal;
            slot->BE[passId].ODP_CONF.U = odpConf.U;
        }
    }
    else
    {
        {
            Ifx_SPU_BE_LDR_CONF ldrConf;
            ldrConf.U                   = slot->BE[passId].LDR_CONF.U;

            ldrConf.B.FFTBYPS           = 1;
            ldrConf.B.IFFT              = 0;
            ldrConf.B.SIZE              = IfxSpu_FftSize_8 - IfxSpu_FftSize_8;

            slot->BE[passId].LDR_CONF.U = ldrConf.U;
        }

        {
            Ifx_SPU_BE_UNLDR_CONF unldrConf;
            unldrConf.U                   = slot->BE[passId].UNLDR_CONF.U;

            unldrConf.B.FORMAT            = 0;
            unldrConf.B.EXPNT             = 16;
            slot->BE[passId].UNLDR_CONF.U = unldrConf.U;
        }

        {
            Ifx_SPU_BE_ODP_CONF odpConf;
            odpConf.U                   = slot->BE[passId].ODP_CONF.U;
            odpConf.B.FTR               = 0;
            slot->BE[passId].ODP_CONF.U = odpConf.U;
        }
    }
}


void IfxSpu_setupFftOutput(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_FftOutputConfig *config)
{
    Ifx_SPU_BE_ODP_CONF odpConf;
    odpConf.U = slot->BE[passId].ODP_CONF.U;

    if (config != NULL_PTR)
    {
        /* ON / OFF output data processor */
        odpConf.B.MODE = config->enabled;

        /* set ODP base address (converted from byte address) */
        odpConf.B.BASE = config->baseAddress / 32;

        /* set ODP scaling factor */
        odpConf.B.SCALE = (config->format == IfxSpu_ODP_Format_real16Bit)
                          || (config->format == IfxSpu_ODP_Format_complex16Bit);
        /* set ODP real only format */
        odpConf.B.ROF = (config->format == IfxSpu_ODP_Format_real32Bit)
                        || (config->format == IfxSpu_ODP_Format_real16Bit)
                        || (config->format == IfxSpu_ODP_Format_realHalfFloat);

        /* set ODP half precision floating point */
        odpConf.B.HPFP = (config->format == IfxSpu_ODP_Format_realHalfFloat)
                         || (config->format == IfxSpu_ODP_Format_complexHalfFloat);
        /* set ODP exponenet */
        odpConf.B.EXPNT = config->exponent;

        /* set up in place FFT if selected */
        odpConf.B.IPF = config->inPlace;
    }
    else
    {
        odpConf.B.MODE  = 0;
        odpConf.B.BASE  = 0;
        odpConf.B.SCALE = 0;
        odpConf.B.EXPNT = 0;
        odpConf.B.IPF   = 0;
        odpConf.B.ROF   = 0;
        odpConf.B.HPFP  = 0;
    }

    slot->BE[passId].ODP_CONF.U = odpConf.U;
}


void IfxSpu_setupHistogram(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_HistogramConfig *config)
{
    if (config != NULL_PTR)
    {
        {
            Ifx_SPU_BE_UNLDR_CONF unldrConf;
            unldrConf.U = slot->BE[passId].UNLDR_CONF.U;

            /* enable histogram */
            unldrConf.B.HISTEN = config->enabled;

            /* set number of classes */
            unldrConf.B.HISTBINS = config->numClasses;

            /* set histogram base address */
            unldrConf.B.HISTBASE = config->baseAddress;

            /* if Histogram antenna filter is enabled */
            unldrConf.B.HAFE = config->antennaFilterEnabled;

            /* enable antenna filter value */
            unldrConf.B.AFV               = config->antennaFilterMask;

            slot->BE[passId].UNLDR_CONF.U = unldrConf.U;
        }

        {
            Ifx_SPU_BE_UNLDR_CONF2 unldrConf2;
            unldrConf2.U = slot->BE[passId].UNLDR_CONF2.U;

            /* set start delay */
            unldrConf2.B.STRT = config->startDelay;

            /* set end delay */
            unldrConf2.B.END               = config->endDelay;

            slot->BE[passId].UNLDR_CONF2.U = unldrConf2.U;
        }
    }
    else
    {
        {
            Ifx_SPU_BE_UNLDR_CONF unldrConf;
            unldrConf.U                   = slot->BE[passId].UNLDR_CONF.U;

            unldrConf.B.HISTEN            = 0;
            unldrConf.B.HISTBINS          = 0;
            unldrConf.B.HISTBASE          = 0;
            unldrConf.B.HAFE              = 0;
            unldrConf.B.AFV               = 0;

            slot->BE[passId].UNLDR_CONF.U = unldrConf.U;
        }

        {
            Ifx_SPU_BE_UNLDR_CONF2 unldrConf2;
            unldrConf2.U                   = slot->BE[passId].UNLDR_CONF2.U;
            unldrConf2.B.STRT              = 0;
            unldrConf2.B.END               = 0xFFFF;
            slot->BE[passId].UNLDR_CONF2.U = unldrConf2.U;
        }
    }
}


void IfxSpu_setupInput(IfxSpu_ConfigSlot *slot, const IfxSpu_FftInputConfig *config)
{
    /* set data source */
    Ifx_SPU_ID_CONF idConf;
    idConf.U     = slot->ID.CONF.U;
    idConf.B.SRC = config->dataSource;

    if (config->dataSource != IfxSpu_DataSource_emem)
    {
        /* when input data source is RIF */
        {
            /* set input data format */
            idConf.B.FORMAT = config->rif.dataFormat;

            /* set input data type */
            idConf.B.SIGNED = config->rif.dataType;

            /* set sample count */
            idConf.B.SMPLCNT = config->rif.sampleCount - 1;

            /* set number of ramps per measurement cycle */
            idConf.B.RAMPS = config->rif.numRamps - 1;

            /* set number of antennae */
            idConf.B.ANT = config->rif.numAntennae - 1;
        }

        {
            Ifx_SPU_ID_CONF2 idConf2;
            idConf2.U = slot->ID.CONF2.U;

            /* set up split processing mode configuration if selected */
            idConf2.B.BYPASS = config->rif.bypassModeEnabled;

            /* set bypass address */
            idConf2.B.BPADDR = config->rif.bypassAddress / 32;

            /* enable bypass reload if selected */
            idConf2.B.BPRLD  = config->rif.bypassReloadEnabled;

            slot->ID.CONF2.U = idConf2.U;
        }

        {
            Ifx_SPU_PACTR pactr;
            pactr.U = slot->PACTR.U;

            /* set up pre acquisition counter configuration if selected */
            pactr.B.EN = config->rif.preAcquisitionCounterEnabled;

            /* set pre acquisition counter limit */
            pactr.B.LIMIT = config->rif.preAcquisitionCounterLimit;

            slot->PACTR.U = pactr.U;
        }
    }

    slot->ID.CONF.U = idConf.U;

    {
        /* radar memory access configuration */

        {
            Ifx_SPU_ID_RM_CONF idRmConf;
            idRmConf.U = slot->ID.RM_CONF.U;

            /* set input data format */
            idRmConf.B.FORMAT = config->rm.dataFormat;

            /* set radar memory base address (converted from byte address) */
            idRmConf.B.BASE = config->rm.baseAddress / 32;

            /* set addressing mode */
            idRmConf.B.TRNSPS = config->rm.addressingMode;

            /* set processing mode */
            idRmConf.B.PM = config->rm.processingMode;

            /* set number of data blocks */
            idRmConf.B.BLOCKS = config->rm.numBlocks - 1;

            /* set antenna mapping */
            idRmConf.B.AM      = config->rm.antennaMapping;

            slot->ID.RM_CONF.U = idRmConf.U;
        }

        /* set loop offsets */
        {
            Ifx_SPU_ID_RM_ILO ilo;
            ilo.U             = slot->ID.RM_ILO.U;
            ilo.B.ILO         = config->rm.innerLoopOffset;
            slot->ID.RM_ILO.U = ilo.U;
        }

        {
            Ifx_SPU_ID_RM_OLO olo;
            olo.U             = slot->ID.RM_OLO.U;
            olo.B.OLO         = config->rm.outerLoopOffset;
            slot->ID.RM_OLO.U = olo.U;
        }

        {
            Ifx_SPU_ID_RM_BLO blo;
            blo.U             = slot->ID.RM_BLO.U;
            blo.B.BLO         = config->rm.binLoopOffset;
            slot->ID.RM_BLO.U = blo.U;
        }

        /* set loop repeat values */
        {
            Ifx_SPU_ID_RM_IOLR iolr;
            iolr.U             = slot->ID.RM_IOLR.U;
            iolr.B.ILR         = config->rm.innerLoopRepeat - 1;
            iolr.B.OLR         = config->rm.outerLoopRepeat - 1;
            slot->ID.RM_IOLR.U = iolr.U;
        }

        {
            Ifx_SPU_ID_RM_BLR blr;
            blr.U             = slot->ID.RM_BLR.U;
            blr.B.BLR         = config->rm.binLoopRepeat - 1;
            slot->ID.RM_BLR.U = blr.U;
        }
    }
}


void IfxSpu_setupLocalMax(IfxSpu_ConfigSlot *slot, const IfxSpu_LocalMaxConfig *config)
{
    Ifx_SPU_LCLMAX lclmax;
    lclmax.U = 0;

    if (config != NULL_PTR)
    {
        lclmax.U = slot->LCLMAX.U;

        /* set the Local Max mode */
        lclmax.B.LMODE = config->localMaxMode;

        /* set the Threshold mode */
        lclmax.B.TMODE = config->thresholdMode;

        /* set Local Max window width */
        lclmax.B.WIDTH = config->windowWidth;

        /* set left justify value */
        lclmax.B.LJUST = config->leftJustify;

        /* set threshold value */
        lclmax.B.TSHLD = config->thresholdValue;
    }

    slot->LCLMAX.U = lclmax.U;
}


void IfxSpu_setupMagnitude(IfxSpu_ConfigSlot *slot, IfxSpu_MagnitudeConfig *config)
{
    if (config != NULL_PTR)
    {
        Ifx_SPU_MAGAPPROX magapprox;
        magapprox.U       = 0;

        magapprox.B.ALPHA = config->alpha;
        magapprox.B.BETA  = config->beta;

        slot->MAGAPPROX.U = magapprox.U;
    }
    else
    {
        slot->MAGAPPROX.U = 0xFFFFFFFFu;
    }
}


void IfxSpu_setupMath1(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_Math1Config *config)
{
    {
        Ifx_SPU_BE_LDR_CONF ldrConf;
        ldrConf.U = slot->BE[passId].LDR_CONF.U;

        /* set up truncation */
        ldrConf.B.DRPL = config->numDropLastSamples;
        ldrConf.B.DRPF = config->numDropFirstSamples;

        /* set up alignment correction */
        ldrConf.B.EXPNT = config->loaderExponent;

        /* set up phase shift */
        ldrConf.B.PHSHFT = config->phaseShift;

        /* set window data format */
        ldrConf.B.FORMAT            = config->window.dataFormat;

        slot->BE[passId].LDR_CONF.U = ldrConf.U;
    }

    /* set up windowing */
    {
        Ifx_SPU_BE_LDR_CONF2 ldrConf2;
        ldrConf2.U                   = slot->BE[passId].LDR_CONF2.U;
        ldrConf2.B.WBASE             = config->window.baseAddress;
        ldrConf2.B.PADF              = config->numPadFrontSamples;
        ldrConf2.B.WINEN             = config->window.enabled;
        slot->BE[passId].LDR_CONF2.U = ldrConf2.U;
    }

    /* set up antenna offsets */

    uint32 i;

    for (i = 0; i < (IFXSPU_MAX_ANTENNAE / 2); i++)
    {
        Ifx_SPU_BE_ANTOFST ant01;
        ant01.B.ADROFST0_ANT          = config->window.antennaOffsets[2 * i];
        ant01.B.ADROFST1_ANT          = config->window.antennaOffsets[2 * i + 1];
        slot->BE[passId].ANTOFST[i].U = ant01.U;
    }
}


void IfxSpu_setupNci(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_NciConfig *config)
{
    Ifx_SPU_BE_NCICTRL ncictrl;
    ncictrl.U = 0;

    if (config != NULL_PTR)
    {
        ncictrl.U = slot->BE[passId].NCICTRL.U;

        /* Enable / Disable NCI */
        ncictrl.B.EN = config->enabled;

        /* set NCI base address (converted from byte address) */
        ncictrl.B.BASE = config->baseAddress / 32;

        /* set NCI format */
        ncictrl.B.FORMAT = config->outputFormat;

        /* set NCI scaling factor */
        ncictrl.B.SCALE = config->scalingFactor;
    }

    slot->BE[passId].NCICTRL.U = ncictrl.U;
}


void IfxSpu_setupNciWeights(IfxSpu_ConfigSlot *slot, uint16 weights[IFXSPU_MAX_ANTENNAE])
{
    Ifx_SPU_NCI_SCALAR0 nciScalar0;
    Ifx_SPU_NCI_SCALAR1 nciScalar1;
    Ifx_SPU_NCI_SCALAR2 nciScalar2;
    Ifx_SPU_NCI_SCALAR3 nciScalar3;

    nciScalar0.U = slot->NCI.SCALAR0.U;
    nciScalar1.U = slot->NCI.SCALAR1.U;
    nciScalar2.U = slot->NCI.SCALAR2.U;
    nciScalar3.U = slot->NCI.SCALAR3.U;

    if (weights != NULL_PTR)
    {
        /* Set NCI Antenna Scaling factors */
        nciScalar0.B.ANT0 = weights[0];
        nciScalar0.B.ANT1 = weights[1];
        nciScalar1.B.ANT2 = weights[2];
        nciScalar1.B.ANT3 = weights[3];
        nciScalar2.B.ANT4 = weights[4];
        nciScalar2.B.ANT5 = weights[5];
        nciScalar3.B.ANT6 = weights[6];
        nciScalar3.B.ANT7 = weights[7];
    }
    else
    {
        nciScalar0.U = 0xFFFFFFFFu;
        nciScalar1.U = 0xFFFFFFFFu;
        nciScalar2.U = 0xFFFFFFFFu;
        nciScalar3.U = 0xFFFFFFFFu;
    }

    slot->NCI.SCALAR0.U = nciScalar0.U;
    slot->NCI.SCALAR1.U = nciScalar1.U;
    slot->NCI.SCALAR2.U = nciScalar2.U;
    slot->NCI.SCALAR3.U = nciScalar3.U;
}


void IfxSpu_setupPowerChannel(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_PowerOutputConfig *config)
{
    Ifx_SPU_BE_PWRCTRL pwrctrl;
    pwrctrl.U = slot->BE[passId].PWRCTRL.U;

    if (config != NULL_PTR)
    {
        /* Enable / Disable Power information channel */
        pwrctrl.B.EN = config->enabled;

        /* set Power channel base address (converted from byte address) */
        pwrctrl.B.BASE = config->baseAddress / 32;
    }
    else
    {
        pwrctrl.B.EN   = 0;
        pwrctrl.B.BASE = 0;
    }

    slot->BE[passId].PWRCTRL.U = pwrctrl.U;
}


void IfxSpu_setupSidebandStatistics(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_SidebandStatisticsConfig *config)
{
    Ifx_SPU_BE_SBCTRL sbctrl;
    sbctrl.U = slot->BE[passId].SBCTRL.U;

    if (config != NULL_PTR)
    {
        /* Enable / Disable Sideband statistics */
        sbctrl.B.EN = config->enabled;

        /* set Sideband statistics base address (converted from byte address) */
        sbctrl.B.BASE = config->baseAddress / 32;
    }
    else
    {
        sbctrl.B.EN   = 0;
        sbctrl.B.BASE = 0;
    }

    slot->BE[passId].SBCTRL.U = sbctrl.U;
}


void IfxSpu_setupSummation(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_SummationConfig *config)
{
    Ifx_SPU_BE_PWRSUM  pwrSum;
    Ifx_SPU_BE_SUMCTRL sumctrl;
    sumctrl.U = slot->BE[passId].SUMCTRL.U;
    pwrSum.U  = slot->BE[passId].PWRSUM.U;

    if (config != NULL_PTR)
    {
        /* set base address (converted from byte address) */
        sumctrl.B.BASE = config->baseAddress / 32;

        /* set operating mode on complex or magnitude data */
        sumctrl.B.SUMMODE = config->mode;

        /* set real component only calculations */
        sumctrl.B.REAL = config->dataFormat;

        /* set Antennae to use */
        sumctrl.B.USEANT = config->antennaeToUse;

        /* set Output data format */
        pwrSum.B.PRECISION = config->precision;

        /* set Scaling */
        pwrSum.B.SCALE = config->shift;
    }
    else
    {
        sumctrl.B.BASE     = 0;
        sumctrl.B.SUMMODE  = 0;
        sumctrl.B.REAL     = 0;
        sumctrl.B.USEANT   = 0;
        pwrSum.B.PRECISION = 0;
        pwrSum.B.SCALE     = 0;
    }

    slot->BE[passId].SUMCTRL.U = sumctrl.U;
    slot->BE[passId].PWRSUM.U  = pwrSum.U;
}


void IfxSpu_swTrigger(Ifx_SPU *spu)
{
    Ifx_SPU_CTRL ctrl;
    ctrl.U = spu->CTRL.U;

    /* set the trigger mode as reload*/
    ctrl.B.MODE = IfxSpu_TriggerMode_software;

    /* set the software trigger */
    ctrl.B.TRIG = TRUE;

    /* write 32 bit at once */
    spu->CTRL.U = ctrl.U;
}


IFX_STATIC uint32 IfxSpu_crc32single(uint32 data, uint32 crc)
{
    uint8 *bytes = (uint8 *)&data;
    sint32 i;
    uint32 c;

    for (i = 0; i < 4; i++)
    {
        c   = crc ^ (((uint32)IfxSpu_reflect8[bytes[3 - i]]) << 24U);
        crc = (c << 8U) ^ IfxSpu_crcTable[(c >> 24U)];
    }

    return crc;
}


IFX_STATIC uint32 IfxSpu_crc32final(uint32 crc)
{
    uint32 crcTmp;

    uint8 *obytes = (void *)&crcTmp;
    uint8 *bytes  = (void *)&crc;

    obytes[0] = IfxSpu_reflect8[bytes[3]];
    obytes[1] = IfxSpu_reflect8[bytes[2]];
    obytes[2] = IfxSpu_reflect8[bytes[1]];
    obytes[3] = IfxSpu_reflect8[bytes[0]];

    return crcTmp ^ 0xFFFFFFFFUL;
}


IFX_STATIC uint32 IfxSpu_crc32calculate(void *data, uint32 count)
{
    uint32  crc    = 0xFFFFFFFFUL;
    uint32  i;
    uint32 *data32 = data;

    for (i = 0; i < count; i++)
    {
        crc = IfxSpu_crc32single(data32[i], crc);
    }

    crc = IfxSpu_crc32final(crc);

    return crc;
}


uint32 IfxSpu_calculateRegisterCrc(IfxSpu_ConfigSlot *slot)
{
    IfxSpu_ConfigSlot slotTmp;
    uint32            k;
    uint32            count = 1 + (IFXSPU_OFFSETOF(IfxSpu_ConfigSlot, CTRL) / sizeof(uint32));

    uint32           *dst   = (void *)&slotTmp;
    uint32           *src   = (void *)slot;

    for (k = 0; k < count; k++)
    {
        dst[k] = src[k];
    }

    /* Unused registers, REGCRC and volatile registers are assumed to be zero */
    *((volatile Ifx_UReg_32Bit *)&(slotTmp.BE[0].UNLDR_ACFG.U)) = 0;
    *((volatile Ifx_UReg_32Bit *)&(slotTmp.BE[1].UNLDR_ACFG.U)) = 0;
    slotTmp.ID.RM_ACFG0.U                                       = 0;
    slotTmp.ID.RM_ACFG1.U                                       = 0;
    slotTmp.ACFG2.U                                             = 0;
    slotTmp.reserved                                            = 0;
    slotTmp.REGCRC.U                                            = 0;

    /* Unused bitfields as zero */
    slotTmp.ID.CONF.B.reserved_15    = 0;
    slotTmp.ID.CONF.B.reserved_27    = 0;
    slotTmp.ID.CONF2.B.reserved_19   = 0;
    slotTmp.ID.CONF2.B.reserved_26   = 0;
    slotTmp.ID.RM_BLO.B.reserved_24  = 0;
    slotTmp.ID.RM_BLR.B.reserved_13  = 0;
    slotTmp.ID.RM_CONF.B.reserved_23 = 0;
    slotTmp.ID.RM_CONF.B.reserved_27 = 0;
    slotTmp.ID.RM_ILO.B.reserved_24  = 0;
    slotTmp.ID.RM_IOLR.B.reserved_13 = 0;
    slotTmp.ID.RM_IOLR.B.reserved_29 = 0;
    slotTmp.ID.RM_OLO.B.reserved_24  = 0;
    slotTmp.PACTR.B.reserved_27      = 0;
    slotTmp.PACTR.B.COUNT            = 0;
    slotTmp.PACTR.B.RST              = 0;
    slotTmp.DPASS_CONF.B.reserved_6  = 0;

    for (k = 0; k < 2; k++)
    {
        slotTmp.BE[k].LDR_CONF2.B.reserved_29  = 0;
        slotTmp.BE[k].UNLDR_CONF.B.reserved_11 = 0;
        slotTmp.BE[k].ODP_CONF.B.reserved_30   = 0;
        slotTmp.BE[k].NCICTRL.B.reserved_24    = 0;
        slotTmp.BE[k].SUMCTRL.B.reserved_31    = 0;
        slotTmp.BE[k].PWRSUM.B.reserved_22     = 0;
        slotTmp.BE[k].PWRCTRL.B.reserved_19    = 0;
        slotTmp.BE[k].SBCTRL.B.reserved_19     = 0;
    }

    slotTmp.CFAR.CFG1.B.reserved_13  = 0;
    slotTmp.CFAR.CFG2.B.reserved_25  = 0;
    slotTmp.BINREJCTRL.B.reserved_28 = 0;
    slotTmp.CTRL.B.TRIG              = 0;
    slotTmp.CTRL.B.BUSY              = 0;
    slotTmp.CTRL.B.reserved_28       = 0;

    return IfxSpu_crc32calculate((void *)&slotTmp, count);
}


void IfxSpu_writeConfigSlot(Ifx_SPU *spu, uint32 slotNr, const uint32 *slotValues)
{
    IfxSpu_ConfigSlot *slot     = IfxSpu_getConfigSlot(spu, slotNr);
    volatile uint32   *destReg  = (volatile uint32 *)&(slot->ID.CONF.U);
    volatile uint32   *finalReg = (volatile uint32 *)&(slot->CTRL.U);

    for ( ; destReg <= finalReg; destReg++)
    {
        *destReg   = *slotValues;
        slotValues = &(slotValues[1]);
    }

    slot->CTRL.B.BUSY = 0;
}


void IfxSpu_setupLog2PowerSummation(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_Log2PowerSummationConfig *config)
{
    Ifx_SPU_BE_PWRSUM  pwrSum;
    Ifx_SPU_BE_SUMCTRL sumctrl;
    sumctrl.U = slot->BE[passId].SUMCTRL.U;
    pwrSum.U  = slot->BE[passId].PWRSUM.U;

    if (config != NULL_PTR)
    {
        /* set base address (converted from byte address) */
        pwrSum.B.BASE = config->baseAddress / 32;

        /* set operating mode in power domain */
        sumctrl.B.PWRMODE = config->enabled;
    }
    else
    {
        pwrSum.B.BASE     = 0;
        sumctrl.B.PWRMODE = IfxSpu_Summation_PowerMode_off;
    }

    slot->BE[passId].SUMCTRL.U = sumctrl.U;
    slot->BE[passId].PWRSUM.U  = pwrSum.U;
}


void IfxSpu_enableModule(Ifx_SPU *spu)
{
    if (spu->CLC.B.DISS != 0)
    {
        spu->CLC.B.DISR = 0;

        while (spu->CLC.B.DISS != 0)
        {}
    }
}


void IfxSpu_resetModule(Ifx_SPU *spu)
{
    /* The kernel reset will be executed if the reset bits of both kernel registers are set. */
    spu->KRST1.B.RST = 0x1;
    spu->KRST0.B.RST = 0x1;

    while (spu->KRST0.B.RSTSTAT == 0)
    {}

    spu->KRSTCLR.B.CLR = 0x1;
}


void IfxSpu_waitReload(Ifx_SPU *spu)
{
    while (spu->CTRL.B.MODE == IfxSpu_TriggerMode_reload)
    {}
}
