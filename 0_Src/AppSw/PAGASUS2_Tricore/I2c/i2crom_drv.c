/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#include "I2c.h"
#include <internal_fw_debug.h>
#include <Configuration.h>
#include "i2crom_drv.h"
#include "resource.h"

/* I2C ROM Drivers */
/* ROM feature */
#define PAGE_SIZE        8    // 8 B page
#define ROM_TW_MS        4


#define I2C_ROM_DRV_DEBUG_ENABLE 1

#if I2C_ROM_DRV_DEBUG_ENABLE == 1
#define I2C_ROM_DRV_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define I2C_ROM_DRV_DEBUG_PRINTF(x)
#endif

#define I2C_ROM_DRV_PRINTF_ALWAYS(x) DEBUG_PRINTF_ALWAYS(x)

// Create String Table
const char Flash_StrTbl_Legacy                        [FLASH_STRTBL_LEGACY_SIZE                         ]= {FLASH_STRTBL_LEGACY                      };
const char Flash_StrTbl_Compliance                    [FLASH_STRTBL_COMPLIANCE_SIZE                     ]= {FLASH_STRTBL_COMPLIANCE                  };
const char Flash_StrTbl_Master                        [FLASH_STRTBL_MASTER_SIZE                         ]= {FLASH_STRTBL_MASTER                      };
const char Flash_StrTbl_Slave                         [FLASH_STRTBL_SLAVE_SIZE                          ]= {FLASH_STRTBL_SLAVE                       };
const char Flash_StrTbl_Wr_Val_Success                [FLASH_STRTBL_WR_VAL_SUCCESS_SIZE                 ]= {FLASH_STRTBL_WR_VAL_SUCCESS              };
const char Flash_StrTbl_Wr_Val_Fail1                  [FLASH_STRTBL_WR_VAL_FAIL1_SIZE                   ]= {FLASH_STRTBL_WR_VAL_FAIL1                };
const char Flash_StrTbl_Rd_Val_Fail1                  [FLASH_STRTBL_RD_VAL_FAIL1_SIZE                   ]= {FLASH_STRTBL_RD_VAL_FAIL1                };

const char Flash_StrTbl_Rd_Val_Fail_EthPHY_Mode_MS1   [FLASH_STRTBL_RD_VAL_FAIL_ETHPHY_MODE_MS1_SIZE    ]= {FLASH_STRTBL_RD_VAL_FAIL_ETHPHY_MODE_MS1 };

const char Flash_StrTbl_Syntax_Err_Val1               [FLASH_STRTBL_SYNTAX_ERR_VAL1_SIZE                ]= {FLASH_STRTBL_SYNTAX_ERR_VAL1             };
const char Flash_StrTbl_Syntax_Err_Val2               [FLASH_STRTBL_SYNTAX_ERR_VAL2_SIZE                ]= {FLASH_STRTBL_SYNTAX_ERR_VAL2             };

const char Flash_StrTbl_Syntax_Err_Socket1            [FLASH_STRTBL_SYNTAX_ERR_SOCKET1_SIZE             ]= {FLASH_STRTBL_SYNTAX_ERR_SOCKET1          };
const char Flash_StrTbl_Syntax_Err_Socket2            [FLASH_STRTBL_SYNTAX_ERR_SOCKET2_SIZE             ]= {FLASH_STRTBL_SYNTAX_ERR_SOCKET2          };
const char Flash_StrTbl_Syntax_Err_Socket_pin1        [FLASH_STRTBL_SYNTAX_ERR_SOCKET_PIN1_SIZE         ]= {FLASH_STRTBL_SYNTAX_ERR_SOCKET_PIN1      };
const char Flash_StrTbl_Syntax_Err_Socket_pin2        [FLASH_STRTBL_SYNTAX_ERR_SOCKET_PIN2_SIZE         ]= {FLASH_STRTBL_SYNTAX_ERR_SOCKET_PIN2      };

// Group String 
const char *Flash_Str_PHY_Mode[FLASH_STR_PHY_MODE_TOL              ]= {Flash_StrTbl_Compliance, Flash_StrTbl_Legacy};
const char *Flash_Str_PHY_MasterSlave[FLASH_STR_PHY_MASTERSLAVE_TOL]= {Flash_StrTbl_Master, Flash_StrTbl_Slave};


