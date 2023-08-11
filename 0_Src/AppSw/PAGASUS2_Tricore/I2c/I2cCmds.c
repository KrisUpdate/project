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

#include "Configuration.h"
#include "SysSe/Comm/Ifx_Shell.h"
#include "SysSe/Bsp/Bsp.h"
#include "I2cCmds.h"
#include "I2c.h"
#include <internal_fw_debug.h>
#include "stdlib.h"
#include "I2cRom.h"
#include "board.h"
#include "string.h"
#include "resource.h"
#include "I2crom_drv.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define I2C_CMD_DEBUG_ENABLE 0

#if I2C_CMD_DEBUG_ENABLE == 1
#define I2C_CMD_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define I2C_CMD_DEBUG_PRINTF(x)
#endif
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

volatile uint32 random_seed = 0;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \brief Handle the 'i2cinit' command
 *
 * \par Syntax
 *  - i2cscan : scan the i2c module for devices
 */
boolean i2cinit(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 i2c_module_number, baudrate;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax     : i2cinit x y"ENDL);
		IfxStdIf_DPipe_print(io, "           > init i2c bus x with baurdrate y"ENDL);
		IfxStdIf_DPipe_print(io, "           > the driver doesn't support speed >400K "ENDL);
	}
	else
	{
		// now we get the first argument (i2c module number)
		if (Ifx_Shell_parseUInt32(&args, &i2c_module_number, FALSE) == FALSE)
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c module %s"ENDL, args);
			return TRUE;
		}
		if (i2c_module_number >= NUM_I2C_MODULE)
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c module %d"ENDL, i2c_module_number);
			return TRUE;
		}

		if (Ifx_Shell_parseUInt32(&args, &baudrate, FALSE) == FALSE)
		{
			baudrate = 400000;
		}
        if(i2c_module_number == 0){
        }else{
        }
		i2c_init((uint8)i2c_module_number, baudrate);
        if(i2c_module_number == 0){
        }else{
        }
		IfxStdIf_DPipe_print(io, "initialize i2c bus %d at baudrate %d"ENDL, i2c_module_number, baudrate);
	}

	return TRUE;
}


/** \brief Handle the 'i2cscan' command
 *
 * \par Syntax
 *  - i2cscan : scan the i2c module for devices
 */
boolean i2cscan(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 i2c_module_number, uiCnt;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : i2cscan x"ENDL);
        IfxStdIf_DPipe_print(io, "           > Scan the i2c module x for devices"ENDL);
    }
    else
    {
    	// now we get the first argument (i2c module number)
    	if (Ifx_Shell_parseUInt32(&args, &i2c_module_number, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c module %s"ENDL, args);
            return TRUE;
        }
        if (i2c_module_number >= NUM_I2C_MODULE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c module %d"ENDL, i2c_module_number);
            return TRUE;
        }
    	//i2c_init((uint8)i2c_module_number);
        IfxStdIf_DPipe_print(io, "Scanning for device..."ENDL);


    	for(uiCnt=1; uiCnt<0x7F; uiCnt++)
    	{
    		if (i2c_check_device((uint8)i2c_module_number,uiCnt) == TRUE)
    		{
    			// we found a device
    	        IfxStdIf_DPipe_print(io, "Found a device on address 0x%02x"ENDL, uiCnt);
    		}
    	}


        IfxStdIf_DPipe_print(io, "Done"ENDL);
    }

    return TRUE;
}

/** \brief Handle the 'i2cread' command
 *
 * \par Syntax
 *  - i2cread x y z n: read from i2c device x at address y n bytes
 */
