/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "SysSe/Comm/Ifx_Shell.h"
#include "SysSe/Bsp/Bsp.h"
#include "I2cCmds.h"
#include "I2c.h"
#include "I2cRom.h"
#include <internal_fw_debug.h>
#include "stdlib.h"
#include "ethernet.h"
#include <stdio.h>
#include <string.h>
#include "i2crom_drv.h"
#include "string.h"
extern IfxStdIf_DPipe *io_tmp;
//#if (PROJECT == PROJECT_DRIVE_AX) && (DRIVE_AX_B0x == 1)
#if (PROJECT == PROJECT_PAGASUS2)
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define I2C_ROM_DEBUG_ENABLE 0

#if I2C_ROM_DEBUG_ENABLE == 1
#define I2C_ROM_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define I2C_ROM_DEBUG_PRINTF(x)
#endif

#define I2C_ROM_PRINTF_ALWAYS(x) DEBUG_PRINTF_ALWAYS(x)

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
/* Setting functions */
/* @brief the structure of the array element
 *          for the fast lookup setting entity with only ID provided
 */
typedef struct
{
    void * const    p;      /**< pointer to the object*/
    uint16          size;   /**< size of the object*/
}t_setting_database;

typedef struct
{
	uint32 setting_key;
	t_ip_addr ip;
	e_eth_mode mode;
	uint8 reserved[3];
	uint32 fw_version;
}t_setting;

static t_setting setting;
static t_setting_database setting_db[ROMID_MAX] =
{
		{&setting.setting_key,   sizeof(setting.setting_key)},
		{&setting.ip,   sizeof(setting.ip)},
		{&setting.mode, sizeof(setting.mode)},
		{&setting.reserved, sizeof(setting.reserved)},
		{&setting.fw_version, sizeof(setting.fw_version)},
};

#define SETTING_KEY              0x55AA01FE
#define SETTING_MAX_SIZE         (32)
#define SETTING_SIZE             sizeof(t_setting)
#define SETTING_START_ADDR       (ROM_SIZE - SETTING_MAX_SIZE)
#define INFO_ROM_SIZE   	     (ROM_SIZE - SETTING_MAX_SIZE)

/* setting */
static uint32 i2crom_get_setting_addr(e_setting_id id);
static void i2crom_write_setting(e_setting_id id);
static void i2crom_read_setting(e_setting_id id);

/* Macro Setting for main board (E3550) ROM */
// Common Header for all objects
typedef struct
{
	uint8  type[3];
	uint32 version:8;
	uint32 subversion:8;
	uint32 bytes:16;
	uint32 checksum:8;
}__attribute__((packed)) t_object_header;

// Object pointer for ROM OBJ
typedef struct
{
	uint8 type[3];
	uint32 location:16;
}__attribute__((packed)) t_object_pointer;

#define OBJ_NUMBER   3  //SYS, RWK, SW

// ROM OBJ
typedef struct
{
	t_object_header header;
	uint32 reserved:16;
	uint32 last_updated_time:32;
	t_object_pointer objects[OBJ_NUMBER];
	uint8  extra_reserved[3];
}__attribute__((packed)) t_ROM_fat;

// SYS OBJ
typedef struct
{
	t_object_header header;
	uint8  build_date[4];
	uint8  part_number[24];
	uint8  pcb_revision[2];
	uint8  BOM_build_revision[2];
	uint8  system_serial_number[16];
	uint8  mode;
	uint8  mode_reserved;
	uint8  bootmedia; /* Upper nibble is TA, Lower nibble is TB */
	uint8  Aurix_mac_address[8];
	uint8  tegra_a_mac_address[8];
	uint8  tegra_b_mac_address[8];
	uint8  Chip_10GE_mac_address_1[8];
	uint8  Chip_10GE_mac_address_2[8];
	uint8  Chip_1GE_mac_address_1[8];
	uint8  Aurix2_mac_address[8];
	uint8  reserved[45];
}__attribute__((packed)) t_brd_info;

#define RWK_REWORK_FLAG_BYTES     16
#define RWK_RELEASE_LEVEL_BYTES    2

// RWK OBJ
typedef struct
{
	t_object_header header;
	uint8  rework_flag[RWK_REWORK_FLAG_BYTES];
	uint8  release_level[RWK_RELEASE_LEVEL_BYTES];
	uint8  reserved[6];
}__attribute__((packed)) t_brd_rwk;

// Complete IDROM structure, excluding the Nv internal FW settings
typedef struct
{
	t_ROM_fat rom_fat;    // ROM
	t_brd_info brd_info;  // SYS
	t_brd_rwk brd_rwk;    // RWK
}__attribute__((packed)) t_ROM_format;

#define HEADER_SIZE      (sizeof(t_object_header))
#define SYS_START_ADDR   (sizeof(t_ROM_fat))
#define RWK_START_ADDR   (sizeof(t_ROM_fat) + sizeof(t_brd_info))
#define ROM_FAT_SIZE     (sizeof(t_ROM_fat))
#define SYS_SIZE         (sizeof(t_brd_info))
#define RWK_SIZE         (sizeof(t_brd_rwk))

#define CE_MODE_BYTE_OFFSET       (56)
#define BOOT_MEDIA_BYTE_OFFSET    (58)