static void i2crom_drv_write_package_mode(t_rom_object * rom_object_p, uint32 addr, const uint8* wbuf, uint32 bytes);
static void i2crom_drv_read_package_mode(t_rom_object * rom_object_p, uint32 addr, uint8* rbuf, uint32 bytes);
static void print_buffer(uint8* buf, uint32 size);

void i2crom_drv_write(t_rom_object * rom_object_p, uint32 addr, const uint8* wbuf, uint32 bytes)
{
    /* write the data to ROM */
    uint32 bytesleft;
    if((addr+bytes) > ROM_SIZE)
    {
        I2C_ROM_DRV_DEBUG_PRINTF(("ROM DRV Error : Bytes to written exceed the ROM size!\r\n"));
        return;
    }
    if(rom_object_p == NULL)
    {
        I2C_ROM_DRV_DEBUG_PRINTF(("ROM DRV Error : ROM object is not valid!\r\n"));
        return;
    }
    I2C_ROM_DRV_DEBUG_PRINTF(("i2cbus: %d, i2caddress: 0x%x\r\n", rom_object_p->i2c_bus, rom_object_p->i2c_address));
    /* need to align to page size */
    uint32 i = 0;
    while(bytes > 0)
    {
        I2C_ROM_DRV_DEBUG_PRINTF(("address is 0x%x\n\r", addr));
        if(addr % PAGE_SIZE == 0)
        {
            I2C_ROM_DRV_DEBUG_PRINTF(("align the address 0x%x\n\r", addr));
            break;
        }
        I2C_ROM_DRV_DEBUG_PRINTF(("write 1 bytes %d to ROM addr 0x%x\n\r", wbuf[i], addr));
        bytesleft = i2c_write_bytes(rom_object_p->i2c_bus, rom_object_p->i2c_address, addr, 1, &wbuf[i]);
        if (bytesleft)
        {
            I2C_ROM_DRV_PRINTF_ALWAYS(("Error : %d bytes not written\r\n", bytesleft));
        }
        waitTime((ROM_TW_MS + 1)*TimeConst[TIMER_INDEX_1MS]);
        i++;
        bytes--;
        addr++;
    }
    if(bytes > 0)
    {
        I2C_ROM_DRV_DEBUG_PRINTF(("write %d bytes to ROM addr 0x%x using page mode\n\r", bytes, addr));
        i2crom_drv_write_package_mode(rom_object_p, addr, &wbuf[i], bytes);
    }
}

void i2crom_drv_read(t_rom_object * rom_object_p, uint32 addr, uint8* rbuf, uint32 bytes)
{
    uint32 bytesleft;
    if((addr+bytes) > ROM_SIZE)
    {
        I2C_ROM_DRV_PRINTF_ALWAYS(("ROM DRV Error : Bytes to read exceed the ROM size!\r\n"));
        return;
    }
    if(rom_object_p == NULL)
    {
        I2C_ROM_DRV_DEBUG_PRINTF(("ROM DRV Error : ROM object is not valid!\r\n"));
        return;
    }
    I2C_ROM_DRV_DEBUG_PRINTF(("i2cbus: %d, i2caddress: 0x%x\r\n", rom_object_p->i2c_bus, rom_object_p->i2c_address));
    /* need to align to page size */
    uint32 i = 0;
    while(bytes > 0)
    {
        I2C_ROM_DRV_DEBUG_PRINTF(("address is 0x%x\n\r", addr));
        if(addr % PAGE_SIZE == 0)
        {
            I2C_ROM_DRV_DEBUG_PRINTF(("align the address 0x%x\n\r", addr));
            break;
        }
        I2C_ROM_DRV_DEBUG_PRINTF(("read 1 bytes %d to ROM addr 0x%x\r\n", rbuf[i], addr));
        bytesleft = i2c_read_bytes(rom_object_p->i2c_bus, rom_object_p->i2c_address, addr, 1, &rbuf[i], 1);
        if (bytesleft)
        {
            I2C_ROM_DRV_PRINTF_ALWAYS(("Error : %d bytes not read out\r\n", bytesleft));
        }
        i++;
        bytes--;
        addr++;
    }
    if(bytes > 0)
    {
        I2C_ROM_DRV_DEBUG_PRINTF(("read %d bytes from ROM addr 0x%x using page mode\n\r", bytes, addr));
        i2crom_drv_read_package_mode(rom_object_p, addr, &rbuf[i], bytes);
    }
}