boolean i2cread(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 i2c_module_number, devAddr, address, count, uiCnt, uiCnt2, bytesleft;
    uint8 datas[256];
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : i2cread x y z n"ENDL);
        IfxStdIf_DPipe_print(io, "           > Read from i2c module x device y at offset z n bytes"ENDL);
    }
    else
    {
    	// now we get the first argument (i2c module)
    	if (Ifx_Shell_parseUInt32(&args, &i2c_module_number, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c module %s"ENDL, args);
            return TRUE;
        }
        if (i2c_module_number > 2)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c module %d"ENDL, i2c_module_number);
            return TRUE;
        }
    	if (Ifx_Shell_parseUInt32(&args, &devAddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device address %s"ENDL, args);
            return TRUE;
        }
        if (devAddr > 0x7F)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device address %d"ENDL, devAddr);
            return TRUE;
        }
    	// now we get the second argument (i2c address)
    	if (Ifx_Shell_parseUInt32(&args, &address, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %s"ENDL, args);
            return TRUE;
        }
    	// now we get the third argument (number of bytes to read)
    	if (Ifx_Shell_parseUInt32(&args, &count, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid count %s"ENDL, args);
            return TRUE;
        }
    	//i2c_init((uint8)i2c_module_number);

        while (count)
        {
        	uiCnt = count;
        	if (uiCnt > 256) uiCnt = 256;
        	bytesleft = i2c_read_bytes((uint8)i2c_module_number,(uint8)devAddr, (uint8)address, uiCnt, &datas[0],1);
        	if (bytesleft)
        	{
                IfxStdIf_DPipe_print(io, "Error : %d bytes not read"ENDL, bytesleft);
                return TRUE;
        	}
        	for (uiCnt2=0; uiCnt2<uiCnt; uiCnt2++)
        	{
        		if(((uiCnt2 % 16) == 0) && (uiCnt2 != 0))
        			IfxStdIf_DPipe_print(io, " "ENDL);
                IfxStdIf_DPipe_print(io, "0x%02x  ", datas[uiCnt2]);
        	}
            IfxStdIf_DPipe_print(io, " "ENDL);
        	count -= uiCnt;
        }
    }
    return TRUE;
}

/** \brief Handle the 'i2cwrite' command
 *
 * \par Syntax
 *  - i2cwrite x y z n ...: write to i2c device x at address y n bytes
 */
boolean i2cwrite(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 i2c_module_number, devAddr, address, count, uiCnt, uiCnt2, bytesleft, value;
    uint8 datas[16];
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : i2cwrite x y z n <bytes to be written>"ENDL);
        IfxStdIf_DPipe_print(io, "           > Write to i2c module x device y at offset z n bytes"ENDL);
    }
    else
    {
    	// now we get the first argument (i2c module)
    	if (Ifx_Shell_parseUInt32(&args, &i2c_module_number, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c module %s"ENDL, args);
            return TRUE;
        }
        if (i2c_module_number > 2)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c module %d"ENDL, i2c_module_number);
            return TRUE;
        }
        // second argument
    	if (Ifx_Shell_parseUInt32(&args, &devAddr, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device address %s"ENDL, args);
            return TRUE;
        }
        if (devAddr > 0x7F)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device address %d"ENDL, devAddr);
            return TRUE;
        }
    	// now we get the third argument (i2c address)
    	if (Ifx_Shell_parseUInt32(&args, &address, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid address %s"ENDL, args);
            return TRUE;
        }
    	// now we get the fourth argument (number of bytes to read)
    	if (Ifx_Shell_parseUInt32(&args, &count, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid count %s"ENDL, args);
            return TRUE;
        }
    	//i2c_init((uint8)i2c_module_number);

        while (count)
        {
        	uiCnt = count;
        	if (uiCnt > 16) uiCnt = 16;
        	for (uiCnt2=0; uiCnt2<uiCnt; uiCnt2++)
        	{
        		// we get here the bytes to write
            	// now we get the fifth argument (number of bytes to read)
            	if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
                {
                    IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
                    return TRUE;
                }
        		datas[uiCnt2] = (uint8)value;
        	}

        	bytesleft = i2c_write_bytes((uint8)i2c_module_number,(uint8)devAddr, (uint8)address, uiCnt, &datas[0]);
        	if (bytesleft)
        	{
                IfxStdIf_DPipe_print(io, "Error : %d bytes not written"ENDL, bytesleft);
                return TRUE;
        	}
        	count -= uiCnt;
        }
    }

    return TRUE;
}

/** \brief Handle the 'i2cwrite' command
 *
 * \par Syntax
 *  - i2cwrite x y z n ...: write to i2c device x at address y n bytes
 */
boolean i2cromtest(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 loop;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : i2cromtest x >"ENDL);
        IfxStdIf_DPipe_print(io, "           > start the i2c test for loop x on the ROM M24C08"ENDL);
    }
    else
    {
    	if (Ifx_Shell_parseUInt32(&args, &loop, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c test loop %s"ENDL, args);
            return TRUE;
        }
        if (loop == 0)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid loop %d, needs to be larger than 0"ENDL, loop);
            return TRUE;
        }
    	//i2c_init((uint8)i2c_module_number);
        while (loop)
        {
        	run_i2crom_test(io);
        	loop--;
        }
    }

    return TRUE;
}

typedef enum
{
	TYPE_NAME = 0,
	TYPE_ID,
	TYPE_MAX
}e_object_type;

static e_object_type get_object_type(uint8* p, uint8* name_id)
{
	uint8* type = strtok(p, "=");
	uint8* l_name_id=  strtok(NULL, "=");
	if((type == NULL) || (l_name_id == NULL))
	{
		return TYPE_MAX;
	}
	strcpy(name_id, l_name_id);
	if(memcmp(type, (uint8*)"object-name", sizeof("object-name")-1) == 0)
	{
		return TYPE_NAME;
	}
	if(memcmp(type, (uint8*)"object-id", sizeof("object-id")-1) == 0)
	{
		return TYPE_ID;
	}
	return TYPE_MAX;
}

/** \brief Handle the 'i2cromrw' command
 *
 * \par Syntax
 *  - i2crom r/w/c : read/write/clear i2c rom
 */

#define INPUT_BUFFER_SIZE   256
#define OUTPUT_BUFFER_SIZE  256
#define CLEAN_BUFFER(x)     memset(x, 0, Ifx_COUNTOF(x))
boolean i2cromrw(pchar args, void *data, IfxStdIf_DPipe *io)
{
	(void)data;
    uint8    output_buffer[OUTPUT_BUFFER_SIZE];
    uint8    input_buffer[INPUT_BUFFER_SIZE];
    CLEAN_BUFFER(output_buffer);
    CLEAN_BUFFER(input_buffer);
    uint32 i2cbus = 0;
    uint32 i2caddress = 0;

	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : inforom set/clear/dump/flash/read/write [object] [content]>"ENDL);
        IfxStdIf_DPipe_print(io, "           > inforom set i2cbus x address y\n\r"
							 "               >>  set the rom parameter before operating\n\r");
        IfxStdIf_DPipe_print(io, "           > inforom clear >  erase the inforom completely"ENDL);
        IfxStdIf_DPipe_print(io, "           > inforom dump  >  dump the inforom content in hex format"ENDL);
        IfxStdIf_DPipe_print(io, "           > inforom flash addr size byte1 byte2 ... \n\r"
							 "               >>  flash the image (bytes) into the inforom\n\r"
							 "                   \"addr\":start address, \"size\":size of bytes\n\r"
							 "                   Please notice that size should be <= 10 bytes"ENDL);
        IfxStdIf_DPipe_print(io, "           > inforom read object-name=\"text name\" [format=HEX]\n\r"
							 "               >>  read the object info by object name [in hex format]"ENDL);
        IfxStdIf_DPipe_print(io, "           > inforom read object-id=0xHEXID [format=HEX]\n\r"
							 "               >>  read the object info by object id [in hex format]"ENDL);
        IfxStdIf_DPipe_print(io, "           > inforom write object-name=\"text name\" string\n\r"
							 "               >>  write the \"string\" into object by object-name"ENDL);
        IfxStdIf_DPipe_print(io, "           > inforom write object-id=0xHEXID string\n\r"
							 "               >>  write the \"string\" into object by object-id"ENDL);
    }
	else
	{
		if(Ifx_Shell_matchToken(&args, "set") != FALSE)
		{
			if(Ifx_Shell_matchToken(&args, "i2cbus") != FALSE)
			{
		    	if (Ifx_Shell_parseUInt32(&args, &i2cbus, FALSE) == FALSE)
		        {
		            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c bus %s"ENDL, args);
		            return TRUE;
		        }
		    	if(Ifx_Shell_matchToken(&args, "address") != FALSE)
		    	{
		    		if (Ifx_Shell_parseUInt32(&args, &i2caddress, FALSE) == FALSE)
					{
						IfxStdIf_DPipe_print(io, "Syntaxerror : invalid i2c address %s"ENDL, args);
						return TRUE;
					}
		    		/* we got parameter here */
		    		i2crom_user_info_set_rom(i2cbus, i2caddress);
		    		return TRUE;
		    	}
		    	else
				{
					IfxStdIf_DPipe_print(io, "Can not get key word \"address\", please use inforom ? for help"ENDL);
					return TRUE;
				}
			}
			else
			{
				IfxStdIf_DPipe_print(io, "Can not get key word \"i2cbus\", please use inforom ? for help"ENDL);
				return TRUE;
			}
		}

		if(Ifx_Shell_matchToken(&args, "clear") != FALSE)
		{
			i2crom_user_info_clear();
			IfxStdIf_DPipe_print(io, "Erased the info ROM!"ENDL);
		}
#if INFOROM_GENERATE_IMAGE_ENABLE == 1
		else if(Ifx_Shell_matchToken(&args, "initial") != FALSE)
		{
			i2crom_user_info_initial();
			IfxStdIf_DPipe_print(io, "Initial the info ROM!"ENDL);
		}
#endif
		else if(Ifx_Shell_matchToken(&args, "dump") != FALSE)
		{
			uint16 size = i2crom_user_info_dump(output_buffer);
			IfxStdIf_DPipe_print(io, "Dump the info from ROM, the info size is %d\n"ENDL, size);
			DEBUG_PRINTF_BUFFER(output_buffer, size);
		}
		else if(Ifx_Shell_matchToken(&args, "flash") != FALSE)
		{
			int startAddr=0;
			int size=0;
			if (Ifx_Shell_parseToken(&args, (char*)input_buffer, Ifx_COUNTOF(input_buffer)) != FALSE)
			{
				sscanf((char*)input_buffer, "%d", &startAddr);
				I2C_CMD_DEBUG_PRINTF(("start address %d"ENDL, startAddr));
			}
			else
			{
				IfxStdIf_DPipe_print(io, "Can not get start address\n"ENDL);
				return TRUE;
			}
			if (Ifx_Shell_parseToken(&args, (char*)input_buffer, Ifx_COUNTOF(input_buffer)) != FALSE)
			{
				sscanf((char*)input_buffer, "%d", &size);
				I2C_CMD_DEBUG_PRINTF(("get the size %d"ENDL, size));
			}
			else
			{
				IfxStdIf_DPipe_print(io, "Can not get the size\n"ENDL);
				return TRUE;
			}
			if(size>0)
			{
				int i;
				int byte;
				for(i = 0; i<size; i++)
				{
					CLEAN_BUFFER(input_buffer);
					if (Ifx_Shell_parseToken(&args, (char*)input_buffer, Ifx_COUNTOF(input_buffer)) != FALSE)
					{
						I2C_CMD_DEBUG_PRINTF(("get %s"ENDL, input_buffer));
						sscanf((char*)input_buffer, "%x", &byte);
						output_buffer[i]= (uint8)byte;
					}
				}
				size = i2crom_user_info_flash(output_buffer, startAddr, size);
				if(size > 0)
					IfxStdIf_DPipe_print(io, "Flash image into the info ROM from %d ~ %d"ENDL, startAddr, startAddr + size);
				else
					IfxStdIf_DPipe_print(io, "Fail to flash into the info ROM\r\n");
			}
		}
		else if(Ifx_Shell_matchToken(&args, "read") != FALSE)
		{
			if (Ifx_Shell_parseToken(&args, (char*)input_buffer, Ifx_COUNTOF(input_buffer)) != FALSE)
			{
				uint8 name_id[20];
				CLEAN_BUFFER(name_id);
				e_object_type object_type = get_object_type(input_buffer, name_id);
				sint32 rBytes = -1;
				uint8 id;
				switch(object_type)
				{
				case TYPE_NAME:
					rBytes = i2crom_user_info_read(output_buffer, name_id, 0);
					break;
				case TYPE_ID:
					id = atoi((char*)name_id);
					rBytes = i2crom_user_info_read(output_buffer, NULL, id);
					break;
				default:
					IfxStdIf_DPipe_print(io, "Syntaxerror : Please specific object-name=\"text name\""
												"or object-id=0xHEXID"ENDL);
					return TRUE;
				}
				if(rBytes == -1)
				{
					IfxStdIf_DPipe_print(io, "The object name/id is not supported"ENDL);
				}
				else if(rBytes == 0)
				{
					IfxStdIf_DPipe_print(io, "I2C ROM is empty"ENDL);
				}
			}
			else
			{
				IfxStdIf_DPipe_print(io, "Syntaxerror : Please specific the object "ENDL);
			}
		}
		else if(Ifx_Shell_matchToken(&args, "write") != FALSE)
		{
			if (Ifx_Shell_parseToken(&args, (char*)input_buffer, Ifx_COUNTOF(input_buffer)) != FALSE)
			{
				uint8 name_id[50];
				e_object_type object_type = get_object_type(input_buffer, name_id);
				uint8 id;
				boolean wret = FALSE;
				if (Ifx_Shell_parseToken(&args, (char*)input_buffer, Ifx_COUNTOF(input_buffer)) == FALSE)
				{
					IfxStdIf_DPipe_print(io, "Syntaxerror : Please specific the content to write"ENDL);
					IfxStdIf_DPipe_print(io, "Use inforom ? to get help message"ENDL);
					return TRUE;
				}
				switch(object_type)
				{
				case TYPE_NAME:
					wret = i2crom_user_info_write(input_buffer, name_id, 0);
					break;
				case TYPE_ID:
					id = atoi((char*)name_id);
					wret = i2crom_user_info_write(input_buffer, NULL, id);
					break;
				default:
					IfxStdIf_DPipe_print(io, "Syntaxerror : Please specific object-name=\"text name\""
												"or object-id=0xHEXID"ENDL);
					return TRUE;
				}
				if(!wret)
				{
					IfxStdIf_DPipe_print(io, "Fail to write, the object name/id is not supported or read-only"ENDL);
				}
				else
				{
					IfxStdIf_DPipe_print(io, "Have written the object info into ROM"ENDL);
				}
			}
			else
			{
				IfxStdIf_DPipe_print(io, "Syntaxerror : Please specific the object "ENDL);
			}
		}
		else if(Ifx_Shell_matchToken(&args, "r") != FALSE){
			uint8 tmp,data;
			if (Ifx_Shell_parseUInt32(&args, &tmp, FALSE) == FALSE){
				
			}
			IfxStdIf_DPipe_print(io, "Read Inforom [%d] byte"ENDL,tmp);
			i2c_read_bytes(INFOROM_I2C_NUM, INFOROM_DEV_ADDR, tmp, 1, &data, 1);
			IfxStdIf_DPipe_print(io, "Read Inforom [%d] byte : %d"ENDL,tmp,data);
		}
		else
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid parameter %s"ENDL, args);
			IfxStdIf_DPipe_print(io, "Use inforom ? to get help message"ENDL);
		}
	}
    return TRUE;
}

