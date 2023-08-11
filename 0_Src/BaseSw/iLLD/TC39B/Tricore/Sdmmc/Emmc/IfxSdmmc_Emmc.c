/**
 * \file IfxSdmmc_Emmc.c
 * \brief SDMMC EMMC details
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

#include "IfxSdmmc_Emmc.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

IfxSdmmc_Status IfxSdmmc_Emmc_initCard(IfxSdmmc_Emmc *emmc, IfxSdmmc_Emmc_CardConfig *cardConfig)
{
    IfxSdmmc_Status   status = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;

    /* set up the SD card interface, power up the card */
    /* power on bus */
    IfxSdmmc_setBusPowerOn(emmc->sdmmcSFR);

    /* FIXME: configure HOST_CTRL2.USH2_IF_ENABLE = 0; UHS_MODE_SEL = 0; default values are 0 anyway */
    /* FIXME: change clock frequency to 400 KHz or less */

    /* Enable the SD clock */
    IfxSdmmc_enableCardClock(emmc->sdmmcSFR);

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

    /* Initilaise and identify the eMMC card */

    /* Reset the card (CMD0) */
    status = IfxSdmmc_sendCommand(emmc->sdmmcSFR, IfxSdmmc_Command_goIdleState, IFXSDMMC_ARG_NONE, IfxSdmmc_ResponseType_none, NULL_PTR);

    if (status == IfxSdmmc_Status_success)  // Blessen
    {
        /* validate Access Mode (MMC_CMD1) */
        status = IfxSdmmc_Emmc_validateAccessMode(emmc);
    }

    if (status == IfxSdmmc_Status_success)
    {
        /* Read CID, CMD2 */
        status = IfxSdmmc_Emmc_readCid(emmc);

        if (status == IfxSdmmc_Status_success)
        {
            /* Set RCA, CMD3 */
            status = IfxSdmmc_Emmc_setRca(emmc, 0x2);

            if (status == IfxSdmmc_Status_success)
            {
                /* Read CSD, CMD9 */
                status = IfxSdmmc_Emmc_readCsd(emmc);
            }
        }
    }

    /* switch speed and bus width */
    if (status == IfxSdmmc_Status_success)
    {
        /* Switch to transfer state (CMD7) */
        status = IfxSdmmc_Emmc_switchToTransferState(emmc);

        if (status == IfxSdmmc_Status_success)
        {
            /* switch Bus Width if selected */
            if (cardConfig->dataWidth > IfxSdmmc_EmmcDataTransferWidth_1Bit)
            {
                status = IfxSdmmc_Emmc_switchBusWidth(emmc, cardConfig->dataWidth);
            }

            /* Switch Speed if selected */
            if (cardConfig->speedMode == IfxSdmmc_EmmcSpeedMode_highSpeedSdr)
            {
                status = IfxSdmmc_Emmc_switchToHighSpeed(emmc);

                if (status == IfxSdmmc_Status_success)
                {
                    /* Configure clock paramenters to 50 Mhz*/
                    IfxSdmmc_configureClock(emmc->sdmmcSFR, 50000000);
                }
            }
            else
            {
                /* Configure clock paramenters to 26 Mhz*/
                IfxSdmmc_configureClock(emmc->sdmmcSFR, emmc->userFrequency);
            }
        }
    }

    if (status == IfxSdmmc_Status_success)
    {
        /* Explicitly set block size as 512 */
        status = IfxSdmmc_sendCommand(emmc->sdmmcSFR, IfxSdmmc_Command_setBlockLength, 512, IfxSdmmc_ResponseType_r1, &response);
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Emmc_initHostController(IfxSdmmc_Emmc *emmc, IfxSdmmc_Emmc_HostConfig *hostConfig)
{
    IfxSdmmc_Status status = IfxSdmmc_Status_success;

    /* set the bus voltage */
    IfxSdmmc_setBusVoltage(emmc->sdmmcSFR);

    /* set the data line timeout value */
    IfxSdmmc_setDataLineTimeoutValue(emmc->sdmmcSFR, hostConfig->timeoutValue);

    /* set card type as eMMC */
    IfxSdmmc_setCardTypeEmmc(emmc->sdmmcSFR);

    /* Configure clock paramenters */
    /* set up clock for 400KHz now; we will change later */
    IfxSdmmc_configureClock(emmc->sdmmcSFR, IFXSDMMC_INIT_FREQUENCY);
    emmc->userFrequency = hostConfig->frequency;

    /* enable internal clock */
    status = IfxSdmmc_setUpInternalClock(emmc->sdmmcSFR);

    /* enable Host version 4 */
    IfxSdmmc_enableHostControllerVersion4(emmc->sdmmcSFR);

    return status;
}


IfxSdmmc_Status IfxSdmmc_Emmc_initModule(IfxSdmmc_Emmc *emmc, IfxSdmmc_Emmc_Config *config)
{
    IfxSdmmc_Status status   = IfxSdmmc_Status_success;

    Ifx_SDMMC      *sdmmcSFR = config->sdmmcSFR;
    emmc->sdmmcSFR = sdmmcSFR;
    emmc->dmaUsed  = config->useDma;
    emmc->dmaType  = config->dmaConfig.dmaType;

    /* enable module */
    IfxSdmmc_enableModule(config->sdmmcSFR);

    /* FIXME: apply kernal reset or apply software reset */
    //IfxSdmmc_resetmodule(config->sdmmcSFR);
    IfxSdmmc_applySoftwareReset(config->sdmmcSFR);

    /* initialise the host controller for the card type */
    IfxSdmmc_Emmc_initHostController(emmc, &config->hostConfig);

    /* set DMA type if DMA is used for transfers */
    if (config->useDma)
    {
        IfxSdmmc_setDmaType(emmc->sdmmcSFR, config->dmaConfig.dmaType);
    }

    /* enable the necessary normal inteerupt statsu flags*/
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
        IfxSdmmc_Emmc_setupPins(emmc, config->pins);
    }

    /* set up, initialise and identify the card  */
    status = IfxSdmmc_Emmc_initCard(emmc, &config->cardConfig);

    return status;
}