// Complete IDROM structure, including the Nv internal FW settings
typedef struct
{
	union
	{
		t_ROM_format t;
		uint8 b[INFO_ROM_SIZE];
	}u_rom_format;
	union
	{
		t_setting t;
		uint8 b[SETTING_MAX_SIZE];
	}u_setting;
}__attribute__((packed)) t_ROM_structure;

//static t_ROM_format rom_format;

/* Macro Setting for CIM board ROM */
#define CIM_SYS_START_ADDR   		  (sizeof(t_ROM_fat)+32)
#define CIM_SYS_PART_NUMBER_OFFSET    (12)
#define CIM_SYS_PART_NUMBER_SIZE      (24)

/* In E3550, we use 32 bytes in the end for sw purpose, hence the info rom valid size is
 *   ROM_SIZE - SETTING_MAX_SIZE
 * In CIM module, all the ROM is used as info rom */
static const t_rom_object E3550_ROM = {0, 0x54, INFO_ROM_SIZE, SETTING_START_ADDR};
static const t_rom_object CIM_ROM = {0, 0x50, ROM_SIZE, ROM_SIZE+1};
/* by default, set it the same as E3550_ROM */
static t_rom_object current_ROM = {0, 0x54, INFO_ROM_SIZE, SETTING_START_ADDR};

/******************************************************************************/
/*-------------------------------Config --------------------------------------*/
/******************************************************************************/
/* ROM TEST */
#define TEST_SIZE  ROM_SIZE

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/



/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Setting Implementations---------------------------*/
/******************************************************************************/
void setting_init(void)
{
	uint32 key = SETTING_KEY;
	setting_set(ROMID_KEY, &key);
}

boolean setting_is_initial(void)
{
	uint32 key = *(uint32*) setting_get(ROMID_KEY);
	if(key == SETTING_KEY)
	{
		I2C_ROM_DEBUG_PRINTF(("setting is initial \n"));
		return TRUE;
	}
	I2C_ROM_DEBUG_PRINTF(("setting is NOT initial \n"));
	return FALSE;
}

void setting_set(e_setting_id id, const void *pValue)
{
	if(id >= ROMID_MAX)
	{
		return;
	}
	if(pValue == NULL)
	{
		return;
	}
	if( id < ROMID_MAX)
	{
		if(memcmp(pValue, setting_db[id].p, setting_db[id].size) != 0)
		{
			memcpy(setting_db[id].p, pValue, setting_db[id].size);
			i2crom_write_setting(id);
			I2C_ROM_DEBUG_PRINTF(("setting_set: save to ROM\n"));
		}
		else
		{
			I2C_ROM_DEBUG_PRINTF(("setting_set: Value is the same, no need to save to ROM\n"));
		}
	}
}

const void* setting_get(e_setting_id id)
{
	void* ret = NULL;
	if(id < ROMID_MAX)
	{
		i2crom_read_setting(id);
		ret = setting_db[id].p;
	}
	return ret;
}

static uint32 i2crom_get_setting_addr(e_setting_id id)
{
	uint32 addr = SETTING_START_ADDR;
	e_setting_id index;
	for(index=0; index < id; index++)
	{
		addr += setting_db[index].size;
	}
	return addr;
}
static void i2crom_write_setting(e_setting_id id)
{
	if(id >= ROMID_MAX)
	{
		return;
	}
	uint32 addr = i2crom_get_setting_addr(id);
	i2crom_drv_write(&E3550_ROM, addr, (uint8*)setting_db[id].p, setting_db[id].size);
}

static void i2crom_read_setting(e_setting_id id)
{
	if(id >= ROMID_MAX)
	{
		return;
	}
	uint32 addr = i2crom_get_setting_addr(id);
	i2crom_drv_read(&E3550_ROM, addr, (uint8*)setting_db[id].p, setting_db[id].size);
}


/******************************************************************************/
/*-------------------------User Info Implementations--------------------------*/
/******************************************************************************/
typedef uint32 (*read_object_func_ptr)(uint8*);
typedef boolean (*write_object_func_ptr)(uint8*);

static uint8 find_object(const uint8* object);
static uint32 read_sys_object(uint8* pbuf);
static uint32 read_rwk_object(uint8* pbuf);
static boolean write_sys_object(uint8* pbuf);
static boolean write_rwk_object(uint8* pbuf);

typedef struct
{
	uint8* name;
	read_object_func_ptr read_object_func;
	write_object_func_ptr write_object_func;
}t_object_list;

static const t_object_list OBJECT_LIST[]=
{
		{
				.name = "sys",
				.read_object_func = read_sys_object,
				.write_object_func = write_sys_object,
		},
		{
				.name = "rwk",
				.read_object_func = read_rwk_object,
				.write_object_func = write_rwk_object,
		},
};
#define OBJECT_NUMBER (sizeof(OBJECT_LIST)/sizeof(t_object_list))