static void print_buffer(uint8* buf, uint32 size)
{
    /* disable the buffer print */
    return;
    I2C_ROM_DRV_DEBUG_PRINTF(("buffer data is\n\r"));
    uint32 i;
    for(i=0; i<size; i++)
    {
        I2C_ROM_DRV_DEBUG_PRINTF(("%d \n", buf[i]));
    }
}


static void i2crom_drv_write_package_mode(t_rom_object * rom_object_p, uint32 addr, const uint8* wbuf, uint32 bytes)
{
    /* write the data to ROM */
    uint32 bytes_per_pacakge, bytesleft;
    uint32 index = 0;
    while(bytes>0)
    {
        if(bytes > PAGE_SIZE)
        {
            bytes_per_pacakge = PAGE_SIZE;
        }
        else
        {
            bytes_per_pacakge = bytes;
        }
        bytesleft = i2c_write_bytes(rom_object_p->i2c_bus, rom_object_p->i2c_address, addr, bytes_per_pacakge, &wbuf[index]);
        if (bytesleft)
        {
            I2C_ROM_DRV_PRINTF_ALWAYS(("Error : %d bytes not written\n", bytesleft));
        }
        I2C_ROM_DRV_DEBUG_PRINTF(("write %d bytes to ROM\n", bytes_per_pacakge));
        bytes -= bytes_per_pacakge;
        addr += bytes_per_pacakge;
        index += bytes_per_pacakge;
        waitTime((ROM_TW_MS + 1)*TimeConst[TIMER_INDEX_1MS]);
    }
}

static void i2crom_drv_read_package_mode(t_rom_object * rom_object_p, uint32 addr, uint8* rbuf, uint32 bytes)
{
    uint32 bytes_per_pacakge, bytesleft;
    uint32 bytes_to_print = bytes;  // only for debug
    uint32 index = 0;
    I2C_ROM_DRV_DEBUG_PRINTF(("rom_object_p->i2c_address = 0x%x addr = 0x%x \n\r", rom_object_p->i2c_address, addr));
    while(bytes > 0)
    {
        if(bytes > PAGE_SIZE)
        {
            bytes_per_pacakge = PAGE_SIZE;
        }
        else
        {
            bytes_per_pacakge = bytes;
        }
        bytesleft = i2c_read_bytes(rom_object_p->i2c_bus, rom_object_p->i2c_address, addr, bytes_per_pacakge, &rbuf[index], 1);
        if (bytesleft)
        {
            I2C_ROM_DRV_PRINTF_ALWAYS(("Error : %d bytes not read\n\r", bytesleft));
        }
        bytes -= bytes_per_pacakge;
        addr += bytes_per_pacakge;
        index += bytes_per_pacakge;
    }
    print_buffer(rbuf, bytes_to_print);
}


// ===============================================================================================================================
// Function Name: Setup for Ignition Check Feature Enablement menu item
// Purpose: Config I2C0 InfoROM data for Ignition Check Feature when startup
// Note: 
//       
//       
// ===============================================================================================================================
uint8 IgnitionChkEngage_ReadValue(uint8 *ubOutdata)
{
    uint8 bytesleft;
    uint8 ubTmp;

    bytesleft= i2c_read_bytes(INFOROM_I2C_NUM, INFOROM_DEV_ADDR, INFOROM_ADDR_TBL_IGNCHKFG, 1, &ubTmp, 1);

    if(ubTmp == 0xFF)    // 0xFF means empty
        *ubOutdata= 0;   // Prevent bit0= 1 when data is empty
    else
        *ubOutdata= ubTmp;

    return bytesleft;
}

uint8 IgnitionChkEngage_WriteValue(uint8 *ubIndata)
{
    uint8 bytesleft;

    bytesleft = i2c_write_bytes(INFOROM_I2C_NUM, INFOROM_DEV_ADDR, INFOROM_ADDR_TBL_IGNCHKFG, 1, ubIndata);

    return bytesleft;
}


