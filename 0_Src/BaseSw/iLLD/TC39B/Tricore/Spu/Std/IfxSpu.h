/**
 * \file IfxSpu.h
 * \brief SPU  basic functionality
 * \ingroup IfxLld_Spu
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
 * \defgroup IfxLld_Spu_Std_Enum Enumerations
 * \ingroup IfxLld_Spu_Std
 * \defgroup IfxLld_Spu_Std_DataStructures Data Structures
 * \ingroup IfxLld_Spu_Std
 */

#ifndef IFXSPU_H
#define IFXSPU_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxSpu_cfg.h"
#include "IfxSpu_bf.h"
#include "Cpu/Std/IfxCpu_Intrinsics.h"
#include "_Utilities/Ifx_Assert.h"
#include "Scu/Std/IfxScuWdt.h"
#include "IfxSpu_reg.h"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Spu_Std_Enum
 * \{ */
/** \brief Bin Rejection mode.\n
 * Definition in IfxSPU_BINREJCTRL.RMODE
 */
typedef enum
{
    IfxSpu_BinRejection_Mode_off,     /**< \brief Pass all data */
    IfxSpu_BinRejection_Mode_reject,  /**< \brief Remove the selected bins from the output data */
    IfxSpu_BinRejection_Mode_zero     /**< \brief Set the selected bins to zero */
} IfxSpu_BinRejection_Mode;

/** \brief CA-CFAR algorithm selection\n
 * Definition in IfxSPU_CFARCFG.CAALGO
 */
typedef enum
{
    IfxSpu_CFAR_CA_Algorithm_cash,  /**< \brief CASH-CFAR */
    IfxSpu_CFAR_CA_Algorithm_ca,    /**< \brief CA-CFAR */
    IfxSpu_CFAR_CA_Algorithm_cago,  /**< \brief CAGO-CFAR */
    IfxSpu_CFAR_CA_Algorithm_caso   /**< \brief CASO-CFAR */
} IfxSpu_CFAR_CA_Algorithm;

/** \brief CFAR engine selection\n
 * Definition in IfxSPU_BEx_CFARCTRL.CFARSEL
 */
typedef enum
{
    IfxSpu_CFAR_Engine_ca,    /**< \brief Output the CA-CFAR engine result for inline CFAR mode */
    IfxSpu_CFAR_Engine_gos,   /**< \brief Output the GOS-CFAR Engine result for inline CFAR mode */
    IfxSpu_CFAR_Engine_both,  /**< \brief Both engines for inline CFAR mode */
    IfxSpu_CFAR_Engine_auto   /**< \brief Automatic selection for inline CFAR mode */
} IfxSpu_CFAR_Engine;

/** \brief GOS-CFAR algorithm selection\n
 * Definition in IfxSPU_CFARCFG.GOSALGO
 */
typedef enum
{
    IfxSpu_CFAR_GOS_Algorithm_gos,    /**< \brief GOS-CFAR */
    IfxSpu_CFAR_GOS_Algorithm_gosgo,  /**< \brief GOSGO-CFAR */
    IfxSpu_CFAR_GOS_Algorithm_gosso   /**< \brief GOSSO-CFAR */
} IfxSpu_CFAR_GOS_Algorithm;

/** \brief CFAR mode\n
 * Definition in IfxSPU_BEx_CFARCTRL.CFAREN
 */
typedef enum
{
    IfxSpu_CFAR_Mode_off,              /**< \brief Off */
    IfxSpu_CFAR_Mode_inline,           /**< \brief Inline CFAR */
    IfxSpu_CFAR_Mode_offline,          /**< \brief Offline CFAR */
    IfxSpu_CFAR_Mode_localMaxInline,   /**< \brief Inline Local Maximum */
    IfxSpu_CFAR_Mode_localMaxOffline,  /**< \brief Offline Local Maximum */
    IfxSpu_CFAR_Mode_nci,              /**< \brief CFAR on NCI output */
    IfxSpu_CFAR_Mode_localMaxNci       /**< \brief Threshold comparison based on NCI output */
} IfxSpu_CFAR_Mode;

/** \brief Spectrum extension for CFAR or threshold units\n
 * Definition in IfxSPU_BEx_CFARCTRL.EXTNSN
 */
typedef enum
{
    IfxSpu_CFAR_SpectrumExtension_off,      /**< \brief No spectrum extension */
    IfxSpu_CFAR_SpectrumExtension_range,    /**< \brief Range spectrum extension */
    IfxSpu_CFAR_SpectrumExtension_velocity  /**< \brief Velocity spectrum extension */
} IfxSpu_CFAR_SpectrumExtension;

/** \brief clock division ratio for the FFT engine\n
 * Definition in IfxSPU_CTRL.DIV
 */
typedef enum
{
    IfxSpu_ClockDivisionRatio_unity,  /**< \brief 1:1 clock ratio */
    IfxSpu_ClockDivisionRatio_div2,   /**< \brief 1:2 clock ratio */
    IfxSpu_ClockDivisionRatio_div4,   /**< \brief 1:4 clock ratio */
    IfxSpu_ClockDivisionRatio_div8    /**< \brief 1:8 clock ratio */
} IfxSpu_ClockDivisionRatio;

/** \brief Source of the data to be processed\n
 * Definition in IfxSPU_ID_CONF.SRC
 */
typedef enum
{
    IfxSpu_DataSource_rif0,     /**< \brief Data source RIF0 */
    IfxSpu_DataSource_rif1,     /**< \brief Data source RIF1 */
    IfxSpu_DataSource_rifBoth,  /**< \brief Data source RIF0 and RIF1 */
    IfxSpu_DataSource_emem      /**< \brief Data source Emem (radar memeory) */
} IfxSpu_DataSource;

/** \brief Radar memeory output data format \n
 * Definition in IfxSPU_BEx_UNLDR_CONF.FORMAT
 */
typedef enum
{
    IfxSpu_FftDataFormat_complex16Bit,  /**< \brief 16 bit complex data */
    IfxSpu_FftDataFormat_complex32Bit   /**< \brief 32 bit complex data */
} IfxSpu_FftDataFormat;

/** \brief Number of samples for the FFT\n
 * Definition in IfxSPU_BEx_LDR_CONF.SIZE
 */
typedef enum
{
    IfxSpu_FftSize_8 = 3,  /**< \brief FFT is eight data samples */
    IfxSpu_FftSize_16,     /**< \brief FFT is sixteen data samples */
    IfxSpu_FftSize_32,     /**< \brief FFT is thirty two data samples */
    IfxSpu_FftSize_64,     /**< \brief FFT is sixty four data samples */
    IfxSpu_FftSize_128,    /**< \brief FFT is one hundred and twenty eight data samples */
    IfxSpu_FftSize_256,    /**< \brief FFT is two hundred and fifty six data samples */
    IfxSpu_FftSize_512,    /**< \brief FFT is five hundred and twelve data samples */
    IfxSpu_FftSize_1024,   /**< \brief FFT is one thousand and twenty four data samples */
    IfxSpu_FftSize_2048    /**< \brief FFT is two thousand and forty eigth data samples */
} IfxSpu_FftSize;

/** \brief Power values per histogram bin
 */
typedef enum
{
    IfxSpu_Histogram_Bins_32   = 0,  /**< \brief HistogramBins 32 */
    IfxSpu_Histogram_Bins_64   = 1,  /**< \brief HistogramBins 64 */
    IfxSpu_Histogram_Bins_128  = 2,  /**< \brief HistogramBins 128 */
    IfxSpu_Histogram_Bins_256  = 3,  /**< \brief HistogramBins 256 */
    IfxSpu_Histogram_Bins_512  = 4,  /**< \brief HistogramBins 512 */
    IfxSpu_Histogram_Bins_1024 = 5,  /**< \brief HistogramBins 1024 */
    IfxSpu_Histogram_Bins_2048 = 6,  /**< \brief HistogramBins 2048 */
    IfxSpu_Histogram_Bins_4096 = 7   /**< \brief HistogramBins 4096 */
} IfxSpu_Histogram_Bins;

/** \brief Input data format (real or complex)\n
 * Definition in IfxSPU_ID_CONF.FORMAT
 */
typedef enum
{
    IfxSpu_InputDataFormat_real,    /**< \brief Input data is real number */
    IfxSpu_InputDataFormat_complex  /**< \brief Input data has real and imaginary components */
} IfxSpu_InputDataFormat;

/** \brief Input data type (signed or unsigned)\n
 * Definition in IfxSPU_ID_CONF.SIGNED
 */
typedef enum
{
    IfxSpu_InputDataType_unsigned,  /**< \brief Unsigned input data */
    IfxSpu_InputDataType_signed     /**< \brief Signed input data */
} IfxSpu_InputDataType;

/** \brief Check combine mode.\n
 * Definition in IfxSPU_LCLMAX.CMBN
 */
typedef enum
{
    IfxSpu_LocalMax_CombinedMode_or,  /**< \brief OR check results */
    IfxSpu_LocalMax_CombinedMode_and  /**< \brief AND check results */
} IfxSpu_LocalMax_CombinedMode;

/** \brief Local maximum mode.\n
 * Definition in IfxSPU_LCLMAX.LMODE
 */
typedef enum
{
    IfxSpu_LocalMax_Mode_off,    /**< \brief Off */
    IfxSpu_LocalMax_Mode_under,  /**< \brief Reject bin if not local maximum */
    IfxSpu_LocalMax_Mode_over    /**< \brief Reject bin if local maximum */
} IfxSpu_LocalMax_Mode;

/** \brief Local maximum mode.\n
 * Definition in IfxSPU_LCLMAX.TMODE
 */
typedef enum
{
    IfxSpu_LocalMax_ThresholdMode_off,    /**< \brief Off */
    IfxSpu_LocalMax_ThresholdMode_under,  /**< \brief Reject bin if under threshold */
    IfxSpu_LocalMax_ThresholdMode_over    /**< \brief Reject bin if over threshold */
} IfxSpu_LocalMax_ThresholdMode;

/** \brief width of the comparison window for the Local Maximum detection.\n
 * Definition in IfxSPU_LCLMAX.WIDTH
 */
typedef enum
{
    IfxSpu_LocalMax_WindowWidth_off,    /**< \brief No local maximum */
    IfxSpu_LocalMax_WindowWidth_three,  /**< \brief On, with window size of three */
    IfxSpu_LocalMax_WindowWidth_five    /**< \brief On, with window size of five */
} IfxSpu_LocalMax_WindowWidth;

/** \brief Precision of the output data written to the Radar Memory from Non-coherent intergration\n
 * Definition in IfxSPU_BEx_NCICTRL.FORMAT
 */
typedef enum
{
    IfxSpu_NCI_OutputFormat_off,        /**< \brief Output channel is off */
    IfxSpu_NCI_OutputFormat_real16Bit,  /**< \brief 16 bit data output (real) */
    IfxSpu_NCI_OutputFormat_real32Bit   /**< \brief 32 bit data output (real) */
} IfxSpu_NCI_OutputFormat;

/** \brief Scaling factor applied to the results from Non-coherent intergration\n
 * Definition in IfxSPU_BEx_NCICTRL.SCALE
 */
typedef enum
{
    IfxSpu_NCI_ScalingFactor_off,   /**< \brief Output channel is off */
    IfxSpu_NCI_ScalingFactor_div2,  /**< \brief Divide result by 2 */
    IfxSpu_NCI_ScalingFactor_div4,  /**< \brief Divide result by 4 */
    IfxSpu_NCI_ScalingFactor_div8   /**< \brief Divide result by 8 */
} IfxSpu_NCI_ScalingFactor;

/** \brief Number of Antennae connected to each RIF instance\n
 * Definition in IfxSPU_ID_CONF.ANT
 */
typedef enum
{
    IfxSpu_Num_Antennae_1 = 1,  /**< \brief 1 antenna is connected */
    IfxSpu_Num_Antennae_2,      /**< \brief 2 antennae are connected  */
    IfxSpu_Num_Antennae_3,      /**< \brief 3 antennae are connected  */
    IfxSpu_Num_Antennae_4       /**< \brief 4 antennae are connected  */
} IfxSpu_Num_Antennae;

/** \brief Processing instance number to choose the register instance for base engine (BEx)
 */
typedef enum
{
    IfxSpu_PassId_0,  /**< \brief First processing instance */
    IfxSpu_PassId_1   /**< \brief Second processing instance */
} IfxSpu_PassId;

/** \brief Double pass configuration mode\n
 * Definition in IfxSPU_DPASS_CONF
 */
typedef enum
{
    IfxSpu_PassMode_single,    /**< \brief Double pass mode disable */
    IfxSpu_PassMode_double,    /**< \brief Double pass mode enabled without buffer memory switch */
    IfxSpu_PassMode_alternate  /**< \brief Double pass mode enabled with buffer memory switch */
} IfxSpu_PassMode;

/** \brief Phase shift\n
 * Definition in IfxSPU_BEx_LDR_CONF.PHSHFT
 */
typedef enum
{
    IfxSpu_PhaseShift_0,    /**< \brief No phase shift */
    IfxSpu_PhaseShift_90,   /**< \brief 90 degree phase shift  */
    IfxSpu_PhaseShift_180,  /**< \brief 180 degree phase shift  */
    IfxSpu_PhaseShift_270   /**< \brief 270 degree phase shift  */
} IfxSpu_PhaseShift;

/** \brief Operating mode of the Input DMA\n
 * Definition in IfxSPU_ID_RM_CONF.TRNSPS
 */
typedef enum
{
    IfxSpu_RM_AddressingMode_linear,    /**< \brief Linear mode */
    IfxSpu_RM_AddressingMode_transpose  /**< \brief Transpose mode */
} IfxSpu_RM_AddressingMode;

/** \brief Defines how to identify each FFT dataset with a particular antenna\n
 * Definition in IfxSPU_ID_RM_CONF.AM
 */
typedef enum
{
    IfxSpu_RM_AntennaMapping_default,          /**< \brief Antenna ID passed to processing chain is permanently 0d */
    IfxSpu_RM_AntennaMapping_indexMode,        /**< \brief Antenna ID is derived from the dataset index */
    IfxSpu_RM_AntennaMapping_innerLoopRepeat,  /**< \brief Antenna ID is derived from the inner loop repeat counter value */
    IfxSpu_RM_AntennaMapping_outerLoopRepeat,  /**< \brief Antenna ID is derived from the outer loop repeat counter value */
    IfxSpu_RM_AntennaMapping_binLoopRepeat     /**< \brief Antenna ID is derived from the bin loop repeat counter value */
} IfxSpu_RM_AntennaMapping;

/** \brief Radar memeory input data format \n
 * Definition in IfxSPU_ID_RM_CONF.FORMAT
 */
typedef enum
{
    IfxSpu_RM_InputDataFormat_complex16Bit     = 0, /**< \brief 16 bit precision complex data */
    IfxSpu_RM_InputDataFormat_complex32Bit     = 1, /**< \brief 32 bit precision complex data */
    IfxSpu_RM_InputDataFormat_power32Bit       = 2, /**< \brief 32 bit precision power data */
    IfxSpu_RM_InputDataFormat_real16Bit        = 4, /**< \brief 16 bit precision real data */
    IfxSpu_RM_InputDataFormat_real32Bit        = 5, /**< \brief 32 bit precision real data */
    IfxSpu_RM_InputDataFormat_realHalfFloat    = 6, /**< \brief half-precision floating-point real data */
    IfxSpu_RM_InputDataFormat_complexHalfFloat = 7  /**< \brief half-precision floating-point complex data */
} IfxSpu_RM_InputDataFormat;

/** \brief Processing mode of the Input DMA\n
 * Definition in IfxSPU_ID_RM_CONF.PM
 */
typedef enum
{
    IfxSpu_RM_ProcessingMode_default,     /**< \brief Default mode */
    IfxSpu_RM_ProcessingMode_integration  /**< \brief Integration mode */
} IfxSpu_RM_ProcessingMode;

/** \brief Calculations Using Complex Data to use the real component only or the complex component also\n
 * Definition in IfxSPU_BEx_SUMCTRL.REAL
 */
typedef enum
{
    IfxSpu_Summation_DataFormat_complex,  /**< \brief Calculations will use real and complex components */
    IfxSpu_Summation_DataFormat_real      /**< \brief Calculations will use the real component of the input data only. The
                                           * complex component will be discarded */
} IfxSpu_Summation_DataFormat;

/** \brief Operating mode of the Summation Unit in the power domain\n
 * Definition in IfxSPU_BEx_SUMCTRL.PWRMODE
 */
typedef enum
{
    IfxSpu_Summation_PowerMode_off,  /**< \brief Off */
    IfxSpu_Summation_PowerMode_sum   /**< \brief Sum */
} IfxSpu_Summation_PowerMode;

/** \brief OCDS Suspend Control (OCDS.SUS)
 */
typedef enum
{
    IfxSpu_SuspendMode_none = 0,  /**< \brief No suspend */
    IfxSpu_SuspendMode_hard = 1,  /**< \brief Hard Suspend */
    IfxSpu_SuspendMode_soft = 2   /**< \brief Soft Suspend */
} IfxSpu_SuspendMode;

/** \brief SPU trigger mode\n
 * Definition in IfxSPU_CTRL.MODE
 */
typedef enum
{
    IfxSpu_TriggerMode_off,       /**< \brief SPU is disabled */
    IfxSpu_TriggerMode_internal,  /**< \brief Internal trigger */
    IfxSpu_TriggerMode_external,  /**< \brief External trigger */
    IfxSpu_TriggerMode_spu0Done,  /**< \brief Trigger on SPU0 done */
    IfxSpu_TriggerMode_spu1Done,  /**< \brief Trigger on SPU1 done */
    IfxSpu_TriggerMode_reload,    /**< \brief Reload configuration from memory with no processing */
    IfxSpu_TriggerMode_software,  /**< \brief Software trigger */
    IfxSpu_TriggerMode_stop       /**< \brief SPU will stop at end of current operation */
} IfxSpu_TriggerMode;

/** \brief Format of the Window Function Data stored in the configuration memeory\n
 * Definition in IfxSPU_BEx_LDR_CONF.FORMAT
 */
typedef enum
{
    IfxSpu_WindowDataFormat_real16,     /**< \brief 16 bit real window data */
    IfxSpu_WindowDataFormat_real32,     /**< \brief 32 bit real window data */
    IfxSpu_WindowDataFormat_complex16,  /**< \brief 16 bit complex window data */
    IfxSpu_WindowDataFormat_complex32   /**< \brief 32 bit complex window data */
} IfxSpu_WindowDataFormat;

/** \} */

/** \brief Output Data Processor  scaling factor
 * Definition in IfxSPU_BEx_ODP_CONF.SCALE
 */
typedef enum
{
    IfxSpu_ODP_Format_complex32Bit,      /**< \brief Output is written with 32 bit precision complex */
    IfxSpu_ODP_Format_real32Bit,         /**< \brief Output is written with 32 bit precision real */
    IfxSpu_ODP_Format_complex16Bit,      /**< \brief Output is written with 16 bit precision complex */
    IfxSpu_ODP_Format_real16Bit,         /**< \brief Output is written with 16 bit precision real */
    IfxSpu_ODP_Format_complexHalfFloat,  /**< \brief Output is written with half-precision floating-point complex */
    IfxSpu_ODP_Format_realHalfFloat      /**< \brief Output is written with half-precision floating-point precision real */
} IfxSpu_ODP_Format;

/** \brief Operating mode of the Summation Unit on complex or magnitude data
 * Definition in IfxSPU_BEx_SUMCTRL.SUMMODE
 */
typedef enum
{
    IfxSpu_Summation_Mode_off,      /**< \brief Off */
    IfxSpu_Summation_Mode_sum,      /**< \brief Sum */
    IfxSpu_Summation_Mode_sumLinP,  /**< \brief Sum linear power */
    IfxSpu_Summation_Mode_sumAnt    /**< \brief Sum across antenna */
} IfxSpu_Summation_Mode;

/** \brief Definition in IfxSPU_BEx_PWRSUM.FORMAT
 */
typedef enum
{
    IfxSpu_Summation_Precision_32bit,  /**< \brief 32 bit precision */
    IfxSpu_Summation_Precision_16bit   /**< \brief 16-bit precision */
} IfxSpu_Summation_Precision;

/** \brief Definition in IfxSPU_BEx_PWRSUM.SCALE
 */
typedef enum
{
    IfxSpu_Summation_Scale_off,
    IfxSpu_Summation_Scale_div2,
    IfxSpu_Summation_Scale_div4,
    IfxSpu_Summation_Scale_div8
} IfxSpu_Summation_Scale;

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Spu_Std_DataStructures
 * \{ */
/** \brief Configuration structure for Windowing
 */
typedef struct
{
    boolean                 enabled;                                   /**< \brief Enable / Disable window function */
    IfxSpu_WindowDataFormat dataFormat;                                /**< \brief Format of the Window Function Data stored in the configuration memeory */
    uint16                  baseAddress;                               /**< \brief Window coefficient base address */
    uint16                  antennaOffsets[IFXSPU_MAX_ANTENNAE];       /**< \brief Offset Addresses for each antenna */
} IfxSpu_WindowingConfig;

/** \} */

/** \addtogroup IfxLld_Spu_Std_DataStructures
 * \{ */
/** \brief Configuration structure for CA-CFAR algorithm
 */
typedef struct
{
    IfxSpu_CFAR_CA_Algorithm algorithm;                   /**< \brief CA-CFAR algorithm selection */
    uint8                    guardCells;                  /**< \brief Number of guard cells in CA-CFAR leading and lagging the cell under test */
    uint8                    windowCellsExponent;         /**< \brief Number of active cells in leading/lagging windows to be averaged in CA-CFAR */
    uint8                    cashSubWindowExponent;       /**< \brief Number of sub window cells CASH-CFAR */
    uint16                   beta;                        /**< \brief Additive constant scaling the CA-CFAR theshold */
} IfxSpu_CfarCAConfig;

/** \brief Configuration structure for CFAR module control
 */
typedef struct
{
    IfxSpu_CFAR_Mode              mode;                        /**< \brief CFAR mode */
    uint32                        baseAddress;                 /**< \brief Base address to be used when writing data to Radar Memory */
    IfxSpu_CFAR_SpectrumExtension spectrumExtension;           /**< \brief Spectrum extension for CFAR or threshold units */
    uint8                         spectrumExtensionSize;       /**< \brief Window size to be used for spectrum extension */
    boolean                       caCfarEnabled;
    boolean                       gosCfarEnabled;
} IfxSpu_CfarConfig;

/** \brief Configuration structure for GOS-CFAR algorithm
 */
typedef struct
{
    IfxSpu_CFAR_GOS_Algorithm algorithm;           /**< \brief GOS-CFAR algorithm selection */
    uint8                     guardCells;          /**< \brief Number of guard cells in GOS-CFAR leading and lagging the cell under test */
    uint8                     indexLead;           /**< \brief Index of sorted statistic in leading window in GOS-CFAR */
    uint8                     indexLag;            /**< \brief Index of sorted statistic in lagging window in GOS-CFAR */
    uint8                     windowCells;         /**< \brief Number of active cells in leading/lagging windows to be averaged in GOS-CFAR */
    uint16                    beta;                /**< \brief Additive constant scaling the CA-CFAR theshold */
    uint16                    addressOffset;       /**< \brief Channel 5 Address Offset in bytes */
} IfxSpu_CfarGOSConfig;

/** \brief Configuration structure for Streaming FFT
 */
typedef struct
{
    boolean              enabled;           /**< \brief Enable / Disable FFT processing */
    boolean              inversed;          /**< \brief Enable / Disable inverse FFT */
    IfxSpu_FftSize       size;              /**< \brief Number of FFT points */
    IfxSpu_FftDataFormat dataFormat;        /**< \brief Format of the data to be written to the buffer memory */
    boolean              forceToReal;       /**< \brief Force the output to Real enable / disable choice */
    uint8                exponent;          /**< \brief Alignment correction to be applied when reformatting to 16 bit */
} IfxSpu_FftConfig;

/** \brief Configuration structure for FFT output
 */
typedef struct
{
    boolean           enabled;           /**< \brief Major Operating Mode for the ODP, ON / OFF */
    uint32            baseAddress;       /**< \brief The base address to be used when writing data to Radar Memory */
    IfxSpu_ODP_Format format;            /**< \brief Output Data Processor  scaling factor */
    uint16            exponent;          /**< \brief The number of LSBs to be removed when writing 16 bit precision data to the Radar Memory */
    boolean           inPlace;           /**< \brief In Place Fft enable / disable choice */
} IfxSpu_FftOutputConfig;

/** \brief Configuration structure for Histogram
 */
typedef struct
{
    boolean               enabled;                    /**< \brief Enable / Disable choice for Histogram */
    IfxSpu_Histogram_Bins numClasses;                 /**< \brief Number of power values per histogram bin */
    uint32                baseAddress;                /**< \brief Base address to be used Histogram */
    uint16                startDelay;                 /**< \brief Delay from Start of Measurement Cycle before the accumulation of histogram is started. */
    uint16                endDelay;                   /**< \brief Delay from Start of Measurement Cycle before the accumulation of histogram is ended. */
    boolean               antennaFilterEnabled;       /**< \brief Enable / Disable choice for Antenna Filter */
    uint8                 antennaFilterMask;          /**< \brief Antenna Filter mask */
} IfxSpu_HistogramConfig;

/** \brief Configuration structure for Math1 unit
 */
typedef struct
{
    uint8                  loaderExponent;            /**< \brief Alignment correction to be applied when reformatting to 32 bit */
    uint8                  numDropLastSamples;        /**< \brief Number of last samples of the input data to be ignored */
    uint8                  numDropFirstSamples;       /**< \brief Number of first samples of the input data to be ignored */
    uint16                 numPadFrontSamples;        /**< \brief Number of  samples to be padded at front */
    IfxSpu_WindowingConfig window;                    /**< \brief Windowing configuration */
    IfxSpu_PhaseShift      phaseShift;                /**< \brief Fast phase shift */
} IfxSpu_Math1Config;

/** \brief Configuration structure for Non-Coherent Integration control
 */
typedef struct
{
    boolean                  enabled;             /**< \brief Enable / Disable choice for Non-Coherent Integration */
    uint32                   baseAddress;         /**< \brief Base address to be used when writing data to Radar Memory */
    IfxSpu_NCI_OutputFormat  outputFormat;        /**< \brief Precision of the output data written to the Radar Memory from Non-coherent intergration */
    IfxSpu_NCI_ScalingFactor scalingFactor;       /**< \brief Scaling factor applied to the results from Non-coherent intergration */
} IfxSpu_NciConfig;

/** \brief Configuration structure for Power Information Channel
 */
typedef struct
{
    boolean enabled;           /**< \brief Enable / Disable choice for Power Information Channel */
    uint32  baseAddress;       /**< \brief Base address to be used when writing data to Radar Memory */
} IfxSpu_PowerOutputConfig;

/** \brief Configuration structure for RIF input data
 */
typedef struct
{
    IfxSpu_InputDataFormat dataFormat;                         /**< \brief Input data format (real or complex) */
    IfxSpu_InputDataType   dataType;                           /**< \brief Input data type (signed or unsigned) */
    uint16                 numRamps;                           /**< \brief ramps per messurement cycle */
    IfxSpu_Num_Antennae    numAntennae;                        /**< \brief Number of Antennae connected to each RIF instance */
    uint16                 sampleCount;                        /**< \brief Number of samples per ramp */
    boolean                preAcquisitionCounterEnabled;       /**< \brief Enable / Disable Preacquisition counter */
    uint8                  preAcquisitionCounterLimit;         /**< \brief Preacquisition counter limit */
    boolean                bypassModeEnabled;                  /**< \brief Enable / Disable Bypass mode */
    uint32                 bypassAddress;                      /**< \brief Bypass address */
    boolean                bypassReloadEnabled;                /**< \brief Enable / Disable Bypass reload */
} IfxSpu_RifConfig;

/** \brief Configuration structure for Radar Memory input data
 */
typedef struct
{
    IfxSpu_RM_InputDataFormat dataFormat;            /**< \brief Radar memeory input data format */
    uint32                    baseAddress;           /**< \brief Radar memeory base address */
    uint32                    innerLoopOffset;       /**< \brief Inner loop offset address */
    uint32                    outerLoopOffset;       /**< \brief Outer loop offset address */
    uint32                    binLoopOffset;         /**< \brief Bin loop offset address */
    uint16                    innerLoopRepeat;       /**< \brief Inner loop repeat value */
    uint16                    outerLoopRepeat;       /**< \brief Outer loop repeat value */
    uint16                    binLoopRepeat;         /**< \brief Bin loop repeat value */
    IfxSpu_RM_AddressingMode  addressingMode;        /**< \brief Operating mode of the Input DMA */
    IfxSpu_RM_ProcessingMode  processingMode;        /**< \brief Processing mode of the Input DMA */
    uint8                     numBlocks;             /**< \brief Number of data blocks that will fit into the buffer memory */
    IfxSpu_RM_AntennaMapping  antennaMapping;        /**< \brief Mapping of FFT datasets to antenna */
} IfxSpu_RmConfig;

/** \brief Configuration structure for Sideband Statistics
 */
typedef struct
{
    boolean enabled;           /**< \brief Enable / Disable choice for Sideband Statistics */
    uint32  baseAddress;       /**< \brief Base address to be used when writing data to Radar Memory */
} IfxSpu_SidebandStatisticsConfig;

/** \brief Configuration structure for Summation unit control
 */
typedef struct
{
    uint32                      baseAddress;         /**< \brief Base address to be used when writing data to Radar Memory */
    IfxSpu_Summation_Mode       mode;                /**< \brief Operating mode of the Summation Unit on complex or magnitude data */
    IfxSpu_Summation_DataFormat dataFormat;          /**< \brief Calculations Using Complex Data to use the real component only or the complex component also */
    uint8                       antennaeToUse;       /**< \brief Selection of which antennae to use */
    IfxSpu_Summation_Scale      shift;               /**< \brief Normalize CSUM result by dividing the result */
    IfxSpu_Summation_Precision  precision;           /**< \brief Output precision format */
} IfxSpu_SummationConfig;

/** \} */

/** \addtogroup IfxLld_Spu_Std_DataStructures
 * \{ */
/** \brief Configuration structure for Bin Rejection unit
 */
typedef struct
{
    IfxSpu_BinRejection_Mode mode;                   /**< \brief Bin rejection mode */
    boolean                  thresholdEnabled;       /**< \brief Enable / Disable choice for Zero with Threshold */
    uint32                   thresholdValue;         /**< \brief Magnitude value to be used for comparison */
    uint16                   numAllowedBins;         /**< \brief Number of passed bins (i.e. non-rejected), starting from the first */
    boolean                  leftJustify;            /**< \brief Define whether the threshold value will be left justified to 24-bit */
    IfxSpu_CFAR_Engine       inlineCfarSelect;       /**< \brief Inline CFAR engine selection */
} IfxSpu_BinRejectionConfig;

/** \brief Configuration structure for FFT input
 */
typedef struct
{
    IfxSpu_DataSource dataSource;       /**< \brief Source of the data to be processed */
    IfxSpu_RifConfig  rif;              /**< \brief Configuration structure for RIF input data */
    IfxSpu_RmConfig   rm;               /**< \brief Configuration structure for Radar Memory input data */
} IfxSpu_FftInputConfig;

/** \brief Configuration structure for interrupts
 */
typedef struct
{
    boolean powerDataWrite;             /**< \brief Enable/Disable the attention request, End of power data write from Output DMA engine */
    boolean complexDataWrite;           /**< \brief Enable/Disable the attention request, End of complex data write from Output DMA engine */
    boolean inputDmaUnit;               /**< \brief Enable/Disable the attention request, input DMA unit */
    boolean linkedListEnd;              /**< \brief Enable/Disable the attention request, End of linked list */
    boolean pactrTrigger;               /**< \brief Enable/Disable the attention request, partial acquisition counter trigger */
    boolean anyStatus;                  /**< \brief Enable/Disable the attention request, Any status event */
    boolean inputDmaReadError;          /**< \brief Enable/Disable the Error flag, Input DMA unit read error */
    boolean inputDmaWriteError;         /**< \brief Enable/Disable the Error flag, Input DMA unit write error */
    boolean outputDmaEngineError;       /**< \brief Enable/Disable the Error flag, Output DMA Engine error */
    boolean fifoOverrun;                /**< \brief Enable/Disable the Error flag, FIFO overrun */
    boolean pactrError;                 /**< \brief Enable/Disable the Error flag, Partial acquisition counter error */
    boolean math2UnitOverrun;           /**< \brief Enable/Disable the Error flag, MATH2 uint overrun */
} IfxSpu_InterruptConfig;

/** \brief Configuration structure for Local Maximum unit
 */
typedef struct
{
    IfxSpu_LocalMax_WindowWidth   windowWidth;          /**< \brief Local Max window width */
    IfxSpu_LocalMax_Mode          localMaxMode;         /**< \brief Local Max mode */
    IfxSpu_LocalMax_ThresholdMode thresholdMode;        /**< \brief Threshold mode */
    IfxSpu_LocalMax_CombinedMode  combinedMode;         /**< \brief Combine operation between result of localMaxMode and thresholdMode */
    uint32                        thresholdValue;       /**< \brief Threshold Value to be Used for Bin Magnitude Comparison */
    boolean                       leftJustify;          /**< \brief Selects if comparison to be made against the 24 MSBs or 24 LSBs */
} IfxSpu_LocalMaxConfig;

/** \brief Configuration structure for Magnitude approximation
 */
typedef struct
{
    uint16 alpha;       /**< \brief Alpha */
    uint16 beta;        /**< \brief Beta */
} IfxSpu_MagnitudeConfig;

/** \brief Configuration Structure for Pass mode registers
 */
typedef struct
{
    IfxSpu_FftConfig                fft;              /**< \brief Configuration structure for Streaming FFT */
    IfxSpu_Math1Config              math1;            /**< \brief Configuration structure for Math1 unit */
    IfxSpu_HistogramConfig          histogram;        /**< \brief Configuration structure for Histogram */
    IfxSpu_NciConfig                nci;              /**< \brief Configuration structure for Non-Coherent Integration control */
    IfxSpu_SummationConfig          csum;             /**< \brief Configuration structure for Summation unit control */
    IfxSpu_CfarConfig               cfar;             /**< \brief Configuration structure for CFAR module control */
    IfxSpu_CfarCAConfig             cfarCA;           /**< \brief Configuration structure for CFAR CA */
    IfxSpu_CfarGOSConfig            cfarGOS;          /**< \brief Configuration structure for CFAR GOS */
    IfxSpu_PowerOutputConfig        power;            /**< \brief Configuration structure for Power Information Channel */
    IfxSpu_FftOutputConfig          fftOut;           /**< \brief Configuration structure for FFT output */
    IfxSpu_SidebandStatisticsConfig statistics;       /**< \brief Configuration structure for Sideband Statistics */
} IfxSpu_PassConfig;

/** \} */

/** \addtogroup IfxLld_Spu_Std_DataStructures
 * \{ */
/** \brief SFRs which are related to linked-list configuration
 */
typedef struct
{
    Ifx_SPU_ID         ID;                 /**< \brief Input DMA Configuration Registers */
    Ifx_SPU_PACTR      PACTR;              /**< \brief Pre-Acquisition Counter */
    Ifx_SPU_DPASS_CONF DPASS_CONF;         /**< \brief Double Pass Configuration */
    Ifx_SPU_BE         BE[2];              /**< \brief SPU BE object */
    Ifx_SPU_BIN_REJ    BIN_REJ[64];        /**< \brief Bin Rejection Mask */
    Ifx_SPU_MAGAPPROX  MAGAPPROX;          /**< \brief Magnitude Approximation Constants */
    Ifx_SPU_NCI        NCI;                /**< \brief NCI Antennae Scaling Factors Registers */
    Ifx_SPU_CFAR       CFAR;               /**< \brief CFAR Configuration */
    Ifx_SPU_SCALARADD  SCALARADD;          /**< \brief Scalar Addition Operand */
    Ifx_SPU_SCALARMULT SCALARMULT;         /**< \brief Scalar Multiplication Operand */
    Ifx_SPU_BINREJCTRL BINREJCTRL;         /**< \brief Bin Rejection Unit Control */
    Ifx_SPU_LCLMAX     LCLMAX;             /**< \brief Local Maximum Control */
    Ifx_SPU_ACFG2      ACFG2;              /**< \brief Spare Configuration Register */
    uint32             reserved;           /**< \brief reserved */
    Ifx_SPU_REGCRC     REGCRC;             /**< \brief Register CRC */
    Ifx_SPU_CTRL       CTRL;               /**< \brief SPU Control */
    uint32             reserved1[4];
} IfxSpu_ConfigSlot;

/** \brief Configuration structure of the SPU module initialisation
 */
typedef struct
{
    IfxSpu_ClockDivisionRatio clockDivisionRatio;       /**< \brief clock division ratio for the FFT engine */
    IfxSpu_InterruptConfig    interrupt;                /**< \brief Configuration structure for enabling/disabling interrupt event */
} IfxSpu_ModuleConfig;

/** \brief Configuration structure for SPU
 */
typedef struct
{
    IfxSpu_FftInputConfig     input;                                 /**< \brief Configuration structure for FFT input */
    IfxSpu_CfarCAConfig       cfarCA;                                /**< \brief Configuration structure for CA-CFAR algorithm */
    IfxSpu_CfarGOSConfig      cfarGOS;                               /**< \brief Configuration structure for GOS-CFAR algorithm */
    IfxSpu_LocalMaxConfig     localMax;                              /**< \brief Configuration structure for Local Maximum unit */
    IfxSpu_BinRejectionConfig binrej;                                /**< \brief Configuration structure for Bin Rejection unit */
    uint16                    nciWeights[IFXSPU_MAX_ANTENNAE];       /**< \brief Antennae scaling factors */
    IfxSpu_MagnitudeConfig    magnitude;                             /**< \brief Configuration structure for Magnitude approximation */
    IfxSpu_PassConfig         pass[2];                               /**< \brief Configuration Structure for Pass mode registers */
} IfxSpu_SlotConfig;

/** \brief Structure for statistical output
 */
typedef struct
{
    uint32 minPwr;          /**< \brief Minimum power */
    uint32 maxPwr;          /**< \brief Maximum power */
    uint32 meanPwr;         /**< \brief Mean power */
    uint32 variance;        /**< \brief Variance */
    uint16 minIdx;          /**< \brief Minimum Index */
    uint16 maxIdx;          /**< \brief Maximum Index */
    uint32 filler[3];       /**< \brief Filler to align into EMEM bus width */
} IfxSpu_StatisticalOutput;

/** \} */

/** \brief Configuration structure for Log2 power summation unit contro
 */
typedef struct
{
    boolean enabled;           /**< \brief Enable / Disable choice for the power summation */
    uint32  baseAddress;       /**< \brief Base address to be used when writing data to Radar Memory */
} IfxSpu_Log2PowerSummationConfig;

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Retruns the Fft size code from length
 * \param length Fft length
 * \return Size code
 */
IFX_INLINE IfxSpu_FftSize IfxSpu_getSizeCode(uint32 length);

/** \brief Returns the status of SPU busy
 * \param spu Pointer to register base
 * \return True, if SPU is busy\n
 * False, if SPU is not busy
 */
IFX_INLINE boolean IfxSpu_isBusy(Ifx_SPU *spu);

/** \brief Is CFAR Mode
 * \param mode CFAR Mode
 * \return True if the mode is CFAR, False otherwise
 */
IFX_INLINE boolean IfxSpu_isCfarMode(IfxSpu_CFAR_Mode mode);

/** \brief Is Local Max Mode
 * \param mode CFAR Mode
 * \return True if the mode is Local Max, False otherwise
 */
IFX_INLINE boolean IfxSpu_isLocalMaxMode(IfxSpu_CFAR_Mode mode);

/** \brief Returns the module's suspend state.
 * TRUE :if module is suspended.
 * FALSE:if module is not yet suspended.
 * \param spu Pointer to SPU module registers
 * \return Suspend status (TRUE / FALSE)
 */
IFX_INLINE boolean IfxSpu_isModuleSuspended(Ifx_SPU *spu);

/** \brief Rejects the selected bin
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param binNumber Number of the bin that needs to be removed
 * \return None
 */
IFX_INLINE void IfxSpu_rejectBin(IfxSpu_ConfigSlot *slot, uint16 binNumber);

/** \brief Configure the Module to Hard/Soft suspend mode.
 * Note: The api works only when the OCDS is enabled and in Supervisor Mode. When OCDS is disabled the OCS suspend control is ineffective.
 * \param spu Pointer to SPU module registers
 * \param mode Module suspend mode
 * \return None
 */
IFX_INLINE void IfxSpu_setSuspendMode(Ifx_SPU *spu, IfxSpu_SuspendMode mode);

/** \brief Setup the Scalar addition
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param operand Additive scaling factor for FFT output data
 * \return None
 */
IFX_INLINE void IfxSpu_setupScalarAddition(IfxSpu_ConfigSlot *slot, uint32 operand);

/** \brief Setup the Scalar multiplication
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param operand Multiplicative scaling factor to be applied to the FFT output data
 * \return None
 */
IFX_INLINE void IfxSpu_setupScalarMultiplication(IfxSpu_ConfigSlot *slot, uint32 operand);

/** \brief Returns base pointer of configuration RAM
 * \param spu Pointer to SPU Base register
 */
IFX_INLINE void *IfxSpu_getConfigRamPtr(Ifx_SPU *spu);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Check the config slot
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param caller caller
 * \param line line
 * \return None
 */
IFX_EXTERN void IfxSpu_checkConfigSlot(IfxSpu_ConfigSlot *slot, const char *caller, uint32 line);

/** \brief Disables the SPU module
 * \param spu Pointer to register base
 * \return None
 */
IFX_EXTERN void IfxSpu_disableModule(Ifx_SPU *spu);

/** \brief Returns the module index of the selected SPU module
 * \param spu Pointer to SPU module registers
 * \return SPU module register address
 */
IFX_EXTERN Ifx_SPU *IfxSpu_getAddress(IfxSpu_Index spu);

/** \brief Get the configuration slot of an SPU module
 * \param spu Pointer to register base
 * \param slotNr If 0 will return pointer to ID_CONF register of the spu, Else pointer to slotNr in the Configuration RAM. Maximum value of slotNr is \ref IFXSPU_CONFIG_SLOT_MAX.
 * \return Pointer to a configuration slot.
 */
IFX_EXTERN IfxSpu_ConfigSlot *IfxSpu_getConfigSlot(Ifx_SPU *spu, uint32 slotNr);

/** \brief Get the data source of a configuration slot
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \return Processing data source
 */
IFX_EXTERN IfxSpu_DataSource IfxSpu_getDataSource(IfxSpu_ConfigSlot *slot);

/** \brief Returns the module index of the selected SPU module
 * \param spu Specifies the pointer to the base of SPU registers
 * \return Module index
 */
IFX_EXTERN IfxSpu_Index IfxSpu_getIndex(Ifx_SPU *spu);

/** \brief Get number of connected antennae when the input is from RIF, not from EMEM.
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \return Number of connected antennae when the input is from RIF\n
 * Zero when the input is from EMEM
 */
IFX_EXTERN uint16 IfxSpu_getNumConnectedAntennae(IfxSpu_ConfigSlot *slot);

/** \brief Get number of input samples per vector.
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \return Number of input samples per vector.
 */
IFX_EXTERN uint16 IfxSpu_getNumInputSamples(IfxSpu_ConfigSlot *slot);

/** \brief Get number of input ramps, when the input is from RIF, not from EMEM
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \return Number of ramps when the input is from RIF\n
 * Zero when the input is from EMEM
 */
IFX_EXTERN uint16 IfxSpu_getNumRamps(IfxSpu_ConfigSlot *slot);

/** \brief Initialises the SPU module
 * \param spu Pointer to register base
 * \param config Configuration structure for module
 * \return None
 */
IFX_EXTERN void IfxSpu_initModule(Ifx_SPU *spu, const IfxSpu_ModuleConfig *config);

/** \brief Initialises the module config structure
 * \param config Configuration structure for module
 * \return None
 */
IFX_EXTERN void IfxSpu_initModuleConfig(IfxSpu_ModuleConfig *config);

/** \brief Load values into configuration RAM
 * \param spu Pointer to register base
 * \param dataPtr Pointer to the location of source data to load
 * \param offset32 Offset in the configuration RAM (in 32-bit words)
 * \param count32 Size of data in the source to load into the configuration RAM (in 32-bit words)
 * \return None
 */
IFX_EXTERN void IfxSpu_loadConfigRAM(Ifx_SPU *spu, const void *dataPtr, uint32 offset32, uint32 count32);

/** \brief Reloads the configuration
 * \param spu Pointer to register base
 * \param slotNr Configuration slot number. Value should be non-zero. Maximum is \ref IFXSPU_CONFIG_SLOT_MAX.
 * \return None
 */
IFX_EXTERN void IfxSpu_reloadConfig(Ifx_SPU *spu, uint32 slotNr);

/** \brief Reset the Non - Coherent Integration Antennae scaling factors
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \return None
 */
IFX_EXTERN void IfxSpu_resetNciWeights(IfxSpu_ConfigSlot *slot);

/** \brief Sets the pass mode of input DMA engine
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param passMode Double pass configuration mode
 * \return None
 */
IFX_EXTERN void IfxSpu_setPassMode(IfxSpu_ConfigSlot *slot, IfxSpu_PassMode passMode);

/** \brief Setup the Bin Rejection unit
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param config Configuration structure for Bin Rejection uint
 * \return None
 */
IFX_EXTERN void IfxSpu_setupBinRejection(IfxSpu_ConfigSlot *slot, const IfxSpu_BinRejectionConfig *config);

/** \brief Setup the CFAR module
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param passId Processing instance number to choose the register instance for base engine (BEx)
 * \param config Configuration structure for CFAR
 * \return None
 */
IFX_EXTERN void IfxSpu_setupCfar(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_CfarConfig *config);

/** \brief Setup the CFAR CA algorithm
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param config Configuration structure for CA-CFAR algorithm
 * \return None
 */
IFX_EXTERN void IfxSpu_setupCfarCA(IfxSpu_ConfigSlot *slot, const IfxSpu_CfarCAConfig *config);

/** \brief Setup the CFAR GOS algorithm
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param config Configuration structure for GOS-CFAR algorithm
 * \return None
 */
IFX_EXTERN void IfxSpu_setupCfarGOS(IfxSpu_ConfigSlot *slot, const IfxSpu_CfarGOSConfig *config);

/** \brief Setup the config sequence
 * \param spu Pointer to register base
 * \param slotNr slot Nr to configure.
 * \param nextSlotNr Next slot number. If zero, slot pointed by slotNr will be the last.
 * \param mode Trigger mode for the slot. (Not the nextSlot)
 * \param clkDiv clock divider
 * \return None
 */
IFX_EXTERN void IfxSpu_setupConfigSequence(Ifx_SPU *spu, uint32 slotNr, uint32 nextSlotNr, IfxSpu_TriggerMode mode, IfxSpu_ClockDivisionRatio clkDiv);

/** \brief Setup the FFT streaming processor, Initialises the input and output parameters for FFT engine
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param passId Processing instance number to choose the register instance for base engine (BEx)
 * \param config Configuration structure for FFT engine
 * \return None
 */
IFX_EXTERN void IfxSpu_setupFft(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_FftConfig *config);

/** \brief Setup the FFT output , Initialises the parameters for Output Data Processor
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param passId Processing instance number to choose the register instance for base engine (BEx)
 * \param config Configuration structure for FFT output
 * \return None
 */
IFX_EXTERN void IfxSpu_setupFftOutput(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_FftOutputConfig *config);

/** \brief Setup the Histogram
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param passId Processing instance number to choose the register instance for base engine (BEx)
 * \param config Configuration structure for Histogram
 * \return None
 */
IFX_EXTERN void IfxSpu_setupHistogram(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_HistogramConfig *config);

/** \brief Setup the input for FFT, Initialises the parameters for input DMA engine
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param config Configuration structure for FFT input
 * \return None
 */
IFX_EXTERN void IfxSpu_setupInput(IfxSpu_ConfigSlot *slot, const IfxSpu_FftInputConfig *config);

/** \brief Setup the Local Max unit
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param config Configuration structure for Local Maximum uint
 * \return None
 */
IFX_EXTERN void IfxSpu_setupLocalMax(IfxSpu_ConfigSlot *slot, const IfxSpu_LocalMaxConfig *config);

/** \brief Setup the Magnitude
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param config Configuration structure for Magnitude approximation
 * \return None
 */
IFX_EXTERN void IfxSpu_setupMagnitude(IfxSpu_ConfigSlot *slot, IfxSpu_MagnitudeConfig *config);

/** \brief Setup the Math1 uint, Initialises the parameters for Math1 uint of the streaming processor1
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param passId Processing instance number to choose the register instance for base engine (BEx)
 * \param config Configuration structure for Math1 uint
 * \return None
 */
IFX_EXTERN void IfxSpu_setupMath1(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_Math1Config *config);

/** \brief Setup the Non - Coherent Integration unit
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param passId Processing instance number to choose the register instance for base engine (BEx)
 * \param config Configuration structure for Non-Coherent intergration
 * \return None
 */
IFX_EXTERN void IfxSpu_setupNci(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_NciConfig *config);

/** \brief Setup the Non - Coherent Integration Antennae scaling factor
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param weights Antennae scaling factors
 * \return None
 */
IFX_EXTERN void IfxSpu_setupNciWeights(IfxSpu_ConfigSlot * slot, uint16 weights[IFXSPU_MAX_ANTENNAE]);

/** \brief Setup the Power Information Channel
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param passId Processing instance number to choose the register instance for base engine (BEx)
 * \param config Configuration structure for Power Information Channel
 * \return None
 */
IFX_EXTERN void IfxSpu_setupPowerChannel(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_PowerOutputConfig *config);

/** \brief Setup the Sideband statistics
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param passId Processing instance number to choose the register instance for base engine (BEx)
 * \param config Configuration structure for Sideband Statistics
 * \return None
 */
IFX_EXTERN void IfxSpu_setupSidebandStatistics(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_SidebandStatisticsConfig *config);

/** \brief Setup the Summation unit
 * \param slot Pointer to a configuration slot (see \ref IfxSpu_getConfigSlot())
 * \param passId Processing instance number to choose the register instance for base engine (BEx)
 * \param config Configuration structure for Summation unit
 * \return None
 */
IFX_EXTERN void IfxSpu_setupSummation(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_SummationConfig *config);

/** \brief Trigger the SPU operation by software
 * \param spu Pointer to register base
 * \return None
 */
IFX_EXTERN void IfxSpu_swTrigger(Ifx_SPU *spu);

/** \brief Function to compute the value for SPU_REGCRC
 * Example usage:
 * uint32 configSlotNr = 0; // 0 = SFR slot, 1 = slot in Configuration RAM.
 * uint32 regcrc = IfxSpu_calculateRegisterCrc(&MODULE_SPU0, configSlotNr);
 * slot->REGCRC.U = regcrc;
 */
IFX_EXTERN uint32 IfxSpu_calculateRegisterCrc(IfxSpu_ConfigSlot *slot);

/** \brief Write a configuration slot with slotValues
 * \return None
 */
IFX_EXTERN void IfxSpu_writeConfigSlot(Ifx_SPU *spu, uint32 slotNr, const uint32 *slotValues);

/**
 * \return None
 */
IFX_EXTERN void IfxSpu_setupLog2PowerSummation(IfxSpu_ConfigSlot *slot, IfxSpu_PassId passId, const IfxSpu_Log2PowerSummationConfig *config);

/** \brief Enables the module(by allowing clock to the module)
 * \param spu SPU SFR's base address
 * \return None
 */
IFX_EXTERN void IfxSpu_enableModule(Ifx_SPU *spu);

/** \brief Performs Kernel Reset
 * \param spu SPU SFR's base address
 * \return None
 */
IFX_EXTERN void IfxSpu_resetModule(Ifx_SPU *spu);

/** \brief Wait for reload
 * \param spu SPU SFR's base address
 * \return None
 */
IFX_EXTERN void IfxSpu_waitReload(Ifx_SPU *spu);

/******************************************************************************/
/*-------------------Global Exported Variables/Constants----------------------*/
/******************************************************************************/

IFX_EXTERN IFX_CONST uint32 IfxSpu_crcTable[];

IFX_EXTERN IFX_CONST uint8  IfxSpu_reflect8[];

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE IfxSpu_FftSize IfxSpu_getSizeCode(uint32 length)
{
    return (IfxSpu_FftSize)(31 - __clz(length));
}


IFX_INLINE boolean IfxSpu_isBusy(Ifx_SPU *spu)
{
    Ifx_SPU_CTRL ctrl;
    ctrl.U = spu->CTRL.U;
    return ctrl.B.BUSY != 0;
}


IFX_INLINE boolean IfxSpu_isCfarMode(IfxSpu_CFAR_Mode mode)
{
    boolean result = FALSE;

    if ((mode == IfxSpu_CFAR_Mode_inline) || (mode == IfxSpu_CFAR_Mode_offline) || (mode == IfxSpu_CFAR_Mode_nci))
    {
        result = TRUE;
    }

    return result;
}


IFX_INLINE boolean IfxSpu_isLocalMaxMode(IfxSpu_CFAR_Mode mode)
{
    boolean result = FALSE;

    if ((mode == IfxSpu_CFAR_Mode_localMaxInline) || (mode == IfxSpu_CFAR_Mode_localMaxOffline) || (mode == IfxSpu_CFAR_Mode_localMaxInline))
    {
        result = TRUE;
    }

    return result;
}


IFX_INLINE boolean IfxSpu_isModuleSuspended(Ifx_SPU *spu)
{
    Ifx_SPU_OCS ocs;

    // read the status
    ocs.U = spu->OCS.U;

    // return the status
    return ocs.B.SUSSTA;
}


IFX_INLINE void IfxSpu_rejectBin(IfxSpu_ConfigSlot *slot, uint16 binNumber)
{
    uint32          index  = binNumber / 32;
    uint32          binPos = binNumber % 32;
    uint32          mask   = 1 << binPos;

    Ifx_SPU_BIN_REJ binRej;
    /* reject the selected bin */
    binRej.U               = slot->BIN_REJ[index].U;
    binRej.U              &= (~mask);
    slot->BIN_REJ[index].U = binRej.U;
}


IFX_INLINE void IfxSpu_setSuspendMode(Ifx_SPU *spu, IfxSpu_SuspendMode mode)
{
    Ifx_SPU_OCS ocs;

    // remove protection and configure the suspend mode.
    ocs.B.SUS_P = 1;
    ocs.B.SUS   = mode;
    spu->OCS.U  = ocs.U;
}


IFX_INLINE void IfxSpu_setupScalarAddition(IfxSpu_ConfigSlot *slot, uint32 operand)
{
    /* Set the additive scaling factor for FFT output data */
    Ifx_SPU_SCALARADD scalarAdd;
    scalarAdd.U         = slot->SCALARADD.U;
    scalarAdd.B.OPERAND = operand;
    slot->SCALARADD.U   = scalarAdd.U;
}


IFX_INLINE void IfxSpu_setupScalarMultiplication(IfxSpu_ConfigSlot *slot, uint32 operand)
{
    /* Set the multiplicative scaling factor to be applied to the FFT output data */
    Ifx_SPU_SCALARMULT scalarMult;
    scalarMult.U         = slot->SCALARMULT.U;
    scalarMult.B.OPERAND = operand;
    slot->SCALARMULT.U   = scalarMult.U;
}


IFX_INLINE void *IfxSpu_getConfigRamPtr(Ifx_SPU *spu)
{
    void *configRamPtr = NULL_PTR;

    if (spu == &MODULE_SPU0)
    {
        configRamPtr = (void *)IFXSPU_CONFIG_RAM0_BASE;
    }

    else if (spu == &MODULE_SPU1)
    {
        configRamPtr = (void *)IFXSPU_CONFIG_RAM1_BASE;
    }

    return configRamPtr;
}


#endif /* IFXSPU_H */