/* load the config by i2c bus and address */
static boolean load_rom_config(uint8 i2cbus, uint8 i2caddress)
{
	boolean ret = FALSE;
	if((i2cbus == E3550_ROM.i2c_bus) && (i2caddress == E3550_ROM.i2c_address))
	{ /* E3550 ROM */
		memcpy(&current_ROM, &E3550_ROM, sizeof(E3550_ROM));
		ret = TRUE;
	}
	else if((i2cbus == CIM_ROM.i2c_bus) && (i2caddress == CIM_ROM.i2c_address))
	{ /* CIM ROM */
		memcpy(&current_ROM, &CIM_ROM, sizeof(CIM_ROM));
		ret = TRUE;
	}
	else
	{
		I2C_ROM_PRINTF_ALWAYS(("Warning: doesn't find a match info rom with current i2c bus %d and address 0x%x\r\n",
				i2cbus, i2caddress));
		I2C_ROM_PRINTF_ALWAYS(("setting i2cbus, address fail\r\n"));
	}
	I2C_ROM_PRINTF_ALWAYS(("i2cbus: %d, address: 0x%x, info size %d\r\n.",
			current_ROM.i2c_bus, current_ROM.i2c_address,
			current_ROM.valid_info_size));
	return ret;
}

void i2crom_user_info_set_rom(uint8 i2cbus, uint8 i2caddress)
{
	if(i2cbus >= NUM_I2C_MODULE)
	{
		I2C_ROM_PRINTF_ALWAYS(("Error: fail to set i2cbus, max I2C modules is %d", NUM_I2C_MODULE));
		return;
	}
	load_rom_config(i2cbus, i2caddress);
}

sint32 i2crom_user_info_read(uint8* pbuf, uint8* object, uint8 id)
{
	uint8 index;
	if(object != NULL)
	{
		index = find_object(object);
	}
	else
	{
		index = id;
	}
	if(index >= OBJECT_NUMBER)
	{
		I2C_ROM_PRINTF_ALWAYS(("Invalid Object name or ID\n"));
		return -1;
	}
	return OBJECT_LIST[index].read_object_func(pbuf);
}


boolean i2crom_user_info_write(uint8* pbuf, uint8* object, uint8 id)
{
	uint8 index;
	if(object != NULL)
	{
		index = find_object(object);
	}
	else
	{
		index = id;
	}
	if(index >= OBJECT_NUMBER)
	{
		I2C_ROM_PRINTF_ALWAYS(("Invalid Object name or ID\n"));
		return -1;
	}
	return OBJECT_LIST[index].write_object_func(pbuf);
}

void i2crom_user_info_initial()
{
	t_ROM_format rom_format;
	i2crom_drv_read(&current_ROM, 0, (uint8*)&rom_format, sizeof(t_ROM_format));

	// Initialize ROM
	rom_format.rom_fat.header.version=2;
	rom_format.rom_fat.header.subversion=0;
	rom_format.rom_fat.header.bytes=0x20;
	memcpy(rom_format.rom_fat.header.type, "rom", 3);

	rom_format.rom_fat.last_updated_time=0x20171010;

	memcpy(rom_format.rom_fat.objects[0].type, "sys", 3);
	rom_format.rom_fat.objects[0].location=SYS_START_ADDR;

	memcpy(rom_format.rom_fat.objects[1].type, "rwk", 3);
	rom_format.rom_fat.objects[1].location=RWK_START_ADDR;

	// Initialize SYS
	rom_format.brd_info.header.version=2;
	rom_format.brd_info.header.subversion=0;
	rom_format.brd_info.header.bytes=0xa0;

	// Initialize RWK
	rom_format.brd_rwk.header.version=1;
	rom_format.brd_rwk.header.subversion=0;
	rom_format.brd_rwk.header.bytes=0x20;

	i2crom_drv_write(&current_ROM, 0, (uint8*)&rom_format, sizeof(t_ROM_format));
}

/* Clear the user info in ROM */
void i2crom_user_info_clear()
{
//	t_ROM_structure rom_struct;
	uint8 buff[current_ROM.valid_info_size];
	memset(buff, 0xFF, current_ROM.valid_info_size);
	/* start address always is 0 now */
	i2crom_drv_write(&current_ROM, 0, buff, current_ROM.valid_info_size);
}

uint16 i2crom_user_info_dump(uint8* buff)
{
    uint8 i;
    /* start address always is 0 now */
    i2crom_drv_read(&current_ROM, 0, buff, current_ROM.valid_info_size);

    for(i = 0; i < current_ROM.valid_info_size/8; i++)
        I2C_ROM_DEBUG_PRINTF(("read %d bytes from ROM, 0x%02x 0x%02x 0x%02x 0x%02x "
                              "0x%02x 0x%02x 0x%02x 0x%02x\n\r",
                              8, buff[8*i], buff[8*i+1], buff[8*i+2], buff[8*i+3],
                              buff[8*i+4], buff[8*i+5], buff[8*i+6], buff[8*i+7]));

    return current_ROM.valid_info_size;
}

uint8 i2crom_user_info_flash(uint8* buff, uint8 startAddr, uint8 size)
{
	if(startAddr >= current_ROM.setting_start_addr)
	{
		I2C_ROM_PRINTF_ALWAYS(("Private data field in Info ROM start at %d\n", SETTING_START_ADDR));
		I2C_ROM_PRINTF_ALWAYS(("Can not flash image into the private data field\n"));
		return 0;
	}

	if((startAddr + size) > current_ROM.valid_info_size)
	{
		I2C_ROM_DEBUG_PRINTF(("the startAddr %d, size %d overflow the info ROM size %d\n",
				startAddr, size, INFO_ROM_SIZE));
		size = current_ROM.valid_info_size - startAddr;
		I2C_ROM_DEBUG_PRINTF(("correct the size to %d and continue to write to flash\n"));
	}

	i2crom_drv_write(&current_ROM, startAddr, buff, size);
	return size;
}