boolean IgnitionChkEngage_Sta(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint8 bytesleft;
    T_ROM_IGNCHK_DATA ignchk;

    //IfxStdIf_DPipe_print(io, "Ignition Check Status Command is Received."ENDL);

    bytesleft= IgnitionChkEngage_ReadValue(&ignchk.uB[0]);   // read 1 byte data from I2C0
    if(!bytesleft)
    {
        //successful
        IfxStdIf_DPipe_print(io, "The Ignition Check Feature Enablement Setting is = %d."ENDL, ignchk.detail.IgnChk);
    }
    else
    {
        //failed
        IfxStdIf_DPipe_print(io, Flash_StrTbl_Rd_Val_Fail1);
    }

    return TRUE;
}

boolean IgnitionChkEngage_Set(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 udwTmp;
    uint8 bytesleft;
    T_ROM_IGNCHK_DATA ignchk;

    //IfxStdIf_DPipe_print(io, "Ignition Check Setup Command is Received."ENDL);

    if(Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax: ignchkset x"ENDL);
        IfxStdIf_DPipe_print(io, "      > Setup Ignition Check Feature Enablement Setting"ENDL);
        IfxStdIf_DPipe_print(io, "      > x = 0~1 for Ignition Check Feature is 0= Disablement / 1= Enablement"ENDL);
    }
    else
    {
        if(Ifx_Shell_parseUInt32(&args, &udwTmp, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, Flash_StrTbl_Syntax_Err_Val1);
            return TRUE;
        }

        #if 1
        else
        {
            if(udwTmp > 1)   // check value range
            {
                IfxStdIf_DPipe_print(io, Flash_StrTbl_Syntax_Err_Val2);
                return TRUE;
            }
        }

        // Read 1 byte data from I2C0 to keep other setting
        bytesleft= IgnitionChkEngage_ReadValue(&ignchk.uB[0]);   
        if(!bytesleft)
        {
            //successful
            if(udwTmp)
                ignchk.detail.IgnChk= 1;
            else
                ignchk.detail.IgnChk= 0;
        }
        else
        {
            //failed
            ignchk.uW= udwTmp;
        }

        #else

        ignchk.uW= udwTmp;    // for testing input all data range
        #endif

        bytesleft= IgnitionChkEngage_WriteValue(&ignchk.uB[0]);   // write 1 byte data to I2C0
        if(!bytesleft)
        {
            //successful
            IfxStdIf_DPipe_print(io, "Write value= %d is successful."ENDL, ignchk.detail.IgnChk);
        }
        else
        {
            //failed
            IfxStdIf_DPipe_print(io, "Write value= %d is failed."ENDL, ignchk.detail.IgnChk);
        }
    }

    return TRUE;
}


// ===============================================================================================================================
// Function Name: Setup for Ethernet PHY work mode & Master/Slave menu item 
// Purpose: Config I2C0 InfoROM data for PHY initialization
// Note: 
//       
//       
// ===============================================================================================================================
uint8 EthPHYModeMS_ReadValue(uint8 *ubOutdata)
{
    uint8 bytesleft;

    bytesleft= i2c_read_bytes(INFOROM_I2C_NUM, INFOROM_DEV_ADDR, INFOROM_ADDR_TBL_ETH_PHY_MODE_B0, 3, ubOutdata, 1);

    return bytesleft;
}

uint8 EthPHYModeMS_WriteValue(uint8 *ubIndata)
{
    uint8 bytesleft;

    bytesleft = i2c_write_bytes(INFOROM_I2C_NUM, INFOROM_DEV_ADDR, INFOROM_ADDR_TBL_ETH_PHY_MODE_B0, 3, ubIndata);

    return bytesleft;
}


