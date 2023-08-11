/**
 * \file IfxRif.h
 * \brief RIF  basic functionality
 * \ingroup IfxLld_Rif
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
 *
 * \defgroup IfxLld_Rif_Std_Enum Enumerations
 * \ingroup IfxLld_Rif_Std
 * \defgroup IfxLld_Rif_Std_Deserializer Deserializer Functions
 * \ingroup IfxLld_Rif_Std
 * \defgroup IfxLld_Rif_Std_FLM Fifo and Line Management Functions
 * \ingroup IfxLld_Rif_Std
 * \defgroup IfxLld_Rif_Std_DFU Data Formating Unit Functions
 * \ingroup IfxLld_Rif_Std
 * \defgroup IfxLld_Rif_Std_RSM Radar State Machine Functions
 * \ingroup IfxLld_Rif_Std
 * \defgroup IfxLld_Rif_Std_Interrupt Interrupt Functions
 * \ingroup IfxLld_Rif_Std
 * \defgroup IfxLld_Rif_Std_Monitor Monitoring Functions
 * \ingroup IfxLld_Rif_Std
 * \defgroup IfxLld_Rif_Std_Module Module Functions
 * \ingroup IfxLld_Rif_Std
 * \defgroup IfxLld_Rif_Std_LVDS LDVS Functions
 * \ingroup IfxLld_Rif_Std
 */