/* Erase the whole content in ROM, only for testing */
void i2crom_erase()
{
	uint8 buf[ROM_SIZE];
	memset(buf, 0, ROM_SIZE);
	i2crom_drv_write(&current_ROM, 0, buf, ROM_SIZE);
}

/******************************************************************************/
/*-------------------Specific functions for IDROM Format--------------------*/
/******************************************************************************/
static uint8 find_object(const uint8* object)
{
	uint8 i;
	for(i = 0; i < OBJECT_NUMBER; i++)
	{
		if(strcmp((char*)object, OBJECT_LIST[i].name) == 0)
		{
			break;
		}
	}
	return i;
}

static uint32 read_sys_object(uint8* pbuf)
{
	uint32 ret = 0;
	t_brd_info brd_info;
	i2crom_drv_read(&current_ROM, SYS_START_ADDR, (uint8*)&brd_info, SYS_SIZE);
	if((memcmp(brd_info.header.type, (uint8*)"sys", 3) == 0) ||
			(memcmp(brd_info.header.type, (uint8*)"SYS", 3) == 0))
	{
		char buffer[256];
		I2C_ROM_PRINTF_ALWAYS(("Version is %d.%d\n", brd_info.header.version, brd_info.header.subversion));

		memset(buffer, 0, 256);
		sprintf(buffer, "%02X%02X%02X%02X", brd_info.build_date[0], brd_info.build_date[1],
				brd_info.build_date[2], brd_info.build_date[3]);
		I2C_ROM_PRINTF_ALWAYS(("System Build Date: %s\n", buffer));

		memset(buffer, 0, 256);
		memcpy(buffer, brd_info.part_number, 24);
		I2C_ROM_PRINTF_ALWAYS(("System part number: %s\n", buffer));

		memset(buffer, 0, 256);
		sprintf(buffer, "%c%02X", brd_info.pcb_revision[0], brd_info.pcb_revision[1]);
		I2C_ROM_PRINTF_ALWAYS(("PCB Revision: %s\n", buffer));

		memset(buffer, 0, 256);
		sprintf(buffer, "%c%c%02X", brd_info.BOM_build_revision[0], brd_info.BOM_build_revision[1],
				brd_info.BOM_build_revision[2]);
		I2C_ROM_PRINTF_ALWAYS(("BOM build revision: %s\n", buffer));

		memset(buffer, 0, 256);
		memcpy(buffer, brd_info.system_serial_number, 16);
		I2C_ROM_PRINTF_ALWAYS(("System (board) Serial number: %s\n", buffer));
		ret = SYS_SIZE;
	}
	else
	{
		I2C_ROM_PRINTF_ALWAYS(("Header is not expected \"sys\"\n"));
		ret = 0;
	}
	return ret;
}

static uint32 read_rwk_object(uint8* pbuf)
{
	uint32 r_bytes = 0;
	t_brd_rwk brd_rwk;
	i2crom_drv_read(&current_ROM, RWK_START_ADDR, (uint8*)&brd_rwk, RWK_SIZE);
	if((memcmp(brd_rwk.header.type, (uint8*)"rwk", 3) == 0) ||
			(memcmp(brd_rwk.header.type, (uint8*)"RWK", 3) == 0))
	{

		I2C_ROM_PRINTF_ALWAYS(("Version is %d.%d\n", brd_rwk.header.version, brd_rwk.header.subversion));
		/* copy binaries data to buffer */
		r_bytes = RWK_SIZE - HEADER_SIZE;
		memcpy(pbuf, brd_rwk.rework_flag, r_bytes);

		/* Print out the ROM RWK info */
		/* Format is 1234_5678_9abc_def0_0123_4567_89ab_cdef ES */
		uint8 rework_flag_str[RWK_REWORK_FLAG_BYTES * 3];
		uint8 i=0, j=0;
		/* every two bytes needs to combine together*/
		for(i=0; i<RWK_REWORK_FLAG_BYTES; i++)
		{
			sprintf(&rework_flag_str[j*5], "%02X%02X_", brd_rwk.rework_flag[i], brd_rwk.rework_flag[i+1]);
			i++;
			j++;
		}
		/* remove the last character '_', replace it with '\0 */
		rework_flag_str[strlen(rework_flag_str)-1] = 0;
		I2C_ROM_PRINTF_ALWAYS(("Rework Flag is %s\n\r", rework_flag_str));
		uint8 release_level[RWK_RELEASE_LEVEL_BYTES+1];
		memset(release_level, 0, RWK_RELEASE_LEVEL_BYTES+1);
		memcpy(release_level, brd_rwk.release_level, RWK_RELEASE_LEVEL_BYTES);
		I2C_ROM_PRINTF_ALWAYS(("Release Level is %s\n\r", release_level));
	}
	else
	{
		I2C_ROM_PRINTF_ALWAYS(("Header is not expected \"rwk\"\n"));
	}
	return r_bytes;
}