//#if (PROJECT == PROJECT_DRIVE_AX) && (DRIVE_AX_B0x == 1)
#if 1
boolean i2cromreadwritecemode(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 ce_mode;
    uint32 curr_ce_mode;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : cemode read/write [n]"ENDL);
        IfxStdIf_DPipe_print(io, "           > Read or write CE Mode in inforom"ENDL);
        IfxStdIf_DPipe_print(io, "           > in P3570, ce mode is used for dgpu mode"ENDL);
        IfxStdIf_DPipe_print(io, "           > cemode read"ENDL);
        IfxStdIf_DPipe_print(io, "           > cemode write n"ENDL);
        IfxStdIf_DPipe_print(io, "           > Possible Values:"ENDL);
        IfxStdIf_DPipe_print(io, "           >  0: Both Tegra controls DGPUs"ENDL);
        if(board.project == P3570)
        {
        	IfxStdIf_DPipe_print(io, "           >  1: Not supported in P3570"ENDL);
        	IfxStdIf_DPipe_print(io, "           >  2: Not supported in P3570"ENDL);
        }
        else
        {
        	IfxStdIf_DPipe_print(io, "           >  1: COM Express controls DGPU A, Tegra controls DGPU B"ENDL);
            IfxStdIf_DPipe_print(io, "           >  2: Tegra controls DGPU A, COM Express controls DGPU B"ENDL);
        }
        if(board.project == P3570)
        {
        	IfxStdIf_DPipe_print(io, "           >  3: X86 controls DGPU A and DGPU B"ENDL);
        }
        else
        {
        	IfxStdIf_DPipe_print(io, "           >  3: COM Express controls DGPU A and DGPU B"ENDL);
        }

        IfxStdIf_DPipe_print(io, "             "ENDL);
    }
    else
    {
        if(Ifx_Shell_matchToken(&args, "read") != FALSE)
        {
            curr_ce_mode = i2crom_read_ce_mode();
            IfxStdIf_DPipe_print(io, "Current CE Mode : %d"ENDL, curr_ce_mode);

        }
        else if(Ifx_Shell_matchToken(&args, "write") != FALSE)
        {
            if (Ifx_Shell_parseUInt32(&args, &ce_mode, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid CE Mode %s"ENDL, args);
                return TRUE;
            }
            if (ce_mode > 3)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid CE Mode, Possible values are 0-3"ENDL);
                return TRUE;
            }
            if(board.project == P3570)
            {
            	if ((ce_mode == 1) || (ce_mode == 2))
            	{
            		IfxStdIf_DPipe_print(io, "Syntaxerror : dgpu mode can not be 1 or 2 in 3570"ENDL);
            		return TRUE;
            	}
            }
            curr_ce_mode = i2crom_read_ce_mode();
            IfxStdIf_DPipe_print(io, "Original CE Mode was  : %d"ENDL, curr_ce_mode);
            i2crom_write_ce_mode(ce_mode);
            IfxStdIf_DPipe_print(io, "Configured CE Mode as : %d"ENDL, ce_mode);

        }
        else
        {
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid parameter %s"ENDL, args);
			IfxStdIf_DPipe_print(io, "Use cemode ? to get help message"ENDL);
        }

    }
    return TRUE;

}

