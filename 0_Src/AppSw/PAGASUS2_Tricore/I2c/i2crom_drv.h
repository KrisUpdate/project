/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef _I2C_I2CROM_DRV_H_
#define _I2C_I2CROM_DRV_H_


/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define ROM_SIZE                                           256    // 256 B total eeprom size

#define INFOROM_I2C_NUM                                    0x00   // EEPROM stay @ Aurix I2C port number
#define INFOROM_DEV_ADDR                                   0x54   // Device Address on I2C bus

// Info ROM Address Table List
#define INFOROM_ADDR_TBL_PCB_REV_HB                         68    // PCB Revision (cnn)
#define INFOROM_ADDR_TBL_PCB_REV_LB                         69    //              (c: A-Z, n: 0-9)
#define INFOROM_ADDR_TBL_IGNCHKFG                          218    // Ignition Check Feature Enablement Setting, 0= Disable, 1= Enable

#define INFOROM_ADDR_TBL_ETH_PHY_MODE_B0                   219    // Ethernet PHY work mode & Master/Slave Byte 0
#define INFOROM_ADDR_TBL_ETH_PHY_MODE_B1                   220    // Ethernet PHY work mode & Master/Slave Byte 1
#define INFOROM_ADDR_TBL_ETH_PHY_MODE_B2                   221    // Ethernet PHY work mode & Master/Slave Byte 2

#define INFOROM_ADDR_TBL_ETH_PHY_MASTER                    0
#define INFOROM_ADDR_TBL_ETH_PHY_SLAVE                     1
#define INFOROM_ADDR_TBL_ETH_PHY_MODE_COMPLIANCE           0
#define INFOROM_ADDR_TBL_ETH_PHY_MODE_LEGACY               1

// Create String Table
#define FLASH_STRTBL_LEGACY                                "Legacy"
#define FLASH_STRTBL_LEGACY_SIZE                           sizeof(FLASH_STRTBL_LEGACY)
#define FLASH_STRTBL_COMPLIANCE                            "Compliance"
#define FLASH_STRTBL_COMPLIANCE_SIZE                       sizeof(FLASH_STRTBL_COMPLIANCE)
#define FLASH_STRTBL_MASTER                                "Master"
#define FLASH_STRTBL_MASTER_SIZE                           sizeof(FLASH_STRTBL_MASTER)
#define FLASH_STRTBL_SLAVE                                 "Slave"
#define FLASH_STRTBL_SLAVE_SIZE                            sizeof(FLASH_STRTBL_SLAVE)

#define FLASH_STRTBL_WR_VAL_SUCCESS                        "Write value is successful."ENDL
#define FLASH_STRTBL_WR_VAL_SUCCESS_SIZE                   sizeof(FLASH_STRTBL_WR_VAL_SUCCESS)
#define FLASH_STRTBL_WR_VAL_FAIL1                          "Write value is failed."ENDL
#define FLASH_STRTBL_WR_VAL_FAIL1_SIZE                     sizeof(FLASH_STRTBL_WR_VAL_FAIL1)
#define FLASH_STRTBL_RD_VAL_FAIL1                          "Read value is failed."ENDL
#define FLASH_STRTBL_RD_VAL_FAIL1_SIZE                     sizeof(FLASH_STRTBL_RD_VAL_FAIL1)

#define FLASH_STRTBL_RD_VAL_FAIL_ETHPHY_MODE_MS1           "Can't read out the existing ethernet PHY work mode & Master/Slave setting."ENDL \
                                                           "Set the others to Compliance & Slave mode."ENDL
#define FLASH_STRTBL_RD_VAL_FAIL_ETHPHY_MODE_MS1_SIZE      sizeof(FLASH_STRTBL_RD_VAL_FAIL_ETHPHY_MODE_MS1)

#define FLASH_STRTBL_SYNTAX_ERR_VAL1                       "Syntax Error: invalid value."ENDL
#define FLASH_STRTBL_SYNTAX_ERR_VAL1_SIZE                  sizeof(FLASH_STRTBL_SYNTAX_ERR_VAL1)
#define FLASH_STRTBL_SYNTAX_ERR_VAL2                       "Syntax Error: invalid value range."ENDL
#define FLASH_STRTBL_SYNTAX_ERR_VAL2_SIZE                  sizeof(FLASH_STRTBL_SYNTAX_ERR_VAL2)

#define FLASH_STRTBL_SYNTAX_ERR_SOCKET1                    "Syntax Error: invalid socket number."ENDL
#define FLASH_STRTBL_SYNTAX_ERR_SOCKET1_SIZE               sizeof(FLASH_STRTBL_SYNTAX_ERR_SOCKET1)
#define FLASH_STRTBL_SYNTAX_ERR_SOCKET2                    "Syntax Error: invalid socket number range."ENDL
#define FLASH_STRTBL_SYNTAX_ERR_SOCKET2_SIZE               sizeof(FLASH_STRTBL_SYNTAX_ERR_SOCKET2)

