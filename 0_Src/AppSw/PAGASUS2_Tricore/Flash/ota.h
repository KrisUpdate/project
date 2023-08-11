/********************************************************************************
** 																		     	*
**  Infineon TriCore Bootloader, CANLoader, Version 1.0                      	*
**																		     	*
**  Id: main.h 2013-06-11 15:02:32Z chen hanshun                     	     	*
**                                                                           	*
**  DESCRIPTION :                                                            	*
**      -Communication with PC via BL protocol                               	*
**      -Implementation of flash functions:         					     	*
**		  (1)Erase flash                                                     	*
**		  (2)Program flash												    	*
**		  (3)Verify flash								    			     	*
**		  (4)Protect flash												     	*
**  	  (5)Program SPRAM												     	*
**  	  (6)Run programmed flash user code from address 0xA0000000          	*
**  	  (7)Run programmed SPRAM user code from address 0xD4001400          	*
**  							                                             	*
**  REMARKS :                                                                	*
**      -Stack and CSA initialized        								     	*
**      -Interrupt and Trap table not initialized                            	*
**  							                                             	*
********************************************************************************/

#ifndef OTA_H_
#define OTA_H_

/*********************************include file**********************************/
#include <Cpu/Std/Ifx_Types.h>
#include <Cpu/Std/IfxCpu.h>
#include "_Reg/IfxScu_reg.h"
#include <RegisterCmds.h>
//#include <CDD_Spitp_Types.h>
#include "SysSe/Bsp/Bsp.h"
//#include "clib_flash.h"
#include "ota_debug.h"
#include "Configuration.h"

typedef unsigned char   			BYTE;
typedef unsigned int    			UINT;
typedef unsigned short  			WORD;
typedef unsigned long   			DWORD;
typedef unsigned long long 			QWORD;

typedef unsigned char  				ubyte;    // 1 byte unsigned; prefix: ub
typedef signed char    				sbyte;    // 1 byte signed;   prefix: sb
typedef unsigned short 				ushort;   // 2 byte unsigned; prefix: us
typedef signed short   				sshort;   // 2 byte signed;   prefix: ss
typedef unsigned int   				uword;    // 4 byte unsigned; prefix: uw
typedef signed int     				sword;    // 4 byte signed;   prefix: sw
//typedef unsigned int   				uint32;   // 4 byte unsigned; prefix: uw

/*******************************************************************************/

/*********************************FLASH CONSTANTS*******************************/
#define PAGE_SIZE        			256   			// program FLASH page size
#define DFLASH_PAGE_SIZE			32	  			// data FLASH burst page size

#define PFLASH_BURST_PAGE_MASK      0x0FF
#define DFLASH_BURST_PAGE_MASK      0x01F
#define PFLASH_PAGE_MASK      		0x01F
#define DFLASH_PAGE_MASK      		0x007

#define FLASHOPER      			    0x00000800      /* Flash Operation Error */
#define FLASHSQER        			0x00001000      /* Command Sequence Error */
#define FLASH_MASK_ERROR (FLASHOPER | FLASHSQER)

#define FLASH_PROTECTION_ERROR_MASK 0x00002000 		/* Protection Error */
#define FLASH_ERASE_VERIFY_ERROR    0x04000000      /* Erase Verify Error*/
#define FLASH_PROGRAM_VERIFY_ERROR  0x02000000      /* Program Verify Error*/

//Masks used in Erase procedure
#define PFLASH_MASK1 				0x3FFF			/*16K size*/
#define PFLASH_MASK2 				0x7FFF			/*32k size*/
#define PFLASH_MASK3 				0xFFFF			/*64k size*/
#define PFLASH_MASK4                0x1FFFF			/*128k size*/
#define PFLASH_MASK5                0x3FFFF			/*256k size*/

#define DFLASH_MASK1  				0x1FFF			/*8k size*/
#define DFLASH_MASK2  				0x3FF			/*1k size*/

#define disable_HSM					1				//HSM supported
/*******************************************************************************/

/*********************************BSL CONSTANTS*********************************/

#define BMHD_MAGIC					0xB359
#define BMHD_MODE					0x60

#if OLD_OTA_ENABLE == 1
#define EB_MSG_PADDING				24
#else
#define EB_MSG_PADDING				0
#endif
#define OTA_BLOCK_SIZE   			100
#define RESPONSE_BLOCK_SIZE			16
#define HEADER_BLOCK_SIZE  	   		16
													/*
													Data 0: Block type;
													Data 1: Mode;
													Data 2-14: Mode specific content;
													Data 15: Check sum
													*/
#define DATA_BLOCK_SIZE  	   		(PAGE_SIZE+8)
													/*
													Data 0: Block type;
													Data 1: Verification option;
													Data 2-33: 32 data bytes;
													Data 34-38: not used
													Data 39: Check sum
													*/

#define HEADER_BLOCK 		   		0x00
#define DATA_BLOCK 	 		   		0x01
#define EOT_BLOCK			   		0x02
#define RESPONSE_BLOCK				0x03

#define BSL_PROGRAM_FLASH      		0x00
#define BSL_RUN_FROM_FLASH     		0x01
#define BSL_PROGRAM_SPRAM      		0x02
#define BSL_RUN_FROM_SPRAM     		0x03
#define BSL_ERASE_FLASH        		0x04
#define BSL_PROTECT_FLASH      		0x06
#define BSL_BOOT_UPDATE_FW			0x07
#define BSL_BOOT_NORMAL_FW			0x08
#define BSL_GET_FW_VERSION			0x09
#define BSL_END_OF_UPDATE			0x0A

#define BSL_BLOCK_TYPE_ERROR   		0xFF
#define BSL_MODE_ERROR 		   		0xFE
#define BSL_CHKSUM_ERROR 	   		0xFD
#define BSL_ADDRESS_ERROR 	   		0xFC
#define BSL_ERASE_ERROR		   		0xFB
#define BSL_PROGRAM_ERROR	   		0xFA
#define BSL_VERIFICATION_ERROR 		0xF9
#define BSL_PROTECTION_ERROR   		0xF8
#define BSL_FLASH_ERROR_STATUS      0xF7
#define BSL_SIZE_ERROR		   		0xF6
#define BSL_CHIPID_ERROR            0xF5
#define BSL_UPDATE_FW_ERROR			0xF4
#define BSL_SUCCESS 		   		0x55

#define OTA_STATE_INIT				0x01
#define OTA_STATE_WAIT_FOR_HEADER	0x02
#define OTA_STATE_WAIT_FOR_DATA		0x03
#define OTA_STATE_SEND_RESPONSE		0x04

#define FLASH_MODE					0x01
#define SPRAM_MODE					0x02

#define TYPE_EB						0x01
#define TYPE_UPDATE					0x02


/*******************************************************************************/

/*********************************external function*****************************/
extern BYTE Read_CHIPID(void);
//#if ETHERNET_OTA_ENABLE == 1
void ota_handle_state(uint8* pData, uint16 len);
//#endif
typedef void (*rec_ptr)(uint8* data, uint16 len);
#if 0
void ota_udp_app_ota_init(uint32 ip32);
void ota_udp_app_send(uint8* buf, uint8 len);
void ota_udp_app_register_rec_cb(rec_ptr receive_cb);
#else
void udp_app_ota_init(uint32 ip32);
void ota_udp_app_send(uint8* buf, uint8 len);
void udp_app_register_rec_ota_cb(rec_ptr receive_cb);
#endif
void OTA_TegraEthComm_init(void);
void OTA_TegraEthComm_resume(void);
void OTA_TegraEthComm_routine(void);

/*******************************************************************************/


#endif /* OTA_H_ */
