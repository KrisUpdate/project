/**
 * \file IfxSdmmc.h
 * \brief SDMMC  basic functionality
 * \ingroup IfxLld_Sdmmc
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
 * \defgroup IfxLld_Sdmmc_Std_Enumerations Enumerations
 * \ingroup IfxLld_Sdmmc_Std
 * \defgroup IfxLld_Sdmmc_Std_DataStrctures Data Strctures
 * \ingroup IfxLld_Sdmmc_Std
 * \defgroup IfxLld_Sdmmc_Std_Functions Functions
 * \ingroup IfxLld_Sdmmc_Std
 */

#ifndef IFXSDMMC_H
#define IFXSDMMC_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxSdmmc_cfg.h"
#include "IfxSdmmc_reg.h"
#include "IfxScu_reg.h"
#include "Src/Std/IfxSrc.h"
#include "Scu/Std/IfxScuWdt.h"
#include "_Utilities/Ifx_Assert.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \brief CMD0, Response: none\n
 * Resets all cards to idle state.
 */
#define IFXSDMMC_CMD0_GO_IDLE_STATE                (0x00)

/** \brief CMD2, Response: R2\n
 * Asks any card to send the CID numbers on the CMD line (any card that is connected to the host will respond)
 */
#define IFXSDMMC_CMD2_ALL_SEND_CID                 (0x02)

/** \brief CMD3, Response: R6\n
 * Ask the card to publish a new relative address (RCA)
 */
#define IFXSDMMC_CMD3_SEND_RELATIVE_ADDR           (0x03)

/** \brief CMD4, Response: none\n
 * Programs the DSR of all cards
 */
#define IFXSDMMC_CMD4_SET_DSR                      (0x04)

/** \brief MMC CMD6
 */
#define IFXSDMMC_CMD6_SWITCH_FUNC                  (0x06)

/** \brief CMD7, Response: R1b (only from the slected card)\n
 * Command toggles a card between the stand-by and transfer states or\n
 * between the programming and disconnect states. In both cases, the card is selected by \n
 * its own relative address and gets deselected by any other address; address 0 deselects all\n
 * In the case that the RCA equals 0, then the host may do one of the following:\n
 * - Use other RCA number to perform card de-selection.\n
 * - Re-send CMD3 to change its RCA number to other than 0 and then use CMD7 with RCA=0 for card deselection.\n
 */
#define IFXSDMMC_CMD7_SELECT_DESELECT_CARD         (0x07)

/** \brief CMD8, Response: R8\n
 * Sends SD Memory Card interface condition, which includes host supply voltage information\n
 * and asks the card whether card supports voltage. Reserved bits shall be set to '0'.
 */
#define IFXSDMMC_CMD8_SEND_IF_COND                 (0x08)

/** \brief CMD9, Response: R2\n
 * Addressed card sends its card-specific data (CSD) on the CMD line.
 */
#define IFXSDMMC_CMD9_SEND_CSD                     (0x09)

/** \brief CMD10, Response: R2\n
 * Addressed card sends its card identification (CID) on CMD the line.
 */
#define IFXSDMMC_CMD10_SEND_CID                    (0x0A)

/** \brief CMD11, Response: R1\n
 * Switch to 1.8V bus signaling level.
 */
#define IFXSDMMC_CMD11_VOLTAGE_SWITCH              (0x0B)

/** \brief CMD12, Response: R1b\n
 * Forces the card to stop transmission
 */
#define IFXSDMMC_CMD12_STOP_TRANSMISSION           (0x0C)

/** \brief CMD13, Response: R1\n
 * Addressed card sends its status register.
 */
#define IFXSDMMC_CMD13_SEND_STATUS                 (0x0D)

/** \brief CMD15, Response: none\n
 * Sends an addressed card into the Inactive State. This command is used \n
 * when the host explicitly wants to deactivate a card. Reserved bits shall be set to '0'.
 */
#define IFXSDMMC_CMD15_GO_INACTIVE_STATE           (0x0F)

/** \brief CMD16, Response: R1\n
 * In the case of a Standard Capacity SD Memory Card, this command sets the block length (in bytes)\n
 * for all following block commands (read, write, lock). Default block length is fixed to 512 Bytes. \n
 * Set length is valid for memory access commands only if partial block read operation are allowed in CSD.\n
 * In the case of SDHC and SDXC Cards, block length set by CMD16 command does not affect \n
 * memory read and write commands. Always 512 Bytes fixed block length is used. \n
 * This command is effective for LOCK_UNLOCK command. In both cases, if block length is set larger than 512Bytes, \n
 * the card sets the BLOCK_LEN_ERROR bit. In DDR50 mode, data is sampled on both edges of the clock.\n
 * Therefore, block length shall always be even.\n
 */
#define IFXSDMMC_CMD16_SET_BLOCKLEN                (0x10)

/** \brief CMD17, Response: R1\n
 * In the case of a Standard Capacity SD Memory Card, this command, this command reads a block \n
 * of the size selected by the SET_BLOCKLEN command. \n
 * In case of SDHC and SDXC Cards, block length is fixed 512 Bytes regardless of the SET_BLOCKLEN command.
 */
#define IFXSDMMC_CMD17_READ_SINGLE_BLOCK           (0x11)

/** \brief CMD18, Response: R1\n
 * Continuously transfers data blocks from card to host until interrupted by a STOP_TRANSMISSION command.\n
 * Block length is specified the same as READ_SINGLE_BLOCK command.
 */
#define IFXSDMMC_CMD18_READ_MULTIPLE_BLOCK         (0x12)

/** \brief CMD19, Response: R1\n
 * 64 bytes tuning pattern is sent for SDR50 and SDR104.
 */
#define IFXSDMMC_CMD19_SEND_TUNING_BLOCK           (0x13)

/** \brief CMD20, Response: R1b\n
 * Speed Class control command.
 */
#define IFXSDMMC_CMD20_SPEED_CLASS_CONTROL         (0x14)

/** \brief CMD23, Response: R1\n
 * Specify block count for CMD18 and CMD25.
 */
#define IFXSDMMC_CMD23_SET_BLOCK_COUNT             (0x17)

/** \brief CMD24, Response: R1\n
 * In case of SDSC Card, block length is set by the SET_BLOCKLEN command\n
 * In case of SDHC and SDXC Cards, block length is fixed 512 Bytes regardless of the SET_BLOCKLEN command.
 */
#define IFXSDMMC_CMD24_WRITE_BLOCK                 (0x18)

/** \brief CMD25, Response: R1\n
 * Continuously writes blocks of data until a STOP_TRANSMISSION follows.\n
 * Block length is specified the same as WRITE_BLOCK command.
 */
#define IFXSDMMC_CMD25_WRITE_MULTIPLE_BLOCK        (0x19)

/** \brief CMD27, Response: R1\n
 * Programming of the programmable bits of the CSD.
 */
#define IFXSDMMC_CMD27_PROGRAM_CSD                 (0x1B)

/** \brief CMD28, Response: R1b\n
 * If the card has write protection features, this command sets the write protection bit of the addressed group.\n
 * The properties of write protection are coded in the card specific data (WP_GRP_SIZE)\n
 * SDHC and SDXC Cards do not support this command.
 */
#define IFXSDMMC_CMD28_SET_WRITE_PROT              (0x1C)

/** \brief CMD29, Response: R1b\n
 * If the card provides write protection features, this command clears the write protection bit of the addressed group.\n
 * SDHC and SDXC Cards do not support this command.
 */
#define IFXSDMMC_CMD29_CLR_WRITE_PROT              (0x1D)

/** \brief CMD30, Response: R1\n
 * If the card provides write protection features, this command asks the card to send the status of the write protection bits.1\n
 * SDHC and SDXC Cards do not support this command.
 */
#define IFXSDMMC_CMD30_SEND_WRITE_PROT             (0x1E)

/** \brief CMD32, Response: R1\n
 * Sets the address of the first write block to be erased.
 */
#define IFXSDMMC_CMD32_ERASE_WR_BLK_START          (0x20)

/** \brief CMD33, Response: R1\n
 * Sets the address of the last write block of the continuous range to be erased.
 */
#define IFXSDMMC_CMD33_ERASE_WR_BLK_END            (0x21)

/** \brief CMD38, Response: R1b\n
 * Erases all previously selected write blocks.
 */
#define IFXSDMMC_CMD38_ERASE                       (0x26)

/** \brief CMD42, Response: R1\n
 * Used to set/reset the password or lock/unlock the card. The size of the data block is set by the SET_BLOCK_LEN command.\n
 * Reserved bits in the argument and in Lock Card Data Structure shall be set to 0.
 */
#define IFXSDMMC_CMD42_LOCK_UNLOCK                 (0x2A)

/** \brief CMD55, Response: R1\n
 * Indicates to the card that the next command is an application specific command rather than a standard command
 */
#define IFXSDMMC_CMD55_APP_CMD                     (0x37)

/** \brief CMD56, Response: R1\n
 * Used either to transfer a data block to the card or to get a data block from the card for general purpose/application specific commands. \n
 * In case of a SDSC Card, block length is set by the SET_BLOCK_LEN command. \n
 * In case of SDHC and SDXC Cards, block length is fixed to 512 bytes.\n
 * The host sets RD/WR=1 for reading data from thecard and sets to 0 for writing data to the card.\n
 */
#define IFXSDMMC_CMD56_GEN_CMD                     (0x38)

/** \brief ACMD6, should be followed by CMD55, Response: R1\n
 * Defines the data bus width ('00'=1bit or '10'=4 bits bus) to be used for data transfer. \n
 * The allowed data bus widths are given in SCR register.
 */
#define IFXSDMMC_ACMD6_SET_BUS_WIDTH               (0x06)

/** \brief ACMD13, should be followed by CMD55, Response: R1\n
 * Send the SD Status.
 */
#define IFXSDMMC_ACMD13_SD_STATUS                  (0x0D)

/** \brief ACMD22, should be followed by CMD55, Response: R1\n
 * Send the number of the written (without errors) write blocks. Responds with 32bit+CRC data block.\n
 * If WRITE_BL_PARTIAL='0', the unit of ACMD22 is always 512 byte.\n
 * If WRITE_BL_PARTIAL='1', the unit of ACMD22 is a block length which was used when the write command was executed.
 */