#define FLASH_STRTBL_SYNTAX_ERR_SOCKET_PIN1                "Syntax Error: invalid socket pin."ENDL
#define FLASH_STRTBL_SYNTAX_ERR_SOCKET_PIN1_SIZE           sizeof(FLASH_STRTBL_SYNTAX_ERR_SOCKET_PIN1)
#define FLASH_STRTBL_SYNTAX_ERR_SOCKET_PIN2                "Syntax Error: invalid socket pin range."ENDL
#define FLASH_STRTBL_SYNTAX_ERR_SOCKET_PIN2_SIZE           sizeof(FLASH_STRTBL_SYNTAX_ERR_SOCKET_PIN2)




// Group String 
#define FLASH_STR_PHY_MODE_TOL                             2
#define FLASH_STR_PHY_MASTERSLAVE_TOL                      2

/******************************************************************************/
/*------------------------------Type Definitions------------------------------*/
/******************************************************************************/
typedef struct
{
    uint8 i2c_bus;
    uint8 i2c_address;
    uint16 valid_info_size;  /* the size that we can used in the ROM to store HW info*/
    uint16 setting_start_addr;  /* the start address to store software private setting data */
}t_rom_object;


// -------------------------------------------------------------------------------------------------------------------------------
// Setup for Ignition Check Feature Enablement menu item
// -------------------------------------------------------------------------------------------------------------------------------
typedef struct _T_ROM_IgnChk_Detail
{
    uint8 IgnChk   : 1;      // low bit first, Ignition Check Feature Flag, 0= Disable, 1= Enable
    uint8 Rev0     : 7;

    uint8 Rev1;              // Even byte alignment
}T_ROM_IGNCHK_DETAIL;

typedef union _T_ROM_IgnChk_Data
{
    uint8  uB[2];
    uint16 uW;
    T_ROM_IGNCHK_DETAIL detail;
}T_ROM_IGNCHK_DATA;


// -------------------------------------------------------------------------------------------------------------------------------
// Setup for Ethernet PHY work mode & Master/Slave menu item 
// -------------------------------------------------------------------------------------------------------------------------------
typedef struct _T_ROM_EthPhy_Mode_MS_Detail
{
    uint32 J10_1_Mode   : 1;      // low bit first, J10.1 PHY Mode Selection, 0= Compliance, 1= Legacy
    uint32 J10_2_Mode   : 1;      // J10.2 PHY Mode Selection, 0= Compliance, 1= Legacy
    uint32 J10_3_Mode   : 1;      // J10.3 PHY Mode Selection, 0= Compliance, 1= Legacy
    uint32 J10_4_Mode   : 1;      // J10.4 PHY Mode Selection, 0= Compliance, 1= Legacy

    uint32 J11_1_Mode   : 1;      // J11.1 PHY Mode Selection(PCIE), 0= Compliance, 1= Legacy
    uint32 J11_2_Mode   : 1;      // J11.2 PHY Mode Selection, 0= Compliance, 1= Legacy
    uint32 J11_3_Mode   : 1;      // J11.3 PHY Mode Selection, 0= Compliance, 1= Legacy
    uint32 J11_4_Mode   : 1;      // J11.4 PHY Mode Selection, 0= Compliance, 1= Legacy

    uint32 J12_1_Mode   : 1;      // J12.1 PHY Mode Selection, 0= Compliance, 1= Legacy
    uint32 J12_2_Mode   : 1;      // J12.2 PHY Mode Selection, 0= Compliance, 1= Legacy
    uint32 J12_3_Mode   : 1;      // J12.3 PHY Mode Selection, 0= Compliance, 1= Legacy
    uint32 J12_4_Mode   : 1;      // J12.4 PHY Mode Selection, 0= Compliance, 1= Legacy

    uint32 J10_1_M_S    : 1;      // J10.1 PHY M/S Selection, 0= Master, 1= Slave
    uint32 J10_2_M_S    : 1;      // J10.1 PHY M/S Selection, 0= Master, 1= Slave
    uint32 J10_3_M_S    : 1;      // J10.1 PHY M/S Selection, 0= Master, 1= Slave
    uint32 J10_4_M_S    : 1;      // J10.1 PHY M/S Selection, 0= Master, 1= Slave

    uint32 J11_1_M_S    : 1;      // J11.1 PHY M/S Selection(PCIE), 0= Master, 1= Slave
    uint32 J11_2_M_S    : 1;      // J11.1 PHY M/S Selection, 0= Master, 1= Slave
    uint32 J11_3_M_S    : 1;      // J11.1 PHY M/S Selection, 0= Master, 1= Slave
    uint32 J11_4_M_S    : 1;      // J11.1 PHY M/S Selection, 0= Master, 1= Slave

    uint32 J12_1_M_S    : 1;      // J12.1 PHY M/S Selection, 0= Master, 1= Slave
    uint32 J12_2_M_S    : 1;      // J12.1 PHY M/S Selection, 0= Master, 1= Slave
    uint32 J12_3_M_S    : 1;      // J12.1 PHY M/S Selection, 0= Master, 1= Slave
    uint32 J12_4_M_S    : 1;      // J12.1 PHY M/S Selection, 0= Master, 1= Slave

    uint32 Rev0         : 8;
}T_ROM_ETHPHY_MODE_MS_DETAIL;