boolean EthPHYModeMS_Sta(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint8 bytesleft;
    uint8 phymode;
    uint8 phyms;
    T_ROM_ETHPHY_MODE_MS_DATA ethphy_mode_ms;

    //IfxStdIf_DPipe_print(io, "Ethernet PHY Work Mode & Master/Slave Command is Received."ENDL);

    if(Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        //IfxStdIf_DPipe_print(io, "To read out J10.1 ~J12.4 Ethernet PHY work mode & Master/Slave setting."ENDL);
        IfxStdIf_DPipe_print(io, "To read out J10.1 ~ J12.4 Ethernet PHY Master/Slave setting."ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: ethphysta"ENDL);
    }
    else
    {
        bytesleft= EthPHYModeMS_ReadValue(&ethphy_mode_ms.uB[0]);   // read 3 bytes data from I2C0 InfoROM
        if(!bytesleft)
        {
            //successful
            //IfxStdIf_DPipe_print(io, "The Ethernet PHY work mode & Master/Slave Setting is 0x%08X."ENDL, ethphy_mode_ms.uDW);
        }
        else
        {
            //failed
            IfxStdIf_DPipe_print(io, Flash_StrTbl_Rd_Val_Fail1);
        }

        // Translate each status
        //phymode= ethphy_mode_ms.detail.J10_1_Mode;
        phyms= ethphy_mode_ms.detail.J10_1_M_S;
        //IfxStdIf_DPipe_print(io, "The J10.1 work mode= %s @ %s."ENDL, Flash_Str_PHY_Mode[phymode], Flash_Str_PHY_MasterSlave[phyms]);
        IfxStdIf_DPipe_print(io, "The J10.1 work role= %s."ENDL, Flash_Str_PHY_MasterSlave[phyms]);

        //phymode= ethphy_mode_ms.detail.J10_2_Mode;
        phyms= ethphy_mode_ms.detail.J10_2_M_S;
        //IfxStdIf_DPipe_print(io, "The J10.2 work mode= %s @ %s."ENDL, Flash_Str_PHY_Mode[phymode], Flash_Str_PHY_MasterSlave[phyms]);
        IfxStdIf_DPipe_print(io, "The J10.2 work role= %s."ENDL, Flash_Str_PHY_MasterSlave[phyms]);
         
        //phymode= ethphy_mode_ms.detail.J10_3_Mode;
        phyms= ethphy_mode_ms.detail.J10_3_M_S;
        //IfxStdIf_DPipe_print(io, "The J10.3 work mode= %s @ %s."ENDL, Flash_Str_PHY_Mode[phymode], Flash_Str_PHY_MasterSlave[phyms]);
        IfxStdIf_DPipe_print(io, "The J10.3 work role= %s."ENDL, Flash_Str_PHY_MasterSlave[phyms]);

        //phymode= ethphy_mode_ms.detail.J10_4_Mode;
        phyms= ethphy_mode_ms.detail.J10_4_M_S;
        //IfxStdIf_DPipe_print(io, "The J10.4 work mode= %s @ %s."ENDL, Flash_Str_PHY_Mode[phymode], Flash_Str_PHY_MasterSlave[phyms]);
        IfxStdIf_DPipe_print(io, "The J10.4 work role= %s."ENDL, Flash_Str_PHY_MasterSlave[phyms]);

        // Note: J11.1 is not supported due to PCIE
        //phymode= ethphy_mode_ms.detail.J11_1_Mode;
        //phyms= ethphy_mode_ms.detail.J11_1_M_S;
        //IfxStdIf_DPipe_print(io, "The J11.1 work mode= %s @ %s."ENDL, Flash_Str_PHY_Mode[phymode], Flash_Str_PHY_MasterSlave[phyms]);

        phymode= ethphy_mode_ms.detail.J11_2_Mode;
        phyms= ethphy_mode_ms.detail.J11_2_M_S;
        //IfxStdIf_DPipe_print(io, "The J11.2 work mode= %s @ %s."ENDL, Flash_Str_PHY_Mode[phymode], Flash_Str_PHY_MasterSlave[phyms]);
        IfxStdIf_DPipe_print(io, "The J11.2 work role= %s, mode= %s."ENDL, Flash_Str_PHY_MasterSlave[phyms],Flash_Str_PHY_Mode[phymode]);
         
        phymode= ethphy_mode_ms.detail.J11_3_Mode;
        phyms= ethphy_mode_ms.detail.J11_3_M_S;
        //IfxStdIf_DPipe_print(io, "The J11.3 work mode= %s @ %s."ENDL, Flash_Str_PHY_Mode[phymode], Flash_Str_PHY_MasterSlave[phyms]);
        IfxStdIf_DPipe_print(io, "The J11.3 work role= %s, mode= %s."ENDL, Flash_Str_PHY_MasterSlave[phyms],Flash_Str_PHY_Mode[phymode]);

        phymode= ethphy_mode_ms.detail.J11_4_Mode;
        phyms= ethphy_mode_ms.detail.J11_4_M_S;
        //IfxStdIf_DPipe_print(io, "The J11.4 work mode= %s @ %s."ENDL, Flash_Str_PHY_Mode[phymode], Flash_Str_PHY_MasterSlave[phyms]);
        IfxStdIf_DPipe_print(io, "The J11.4 work role= %s, mode= %s."ENDL, Flash_Str_PHY_MasterSlave[phyms],Flash_Str_PHY_Mode[phymode]);

        phymode= ethphy_mode_ms.detail.J12_1_Mode;
        phyms= ethphy_mode_ms.detail.J12_1_M_S;
        //IfxStdIf_DPipe_print(io, "The J12.1 work mode= %s @ %s."ENDL, Flash_Str_PHY_Mode[phymode], Flash_Str_PHY_MasterSlave[phyms]);
        IfxStdIf_DPipe_print(io, "The J12.1 work role= %s, mode= %s."ENDL, Flash_Str_PHY_MasterSlave[phyms],Flash_Str_PHY_Mode[phymode]);

        phymode= ethphy_mode_ms.detail.J12_2_Mode;
        phyms= ethphy_mode_ms.detail.J12_2_M_S;
        //IfxStdIf_DPipe_print(io, "The J12.2 work mode= %s @ %s."ENDL, Flash_Str_PHY_Mode[phymode], Flash_Str_PHY_MasterSlave[phyms]);
        IfxStdIf_DPipe_print(io, "The J12.2 work role= %s, mode= %s."ENDL, Flash_Str_PHY_MasterSlave[phyms],Flash_Str_PHY_Mode[phymode]);
         
        phymode= ethphy_mode_ms.detail.J12_3_Mode;
        phyms= ethphy_mode_ms.detail.J12_3_M_S;
        //IfxStdIf_DPipe_print(io, "The J12.3 work mode= %s @ %s."ENDL, Flash_Str_PHY_Mode[phymode], Flash_Str_PHY_MasterSlave[phyms]);
        IfxStdIf_DPipe_print(io, "The J12.3 work role= %s, mode= %s."ENDL, Flash_Str_PHY_MasterSlave[phyms],Flash_Str_PHY_Mode[phymode]);

        phymode= ethphy_mode_ms.detail.J12_4_Mode;
        phyms= ethphy_mode_ms.detail.J12_4_M_S;
        //IfxStdIf_DPipe_print(io, "The J12.4 work mode= %s @ %s."ENDL, Flash_Str_PHY_Mode[phymode], Flash_Str_PHY_MasterSlave[phyms]);
        IfxStdIf_DPipe_print(io, "The J12.4 work role= %s, mode= %s."ENDL, Flash_Str_PHY_MasterSlave[phyms],Flash_Str_PHY_Mode[phymode]);
    }

    return TRUE;
}


boolean EthPHYMode_Set(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint8 bytesleft;
    uint32 udwTmp;
    uint8 socketnum;
    uint8 socketpin;
    uint8 phymode;
    uint8 idx_port;
    uint8 idx_portbit;
    T_ROM_ETHPHY_MODE_MS_DATA ethphy_mode_ms;

    //IfxStdIf_DPipe_print(io, "Ethernet PHY Work Mode Setup command is received."ENDL);

    if(Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "To setup J11.2 ~ J12.4 Ethernet PHY Compliance/Legacy work mode setting."ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : ethphymodeset x y z"ENDL);
        IfxStdIf_DPipe_print(io, "           > x = 11~12 for socket selection"ENDL);
        IfxStdIf_DPipe_print(io, "           > y = 1~4 for socket pin selection"ENDL);
        IfxStdIf_DPipe_print(io, "           > z = 0~1 for 0= Compliance / 1= Legacy mode selection"ENDL);
    }
    else
    {
        // x= socket number selection
        if(Ifx_Shell_parseUInt32(&args, &udwTmp, FALSE) == FALSE)   
        {
            IfxStdIf_DPipe_print(io, Flash_StrTbl_Syntax_Err_Socket1);
            return TRUE;
        }
        else
        {
            if((udwTmp < 11) || (udwTmp > 12))   // check value range
            {
                IfxStdIf_DPipe_print(io, Flash_StrTbl_Syntax_Err_Socket2);
                return TRUE;
            }
        }
        socketnum= udwTmp;

        // y= socket pin selection
        if(Ifx_Shell_parseUInt32(&args, &udwTmp, FALSE) == FALSE)   
        {
            IfxStdIf_DPipe_print(io, Flash_StrTbl_Syntax_Err_Socket_pin1);
            return TRUE;
        }
        else
        {
            if((udwTmp < 1) || (udwTmp > 4))   // check value range
            {
                IfxStdIf_DPipe_print(io, Flash_StrTbl_Syntax_Err_Socket_pin2);
                return TRUE;
            }
        }
        socketpin= udwTmp;

        // Check J11.1
        if((socketnum == 11) && (socketpin == 1))
        {
            IfxStdIf_DPipe_print(io, "Note: J11.1 is not supported due to PCIE."ENDL);
            return TRUE;
        }

        // z= Legacy / Compliance mode selection
        if(Ifx_Shell_parseUInt32(&args, &udwTmp, FALSE) == FALSE)   
        {
            IfxStdIf_DPipe_print(io, "Syntax Error: invalid mode."ENDL);
            return TRUE;
        }
        else
        {
            if(udwTmp > 1)   // check value range
            {
                IfxStdIf_DPipe_print(io, "Syntax Error: invalid mode range."ENDL);
                return TRUE;
            }
        }
        phymode= udwTmp;

        // Read 3 bytes data from I2C0 to keep other setting
        bytesleft= EthPHYModeMS_ReadValue(&ethphy_mode_ms.uB[0]);
        if(!bytesleft)
        {
            //successful
            //IfxStdIf_DPipe_print(io, "The existing ethernet PHY work mode & Master/Slave setting is 0x%04X."ENDL, ethphy_mode_ms.uDW);
        }
        else
        {
            //failed
            IfxStdIf_DPipe_print(io, Flash_StrTbl_Rd_Val_Fail_EthPHY_Mode_MS1);
            ethphy_mode_ms.uDW= 0xFFFFFFFF;
        }

        // calculate corresponding port bit position index
        // if(socketnum == 10)
        //     idx_port= 0;     // bit 0
        if(socketnum == 11)
            idx_port= 4;     // bit 4
        if(socketnum == 12)
            idx_port= 8;     // bit 8
        idx_portbit= idx_port + (socketpin - 1);   // J11.2 ~ J12.4 bit position

        // Extract corresponding mode & m/s data
        //phymode= (ethphy_mode_ms.uDW >> (0 + idx_portbit)) & 0x01;
        //phyms= (ethphy_mode_ms.uDW >> (12 + idx_portbit)) & 0x01;
        if(phymode)
            ethphy_mode_ms.uDW |= (1 << (0 + idx_portbit));
        else
            ethphy_mode_ms.uDW &= ~(1 << (0 + idx_portbit));
        //IfxStdIf_DPipe_print(io, "The modified Ethernet PHY work mode & Master/Slave setting is 0x%04X."ENDL, ethphy_mode_ms.uDW);

        bytesleft= EthPHYModeMS_WriteValue(&ethphy_mode_ms.uB[0]);   // write 3 bytes data to I2C0
        if(!bytesleft)
        {
            //successful
            IfxStdIf_DPipe_print(io, "The new J%d.%d work mode= %s."ENDL, socketnum, socketpin, Flash_Str_PHY_Mode[phymode]);
            IfxStdIf_DPipe_print(io, Flash_StrTbl_Wr_Val_Success);
            IfxStdIf_DPipe_print(io, "Settings will take effective when next boot."ENDL);

        }
        else
        {
            //failed
            IfxStdIf_DPipe_print(io, Flash_StrTbl_Wr_Val_Fail1);
        }
    }

    return TRUE;
}


boolean EthPHYMS_Set(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint8 bytesleft;
    uint32 udwTmp;
    uint8 socketnum;
    uint8 socketpin;
    uint8 phyms;
    uint8 idx_port;
    uint8 idx_portbit;
    T_ROM_ETHPHY_MODE_MS_DATA ethphy_mode_ms;

    //IfxStdIf_DPipe_print(io, "Ethernet PHY Work Mode Setup command is received."ENDL);

    if(Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "To setup J10.1 ~ J12.4 Ethernet PHY Master/Slave setting."ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: ethphymsset x y z"ENDL);
        IfxStdIf_DPipe_print(io, "      > x = 10~12 for socket selection"ENDL);
        IfxStdIf_DPipe_print(io, "      > y = 1~4 for socket pin selection"ENDL);
        IfxStdIf_DPipe_print(io, "      > z = 0~1 for 0= Master / 1= Slave selection"ENDL);
    }
    else
    {
        // x= socket number selection
        if(Ifx_Shell_parseUInt32(&args, &udwTmp, FALSE) == FALSE)   
        {
            IfxStdIf_DPipe_print(io, Flash_StrTbl_Syntax_Err_Socket1);
            return TRUE;
        }
        else
        {
            if((udwTmp < 10) || (udwTmp > 12))   // check value range
            {
                IfxStdIf_DPipe_print(io, Flash_StrTbl_Syntax_Err_Socket2);
                return TRUE;
            }
        }
        socketnum= udwTmp;

        // y= socket pin selection
        if(Ifx_Shell_parseUInt32(&args, &udwTmp, FALSE) == FALSE)   
        {
            IfxStdIf_DPipe_print(io, Flash_StrTbl_Syntax_Err_Socket_pin1);
            return TRUE;
        }
        else
        {
            if((udwTmp < 1) || (udwTmp > 4))   // check value range
            {
                IfxStdIf_DPipe_print(io, Flash_StrTbl_Syntax_Err_Socket_pin2);
                return TRUE;
            }
        }
        socketpin= udwTmp;

        // Check J11.1
        if((socketnum == 11) && (socketpin == 1))
        {
            IfxStdIf_DPipe_print(io, "Note: J11.1 is not supported due to PCIE."ENDL);
            return TRUE;
        }

        // z= Master/Slave selection
        if(Ifx_Shell_parseUInt32(&args, &udwTmp, FALSE) == FALSE)   
        {
            IfxStdIf_DPipe_print(io, "Syntax Error: invalid Master/Slave selection value."ENDL);
            return TRUE;
        }
        else
        {
            if(udwTmp > 1)   // check value range
            {
                IfxStdIf_DPipe_print(io, "Syntax Error: invalid Master/Slave selection value range."ENDL);
                return TRUE;
            }
        }
        phyms= udwTmp;

        // Read 3 bytes data from I2C0 to keep other setting
        bytesleft= EthPHYModeMS_ReadValue(&ethphy_mode_ms.uB[0]);
        if(!bytesleft)
        {
            //successful
            //IfxStdIf_DPipe_print(io, "The existing ethernet PHY work mode & Master/Slave setting is 0x%08X."ENDL, ethphy_mode_ms.uDW);
        }
        else
        {
            //failed
            IfxStdIf_DPipe_print(io, Flash_StrTbl_Rd_Val_Fail_EthPHY_Mode_MS1);
            ethphy_mode_ms.uDW= 0xFFFFFFFF;
        }

        // calculate corresponding port bit position index
        if(socketnum == 10)
            idx_port= 0;     // bit 0
        if(socketnum == 11)
            idx_port= 4;     // bit 4
        if(socketnum == 12)
            idx_port= 8;     // bit 8
        idx_portbit= idx_port + (socketpin - 1);   // J10.1 ~ J12.4 bit position

        // Extract corresponding mode & m/s data
        //phymode= (ethphy_mode_ms.uDW >> (0 + idx_portbit)) & 0x01;
        //phyms= (ethphy_mode_ms.uDW >> (12 + idx_portbit)) & 0x01;
        if(phyms)
            ethphy_mode_ms.uDW |= (1 << (12 + idx_portbit));
        else
            ethphy_mode_ms.uDW &= ~(1 << (12 + idx_portbit));
        //IfxStdIf_DPipe_print(io, "The modified Ethernet PHY work mode & Master/Slave setting is 0x%08X."ENDL, ethphy_mode_ms.uDW);

        bytesleft= EthPHYModeMS_WriteValue(&ethphy_mode_ms.uB[0]);   // write 3 bytes data to I2C0
        if(!bytesleft)
        {
            //successful
            IfxStdIf_DPipe_print(io, "The new J%d.%d M/S mode is %s."ENDL, socketnum, socketpin, Flash_Str_PHY_MasterSlave[phyms]);
            IfxStdIf_DPipe_print(io, Flash_StrTbl_Wr_Val_Success);
            IfxStdIf_DPipe_print(io, "Settings will take effective when next boot."ENDL);

        }
        else
        {
            //failed
            IfxStdIf_DPipe_print(io, Flash_StrTbl_Wr_Val_Fail1);
        }
    }

    return TRUE;
}





