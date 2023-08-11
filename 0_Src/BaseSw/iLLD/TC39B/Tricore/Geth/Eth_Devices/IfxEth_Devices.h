/**
 * \file IfxEth_Devices.h
 * \brief ETH DEVICES details
 * \ingroup IfxLld_Eth
 *
 * \version iLLD_1_0_0_5_0
 * \copyright Copyright (c) 2015 Infineon Technologies AG. All rights reserved.
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

#ifndef IFXETH_DEVICES_TC397_H
#define IFXETH_DEVICES_TC397_H

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Geth/Std/IfxGeth.h"
#include "Configuration.h"

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/**
 * \return Status
 */
IFX_EXTERN uint32 IfxEth_Module_Init(void);

/**
 * \return Link status
 */
IFX_EXTERN boolean IfxEth_Phy_link(void);


IFX_EXTERN void IfxEth_mdioinit(void);

void IfxEth_mdio_set_simulating(boolean enable);

/*\brief   MDIO/MDC clause 22 read function
 *\param:  slaveaddr     uint32     5 bit slave physical address
 *\param:  regaddr       uint32     5 bit register address
 *\param:  pdata         uint32*  the pointer to store the read back value
 *\return: void */
void IfxEth_read_mdio_clause_22(uint32 slaveaddr, uint32 regaddr, uint32 *pdata);

/*\brief   MDIO/MDC clause 22 write function
 *\param:  slaveaddr     uint32     5 bit slave physical address
 *\param:  regaddr       uint32     5 bit register address
 *\param:  data          uint32     data to write
 *\return: void */
void IfxEth_write_mdio_clause_22(uint32 slaveaddr, uint32 regaddr, uint32 data);


/*\brief   MDIO/MDC read wrapper function, which will handle mdio logic as well
 *\param:  slaveaddr     uint32     device slave address
 *\param:  layeraddr     uint32     device layer address, AKA device address.
 *\param:  regaddr       uint32     register address
 *\param:  data          uint32     data to write
 *\note:   when slave address is marvell switch slave address, this function will decide
 *\        whether to send slave address or layer address in mdio line, based on
 *\        single-chip/multi-chip mode configuration
 *\return: void */
IFX_EXTERN void IfxEth_read_mdio_reg(uint32 slaveaddr, uint32 layeraddr, uint32 regaddr, uint32 *pdata);

/*\brief   MDIO/MDC write wrapper function, which will handle mdio logic as well
 *\param:  slaveaddr     uint32     device slave address
 *\param:  layeraddr     uint32     device layer address, AKA device address.
 *\param:  regaddr       uint32     register address
 *\param:  data          uint32     data to write
 *\note:   when slave address is marvell switch slave address, this function will decide
 *\        whether to send slave address or layer address in mdio line, based on
 *\        single-chip/multi-chip mode configuration
 *\return: void */
IFX_EXTERN void IfxEth_write_mdio_reg(uint32 slaveaddr, uint32 layeraddr, uint32 regaddr, uint32 data);


/*\brief   Set MDIO logic GPIO table based on the slave address
 *\param:  slaveaddr     uint32     device slave address
 *\return: void */
IFX_EXTERN void IfxEth_Cfg_Aurix_MDC(uint32 slaveaddr);

/*\brief   Set MDIO logic GPIO table for 88Q5050
 *\param:  void
 *\return: void */
IFX_EXTERN void IfxEth_Cfg_Aurix_MDC_5050(void);

/*\brief   Set MDIO logic GPIO table for AQR107
 *\param:  void
 *\return: void */
IFX_EXTERN void IfxEth_Cfg_Aurix_MDC_107(void);


/*\brief   Reset the MDC logic GPIO table before power down
 *\param:  void
 *\return: void */
IFX_EXTERN void IfxEth_Cfg_Aurix_MDC_Reset(void);

IFX_EXTERN void IfxEth_read_mdio_clause_45(uint32 slaveaddr, uint32 devaddr, uint32 regaddr, uint32 *pdata);
IFX_EXTERN void IfxEth_write_mdio_clause_45(uint32 slaveaddr, uint32 devaddr, uint32 regaddr, uint32 data);

/** \brief Checks whether physical connection is active
 * \param eth ETH driver structure
 * \return retval zero Connection is inactive
 * retval non zero Connection is active
 */
IFX_INLINE boolean IfxEth_isLinkActive(void);


IFX_INLINE boolean IfxEth_isLinkActive(void)
{
    return IfxEth_Phy_link() != 0;
}

/** \} */

/******************************************************************************/
/*-------------------Global Exported Variables/Constants----------------------*/
/******************************************************************************/

/******************************************************************************/
/*----------------------------------Macros------------------------------------*/
/******************************************************************************/


//Marvell PHY Registers


#endif /* IFXETH_DEVICES_H */