static boolean write_sys_object(uint8* pbuf)
{
	I2C_ROM_PRINTF_ALWAYS(("SYS object is CONST content\n"));
	/*
	 * The below data will need to be updated as per the inputs
	 * from HW folks in the support for flashing Inforom is required.
	 */
#if INFOROM_GENERATE_IMAGE_ENABLE == 1
	t_brd_info brd_info;
	memset(&brd_info, 0, sizeof(t_brd_info));

	memcpy(brd_info.header.type, "sys", 3);

	brd_info.build_date[0]=0x20;
	brd_info.build_date[1]=0x17;
	brd_info.build_date[2]=0x10;
	brd_info.build_date[3]=0x20;

	memcpy(brd_info.part_number, "940-63550-0000-B00", 24);

	brd_info.pcb_revision[0] = 'A';
	brd_info.pcb_revision[1] = 0x0;

	brd_info.BOM_build_revision[0] = 'A';
	brd_info.BOM_build_revision[1] = 'A';
	brd_info.BOM_build_revision[2] = 0x00;

	memcpy(brd_info.system_serial_number, "E3550-B00-P000", 16);

	i2crom_drv_write(&current_ROM, SYS_START_ADDR, (uint8*)&brd_info, SYS_SIZE);
#endif
	return FALSE;
}

static boolean write_rwk_object(uint8* pbuf)
{
	t_brd_rwk brd_rwk;
	i2crom_drv_read(&current_ROM, RWK_START_ADDR, (uint8*)&brd_rwk, RWK_SIZE);

	if(memcmp(brd_rwk.header.type, (uint8*)"rwk", 3) != 0)
	{
		I2C_ROM_DEBUG_PRINTF(("Header is not correct, format the ROM with header\n"));
		/* do we add the header type here? */
		memcpy(brd_rwk.header.type, "rwk", 3);
	}
	else
	{
		I2C_ROM_DEBUG_PRINTF(("Header is correct\n"));
	}
	/* analyze the content */
	/* format 1234_5678_9abc_def0_0123_4567_89ab_cdef#ES, that's rwk_flag#release_level */
	uint8* rwk_flag = strtok(pbuf, "#");
	uint8* release_level = strtok(NULL, "#");

	if(rwk_flag != NULL)
	{
		I2C_ROM_PRINTF_ALWAYS(("rwk_flag is %s\n", rwk_flag));
		memset(brd_rwk.rework_flag, 0xFF, RWK_REWORK_FLAG_BYTES);
		uint8* rwk_2byte_str = strtok(rwk_flag, "_");
		uint8 rwk_byte_str_h[3];
		uint8 rwk_byte_str_l[3];
		//uint8 rwk_byte_int = 0;
		uint8 index = 0;
		while(rwk_2byte_str != NULL)
		{
			if(strlen(rwk_2byte_str) > 2)
			{
				/* divide the 2byte into 2 single bytes */
				memset(rwk_byte_str_l, 0, sizeof(rwk_byte_str_l));
				memset(rwk_byte_str_h, 0, sizeof(rwk_byte_str_h));
				strncpy(rwk_byte_str_l, rwk_2byte_str, 2);
				strncpy(rwk_byte_str_h, &rwk_2byte_str[2], 2);
				I2C_ROM_DEBUG_PRINTF(("rwk_byte_str_l is %s\n", rwk_byte_str_l));
				I2C_ROM_DEBUG_PRINTF(("rwk_byte_str_h is %s\n", rwk_byte_str_h));

				/* Convert the byte string to uint8 */
				brd_rwk.rework_flag[index++] = (uint8) strtol(rwk_byte_str_l, NULL, 16);
				if(index > RWK_REWORK_FLAG_BYTES)
				{
					I2C_ROM_PRINTF_ALWAYS(("The rework flag exceed max size %d, ignore the rest\n", RWK_REWORK_FLAG_BYTES));
					break;
				}
				brd_rwk.rework_flag[index++] = (uint8) strtol(rwk_byte_str_h, NULL, 16);
				if(index > RWK_REWORK_FLAG_BYTES)
				{
					I2C_ROM_PRINTF_ALWAYS(("The rework flag exceed max size %d, ignore the rest\n", RWK_REWORK_FLAG_BYTES));
					break;
				}
			}
			else
			{
				I2C_ROM_PRINTF_ALWAYS(("Rework flag needs to follow format xxxx_xxxx...\n"));
			}
			rwk_2byte_str = strtok(NULL, "_");
		}
	}
	else
	{
		I2C_ROM_PRINTF_ALWAYS(("Can not get rwk_flag, please make sure format is rwk_flag#release_level\n"));
	}
	if(release_level != NULL)
	{
		I2C_ROM_PRINTF_ALWAYS(("release_level is %s\n", release_level));
		memset(brd_rwk.release_level, 0xFF, RWK_RELEASE_LEVEL_BYTES);
		strncpy(brd_rwk.release_level, release_level, 2);
	}
	else
	{
		I2C_ROM_PRINTF_ALWAYS(("Can not get release_level, please make sure format is rwk_flag#release_level\n"));
	}
	i2crom_drv_write(&current_ROM, RWK_START_ADDR, (uint8*)&brd_rwk, RWK_SIZE);
	return TRUE;
}