#define IFXSDMMC_ACMD22_SEND_NUM_WR_BLOCKS         (0x16)

/** \brief ACMD23, should be followed by CMD55, Response: R1\n
 * Set the number of write blocks to be pre-erased before writing (to be used for faster Multiple Block WR command). \n
 * "1"=default (one wr block)
 */
#define IFXSDMMC_ACMD23_SET_WR_BLK_ERASE_COUNT     (0x17)

/** \brief ACMD41, should be followed by CMD55, Response: R1\n
 * Sends host capacity support information (HCS) and asks the accessed card to send its operating condition register (OCR) content \n
 * in the response on the CMD line. HCS is effective when card receives SEND_IF_COND command.\n
 * Sends request to switch to 1.8V signaling (S18R). Reserved bit shall be set to '0'. CCS bit is assigned to OCR[30].\n
 * XPC controls the maximum power in the default speed mode of SDXC card. XPC=0 means 0.36W (100mA at 3.6V on VDD1) (max.) \n
 * but speed class is not supported. XPC=1 means 0.54W (150mA at 3.6V on VDD1) (max.) and speed class is supported.
 */
#define IFXSDMMC_ACMD41_SD_SEND_OP_COND            (0x29)

/** \brief ACMD42, should be followed by CMD55, Response: R1\n
 * Connect[1]/Disconnect[0] the 50 KOhm pull-up resistor on CD/DAT3 (pin 1) of the card.
 */
#define IFXSDMMC_ACMD42_SET_CLR_CARD_DETECT        (0x1A)

/** \brief ACMD51, should be followed by CMD55, Response: R1\n
 * Reads the SD Configuration Register (SCR).
 */
#define IFXSDMMC_ACMD51_SEND_SCR                   (0x33)

/** \brief MMC CMD1,
 */
#define IFXSDMMC_CMD1_MMC_SEND_OP_COND             (0x01)

/** \brief MMC CMD3,
 */
#define IFXSDMMC_CMD3_MMC_SET_RELATIVE_ADDR        (0x03)

/** \brief MMC CMD5,
 */
#define IFXSDMMC_CMD5_MMC_SLEEP_AWAKE              (0x05)

/** \brief MMC CMD6
 */
#define IFXSDMMC_CMD6_MMC_SWITCH_FUNC              (0x06)

/** \brief MMC CMD8
 */
#define IFXSDMMC_CMD8_MMC_SEND_EXT_CSD             (0x08)

/** \brief MMC CMD35
 */
#define IFXSDMMC_CMD35_MMC_ERASE_GROUP_START       (0x23)

/** \brief MMC CMD36
 */
#define IFXSDMMC_CMD36_MMC_ERASE_GROUP_END         (0x24)

/** \brief Argument none (Stuff bits)
 */
#define IFXSDMMC_ARG_NONE                          (0x00000000U)

/** \brief Argument for Command 8
 */
#define IFXSDMMC_ARG_CMD8                          (0x000001AAU)

/** \brief Arguments for ACMD41 F80 (HCS=0)
 */
#define IFXSDMMC_ARG_ACMD41_F80                    (0x00100000U)

/** \brief Argument for switching SD card to 4 bit mode
 */
#define IFXSDMMC_ARG_SD_BUSWIDTH_4                 (0x00000002U)

/** \brief Arguments for ACMD41 F81 (HCS=1)
 */
#define IFXSDMMC_ARG_ACMD41_F81                    (0x40100000U)

/** \brief Argument for switching SD card to high speed mode
 */
#define IFXSDMMC_ARG_SD_HIGHSPEED                  (0x80000001U)

/** \brief Argument for switching MMC card to 4 bit mode
 */
#define IFXSDMMC_ARG_MMC_BUSWIDTH_4                (0x03B70100U)

/** \brief Argument for switching MMC card to high speed mode
 */
#define IFXSDMMC_ARG_MMC_HIGHSPEED                 (0x03B90100U)

/** \brief Defalut transfer block size
 */
#define IFXSDMMC_BLOCK_SIZE_DEFAULT                (512)

/** \brief Argument for MMC CMD1
 */
#define IFXSDMMC_ARG_MMCCMD1                       (0x40FF8080U)

/** \brief Card relative address RCA none
 */
#define IFXSDMMC_RCA_NONE                          (0x00000000U)

/** \brief Voltage check: Host supported voltage 2.7 to 3.6V
 */
#define IFXSDMMC_HOST_SUPPORTED_VOLTAGE            (0x1)

/** \brief Interface Condition Command (CMD8) Check Pattern
 */
#define IFXSDMMC_IF_COND_CHECK_PATTERN             (0xAA)

/** \brief Mask for Card power up status bit (busy)
 */
#define IFXSDMMC_CARD_POWERUP_STATUS_MASK          (0x80000000)

/** \brief CMD register reserved bitfild value
 */
#define IFXSDMMC_CMD_REG_BIT_RES                   (0)

/** \brief Argument for switching MMC card to 8 bit mode
 */
#define IFXSDMMC_ARG_MMC_BUSWIDTH_8                (0x03B70200U)

/** \brief Argument for TRIM option in ERASE command.
 */
#define IFXSDMMC_ARG_CMD38_TRIM                    (0x00000001u)

/** \brief CMD5, Response: R5
 * Request Card operation condition
 */
#define IFXSDMMC_CMD5_IO_SEND_OP_COND              (0x05)

/** \brief SDIO CMD52
 * Read/Write Register Direct
 */
#define IFXSDMMC_CMD52_IO_RW_DIRECT                (0x34)

/** \brief SDIO CMD53
 * Read / Write Register Blocks
 */
#define IFXSDMMC_CMD53_IO_RW_EXTENDED              (0x35)

/** \brief SD Memory Card R1 status error check Mask
 */
#define IFXSDMMC_CARD_R1STATUS_ERROR_CHECK_MSK     (0xFD398008)

/** \brief SDIO card R1 status error check mask.
 */
#define IFXSDMMC_IOCARD_R1STATUS_ERROR_CHECK_MSK   (0x80C80000)

/** \brief SD memory card R6 status error check mask
 */
#define IFXSDMMC_CARD_R6STATUS_ERROR_CHECK_MSK     (0xE008)

/** \brief SDIO card R6 status error check mask
 */
#define IFXSDMMC_IOCARD_R6STATUS_ERROR_CHECK_MSK   (0xE000)

/** \brief SDIO card (IO only) error status check mask
 */
#define IFXSDMMC_CARD_IOONLY_STATUS_CHECK_MSK      (0x00001D00)

/** \brief Argument for CMD5 for SDIO : 3.2-3.3V OCR WV
 */
#define IFXSDMMC_ARG_CMD5_WV                       ((uint32)(0x00100000U))

/** \brief error check mask for CMD52, CMD53 for SDIO
 * value: 0b1100 1011
 */
#define IFXSDMMC_IOCARD_R5RESPONSE_ERROR_CHECK_MSK ((uint8)0xCB)

/** \brief CCCR: address of IO enable register
 */
#define IFXSDMMC_SDIO_CCCR_ADDRESS_IOEX            ((uint32)0x02)

/** \brief CCCR: address of Card capability register
 */
#define IFXSDMMC_SDIO_CCCR_ADDRESS_CARDCPBLTY      ((uint32)0x08)

/** \brief CCCR: Card Capability: LSC bit position
 */
#define IFXSDMMC_SDIO_CCCR_CARDCPBLTY_LSC_BITPOS   ((uint8)6)

/** \brief CCCR: Card Capability: 4BLS bit position
 */
#define IFXSDMMC_SDIO_CCCR_CARDCPBLTY_4BLS_BITPOS  ((uint8)7)

/** \brief CCCR: Card Capability: SMB bit position
 */
#define IFXSDMMC_SDIO_CCCR_CARDCPBLTY_SMB_BITPOS   ((uint8)1)

/** \brief CCCR: Card Capability: S4MI bit position
 */
#define IFXSDMMC_SDIO_CCCR_CARDCPBLTY_S4MI_BITPOS  ((uint8)4)

/** \brief CCCR: Card Capability: E4MI bit position
 */
#define IFXSDMMC_SDIO_CCCR_CARDCPBLTY_E4MI_BITPOS  ((uint8)5)

/** \brief CCCR: address of BUS IF CONTROL register
 */
#define IFXSDMMC_SDIO_CCCR_ADDRESS_BUSIFCTRL       ((uint32)0x7)

/** \brief CCCR: Address of Interrupt Enable Register
 */
#define IFXSDMMC_SDIO_CCCR_ADDRESS_INTENABLE       ((uint32)0x04)

/** \brief CCCR: Address of Interrupt Pending Register
 */
#define IFXSDMMC_SDIO_CCCR_ADDRESS_INTPENDING      ((uint32)0x05)

/** \brief CCCR: address of bus speed select register
 */
#define IFXSDMMC_SDIO_CCCR_ADDRESS_BUSSPEEDSEL     ((uint32)0x13)

/** \brief CCCR: Bus speed select: SHS bit position
 */
#define IFXSDMMC_SDIO_CCCR_BUSSPEEDSEL_SHS_BITPOS  ((uint32)0x00)

/** \brief Macro to calculate the BlockSize Register Address of function
 */
#define IFXSDMMC_SDIO_FBR_ADDRESS_BLOCKSIZE(func) ((((uint32)0x100) * func) + (uint32)0x10)

/** \brief CCCR: Bus speed select: EHS bit position
 */
#define IFXSDMMC_SDIO_CCCR_BUSSPEEDSEL_EHS_BITPOS  ((uint32)0x01)

/** \brief Initialization Frequency of SD card - SD spec max: 400KHz
 */
#ifndef IFXSDMMC_INIT_FREQUENCY
#define IFXSDMMC_INIT_FREQUENCY                    ((uint32)(400000))
#endif

/** \brief Physical Time duration (microseconds) for achieving 74 cycle clock delay
 */
#define IFXSDMMC_DELAY_74CYCLE_US                  ((uint32)((74 * 1000000) / IFXSDMMC_INIT_FREQUENCY))

/** \brief Number of CPU cycles for one loop of delay
 */
#define IFXSDMMC_1LOOP_CPUCYCLES                   ((uint32)(3))

/** \brief Time delay (microseconds) for 1000 loop counts
 */
