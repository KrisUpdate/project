/**
 * \file IfxSdmmc_Sd.c
 * \brief SDMMC SD details
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

#include "IfxSdmmc_Sd.h"

/** \addtogroup IfxLld_Sdmmc_Sd_SupportFunctions
 * \{ */

/******************************************************************************/
/*-----------------------Private Function Prototypes--------------------------*/
/******************************************************************************/

/** \brief Set voltage window in the OCR register
 * \param sd Handle for SD interface
 * \return Status
 */
IFX_STATIC IfxSdmmc_Status IfxSdmmc_Sd_setVoltageWindow(IfxSdmmc_Sd *sd);

/** \brief Validate Interface Condition of Host and Card, (CMD8)
 * \param sd Handle for SD interface
 * \return Status
 */
IFX_STATIC IfxSdmmc_Status IfxSdmmc_Sd_validateInterfaceCondition(IfxSdmmc_Sd *sd);

/** \brief Validate the Operating Condition of the Card, (ACMD41)
 * \param sd Handle for SD interface
 * \return Status
 */
IFX_STATIC IfxSdmmc_Status IfxSdmmc_Sd_validateOperatingCondition(IfxSdmmc_Sd *sd);

/** \} */

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/** \brief Array holding the running block sizes of functions
 */
IFX_STATIC uint32 IfxSdmmc_Sd_FuncBlockSize[8] = {0};

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