typedef union _T_ROM_EthPhy_Mode_MS_Data
{
    uint8  uB[4];
    uint16 uW[2];
    uint32 uDW;

    T_ROM_ETHPHY_MODE_MS_DETAIL detail;
}T_ROM_ETHPHY_MODE_MS_DATA;






/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
// Create String Table
extern const char Flash_StrTbl_Legacy                        [FLASH_STRTBL_LEGACY_SIZE                         ];
extern const char Flash_StrTbl_Compliance                    [FLASH_STRTBL_COMPLIANCE_SIZE                     ];
extern const char Flash_StrTbl_Master                        [FLASH_STRTBL_MASTER_SIZE                         ];
extern const char Flash_StrTbl_Slave                         [FLASH_STRTBL_SLAVE_SIZE                          ];
extern const char Flash_StrTbl_Wr_Val_Success                [FLASH_STRTBL_WR_VAL_SUCCESS_SIZE                 ];
extern const char Flash_StrTbl_Wr_Val_Fail1                  [FLASH_STRTBL_WR_VAL_FAIL1_SIZE                   ];
extern const char Flash_StrTbl_Rd_Val_Fail1                  [FLASH_STRTBL_RD_VAL_FAIL1_SIZE                   ];

extern const char Flash_StrTbl_Rd_Val_Fail_EthPHY_Mode_MS1   [FLASH_STRTBL_RD_VAL_FAIL_ETHPHY_MODE_MS1_SIZE    ];

extern const char Flash_StrTbl_Syntax_Err_Val1               [FLASH_STRTBL_SYNTAX_ERR_VAL1_SIZE                ];
extern const char Flash_StrTbl_Syntax_Err_Val2               [FLASH_STRTBL_SYNTAX_ERR_VAL2_SIZE                ];

extern const char Flash_StrTbl_Syntax_Err_Socket1            [FLASH_STRTBL_SYNTAX_ERR_SOCKET1_SIZE             ];
extern const char Flash_StrTbl_Syntax_Err_Socket2            [FLASH_STRTBL_SYNTAX_ERR_SOCKET2_SIZE             ];
extern const char Flash_StrTbl_Syntax_Err_Socket_pin1        [FLASH_STRTBL_SYNTAX_ERR_SOCKET_PIN1_SIZE         ];
extern const char Flash_StrTbl_Syntax_Err_Socket_pin2        [FLASH_STRTBL_SYNTAX_ERR_SOCKET_PIN2_SIZE         ];

// Group String 
extern const char *Flash_Str_PHY_Mode[FLASH_STR_PHY_MODE_TOL];
extern const char *Flash_Str_PHY_MasterSlave[FLASH_STR_PHY_MASTERSLAVE_TOL];






/******************************************************************************/
/*------------------------------Global functions------------------------------*/
/******************************************************************************/

void i2crom_drv_write(t_rom_object * rom_object_p, uint32 addr, const uint8* wbuf, uint32 bytes);
void i2crom_drv_read(t_rom_object * rom_object_p, uint32 addr, uint8* rbuf, uint32 bytes);

uint8 IgnitionChkEngage_ReadValue(uint8 *ubOutdata);
uint8 IgnitionChkEngage_WriteValue(uint8 *ubIndata);
IFX_EXTERN boolean IgnitionChkEngage_Sta(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean IgnitionChkEngage_Set(pchar args, void *data, IfxStdIf_DPipe *io);

uint8 EthPHYModeMS_ReadValue(uint8 *ubOutdata);
uint8 EthPHYModeMS_WriteValue(uint8 *ubIndata);
IFX_EXTERN boolean EthPHYModeMS_Sta(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean EthPHYMode_Set(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean EthPHYMS_Set(pchar args, void *data, IfxStdIf_DPipe *io);





#endif /* _I2C_I2CROM_DRV_H_ */


