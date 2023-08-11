/**
 * \file IfxEmem.h
 * \brief EMEM  basic functionality
 * \ingroup IfxLld_Emem
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
 * \defgroup IfxLld_Emem_Std_Enumerations Enumerations
 * \ingroup IfxLld_Emem_Std
 * \defgroup IfxLld_Emem_Std_Module Module Functions
 * \ingroup IfxLld_Emem_Std
 */

#ifndef IFXEMEM_H
#define IFXEMEM_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxEmem_cfg.h"
#include "Scu/Std/IfxScuWdt.h"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Emem_Std_Enumerations
 * \{ */
/** \brief EMEM tile configuration mode defined in MODULE_EMEM.TILECONFIGXM.B.Tx( x = 0,1,..).
 */
typedef enum
{
    IfxEmem_ExtendMemoryConfigMode_calibMode  = 0, /**< \brief EMEM tile mode to calibration memory. */
    IfxEmem_ExtendMemoryConfigMode_adasMode   = 1, /**< \brief EMEM tile mode to ADAS memory. */
    IfxEmem_ExtendMemoryConfigMode_unusedMode = 3  /**< \brief Tile in Unused Mode */
} IfxEmem_ExtendMemoryConfigMode;

/** \brief EMEM lock state defined in MODULE_EMEM.SBRCTR.B.STBLOCK.
 */
typedef enum
{
    IfxEmem_LockedState_locked   = 0, /**< \brief EMEM locked state. */
    IfxEmem_LockedState_unlocked = 1  /**< \brief EMEM unlocked state. */
} IfxEmem_LockedState;

/** \brief EMEM module clock enabled or disabled state defined in MODULE_EMEM.CLC.B.DISR.
 */
typedef enum
{
    IfxEmem_State_disabled = 0,  /**< \brief EMEM module clock disabled state. */
    IfxEmem_State_enabled  = 1   /**< \brief EMEM module clock enabled state. */
} IfxEmem_State;

/** \brief EMEM tile configuration mode defined in MODULE_EMEM.TILECONFIG.B.Tx( x = 0,1,..).
 */
typedef enum
{
    IfxEmem_TileConfigMode_calibMode        = 0, /**< \brief EMEM tile mode to calibration memory. */
    IfxEmem_TileConfigMode_traceMode        = 2, /**< \brief EMEM tile mode to Trace memory. */
    IfxEmem_TileConfigMode_unusedMode       = 3, /**< \brief Tile in Unused Mode */
    IfxEmem_TileConfigMode_commonMemoryMode = 0  /**< \brief EMEM Tile to be configured to Common Memory Mode */
} IfxEmem_TileConfigMode;

/** \brief Tile Number
 */
typedef enum
{
    IfxEmem_TileNumber_0 = 0,      /**< \brief Tile Number0  */
    IfxEmem_TileNumber_1,          /**< \brief Tile Number1  */
    IfxEmem_TileNumber_2,          /**< \brief Tile Number2  */
    IfxEmem_TileNumber_3,          /**< \brief Tile Number3  */
    IfxEmem_TileNumber_4,          /**< \brief Tile Number4  */
    IfxEmem_TileNumber_5,          /**< \brief Tile Number5  */
    IfxEmem_TileNumber_6,          /**< \brief Tile Number6  */
    IfxEmem_TileNumber_7,          /**< \brief Tile Number7  */
    IfxEmem_TileNumber_8,          /**< \brief Tile Number8  */
    IfxEmem_TileNumber_9,          /**< \brief Tile Number9  */
    IfxEmem_TileNumber_10,         /**< \brief Tile Number10  */
    IfxEmem_TileNumber_11,         /**< \brief Tile Number11  */
    IfxEmem_TileNumber_12,         /**< \brief Tile Number12  */
    IfxEmem_TileNumber_13,         /**< \brief Tile Number13  */
    IfxEmem_TileNumber_14,         /**< \brief Tile Number14  */
    IfxEmem_TileNumber_15          /**< \brief Tile Number15  */
} IfxEmem_TileNumber;

/** \} */

/** \addtogroup IfxLld_Emem_Std_Module
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Returns the status of module enabled or disabled
 * \return Status (TRUE / FALSE)
 */
IFX_INLINE boolean IfxEmem_isModuleEnabled(void);

/** \brief Set the tile to calibration/BBB mode
 * \param calibrationMode enable calibration/BBB Mode
 * \param tile tile number
 * \return None
 */
IFX_INLINE void IfxEmem_setCalibrationTileControlMode(boolean calibrationMode, IfxEmem_TileNumber tile);

/** \brief Sets all EMEM tiles to calibration memory mode.
 * \param mode EMEM tile configuration mode.
 * \param tile tile number
 * \return None
 */
IFX_INLINE void IfxEmem_setTileConfigMode(const IfxEmem_TileConfigMode mode, IfxEmem_TileNumber tile);

/** \brief Set the tile to trace/BBB mode
 * \param traceMode enable Trace/BBB mode
 * \param tile tile number
 * \return None
 */
IFX_INLINE void IfxEmem_setTraceTileControlMode(boolean traceMode, IfxEmem_TileNumber tile);

/** \brief Sets Unlock standby lock flag.
 * \param flag Unlock standby lock flag value.
 * \return None
 */
IFX_INLINE void IfxEmem_setUnlockStandbyLockFlag(const uint8 flag);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Gets the EMEM stand RAM lock state.
 * \return EMEM stand RAM lock state.
 */
IFX_EXTERN IfxEmem_LockedState IfxEmem_getLockedState(void);

/** \brief Sets state of the EMEM module clock.
 * Note: Do not use this API for enabling and disabling EMEM without handling Endinit protection in application.
 * for complete enable and disable of EMEM with endint protection handling, please use the following APIs
 * /ref IfxEmem_enableModule
 * /ref IfxEmem_disableModule
 * \param state EMEM module clock enabled or disabled state.
 * \return None
 */
IFX_EXTERN void IfxEmem_setClockEnableState(const IfxEmem_State state);

/** \} */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Unlocks the EMEM RAM areas for bus accesses(RW).
 * NOTE: After a Power On Reset, the SRAM initialization sequence has to be executed atleast once following the Unlock sequence.
 * \param ememCore Pointer to EMEM Core Registers
 * \return None
 */
IFX_EXTERN void IfxEmem_setUnlockMode(Ifx_EMEM *ememCore);

/**
 * \param ememCore Pointer to EMEM Core Registers
 * \return None
 */
IFX_EXTERN void IfxEmem_enableModule(Ifx_EMEM *ememCore);

/**
 * \param ememCore Pointer to EMEM Core Registers
 * \return None
 */
IFX_EXTERN void IfxEmem_disableModule(Ifx_EMEM *ememCore);

/** \brief Disables reporting of bus errors by writing into MODULE_EMEMx.MEMCON.B.ERRDIS
 * \param mpuIndex EMEM Mpu Index
 * \return None
 */
IFX_EXTERN void IfxEmem_disableEccErrorReporting(IfxEmem_MpuIndex mpuIndex);

/** \brief Disables reporting of bus errors by writing into MODULE_EMEMx.MEMCON.B.ERRDIS
 * \param mpuIndex EMEM Mpu Index
 * \return None
 */
IFX_EXTERN void IfxEmem_enableEccErrorReporting(IfxEmem_MpuIndex mpuIndex);

/**
 * \param ememMpu Pointer to EMEM MPU base SFR
 * \return Index of the EMEM MPU instance
 */
IFX_EXTERN IfxEmem_MpuIndex IfxEmem_getIndex(Ifx_EMEM_MPU *ememMpu);

/**
 * \param ememMpu Index of an EMEM MPU instance
 * \return Address of the EMEM MPU instance given by Index number
 */
IFX_EXTERN Ifx_EMEM_MPU *IfxEmem_getAddress(IfxEmem_MpuIndex ememMpu);

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE boolean IfxEmem_isModuleEnabled(void)
{
    return (MODULE_EMEM.CLC.B.DISS == 0) ? TRUE : FALSE;
}


IFX_INLINE void IfxEmem_setCalibrationTileControlMode(boolean calibrationMode, IfxEmem_TileNumber tile)
{
    MODULE_EMEM.TILECC.U |= (uint32)(calibrationMode << tile);
}


IFX_INLINE void IfxEmem_setTileConfigMode(const IfxEmem_TileConfigMode mode, IfxEmem_TileNumber tile)
{
    uint32 shift = tile * 2;
    uint32 mask  = ~(0x3 << shift);
    uint32 value = (uint32)mode << shift;
    MODULE_EMEM.TILECONFIG.U = (MODULE_EMEM.TILESTATE.U & mask) | value;
}


IFX_INLINE void IfxEmem_setTraceTileControlMode(boolean traceMode, IfxEmem_TileNumber tile)
{
    MODULE_EMEM.TILECT.U |= (uint32)(traceMode << tile);
}


IFX_INLINE void IfxEmem_setUnlockStandbyLockFlag(const uint8 flag)
{
    if (8 > flag)
    {
        MODULE_EMEM.SBRCTR.B.STBULK = flag;
    }
}


#endif /* IFXEMEM_H */