void IfxSdmmc_Emmc_initModuleConfig(IfxSdmmc_Emmc_Config *config, Ifx_SDMMC *sdmmcSFR)
{
    const IfxSdmmc_Emmc_Config defaultConfig = {
        .sdmmcSFR   = NULL_PTR,
        .hostConfig = {
            .timeoutValue    = IfxSdmmc_DataLineTimeout_2Power27,
            .usePresetValues = TRUE,
            .frequency       = 26000000
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
            .dataWidth = IfxSdmmc_EmmcDataTransferWidth_1Bit,
            .speedMode = IfxSdmmc_EmmcSpeedMode_legacy
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


IfxSdmmc_Status IfxSdmmc_Emmc_readBlock(IfxSdmmc_Emmc *emmc, uint32 address, uint32 *data)
{
    IfxSdmmc_Status status  = IfxSdmmc_Status_success;
    uint32          timeout = 0;

    /* If the card is not initialized */
    if ((emmc->cardState & (uint8)IfxSdmmc_CardState_notInitialised) != 0U)
    {
        status = IfxSdmmc_Status_cardNotInitialised;
    }
    else
    {
        /* FIXME: check Sector bound if needed */

        /* If byte Addressing, then multiply by 512 bytes */
        if ((uint32)emmc->cardCapacity == (uint32)IfxSdmmc_EmmcCardCapacity_byteAddressing)
        {
            address = (uint32)(address * 512U);
        }

        if (emmc->dmaUsed == TRUE)
        {
            status = IfxSdmmc_Emmc_singleBlockDmaTransfer(emmc, IfxSdmmc_Command_readSingleBlock, address, IFXSDMMC_BLOCK_SIZE_DEFAULT, data, IfxSdmmc_TransferDirection_read);
        }
        else
        {
            /* Single block transfer function */
            status = IfxSdmmc_Emmc_singleBlockTransfer(emmc, IfxSdmmc_Command_readSingleBlock, address, IFXSDMMC_BLOCK_SIZE_DEFAULT, data, IfxSdmmc_TransferDirection_read);
        }
    }

    /* Wait for until the command OR data lines aren't busy */
    /* check if command and data lines are free */
    timeout = 1000000; // FIXME: get the recommeneded timeout value

    while ((IfxSdmmc_isDataLineBusy(emmc->sdmmcSFR) || (IfxSdmmc_isCommandLineBusy(emmc->sdmmcSFR))) && (timeout > 0))
    {
        timeout--;
    }

    if (timeout == 0)
    {
        status = IfxSdmmc_Status_timeout;
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Emmc_readCid(IfxSdmmc_Emmc *emmc)
{
    IfxSdmmc_Status   status = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;

    status                = IfxSdmmc_sendCommand(emmc->sdmmcSFR, IfxSdmmc_Command_allSendCID, IFXSDMMC_ARG_NONE, IfxSdmmc_ResponseType_r2, &response);
    emmc->cardInfo.cid[0] = response.resp01;
    emmc->cardInfo.cid[1] = response.resp23;
    emmc->cardInfo.cid[2] = response.resp45;
    emmc->cardInfo.cid[3] = response.resp67;

    return status;
}


IfxSdmmc_Status IfxSdmmc_Emmc_readCsd(IfxSdmmc_Emmc *emmc)
{
    IfxSdmmc_Status   status   = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            argument = 0;
    argument              = (uint32)(emmc->cardInfo.rca << 16);
    status                = IfxSdmmc_sendCommand(emmc->sdmmcSFR, IfxSdmmc_Command_sendCSD, argument, IfxSdmmc_ResponseType_r2, &response);
    emmc->cardInfo.csd[0] = response.resp01;
    emmc->cardInfo.csd[1] = response.resp23;
    emmc->cardInfo.csd[2] = response.resp45;
    emmc->cardInfo.csd[3] = response.resp67;
    return status;
}


IfxSdmmc_Status IfxSdmmc_Emmc_setRca(IfxSdmmc_Emmc *emmc, uint16 rca)
{
    IfxSdmmc_Status   status   = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            argument = (IFXSDMMC_ARG_NONE | (uint32)(rca << 16));
    status = IfxSdmmc_sendCommand(emmc->sdmmcSFR, IfxSdmmc_Command_mmcSetRelativeAddress, argument, IfxSdmmc_ResponseType_r1, &response);

    if (status == IfxSdmmc_Status_success)
    {
        emmc->cardInfo.rca = rca;
    }
    else
    {
        emmc->cardInfo.rca = IFXSDMMC_RCA_NONE;
    }

    return status;
}


void IfxSdmmc_Emmc_setupPins(IfxSdmmc_Emmc *emmc, IfxSdmmc_Emmc_Pins *pins)
{
    IfxPort_InputMode    inMode     = pins->inputMode;
    IfxPort_PadDriver    speedGrade = pins->pinDriver;

    IfxSdmmc_Clk_Out    *clk        = pins->clk;
    IfxSdmmc_Cmd_InOut  *cmd        = pins->cmd;
    IfxSdmmc_Dat0_InOut *dat0       = pins->dat0;
    IfxSdmmc_Dat1_InOut *dat1       = pins->dat1;
    IfxSdmmc_Dat2_InOut *dat2       = pins->dat2;
    IfxSdmmc_Dat3_InOut *dat3       = pins->dat3;
    IfxSdmmc_Dat4_InOut *dat4       = pins->dat4;
    IfxSdmmc_Dat5_InOut *dat5       = pins->dat5;
    IfxSdmmc_Dat6_InOut *dat6       = pins->dat6;
    IfxSdmmc_Dat7_InOut *dat7       = pins->dat7;

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

    if (dat4 != NULL_PTR)
    {
        IfxPort_setPinModeInput(dat4->pin.port, dat4->pin.pinIndex, inMode);
        IfxPort_setPinPadDriver(dat4->pin.port, dat4->pin.pinIndex, speedGrade);
    }

    if (dat5 != NULL_PTR)
    {
        IfxPort_setPinModeInput(dat5->pin.port, dat5->pin.pinIndex, inMode);
        IfxPort_setPinPadDriver(dat5->pin.port, dat5->pin.pinIndex, speedGrade);
    }

    if (dat6 != NULL_PTR)
    {
        IfxPort_setPinModeInput(dat6->pin.port, dat6->pin.pinIndex, inMode);
        IfxPort_setPinPadDriver(dat6->pin.port, dat6->pin.pinIndex, speedGrade);
    }

    if (dat7 != NULL_PTR)
    {
        IfxPort_setPinModeInput(dat7->pin.port, dat7->pin.pinIndex, inMode);
        IfxPort_setPinPadDriver(dat7->pin.port, dat7->pin.pinIndex, speedGrade);
    }
}


IfxSdmmc_Status IfxSdmmc_Emmc_singleBlockAdma2Transfer(IfxSdmmc_Emmc *emmc, IfxSdmmc_Command command, uint32 address, uint16 blockSize, uint32 *descrAddress, IfxSdmmc_TransferDirection direction)
{
    IfxSdmmc_Status   status  = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            timeout = 0;

    /* set the system address */
    IfxSdmmc_setSystemAddressForDma(emmc->sdmmcSFR, (uint32)descrAddress);

    IfxSdmmc_setBlockSize(emmc->sdmmcSFR, blockSize);

    /* FIXME: set block count if needed  for host control ver 4*/

    IfxSdmmc_setMultiBlockSelect(emmc->sdmmcSFR, FALSE);

    IfxSdmmc_disableBlockCount(emmc->sdmmcSFR);

    IfxSdmmc_enableDmaTransfers(emmc->sdmmcSFR);

    /* set transfer direction in host controller */
    if (direction == IfxSdmmc_TransferDirection_read)
    {
        IfxSdmmc_setTransferDirection(emmc->sdmmcSFR, IfxSdmmc_TransferDirection_read);
    }
    else
    {
        IfxSdmmc_setTransferDirection(emmc->sdmmcSFR, IfxSdmmc_TransferDirection_write);
    }

    /* FIXME : disable auto command if needed, by default 0 */

    status = IfxSdmmc_sendCommand(emmc->sdmmcSFR, command, address, IfxSdmmc_ResponseType_r1, &response);

    if (status == IfxSdmmc_Status_success)
    {
        timeout = 100000; /* FIXME: get the recommeneded timeout value*/

        /* wait until transfer complete or adma error flags are set */
        while ((IfxSdmmc_isNormalInterruptOccured(emmc->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete) == 0) \
               && (IfxSdmmc_isErrorInterruptOccured(emmc->sdmmcSFR, IfxSdmmc_ErrorInterrupt_adma) == 0)            \
               && (timeout > 0))
        {
            timeout--;
        }

        if ((timeout == 0) || (IfxSdmmc_isErrorInterruptOccured(emmc->sdmmcSFR, IfxSdmmc_ErrorInterrupt_adma) != 0))
        {
            status = IfxSdmmc_Status_dataError;

            if (IfxSdmmc_isErrorInterruptOccured(emmc->sdmmcSFR, IfxSdmmc_ErrorInterrupt_adma) != 0)
            {
                IfxSdmmc_clearErrorInterrupt(emmc->sdmmcSFR, IfxSdmmc_ErrorInterrupt_adma);
            }

            /* FIME: implement abort command process */
        }
        else
        {
            IfxSdmmc_clearNormalInterrupt(emmc->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Emmc_singleBlockDmaTransfer(IfxSdmmc_Emmc *emmc, IfxSdmmc_Command command, uint32 address, uint16 blockSize, uint32 *data, IfxSdmmc_TransferDirection direction)
{
    IfxSdmmc_Status   status  = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            timeout = 0;

    /* set the system address */
    IfxSdmmc_setSystemAddressForDma(emmc->sdmmcSFR, (uint32)data);

    IfxSdmmc_setBlockSize(emmc->sdmmcSFR, blockSize);

    /* FIXME: set block count if needed  for host control ver 4*/

    IfxSdmmc_setMultiBlockSelect(emmc->sdmmcSFR, FALSE);

    IfxSdmmc_disableBlockCount(emmc->sdmmcSFR);

    IfxSdmmc_enableDmaTransfers(emmc->sdmmcSFR);

    /* set transfer direction in host controller */
    if (direction == IfxSdmmc_TransferDirection_read)
    {
        IfxSdmmc_setTransferDirection(emmc->sdmmcSFR, IfxSdmmc_TransferDirection_read);
    }
    else
    {
        IfxSdmmc_setTransferDirection(emmc->sdmmcSFR, IfxSdmmc_TransferDirection_write);
    }

    /* FIXME : disable auto command if needed, by default 0 */

    status = IfxSdmmc_sendCommand(emmc->sdmmcSFR, command, address, IfxSdmmc_ResponseType_r1, &response);

    if (status == IfxSdmmc_Status_success)
    {
#if 0
        uint32 loopIndex = 0;
        uint32 numWords  = (uint32)((blockSize + 3) >> 2);

        /* need to confirm with concept */
        /* Perform data transfer */
        /* 4 bytes data transmission in each iteration */
        for (loopIndex = 0U; loopIndex < numWords; loopIndex++)
        {
            IfxSdmmc_setSystemAddressForDma(emmc->sdmmcSFR, (uint32)data);

            /* wait until dma interrupt flag is set */
            timeout = 1000000; /* FIXME: get the recommeneded timeout value*/

            while ((IfxSdmmc_isNormalInterruptOccured(emmc->sdmmcSFR, IfxSdmmc_NormalInterrupt_dma) == 0) && (timeout > 0))
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
                IfxSdmmc_clearNormalInterrupt(emmc->sdmmcSFR, IfxSdmmc_NormalInterrupt_dma);
            }

            data++;
        }

#endif
        timeout = 1000000; /* FIXME: get the recommeneded timeout value*/

        /* wait until transfer complete flag is set */
        while ((IfxSdmmc_isNormalInterruptOccured(emmc->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete) == 0) && (timeout > 0))
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
            IfxSdmmc_clearNormalInterrupt(emmc->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Emmc_singleBlockTransfer(IfxSdmmc_Emmc *emmc, IfxSdmmc_Command command, uint32 address, uint16 blockSize, uint32 *data, IfxSdmmc_TransferDirection direction)
{
    IfxSdmmc_Status       status       = IfxSdmmc_Status_success;
    IfxSdmmc_Response     response;
    IfxSdmmc_ResponseType responseType = IfxSdmmc_ResponseType_r1;

    uint32                loopIndex    = 0;
    uint32                timeout      = 0;
    uint32                numWords     = (uint32)((blockSize + 3) >> 2);

    IfxSdmmc_setBlockSize(emmc->sdmmcSFR, blockSize);

    /* FIXME: set block count if needed  for host control ver 4*/

    IfxSdmmc_setMultiBlockSelect(emmc->sdmmcSFR, FALSE);

    IfxSdmmc_disableBlockCount(emmc->sdmmcSFR);

    /* set transfer direction in host controller */
    if (direction == IfxSdmmc_TransferDirection_read)
    {
        IfxSdmmc_setTransferDirection(emmc->sdmmcSFR, IfxSdmmc_TransferDirection_read);
    }
    else
    {
        IfxSdmmc_setTransferDirection(emmc->sdmmcSFR, IfxSdmmc_TransferDirection_write);
    }

    /* FIXME : disable auto command if needed, by default 0 */

    status = IfxSdmmc_sendCommand(emmc->sdmmcSFR, command, address, responseType, &response);

    if (status == IfxSdmmc_Status_success)
    {
        if (direction == IfxSdmmc_TransferDirection_read)
        {
            timeout = 1000000; /* FIXME: get the recommeneded timeout value*/

            /* wait until Read ready flag is set */
            while ((IfxSdmmc_isNormalInterruptOccured(emmc->sdmmcSFR, IfxSdmmc_NormalInterrupt_bufferReadReady) == 0) && (timeout > 0))
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
                IfxSdmmc_clearNormalInterrupt(emmc->sdmmcSFR, IfxSdmmc_NormalInterrupt_bufferReadReady);
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
                *data = (uint32)IfxSdmmc_readBufferData(emmc->sdmmcSFR);
            }

            if (direction == IfxSdmmc_TransferDirection_write)
            {
                IfxSdmmc_writeBufferData(emmc->sdmmcSFR, data);
            }

            data++;
        }

        timeout = 1000000; /* FIXME: get the recommeneded timeout value*/

        /* wait until transfer complete flag is set */
        while ((IfxSdmmc_isNormalInterruptOccured(emmc->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete) == 0) && (timeout > 0))
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
            IfxSdmmc_clearNormalInterrupt(emmc->sdmmcSFR, IfxSdmmc_NormalInterrupt_transferComplete);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Emmc_switchBusWidth(IfxSdmmc_Emmc *emmc, IfxSdmmc_EmmcDataTransferWidth busWidth)
{
    IfxSdmmc_Status   status   = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            argument = 0;

    if (busWidth == IfxSdmmc_EmmcDataTransferWidth_4Bit)
    {
        argument = IFXSDMMC_ARG_MMC_BUSWIDTH_4;
    }

    else if (busWidth == IfxSdmmc_EmmcDataTransferWidth_8Bit)
    {
        argument = IFXSDMMC_ARG_MMC_BUSWIDTH_8;
    }

    /* set the bus width on the host controller */
    IfxSdmmc_setEmmcDataTransferWidth(emmc->sdmmcSFR, busWidth);

    /* Switch card to desired buswidth (CMD6) */
    status = IfxSdmmc_sendCommand(emmc->sdmmcSFR, IfxSdmmc_Command_mmcSwitchFunction, argument, IfxSdmmc_ResponseType_r1b, &response);

    /* if switch failed, change the bus width back to 1 bit */
    if (status != IfxSdmmc_Status_success)
    {
        IfxSdmmc_setEmmcDataTransferWidth(emmc->sdmmcSFR, IfxSdmmc_EmmcDataTransferWidth_1Bit);
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Emmc_switchToHighSpeed(IfxSdmmc_Emmc *emmc)
{
    IfxSdmmc_Status   status = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;

    /* Switch card to high speed (CMD6) */
    status = IfxSdmmc_sendCommand(emmc->sdmmcSFR, IfxSdmmc_Command_mmcSwitchFunction, IFXSDMMC_ARG_MMC_HIGHSPEED, IfxSdmmc_ResponseType_r1b, &response);

    return status;
}


IfxSdmmc_Status IfxSdmmc_Emmc_switchToTransferState(IfxSdmmc_Emmc *emmc)
{
    IfxSdmmc_Status   status   = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            argument = 0;

    argument |= (uint32)(emmc->cardInfo.rca << 16);

    /* read card status CMD13 */
    status = IfxSdmmc_sendCommand(emmc->sdmmcSFR, IfxSdmmc_Command_sendStatus, argument, IfxSdmmc_ResponseType_r1, &response);

    if (status == IfxSdmmc_Status_success)
    {
        /* if the card is not in transfer state already Switch to transfer state (CMD7)*/
        if (response.cardStatus.B.currentState != 4)
        {
            status = IfxSdmmc_sendCommand(emmc->sdmmcSFR, IfxSdmmc_Command_selectDeselectCard, argument, IfxSdmmc_ResponseType_r1b, &response);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Emmc_validateAccessMode(IfxSdmmc_Emmc *emmc)
{
    IfxSdmmc_Status   status  = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            timeout = (uint32)0;

    timeout = (uint32)1000000; // FIXME: get the correct number

    /* validate access mode (MMC_CMD1) */
    do
    {
        status = IfxSdmmc_sendCommand(emmc->sdmmcSFR, IfxSdmmc_Command_mmcSendOpCond, IFXSDMMC_ARG_MMCCMD1, IfxSdmmc_ResponseType_r3, &response);
        timeout--;
    } while (((response.resp01 >> 31) & 0x1) != 1 && (timeout > (uint32)0));

    if (status == IfxSdmmc_Status_success)
    {
        /* Check if pattern matches in both the argument and response */
        if (response.resp01 == 0x80FF8080)
        {
            emmc->cardCapacity = (uint8)IfxSdmmc_EmmcCardCapacity_byteAddressing;
        }
        else if (response.resp01 == 0xC0FF8080)
        {
            emmc->cardCapacity = (uint8)IfxSdmmc_EmmcCardCapacity_sectorAddressing;
        }
        else /*((response.resp01 != 0x80FF8080) && (response.resp01 != 0xC0FF8080)) */
        {
            status = IfxSdmmc_Status_badResponse; /* device i s not compliant */
            IfxSdmmc_setBusPowerOff(emmc->sdmmcSFR);
            /* Update card initialization status */
            emmc->cardState &= (uint8)~((uint8)IfxSdmmc_CardState_notInitialised | (uint8)IfxSdmmc_CardState_noCard);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Emmc_writeBlock(IfxSdmmc_Emmc *emmc, uint32 address, uint32 *data)
{
    IfxSdmmc_Status status  = IfxSdmmc_Status_success;
    uint32          timeout = 0;

    /* If the card is not initialized */
    if ((emmc->cardState & (uint8)IfxSdmmc_CardState_notInitialised) != 0U)
    {
        status = IfxSdmmc_Status_cardNotInitialised;
    }
    else
    {
        /* FIXME: check Sector bound if needed */

        /* If byte addressing, then multiply by 512 bytes */
        if ((uint32)emmc->cardCapacity == (uint32)IfxSdmmc_EmmcCardCapacity_byteAddressing)
        {
            address = (uint32)(address * 512U);
        }

        if (emmc->dmaUsed == TRUE)
        {
            if (emmc->dmaType == IfxSdmmc_DmaType_adma2)
            {
                status = IfxSdmmc_Emmc_singleBlockAdma2Transfer(emmc, IfxSdmmc_Command_writeBlock, address, IFXSDMMC_BLOCK_SIZE_DEFAULT, data, IfxSdmmc_TransferDirection_write);
            }
            else
            {
                status = IfxSdmmc_Emmc_singleBlockDmaTransfer(emmc, IfxSdmmc_Command_writeBlock, address, IFXSDMMC_BLOCK_SIZE_DEFAULT, data, IfxSdmmc_TransferDirection_write);
            }
        }
        else
        {
            /* Single block transfer function */
            status = IfxSdmmc_Emmc_singleBlockTransfer(emmc, IfxSdmmc_Command_writeBlock, address, IFXSDMMC_BLOCK_SIZE_DEFAULT, data, IfxSdmmc_TransferDirection_write);
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

    while ((IfxSdmmc_isDataLineBusy(emmc->sdmmcSFR) || (IfxSdmmc_isCommandLineBusy(emmc->sdmmcSFR))) && (timeout > 0))
    {
        timeout--;
    }

    if (timeout == 0)
    {
        status = IfxSdmmc_Status_timeout;
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_Emmc_eraseBlocks(IfxSdmmc_Emmc *emmc, uint32 startAddress, uint32 endAddress)
{
    IfxSdmmc_Status   status   = IfxSdmmc_Status_success;
    uint32            argument = (uint32)0;
    IfxSdmmc_Response response;

    /* If the card is not initialized */
    if ((emmc->cardState & (uint8)IfxSdmmc_CardState_notInitialised) != 0U)
    {
        status = IfxSdmmc_Status_cardNotInitialised;
    }
    else
    {
        /* FIXME: check Sector bound if needed */

        /* If byte addressing, then multiply by 512 bytes */
        if ((uint32)emmc->cardCapacity == (uint32)IfxSdmmc_EmmcCardCapacity_byteAddressing)
        {
            startAddress = (uint32)(startAddress * IFXSDMMC_BLOCK_SIZE_DEFAULT);
            endAddress   = (uint32)(endAddress * IFXSDMMC_BLOCK_SIZE_DEFAULT);
        }

        /* Set the Start Address */
        argument = startAddress;
        status   = IfxSdmmc_sendCommand(emmc->sdmmcSFR, IfxSdmmc_Command_mmcEraseGroupStart, argument, IfxSdmmc_ResponseType_r1, &response);

        if (status == IfxSdmmc_Status_success)
        {
            /* set the end Address */
            argument = endAddress;
            status   = IfxSdmmc_sendCommand(emmc->sdmmcSFR, IfxSdmmc_Command_mmcEraseGroupEnd, argument, IfxSdmmc_ResponseType_r1, &response);

            if (status == IfxSdmmc_Status_success)
            {
                /* send the TRIM command to erase */
                argument = IFXSDMMC_ARG_CMD38_TRIM;
                status   = IfxSdmmc_sendCommand(emmc->sdmmcSFR, IfxSdmmc_Command_erase, argument, IfxSdmmc_ResponseType_r1b, &response);
            }
        }
    }

    return status;
}