/* for command cimrom */
IFX_EXTERN boolean i2cromreadcimrom(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if(board.project == P3570)
	{
		IfxStdIf_DPipe_print(io, "Syntaxerror: CIM ROM is removed in P3570 so this command is not supported\r\n");
		return TRUE;
	}
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax     : cimrom get sku"ENDL);
		IfxStdIf_DPipe_print(io, "           > read the sku from cim rom"ENDL);
		IfxStdIf_DPipe_print(io, "             "ENDL);
	}
	else
	{
		if(Ifx_Shell_matchToken(&args, "get") != FALSE)
		{
			if(Ifx_Shell_matchToken(&args, "sku") != FALSE)
			{
				uint8 buffer[100];
				memset(buffer, 0, sizeof(buffer));
				i2crom_read_cim_sku(buffer);
				IfxStdIf_DPipe_print(io, "Get SKU is %s\r\n", buffer);
			}
			else
			{
				IfxStdIf_DPipe_print(io, "Syntaxerror : invalid parameter, Use cimrom ? to get help message"ENDL);
				return TRUE;
			}
		}
		else
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid parameter, Use cimrom ? to get help message"ENDL);
			return TRUE;
		}
	}
}
#endif //#if (PROJECT == PROJECT_DRIVE_AX) && (DRIVE_AX_B0x == 1)