/******************************************************************************/
/*-------------------Specific functions for IDROM Usage (not command) --------*/
/******************************************************************************/
static void revert_string(uint8* target_buf, uint8* origin_buf, uint8 size)
{
	uint8 j = size - 1, i, string_size;
	while(origin_buf[j]==0)
	{ /* ignore all the 0 at end of origin_buf */
		if(j>0)
		{
			j--;
		}
		else
		{
			break;
		}
	}
	if(j==0)
	{ /* origin_buf are all 0, can't do string revert */
		return;
	}
	/* j points to the last non 0 item of origin_buf */
	string_size = j+1;
	for(i=0; i<string_size; i++)
	{
		target_buf[i] = origin_buf[j];
		j--;
	}
}



/*compare the type with the type reading from ROM, which is reverted */
static boolean compare_object_type(const uint8* type1, const uint8* type2)
{
	uint8 buf[3];
	revert_string(buf, type1, 3);
	if(memcmp(buf, type2, 3) == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/* This funciton to get the dynamic offset of each object */
static boolean get_object_offset(uint8* type, uint32 * offset_p)
{
	t_ROM_fat rom_fat;
	uint8 i;

	i2crom_drv_read(&E3550_ROM, 0, (uint8*)&rom_fat, ROM_FAT_SIZE);
	/* Compared to ROM object, but it's reverted */
	if(compare_object_type(rom_fat.header.type, (uint8*)"ROM"))
	{
		for(i=0; i<OBJ_NUMBER; i++)
		{
			if(compare_object_type(rom_fat.objects[i].type, type))
			{
				*offset_p = rom_fat.objects[i].location;
				return TRUE;
			}
		}
		if(i==3)
		{
			I2C_ROM_PRINTF_ALWAYS(("Warning: Can NOT find object type in ROM table!!!\r\n"));
			return FALSE;
		}
	}
	else
	{
		I2C_ROM_PRINTF_ALWAYS(("Warning: Read inforom: Header is not programmed in \"ROM\" object!!!\r\n"));
		return FALSE;
	}
}

/* specific function to operate main board ROM (E3550) */
/* so we do not use current_ROM here as they don't follow the ROM parameter setting */

/*   Return value CE_MODE:
 *   0:  Both Tegra Control DGPUs
 *   1:  COM Express control DGPU A, Tegra DGPU B
 *   2:  COM Express control DGPU B, Tegra DGPU A
 *   3:  COM Express control DGPU A and DGPU B
 */
uint8 i2crom_read_ce_mode()
{
	t_brd_info brd_info;
	uint8 ce_mode = 0;

	uint32 offset;
	if(!get_object_offset((uint8*)"SYS", &offset))
	{
		return FALSE;
	}

	i2crom_drv_read(&E3550_ROM, offset, (uint8*)&brd_info, SYS_SIZE);
	if((memcmp(brd_info.header.type, (uint8*)"sys", 3) == 0) ||
			(memcmp(brd_info.header.type, (uint8*)"SYS", 3) == 0))
	{
		/* Read Mode (byte 56); byte 57 is reserved */
		memcpy(&ce_mode, &brd_info.mode, 1);
		I2C_ROM_DEBUG_PRINTF(("Read inforom: CE_Mode: %d\r\n", *ce_mode));
		return ce_mode;
	}
	else
	{
		I2C_ROM_PRINTF_ALWAYS(("Warning: Read inforom: Header is not programmed in \"SYS\" object!!!\r\n"));
		I2C_ROM_PRINTF_ALWAYS(("         CE_Mode may not be programmed! Assume CE_Mode 0\r\n"));
		return 0;
	}
}

/*   Possible values CE_MODE :
 *   0:  Both Tegra Control DGPUs
 *   1:  COM Express control DGPU A, Tegra DGPU B
 *   2:  COM Express control DGPU B, Tegra DGPU A
 *   3:  COM Express control DGPU A and DGPU B
 */
void i2crom_write_ce_mode(uint8 ce_mode)
{

//	t_brd_info brd_info;
	uint32 ce_mode_offset;
	uint8 mode_l = 0;
	uint8* mode_p = &mode_l;
	uint32 offset;

	if (ce_mode > 3)
	{
		I2C_ROM_PRINTF_ALWAYS(("Error: Incorrect CE Mode selected \n"));
		return;
	}

	if(!get_object_offset((uint8*)"SYS", &offset))
	{
		return;
	}

	/* CE mode is byte 56; byte 57 is reserved */
	*mode_p = ce_mode;
	ce_mode_offset = offset + CE_MODE_BYTE_OFFSET;
	i2crom_drv_write(&E3550_ROM, ce_mode_offset, mode_p, 1);
	I2C_ROM_DEBUG_PRINTF(("Programming CE MODE = %d\n", boot_media));
	return;
}

/* Possible value for boot medium
 * 0: UFS
 * 1: Qspi
 * 2: Emmc
 */
void i2crom_read_boot_media(uint8* boot_medium_ta, uint8* boot_medium_tb)
{
	t_brd_info brd_info;
	uint8 boot_medium_l = 0;
	uint8* boot_medium_p = &boot_medium_l;

	uint32 offset;
	if(!get_object_offset((uint8*)"SYS", &offset))
	{
		return;
	}

	i2crom_drv_read(&E3550_ROM, offset, (uint8*)&brd_info, SYS_SIZE);
	if((memcmp(brd_info.header.type, (uint8*)"sys", 3) == 0) ||
			(memcmp(brd_info.header.type, (uint8*)"SYS", 3) == 0))
	{
		/* Read bootmedia (byte 58)
		 * Upper nibble is for TA, lower nibble is for TB */
		memcpy(boot_medium_p, &brd_info.bootmedia, 1);
		*boot_medium_ta = ((*boot_medium_p >> 4) & 0x0F);
		*boot_medium_tb = ((*boot_medium_p) & 0x0F);
		I2C_ROM_DEBUG_PRINTF(("Read inforom: Boot Medium for Tegra A = %d\r\n", *boot_medium_ta));
		I2C_ROM_DEBUG_PRINTF(("Read inforom: Boot Medium for Tegra B = %d\r\n", *boot_medium_tb));
		return;
	}
	else
	{
		I2C_ROM_PRINTF_ALWAYS(("Warning: Read inforom: Header is not programmed in \"SYS\" object!!!\r\n"));
		I2C_ROM_PRINTF_ALWAYS(("         Boot Medium may not be programmed! Assume default Boot Medium 1 (qspi)\r\n"));
		/* set default boot medium = 1 for Qspi */
		*boot_medium_ta = 1;
		*boot_medium_tb = 1;
		return;
	}
}

/* Possible values for boot_media
 * 0: UFS
 * 1: Qspi
 * 2: Emmc
 */
void i2crom_write_boot_media(uint8 boot_medium_ta, uint8 boot_medium_tb)
{
	t_brd_info brd_info;
	uint32 boot_media_offset;
	uint8 boot_medium_l = 0;
	uint8* boot_medium_p = &boot_medium_l;

	if (boot_medium_ta > 2)
	{
		I2C_ROM_PRINTF_ALWAYS(("Error: Incorrect boot medium selected for TA \n"));
		return;
	}
	if (boot_medium_tb > 2)
	{
		I2C_ROM_PRINTF_ALWAYS(("Error: Incorrect boot medium selected for TB \n"));
		return;
	}

	uint32 offset;
	if(!get_object_offset((uint8*)"SYS", &offset))
	{
		return;
	}

	/* Write boot_medium (byte 58)
	 * Upper nibble is for TA, lower nibble is for TB */
	boot_media_offset = offset + BOOT_MEDIA_BYTE_OFFSET;
	*boot_medium_p = ((boot_medium_ta << 4)& 0xF0) | (boot_medium_tb & 0x0F);
	i2crom_drv_write(&E3550_ROM, boot_media_offset, boot_medium_p, 1);
	I2C_ROM_DEBUG_PRINTF(("Programming Boot Medium for TA = %d\n", boot_medium_ta));
	I2C_ROM_DEBUG_PRINTF(("Programming Boot Medium for TB = %d\n", boot_medium_tb));
	return;
}

/* operation on CIM ROM */
/*\brief read CIM SKU from IDrom in CIM
 *\param: in uint8* the buffer pointer to store the sku, buffer size >= 24
 *\return void */
void i2crom_read_cim_sku(uint8* pbuf)
{
	uint8 buffer[CIM_SYS_PART_NUMBER_SIZE];
	uint8 buffer_reorder[CIM_SYS_PART_NUMBER_SIZE];
	memset(buffer, 0, CIM_SYS_PART_NUMBER_SIZE);
	memset(buffer_reorder, 0, CIM_SYS_PART_NUMBER_SIZE);
	/* TODO: verify the header */
	i2crom_drv_read(&CIM_ROM, CIM_SYS_START_ADDR + CIM_SYS_PART_NUMBER_OFFSET, buffer, CIM_SYS_PART_NUMBER_SIZE);
	revert_string(buffer_reorder, buffer, CIM_SYS_PART_NUMBER_SIZE);
	memcpy(pbuf, buffer_reorder, CIM_SYS_PART_NUMBER_SIZE);

#if I2C_ROM_DEBUG_ENABLE == 1
	uint8 buffer_print[CIM_SYS_PART_NUMBER_SIZE+2];
	memset(buffer_print, 0, CIM_SYS_PART_NUMBER_SIZE+2);
	memcpy(buffer_print, buffer, CIM_SYS_PART_NUMBER_SIZE);
	I2C_ROM_DEBUG_PRINTF(("read out the sku is %s\r\n", buffer_print));
#endif
}


boolean i2crom_getPartNumber(uint8* part_number)
{
	t_brd_info brd_info;
	uint8 buffer[24];

	uint32 offset;
	if(!get_object_offset((uint8*)"SYS", &offset))
	{
		return FALSE;
	}

	i2crom_drv_read(&E3550_ROM, offset, (uint8*)&brd_info, SYS_SIZE);
	if(compare_object_type(brd_info.header.type, (uint8*)"SYS"))
	{
		/* Read Part Number */
		memcpy(buffer, &brd_info.part_number, 24);
		revert_string(part_number, buffer, 24);
		return TRUE;
	}
	else
	{
		I2C_ROM_PRINTF_ALWAYS(("Warning: Read inforom: Header is not programmed in \"SYS\" object!!!\r\n"));
		I2C_ROM_PRINTF_ALWAYS(("Warning: Fail to get board number\n"));
		return FALSE;
	}
}

void i2crom_geROM_test(void)
{
//    E3550_ROM.i2c_address = 0x54;
    IfxStdIf_DPipe_print(io_tmp, "i2crom_geROM_test test.i2c_bus = %d, test.i2c_address = 0x%x\n\r", 0x99, 0x99);
    IfxStdIf_DPipe_print(io_tmp, "i2crom_geROM_test current_ROM.i2c_bus = %d, current_ROM.i2c_address = 0x%x\r\n", current_ROM.i2c_bus, current_ROM.i2c_address);
    IfxStdIf_DPipe_print(io_tmp, "i2crom_geROM_test E3550_ROM.i2c_bus = %d, E3550_ROM.i2c_address = 0x%x\r\n", E3550_ROM.i2c_bus, E3550_ROM.i2c_address);
//    I2C_ROM_PRINTF_ALWAYS(("i2crom_geROM_test E3550_ROM.i2c_bus = %d, E3550_ROM.i2c_address = 0x%x\n", E3550_ROM.i2c_bus, E3550_ROM.i2c_address));
}


boolean i2crom_getPcbRev(uint8* pcbRev)
{
	t_brd_info brd_info;
	uint8 buffer[24];

	uint32 offset;
	if(!get_object_offset((uint8*)"SYS", &offset))
	{
		return FALSE;
	}
	i2crom_drv_read(&E3550_ROM, offset, (uint8*)&brd_info, SYS_SIZE);
	if(compare_object_type(brd_info.header.type, (uint8*)"SYS"))
	{
		/* Read Part Number */
		memcpy(buffer, &brd_info.pcb_revision, 2);
		revert_string(pcbRev, buffer, 2);
		return TRUE;
	}
	else
	{
		I2C_ROM_PRINTF_ALWAYS(("Warning: Read inforom: Header is not programmed in \"SYS\" object!!!\r\n"));
		I2C_ROM_PRINTF_ALWAYS(("Warning: Fail to get pcb reverison\r\n"));
		return FALSE;
	}
}

boolean i2crom_getEthMac(uint8* ethMac, e_AurixIndex aurix_index)
{
	t_brd_info brd_info;

	uint32 offset;
	if(!get_object_offset((uint8*)"SYS", &offset))
	{
		return FALSE;
	}
	i2crom_drv_read(&E3550_ROM, offset, (uint8*)&brd_info, SYS_SIZE);
	if(compare_object_type(brd_info.header.type, (uint8*)"SYS"))
	{
		/* Read Part Number */
		if(aurix_index == MAIN_AURIX)
			memcpy(ethMac, &brd_info.Aurix_mac_address, 6);
		else if(aurix_index == SECOND_AURIX)
			memcpy(ethMac, &brd_info.Aurix2_mac_address, 6);
		else
		{
			I2C_ROM_PRINTF_ALWAYS(("Warning: Read inforom: Fail to get ethernet mac address with invalid aurix index %d\r\n", aurix_index));
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		I2C_ROM_PRINTF_ALWAYS(("Warning: Read inforom: Header is not programmed in \"SYS\" object!!!\r\n"));
		I2C_ROM_PRINTF_ALWAYS(("Warning: Fail to get ethernet mac address\r\n"));
		return FALSE;
	}
}

/******************************************************************************/
/*------------------------------- ROM TEST -------------------------------*/
/******************************************************************************/
void run_i2crom_test()
{
	uint32 i;
	uint8 wbuf[TEST_SIZE];
	uint8 rbuf[TEST_SIZE];
	uint8 backup_buf[TEST_SIZE];

	I2C_ROM_PRINTF_ALWAYS(("Start I2C test on ROM M24C08\r\n"));

	I2C_ROM_PRINTF_ALWAYS(("Back up the data from ROM\r\n"));
	i2crom_drv_read(&current_ROM, 0, backup_buf, TEST_SIZE);

	/* init the random data */
	memset(rbuf, 0, TEST_SIZE);
	srand(random_seed);
	/* prepare the data */
	for(i=0; i<TEST_SIZE; i++)
		wbuf[i] = rand() & 0xFF;

	i2crom_drv_write(&current_ROM, 0, wbuf, TEST_SIZE);

	/* read the data from ROM */
	i2crom_drv_read(&current_ROM, 0, rbuf, TEST_SIZE);

	I2C_ROM_PRINTF_ALWAYS(("Restore the data to ROM\r\n"));
	i2crom_drv_write(&current_ROM, 0, backup_buf, TEST_SIZE);

	if(memcmp(wbuf, rbuf, TEST_SIZE) == 0)
	{
		IfxStdIf_DPipe_print(io_tmp, ("Read back the data and it's the same as the written data!!\r\n"));
		IfxStdIf_DPipe_print(io_tmp, ("/**********************/\r\n"));
		IfxStdIf_DPipe_print(io_tmp, "  I2C ROM Test PASS!!  \n\r");
		IfxStdIf_DPipe_print(io_tmp, ("/**********************/\r\n"));
	}
	else
	{
		IfxStdIf_DPipe_print(io_tmp, ("Read back the data but it's different from the written data!!\r\n"));
		IfxStdIf_DPipe_print(io_tmp, ("/**********************/\r\n"));
		IfxStdIf_DPipe_print(io_tmp, (" !!I2C ROM Test FAIL!!  \n\r"));
		IfxStdIf_DPipe_print(io_tmp, ("/**********************/\r\n"));
	}
}


#endif //#if (PROJECT == PROJECT_DRIVE_AX) && (DRIVE_AX_B0x == 1)