#define IFXSDMMC_1KLOOPDELAY_US                    ((uint32)((IFXSDMMC_1LOOP_CPUCYCLES * (uint32)1000 * (uint32)1000000) / IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex())))

/** \brief Delay between two tries of CMD5 for SDIO card
 */
#define IFXSDMMC_SDIO_RETRYDELAY_CMD5_US           ((uint32)(3 * 1000))

/** \brief Number of retries of CMD5 (every 3ms for 1second)
 */
#define IFXSDMMC_SDIO_RETRYCOUNT_CMD5_1S           ((uint32)(350))

/** \brief Delay between retries for ACMD41
 */
#define IFXSDMMC_RETRYDELAY_ACMD41_US              ((uint32)(3 * 1000))

/** \brief Number or retries of ACMD41 for memory cards (every 3ms for 1s)
 */
#define IFXSDMMC_RETRYCOUNT_ACMD41_1S              ((uint32)(350))

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Sdmmc_Std_Enumerations
 * \{ */
/** \brief Combination of Act2, Act1 and Act0 of ADMA descriptors
 */
typedef enum
{
    IfxSdmmc_AdmaActionSymbol_nop  = 0,  /**< \brief No Operation, do not execute current line and go to next line */
    IfxSdmmc_AdmaActionSymbol_rsv  = 2,  /**< \brief Reserved, same as nop */
    IfxSdmmc_AdmaActionSymbol_tran = 4,  /**< \brief Transfer Data, transfer data of one descriptor line */
    IfxSdmmc_AdmaActionSymbol_link = 6   /**< \brief Link Descriptor, Link to another descriptor */
} IfxSdmmc_AdmaActionSymbol;

/** \brief Card lock status
 */
typedef enum
{
    IfxSdmmc_CardLockStatus_unlocked,  /**< \brief Card in Unlocked state */
    IfxSdmmc_CardLockStatus_locked     /**< \brief Locked */
} IfxSdmmc_CardLockStatus;

/** \brief The state of the card
 */
typedef enum
{
    IfxSdmmc_CardState_initialised    = 0,  /**< \brief initialised */
    IfxSdmmc_CardState_notInitialised = 1,  /**< \brief Not Initialised */
    IfxSdmmc_CardState_noCard         = 2,  /**< \brief No Card */
    IfxSdmmc_CardState_writeProtected = 4,  /**< \brief Write Protected */
    IfxSdmmc_CardState_locked         = 8,  /**< \brief Locked */
    IfxSdmmc_CardState_commandActive  = 16, /**< \brief Command Active */
    IfxSdmmc_CardState_dataActive     = 32, /**< \brief Data Active */
    IfxSdmmc_CardState_hcInitialised  = 64  /**< \brief HC Initialised */
} IfxSdmmc_CardState;

/** \brief Card Type
 */
typedef enum
{
    IfxSdmmc_CardType_sd,   /**< \brief SD card */
    IfxSdmmc_CardType_emmc  /**< \brief eMMC card */
} IfxSdmmc_CardType;

/** \brief Command
 */
typedef enum
{
    IfxSdmmc_Command_goIdleState,            /**< \brief Go Idle State, CMD0 */
    IfxSdmmc_Command_allSendCID,             /**< \brief All Send CID, CMD2 */
    IfxSdmmc_Command_sendRelativeAddress,    /**< \brief Send Relative Address, CMD3 */
    IfxSdmmc_Command_setDSR,                 /**< \brief Set DSR, CMD4 */
    IfxSdmmc_Command_switchFunction,         /**< \brief Switch Function, CMD6 */
    IfxSdmmc_Command_selectDeselectCard,     /**< \brief Select Deselect Card, CMD7 */
    IfxSdmmc_Command_sendIfCond,             /**< \brief Send If Cond, CMD8 */
    IfxSdmmc_Command_sendCSD,                /**< \brief Send CSD, CMD9 */
    IfxSdmmc_Command_sendCID,                /**< \brief Send CID, CMD10 */
    IfxSdmmc_Command_stopTransmission,       /**< \brief Stop Transmission, CMD12 */
    IfxSdmmc_Command_sendStatus,             /**< \brief Send Status, CMD13 */
    IfxSdmmc_Command_goInactiveState,        /**< \brief Go Inactive State, CMD15 */
    IfxSdmmc_Command_setBlockLength,         /**< \brief Set Block Length, CMD16 */
    IfxSdmmc_Command_readSingleBlock,        /**< \brief Read Single Block, CMD17 */
    IfxSdmmc_Command_readMultipleBLock,      /**< \brief Read Multiple Block, CMD18 */
    IfxSdmmc_Command_writeBlock,             /**< \brief Write Block, CMD24 */
    IfxSdmmc_Command_writeMultipleBlock,     /**< \brief Write Multiple Block, CMD25 */
    IfxSdmmc_Command_programCSD,             /**< \brief Program CSD, CMD27 */
    IfxSdmmc_Command_setWriteProtect,        /**< \brief Set Write Protect, CMD28 */
    IfxSdmmc_Command_clearWriteProtect,      /**< \brief Clear Write Protect, CMD29 */
    IfxSdmmc_Command_sendWriteProtect,       /**< \brief Send Write Protect, CMD30 */
    IfxSdmmc_Command_eraseWrBlkStart,        /**< \brief Erase Wr Blk Start, CMD32 */
    IfxSdmmc_Command_eraseWrBlkEnd,          /**< \brief Erase Wr Blk End, CMD33 */
    IfxSdmmc_Command_erase,                  /**< \brief Erase, CMD38 */
    IfxSdmmc_Command_lockUnlock,             /**< \brief Lock Unlock, CMD42 */
    IfxSdmmc_Command_appCommand,             /**< \brief App Command, CMD55 */
    IfxSdmmc_Command_genCommand,             /**< \brief Gen Command, CMD56 */
    IfxSdmmc_Command_setBusWidth,            /**< \brief Set Bus Width, ACMD6 */
    IfxSdmmc_Command_sdStatus,               /**< \brief SD Status, ACMD13 */
    IfxSdmmc_Command_sendNumWrBlocks,        /**< \brief Send Num Wr Blocks, CMDACMD22 */
    IfxSdmmc_Command_setWrBlkEraseCount,     /**< \brief Set Wr Blk Erase Count, ACMD23 */
    IfxSdmmc_Command_sdSendOpCond,           /**< \brief Sd Send Op Cond, ACMD41 */
    IfxSdmmc_Command_setClrCardDetect,       /**< \brief Set Clr Card Detect, ACMD42 */
    IfxSdmmc_Command_sendSCR,                /**< \brief Send SCR */
    IfxSdmmc_Command_mmcSendOpCond,          /**< \brief MMC Send Op Cond, MMC_CMD1 */
    IfxSdmmc_Command_mmcSetRelativeAddress,  /**< \brief MMC Set Relative Address, MMC_CMD3 */
    IfxSdmmc_Command_mmcSleepAwake,          /**< \brief MMC Sleep Awake, MMC_CMD5 */
    IfxSdmmc_Command_mmcSwitchFunction,      /**< \brief MMC Switch Funtion, MMC_CMD6 */
    IfxSdmmc_Command_mmcSendExtCSD,          /**< \brief MMC Send Ext CSD, MMC_CMD8 */
    IfxSdmmc_Command_mmcEraseGroupStart,     /**< \brief MMC Erase Group Start, MMC_CMD35 */
    IfxSdmmc_Command_mmcEraseGroupEnd,       /**< \brief MMC Erase Group End, MMC_CMD36 */
    IfxSdmmc_Command_ioSendOpCond,           /**< \brief IO request for operating condition CMD5 */
    IfxSdmmc_Command_ioRwDirect,             /**< \brief IO RW direct, CMD52 */
    IfxSdmmc_Command_ioRwExtended            /**< \brief IO RW extended, CMD53 */
} IfxSdmmc_Command;

/** \brief Command type\n
 * Definition in CMD.CMD_CRC_CHK_ENABLE
 */
typedef enum
{
    IfxSdmmc_CommandCrcCheck_disable,  /**< \brief Disable */
    IfxSdmmc_CommandCrcCheck_enable    /**< \brief Enable */
} IfxSdmmc_CommandCrcCheck;

/** \brief Command type\n
 * Definition in CMD.SUB_CMD_FLAG
 */
typedef enum
{
    IfxSdmmc_CommandFlag_main,  /**< \brief Main Command */
    IfxSdmmc_CommandFlag_sub    /**< \brief Sub Command */
} IfxSdmmc_CommandFlag;

/** \brief Command type\n
 * Definition in CMD.CMD_IDX_CHK_ENABLE
 */
typedef enum
{
    IfxSdmmc_CommandIndexCheck_disable,  /**< \brief Disable */
    IfxSdmmc_CommandIndexCheck_enable    /**< \brief Enable */
} IfxSdmmc_CommandIndexCheck;

/** \brief Command type\n
 * Definition in CMD.CMD_TYPE
 */
typedef enum
{
    IfxSdmmc_CommandType_normal,   /**< \brief Normal Command */
    IfxSdmmc_CommandType_suspend,  /**< \brief Suspend Command */
    IfxSdmmc_CommandType_resume,   /**< \brief Resume Command */
    IfxSdmmc_CommandType_abort     /**< \brief Abort Command */
} IfxSdmmc_CommandType;

/** \brief the interval by which DAT line timeouts are detected\n
 * Definition in TOUT_CTRL.TOUT_CNT
 */
typedef enum
{
    IfxSdmmc_DataLineTimeout_2Power13,     /**< \brief TMCLK * 2Power13  */
    IfxSdmmc_DataLineTimeout_2Power14,     /**< \brief TMCLK * 2Power14  */
    IfxSdmmc_DataLineTimeout_2Power15,     /**< \brief TMCLK * 2Power15  */
    IfxSdmmc_DataLineTimeout_2Power16,     /**< \brief TMCLK * 2Power16  */
    IfxSdmmc_DataLineTimeout_2Power17,     /**< \brief TMCLK * 2Power17  */
    IfxSdmmc_DataLineTimeout_2Power18,     /**< \brief TMCLK * 2Power18  */
    IfxSdmmc_DataLineTimeout_2Power19,     /**< \brief TMCLK * 2Power19  */
    IfxSdmmc_DataLineTimeout_2Power20,     /**< \brief TMCLK * 2Power20  */
    IfxSdmmc_DataLineTimeout_2Power21,     /**< \brief TMCLK * 2Power21  */
    IfxSdmmc_DataLineTimeout_2Power22,     /**< \brief TMCLK * 2Power22  */
    IfxSdmmc_DataLineTimeout_2Power23,     /**< \brief TMCLK * 2Power23  */
    IfxSdmmc_DataLineTimeout_2Power24,     /**< \brief TMCLK * 2Power24  */
    IfxSdmmc_DataLineTimeout_2Power25,     /**< \brief TMCLK * 2Power25  */
    IfxSdmmc_DataLineTimeout_2Power26,     /**< \brief TMCLK * 2Power26  */
    IfxSdmmc_DataLineTimeout_2Power27      /**< \brief TMCLK * 2Power27  */
} IfxSdmmc_DataLineTimeout;

/** \brief Command type\n
 * Definition in CMD.DATA_PRESENT_SEL
 */
typedef enum
{
    IfxSdmmc_DataPresentSelect_noData,   /**< \brief No Data Present */
    IfxSdmmc_DataPresentSelect_withData  /**< \brief Data Present */
} IfxSdmmc_DataPresentSelect;

/** \brief Dma Type,\n
 * Definition in HOST_CTRL1.DMA_SEL
 */
typedef enum
{
    IfxSdmmc_DmaType_sdma     = 0, /**< \brief SDMA */
    IfxSdmmc_DmaType_adma2    = 2, /**< \brief ADMA2 */
    IfxSdmmc_DmaType_adma2Or3 = 3  /**< \brief ADMA2 or ADMA3 */
} IfxSdmmc_DmaType;

/** \brief EMMC card capacity
 */
typedef enum
{
    IfxSdmmc_EmmcCardCapacity_byteAddressing   = 0, /**< \brief less than 2GB: Byte Addressing */
    IfxSdmmc_EmmcCardCapacity_sectorAddressing = 1  /**< \brief More than 2GB : Sector Addressing */
} IfxSdmmc_EmmcCardCapacity;

/** \brief Data width of the host controller, number of data lines.\n
 * Definition in HOST_CTRL1.DAT_XFER_WIDTH and HOST_CTRL1.EXT_DAT_XFER
 */
typedef enum
{
    IfxSdmmc_EmmcDataTransferWidth_1Bit,  /**< \brief 1 bit mode */
    IfxSdmmc_EmmcDataTransferWidth_4Bit,  /**< \brief 4 bit mode */
    IfxSdmmc_EmmcDataTransferWidth_8Bit   /**< \brief 8 bit mode */
} IfxSdmmc_EmmcDataTransferWidth;

/** \brief Speed Mode\n
 * Definition HOST_CTRL2.UHS_MODE
 */
typedef enum
{
    IfxSdmmc_EmmcSpeedMode_legacy,       /**< \brief eMMC Speed mode: Legacy */
    IfxSdmmc_EmmcSpeedMode_highSpeedSdr  /**< \brief eMMC Speed mode: High Speed SDR */
} IfxSdmmc_EmmcSpeedMode;

/** \brief Error Interrupts\n
 * Definition in ERROR_INT_STAT, ERROR_INT_STAT_EN, ERROR_INT_SIGNAL_EN
 */
typedef enum
{
    IfxSdmmc_ErrorInterrupt_commandTimeout,      /**< \brief Command Timeout Error */
    IfxSdmmc_ErrorInterrupt_commandCrc,          /**< \brief Command Crc Error */
    IfxSdmmc_ErrorInterrupt_commandEndBit,       /**< \brief Command End Bit Error */
    IfxSdmmc_ErrorInterrupt_commandIndex,        /**< \brief Command Index Error */
    IfxSdmmc_ErrorInterrupt_dataTimeout,         /**< \brief Data Timeout Error */
    IfxSdmmc_ErrorInterrupt_dataCrc,             /**< \brief Data Crc Error */
    IfxSdmmc_ErrorInterrupt_dataEndBit,          /**< \brief Data End Bit Error */
    IfxSdmmc_ErrorInterrupt_currentLimit,        /**< \brief Current Limit Error */
    IfxSdmmc_ErrorInterrupt_adma,                /**< \brief ADMA Error */
    IfxSdmmc_ErrorInterrupt_tuning,              /**< \brief Tuning Error */
    IfxSdmmc_ErrorInterrupt_response,            /**< \brief Response Error */
    IfxSdmmc_ErrorInterrupt_bootAcknowledgement  /**< \brief Boot Acknowledgement Error */
} IfxSdmmc_ErrorInterrupt;

/** \brief Function IO enable status
 */
typedef enum
{
    IfxSdmmc_FunctionIO_disabled,  /**< \brief IO disabled */
    IfxSdmmc_FunctionIO_enabled,   /**< \brief IO enabled */
    IfxSdmmc_FunctionIO_unknown    /**< \brief IO status unkown */
} IfxSdmmc_FunctionIO;

/** \brief Interrupt Type
 */
typedef enum
{
    IfxSdmmc_InterruptType_normal,  /**< \brief Normal Interrupt type */
    IfxSdmmc_InterruptType_dma      /**< \brief Error Interrupt type */
} IfxSdmmc_InterruptType;

/** \brief Normal Interrupts\n
 * Definition in NORMAL_INT_STAT, NORMAL_INT_STAT_EN, NORMAL_INT_SIGNAL_EN
 */
typedef enum
{
    IfxSdmmc_NormalInterrupt_commandComplete,      /**< \brief Command Complete */
    IfxSdmmc_NormalInterrupt_transferComplete,     /**< \brief Transfer Complete */
    IfxSdmmc_NormalInterrupt_blockGapEvent,        /**< \brief Block Gap Event */
    IfxSdmmc_NormalInterrupt_dma,                  /**< \brief DMA */
    IfxSdmmc_NormalInterrupt_bufferWriteReady,     /**< \brief Buffer Write Ready */
    IfxSdmmc_NormalInterrupt_bufferReadReady,      /**< \brief Buffer Read Ready */
    IfxSdmmc_NormalInterrupt_cardInsertion,        /**< \brief Card Insertion */
    IfxSdmmc_NormalInterrupt_cardRemoval,          /**< \brief Card Removal */
    IfxSdmmc_NormalInterrupt_card,                 /**< \brief Card Interrupt */
    IfxSdmmc_NormalInterrupt_a,                    /**< \brief INT_A */
    IfxSdmmc_NormalInterrupt_b,                    /**< \brief INT_B */
    IfxSdmmc_NormalInterrupt_c,                    /**< \brief INT_C */
    IfxSdmmc_NormalInterrupt_reTuningEvent,        /**< \brief Re Tuning Event */
    IfxSdmmc_NormalInterrupt_fXEvent,              /**< \brief FX Event */
    IfxSdmmc_NormalInterrupt_commandQueuingEvent,  /**< \brief Command Queuing Event */
    IfxSdmmc_NormalInterrupt_error                 /**< \brief Error */
} IfxSdmmc_NormalInterrupt;

/** \brief Response Length\n
 * Definition in CMD.RESP_TYPE_SELECT
 */
typedef enum
{
    IfxSdmmc_ResponseLength_none,            /**< \brief No Response */
    IfxSdmmc_ResponseLength_136,             /**< \brief Response Length 136 */
    IfxSdmmc_ResponseLength_48,              /**< \brief Response Length 48 */
    IfxSdmmc_ResponseLength_48WithBusyCheck  /**< \brief Response Length 48 (check Busy after response) */
} IfxSdmmc_ResponseLength;

/** \brief Response Type
 */
typedef enum
{
    IfxSdmmc_ResponseType_none,  /**< \brief No Response */
    IfxSdmmc_ResponseType_r1,    /**< \brief R1 */
    IfxSdmmc_ResponseType_r1b,   /**< \brief R1b */
    IfxSdmmc_ResponseType_r2,    /**< \brief R2 */
    IfxSdmmc_ResponseType_r3,    /**< \brief R3 */
    IfxSdmmc_ResponseType_r4,    /**< \brief R4 */
    IfxSdmmc_ResponseType_r5,    /**< \brief R5 */
    IfxSdmmc_ResponseType_r6,    /**< \brief R6 */
    IfxSdmmc_ResponseType_r7     /**< \brief R7 */
} IfxSdmmc_ResponseType;

/** \brief Card Capacity
 */
typedef enum
{
    IfxSdmmc_SdCardCapacity_standardV2      = 1, /**< \brief Capcity V2 */
    IfxSdmmc_SdCardCapacity_standardV1x     = 2, /**< \brief Capacity V1x */
    IfxSdmmc_SdCardCapacity_high            = 4, /**< \brief High Capacity */
    IfxSdmmc_SdCardCapacity_blockAddressing = 8  /**< \brief Block Addressing */
} IfxSdmmc_SdCardCapacity;

/** \brief enum holding card type
 */
typedef enum
{
    IfxSdmmc_SdCardType_io = 1,  /**< \brief IO only card */
    IfxSdmmc_SdCardType_mem,     /**< \brief Memory only card */
    IfxSdmmc_SdCardType_combo    /**< \brief Combo card */
} IfxSdmmc_SdCardType;

/** \brief Data width of the host controller, number of data lines.\n
 * Definition in HOST_CTRL1.DAT_XFER_WIDTH
 */
typedef enum
{
    IfxSdmmc_SdDataTransferWidth_1Bit,  /**< \brief 1 bit mode */
    IfxSdmmc_SdDataTransferWidth_4Bit   /**< \brief 4 bit mode */
} IfxSdmmc_SdDataTransferWidth;

/** \brief Enum holding the function numbers
 */
typedef enum
{
    IfxSdmmc_SdIoFunction_none = -1, /**< \brief Not Selected */
    IfxSdmmc_SdIoFunction_0,         /**< \brief "Function"+str(x)  */
    IfxSdmmc_SdIoFunction_1,         /**< \brief "Function"+str(x)  */
    IfxSdmmc_SdIoFunction_2,         /**< \brief "Function"+str(x)  */
    IfxSdmmc_SdIoFunction_3,         /**< \brief "Function"+str(x)  */
    IfxSdmmc_SdIoFunction_4,         /**< \brief "Function"+str(x)  */
    IfxSdmmc_SdIoFunction_5,         /**< \brief "Function"+str(x)  */
    IfxSdmmc_SdIoFunction_6,         /**< \brief "Function"+str(x)  */
    IfxSdmmc_SdIoFunction_7          /**< \brief "Function"+str(x)  */
} IfxSdmmc_SdIoFunction;

/** \brief Init Status of SDIO card
 */
typedef enum
{
    IfxSdmmc_SdIoInit_notReady,  /**< \brief Not Initialized */
    IfxSdmmc_SdIoInit_ready      /**< \brief Initialized */
} IfxSdmmc_SdIoInit;

/** \brief SDIO Function Interrupt Pending Status
 */
typedef enum
{
    IfxSdmmc_SdIoInterruptPendingStatus_cleared,  /**< \brief Interrupt cleared */
    IfxSdmmc_SdIoInterruptPendingStatus_pending,  /**< \brief Interrupt pending */
    IfxSdmmc_SdIoInterruptPendingStatus_unknown   /**< \brief Interrupt Pending Status unknown */
} IfxSdmmc_SdIoInterruptPendingStatus;

typedef enum
{
    IfxSdmmc_SdIoInterruptStatus_disabled,  /**< \brief Interrupt disabled */
    IfxSdmmc_SdIoInterruptStatus_enabled,   /**< \brief Interrupt enabled */
    IfxSdmmc_SdIoInterruptStatus_unknown    /**< \brief Interrupt status unkown */
} IfxSdmmc_SdIoInterruptStatus;

/** \brief SDIO bus width for transfer
 */
typedef enum
{
    IfxSdmmc_SdIoTransferWidth_1Bit,      /**< \brief 1 Bit Bus width */
    IfxSdmmc_SdIoTransferWidth_reserved,  /**< \brief reserved at Spec */
    IfxSdmmc_SdIoTransferWidth_4Bit,      /**< \brief 4 Bit bus width */
    IfxSdmmc_SdIoTransferWidth_8Bit       /**< \brief 8 Bit bus width */
} IfxSdmmc_SdIoTransferWidth;

/** \brief SD modes supported by HOST
 */
typedef enum
{
    IfxSdmmc_SdModes_ioOnly,   /**< \brief Support only SDIO mode */
    IfxSdmmc_SdModes_memOnly,  /**< \brief support only SD memory mode */
    IfxSdmmc_SdModes_combo     /**< \brief support Combo cards */
} IfxSdmmc_SdModes;

/** \brief Speed Mode\n
 * Definition HOST_CTRL1.HIGH_SPEED_EN
 */
typedef enum
{
    IfxSdmmc_SdSpeedMode_normal,  /**< \brief Normal Speed MOde */
    IfxSdmmc_SdSpeedMode_high     /**< \brief High Speed Mode */
} IfxSdmmc_SdSpeedMode;

/** \brief Status of the SDMMC hardware
 */
typedef enum
{
    IfxSdmmc_Status_success,               /**< \brief Success */
    IfxSdmmc_Status_failure,               /**< \brief Failure */
    IfxSdmmc_Status_commandLineBusy,       /**< \brief command Line Busy */
    IfxSdmmc_Status_dataLineBusy,          /**< \brief data Line Busy */
    IfxSdmmc_Status_commandError,          /**< \brief Command Error */
    IfxSdmmc_Status_responseError,         /**< \brief Response Error */
    IfxSdmmc_Status_timeout,               /**< \brief Timeout */
    IfxSdmmc_Status_badResponse,           /**< \brief Bad Response */
    IfxSdmmc_Status_initialisedButLocked,  /**< \brief Initialised but locked */
    IfxSdmmc_Status_cardNotInitialised,    /**< \brief Card not initialised */
    IfxSdmmc_Status_cardLocked,            /**< \brief Card Locked */
    IfxSdmmc_Status_cardWrProtected,       /**< \brief Card Write Protected */
    IfxSdmmc_Status_bufferReady,           /**< \brief Buffer Ready */
    IfxSdmmc_Status_dataError              /**< \brief Data Error */
} IfxSdmmc_Status;

/** \brief Transfer Direction\n
 * Definition in XFER_MODE.DATA_XFER_DIR
 */
typedef enum
{
    IfxSdmmc_TransferDirection_write,  /**< \brief Write, Host to Card */
    IfxSdmmc_TransferDirection_read    /**< \brief Read, Card to Host */
} IfxSdmmc_TransferDirection;

/** \} */

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Sdmmc_Std_DataStrctures
 * \{ */
/** \brief R4 response Bits: Card information
 */
typedef struct
{
    uint32 ocr : 24;
    uint32 s18a : 1;
    uint32 stuff : 2;
    uint32 mp : 1;
    uint32 ioNum : 3;
    uint32 c : 1;
} IfxSdmmc_SdIoResponseR4Bits;

/** \} */

/** \addtogroup IfxLld_Sdmmc_Std_DataStrctures
 * \{ */
/** \brief SCR card Register Bitfields\n
 * these bit fields are rearranged to match the SCR register bit fields of card when read by host\n
 * data comes from card to host with MSB(it) first, CPU reads the data with LSB(yte) first.
 */
typedef struct
{
    uint32 sdSpec : 4;           /**< \brief SD_SPEC, [59:56] */
    uint32 scrStructure : 4;     /**< \brief SCR Structure, [63:60] */
    uint32 sdBusWidth : 4;       /**< \brief DAT Bus widths supported, [51:48] */
    uint32 sdSecurity : 3;       /**< \brief CPRM Security Support, [54:52] */
    uint32 eraseStat : 1;        /**< \brief data status after erases, [55:55] */
    uint32 res40 : 2;            /**< \brief Reserved (2), [41:40] */
    uint32 sdSpec4 : 1;          /**< \brief Spec. Version 4.00 or higher, [42:42] */
    uint32 exSecurity : 4;       /**< \brief Extended Security Support, [46:43] */
    uint32 sdSpec3 : 1;          /**< \brief Spec. Version 3.00 or higher, [47:47] */
    uint32 cmdSupport : 4;       /**< \brief Command Support bits, [35:32] */
    uint32 res36 : 4;            /**< \brief Reserved (4), [39:36] */
    uint32 res0 : 32;            /**< \brief Reserved (32), [31:0] */
} IfxSdmmc_CardReg_SCRBits;

/** \brief Card status recieved in response
 */
typedef struct
{
    uint32 res0 : 3;                            /**< \brief Reserved */
    uint32 authenticationSequenceError : 1;     /**< \brief AKE_SEQ_ERROR (SD Memory Card app. spec.), Error in the sequence of the authentication process */
    uint32 res4 : 1;                            /**< \brief Reserved */
    uint32 acmd : 1;                            /**< \brief APP_CMD, The card will expect ACMD, or an indication that the command has been interpreted as ACMD */
    uint32 functionEvent : 1;                   /**< \brief FX_EVENT, Extension Functions may set this bit to get host to deal with events */
    uint32 res7 : 1;                            /**< \brief Reserved */
    uint32 readyForData : 1;                    /**< \brief READY_FOR_DATA, Corresponds to buffer empty signaling on the bus */
    uint32 currentState : 4;                    /**< \brief CURRENT_STATE, The state of the card when receiving the command. */
    uint32 eraseReset : 1;                      /**< \brief ERASE_RESET, An erase sequence was cleared before executing because an out of erase sequence command was received */
    uint32 eccDisabled : 1;                     /**< \brief CARD_ECC_DISABLED, The command has been executed without using the internal ECC */
    uint32 wpEraseSkip : 1;                     /**< \brief WP_ERASE_SKIP, Set when only partial address space was erased due to existing write protected blocks or the temporary or permanent write protected card was erased */
    uint32 csdOverwrite : 1;                    /**< \brief CSD_OVERWRITE, Can be either one of the following errors:\n
                                                 * - The read only section of the CSD does not match the card content.\n
                                                 * - An attempt to reverse the copy (set as original) or permanent WP (unprotected) bits was made. */
    uint32 res17 : 1;                           /**< \brief Reserved */
    uint32 res18 : 1;                           /**< \brief Reserved */
    uint32 error : 1;                           /**< \brief ERROR, A general or an unknown error occurred during the operation */
    uint32 ccError : 1;                         /**< \brief CC_ERROR, Internal card controller error */
    uint32 eccFailed : 1;                       /**< \brief CARD_ECC_FAILED, Card internal ECC was applied but failed to correct the data */
    uint32 illegalCommand : 1;                  /**< \brief ILLEGAL_COMMAND, Command not legal for the card state */
    uint32 commandCrcError : 1;                 /**< \brief COM_CRC_ERROR, The CRC check of the previous command failed. */
    uint32 lockUnlockFailed : 1;                /**< \brief LOCK_UNLOCK_FAILED, Set when a sequence or password error has been detected in lock/unlock card command. */
    uint32 locked : 1;                          /**< \brief CARD_IS_LOCKED, When set, signals that the card is locked by the host */
    uint32 wpViolation : 1;                     /**< \brief WP_VIOLATION, Set when the host attempts to write to a protected block or to the temporary or permanent write protected card. */
    uint32 eraseParam : 1;                      /**< \brief ERASE_PARAM, An invalid selection of write-blocks for erase occurred. */
    uint32 eraseSeqError : 1;                   /**< \brief ERASE_SEQ_ERROR, An error in the sequence of erase commands occurred */
    uint32 blockLengthError : 1;                /**< \brief BLOCK_LEN_ERROR, The transferred block length is not allowed for this card, or the number of transferred bytes does not match the block length. */
    uint32 addressError : 1;                    /**< \brief ADDRESS_ERROR, A misaligned address which did not match the block length was used in the command. */
    uint32 outOfRange : 1;                      /**< \brief OUT_OF_RANGE, The command's argument was out of the allowed range for this card */
} IfxSdmmc_CardStatusBits;

/** \brief R4 response
 */
typedef union
{
    IfxSdmmc_SdIoResponseR4Bits bits;
    uint32                      r4;
} IfxSdmmc_SdIoResponseR4;

/** \} */

/** \addtogroup IfxLld_Sdmmc_Std_DataStrctures
 * \{ */
/** \brief Card register SCR
 */
typedef union
{
    uint32                   U[2];       /**< \brief Word access */
    IfxSdmmc_CardReg_SCRBits B;          /**< \brief SCR card Register Bitfields */
} IfxSdmmc_CardReg_SCR;

/** \brief Card Status
 */
typedef union
{
    IfxSdmmc_CardStatusBits B;       /**< \brief Card status in Bits format */
    uint32                  U;       /**< \brief Card Status in word format */
} IfxSdmmc_CardStatus;

typedef struct
{
    uint32 data : 8;
    uint32 stuff1 : 1;
    uint32 regAddr : 17;
    uint32 stuff2 : 1;
    uint32 raw : 1;
    uint32 func : 3;
    uint32 rw : 1;
} IfxSdmmc_SdIoCmd52Bits;

/** \brief CMD53 Command bitfield Structure
 */
typedef struct
{
    uint32 Count : 9;
    uint32 regAddr : 17;
    uint32 opCode : 1;
    uint32 blockMode : 1;
    uint32 func : 3;
    uint32 rw : 1;
} IfxSdmmc_SdIoCmd53Bits;

/** \brief R5 response: bit field structure
 */
typedef struct
{
    uint32 data : 8;
    uint32 flags : 8;
    uint32 stuff : 16;
} IfxSdmmc_SdIoResponseR5Bits;

/** \brief SDIO card information
 */
typedef struct
{
    IfxSdmmc_SdIoResponseR4 ioInfo;
} IfxSdmmc_SdIo_CardInfo;

/** \} */

/** \addtogroup IfxLld_Sdmmc_Std_DataStrctures
 * \{ */
/** \brief Descriptor for ADMA2 transfers
 */
typedef struct
{
    uint32 valid : 1;            /**< \brief Indicates validity of a descriptor line */
    uint32 end : 1;              /**< \brief End of Descriptor */
    uint32 intEn : 1;            /**< \brief Force to generate ADMA Interrupt */
    uint32 act : 3;              /**< \brief Action Symbols combination */
    uint32 lengthUpper : 10;     /**< \brief Upper value of the Length in 26 bit length mode */
    uint32 length : 16;          /**< \brief data length (lower part of the length in 26 bit length mode) */
    uint32 address : 32;         /**< \brief Address of the data location */
} IfxSdmmc_Adma2Descriptor;

/** \brief Card information structure, contents of Card registers.
 */
typedef struct
{
    uint32                 ocr;          /**< \brief OCR register contents */
    uint32                 cid[4];       /**< \brief CID register contents */
    uint32                 csd[4];       /**< \brief CSD register contents */
    IfxSdmmc_CardReg_SCR   scr;          /**< \brief SCR register contents */
    uint16                 rca;          /**< \brief RCA register contents */
    uint16                 dsr;          /**< \brief DSR register contents */
    IfxSdmmc_SdIo_CardInfo io;           /**< \brief SDIO card info */
} IfxSdmmc_CardInfo;

/** \brief Configuration structure for Normal and Error interrupts
 */
typedef struct
{
    boolean      commandCompleteInterruptEnable;        /**< \brief Command Complete Interrupt Enable/Disable selection */
    boolean      transferCompleteInterruptEnable;       /**< \brief Transfer Complete Interrupt Enable/Disable selection */
    boolean      dmaInterruptEnable;                    /**< \brief Transfer Complete Interrupt Enable/Disable selection */
    boolean      commandTimeoutInterruptEnable;         /**< \brief Command Timeout Interrupt Enable/Disable selection */
    boolean      dataTimeoutInterruptEnable;            /**< \brief Data Timeout Interrupt Enable/Disable selection */
    Ifx_Priority priority;                              /**< \brief Interrupt service priority */
    IfxSrc_Tos   provider;                              /**< \brief Interrupt service provider */
} IfxSdmmc_InterruptConfig;

/** \brief Response Data
 */
typedef struct
{
    IfxSdmmc_CardStatus cardStatus;       /**< \brief Card status recieved in response */
    uint32              resp01;           /**< \brief Response Register 01 value */
    uint32              resp23;           /**< \brief Response register 23 value */
    uint32              resp45;           /**< \brief Response register 45 value */
    uint32              resp67;           /**< \brief Response register 67 value */
} IfxSdmmc_Response;

/** \brief CMD52 command
 */
typedef union
{
    IfxSdmmc_SdIoCmd52Bits bits;
    uint32                 arg;
} IfxSdmmc_SdIoCmd52;

/** \brief CMD53 Command
 */
typedef union
{
    IfxSdmmc_SdIoCmd53Bits bits;
    uint32                 arg;
} IfxSdmmc_SdIoCmd53;

/** \brief SDIO Response R5
 */
typedef union
{
    uint32                      r5;
    IfxSdmmc_SdIoResponseR5Bits bits;
} IfxSdmmc_SdIoResponseR5;

/** \} */

/** \addtogroup IfxLld_Sdmmc_Std_Functions
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Applies the software reset of the module
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_applySoftwareReset(Ifx_SDMMC *sdmmcSFR);

/** \brief Checks if there is any error in the response recieved
 * \param cardStatus Card Status
 * \return Status
 */
IFX_INLINE IfxSdmmc_Status IfxSdmmc_checkErrorInResponseR6(uint32 cardStatus);

/** \brief Clears the status flag of the slected Error Interrupt
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param interrupt Normal Interrupt
 * \return None
 */
IFX_INLINE void IfxSdmmc_clearErrorInterrupt(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_ErrorInterrupt interrupt);

/** \brief Clears the status flag of the selected Normal Interrupt
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param interrupt Normal Interrupt
 * \return None
 */
IFX_INLINE void IfxSdmmc_clearNormalInterrupt(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_NormalInterrupt interrupt);

/** \brief Disables the Block count in transfer mode
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_disableBlockCount(Ifx_SDMMC *sdmmcSFR);

/** \brief Disables the clock for card
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_disableCardClock(Ifx_SDMMC *sdmmcSFR);

/** \brief Enables All the Error Interrupt for interrupt generation
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_enableAllErrorInterruptStatus(Ifx_SDMMC *sdmmcSFR);

/** \brief Enables the Block count in transfer mode
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_enableBlockCount(Ifx_SDMMC *sdmmcSFR);

/** \brief Enables the clock for card
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_enableCardClock(Ifx_SDMMC *sdmmcSFR);

/** \brief Enables the DMA transfers in host controller
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_enableDmaTransfers(Ifx_SDMMC *sdmmcSFR);

/** \brief Enables the slected Error Interrupt for interrupt generation
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param interrupt Normal Interrupt
 * \return None
 */
IFX_INLINE void IfxSdmmc_enableErrorInterrupt(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_ErrorInterrupt interrupt);

/** \brief Enables the slected Error Interrupt for interrupt generation
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param interrupt Normal Interrupt
 * \return None
 */
IFX_INLINE void IfxSdmmc_enableErrorInterruptStatus(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_ErrorInterrupt interrupt);

/** \brief Enables the host version 4
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_enableHostControllerVersion4(Ifx_SDMMC *sdmmcSFR);

/** \brief Enables the internal clock of host controller
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_enableInternalClock(Ifx_SDMMC *sdmmcSFR);

/** \brief Enables the slected Normal Interrupt for interrupt generation
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param interrupt Normal Interrupt
 * \return None
 */
IFX_INLINE void IfxSdmmc_enableNormalInterrupt(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_NormalInterrupt interrupt);

/** \brief Enables the PLL of host controller
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_enablePll(Ifx_SDMMC *sdmmcSFR);

/** \brief Enables the slected Normal Interrupt status for interrupt generation
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param interrupt Normal Interrupt
 * \return None
 */
IFX_INLINE void IfxSdmmc_enableNormalInterruptStatus(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_NormalInterrupt interrupt);

/** \brief Enables the preset values for clock generation
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_enablePresetValues(Ifx_SDMMC *sdmmcSFR);

/** \brief Returns the whole value of the Error Interrupt Status register
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return Value of the Error Interrupt Status register
 */
IFX_INLINE uint16 IfxSdmmc_getErrorInterruptStatus(Ifx_SDMMC *sdmmcSFR);

/** \brief Returns the whole value of the Normal Interrupt Status register
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return Value of the Normal Interrupt Status register
 */
IFX_INLINE uint16 IfxSdmmc_getNormalInterruptStatus(Ifx_SDMMC *sdmmcSFR);

/** \brief Returns the Src Pointer of the selected SDMMC service request node
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param interruptType Interrupt type (service rerquest node number)
 * \return Src Pointer of the selected SDMMC service request node
 */
IFX_INLINE volatile Ifx_SRC_SRCR *IfxSdmmc_getSrcPointer(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_InterruptType interruptType);

/** \brief Returns the status whether the command line is busy to issue a command or not
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return Status whether the command line is busy to issue a command or not\n
 * True: If busy, False: If not available
 */
IFX_INLINE boolean IfxSdmmc_isCommandLineBusy(Ifx_SDMMC *sdmmcSFR);

/** \brief Returns the status whether the DAta line is busy to issue a command with Data or not
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return Status whether the data line is busy to issue a command with Data or not\n
 * True: If busy, False: If not available
 */
IFX_INLINE boolean IfxSdmmc_isDataLineBusy(Ifx_SDMMC *sdmmcSFR);

/** \brief Returns the status whether the selected Error Interrupt flag is set or not
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param interrupt Error Interrupt
 * \return status whether the selected Error Interrupt flag is set or not\n
 * True: If set, False: If not set
 */
IFX_INLINE boolean IfxSdmmc_isErrorInterruptOccured(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_ErrorInterrupt interrupt);

/** \brief Returns the status of the version 4 support of host controller
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return Version 4 Enable\n
 * TRUE: enable; FALSE: disable
 */
IFX_INLINE boolean IfxSdmmc_isHostControllerVersion4Enable(Ifx_SDMMC *sdmmcSFR);

/** \brief Returns the status of the internal clock stability of host controller
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return Internal Clock State\n
 * TRUE: stable; FALSE: unstable
 */
IFX_INLINE boolean IfxSdmmc_isInternalClockStable(Ifx_SDMMC *sdmmcSFR);

/** \brief Returns the status of whether clock for SDMMC module is enabled or diabled
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return Status:\n
 * TRUE : if module is enabled\n
 * FALSE : if module is disabled
 */
IFX_INLINE boolean IfxSdmmc_isModuleEnabled(Ifx_SDMMC *sdmmcSFR);

/** \brief Returns the status whether the selected Normal Interrupt flag is set or not
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param interrupt Normal Interrupt
 * \return status whether the selected Normal Interrupt flag is set or not\n
 * True: If set, False: If not set
 */
IFX_INLINE boolean IfxSdmmc_isNormalInterruptOccured(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_NormalInterrupt interrupt);

/** \brief Returns data from buffer data register
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return Data
 */
IFX_INLINE uint32 IfxSdmmc_readBufferData(Ifx_SDMMC *sdmmcSFR);

/** \brief Returns the contents of Response register Resp01
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return value of Res01 register
 */
IFX_INLINE uint32 IfxSdmmc_readResponseReg01(Ifx_SDMMC *sdmmcSFR);

/** \brief Returns the contents of Response register Resp23
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return value of Res23 register
 */
IFX_INLINE uint32 IfxSdmmc_readResponseReg23(Ifx_SDMMC *sdmmcSFR);

/** \brief Returns the contents of Response register Resp45
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return value of Res45 register
 */
IFX_INLINE uint32 IfxSdmmc_readResponseReg45(Ifx_SDMMC *sdmmcSFR);

/** \brief Returns the contents of Response register Resp67
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return value of Res67 register
 */
IFX_INLINE uint32 IfxSdmmc_readResponseReg67(Ifx_SDMMC *sdmmcSFR);

/** \brief Sets the value to Argument register
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param argument value to be written to argument register
 * \return None
 */
IFX_INLINE void IfxSdmmc_setArgument(Ifx_SDMMC *sdmmcSFR, uint32 argument);

/** \brief Sets the block size
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param size blockSize
 * \return None
 */
IFX_INLINE void IfxSdmmc_setBlockSize(Ifx_SDMMC *sdmmcSFR, uint16 size);

/** \brief Clears the Power Bus On request for card
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_setBusPowerOff(Ifx_SDMMC *sdmmcSFR);

/** \brief Sets the Power Bus On request for card
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_setBusPowerOn(Ifx_SDMMC *sdmmcSFR);

/** \brief Sets SD Bus Voltage Select for VDD1 (3.3V)/eMMC Bus Voltage Select for VDD(3V).
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_setBusVoltage(Ifx_SDMMC *sdmmcSFR);

/** \brief Sets the cardtype as EMMC
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_setCardTypeEmmc(Ifx_SDMMC *sdmmcSFR);

/** \brief Sets the value to command register
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param command value to be written to command register
 * \return None
 */
IFX_INLINE void IfxSdmmc_setCommand(Ifx_SDMMC *sdmmcSFR, uint16 command);

/** \brief Sets the interval by which DAT line timeouts are detected
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param value the interval by which DAT line timeouts are detected
 * \return None
 */
IFX_INLINE void IfxSdmmc_setDataLineTimeoutValue(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_DataLineTimeout value);

/** \brief Selects the type of DMA used for data transfers in host controller
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param dmaType type of DMA used for data transfers in host controller
 * \return None
 */
IFX_INLINE void IfxSdmmc_setDmaType(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_DmaType dmaType);

/** \brief Sets the data width of the eMMC transfers in host controller, (number of data lines)
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param width Data width of the eMMC transfers
 * \return None
 */
IFX_INLINE void IfxSdmmc_setEmmcDataTransferWidth(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_EmmcDataTransferWidth width);

/** \brief Sets the speed mode of the eMMC transfers in host controller,
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param speed Speed Mode of the EMMC transfers
 * \return None
 */
IFX_INLINE void IfxSdmmc_setEmmcSpeedMode(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_EmmcSpeedMode speed);

/** \brief Sets the multiblock block select , single block or multi block
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param select Multi block select
 * \return None
 */
IFX_INLINE void IfxSdmmc_setMultiBlockSelect(Ifx_SDMMC *sdmmcSFR, boolean select);

/** \brief Sets the data width of the SD transfers in host controller, (number of data lines)
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param width Data width of the SD transfers
 * \return None
 */
IFX_INLINE void IfxSdmmc_setSdDataTransferWidth(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_SdDataTransferWidth width);

/** \brief Sets the speed mode of the SD transfers in host controller,
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param speed Speed Mode of the SD transfers
 * \return None
 */
IFX_INLINE void IfxSdmmc_setSdSpeedMode(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_SdSpeedMode speed);

/** \brief Sets the Sytem address for the DMA transfers.
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param address Sysytem Address for DMA transfers
 * \return None
 */
IFX_INLINE void IfxSdmmc_setSystemAddressForDma(Ifx_SDMMC *sdmmcSFR, uint32 address);

/** \brief Sets the Data transfer direcyion , write/read
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param direction Data transfer Direction
 * \return None
 */
IFX_INLINE void IfxSdmmc_setTransferDirection(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_TransferDirection direction);

/** \brief Writes data to buffer data register
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param data Pointer to data buffer
 * \return None
 */
IFX_INLINE void IfxSdmmc_writeBufferData(Ifx_SDMMC *sdmmcSFR, uint32 *data);

/** \brief Returns fail if error is present in R1 response
 * \param cardStatus R1 response data
 * \return error status
 */
IFX_INLINE IfxSdmmc_Status IfxSdmmc_checkErrorInResponseR1(uint32 cardStatus);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Configures the parameters for clock generation
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param frequency Frequency Select
 * \return None
 */
IFX_EXTERN void IfxSdmmc_configureClock(Ifx_SDMMC *sdmmcSFR, uint32 frequency);

/** \brief Enables the module
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_EXTERN void IfxSdmmc_enableModule(Ifx_SDMMC *sdmmcSFR);

/** \brief Reads the response from response registers based on response type
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param command Command to send
 * \param responseType response type (R1, R1b, R2, R3, R6, R7)
 * \param response value of the response recieved, fo rthe commands which recieves response
 * \return Status
 */
IFX_EXTERN IfxSdmmc_Status IfxSdmmc_readResponse(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_Command command, IfxSdmmc_ResponseType responseType, IfxSdmmc_Response *response);

/** \brief Applies the Kernel reset of the module
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_EXTERN void IfxSdmmc_resetModule(Ifx_SDMMC *sdmmcSFR);

/** \brief Issues a application command APP_CMD to card
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param rca RCA of the card
 * \return Status
 */
IFX_EXTERN IfxSdmmc_Status IfxSdmmc_sendApplicationCommand(Ifx_SDMMC *sdmmcSFR, uint16 rca);

/** \brief Issues a command to card
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \param command Command to send
 * \param argument Argument of the command
 * \param responseType response type (R1, R1b, R2, R3, R6, R7)
 * \param response value of the response recieved, for the commands which recieves response
 * \return Status
 */
IFX_EXTERN IfxSdmmc_Status IfxSdmmc_sendCommand(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_Command command, uint32 argument, IfxSdmmc_ResponseType responseType, IfxSdmmc_Response *response);

/** \brief Sets up the Internal clock of host controller
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return Status
 */
IFX_EXTERN IfxSdmmc_Status IfxSdmmc_setUpInternalClock(Ifx_SDMMC *sdmmcSFR);

/** \} */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief disable the PLL
 * \param sdmmcSFR pointer to base address of SDMMC register space
 * \return None
 */
IFX_INLINE void IfxSdmmc_disablePll(Ifx_SDMMC *sdmmcSFR);

/** \brief Set the Block Count for multiblock Transfers
 * \param sdmmcSFR Pointer to SDMMC SFR
 * \param blockCount Block Count value
 * \return None
 */
IFX_INLINE void IfxSdmmc_setBlockCount(Ifx_SDMMC *sdmmcSFR, uint32 blockCount);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Switches the clock frequency of communication during run.
 * Caution: Ensure compatibility in the speed mode before switching Clock Frequency.
 * \param sdmmcSFR pointer to the base address of SDMCC register space
 * \param frequency frequency of clock required
 * \return None
 */
IFX_EXTERN void IfxSdmmc_switchClockFrequency(Ifx_SDMMC *sdmmcSFR, uint32 frequency);

/** \brief Check any error in reponse type R5
 * \param resp01 response data
 * \return error status
 */
IFX_EXTERN IfxSdmmc_Status IfxSdmmc_checkerrorInReponseR5(uint32 resp01);

/******************************************************************************/
/*-------------------Global Exported Variables/Constants----------------------*/
/******************************************************************************/

/** \brief Array of all Command Structures defining SD, MMC and Application specific commands
 */
IFX_EXTERN Ifx_SDMMC_CMD_Bits IfxSdmmc_CMD[IFXSDMMC_NUM_COMMANDS];

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxSdmmc_applySoftwareReset(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->SW_RST.U |= 1;
}


IFX_INLINE IfxSdmmc_Status IfxSdmmc_checkErrorInResponseR6(uint32 cardStatus)
{
    return (IfxSdmmc_Status)(cardStatus & IFXSDMMC_CARD_R6STATUS_ERROR_CHECK_MSK);
}


IFX_INLINE void IfxSdmmc_clearErrorInterrupt(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_ErrorInterrupt interrupt)
{
    uint16 value = 1 << interrupt;
    sdmmcSFR->ERROR_INT_STAT.U &= value;
}


IFX_INLINE void IfxSdmmc_clearNormalInterrupt(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_NormalInterrupt interrupt)
{
    uint16 value = 1 << interrupt;
    sdmmcSFR->NORMAL_INT_STAT.U &= value;
}


IFX_INLINE void IfxSdmmc_disableBlockCount(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->XFER_MODE.B.BLOCK_COUNT_ENABLE = 0;
}


IFX_INLINE void IfxSdmmc_disableCardClock(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->CLK_CTRL.B.SD_CLK_EN = 0;
}


IFX_INLINE void IfxSdmmc_enableAllErrorInterruptStatus(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->ERROR_INT_STAT_EN.U = 0xFFFF;
}


IFX_INLINE void IfxSdmmc_enableBlockCount(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->XFER_MODE.B.BLOCK_COUNT_ENABLE = 1;
}


IFX_INLINE void IfxSdmmc_enableCardClock(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->CLK_CTRL.B.SD_CLK_EN = 1;
}


IFX_INLINE void IfxSdmmc_enableDmaTransfers(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->XFER_MODE.B.DMA_ENABLE = 1;
}


IFX_INLINE void IfxSdmmc_enableErrorInterrupt(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_ErrorInterrupt interrupt)
{
    uint16 value = 1 << interrupt;
    sdmmcSFR->ERROR_INT_SIGNAL_EN.U |= value;
}


IFX_INLINE void IfxSdmmc_enableErrorInterruptStatus(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_ErrorInterrupt interrupt)
{
    uint16 value = 1 << interrupt;
    sdmmcSFR->ERROR_INT_STAT_EN.U |= value;
}


IFX_INLINE void IfxSdmmc_enableHostControllerVersion4(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->HOST_CTRL2.B.HOST_VER4_ENABLE = 1;
}


IFX_INLINE void IfxSdmmc_enableInternalClock(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->CLK_CTRL.B.INTERNAL_CLK_EN = 1;
}


IFX_INLINE void IfxSdmmc_enableNormalInterrupt(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_NormalInterrupt interrupt)
{
    uint16 value = 1 << interrupt;
    sdmmcSFR->NORMAL_INT_SIGNAL_EN.U |= value;
}


IFX_INLINE void IfxSdmmc_enablePll(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->CLK_CTRL.B.PLL_ENABLE = 1;
}


IFX_INLINE void IfxSdmmc_enableNormalInterruptStatus(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_NormalInterrupt interrupt)
{
    uint16 value = 1 << interrupt;
    sdmmcSFR->NORMAL_INT_STAT_EN.U |= value;
}


IFX_INLINE void IfxSdmmc_enablePresetValues(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->HOST_CTRL2.B.PRESET_VAL_ENABLE = 1;
}


IFX_INLINE uint16 IfxSdmmc_getErrorInterruptStatus(Ifx_SDMMC *sdmmcSFR)
{
    return sdmmcSFR->ERROR_INT_STAT.U;
}


IFX_INLINE uint16 IfxSdmmc_getNormalInterruptStatus(Ifx_SDMMC *sdmmcSFR)
{
    return sdmmcSFR->NORMAL_INT_STAT.U;
}


IFX_INLINE volatile Ifx_SRC_SRCR *IfxSdmmc_getSrcPointer(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_InterruptType interruptType)
{
    if (interruptType == IfxSdmmc_InterruptType_normal)
    {
        return &MODULE_SRC.SDMMC.SDMMC[0].ERR;
    }
    else
    {
        return &MODULE_SRC.SDMMC.SDMMC[0].DMA;
    }
}


IFX_INLINE boolean IfxSdmmc_isCommandLineBusy(Ifx_SDMMC *sdmmcSFR)
{
    return sdmmcSFR->PSTATE_REG.B.CMD_INHIBIT;  // FIXME: or its compliment?
}


IFX_INLINE boolean IfxSdmmc_isDataLineBusy(Ifx_SDMMC *sdmmcSFR)
{
    return sdmmcSFR->PSTATE_REG.B.CMD_INHIBIT_DAT;
}


IFX_INLINE boolean IfxSdmmc_isErrorInterruptOccured(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_ErrorInterrupt interrupt)
{
    uint16 flag = (1 << interrupt);
    return sdmmcSFR->ERROR_INT_STAT.U & flag;
}


IFX_INLINE boolean IfxSdmmc_isHostControllerVersion4Enable(Ifx_SDMMC *sdmmcSFR)
{
    return sdmmcSFR->HOST_CTRL2.B.HOST_VER4_ENABLE;
}


IFX_INLINE boolean IfxSdmmc_isInternalClockStable(Ifx_SDMMC *sdmmcSFR)
{
    return sdmmcSFR->CLK_CTRL.B.INTERNAL_CLK_STABLE;
}


IFX_INLINE boolean IfxSdmmc_isModuleEnabled(Ifx_SDMMC *sdmmcSFR)
{
    return (sdmmcSFR->CLC.B.DISS == 0) ? 1 : 0;
}


IFX_INLINE boolean IfxSdmmc_isNormalInterruptOccured(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_NormalInterrupt interrupt)
{
    uint16 flag = (1 << interrupt);
    return sdmmcSFR->NORMAL_INT_STAT.U & flag;
}


IFX_INLINE uint32 IfxSdmmc_readBufferData(Ifx_SDMMC *sdmmcSFR)
{
    return sdmmcSFR->BUF_DATA.U;
}


IFX_INLINE uint32 IfxSdmmc_readResponseReg01(Ifx_SDMMC *sdmmcSFR)
{
    return sdmmcSFR->RESP01.U;
}


IFX_INLINE uint32 IfxSdmmc_readResponseReg23(Ifx_SDMMC *sdmmcSFR)
{
    return sdmmcSFR->RESP23.U;
}


IFX_INLINE uint32 IfxSdmmc_readResponseReg45(Ifx_SDMMC *sdmmcSFR)
{
    return sdmmcSFR->RESP45.U;
}


IFX_INLINE uint32 IfxSdmmc_readResponseReg67(Ifx_SDMMC *sdmmcSFR)
{
    return sdmmcSFR->RESP67.U;
}


IFX_INLINE void IfxSdmmc_setArgument(Ifx_SDMMC *sdmmcSFR, uint32 argument)
{
    sdmmcSFR->ARGUMENT.U = argument;
}


IFX_INLINE void IfxSdmmc_setBlockSize(Ifx_SDMMC *sdmmcSFR, uint16 size)
{
    sdmmcSFR->BLOCKSIZE.B.XFER_BLOCK_SIZE = size;
}


IFX_INLINE void IfxSdmmc_setBusPowerOff(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->PWR_CTRL.B.SD_BUS_PWR_VDD1 = 0;
}


IFX_INLINE void IfxSdmmc_setBusPowerOn(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->PWR_CTRL.B.SD_BUS_PWR_VDD1 = 1;
}


IFX_INLINE void IfxSdmmc_setBusVoltage(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->PWR_CTRL.B.SD_BUS_VOL_VDD1 = 7;
}


IFX_INLINE void IfxSdmmc_setCardTypeEmmc(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->EMMC_CTRL.B.CARD_IS_EMMC = 1;
}


IFX_INLINE void IfxSdmmc_setCommand(Ifx_SDMMC *sdmmcSFR, uint16 command)
{
    sdmmcSFR->CMD.U = command;
}


IFX_INLINE void IfxSdmmc_setDataLineTimeoutValue(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_DataLineTimeout value)
{
    sdmmcSFR->TOUT_CTRL.B.TOUT_CNT = value;
}


IFX_INLINE void IfxSdmmc_setDmaType(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_DmaType dmaType)
{
    sdmmcSFR->HOST_CTRL1.B.DMA_SEL = dmaType;
}


IFX_INLINE void IfxSdmmc_setEmmcDataTransferWidth(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_EmmcDataTransferWidth width)
{
    if (width < IfxSdmmc_EmmcDataTransferWidth_8Bit)
    {
        sdmmcSFR->HOST_CTRL1.B.DAT_XFER_WIDTH = width;
    }
    else
    {
        sdmmcSFR->HOST_CTRL1.B.DAT_XFER_WIDTH = 1;
        sdmmcSFR->HOST_CTRL1.B.EXT_DAT_XFER   = 1;
    }
}


IFX_INLINE void IfxSdmmc_setEmmcSpeedMode(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_EmmcSpeedMode speed)
{
    sdmmcSFR->HOST_CTRL2.B.UHS_MODE_SEL = speed;
}


IFX_INLINE void IfxSdmmc_setMultiBlockSelect(Ifx_SDMMC *sdmmcSFR, boolean select)
{
    sdmmcSFR->XFER_MODE.B.MULTI_BLK_SEL = select;
}


IFX_INLINE void IfxSdmmc_setSdDataTransferWidth(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_SdDataTransferWidth width)
{
    sdmmcSFR->HOST_CTRL1.B.DAT_XFER_WIDTH = width;
}


IFX_INLINE void IfxSdmmc_setSdSpeedMode(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_SdSpeedMode speed)
{
    sdmmcSFR->HOST_CTRL1.B.HIGH_SPEED_EN = speed;
}


IFX_INLINE void IfxSdmmc_setSystemAddressForDma(Ifx_SDMMC *sdmmcSFR, uint32 address)
{
    if (IfxSdmmc_isHostControllerVersion4Enable(sdmmcSFR))
    {
        sdmmcSFR->ADMA_SA_LOW.U = address;
    }
    else
    {
        sdmmcSFR->SDMASA.U = address;
    }
}


IFX_INLINE void IfxSdmmc_setTransferDirection(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_TransferDirection direction)
{
    sdmmcSFR->XFER_MODE.B.DATA_XFER_DIR = direction;
}


IFX_INLINE void IfxSdmmc_writeBufferData(Ifx_SDMMC *sdmmcSFR, uint32 *data)
{
    sdmmcSFR->BUF_DATA.U = *data;
}


IFX_INLINE void IfxSdmmc_disablePll(Ifx_SDMMC *sdmmcSFR)
{
    sdmmcSFR->CLK_CTRL.B.PLL_ENABLE = 0;
}


IFX_INLINE IfxSdmmc_Status IfxSdmmc_checkErrorInResponseR1(uint32 cardStatus)
{
    return ((cardStatus & IFXSDMMC_CARD_IOONLY_STATUS_CHECK_MSK) == IFXSDMMC_CARD_IOONLY_STATUS_CHECK_MSK) ? (IfxSdmmc_Status)(cardStatus & IFXSDMMC_CARD_R1STATUS_ERROR_CHECK_MSK) : (IfxSdmmc_Status)(cardStatus & IFXSDMMC_IOCARD_R1STATUS_ERROR_CHECK_MSK);
}


IFX_INLINE void IfxSdmmc_setBlockCount(Ifx_SDMMC *sdmmcSFR, uint32 blockCount)
{
    if (IfxSdmmc_isHostControllerVersion4Enable(sdmmcSFR))
    {
        sdmmcSFR->SDMASA.U = blockCount;
    }
    else
    {
        sdmmcSFR->BLOCKCOUNT.U = (uint16)blockCount;
    }
}


#endif /* IFXSDMMC_H */
