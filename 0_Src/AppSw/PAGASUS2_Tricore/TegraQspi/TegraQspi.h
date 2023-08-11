/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef TEGRAQSPI_H
#define TEGRAQSPI_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Cpu/Std/Ifx_Types.h>
#include "Configuration.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define NUM_TEGRA_QSPI_MODULE 		2
/* define the MAX qspi modules number in the chip */
#define MAX_QSPI_MODULE             6

#define QSPI_BUFFER_SIZE 	80   /**< \brief Tx/Rx Buffer size */
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN const void * qspi_pins_ptr[MAX_QSPI_MODULE];
IFX_EXTERN const boolean is_cs_pin_automatic[MAX_QSPI_MODULE];
IFX_EXTERN const void * cs_pins_ptr[MAX_QSPI_MODULE];
IFX_EXTERN const uint8 SPI_BIT_WIDTH[MAX_QSPI_MODULE];
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void TegraQspiMaster_init(uint8 qspi_module_number, uint8 qspi_mode);
IFX_EXTERN uint32 TegraQspiMaster_read_write(uint32 slvNr, uint32 count, uint32 *pSendData, uint32 *pRecData);

IFX_EXTERN void TegraQspiSlave_init(uint8 qspi_module_number);
IFX_EXTERN void TegraQspiSlave_loopback(uint16 count);
IFX_EXTERN boolean TegraQspiSlave_read_write(uint32 count, uint8 *pSendData, uint8 *pRecData);

/** \brief Prepare the spi to transfer data
 * \param count number of data (32 Bit per count)
 * \param pSendData (Pointer to the data to be transmitted)
 * \return TRUE/FALSE
 */
extern boolean TegraQspiSlave_PrepareTransmit(uint32 count, uint8 *pSendData);

/** \brief Prepare the spi to receive data
 * \param count number of data (32 Bit per count)
 * \param pRecData (Pointer to the data to be received)
 * \return TRUE/FALSE
 */
extern boolean TegraQspiSlave_PrepareReceive(uint32 count, uint8 *pRecData);



/** \brief Callback function called at end of reception
 * \return none
 */
extern void TegraQspiSlave_JobEndNotification(void);

#endif  // TEGRAQSPI_H