#ifndef IFXRIF_H
#define IFXRIF_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxRif_cfg.h"
#include "Src/Std/IfxSrc.h"
#include "Scu/Std/IfxScuWdt.h"
#include "_Utilities/Ifx_Assert.h"
#include "IfxRif_reg.h"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Rif_Std_Enum
 * \{ */
/** \brief Defines the Type of ADCs used for radar cycle internal /external
 */
typedef enum
{
    IfxRif_Adc_external,  /**< \brief External ADCs (deserializer enabled) */
    IfxRif_Adc_internal   /**< \brief Internal ADCs (deserializer disabled) */
} IfxRif_Adc;

/** \brief Defines the current state of the deserializer\n
 * Definition in Ifx_RIF.ESI.B.CALBSY
 */
typedef enum
{
    IfxRif_CalibrationState_done,  /**< \brief Calibration is finished (no calibration ongoing) */
    IfxRif_CalibrationState_busy   /**< \brief Calibration is ongoing */
} IfxRif_CalibrationState;

/** \brief Defines the status of the latest timing calibration sequence.\n
 * Definition in Ifx_RIF.ESI.B.CALSTAT
 */
typedef enum
{
    IfxRif_CalibrationStatus_fail,    /**< \brief Calibration is failed (signal paths uncalibrated) */
    IfxRif_CalibrationStatus_success  /**< \brief Calibration is successful */
} IfxRif_CalibrationStatus;

/** \brief Defines the polarity of the clock signal on the clock input pins.\n
 * Definition in Ifx_RIF.ESI.B.CP
 */
typedef enum
{
    IfxRif_ClockPolarity_default,  /**< \brief Default polarity of the clock signal */
    IfxRif_ClockPolarity_inverted  /**< \brief Inverted polarity of the clock signal */
} IfxRif_ClockPolarity;

/** \brief Defines the Common control selection for all LVDS Pads .\n
 * Definition in Ifx_RIF.LVDSCON1.B.MISC
 */
typedef enum
{
    IfxRif_CommonLvdsPadControl_5vMode,       /**< \brief enable 5V Mode for all LVDS pads */
    IfxRif_CommonLvdsPadControl_lowSpeedMode  /**< \brief enable low speed mode for all LVDS pads */
} IfxRif_CommonLvdsPadControl;

/** \brief Defines the alignment of the data delivered to the SPU.\n
 * Definition in Ifx_RIF.DFU.B.DA
 */
typedef enum
{
    IfxRif_DataAlignment_right,  /**< \brief Data alignment right (integer) */
    IfxRif_DataAlignment_left    /**< \brief Data alignment left (fractional) */
} IfxRif_DataAlignment;

/** \brief Defines the format of the data delivered to the SPU.\n
 * Definition in Ifx_RIF.DFU.B.DF
 */
typedef enum
{
    IfxRif_DataFormat_unsigned,  /**< \brief Data format unsigned */
    IfxRif_DataFormat_signed     /**< \brief Data format signed */
} IfxRif_DataFormat;

/** \brief Defines the data length of the frame signal.\n
 * Definition in Ifx_RIF.IPI.B.DL
 */
typedef enum
{
    IfxRif_DataLength_10bit,     /**< \brief Data length 10bit  */
    IfxRif_DataLength_12bit,     /**< \brief Data length 12bit  */
    IfxRif_DataLength_14bit,     /**< \brief Data length 14bit  */
    IfxRif_DataLength_16bit      /**< \brief Data length 16bit  */
} IfxRif_DataLength;

/** \brief Defines the polarity of the data signals on the data input pins.\n
 * Definition in Ifx_RIF.ESI.B.DP
 */
typedef enum
{
    IfxRif_DataPolarity_default,  /**< \brief Default polarity of the data signal */
    IfxRif_DataPolarity_inverted  /**< \brief Inverted polarity of the data signal */
} IfxRif_DataPolarity;

/** \brief Defines the deserializer number (0 to 3).\n
 */
typedef enum
{
    IfxRif_DeserializerId_0,     /**< \brief Deserializer 0  */
    IfxRif_DeserializerId_1,     /**< \brief Deserializer 1  */
    IfxRif_DeserializerId_2,     /**< \brief Deserializer 2  */
    IfxRif_DeserializerId_3      /**< \brief Deserializer 3  */
} IfxRif_DeserializerId;

/** \brief Defines the FIFO number (0 to 3).\n
 */
typedef enum
{
    IfxRif_FifoId_0,     /**< \brief Fifo 0  */
    IfxRif_FifoId_1,     /**< \brief Fifo 1  */
    IfxRif_FifoId_2,     /**< \brief Fifo 2  */
    IfxRif_FifoId_3      /**< \brief Fifo 3  */
} IfxRif_FifoId;

/** \brief Defines the FLM mode of operation.\n
 * Definition in Ifx_RIF.FLM.B.MODE
 */
typedef enum
{
    IfxRif_FlmMode_direct,  /**< \brief No swap of the data paths */
    IfxRif_FlmMode_swap     /**< \brief Swap of data paths of channels 0-1 and 2-3 */
} IfxRif_FlmMode;

/** \brief Defines the polarity of the frame signal on the frame input pins.\n
 * Definition in Ifx_RIF.ESI.B.FP
 */
typedef enum
{
    IfxRif_FramePolarity_default,  /**< \brief Default polarity of the frame signal */
    IfxRif_FramePolarity_inverted  /**< \brief Inverted polarity of the frame signal */
} IfxRif_FramePolarity;

/** \brief Defines the Full swap mode operation of FLM.\n
 * Definition in Ifx_RIF.FLM.B.FSWP
 */
typedef enum
{
    IfxRif_FullSwapMode_direct,  /**< \brief No swap of the data paths */
    IfxRif_FullSwapMode_swap     /**< \brief Swap of data paths of channels 0-3 and 1-2 */
} IfxRif_FullSwapMode;

/** \brief Defines the available interrupts in RIF module.\n
 * Definition in Ifx_RIF.INTCON and Ifx_RIF.CRC
 */
typedef enum
{
    IfxRif_Interrupt_calibrationEnd,         /**< \brief Calibration End interrupt */
    IfxRif_Interrupt_frameWatchdogOverflow,  /**< \brief Frame Watchdog Overflow interrupt */
    IfxRif_Interrupt_rampEnd,                /**< \brief Ramp End interrupt */
    IfxRif_Interrupt_chirpEnd,               /**< \brief Chirp End interrupt */
    IfxRif_Interrupt_crcErrorOnLine0,        /**< \brief CRC error interrupt on Line 0 */
    IfxRif_Interrupt_crcErrorOnLine1,        /**< \brief CRC error interrupt on Line 1 */
    IfxRif_Interrupt_crcErrorOnLine2,        /**< \brief CRC error interrupt on Line 2 */
    IfxRif_Interrupt_crcErrorOnLine3,        /**< \brief CRC error interrupt on Line 3 */
    IfxRif_Interrupt_ramp1Error,             /**< \brief Ramp1 Error interrupt */
    IfxRif_Interrupt_chirp1Error,            /**< \brief Chirp1 Error interrupt */
    IfxRif_Interrupt_ramp1Start              /**< \brief Ramp1 Start interrupt */
} IfxRif_Interrupt;

/** \brief Defines the mode of Lvds Bias DistributorMode.\n
 * Definition in Ifx_RIF.LVDSCON1.B.PWRDN
 */
typedef enum
{
    IfxRif_LvdsBiasDistributorMode_active,      /**< \brief Active */
    IfxRif_LvdsBiasDistributorMode_poweredDown  /**< \brief Powered Down */
} IfxRif_LvdsBiasDistributorMode;

/** \brief Defines the LVDS PAD control selection.\n
 * Definition in Ifx_RIF.LVDSCON0.B.FRAME / CLK / DATA0 / DATA1 / LVDSCON1.B.DATA2 / DATA3
 */
typedef enum
{
    IfxRif_LvdsPadControl_none       = 0x00,  /**< \brief NO selection */
    IfxRif_LvdsPadControl_test       = 0x01,  /**< \brief Test Enable (enable Weak Driver Mode) */
    IfxRif_LvdsPadControl_rterm      = 0x02,  /**< \brief Rterm Enable */
    IfxRif_LvdsPadControl_frameClock = 0x04,  /**< \brief Frame Clock LVDS Pad Enable */
    IfxRif_LvdsPadControl_xor        = 0x08,  /**< \brief LVDS XOR-IN tied to LO by RIF-IP */
    IfxRif_LvdsPadControl_spareBit   = 0x10,  /**< \brief spare bit - tied to LO by RIF-IP */
    IfxRif_LvdsPadControl_TGatePTest = 0x20,  /**< \brief LVDS pad enable T-Gate P Test */
    IfxRif_LvdsPadControl_TGateNTest = 0x40   /**< \brief LVDS pad enable T-Gate N-Test */
} IfxRif_LvdsPadControl;

/** \brief Defines the number of samples, used in different phases of radar operation cycle
 */
typedef enum
{
    IfxRif_NumberOfSamples_1,     /**< \brief Number of samples 1  */
    IfxRif_NumberOfSamples_2,     /**< \brief Number of samples 2  */
    IfxRif_NumberOfSamples_3,     /**< \brief Number of samples 3  */
    IfxRif_NumberOfSamples_4,     /**< \brief Number of samples 4  */
    IfxRif_NumberOfSamples_5,     /**< \brief Number of samples 5  */
    IfxRif_NumberOfSamples_6,     /**< \brief Number of samples 6  */
    IfxRif_NumberOfSamples_7,     /**< \brief Number of samples 7  */
    IfxRif_NumberOfSamples_8,     /**< \brief Number of samples 8  */
    IfxRif_NumberOfSamples_9,     /**< \brief Number of samples 9  */
    IfxRif_NumberOfSamples_10,    /**< \brief Number of samples 10  */
    IfxRif_NumberOfSamples_11,    /**< \brief Number of samples 11  */
    IfxRif_NumberOfSamples_12,    /**< \brief Number of samples 12  */
    IfxRif_NumberOfSamples_13,    /**< \brief Number of samples 13  */
    IfxRif_NumberOfSamples_14,    /**< \brief Number of samples 14  */
    IfxRif_NumberOfSamples_15,    /**< \brief Number of samples 15  */
    IfxRif_NumberOfSamples_16     /**< \brief Number of samples 16  */
} IfxRif_NumberOfSamples;

/** \brief Defines the source for the RAMP1 signal multiplexer.\n
 * Definition in Ifx_RIF.RSM1.B.R1SEL
 */
typedef enum
{
    IfxRif_Ramp1SignalInput_0,     /**< \brief input 0  */
    IfxRif_Ramp1SignalInput_1,     /**< \brief input 1  */
    IfxRif_Ramp1SignalInput_2,     /**< \brief input 2  */
    IfxRif_Ramp1SignalInput_3      /**< \brief input 3  */
} IfxRif_Ramp1SignalInput;

/** \brief Defines the polarity of the RAMP1 signal .\n
 * Definition in Ifx_RIF.RSM1.B.R1POL
 */
typedef enum
{
    IfxRif_Ramp1SignalPolarity_lowActive,  /**< \brief Low Active */
    IfxRif_Ramp1SignalPolarity_highActive  /**< \brief High Active */
} IfxRif_Ramp1SignalPolarity;

/** \brief Defines the shift direction of the serial data, corresponding to the data bit on
 * the lsb position in the delivered parallel data.\n
 * Definition in Ifx_RIF.DFU.B.MSB
 */
typedef enum
{
    IfxRif_ShiftDirection_msbFirst,  /**< \brief Shift direction MSB first */
    IfxRif_ShiftDirection_lsbFirst   /**< \brief Shift direction LSB first */
} IfxRif_ShiftDirection;

/** \brief OCDS Suspend Control (OCDS.SUS)
 */
typedef enum
{
    IfxRif_SuspendMode_none = 0,  /**< \brief No suspend */
    IfxRif_SuspendMode_hard = 1,  /**< \brief Hard Suspend */
    IfxRif_SuspendMode_soft = 2   /**< \brief Soft Suspend */
} IfxRif_SuspendMode;

/** \} */

/** \addtogroup IfxLld_Rif_Std_Deserializer
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Disables the selected deserializer
 * \param rif Specifies the pointer to the base of RIF registers
 * \param deserializerId Deserializer number
 * \return None
 */
IFX_INLINE void IfxRif_disableDeserializer(Ifx_RIF *rif, IfxRif_DeserializerId deserializerId);

/** \brief Enables the calibration mode of the deserializer
 * \param rif Specifies the pointer to the base of RIF registers
 * \return None
 */
IFX_INLINE void IfxRif_enableCalibration(Ifx_RIF *rif);

/** \brief Enables the selected deserializer
 * \param rif Specifies the pointer to the base of RIF registers
 * \param deserializerId Deserializer number
 * \return None
 */
IFX_INLINE void IfxRif_enableDeserializer(Ifx_RIF *rif, IfxRif_DeserializerId deserializerId);

/** \brief Returns the current state of the deserializer
 * \param rif Specifies the pointer to the base of RIF registers
 * \return Calibration state (done / on going)
 */
IFX_INLINE IfxRif_CalibrationState IfxRif_getCalibrationState(Ifx_RIF *rif);

/** \brief Returns the status of the latest timing calibration sequence
 * \param rif Specifies the pointer to the base of RIF registers
 * \return Calibration status (fail / success)
 */
IFX_INLINE IfxRif_CalibrationStatus IfxRif_getCalibrationStatus(Ifx_RIF *rif);

/** \} */

/** \addtogroup IfxLld_Rif_Std_FLM
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Enables the selected FIFO
 * \param rif Specifies the pointer to the base of RIF registers
 * \param fifoId FIFO number
 * \return None
 */
IFX_INLINE void IfxRif_enableFifo(Ifx_RIF *rif, IfxRif_FifoId fifoId);

/** \brief Sets the FLM mode of operation
 * \param rif Specifies the pointer to the base of RIF registers
 * \param mode FLM mode of operation
 * \return None
 */
IFX_INLINE void IfxRif_setFlmMode(Ifx_RIF *rif, IfxRif_FlmMode mode);

/** \brief Sets the Full Swap Mode of FLM unit
 * \param rif Specifies the pointer to the base of RIF registers
 * \param swapMode Full Swap Mod eof FLM unit
 * \return None
 */
IFX_INLINE void IfxRif_setFullSwapMode(Ifx_RIF *rif, IfxRif_FullSwapMode swapMode);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Enables the number of FIFOs based on number of ADC channels used
 * \param rif Specifies the pointer to the base of RIF registers
 * \param count Number of ADC channels used
 * \return None
 */
IFX_EXTERN void IfxRif_enableFifos(Ifx_RIF *rif, uint8 count);

/** \} */

/** \addtogroup IfxLld_Rif_Std_DFU
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Returns the data length of the frame signal
 * \param rif Specifies the pointer to the base of RIF registers
 * \return Data length of the frame signal
 */
IFX_INLINE IfxRif_DataLength IfxRif_getDataLength(Ifx_RIF *rif);

/** \brief Sets the polarity of the clock signal on the clock input pins
 * \param rif Specifies the pointer to the base of RIF registers
 * \param polarity Polarity of the clock signal
 * \return None
 */
IFX_INLINE void IfxRif_setClockPolarity(Ifx_RIF *rif, IfxRif_ClockPolarity polarity);

/** \brief Sets the alignment of the data delivered to the SPU
 * \param rif Specifies the pointer to the base of RIF registers
 * \param alignment Alignment of the data delivered to the SPU
 * \return None
 */
IFX_INLINE void IfxRif_setDataAlignment(Ifx_RIF *rif, IfxRif_DataAlignment alignment);

/** \brief Sets the format of the data delivered to the SPU
 * \param rif Specifies the pointer to the base of RIF registers
 * \param format format of the data delivered to the SPU
 * \return None
 */
IFX_INLINE void IfxRif_setDataFormat(Ifx_RIF *rif, IfxRif_DataFormat format);

/** \brief Sets the data length of the frame signal
 * \param rif Specifies the pointer to the base of RIF registers
 * \param length Data length of the frame signal
 * \return None
 */
IFX_INLINE void IfxRif_setDataLength(Ifx_RIF *rif, IfxRif_DataLength length);

/** \brief Sets the polarity of the data signal 0 on the data input pins
 * \param rif Specifies the pointer to the base of RIF registers
 * \param polarity Polarity of the data signal
 * \return None
 */
IFX_INLINE void IfxRif_setDataPolarity0(Ifx_RIF *rif, IfxRif_DataPolarity polarity);

/** \brief Sets the polarity of the data signal 1 on the data input pins
 * \param rif Specifies the pointer to the base of RIF registers
 * \param polarity Polarity of the data signal
 * \return None
 */
IFX_INLINE void IfxRif_setDataPolarity1(Ifx_RIF *rif, IfxRif_DataPolarity polarity);

/** \brief Sets the polarity of the data signal 2 on the data input pins
 * \param rif Specifies the pointer to the base of RIF registers
 * \param polarity Polarity of the data signal
 * \return None
 */
IFX_INLINE void IfxRif_setDataPolarity2(Ifx_RIF *rif, IfxRif_DataPolarity polarity);

/** \brief Sets the polarity of the data signal 3 on the data input pins
 * \param rif Specifies the pointer to the base of RIF registers
 * \param polarity Polarity of the data signal
 * \return None
 */
IFX_INLINE void IfxRif_setDataPolarity3(Ifx_RIF *rif, IfxRif_DataPolarity polarity);

/** \brief Sets the polarity of the frame signal on the frame input pins
 * \param rif Specifies the pointer to the base of RIF registers
 * \param polarity Polarity of the frame signal
 * \return None
 */
IFX_INLINE void IfxRif_setFramePolarity(Ifx_RIF *rif, IfxRif_FramePolarity polarity);

/** \brief Sets the polarity of the RAMP1 signal
 * \param rif Specifies the pointer to the base of RIF registers
 * \param polarity Polarity of the RAMP1 signal
 * \return None
 */
IFX_INLINE void IfxRif_setRamp1SignalPolarity(Ifx_RIF *rif, IfxRif_Ramp1SignalPolarity polarity);

/** \brief Sets the the shift direction of the serial data, corresponding to the data bit on
 * the lsb position in the delivered parallel data
 * \param rif Specifies the pointer to the base of RIF registers
 * \param direction Shift direction
 * \return None
 */
IFX_INLINE void IfxRif_setShiftDirection(Ifx_RIF *rif, IfxRif_ShiftDirection direction);

/** \} */

/** \addtogroup IfxLld_Rif_Std_RSM
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Enables the external ADCs, the radar interface to accepts input from the external ADCs.
 * \param rif Specifies the pointer to the base of RIF registers
 * \return None
 */
IFX_INLINE void IfxRif_enableExternalAdc(Ifx_RIF *rif);

/** \brief Enables the internal ADCs, the radar interface to accepts input from the internal ADCs.
 * \param rif Specifies the pointer to the base of RIF registers
 * \return None
 */
IFX_INLINE void IfxRif_enableInternalAdc(Ifx_RIF *rif);

/** \brief Enables lockstep, synchronous delivery of ADC samples from two RIFs to two SPUs\n
 * (in case two RIF instances are available and used)
 * \param rif Specifies the pointer to the base of RIF registers
 * \param enabled Choise (enable / disable)
 * \return None
 */
IFX_INLINE void IfxRif_enableLockstep(Ifx_RIF *rif, boolean enabled);

/** \brief Enables the RAMP1 signal and disables the Frame Watchdog Timer\n
 * \param rif Specifies the pointer to the base of RIF registers
 * \param enabled Choise (enable / disable)
 * \return None
 */
IFX_INLINE void IfxRif_enableRamp1Signal(Ifx_RIF *rif, boolean enabled);

/** \brief Returns the current ramp number of the chirp
 * \param rif Specifies the pointer to the base of RIF registers
 * \return Current ramp number of the chirp
 */
IFX_INLINE uint16 IfxRif_getCurrentRampNumber(Ifx_RIF *rif);

/** \brief Returns the current valid data sample number of the chirp
 * \param rif Specifies the pointer to the base of RIF registers
 * \return Current valid data sample number of the chirp
 */
IFX_INLINE uint16 IfxRif_getCurrentValidDataSampleNumber(Ifx_RIF *rif);

/** \brief Sets the number of ramps per chirp in the range of 1 to 256 ramps
 * \param rif Specifies the pointer to the base of RIF registers
 * \param rampsNum Number of ramps
 * \return None
 */
IFX_INLINE void IfxRif_setChirpLength(Ifx_RIF *rif, uint8 rampsNum);

/** \brief Sets the source for the RAMP1 signal multiplexer
 * \param rif Specifies the pointer to the base of RIF registers
 * \param signal Source for the RAMP1 signal multiplexer
 * \return None
 */
IFX_INLINE void IfxRif_setRamp1SignalInput(Ifx_RIF *rif, IfxRif_Ramp1SignalInput signal);

/** \brief Sets the number of valid data samples per chirp in the range of 1 to 2048 samples
 * \param rif Specifies the pointer to the base of RIF registers
 * \param samplesNum Number of valid data samples
 * \return None
 */
IFX_INLINE void IfxRif_setValidDataSamplesNumber(Ifx_RIF *rif, uint16 samplesNum);

/** \} */

/** \addtogroup IfxLld_Rif_Std_Interrupt
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Clears the selected interrupt flag status
 * \param rif Specifies the pointer to the base of RIF registers
 * \param interrupt Type of interrupt
 * \return None
 */
IFX_INLINE void IfxRif_clearInterruptFlag(Ifx_RIF *rif, IfxRif_Interrupt interrupt);

/** \brief Disables the selected interrupt
 * \param rif Specifies the pointer to the base of RIF registers
 * \param interrupt Type of interrupt
 * \return None
 */
IFX_INLINE void IfxRif_disableInterrupt(Ifx_RIF *rif, IfxRif_Interrupt interrupt);

/** \brief Enables the selected interrupt
 * \param rif Specifies the pointer to the base of RIF registers
 * \param interrupt Type of interrupt
 * \return None
 */
IFX_INLINE void IfxRif_enableInterrupt(Ifx_RIF *rif, IfxRif_Interrupt interrupt);

/** \brief Returns the selected interrupt flag status
 * \param rif Specifies the pointer to the base of RIF registers
 * \param interrupt Type of interrupt
 * \return Status
 */
IFX_INLINE uint8 IfxRif_getInterruptFlagStatus(Ifx_RIF *rif, IfxRif_Interrupt interrupt);

/** \brief Sets the selected interrupt flag status
 * \param rif Specifies the pointer to the base of RIF registers
 * \param interrupt Type of interrupt
 * \return None
 */
IFX_INLINE void IfxRif_setInterruptFlag(Ifx_RIF *rif, IfxRif_Interrupt interrupt);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Returns the SRC pointer of RIF module ERR interrupt
 * \param rif Specifies the pointer to the base of RIF registers
 * \return SRC pointer of RIF module ERR interrupt
 */
IFX_EXTERN volatile Ifx_SRC_SRCR *IfxRif_getSrcPointerErr(Ifx_RIF *rif);

/** \brief Returns the SRC pointer of RIF module INT interrupt
 * \param rif Specifies the pointer to the base of RIF registers
 * \return SRC pointer of RIF module INT interrupt
 */
IFX_EXTERN volatile Ifx_SRC_SRCR *IfxRif_getSrcPointerInt(Ifx_RIF *rif);

/** \} */

/** \addtogroup IfxLld_Rif_Std_Monitor
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Disables the CRC.
 * \param rif Specifies the pointer to the base of RIF registers
 * \return None
 */
IFX_INLINE void IfxRif_disableCrc(Ifx_RIF *rif);

/** \brief Enables the CRC.
 * \param rif Specifies the pointer to the base of RIF registers
 * \return None
 */
IFX_INLINE void IfxRif_enableCrc(Ifx_RIF *rif);

/** \brief Sets the frame watchdog threshold, (the reload value for the watchdog timer) in the range of 0-255.
 * \param rif Specifies the pointer to the base of RIF registers
 * \param value Threshold value
 * \return None
 */
IFX_INLINE void IfxRif_setFrameWatchdogThreshold(Ifx_RIF *rif, uint16 value);

/** \} */

/** \addtogroup IfxLld_Rif_Std_Module
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Returns the status of module, enabled or disabled
 * \param rif Specifies the pointer to the base of RIF registers
 * \return Status (TRUE / FALSE)
 */
IFX_INLINE boolean IfxRif_isModuleEnabled(Ifx_RIF *rif);

/** \brief Returns the module's suspend state.
 * TRUE :if module is suspended.
 * FALSE:if module is not yet suspended.
 * \param rif Pointer to RIF module registers
 * \return Suspend status (TRUE / FALSE)
 */
IFX_INLINE boolean IfxRif_isModuleSuspended(Ifx_RIF *rif);

/** \brief Configure the Module to Hard/Soft suspend mode.
 * Note: The api works only when the OCDS is enabled and in Supervisor Mode. When OCDS is disabled the OCS suspend control is ineffective.
 * \param rif Pointer to RIF module registers
 * \param mode Module suspend mode
 * \return None
 */
IFX_INLINE void IfxRif_setSuspendMode(Ifx_RIF *rif, IfxRif_SuspendMode mode);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Disables the module (clears the disable request)
 * \param rif Specifies the pointer to the base of RIF registers
 * \return None
 */
IFX_EXTERN void IfxRif_disableModule(Ifx_RIF *rif);

/** \brief Enables the module (clears the disable request)
 * \param rif Specifies the pointer to the base of RIF registers
 * \return None
 */
IFX_EXTERN void IfxRif_enableModule(Ifx_RIF *rif);

/** \brief Returns the module index of the selected RIF module
 * \param rif Pointer to RIF module registers
 * \return RIF module register address
 */
IFX_EXTERN Ifx_RIF *IfxRif_getAddress(IfxRif_Index rif);

/** \brief Returns the module index of the selected RIF module
 * \param rif Specifies the pointer to the base of RIF registers
 * \return Module index
 */
IFX_EXTERN IfxRif_Index IfxRif_getIndex(Ifx_RIF *rif);

/** \} */

/** \addtogroup IfxLld_Rif_Std_LVDS
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Sets the Clock LVDS PAD control mode
 * \param rif Specifies the pointer to the base of RIF registers
 * \param value Clock LVDS PAD control
 * \return None
 */
IFX_INLINE void IfxRif_setClockLvdsPadControl(Ifx_RIF *rif, uint8 value);

/** \brief Sets the Miscellaneous Common LVDS PAD control mode
 * \param rif Specifies the pointer to the base of RIF registers
 * \param mode Miscellaneous Common LVDS PAD control
 * \return None
 */
IFX_INLINE void IfxRif_setCommonLvdsPadControl(Ifx_RIF *rif, IfxRif_CommonLvdsPadControl mode);

/** \brief Sets the DATA0 LVDS PAD control mode
 * \param rif Specifies the pointer to the base of RIF registers
 * \param value DATA0 LVDS PAD control
 * \return None
 */
IFX_INLINE void IfxRif_setData0LvdsPadControl(Ifx_RIF *rif, uint8 value);

/** \brief Sets the DATA1 LVDS PAD control mode
 * \param rif Specifies the pointer to the base of RIF registers
 * \param value DATA1 LVDS PAD control
 * \return None
 */
IFX_INLINE void IfxRif_setData1LvdsPadControl(Ifx_RIF *rif, uint8 value);

/** \brief Sets the DATA2 LVDS PAD control mode
 * \param rif Specifies the pointer to the base of RIF registers
 * \param value DATA2 LVDS PAD control
 * \return None
 */
IFX_INLINE void IfxRif_setData2LvdsPadControl(Ifx_RIF *rif, uint8 value);

/** \brief Sets the DATA3 LVDS PAD control mode
 * \param rif Specifies the pointer to the base of RIF registers
 * \param value DATA3 LVDS PAD control
 * \return None
 */
IFX_INLINE void IfxRif_setData3LvdsPadControl(Ifx_RIF *rif, uint8 value);

/** \brief Sets the Frame LVDS PAD control mode
 * \param rif Specifies the pointer to the base of RIF registers
 * \param value Frame LVDS PAD control
 * \return None
 */
IFX_INLINE void IfxRif_setFrameLvdsPadControl(Ifx_RIF *rif, uint8 value);

/** \brief enbales / disables the Lvds Bias Distributor 5V Mode
 * \param rif Specifies the pointer to the base of RIF registers
 * \param enabled enable / disable choise for LVDS Bias Distributor 5V mode
 * \return None
 */
IFX_INLINE void IfxRif_setLvdsBiasDistributor5VMode(Ifx_RIF *rif, boolean enabled);

/** \brief Sets the Lvds Bias Distributor Mode
 * \param rif Specifies the pointer to the base of RIF registers
 * \param mode LVDS Bias Distributor Power Down mode
 * \return None
 */
IFX_INLINE void IfxRif_setLvdsBiasDistributorPowerDownMode(Ifx_RIF *rif, IfxRif_LvdsBiasDistributorMode mode);

/** \brief Sets the Termination Resistor Trimming value
 * \param rif Specifies the pointer to the base of RIF registers
 * \param value Termination Resistor Trimming value
 * \return None
 */
IFX_INLINE void IfxRif_setRtermTrimmingValue(Ifx_RIF *rif, uint8 value);

/** \} */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/**
 * \param rif Specifies the pointer to the base of RIF registers
 * \param fifoId FIFO number
 * \return None
 */
IFX_EXTERN void IfxRif_disableFifo(Ifx_RIF *rif, IfxRif_FifoId fifoId);

/**
 * \param rif Specifies the pointer to the base of RIF registers
 * \return None
 */
IFX_EXTERN void IfxRif_disableAllFifos(Ifx_RIF *rif);

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxRif_clearInterruptFlag(Ifx_RIF *rif, IfxRif_Interrupt interrupt)
{
    uint32 shift    = (16 + interrupt);
    uint32 setValue = (1U << shift);
    rif->FLAGSCL.U = rif->FLAGSCL.U | (setValue);
}


IFX_INLINE void IfxRif_disableCrc(Ifx_RIF *rif)
{
    rif->FLM.B.CRCEN = 0U;
}


IFX_INLINE void IfxRif_disableDeserializer(Ifx_RIF *rif, IfxRif_DeserializerId deserializerId)
{
    uint32 setValue = 1U << (deserializerId + 16);
    rif->IPI.U = (rif->IPI.U & (~setValue));
}


IFX_INLINE void IfxRif_disableInterrupt(Ifx_RIF *rif, IfxRif_Interrupt interrupt)
{
    uint32 setValue = (1U << interrupt);
    rif->INTCON.U = rif->INTCON.U & (~setValue);
}


IFX_INLINE void IfxRif_enableCalibration(Ifx_RIF *rif)
{
    rif->ESI.B.CALEN = 1U;
}


IFX_INLINE void IfxRif_enableCrc(Ifx_RIF *rif)
{
    rif->FLM.B.CRCEN = 1U;
}


IFX_INLINE void IfxRif_enableDeserializer(Ifx_RIF *rif, IfxRif_DeserializerId deserializerId)
{
    uint32 setValue = 1U << (deserializerId + 16);
    rif->IPI.U = rif->IPI.U | setValue;
}


IFX_INLINE void IfxRif_enableExternalAdc(Ifx_RIF *rif)
{
    rif->RSM0.B.INTADC = 0U;
}


IFX_INLINE void IfxRif_enableFifo(Ifx_RIF *rif, IfxRif_FifoId fifoId)
{
    uint32 setValue = 1U << fifoId;
    rif->DMI.U = rif->DMI.U | setValue;
}


IFX_INLINE void IfxRif_enableInternalAdc(Ifx_RIF *rif)
{
    rif->RSM0.B.INTADC = 1U;
}


IFX_INLINE void IfxRif_enableInterrupt(Ifx_RIF *rif, IfxRif_Interrupt interrupt)
{
    uint32 setValue = 1U << interrupt;
    rif->INTCON.U = rif->INTCON.U | setValue;
}


IFX_INLINE void IfxRif_enableLockstep(Ifx_RIF *rif, boolean enabled)
{
    rif->RSM0.B.LCKSTP = enabled ? 1 : 0;
}


IFX_INLINE void IfxRif_enableRamp1Signal(Ifx_RIF *rif, boolean enabled)
{
    rif->RSM1.B.R1EN = enabled ? 1 : 0;
}


IFX_INLINE IfxRif_CalibrationState IfxRif_getCalibrationState(Ifx_RIF *rif)
{
    return (IfxRif_CalibrationState)rif->ESI.B.CALBSY;
}


IFX_INLINE IfxRif_CalibrationStatus IfxRif_getCalibrationStatus(Ifx_RIF *rif)
{
    return (IfxRif_CalibrationStatus)rif->ESI.B.CALSTAT;
}


IFX_INLINE uint16 IfxRif_getCurrentRampNumber(Ifx_RIF *rif)
{
    return (uint16)rif->RSM1.B.CURRAMP;
}


IFX_INLINE uint16 IfxRif_getCurrentValidDataSampleNumber(Ifx_RIF *rif)
{
    return (uint16)rif->RSM2.B.CURSAMPLE;
}


IFX_INLINE IfxRif_DataLength IfxRif_getDataLength(Ifx_RIF *rif)
{
    return (IfxRif_DataLength)rif->IPI.B.DL;
}


IFX_INLINE uint8 IfxRif_getInterruptFlagStatus(Ifx_RIF *rif, IfxRif_Interrupt interrupt)
{
    uint32 shift = (16 + interrupt);
    uint32 mask  = (1U << shift);
    return (rif->INTCON.U & mask) ? 1 : 0;
}


IFX_INLINE boolean IfxRif_isModuleEnabled(Ifx_RIF *rif)
{
    return rif->CLC.B.DISS == 0;
}


IFX_INLINE boolean IfxRif_isModuleSuspended(Ifx_RIF *rif)
{
    Ifx_RIF_OCS ocs;

    // read the status
    ocs.U = rif->OCS.U;

    // return the status
    return ocs.B.SUSSTA;
}


IFX_INLINE void IfxRif_setChirpLength(Ifx_RIF *rif, uint8 rampsNum)
{
    rif->RSM1.B.RAMPS = rampsNum;
}


IFX_INLINE void IfxRif_setClockLvdsPadControl(Ifx_RIF *rif, uint8 value)
{
    rif->LVDSCON0.B.CLK = value;
}


IFX_INLINE void IfxRif_setClockPolarity(Ifx_RIF *rif, IfxRif_ClockPolarity polarity)
{
    rif->ESI.B.CP = polarity;
}


IFX_INLINE void IfxRif_setCommonLvdsPadControl(Ifx_RIF *rif, IfxRif_CommonLvdsPadControl mode)
{
    rif->LVDSCON1.B.MISC = mode;
}


IFX_INLINE void IfxRif_setData0LvdsPadControl(Ifx_RIF *rif, uint8 value)
{
    rif->LVDSCON0.B.DATA0 = value;
}


IFX_INLINE void IfxRif_setData1LvdsPadControl(Ifx_RIF *rif, uint8 value)
{
    rif->LVDSCON0.B.DATA1 = value;
}


IFX_INLINE void IfxRif_setData2LvdsPadControl(Ifx_RIF *rif, uint8 value)
{
    rif->LVDSCON1.B.DATA2 = value;
}


IFX_INLINE void IfxRif_setData3LvdsPadControl(Ifx_RIF *rif, uint8 value)
{
    rif->LVDSCON1.B.DATA3 = value;
}


IFX_INLINE void IfxRif_setDataAlignment(Ifx_RIF *rif, IfxRif_DataAlignment alignment)
{
    rif->DFU.B.DA = alignment;
}


IFX_INLINE void IfxRif_setDataFormat(Ifx_RIF *rif, IfxRif_DataFormat format)
{
    rif->DFU.B.DF = format;
}


IFX_INLINE void IfxRif_setDataLength(Ifx_RIF *rif, IfxRif_DataLength length)
{
    rif->IPI.B.DL = length;
}


IFX_INLINE void IfxRif_setDataPolarity0(Ifx_RIF *rif, IfxRif_DataPolarity polarity)
{
    rif->ESI.B.DP0 = polarity;
}


IFX_INLINE void IfxRif_setDataPolarity1(Ifx_RIF *rif, IfxRif_DataPolarity polarity)
{
    rif->ESI.B.DP1 = polarity;
}


IFX_INLINE void IfxRif_setDataPolarity2(Ifx_RIF *rif, IfxRif_DataPolarity polarity)
{
    rif->ESI.B.DP2 = polarity;
}


IFX_INLINE void IfxRif_setDataPolarity3(Ifx_RIF *rif, IfxRif_DataPolarity polarity)
{
    rif->ESI.B.DP3 = polarity;
}


IFX_INLINE void IfxRif_setFlmMode(Ifx_RIF *rif, IfxRif_FlmMode mode)
{
    rif->FLM.B.MODE = mode;
}


IFX_INLINE void IfxRif_setFrameLvdsPadControl(Ifx_RIF *rif, uint8 value)
{
    rif->LVDSCON0.B.FRAME = value;
}


IFX_INLINE void IfxRif_setFramePolarity(Ifx_RIF *rif, IfxRif_FramePolarity polarity)
{
    rif->ESI.B.FP = polarity;
}


IFX_INLINE void IfxRif_setFrameWatchdogThreshold(Ifx_RIF *rif, uint16 value)
{
    rif->FWDG.B.THRESHOLD = value;
}


IFX_INLINE void IfxRif_setFullSwapMode(Ifx_RIF *rif, IfxRif_FullSwapMode swapMode)
{
    rif->FLM.B.FSWP = swapMode;
}


IFX_INLINE void IfxRif_setInterruptFlag(Ifx_RIF *rif, IfxRif_Interrupt interrupt)
{
    uint32 shift = (16 + interrupt);
    uint32 mask  = (1U << shift);
    rif->FLAGSSET.U = rif->FLAGSSET.U | mask;
}


IFX_INLINE void IfxRif_setLvdsBiasDistributor5VMode(Ifx_RIF *rif, boolean enabled)
{
    rif->LVDSCON1.B.LVDS5VEN = enabled ? 1 : 0;
}


IFX_INLINE void IfxRif_setLvdsBiasDistributorPowerDownMode(Ifx_RIF *rif, IfxRif_LvdsBiasDistributorMode mode)
{
    rif->LVDSCON1.B.PWRDN = mode;
}


IFX_INLINE void IfxRif_setRamp1SignalInput(Ifx_RIF *rif, IfxRif_Ramp1SignalInput signal)
{
    rif->RSM1.B.R1SEL = signal;
}


IFX_INLINE void IfxRif_setRamp1SignalPolarity(Ifx_RIF *rif, IfxRif_Ramp1SignalPolarity polarity)
{
    rif->RSM1.B.R1POL = polarity;
}


IFX_INLINE void IfxRif_setRtermTrimmingValue(Ifx_RIF *rif, uint8 value)
{
    rif->LVDSCON1.B.RTERM = value;
}


IFX_INLINE void IfxRif_setShiftDirection(Ifx_RIF *rif, IfxRif_ShiftDirection direction)
{
    rif->DFU.B.MSB = direction;
}


IFX_INLINE void IfxRif_setSuspendMode(Ifx_RIF *rif, IfxRif_SuspendMode mode)
{
    Ifx_RIF_OCS ocs;

    // remove protection and configure the suspend mode.
    ocs.B.SUS_P = 1;
    ocs.B.SUS   = mode;
    rif->OCS.U  = ocs.U;
}


IFX_INLINE void IfxRif_setValidDataSamplesNumber(Ifx_RIF *rif, uint16 samplesNum)
{
    rif->RSM2.B.SAMPLES = samplesNum;
}


#endif /* IFXRIF_H */
