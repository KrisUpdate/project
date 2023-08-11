/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef I2CROM_H
#define I2CROM_H 1

#include "Configuration.h"
#include "board.h"

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/
/* for saving system info */
typedef enum
{
	ROMID_KEY,
	ROMID_IP_ADDR,
	ROMID_ETH_MODE,
	ROMID_RESERVED,
	ROMID_FW_VERSION,
	ROMID_MAX,
}e_setting_id;

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
/*\brief initial the system setting entry
 *\param: in: void
 *\return: void */
void setting_init(void);

/*\brief check if setting data is initial
 *\param: in: void
 *\return: void */
boolean setting_is_initial(void);

/*\brief Save data to setting database
 *\param: in: e_setting_id id
			 the id to save
 *\param: in: pValue  the value to save
 *\return: void */
void setting_set(e_setting_id id, const void *pValue);

/*\brief Get the data from setting database
 *\param: in: e_setting_id id
			 the id to read
 *\return void *  the value */
const void* setting_get(e_setting_id id);



/*\brief set the ROM parameter that being used
 *\param: in: uint8 i2cbus: the MCU i2c bus
 *\param: in: uint8 i2caddress: the slave address of I2C ROM (7bit)
 *\return void
 */
void i2crom_user_info_set_rom(uint8 i2cbus, uint8 i2caddress);

/*\brief read the user info from ROM
 *\param: in: uint8* pbuf:
 *       the buffer to save for the reading data, size should be larger than USER_VALID_DATA_SIZE
 *\param: in: uint8* object: the object that reads
 *\param: in: uint8 id: the object id
 *\return int32
 *       return -1 if the object is not in the list
 *       return 0 if no data is read out (empty)
 *       return the size of data that read out from ROM*/
sint32 i2crom_user_info_read(uint8* pbuf, uint8* object, uint8 id);

/*\brief write data to the user info part in ROM
 *\param: in: uint8* pbuf:
 *       the data to write
 *\param: in: uint8* object:
 *       the object that reads
 *\return boolean
 *       result of the writing, TRUE success and FALSE fail */
boolean i2crom_user_info_write(uint8* pbuf, uint8* object, uint8 id);

/*\brief initial the info ROM
 *\param: in: void
 *\return void */
void i2crom_user_info_initial();

/*\brief clear/erase the user info part in ROM
 *\param: in: void
 *\return void */
void i2crom_user_info_clear();

/*\brief dump the user info from ROM
 *\param: in: the buffer to store the data
 *\return uint16 size: return the size o bytes that dump out */
uint16 i2crom_user_info_dump(uint8* buff);

/*\brief flash the user info into info ROM
 *\param: in: the buffer to write
 *\return void */
uint8 i2crom_user_info_flash(uint8* buff, uint8 startAddr, uint8 size);

/*\brief clear/erase the whole ROM
 *\param: in: void
 *\return void */
void i2crom_erase();

/*\brief run the i2c rom test
 *\param: in: void
 *\return void */
void run_i2crom_test();

/* specific function to operate main board ROM (E3550) */
#if (PROJECT == PROJECT_PAGASUS2) && (PG2_PREEVT == 1)
/*\brief read CE Mode from IDrom
 *\param: in: void
 *\return  uint8 */
uint8 i2crom_read_ce_mode();

/*\brief Write CE Mode to IDrom
 *\param: in: ce mode to write
 *\return  void */
void i2crom_write_ce_mode(uint8 ce_mode);

/*\brief read Boot Media from IDrom
 *\param: in: boot medium for ta
 *\param: in: boot medium for tb
 *\return  uint8: 0: UFS, 1: Qspi; 2: Emmc */
void i2crom_read_boot_media(uint8* boot_medium_ta, uint8* boot_medium_tb);

/*\brief Write Boot Media to IDrom
 *\param: in: boot medium for ta
 *\param: in: boot medium for tb
 *\return  void */
void i2crom_write_boot_media(uint8 boot_medium_ta, uint8 boot_medium_tb);


/*\brief Get board part number
 *\param: in: uint8* buffer to store part number, at least 24 bytes length
 *\return  boolean  true if reading pat number successfully
 *\                 false if reading fail */
boolean i2crom_getPartNumber(uint8* part_number);

/*\brief Get board pcb revesion number,
 *\param: in: uint8* buffer to store pcb rev, at least 2 bytes length
 *       First character is ASC "A", "B" etc
 *       Second character is BCD Digital, indicate the version
 *\return  boolean  true if reading pat number successfully
 *\                 false if reading fail */
boolean i2crom_getPcbRev(uint8* pcbRev);

/*\brief Get Aurix ethernet Mac address,
 *\param: in: uint8* buffer to store ethernet mac address, at least 8 bytes length
 *\return  boolean  true if reading successfully
 *\                 false if reading fail */
boolean i2crom_getEthMac(uint8* ethMac, e_AurixIndex aurix_index);

/* operation on CIM ROM */
/*\brief read CIM SKU from IDrom in CIM
 *\param: in uint8* the buffer pointer to store the sku, buffer size >= 24
 *\return void */
void i2crom_read_cim_sku(uint8* pbuf);

#endif //endof #if (PROJECT == PROJECT_DRIVE_AX) && (DRIVE_AX_B0x == 1)

#endif
