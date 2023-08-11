/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef SWITCH_SJA1105_H_
#define SWITCH_SJA1105_H_

#include <Configuration.h>

#define SJA1105_SPI_CONTROL_PHASE_BIT_OP_RD                (0x0 << 31)
#define SJA1105_SPI_CONTROL_PHASE_BIT_OP_WR                (0x1 << 31)
#define SJA1105_SPI_CONTROL_PHASE_BIT_COUNT_OF_DWORD(x)    (((x)&0x1F) << 25)
#define SJA1105_SPI_CONTROL_PHASE_BIT_REG_ADDR(x)          (((x)&0x1FFFFF) << 4)

/*\brief Initial all the Ethernet switch
 *\param:  Void
 *\return: void */
IFX_EXTERN void Switch_SJA1105_Initial(void);

/*\brief Initial SJA1105 Step 2
 *       we reset TDL in port 4 in this step
 *\param:  Void
 *\return: void */
IFX_EXTERN void Switch_SJA1105_Step2_Init(void);

/*\brief Read register value from SAJ1105
 *\param:  wordCount         uint8_t   the number of register to read
 *\param:  registerAddress   uint32_t  register address
 *\param:  p_registerValue   uint32_t* the pointer to store the read back value
 *\return: void */
IFX_EXTERN void Switch_SJA1105_Register_Read(uint8 wordCount, uint32 registerAddress, uint32 *p_registerValue);

/*\brief Write register value to SAJ1105
 *\param:  wordCount         uint8_t   the number of register to write, max 64
 *\param:  registerAddress   uint32_t  register address
 *\param:  p_registerValue   uint32_t* the pointer of register value to write
 *\return: void */
IFX_EXTERN void Switch_SJA1105_Register_Write(uint8 wordCount, uint32 registerAddress, uint32 *p_registerValue);

#endif /* SWITCH_SJA1105_H_ */