IfxSdmmc_Status IfxSdmmc_Sd_getLockStatus(IfxSdmmc_Sd *sd, IfxSdmmc_CardLockStatus *lockStatus)
{
    IfxSdmmc_Status   status   = IfxSdmmc_Status_failure;
    IfxSdmmc_Response response;
    uint32            argument = 0UL;

    argument |= (sd->cardInfo.rca << 16);

    /* Send CMD13 to read card status  */
    status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_sendStatus, argument, IfxSdmmc_ResponseType_r1, &response);

    if (status == IfxSdmmc_Status_success)
    {
        /* Lock/unlock status bit in the CSR register */
        if (response.cardStatus.B.locked != 0)
        {
            *lockStatus    = IfxSdmmc_CardLockStatus_locked;
            sd->cardState |= (uint8)IfxSdmmc_CardState_locked;
        }
        else
        {
            *lockStatus    = IfxSdmmc_CardLockStatus_unlocked;
            sd->cardState &= (uint8)~(uint8)(IfxSdmmc_CardState_locked);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_initCard(IfxSdmmc_Sd *sd, IfxSdmmc_Sd_CardConfig *cardConfig)
{
    IfxSdmmc_Status         status     = IfxSdmmc_Status_success;
    IfxSdmmc_CardLockStatus lockStatus = IfxSdmmc_CardLockStatus_unlocked;
    IfxSdmmc_Response       response;
    IfxSdmmc_Sd_Flags      *sdflags    = &(sd->flags);

    /* set up the SD card interface, power up the card */
    /* power on bus */
    IfxSdmmc_setBusPowerOn(sd->sdmmcSFR);

    /* Enable the SD clock */
    IfxSdmmc_enableCardClock(sd->sdmmcSFR);

    /* according to SD Card spec we've to wait for 74 cycles with CMD and DAT high */
    {
        uint32 loopCount = (IFXSDMMC_DELAY_74CYCLE_US * 1000) / IFXSDMMC_1KLOOPDELAY_US;
        uint32 index;

        for (index = 0; index < loopCount; ++index)
        {
            //do nothing
            __nop();
        }
    }

    /* FIXME: configure HOST_CTRL2.USH2_IF_ENABLE = 0; UHS_MODE_SEL = 0; default values are 0 anyway */

    /* Initialize and identify the SD card */
    /* reset all flags first */
    sd->flags.f2      = FALSE;
    sd->flags.f8      = FALSE;
    sd->flags.ioInit  = FALSE;
    sd->flags.memInit = FALSE;

    /* Reset the card (CMD0) */
    status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_goIdleState, IFXSDMMC_ARG_NONE, IfxSdmmc_ResponseType_none, NULL_PTR);

    if (status == IfxSdmmc_Status_success)
    {
        /* validate Interface condition (CMD8) */
        status = IfxSdmmc_Sd_validateInterfaceCondition(sd);
    }

    if (status == IfxSdmmc_Status_success)
    {
        if (sdflags->supportIO)           /* Check if IO mode is supported */
        {
            if (sdflags->ioInit == FALSE) /* IO is not initialized */
            {
                /* Validate Operating Condition for SDIO (CMD5)*/
                status = IfxSdmmc_Sd_ioValidateOperatingCondition(sd);

                if (status == IfxSdmmc_Status_success)
                {
                    sdflags->ioInit = TRUE; /* IO is now initialized */

                    /* update memory present status */
                    sdflags->memoryPresent = (sd->cardInfo.io.ioInfo.bits.mp != 0);
                }
                else
                {
                    sdflags->ioInit = FALSE;    /* IO is NOT initialized */
                }
            }
        }

        if (sdflags->supportMEM && sdflags->memoryPresent)
        {
            /* validate operating condition (ACMD41) */
            status = IfxSdmmc_Sd_validateOperatingCondition(sd);

            if (status == IfxSdmmc_Status_success)
            {
                // Update mem flag here.
                sdflags->memInit = TRUE;
            }
        }

        /* we skip voltage switch sequences (CMD11 based on S18A) */
        /* This is because our controller does not support 1.8V */

        if (status == IfxSdmmc_Status_success)
        {
            if ((sdflags->f2 == FALSE) && (sdflags->memInit))   /* CMD2 not completed */
            {
                /* Read CID (CMD2)*/
                status = IfxSdmmc_Sd_readCid(sd);
            }

            if (status == IfxSdmmc_Status_success)
            {
                // set F2 flag here.
                sdflags->f2 = TRUE;

                /* Read RCA (CMD3)*/
                status = IfxSdmmc_Sd_readRca(sd);

                if (status == IfxSdmmc_Status_success)
                {
                    // now based on MEM / IO flags:
                    // populate the SD card type
                    // Capacity of card is initialized during ACMD41 test
                    sd->cardType = (IfxSdmmc_SdCardType)((uint8)(sdflags->ioInit) | ((uint8)(sdflags->memInit) << 1));

                    /* Check card lock/unlock status */
                    if (sd->cardType != IfxSdmmc_SdCardType_io)
                    {
                        status = IfxSdmmc_Sd_getLockStatus(sd, &lockStatus);
                    }

                    if (status == IfxSdmmc_Status_success)
                    {
                        if (lockStatus == IfxSdmmc_CardLockStatus_locked)
                        {
                            status = IfxSdmmc_Status_initialisedButLocked;
                        }

                        /* Update card initialization status */
                        sd->cardState &= (uint8)~((uint8)IfxSdmmc_CardState_notInitialised | (uint8)IfxSdmmc_CardState_noCard);
                    }
                }
            }
        }
    }

    /* switch speed and bus width */
    if (status == IfxSdmmc_Status_success)
    {
        if (sd->cardType != IfxSdmmc_SdCardType_io)
        {
            /* Switch to transfer state (CMD7) */
            status = IfxSdmmc_Sd_switchToTransferState(sd);
        }
        else
        {
            status = IfxSdmmc_Sd_ioSwitchToTransferState(sd);
        }

        if (status == IfxSdmmc_Status_success)
        {
            /* switch card clock to desired frequency now */
            if (sd->presetMode == TRUE)
            {
                IfxSdmmc_disableCardClock(sd->sdmmcSFR);
                IfxSdmmc_enablePresetValues(sd->sdmmcSFR);
                IfxSdmmc_enableCardClock(sd->sdmmcSFR);
            }
            else
            {
                IfxSdmmc_switchClockFrequency(sd->sdmmcSFR, sd->userFrequency);
            }
        }

        if (sd->cardType != IfxSdmmc_SdCardType_io)
        {
            /* read SCR register contents */
            status = IfxSdmmc_Sd_readScr(sd);
        }

        if (status == IfxSdmmc_Status_success)
        {
            /* support SDIO switches also here */
            /* switch Bus Width if selected */
            if (cardConfig->dataWidth == IfxSdmmc_SdDataTransferWidth_4Bit)
            {
                status = IfxSdmmc_Sd_switchToBusWidth4(sd);
            }

            /* Switch Speed if selected */
            if (cardConfig->speedMode == IfxSdmmc_SdSpeedMode_high)
            {
                status = IfxSdmmc_Sd_switchToHighSpeed(sd);
            }
        }
    }

    if (status == IfxSdmmc_Status_success)
    {
        if (sd->cardType != IfxSdmmc_SdCardType_io)
        {
            /* Explicitly set block size as 512 */
            status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_setBlockLength, 512, IfxSdmmc_ResponseType_r1, &response);
        }

        if (sd->cardType != IfxSdmmc_SdCardType_mem)
        {
            /* Initialize the block sizes for all functions internally */
            uint8 func = 0;

            while (func < (uint8)8)
            {
                IfxSdmmc_Sd_FuncBlockSize[func] = (uint32)0;
                func++;
            }
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_initHostController(IfxSdmmc_Sd *sd, IfxSdmmc_Sd_HostConfig *hostConfig)
{
    IfxSdmmc_Status status = IfxSdmmc_Status_success;
    /* set the bus voltage */
    IfxSdmmc_setBusVoltage(sd->sdmmcSFR);

    /* set the data line timeout value */
    IfxSdmmc_setDataLineTimeoutValue(sd->sdmmcSFR, hostConfig->timeoutValue);

    /* read and set Host Support Flags */
    IfxSdmmc_SdModes mode = hostConfig->supportedModes;
    sd->flags.supportIO  = (mode == IfxSdmmc_SdModes_combo) || (mode == IfxSdmmc_SdModes_ioOnly);
    sd->flags.supportMEM = (mode == IfxSdmmc_SdModes_combo) || (mode == IfxSdmmc_SdModes_memOnly);

    /* update memory present flag by default */
    sd->flags.memoryPresent = TRUE;

    /* version is above 4 no need to check version register */

    /* set up clock for 400KHz now; we will change later */
    IfxSdmmc_configureClock(sd->sdmmcSFR, IFXSDMMC_INIT_FREQUENCY);

    if (hostConfig->usePresetValues == TRUE)
    {
        sd->presetMode = TRUE;
    }
    else
    {
        sd->presetMode    = FALSE;
        sd->userFrequency = hostConfig->frequency;
    }

    /* enable internal clock */
    status = IfxSdmmc_setUpInternalClock(sd->sdmmcSFR);

    /* enable Host version 4 */
    IfxSdmmc_enableHostControllerVersion4(sd->sdmmcSFR);

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_initModule(IfxSdmmc_Sd *sd, IfxSdmmc_Sd_Config *config)
{
    IfxSdmmc_Status status   = IfxSdmmc_Status_success;

    Ifx_SDMMC      *sdmmcSFR = config->sdmmcSFR;
    sd->sdmmcSFR = sdmmcSFR;
    sd->dmaUsed  = config->useDma;
    sd->dmaType  = config->dmaConfig.dmaType;

    /* enable module */
    IfxSdmmc_enableModule(config->sdmmcSFR);

    /* FIXME: apply kernal reset or apply software reset */
    //IfxSdmmc_resetmodule(config->sdmmcSFR);
    IfxSdmmc_applySoftwareReset(config->sdmmcSFR);

    /* initialise the host controller for the card type */
    IfxSdmmc_Sd_initHostController(sd, &config->hostConfig);

    /* set DMA type if DMA is used for transfers */
    if (config->useDma)
    {
        IfxSdmmc_setDmaType(sd->sdmmcSFR, config->dmaConfig.dmaType);
    }

    /* enable the necessary normal inteerupt status flags*/
    IfxSdmmc_enableNormalInterruptStatus(sdmmcSFR, IfxSdmmc_NormalInterrupt_commandComplete);
    IfxSdmmc_enableNormalInterruptStatus(sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete);
    IfxSdmmc_enableNormalInterruptStatus(sdmmcSFR, IfxSdmmc_NormalInterrupt_dma);
    IfxSdmmc_enableNormalInterruptStatus(sdmmcSFR, IfxSdmmc_NormalInterrupt_bufferWriteReady);
    IfxSdmmc_enableNormalInterruptStatus(sdmmcSFR, IfxSdmmc_NormalInterrupt_bufferReadReady);

    /* enable all the error interrupt status flags */
    IfxSdmmc_enableAllErrorInterruptStatus(sdmmcSFR);

    /* initilaise the normal interrupts */
    if (config->interruptConfig.priority > 0)
    {
        if (config->interruptConfig.commandCompleteInterruptEnable)
        {
            IfxSdmmc_enableNormalInterrupt(sdmmcSFR, IfxSdmmc_NormalInterrupt_commandComplete);
        }

        if (config->interruptConfig.transferCompleteInterruptEnable)
        {
            IfxSdmmc_enableNormalInterrupt(sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete);
        }

        if (config->interruptConfig.dmaInterruptEnable)
        {
            IfxSdmmc_enableNormalInterrupt(sdmmcSFR, IfxSdmmc_NormalInterrupt_dma);
        }

        if (config->interruptConfig.commandTimeoutInterruptEnable)
        {
            IfxSdmmc_enableErrorInterrupt(sdmmcSFR, IfxSdmmc_ErrorInterrupt_commandTimeout);
        }

        if (config->interruptConfig.dataTimeoutInterruptEnable)
        {
            IfxSdmmc_enableErrorInterrupt(sdmmcSFR, IfxSdmmc_ErrorInterrupt_dataTimeout);
        }

        /* initialise service request node */
        volatile Ifx_SRC_SRCR *srcSFR;
        srcSFR = IfxSdmmc_getSrcPointer(sdmmcSFR, IfxSdmmc_InterruptType_normal);
        IfxSrc_init(srcSFR, config->interruptConfig.provider, config->interruptConfig.priority);
        IfxSrc_enable(srcSFR);
    }

    /* setup pins */
    if (config->pins != NULL_PTR)
    {
        IfxSdmmc_Sd_setupPins(sd, config->pins);
    }

    /* set up, initialise and identify the card  */
    status = IfxSdmmc_Sd_initCard(sd, &config->cardConfig);

    return status;
}


void IfxSdmmc_Sd_initModuleConfig(IfxSdmmc_Sd_Config *config, Ifx_SDMMC *sdmmcSFR)
{
    const IfxSdmmc_Sd_Config defaultConfig = {
        .sdmmcSFR   = NULL_PTR,
        .hostConfig = {
            .timeoutValue    = IfxSdmmc_DataLineTimeout_2Power27,
            .usePresetValues = TRUE,
            .frequency       = 10000000,
            .supportedModes  = IfxSdmmc_SdModes_memOnly
        },
        .interruptConfig                     = {
            .commandCompleteInterruptEnable  = FALSE,
            .transferCompleteInterruptEnable = FALSE,
            .dmaInterruptEnable              = FALSE,
            .commandTimeoutInterruptEnable   = FALSE,
            .dataTimeoutInterruptEnable      = FALSE,
            .priority                        = 0,
            .provider                        = IfxSrc_Tos_cpu0
        },
        .pins       = NULL_PTR,
        .cardConfig = {
            .dataWidth = IfxSdmmc_SdDataTransferWidth_1Bit,
            .speedMode = IfxSdmmc_SdSpeedMode_normal
        },
        .useDma    = FALSE,
        .dmaConfig = {
            .dmaType                         = IfxSdmmc_DmaType_sdma
        }
    };

    /* Default Configuration */
    *config = defaultConfig;

    /* take over module pointer */
    config->sdmmcSFR = sdmmcSFR;
}


IfxSdmmc_Status IfxSdmmc_Sd_readBlock(IfxSdmmc_Sd *sd, uint32 address, uint32 *data)
{
    IfxSdmmc_Status status  = IfxSdmmc_Status_success;
    uint32          timeout = 0;

    /* If the card is not initialized */
    if ((sd->cardState & (uint8)IfxSdmmc_CardState_notInitialised) != 0U)
    {
        status = IfxSdmmc_Status_cardNotInitialised;
    }
    else if ((sd->cardState & (uint8)IfxSdmmc_CardState_locked) != 0U)
    {
        status = IfxSdmmc_Status_cardLocked;
    }
    else
    {
        /* FIXME: check Sector bound if needed */

        /* If not block addressing, then multiply by 512 bytes */
        if (((uint32)sd->cardCapacity & (uint32)IfxSdmmc_SdCardCapacity_blockAddressing) == 0U)
        {
            address = (uint32)(address * 512U);
        }

        if (sd->dmaUsed == TRUE)
        {
            status = IfxSdmmc_Sd_singleBlockDmaTransfer(sd, IfxSdmmc_Command_readSingleBlock, address, IFXSDMMC_BLOCK_SIZE_DEFAULT, data, IfxSdmmc_TransferDirection_read);
        }
        else
        {
            /* Single block transfer function */
            status = IfxSdmmc_Sd_singleBlockTransfer(sd, IfxSdmmc_Command_readSingleBlock, address, IFXSDMMC_BLOCK_SIZE_DEFAULT, data, IfxSdmmc_TransferDirection_read);
        }
    }

    /* Wait for until the command OR data lines aren't busy */
    /* check if command and data lines are free */
    timeout = 1000000; // FIXME: get the recommeneded timeout value

    while ((IfxSdmmc_isDataLineBusy(sd->sdmmcSFR) || (IfxSdmmc_isCommandLineBusy(sd->sdmmcSFR))) && (timeout > 0))
    {
        timeout--;
    }

    if (timeout == 0)
    {
        status = IfxSdmmc_Status_timeout;
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_readCid(IfxSdmmc_Sd *sd)
{
    IfxSdmmc_Status   status = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;

    status              = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_allSendCID, IFXSDMMC_ARG_NONE, IfxSdmmc_ResponseType_r2, &response);
    sd->cardInfo.cid[0] = response.resp01;
    sd->cardInfo.cid[1] = response.resp23;
    sd->cardInfo.cid[2] = response.resp45;
    sd->cardInfo.cid[3] = response.resp67;

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_readRca(IfxSdmmc_Sd *sd)
{
    IfxSdmmc_Status   status = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_sendRelativeAddress, IFXSDMMC_ARG_NONE, IfxSdmmc_ResponseType_r6, &response);

    if (status == IfxSdmmc_Status_success)
    {
        sd->cardInfo.rca = (uint16)((response.resp01 & 0xFFFF0000) >> 16);
    }
    else
    {
        sd->cardInfo.rca = IFXSDMMC_RCA_NONE;
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_readScr(IfxSdmmc_Sd *sd)
{
    IfxSdmmc_Status      status   = IfxSdmmc_Status_success;
    uint32               argument = 0;
    IfxSdmmc_CardReg_SCR scr;

    status = IfxSdmmc_sendApplicationCommand(sd->sdmmcSFR, sd->cardInfo.rca);

    if (status == IfxSdmmc_Status_success)
    {
        IfxSdmmc_setTransferDirection(sd->sdmmcSFR, IfxSdmmc_TransferDirection_read);
        status           = IfxSdmmc_Sd_singleBlockTransfer(sd, IfxSdmmc_Command_sendSCR, argument, 8, (uint32 *)&scr, IfxSdmmc_TransferDirection_read);
        sd->cardInfo.scr = scr;
    }

    return status;
}


IFX_STATIC IfxSdmmc_Status IfxSdmmc_Sd_setVoltageWindow(IfxSdmmc_Sd *sd)
{
    IfxSdmmc_Status   status       = IfxSdmmc_Status_failure;
    IfxSdmmc_Response response;
    uint32            loopCount    = 0U;
    uint32            argument     = 0U;
    uint32            retryLoopCtr = 0U;

    if (sd->flags.f8 == 0U)
    {
        argument = IFXSDMMC_ARG_ACMD41_F80; /* Set HCS=0 for standard cards */
    }
    else
    {
        argument = IFXSDMMC_ARG_ACMD41_F81; /* Set HCS=1 for high capacity cards */
    }

    do
    {
        /* Send CMD55 for application specific commands (default RCA: 0) */
        status = IfxSdmmc_sendApplicationCommand(sd->sdmmcSFR, IFXSDMMC_RCA_NONE);

        if (status == IfxSdmmc_Status_success)
        {
            /* Send ACMD41 to set the WV  */
            status           = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_sdSendOpCond, argument, IfxSdmmc_ResponseType_r3, &response);
            sd->cardInfo.ocr = response.resp01;

            if ((status != IfxSdmmc_Status_success) && (status != IfxSdmmc_Status_commandError))
            {
                break;
            }

            status = IfxSdmmc_Status_success;

            /* This command to be sent every 3ms for 1 second */
            {
                retryLoopCtr = (IFXSDMMC_RETRYDELAY_ACMD41_US * 1000) / IFXSDMMC_1KLOOPDELAY_US;
                uint32 index;

                for (index = 0; index < retryLoopCtr; ++index)
                {
                    //do nothing
                    __nop();
                }
            }
        }
        else
        {
            loopCount = IFXSDMMC_RETRYCOUNT_ACMD41_1S;
        }

        /* send this command for 1second, or until busy bit in OCR = 1 */
    } while (((loopCount++) < IFXSDMMC_RETRYCOUNT_ACMD41_1S) && (!(((uint32)sd->cardInfo.ocr & IFXSDMMC_CARD_POWERUP_STATUS_MASK) != 0)));

    return status;
}


void IfxSdmmc_Sd_setupPins(IfxSdmmc_Sd *sd, IfxSdmmc_Sd_Pins *pins)
{
    IfxPort_InputMode    inMode     = pins->inputMode;
    IfxPort_PadDriver    speedGrade = pins->pinDriver;

    IfxSdmmc_Clk_Out    *clk        = pins->clk;
    IfxSdmmc_Cmd_InOut  *cmd        = pins->cmd;
    IfxSdmmc_Dat0_InOut *dat0       = pins->dat0;
    IfxSdmmc_Dat1_InOut *dat1       = pins->dat1;
    IfxSdmmc_Dat2_InOut *dat2       = pins->dat2;
    IfxSdmmc_Dat3_InOut *dat3       = pins->dat3;

    if (clk != NULL_PTR)
    {
        IfxPort_setPinModeOutput(clk->pin.port, clk->pin.pinIndex, IfxPort_OutputMode_pushPull, clk->outSelect);
        IfxPort_setPinPadDriver(clk->pin.port, clk->pin.pinIndex, speedGrade);
    }

    if (cmd != NULL_PTR)
    {
        IfxPort_setPinModeInput(cmd->pin.port, cmd->pin.pinIndex, inMode);
        IfxPort_setPinPadDriver(cmd->pin.port, cmd->pin.pinIndex, speedGrade);
    }

    if (dat0 != NULL_PTR)
    {
        IfxPort_setPinModeInput(dat0->pin.port, dat0->pin.pinIndex, inMode);
        IfxPort_setPinPadDriver(dat0->pin.port, dat0->pin.pinIndex, speedGrade);
    }

    if (dat1 != NULL_PTR)
    {
        IfxPort_setPinModeInput(dat1->pin.port, dat1->pin.pinIndex, inMode);
        IfxPort_setPinPadDriver(dat1->pin.port, dat1->pin.pinIndex, speedGrade);
    }

    if (dat2 != NULL_PTR)
    {
        IfxPort_setPinModeInput(dat2->pin.port, dat2->pin.pinIndex, inMode);
        IfxPort_setPinPadDriver(dat2->pin.port, dat2->pin.pinIndex, speedGrade);
    }

    if (dat3 != NULL_PTR)
    {
        IfxPort_setPinModeInput(dat3->pin.port, dat3->pin.pinIndex, inMode);
        IfxPort_setPinPadDriver(dat3->pin.port, dat3->pin.pinIndex, speedGrade);
    }
}


IfxSdmmc_Status IfxSdmmc_Sd_singleBlockAdma2Transfer(IfxSdmmc_Sd *sd, IfxSdmmc_Command command, uint32 address, uint16 blockSize, uint32 *descrAddress, IfxSdmmc_TransferDirection direction)
{
    IfxSdmmc_Status   status  = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            timeout = 0;

    /* set the system address */
    IfxSdmmc_setSystemAddressForDma(sd->sdmmcSFR, (uint32)descrAddress);

    IfxSdmmc_setBlockSize(sd->sdmmcSFR, blockSize);

    /* FIXME: set block count if needed  for host control ver 4*/

    IfxSdmmc_setMultiBlockSelect(sd->sdmmcSFR, FALSE);

    IfxSdmmc_disableBlockCount(sd->sdmmcSFR);

    IfxSdmmc_enableDmaTransfers(sd->sdmmcSFR);

    /* set transfer direction in host controller */
    if (direction == IfxSdmmc_TransferDirection_read)
    {
        IfxSdmmc_setTransferDirection(sd->sdmmcSFR, IfxSdmmc_TransferDirection_read);
    }
    else
    {
        IfxSdmmc_setTransferDirection(sd->sdmmcSFR, IfxSdmmc_TransferDirection_write);
    }

    /* FIXME : disable auto command if needed, by default 0 */

    status = IfxSdmmc_sendCommand(sd->sdmmcSFR, command, address, IfxSdmmc_ResponseType_r1, &response);

    if (status == IfxSdmmc_Status_success)
    {
        timeout = 100000; /* FIXME: get the recommeneded timeout value*/

        /* wait until transfer complete or adma error flags are set */
        while ((IfxSdmmc_isNormalInterruptOccured(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete) == 0) \
               && (IfxSdmmc_isErrorInterruptOccured(sd->sdmmcSFR, IfxSdmmc_ErrorInterrupt_adma) == 0)            \
               && (timeout > 0))
        {
            timeout--;
        }

        if ((timeout == 0) || (IfxSdmmc_isErrorInterruptOccured(sd->sdmmcSFR, IfxSdmmc_ErrorInterrupt_adma) != 0))
        {
            status = IfxSdmmc_Status_dataError;

            if (IfxSdmmc_isErrorInterruptOccured(sd->sdmmcSFR, IfxSdmmc_ErrorInterrupt_adma) != 0)
            {
                IfxSdmmc_clearErrorInterrupt(sd->sdmmcSFR, IfxSdmmc_ErrorInterrupt_adma);
            }

            /* FIME: implement abort command process */
        }
        else
        {
            IfxSdmmc_clearNormalInterrupt(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_singleBlockDmaTransfer(IfxSdmmc_Sd *sd, IfxSdmmc_Command command, uint32 address, uint16 blockSize, uint32 *data, IfxSdmmc_TransferDirection direction)
{
    IfxSdmmc_Status   status  = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            timeout = 0;

    /* set the system address */
    IfxSdmmc_setSystemAddressForDma(sd->sdmmcSFR, (uint32)data);

    IfxSdmmc_setBlockSize(sd->sdmmcSFR, blockSize);

    /* FIXME: set block count if needed  for host control ver 4*/

    IfxSdmmc_setMultiBlockSelect(sd->sdmmcSFR, FALSE);

    IfxSdmmc_disableBlockCount(sd->sdmmcSFR);

    IfxSdmmc_enableDmaTransfers(sd->sdmmcSFR);

    /* set transfer direction in host controller */
    if (direction == IfxSdmmc_TransferDirection_read)
    {
        IfxSdmmc_setTransferDirection(sd->sdmmcSFR, IfxSdmmc_TransferDirection_read);
    }
    else
    {
        IfxSdmmc_setTransferDirection(sd->sdmmcSFR, IfxSdmmc_TransferDirection_write);
    }

    /* FIXME : disable auto command if needed, by default 0 */

    status = IfxSdmmc_sendCommand(sd->sdmmcSFR, command, address, IfxSdmmc_ResponseType_r1, &response);

    if (status == IfxSdmmc_Status_success)
    {
        /* Perform data transfer */
#if 0
        uint32 loopIndex = 0;
        uint32 numWords  = (uint32)((blockSize + 3) >> 2);

        /* need to confirm with concept */
        /* 4 bytes data transmission in each iteration */
        for (loopIndex = 0U; loopIndex < numWords; loopIndex++)
        {
            IfxSdmmc_setSystemAddressForDma(sd->sdmmcSFR, (uint32)data);

            /* wait until dma interrupt flag is set */
            timeout = 1000000; /* FIXME: get the recommeneded timeout value*/

            while ((IfxSdmmc_isNormalInterruptOccured(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_dma) == 0) && (timeout > 0))
            {
                timeout--;
            }

            if (timeout == 0)
            {
                status = IfxSdmmc_Status_dataError;
            }
            else
            {
                status = IfxSdmmc_Status_success;
                IfxSdmmc_clearNormalInterrupt(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_dma);
            }

            data++;
        }

#endif
        timeout = 1000000; /* FIXME: get the recommeneded timeout value*/

        /* wait until transfer complete flag is set */
        while ((IfxSdmmc_isNormalInterruptOccured(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete) == 0) && (timeout > 0))
        {
            timeout--;
        }

        if (timeout == 0)
        {
            status = IfxSdmmc_Status_dataError;
            /* FIME: implement abort command process */
        }
        else
        {
            IfxSdmmc_clearNormalInterrupt(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_singleBlockTransfer(IfxSdmmc_Sd *sd, IfxSdmmc_Command command, uint32 address, uint16 blockSize, uint32 *data, IfxSdmmc_TransferDirection direction)
{
    IfxSdmmc_Status   status    = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            loopIndex = 0;
    uint32            timeout   = 0;
    uint32            numWords  = (uint32)((blockSize + 3) >> 2);

    IfxSdmmc_setBlockSize(sd->sdmmcSFR, blockSize);

    /* FIXME: set block count if needed  for host control ver 4*/

    IfxSdmmc_setMultiBlockSelect(sd->sdmmcSFR, FALSE);

    IfxSdmmc_disableBlockCount(sd->sdmmcSFR);

    /* set transfer direction in host controller */
    if (direction == IfxSdmmc_TransferDirection_read)
    {
        IfxSdmmc_setTransferDirection(sd->sdmmcSFR, IfxSdmmc_TransferDirection_read);
    }
    else
    {
        IfxSdmmc_setTransferDirection(sd->sdmmcSFR, IfxSdmmc_TransferDirection_write);
    }

    /* FIXME : disable auto command if needed, by default 0 */

    status = IfxSdmmc_sendCommand(sd->sdmmcSFR, command, address, IfxSdmmc_ResponseType_r1, &response);

    if (status == IfxSdmmc_Status_success)
    {
        if (direction == IfxSdmmc_TransferDirection_read)
        {
            timeout = 1000000; /* FIXME: get the recommeneded timeout value*/

            /* wait until Read ready flag is set */
            while ((IfxSdmmc_isNormalInterruptOccured(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_bufferReadReady) == 0) && (timeout > 0))
            {
                timeout--;
            }

            if (timeout == 0)
            {
                status = IfxSdmmc_Status_dataError;
                /* FIME: implement abort command process */
            }
            else
            {
                IfxSdmmc_clearNormalInterrupt(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_bufferReadReady);
            }
        }
    }

    if (status == IfxSdmmc_Status_success)
    {
        /* Perform data transfer */
        /* 4 bytes data transmission in each iteration */
        for (loopIndex = 0U; loopIndex < numWords; loopIndex++)
        {
            if (direction == IfxSdmmc_TransferDirection_read)
            {
                *data = (uint32)IfxSdmmc_readBufferData(sd->sdmmcSFR);
            }

            if (direction == IfxSdmmc_TransferDirection_write)
            {
                IfxSdmmc_writeBufferData(sd->sdmmcSFR, data);
            }

            data++;
        }

        timeout = 1000000; /* FIXME: get the recommeneded timeout value*/

        /* wait until transfer complete flag is set */
        while ((IfxSdmmc_isNormalInterruptOccured(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete) == 0) && (timeout > 0))
        {
            timeout--;
        }

        if (timeout == 0)
        {
            status = IfxSdmmc_Status_dataError;
            /* FIME: implement abort command process */
        }
        else
        {
            IfxSdmmc_clearNormalInterrupt(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_switchToBusWidth4(IfxSdmmc_Sd *sd)
{
    IfxSdmmc_Status   status   = IfxSdmmc_Status_failure;
    IfxSdmmc_Response response;
    uint32            argument = 0;
    uint8             cardCpblty;
    uint8             busIfCtrl;

    if (sd->flags.memInit)  /* for Mem cards */
    {
        if (sd->cardInfo.scr.B.sdBusWidth & 0x4)
        {
            argument |= sd->cardInfo.rca << 16;
            status    = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_appCommand, argument, IfxSdmmc_ResponseType_r1, &response);

            if (status == IfxSdmmc_Status_success)
            {
                status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_setBusWidth, IFXSDMMC_ARG_SD_BUSWIDTH_4, IfxSdmmc_ResponseType_r1, &response);
            }
        }
    }

    if (sd->flags.ioInit) /* for IO cards */
    {
        /* read Card Capability register from CCCR */
        status = IfxSdmmc_Sd_ioReadRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_CARDCPBLTY, &cardCpblty);

        /* check if Low speed card supports 4 bit */
        if (((cardCpblty >> IFXSDMMC_SDIO_CCCR_CARDCPBLTY_LSC_BITPOS) & (uint8)0x3) == 0x1)
        {
            status = IfxSdmmc_Status_failure;
        }

        if (status == IfxSdmmc_Status_success)
        {
            /* switch to bus width 4 now */
            /* read BUS IF CTRL register */
            status = IfxSdmmc_Sd_ioReadRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_BUSIFCTRL, &busIfCtrl);

            if (status == IfxSdmmc_Status_success)
            {
                /* reset bus width to 4Bit mode. (it could have been 8 bit before!) */
                busIfCtrl &= (uint8)IfxSdmmc_SdIoTransferWidth_4Bit;
                busIfCtrl |= (uint8)IfxSdmmc_SdIoTransferWidth_4Bit;

                /* write back */
                status = IfxSdmmc_Sd_ioWriteRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_BUSIFCTRL, busIfCtrl);
            }
        }
    }

    if (status == IfxSdmmc_Status_success)
    {
        IfxSdmmc_setSdDataTransferWidth(sd->sdmmcSFR, IfxSdmmc_SdDataTransferWidth_4Bit);
    }
    else
    {
        IfxSdmmc_setSdDataTransferWidth(sd->sdmmcSFR, IfxSdmmc_SdDataTransferWidth_1Bit);
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_switchToHighSpeed(IfxSdmmc_Sd *sd)
{
    IfxSdmmc_Status status           = IfxSdmmc_Status_failure;
    uint32          switchStatus[16] = {0};
    uint8           busSpeedSel;

    if (sd->flags.memInit) /* for memory cards */
    {
        /* Check CMD6 support in SCR register */
        if (sd->cardInfo.scr.B.sdSpec != 0)
        {
            /* Switch to transfer state (CMD7) */
            status = IfxSdmmc_Sd_switchToTransferState(sd);

            if (status == IfxSdmmc_Status_success)
            {
                status = IfxSdmmc_Sd_singleBlockTransfer(sd, IfxSdmmc_Command_switchFunction, IFXSDMMC_ARG_SD_HIGHSPEED, 64, (uint32 *)switchStatus, IfxSdmmc_TransferDirection_read);

                if (status == IfxSdmmc_Status_success)
                {
                    if ((switchStatus[4] & (uint32)0xF) != 1)
                    {
                        status = IfxSdmmc_Status_failure;
                    }
                }
            }
        }
    }

    if ((sd->flags.ioInit) && (status != IfxSdmmc_Status_success))   /* for IO cards */
    {
        /* read Card Capability register from CCCR */
        status = IfxSdmmc_Sd_ioReadRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_BUSSPEEDSEL, &busSpeedSel);

        /* check if Card supports High speed*/
        if (((busSpeedSel >> IFXSDMMC_SDIO_CCCR_BUSSPEEDSEL_SHS_BITPOS) & (uint8)0x1) == 0x1)
        {
            /* switch to high speed now */
            busSpeedSel |= (uint8)0x1 << IFXSDMMC_SDIO_CCCR_BUSSPEEDSEL_EHS_BITPOS;
            /* write back */
            status       = IfxSdmmc_Sd_ioWriteRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_BUSSPEEDSEL, busSpeedSel);
        }
    }

    if (status == IfxSdmmc_Status_success)
    {
        IfxSdmmc_setSdSpeedMode(sd->sdmmcSFR, IfxSdmmc_SdSpeedMode_high);
    }
    else
    {
        IfxSdmmc_setSdSpeedMode(sd->sdmmcSFR, IfxSdmmc_SdSpeedMode_normal);
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_switchToTransferState(IfxSdmmc_Sd *sd)
{
    IfxSdmmc_Status   status   = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            argument = 0;

    argument |= (uint32)(sd->cardInfo.rca << 16);

    /* read card status CMD13 */
    status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_sendStatus, argument, IfxSdmmc_ResponseType_r1, &response);

    if (status == IfxSdmmc_Status_success)
    {
        /* if the card is not in transfer state already,  Switch to transfer state (CMD7)*/
        if (response.cardStatus.B.currentState != 4)
        {
            status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_selectDeselectCard, argument, IfxSdmmc_ResponseType_r1b, &response);
        }
    }

    return status;
}


IFX_STATIC IfxSdmmc_Status IfxSdmmc_Sd_validateInterfaceCondition(IfxSdmmc_Sd *sd)
{
    IfxSdmmc_Status   status   = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            argument = 0;

    /* Check operating condition (cmd8) */
    argument |= (IFXSDMMC_HOST_SUPPORTED_VOLTAGE << 8) | (IFXSDMMC_IF_COND_CHECK_PATTERN);
    status    = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_sendIfCond, argument, IfxSdmmc_ResponseType_r7, &response);

    /* Response received; High capacity cards */
    if (status == IfxSdmmc_Status_success)
    {
        /* Check if pattern matches in both the argument and response */
        if ((response.resp01 & 0xFF) != IFXSDMMC_IF_COND_CHECK_PATTERN)
        {
            status = IfxSdmmc_Status_badResponse;
        }
        /* Check if input voltage accepted by the card in the response  */
        else if (((uint8)(response.resp01 >> 8) & 0xF) != IFXSDMMC_HOST_SUPPORTED_VOLTAGE)
        {
            status = IfxSdmmc_Status_badResponse;
        }
        else
        {
            /* Set Flag f8 */
            sd->flags.f8 = 1U;
        }
    }
    /*  No response is received for Standard Capacity SD cards or MMC card. */
    else
    {
        if (status == IfxSdmmc_Status_commandError)
        {
            sd->flags.f8 = 0U;
            status       = IfxSdmmc_Status_success;
        }
    }

    return status;
}


IFX_STATIC IfxSdmmc_Status IfxSdmmc_Sd_validateOperatingCondition(IfxSdmmc_Sd *sd)
{
    IfxSdmmc_Status   status = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    /* Send CMD55 for application specific commands with 0 as the default RCA */
    status = IfxSdmmc_sendApplicationCommand(sd->sdmmcSFR, IFXSDMMC_RCA_NONE);

    if ((status == IfxSdmmc_Status_success) || ((sd->flags.f8 == 0U) && (status == IfxSdmmc_Status_commandError)))
    {
        /* Send ACMD41 to query the card's voltage window */
        status           = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_sdSendOpCond, IFXSDMMC_RCA_NONE, IfxSdmmc_ResponseType_r3, &response);
        sd->cardInfo.ocr = response.resp01;

        if ((status == IfxSdmmc_Status_commandError) || (status == IfxSdmmc_Status_success))
        {
            /* Send ACMD41 with voltage window argument set */
            status = IfxSdmmc_Sd_setVoltageWindow(sd);

            /* SD Card Capacity */
            if (status == IfxSdmmc_Status_success)
            {
                /* If F8=0; standard capacity v1 card */
                if (sd->flags.f8 == 0U)
                {
                    sd->cardCapacity |= (uint8)IfxSdmmc_SdCardCapacity_standardV1x;
                }
                /* If F8=1; standard capacity v2 or high capacity card */
                else
                {
                    /* Check CCS bit in the OCR register; CCS=1 implies a high capacity card */
                    if ((sd->cardInfo.ocr & 0x40000000) != 0U)
                    {
                        sd->cardCapacity |= (uint8)((uint8)IfxSdmmc_SdCardCapacity_high |
                                                    (uint8)IfxSdmmc_SdCardCapacity_blockAddressing);
                    }
                    else
                    {
                        /* CCS=0 implies standard capacity v2 */
                        sd->cardCapacity |= (uint8)IfxSdmmc_SdCardCapacity_standardV2;
                    }
                }
            }
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_writeBlock(IfxSdmmc_Sd *sd, uint32 address, uint32 *data)
{
    IfxSdmmc_Status status  = IfxSdmmc_Status_success;
    uint32          timeout = 0;

    /* If the card is not initialized */
    if ((sd->cardState & (uint8)IfxSdmmc_CardState_notInitialised) != 0U)
    {
        status = IfxSdmmc_Status_cardNotInitialised;
    }
    else if ((sd->cardState & (uint8)IfxSdmmc_CardState_locked) != 0U)
    {
        status = IfxSdmmc_Status_cardLocked;
    }
    /* If the card is read only or write protected */
    else if ((sd->cardState & (uint8)IfxSdmmc_CardState_writeProtected) != 0U)
    {
        status = IfxSdmmc_Status_cardWrProtected;
    }
    else
    {
        /* FIXME: check Sector bound if needed */

        /* If not block addressing, then multiply by 512 bytes */
        if (((uint32)sd->cardCapacity & (uint32)IfxSdmmc_SdCardCapacity_blockAddressing) == 0U)
        {
            address = (uint32)(address * 512U);
        }

        if (sd->dmaUsed == TRUE)
        {
            if (sd->dmaType == IfxSdmmc_DmaType_adma2)
            {
                status = IfxSdmmc_Sd_singleBlockAdma2Transfer(sd, IfxSdmmc_Command_writeBlock, address, IFXSDMMC_BLOCK_SIZE_DEFAULT, data, IfxSdmmc_TransferDirection_write);
            }
            else
            {
                status = IfxSdmmc_Sd_singleBlockDmaTransfer(sd, IfxSdmmc_Command_writeBlock, address, IFXSDMMC_BLOCK_SIZE_DEFAULT, data, IfxSdmmc_TransferDirection_write);
            }
        }
        else
        {
            /* Single block transfer function */
            status = IfxSdmmc_Sd_singleBlockTransfer(sd, IfxSdmmc_Command_writeBlock, address, IFXSDMMC_BLOCK_SIZE_DEFAULT, data, IfxSdmmc_TransferDirection_write);
        }

        if ((status == IfxSdmmc_Status_success) || (status == IfxSdmmc_Status_bufferReady))
        {
            status = IfxSdmmc_Status_success;
        }
        else
        {
            status = IfxSdmmc_Status_failure;
        }
    }

    /* Wait for until the command OR data lines aren't busy */
    /* check if command and data lines are free */
    timeout = 1000000; // FIXME: get the recommeneded timeout value

    while ((IfxSdmmc_isDataLineBusy(sd->sdmmcSFR) || (IfxSdmmc_isCommandLineBusy(sd->sdmmcSFR))) && (timeout > 0))
    {
        timeout--;
    }

    if (timeout == 0)
    {
        status = IfxSdmmc_Status_timeout;
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioValidateOperatingCondition(IfxSdmmc_Sd *sd)
{
    IfxSdmmc_Status          status   = IfxSdmmc_Status_success;
    IfxSdmmc_Response        response;
    uint32                   argument = 0;
    IfxSdmmc_SdIoResponseR4 *ioInfo   = &(sd->cardInfo.io.ioInfo);

    /* Send CMD5 with argument 0 to query the card's voltage window */
    status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_ioSendOpCond, argument, IfxSdmmc_ResponseType_r4, &response);

    /* read card info here */
    ioInfo->r4 = (response.resp01);

    if (status == IfxSdmmc_Status_success)   /* only if success : check working of this */
    {
        if (ioInfo->bits.ioNum == 0)
        {
            status = IfxSdmmc_Status_failure; /* Number of functions supported = 0 */
        }
        else
        {
            /* set the WV */
            status = IfxSdmmc_Sd_ioSetVoltageWindow(sd);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioSetVoltageWindow(IfxSdmmc_Sd *sd)
{
    IfxSdmmc_Status         status = IfxSdmmc_Status_success;
    IfxSdmmc_Response       response;
    uint32                  argument;
    IfxSdmmc_SdIoResponseR4 ioInfo;
    uint32                  loopCount    = IFXSDMMC_SDIO_RETRYCOUNT_CMD5_1S;
    uint32                  retryLoopCtr = (uint32)0;

    /* send CMD5 with WV set (3.3V) */
    argument      = IFXSDMMC_ARG_CMD5_WV;

    ioInfo.bits.c = (uint8)IfxSdmmc_SdIoInit_notReady; /* Initialize to NOT READY first */

    while ((ioInfo.bits.c != (uint8)IfxSdmmc_SdIoInit_ready) && (loopCount > 0))
    {
        status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_ioSendOpCond, argument, IfxSdmmc_ResponseType_r4, &response);

        if ((status != IfxSdmmc_Status_success) && (status != IfxSdmmc_Status_commandError))
        {
            loopCount = 0; /* break out if command is not accepted */
        }
        else
        {
            ioInfo.r4 = response.resp01; /* read response in */

            /* This command to be sent every 3ms for 1 second */
            {
                retryLoopCtr = (IFXSDMMC_SDIO_RETRYDELAY_CMD5_US * 1000) / IFXSDMMC_1KLOOPDELAY_US;
                uint32 index;

                for (index = 0; index < retryLoopCtr; ++index)
                {
                    //do nothing
                    __nop();
                }
            }

            loopCount--;
        }
    }                                      /* loop is repeated 350 times every 3ms; or till IORDY */

    sd->cardInfo.io.ioInfo.r4 = ioInfo.r4; /* copy final response to sd card info */

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioReadRegister(IfxSdmmc_Sd *sd, IfxSdmmc_SdIoFunction func, uint32 addr, uint8 *reg)
{
    IfxSdmmc_Status         status = IfxSdmmc_Status_failure;
    IfxSdmmc_Response       response;
    IfxSdmmc_SdIoCmd52      argument;
    IfxSdmmc_SdIoResponseR5 r5resp;

    /* Populate the argument */
    argument.arg          = 0;      /* Init to 0 */

    argument.bits.func    = (uint32)func;
    argument.bits.regAddr = addr;
    argument.bits.rw      = 0;   /* read operation */

    /* check if function is enabled */
    if (IfxSdmmc_Sd_ioIsFunctionEnabled(sd, func) == IfxSdmmc_FunctionIO_enabled)
    {
        /* Send the command */
        status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_ioRwDirect, argument.arg, IfxSdmmc_ResponseType_r5, &response);

        if (status == IfxSdmmc_Status_success)
        {
            r5resp.r5 = (response.resp01);
            *reg      = (uint8)(r5resp.bits.data);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioWriteRegister(IfxSdmmc_Sd *sd, IfxSdmmc_SdIoFunction func, uint32 addr, uint8 reg)
{
    IfxSdmmc_Status    status = IfxSdmmc_Status_failure;
    IfxSdmmc_Response  response;
    IfxSdmmc_SdIoCmd52 argument;

    /* Populate the argument */
    argument.arg          = 0;      /* Init to 0 */

    argument.bits.func    = (uint32)func;
    argument.bits.regAddr = addr;
    argument.bits.rw      = 1;   /* write operation */
    argument.bits.data    = (uint32)reg;

    /* check if function is enabled */
    if (IfxSdmmc_Sd_ioIsFunctionEnabled(sd, func) == IfxSdmmc_FunctionIO_enabled)
    {
        /* Send the command */
        status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_ioRwDirect, argument.arg, IfxSdmmc_ResponseType_r5, &response);
    }

    /* RAW (read after write) is not implemented for now */

    return status;
}


boolean IfxSdmmc_Sd_ioIsMultiBlockSupported(IfxSdmmc_Sd *sd)
{
    IfxSdmmc_Status status       = IfxSdmmc_Status_failure;
    uint8           reg;
    boolean         blockSupport = FALSE;

    status = IfxSdmmc_Sd_ioReadRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_CARDCPBLTY, &reg);

    if (status == IfxSdmmc_Status_success)
    {
        if ((reg >> IFXSDMMC_SDIO_CCCR_CARDCPBLTY_SMB_BITPOS) & (uint8)1)
        {
            /* card supports the multiBlock feature */
            blockSupport = TRUE;
        }
    }

    return blockSupport;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioSetFuncBlockSize(IfxSdmmc_Sd *sd, IfxSdmmc_SdIoFunction func, uint16 blockSize)
{
    IfxSdmmc_Status status  = IfxSdmmc_Status_failure;
    uint32          timeout = (uint32)0;

    if (blockSize != IfxSdmmc_Sd_FuncBlockSize[func])
    {
        /* set the block size with block command */
        uint8              blockSizeArr[4];
        blockSizeArr[1] = (uint8)(blockSize >> (uint8)8);
        blockSizeArr[0] = (uint8)blockSize;
        uint32             address = IFXSDMMC_SDIO_FBR_ADDRESS_BLOCKSIZE(func);
        IfxSdmmc_SdIoCmd53 argument;

        /* Populate the argument */
        argument.arg            = 0;    /* Init to 0 */

        argument.bits.func      = (uint32)IfxSdmmc_SdIoFunction_0;
        argument.bits.regAddr   = address;
        argument.bits.rw        = 1;    /* write operation */
        argument.bits.blockMode = 0;    /* write bytes */
        argument.bits.Count     = 2;
        argument.bits.opCode    = 1;    /* write to incrementing address */

        /* transfer */
        status = IfxSdmmc_Sd_ioBlockTransfer(sd, argument.arg, 2, 1, (uint32 *)blockSizeArr, IfxSdmmc_TransferDirection_write);

        /* Wait for until the command OR data lines aren't busy */
        /* check if command and data lines are free */
        timeout = 1000000; // FIXME: get the recommeneded timeout value

        while ((IfxSdmmc_isDataLineBusy(sd->sdmmcSFR) || (IfxSdmmc_isCommandLineBusy(sd->sdmmcSFR))) && (timeout > 0))
        {
            timeout--;
        }

        if (timeout == 0)
        {
            status = IfxSdmmc_Status_timeout;
        }

        if (status == IfxSdmmc_Status_success)
        {
            IfxSdmmc_Sd_FuncBlockSize[func] = blockSize; /* update running block size now */
        }
    }
    else
    {
        status = IfxSdmmc_Status_success;
        /* do nothing - already set block size */
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioReadRegisterBlocks(IfxSdmmc_Sd *sd, IfxSdmmc_SdIoFunction func, uint32 startAddr, uint8 *regptr, uint16 blocksize, uint32 blockCount)
{
    IfxSdmmc_Status    status = IfxSdmmc_Status_failure;
    IfxSdmmc_SdIoCmd53 argument;
    uint32             timeout;

    /* If the card is not initialized */
    if ((sd->cardState & (uint8)IfxSdmmc_CardState_notInitialised) != 0U)
    {
        status = IfxSdmmc_Status_cardNotInitialised;
    }
    else if ((blockCount > 1) && (IfxSdmmc_Sd_ioIsMultiBlockSupported(sd) == FALSE))
    {
        status = IfxSdmmc_Status_failure;
    }
    else if (IfxSdmmc_Sd_ioIsFunctionEnabled(sd, func) == IfxSdmmc_FunctionIO_enabled)
    {
        /* Populate the argument */
        argument.arg          = 0;      /* Init to 0 */

        argument.bits.func    = (uint32)func;
        argument.bits.regAddr = startAddr;
        argument.bits.rw      = 0;      /* read operation */

        if (blockCount > 1)
        {
            argument.bits.blockMode = 1;    /* read blocks */
            argument.bits.Count     = blockCount;
        }
        else
        {
            argument.bits.blockMode = 0;    /* read bytes */
            argument.bits.Count     = blocksize;
        }

        argument.bits.opCode = 1;       /* read from incrementing address */

        if (blockCount > 1)
        {
            status = IfxSdmmc_Sd_ioSetFuncBlockSize(sd, func, (uint16)blocksize);
        }

        if ((status == IfxSdmmc_Status_success) || (blockCount == 1))
        {
            if (sd->dmaUsed == TRUE)
            {
                status = IfxSdmmc_Sd_ioBlockDmaTransfer(sd, argument.arg, blocksize, blockCount, (uint32 *)regptr, IfxSdmmc_TransferDirection_read);
            }
            else
            {
                status = IfxSdmmc_Sd_ioBlockTransfer(sd, argument.arg, blocksize, blockCount, (uint32 *)regptr, IfxSdmmc_TransferDirection_read);
            }
        }

        /* Wait for until the command OR data lines aren't busy */
        /* check if command and data lines are free */
        timeout = 1000000; // FIXME: get the recommeneded timeout value

        while ((IfxSdmmc_isDataLineBusy(sd->sdmmcSFR) || (IfxSdmmc_isCommandLineBusy(sd->sdmmcSFR))) && (timeout > 0))
        {
            timeout--;
        }

        if (timeout == 0)
        {
            status = IfxSdmmc_Status_timeout;
        }
    }
    else
    {
        /* do nothing - return failure */
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioWriteRegisterBlocks(IfxSdmmc_Sd *sd, IfxSdmmc_SdIoFunction func, uint32 startAddr, uint8 *regptr, uint16 blocksize, uint32 blockCount)
{
    IfxSdmmc_Status    status = IfxSdmmc_Status_failure;
    IfxSdmmc_SdIoCmd53 argument;
    uint32             timeout;

    /* If the card is not initialized */
    if ((sd->cardState & (uint8)IfxSdmmc_CardState_notInitialised) != 0U)
    {
        status = IfxSdmmc_Status_cardNotInitialised;
    }
    else if ((blockCount > 1) && (IfxSdmmc_Sd_ioIsMultiBlockSupported(sd) == FALSE))
    {
        status = IfxSdmmc_Status_failure;
    }
    else if (IfxSdmmc_Sd_ioIsFunctionEnabled(sd, func) == IfxSdmmc_FunctionIO_enabled)
    {
        /* Populate the argument */
        argument.arg          = 0;      /* Init to 0 */

        argument.bits.func    = (uint32)func;
        argument.bits.regAddr = startAddr;
        argument.bits.rw      = 1;      /* write operation */

        if (blockCount > 1)
        {
            argument.bits.blockMode = 1;    /* write blocks */
            argument.bits.Count     = blockCount;
        }
        else
        {
            argument.bits.blockMode = 0;    /* write bytes */
            argument.bits.Count     = blocksize;
        }

        argument.bits.opCode = 1;       /* write to incrementing address */

        if (blockCount > 1)
        {
            status = IfxSdmmc_Sd_ioSetFuncBlockSize(sd, func, (uint16)blocksize);
        }

        if ((status == IfxSdmmc_Status_success) || (blockCount == 1))
        {
            if (sd->dmaUsed == TRUE)
            {
                if (sd->dmaType == IfxSdmmc_DmaType_adma2)
                {
                    status = IfxSdmmc_Sd_ioBlockAdma2Transfer(sd, argument.arg, blocksize, blockCount, (uint32 *)regptr, IfxSdmmc_TransferDirection_write);
                }
                else
                {
                    status = IfxSdmmc_Sd_ioBlockDmaTransfer(sd, argument.arg, blocksize, blockCount, (uint32 *)regptr, IfxSdmmc_TransferDirection_write);
                }
            }
            else
            {
                status = IfxSdmmc_Sd_ioBlockTransfer(sd, argument.arg, blocksize, blockCount, (uint32 *)regptr, IfxSdmmc_TransferDirection_write);
            }
        }

        /* Wait for until the command OR data lines aren't busy */
        /* check if command and data lines are free */
        timeout = 1000000; // FIXME: get the recommeneded timeout value

        while ((IfxSdmmc_isDataLineBusy(sd->sdmmcSFR) || (IfxSdmmc_isCommandLineBusy(sd->sdmmcSFR))) && (timeout > 0))
        {
            timeout--;
        }

        if (timeout == 0)
        {
            status = IfxSdmmc_Status_timeout;
        }
    }
    else
    {
        /* do nothing - return failure */
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioBlockTransfer(IfxSdmmc_Sd *sd, uint32 argument, uint16 blockSize, uint32 blockCount, uint32 *data, IfxSdmmc_TransferDirection direction)
{
    IfxSdmmc_Status   status    = IfxSdmmc_Status_failure;
    IfxSdmmc_Response response;
    uint32            loopIndex = 0;
    uint32            timeout   = 0;
    uint32            numWords  = (uint32)(((blockSize) + 3) >> 2);
    uint32            numBlocks = blockCount;

    IfxSdmmc_setBlockSize(sd->sdmmcSFR, blockSize);

    /* FIXME: set block count if needed  for host control ver 4*/

    if (blockCount > 1)
    {
        IfxSdmmc_setMultiBlockSelect(sd->sdmmcSFR, TRUE);
        IfxSdmmc_setBlockCount(sd->sdmmcSFR, blockCount);
        IfxSdmmc_enableBlockCount(sd->sdmmcSFR);
    }
    else
    {
        IfxSdmmc_setMultiBlockSelect(sd->sdmmcSFR, FALSE);
        IfxSdmmc_disableBlockCount(sd->sdmmcSFR);
    }

    /* set Transfer direction */
    IfxSdmmc_setTransferDirection(sd->sdmmcSFR, direction);

    status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_ioRwExtended, argument, IfxSdmmc_ResponseType_r5, &response);

    while (numBlocks > 0)   /* iterate over all the blocks */
    {
        if (status == IfxSdmmc_Status_success)
        {
            timeout = 1000000; /* FIXME: get the recommeneded timeout value*/

            if (direction == IfxSdmmc_TransferDirection_read)
            {
                /* wait until Read ready flag is set */
                while ((IfxSdmmc_isNormalInterruptOccured(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_bufferReadReady) == 0) && (timeout > 0))
                {
                    timeout--;
                }

                if (timeout == 0)
                {
                    status = IfxSdmmc_Status_dataError;
                    /* FIME: implement abort command process */
                }
                else
                {
                    IfxSdmmc_clearNormalInterrupt(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_bufferReadReady);
                }
            }
            else if ((direction == IfxSdmmc_TransferDirection_write) && (numBlocks < blockCount))
            {
                /* wait until Read ready flag is set */
                while ((IfxSdmmc_isNormalInterruptOccured(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_bufferWriteReady) == 0) && (timeout > 0))
                {
                    timeout--;
                }

                if (timeout == 0)
                {
                    status = IfxSdmmc_Status_dataError;
                    /* FIME: implement abort command process */
                }
                else
                {
                    IfxSdmmc_clearNormalInterrupt(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_bufferWriteReady);
                }
            }
        }

        if (status == IfxSdmmc_Status_success)
        {
            /* Perform data transfer */
            /* 4 bytes data transmission in each iteration */
            for (loopIndex = 0U; loopIndex < numWords; loopIndex++)
            {
                if (direction == IfxSdmmc_TransferDirection_read)
                {
                    *data = (uint32)IfxSdmmc_readBufferData(sd->sdmmcSFR);
                }

                if (direction == IfxSdmmc_TransferDirection_write)
                {
                    IfxSdmmc_writeBufferData(sd->sdmmcSFR, data);
                }

                data++;
            }

            numBlocks--;    /* we have one less block to send / receive now */
        }
    }

    if (status == IfxSdmmc_Status_success)
    {
        timeout = 1000000; /* FIXME: get the recommended timeout value*/

        /* wait until transfer complete flag is set */
        while ((IfxSdmmc_isNormalInterruptOccured(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete) == 0) && (timeout > 0))
        {
            timeout--;
        }

        if (timeout == 0)
        {
            status = IfxSdmmc_Status_dataError;
            /* FIME: implement abort command process */
        }
        else
        {
            IfxSdmmc_clearNormalInterrupt(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioBlockAdma2Transfer(IfxSdmmc_Sd *sd, uint32 argument, uint16 blockSize, uint32 blockCount, uint32 *descrAddress, IfxSdmmc_TransferDirection direction)
{
    IfxSdmmc_Status   status  = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            timeout = 0;

    /* set the system address */
    IfxSdmmc_setSystemAddressForDma(sd->sdmmcSFR, (uint32)descrAddress);

    IfxSdmmc_setBlockSize(sd->sdmmcSFR, blockSize);

    if (blockCount > 1)
    {
        IfxSdmmc_setMultiBlockSelect(sd->sdmmcSFR, TRUE);
        IfxSdmmc_setBlockCount(sd->sdmmcSFR, blockCount);
        IfxSdmmc_disableBlockCount(sd->sdmmcSFR);   /* To be disabled when ADMA is used :: UM */
    }
    else
    {
        IfxSdmmc_setMultiBlockSelect(sd->sdmmcSFR, FALSE);
        IfxSdmmc_disableBlockCount(sd->sdmmcSFR);
    }

    IfxSdmmc_enableDmaTransfers(sd->sdmmcSFR);

    /* set transfer direction in host controller */
    if (direction == IfxSdmmc_TransferDirection_read)
    {
        IfxSdmmc_setTransferDirection(sd->sdmmcSFR, IfxSdmmc_TransferDirection_read);
    }
    else
    {
        IfxSdmmc_setTransferDirection(sd->sdmmcSFR, IfxSdmmc_TransferDirection_write);
    }

    /* FIXME : disable auto command if needed, by default 0 */
    status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_ioRwExtended, argument, IfxSdmmc_ResponseType_r5, &response);

    if (status == IfxSdmmc_Status_success)
    {
        timeout = 100000; /* FIXME: get the recommeneded timeout value*/

        /* wait until transfer complete or adma error flags are set */
        while ((IfxSdmmc_isNormalInterruptOccured(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete) == 0) \
               && (IfxSdmmc_isErrorInterruptOccured(sd->sdmmcSFR, IfxSdmmc_ErrorInterrupt_adma) == 0)            \
               && (timeout > 0))
        {
            timeout--;
        }

        if ((timeout == 0) || (IfxSdmmc_isErrorInterruptOccured(sd->sdmmcSFR, IfxSdmmc_ErrorInterrupt_adma) != 0))
        {
            status = IfxSdmmc_Status_dataError;

            if (IfxSdmmc_isErrorInterruptOccured(sd->sdmmcSFR, IfxSdmmc_ErrorInterrupt_adma) != 0)
            {
                IfxSdmmc_clearErrorInterrupt(sd->sdmmcSFR, IfxSdmmc_ErrorInterrupt_adma);
            }

            /* FIME: implement abort command process */
        }
        else
        {
            IfxSdmmc_clearNormalInterrupt(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioBlockDmaTransfer(IfxSdmmc_Sd *sd, uint32 argument, uint16 blockSize, uint32 blockCount, uint32 *data, IfxSdmmc_TransferDirection direction)
{
    IfxSdmmc_Status   status  = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            timeout = 0;

    /* set the system address */
    IfxSdmmc_setSystemAddressForDma(sd->sdmmcSFR, (uint32)data);

    IfxSdmmc_setBlockSize(sd->sdmmcSFR, blockSize);

    if (blockCount > 1)
    {
        IfxSdmmc_setMultiBlockSelect(sd->sdmmcSFR, TRUE);
        IfxSdmmc_setBlockCount(sd->sdmmcSFR, blockCount);
        IfxSdmmc_enableBlockCount(sd->sdmmcSFR);
    }
    else
    {
        IfxSdmmc_setMultiBlockSelect(sd->sdmmcSFR, FALSE);
        IfxSdmmc_disableBlockCount(sd->sdmmcSFR);
    }

    IfxSdmmc_enableDmaTransfers(sd->sdmmcSFR);

    /* set transfer direction in host controller */
    if (direction == IfxSdmmc_TransferDirection_read)
    {
        IfxSdmmc_setTransferDirection(sd->sdmmcSFR, IfxSdmmc_TransferDirection_read);
    }
    else
    {
        IfxSdmmc_setTransferDirection(sd->sdmmcSFR, IfxSdmmc_TransferDirection_write);
    }

    /* FIXME : disable auto command if needed, by default 0 */

    status = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_ioRwExtended, argument, IfxSdmmc_ResponseType_r5, &response);

    if (status == IfxSdmmc_Status_success)
    {
        /* Perform data transfer */
        timeout = 1000000; /* FIXME: get the recommeneded timeout value*/

        /* wait until transfer complete flag is set */
        while ((IfxSdmmc_isNormalInterruptOccured(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete) == 0) && (timeout > 0))
        {
            timeout--;
        }

        if (timeout == 0)
        {
            status = IfxSdmmc_Status_dataError;
            /* FIME: implement abort command process */
        }
        else
        {
            IfxSdmmc_clearNormalInterrupt(sd->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioSwitchToTransferState(IfxSdmmc_Sd *sd)
{
    IfxSdmmc_Status   status   = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            argument = 0;

    argument |= (uint32)(sd->cardInfo.rca << 16);

    status    = IfxSdmmc_sendCommand(sd->sdmmcSFR, IfxSdmmc_Command_selectDeselectCard, argument, IfxSdmmc_ResponseType_r1b, &response);

    return status;
}


IfxSdmmc_FunctionIO IfxSdmmc_Sd_ioIsFunctionEnabled(IfxSdmmc_Sd *sd, IfxSdmmc_SdIoFunction func)
{
    IfxSdmmc_FunctionIO ioe = IfxSdmmc_FunctionIO_unknown;
    uint8               reg;
    IfxSdmmc_Status     status;

    if (func == IfxSdmmc_SdIoFunction_0)
    {
        ioe = IfxSdmmc_FunctionIO_enabled;  // Function 0 is always enabled
    }
    else
    {
        status = IfxSdmmc_Sd_ioReadRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_IOEX, &reg);

        if (status == IfxSdmmc_Status_success)
        {
            ioe = (IfxSdmmc_FunctionIO)((reg >> (uint8)func) & (uint8)0x1);
        }
    }

    return ioe;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioEnableFunction(IfxSdmmc_Sd *sd, IfxSdmmc_SdIoFunction func)
{
    IfxSdmmc_Status status = IfxSdmmc_Status_failure;
    uint8           ioeReg = (uint8)IfxSdmmc_FunctionIO_enabled << (uint8)func;

    if (func == IfxSdmmc_SdIoFunction_0)
    {
        status = IfxSdmmc_Status_commandError; // Error - Function 0 is always enabled.
    }
    else
    {
        /* read the IOE register first */
        status = IfxSdmmc_Sd_ioReadRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_IOEX, &ioeReg);

        if (status == IfxSdmmc_Status_success)
        {
            ioeReg |= (uint8)IfxSdmmc_FunctionIO_enabled << (uint8)func;

            /* write back to IOE*/
            status = IfxSdmmc_Sd_ioWriteRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_IOEX, ioeReg);
        }
    }

    return status;
}


IfxSdmmc_SdIoInterruptStatus IfxSdmmc_Sd_ioGetInterruptEnableStatus(IfxSdmmc_Sd *sd, IfxSdmmc_SdIoFunction func)
{
    uint8                        ienReg;
    IfxSdmmc_SdIoInterruptStatus intStatus = IfxSdmmc_SdIoInterruptStatus_unknown;

    if (func != IfxSdmmc_SdIoFunction_0)
    {
        if (IfxSdmmc_Sd_ioReadRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_INTENABLE, &ienReg) == IfxSdmmc_Status_success)
        {
            intStatus = (IfxSdmmc_SdIoInterruptStatus)((ienReg >> (uint8)func) & 0x1);
        }
    }

    return intStatus;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioEnableFuncInterrupt(IfxSdmmc_Sd *sd, IfxSdmmc_SdIoFunction func)
{
    uint8           ienReg;
    IfxSdmmc_Status status = IfxSdmmc_Status_failure;

    if (func != IfxSdmmc_SdIoFunction_0)
    {
        /* read register first */
        status = IfxSdmmc_Sd_ioReadRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_INTENABLE, &ienReg);

        if (status == IfxSdmmc_Status_success)
        {
            /* manipulate ienReg to enable interrupt */
            ienReg |= (uint8)IfxSdmmc_SdIoInterruptStatus_enabled << (uint8)func;
            /* write back */
            status  = IfxSdmmc_Sd_ioWriteRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_INTENABLE, ienReg);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioDisableFuncInterrupt(IfxSdmmc_Sd *sd, IfxSdmmc_SdIoFunction func)
{
    uint8           ienReg;
    IfxSdmmc_Status status = IfxSdmmc_Status_failure;

    if (func != IfxSdmmc_SdIoFunction_0)
    {
        /* read register first */
        status = IfxSdmmc_Sd_ioReadRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_INTENABLE, &ienReg);

        if (status == IfxSdmmc_Status_success)
        {
            /* manipulate ienReg to disable interrupt */
            ienReg &= ~((uint8)(~IfxSdmmc_SdIoInterruptStatus_disabled) << (uint8)func);
            /* write back */
            status  = IfxSdmmc_Sd_ioWriteRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_INTENABLE, ienReg);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioSetMasterInterruptEnable(IfxSdmmc_Sd *sd, IfxSdmmc_SdIoInterruptStatus irqEnable)
{
    uint8           ienReg;
    IfxSdmmc_Status status = IfxSdmmc_Status_failure;

    /* read register first */
    status = IfxSdmmc_Sd_ioReadRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_INTENABLE, &ienReg);

    if (status == IfxSdmmc_Status_success)
    {
        /* manipulate ienReg to for Master interrupt */
        ienReg &= (uint8)0xFE;      /* clear the IENM bit */
        ienReg |= (uint8)irqEnable; /* now set the bit with requested val */

        /* write back */
        status = IfxSdmmc_Sd_ioWriteRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_INTENABLE, ienReg);
    }

    return status;
}


IfxSdmmc_SdIoInterruptPendingStatus IfxSdmmc_Sd_ioGetInterruptPendingStatus(IfxSdmmc_Sd *sd, IfxSdmmc_SdIoFunction func)
{
    uint8                               intReg;
    IfxSdmmc_SdIoInterruptPendingStatus intStatus = IfxSdmmc_SdIoInterruptPendingStatus_unknown;

    if (func != IfxSdmmc_SdIoFunction_0)
    {
        if (IfxSdmmc_Sd_ioReadRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_INTPENDING, &intReg) == IfxSdmmc_Status_success)
        {
            intStatus = (IfxSdmmc_SdIoInterruptPendingStatus)((intReg >> (uint8)func) & 0x1);
        }
    }

    return intStatus;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioClearPendingInterrupt(IfxSdmmc_Sd *sd, IfxSdmmc_SdIoFunction func)
{
    uint8           intReg;
    IfxSdmmc_Status status = IfxSdmmc_Status_failure;

    if (func != IfxSdmmc_SdIoFunction_0)
    {
        /* read register first */
        status = IfxSdmmc_Sd_ioReadRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_INTPENDING, &intReg);

        if (status == IfxSdmmc_Status_success)
        {
            /* manipulate ienReg to disable interrupt */
            intReg &= ~((uint8)(~IfxSdmmc_SdIoInterruptPendingStatus_cleared) << (uint8)func);
            /* write back */
            status  = IfxSdmmc_Sd_ioWriteRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_INTPENDING, intReg);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Sd_ioEnableMultiBlockInterrupt(IfxSdmmc_Sd *sd)
{
    uint8           cardCpblty;
    IfxSdmmc_Status status = IfxSdmmc_Status_failure;

    /* read register first */
    status = IfxSdmmc_Sd_ioReadRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_CARDCPBLTY, &cardCpblty);

    if (status == IfxSdmmc_Status_success)
    {
        if (cardCpblty >> IFXSDMMC_SDIO_CCCR_CARDCPBLTY_S4MI_BITPOS)
        {
            /* multiBlock Interrupt is supported. Enable now. */
            cardCpblty |= (uint8)1 << IFXSDMMC_SDIO_CCCR_CARDCPBLTY_E4MI_BITPOS;

            /* write back */
            status = IfxSdmmc_Sd_ioWriteRegister(sd, IfxSdmmc_SdIoFunction_0, IFXSDMMC_SDIO_CCCR_ADDRESS_CARDCPBLTY, cardCpblty);
        }
        else
        {
            status = IfxSdmmc_Status_failure;
        }
    }

    return status;
}
