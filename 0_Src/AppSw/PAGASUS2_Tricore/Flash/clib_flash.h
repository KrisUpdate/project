/* -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*- */
/**
* \defgroup clib_flash clib_flash: functions to erase/program flash sectors
*
* \author Berenice Terebinto / Thorsten Klose
* Copyright(c) INFINEON TECHNOLOGIES 2010 all rights reserved
*
* \{
*/

#ifndef _CLIB_FLASH_H
#define _CLIB_FLASH_H

#include "Configuration.h"
#if MCU_PLATFORM == _AURIX_TC29X_
#include "Cpu/Std/IfxCpu_Intrinsics.h"
#include "_Reg/IfxFlash_reg.h"
#include "_Reg/IfxFlash_regdef.h"


#define CLIB_FLASH_TYPE_FA         0
#define CLIB_FLASH_TYPE_D0         1
#define CLIB_FLASH_TYPE_D1         2
#define CLIB_FLASH_TYPE_P0         3
#define CLIB_FLASH_TYPE_P1         4
#define CLIB_FLASH_TYPE_P2         5
#define CLIB_FLASH_TYPE_P3         6

// this macro is used to address SFRs directly in C programs
#ifndef MEM
# define MEM(address) (*((volatile unsigned int *)(address)))  // makes live easier
#endif
#ifndef MEM16
# define MEM16(address) (*((volatile unsigned short *)(address)))
#endif
#ifndef MEM8
# define MEM8(address) (*((volatile unsigned char *)(address)))
#endif

//! sector address and page offset definitions for different derivatives

//! base address for general command sequences like "reset to read"
# define CLIB_FLASH_CMD_BASE					0xaf000000

//! number of flash modules, and offset between modules (only relevant if number > 1)
# define CLIB_FLASH_NUMBER_MODULES      		1 //! number of PMUs

# define CLIB_FLASH_P_OFFSET            		0x00800000 //! offset between PMU PFlash ranges
# define CLIB_FLASH_CMD_BASE_OFFSET     		0x00100000 //! offset between command areas (in DFlash range)

//! program flash
# define CLIB_FLASH_P_START             		0xa0000000
//# define TC26x_TC23x_CLIB_FLASH_P_BANK_OFFSET   0x00100000
//# define TC29x_TC27x_CLIB_FLASH_P_BANK_OFFSET   0x00200000
//# define TC24x_CLIB_FLASH_P_BANK_OFFSET         0x00080000
# define CLIB_FLASH_P_PAGE_LENGTH       		0x20
# define CLIB_FLASH_P_BURST_LENGTH      		0x100

//#ifdef DERIVATIVE_TC29x
# define TC29x_CLIB_FLASH_P_SIZE              	0x00800000
# define TC29x_CLIB_FLASH_P_BANKS             	4
# define TC29x_CLIB_FLASH_P0_START            	0xa0000000
# define TC29x_CLIB_FLASH_P0_SIZE             	0x00200000
# define TC29x_CLIB_FLASH_P0_END              	0xa01fffff
# define TC29x_CLIB_FLASH_P1_START            	0xa0200000
# define TC29x_CLIB_FLASH_P1_SIZE             	0x00200000
# define TC29x_CLIB_FLASH_P1_END              	0xa03fffff
# define TC29x_CLIB_FLASH_P2_START            	0xa0400000
# define TC29x_CLIB_FLASH_P2_SIZE             	0x00200000
# define TC29x_CLIB_FLASH_P2_END              	0xa05fffff
# define TC29x_CLIB_FLASH_P3_START            	0xa0600000
# define TC29x_CLIB_FLASH_P3_SIZE             	0x00200000
# define TC29x_CLIB_FLASH_P3_END              	0xa07fffff

# define TC29x_CLIB_FLASH_P_END               	(CLIB_FLASH_P_START + TC29x_CLIB_FLASH_P_SIZE-1)
# define TC29x_CLIB_FLASH_P_PAGES             	(TC29x_CLIB_FLASH_P_SIZE / CLIB_FLASH_P_PAGE_LENGTH)

/*TC29x has PF0 = 2.0MB => 4 phy sectors => 27 logical sectors
			PF1 = 2.0MB => 4 phy sectors => 27 logical sectors
			PF2 = 2.0MB => 4 phy sectors => 27 logical sectors
			PF3 = 2.0MB => 4 phy sectors => 27 logical sectors
*/
# define TC29x_CLIB_FLASH_P0_PHYS_SECTORS		4
# define TC29x_CLIB_FLASH_P0_LOG_SECTORS		27
# define TC29x_CLIB_FLASH_P1_PHYS_SECTORS		4
# define TC29x_CLIB_FLASH_P1_LOG_SECTORS		27
# define TC29x_CLIB_FLASH_P2_PHYS_SECTORS		4
# define TC29x_CLIB_FLASH_P2_LOG_SECTORS		27
# define TC29x_CLIB_FLASH_P3_PHYS_SECTORS		4
# define TC29x_CLIB_FLASH_P3_LOG_SECTORS		27
# define TC29x_CLIB_FLASH_P_PHYS_SECTORS		(TC29x_CLIB_FLASH_P0_PHYS_SECTORS*4)
# define TC29x_CLIB_FLASH_P_LOG_SECTORS	    	(TC29x_CLIB_FLASH_P0_LOG_SECTORS*4)

//#ifdef DERIVATIVE_TC27x
# define TC27x_CLIB_FLASH_P_SIZE              	0x00400000
# define TC27x_CLIB_FLASH_P_BANKS             	2
# define TC27x_CLIB_FLASH_P0_START            	0xa0000000
# define TC27x_CLIB_FLASH_P0_SIZE             	0x00200000
# define TC27x_CLIB_FLASH_P0_END              	0xa01fffff
# define TC27x_CLIB_FLASH_P1_START            	0xa0200000
# define TC27x_CLIB_FLASH_P1_SIZE             	0x00200000
# define TC27x_CLIB_FLASH_P1_END              	0xa03fffff

# define TC27x_CLIB_FLASH_P_END               	(CLIB_FLASH_P_START + TC27x_CLIB_FLASH_P_SIZE-1)
# define TC27x_CLIB_FLASH_P_PAGES             	(TC27x_CLIB_FLASH_P_SIZE / CLIB_FLASH_P_PAGE_LENGTH)

/*TC27x has PF0 = 2.0MB => 4 phy sectors => 27 logical sectors
			PF1 = 2.0MB => 4 phy sectors => 27 logical sectors
*/
# define TC27x_CLIB_FLASH_P0_PHYS_SECTORS		4
# define TC27x_CLIB_FLASH_P0_LOG_SECTORS		27
# define TC27x_CLIB_FLASH_P1_PHYS_SECTORS		4
# define TC27x_CLIB_FLASH_P1_LOG_SECTORS		27
# define TC27x_CLIB_FLASH_P_PHYS_SECTORS		(TC27x_CLIB_FLASH_P0_PHYS_SECTORS*2)
# define TC27x_CLIB_FLASH_P_LOG_SECTORS	    	(TC27x_CLIB_FLASH_P0_LOG_SECTORS*2)

//#ifdef DERIVATIVE_TC26x
# define TC26x_CLIB_FLASH_P_SIZE              	0x00280000
# define TC26x_CLIB_FLASH_P_BANKS             	2
# define TC26x_CLIB_FLASH_P0_START            	0xa0000000
# define TC26x_CLIB_FLASH_P0_SIZE             	0x00100000
# define TC26x_CLIB_FLASH_P0_END              	0xa00fffff
# define TC26x_CLIB_FLASH_P1_START            	0xa0100000
# define TC26x_CLIB_FLASH_P1_SIZE             	0x00180000
# define TC26x_CLIB_FLASH_P1_END              	0xa027ffff

# define TC26x_CLIB_FLASH_P_END               	(CLIB_FLASH_P_START + TC26x_CLIB_FLASH_P_SIZE-1)
# define TC26x_CLIB_FLASH_P_PAGES             	(TC26x_CLIB_FLASH_P_SIZE / CLIB_FLASH_P_PAGE_LENGTH)

/*TC26x has PF0 = 1.0MB => 2phy sectors => 23 logical sectors
            PF1 = 1.5MB => 3phy sectors => 25 logical sectors
*/
# define TC26x_CLIB_FLASH_P0_PHYS_SECTORS		2
# define TC26x_CLIB_FLASH_P0_LOG_SECTORS		23
# define TC26x_CLIB_FLASH_P1_PHYS_SECTORS		3
# define TC26x_CLIB_FLASH_P1_LOG_SECTORS		25
# define TC26x_CLIB_FLASH_P_PHYS_SECTORS		(TC26x_CLIB_FLASH_P0_PHYS_SECTORS + TC26x_CLIB_FLASH_P1_PHYS_SECTORS)
# define TC26x_CLIB_FLASH_P_LOG_SECTORS	    	(TC26x_CLIB_FLASH_P0_LOG_SECTORS + TC26x_CLIB_FLASH_P1_LOG_SECTORS)

//#ifdef DERIVATIVE_TC24x
# define TC24x_CLIB_FLASH_P_SIZE              	0x00200000
# define TC24x_CLIB_FLASH_P_BANKS             	2
# define TC24x_CLIB_FLASH_P0_START            	0xa0000000
# define TC24x_CLIB_FLASH_P0_SIZE             	0x00080000
# define TC24x_CLIB_FLASH_P0_END              	0xa007ffff
# define TC24x_CLIB_FLASH_P1_START            	0xa0080000
# define TC24x_CLIB_FLASH_P1_SIZE             	0x00180000
# define TC24x_CLIB_FLASH_P1_END              	0xa01fffff

# define TC24x_CLIB_FLASH_P_END               	(CLIB_FLASH_P_START + TC24x_CLIB_FLASH_P_SIZE-1)
# define TC24x_CLIB_FLASH_P_PAGES             	(TC24x_CLIB_FLASH_P_SIZE / CLIB_FLASH_P_PAGE_LENGTH)

/*TC24x has PF0 = 0.5MB => 1 phy sectors => 18 logical sectors
			PF1 = 1.5MB => 3 phy sectors => 25 logical sectors
*/
# define TC24x_CLIB_FLASH_P0_PHYS_SECTORS		1
# define TC24x_CLIB_FLASH_P0_LOG_SECTORS		18
# define TC24x_CLIB_FLASH_P1_PHYS_SECTORS		3
# define TC24x_CLIB_FLASH_P1_LOG_SECTORS		25
# define TC24x_CLIB_FLASH_P_PHYS_SECTORS		(TC24x_CLIB_FLASH_P0_PHYS_SECTORS + TC24x_CLIB_FLASH_P1_PHYS_SECTORS)
# define TC24x_CLIB_FLASH_P_LOG_SECTORS	    	(TC24x_CLIB_FLASH_P0_LOG_SECTORS + TC24x_CLIB_FLASH_P1_LOG_SECTORS)

//#ifdef DERIVATIVE_TC23x
# define TC23x_CLIB_FLASH_P_SIZE              	0x00200000
# define TC23x_CLIB_FLASH_P_BANKS             	2
# define TC23x_CLIB_FLASH_P0_START            	0xa0000000
# define TC23x_CLIB_FLASH_P0_SIZE             	0x00100000
# define TC23x_CLIB_FLASH_P0_END              	0xa00fffff
# define TC23x_CLIB_FLASH_P1_START            	0xa0100000
# define TC23x_CLIB_FLASH_P1_SIZE             	0x00100000
# define TC23x_CLIB_FLASH_P1_END              	0xa01fffff

# define TC23x_CLIB_FLASH_P_END               	(CLIB_FLASH_P_START + TC23x_CLIB_FLASH_P_SIZE-1)
# define TC23x_CLIB_FLASH_P_PAGES             	(TC23x_CLIB_FLASH_P_SIZE / CLIB_FLASH_P_PAGE_LENGTH)

/*TC23x has PF0 = 1.0MB => 2 phy sectors => 23 logical sectors
			PF1 = 1.0MB => 2 phy sectors => 23 logical sectors
 */
# define TC23x_CLIB_FLASH_P0_PHYS_SECTORS		2
# define TC23x_CLIB_FLASH_P0_LOG_SECTORS		23
# define TC23x_CLIB_FLASH_P1_PHYS_SECTORS		2
# define TC23x_CLIB_FLASH_P1_LOG_SECTORS		23
# define TC23x_CLIB_FLASH_P_PHYS_SECTORS		(TC23x_CLIB_FLASH_P0_PHYS_SECTORS + TC23x_CLIB_FLASH_P1_PHYS_SECTORS)
# define TC23x_CLIB_FLASH_P_LOG_SECTORS	    	(TC23x_CLIB_FLASH_P0_LOG_SECTORS + TC23x_CLIB_FLASH_P1_LOG_SECTORS)

//#ifdef DERIVATIVE_TC22x
# define TC22x_CLIB_FLASH_P_SIZE              	0x00100000
# define TC22x_CLIB_FLASH_P_BANKS             	1
# define TC22x_CLIB_FLASH_P0_START            	0xa0000000
# define TC22x_CLIB_FLASH_P0_SIZE             	0x00100000
# define TC22x_CLIB_FLASH_P0_END              	0xa00fffff

# define TC22x_CLIB_FLASH_P_END               	(CLIB_FLASH_P_START + TC22x_CLIB_FLASH_P_SIZE-1)
# define TC22x_CLIB_FLASH_P_PAGES             	(TC22x_CLIB_FLASH_P_SIZE / CLIB_FLASH_P_PAGE_LENGTH)

//TC22x has PF0 = 1.0MB => 2 phy sectors => 23 logical sectors
# define TC22x_CLIB_FLASH_P0_PHYS_SECTORS		2
# define TC22x_CLIB_FLASH_P0_LOG_SECTORS		23
# define TC22x_CLIB_FLASH_P_PHYS_SECTORS		TC22x_CLIB_FLASH_P0_PHYS_SECTORS
# define TC22x_CLIB_FLASH_P_LOG_SECTORS	    	TC22x_CLIB_FLASH_P0_LOG_SECTORS

//#ifdef DERIVATIVE_TC21x
# define TC21x_CLIB_FLASH_P_SIZE              	0x00080000
# define TC21x_CLIB_FLASH_P_BANKS             	1
# define TC21x_CLIB_FLASH_P0_START            	0xa0000000
# define TC21x_CLIB_FLASH_P0_SIZE             	0x00080000
# define TC21x_CLIB_FLASH_P0_END              	0xa007ffff

# define TC21x_CLIB_FLASH_P_END               	(CLIB_FLASH_P_START + TC21x_CLIB_FLASH_P_SIZE-1)
# define TC21x_CLIB_FLASH_P_PAGES             	(TC21x_CLIB_FLASH_P_SIZE / CLIB_FLASH_P_PAGE_LENGTH)

//TC21x has PF0 = 0.5MB => 1 phy sectors => 18 logical sectors
# define TC21x_CLIB_FLASH_P0_PHYS_SECTORS		1
# define TC21x_CLIB_FLASH_P0_LOG_SECTORS		18
# define TC21x_CLIB_FLASH_P_PHYS_SECTORS		TC21x_CLIB_FLASH_P0_PHYS_SECTORS
# define TC21x_CLIB_FLASH_P_LOG_SECTORS	    	TC21x_CLIB_FLASH_P0_LOG_SECTORS

//! data flash
//! Note: UCB and HSM addresses only available in clib_flash_table_d_log and clib_flash_table_phys_log\n
//! Separate defines are not available, as for the final TC27x these data sections will be part of a single sector anyhow!
# define CLIB_FLASH_D_START             		0xaf000000
# define CLIB_FLASH_D_BANKS             		1
# define CLIB_FLASH_D_BANK_OFFSET       		0x00100000
# define CLIB_FLASH_D_PAGE_LENGTH       		0x8
# define CLIB_FLASH_D_BURST_LENGTH      		0x20
# define CLIB_FLASH_D0_START            		(CLIB_FLASH_D_START+0*CLIB_FLASH_D_BANK_OFFSET)
# define CLIB_FLASH_D0_UCB_START                (CLIB_FLASH_D_START+ 1 * CLIB_FLASH_D_BANK_OFFSET)

# define TC29x_CLIB_FLASH_D_SIZE            	0x000C0000 		//768k
# define TC27x_CLIB_FLASH_D_SIZE            	0x00060000 		//384k
# define TC26x_CLIB_FLASH_D_SIZE            	0x00018000 		//96k
# define TC24x_CLIB_FLASH_D_SIZE            	0x0000C000 		//48k
# define TC23x_CLIB_FLASH_D_SIZE            	0x00020000 		//128k
# define TC22x_CLIB_FLASH_D_SIZE            	0x00018000 		//96k
# define TC21x_CLIB_FLASH_D_SIZE            	0x00010000 		//64k
# define CLIB_FLASH_D_UCB_SIZE                  0x00004000 		//16k

# define TC29x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC29x_CLIB_FLASH_D_SIZE - 1)
# define TC27x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC27x_CLIB_FLASH_D_SIZE - 1)
# define TC26x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC26x_CLIB_FLASH_D_SIZE - 1)
# define TC24x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC24x_CLIB_FLASH_D_SIZE - 1)
# define TC23x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC23x_CLIB_FLASH_D_SIZE - 1)
# define TC22x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC22x_CLIB_FLASH_D_SIZE - 1)
# define TC21x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC21x_CLIB_FLASH_D_SIZE - 1)
# define CLIB_FLASH_D_UCB_END           	    (CLIB_FLASH_D0_UCB_START + CLIB_FLASH_D_UCB_SIZE - 1)

# define TC29x_CLIB_FLASH_D0_SIZE           	0x000C0000		//768k
# define TC27x_CLIB_FLASH_D0_SIZE           	0x00060000		//384k
# define TC26x_CLIB_FLASH_D0_SIZE           	0x00018000		//96k
# define TC24x_CLIB_FLASH_D0_SIZE           	0x0000C000		//48k
# define TC23x_CLIB_FLASH_D0_SIZE           	0x00020000		//128k
# define TC22x_CLIB_FLASH_D0_SIZE           	0x00018000		//96k
# define TC21x_CLIB_FLASH_D0_SIZE           	0x00010000		//64k

# define TC29xCLIB_FLASH_D0_LOGIC_SIZE     		0x00020000		//128k
# define TC27xCLIB_FLASH_D0_LOGIC_SIZE     		0x00010000		//64k
# define TC26xCLIB_FLASH_D0_LOGIC_SIZE     		0x00004000		//16k
# define TC24xCLIB_FLASH_D0_LOGIC_SIZE     		0x00002000		//8k
# define TC23xCLIB_FLASH_D0_LOGIC_SIZE     		0x00008000      //32k,TBD
# define TC22xCLIB_FLASH_D0_LOGIC_SIZE     		0x00004000		//16k
# define TC21xCLIB_FLASH_D0_LOGIC_SIZE     		0x00004000   	//16k,TBD
# define CLIB_FLASH_D0_UCB_LOGIC_SIZE   	    0x00000400   	//1k

# define TC29x_CLIB_FLASH_D0_END            	(CLIB_FLASH_D0_START + TC29x_CLIB_FLASH_D0_SIZE-1)
# define TC27x_CLIB_FLASH_D0_END            	(CLIB_FLASH_D0_START + TC27x_CLIB_FLASH_D0_SIZE-1)
# define TC26x_CLIB_FLASH_D0_END            	(CLIB_FLASH_D0_START + TC26x_CLIB_FLASH_D0_SIZE-1)
# define TC24x_CLIB_FLASH_D0_END            	(CLIB_FLASH_D0_START + TC24x_CLIB_FLASH_D0_SIZE-1)
# define TC23x_CLIB_FLASH_D0_END            	(CLIB_FLASH_D0_START + TC23x_CLIB_FLASH_D0_SIZE-1)
# define TC22x_CLIB_FLASH_D0_END            	(CLIB_FLASH_D0_START + TC22x_CLIB_FLASH_D0_SIZE-1)
# define TC21x_CLIB_FLASH_D0_END            	(CLIB_FLASH_D0_START + TC21x_CLIB_FLASH_D0_SIZE-1)

# define TC29x_CLIB_FLASH_D_PAGES     			(((TC29x_CLIB_FLASH_D0_END - CLIB_FLASH_D0_START+1)/CLIB_FLASH_D_PAGE_LENGTH))
# define TC27x_CLIB_FLASH_D_PAGES     			(((TC27x_CLIB_FLASH_D0_END - CLIB_FLASH_D0_START+1)/CLIB_FLASH_D_PAGE_LENGTH))
# define TC26x_CLIB_FLASH_D_PAGES     			(((TC26x_CLIB_FLASH_D0_END - CLIB_FLASH_D0_START+1)/CLIB_FLASH_D_PAGE_LENGTH))
# define TC24x_CLIB_FLASH_D_PAGES     			(((TC24x_CLIB_FLASH_D0_END - CLIB_FLASH_D0_START+1)/CLIB_FLASH_D_PAGE_LENGTH))
# define TC23x_CLIB_FLASH_D_PAGES     			(((TC23x_CLIB_FLASH_D0_END - CLIB_FLASH_D0_START+1)/CLIB_FLASH_D_PAGE_LENGTH))
# define TC22x_CLIB_FLASH_D_PAGES     			(((TC22x_CLIB_FLASH_D0_END - CLIB_FLASH_D0_START+1)/CLIB_FLASH_D_PAGE_LENGTH))
# define TC21x_CLIB_FLASH_D_PAGES     			(((TC21x_CLIB_FLASH_D0_END - CLIB_FLASH_D0_START+1)/CLIB_FLASH_D_PAGE_LENGTH))

//! data flash sectors
# define TC29x_CLIB_FLASH_D_PHYS_SECTORS		(3)  // Not used anyway for Tc29x
# define TC29x_CLIB_FLASH_D_LOG_SECTORS			(96) // 96 EEPROMS
# define TC29x_CLIB_FLASH_U_LOG_SECTORS			(16) // 16 UCBs
# define TC29x_CLIB_FLASH_H_LOG_SECTORS			(8)  // 8 Hsm Sectors on DF1

# define TC27x_CLIB_FLASH_D_PHYS_SECTORS		(3)  // Not used anyway for Tc27x
# define TC27x_CLIB_FLASH_D_LOG_SECTORS			(48) // 48 EEPROMS
# define TC27x_CLIB_FLASH_U_LOG_SECTORS			(16) // 16 UCBs
# define TC27x_CLIB_FLASH_H_LOG_SECTORS			(8)  // 8 Hsm Sectors on DF1

# define TC26x_CLIB_FLASH_D_PHYS_SECTORS		(3)  // Not used anyway for Tc26x
# define TC26x_CLIB_FLASH_D_LOG_SECTORS			(12) // 12 EEPROMS
# define TC26x_CLIB_FLASH_U_LOG_SECTORS			(16) // 16 UCBs
//# define TC26x_CLIB_FLASH_H_LOG_SECTORS		(8)  // 8 Hsm Sectors on DF1

# define TC24x_CLIB_FLASH_D_PHYS_SECTORS		(3)  // Not used anyway for Tc24x
# define TC24x_CLIB_FLASH_D_LOG_SECTORS			(6) // 6 EEPROMS
# define TC24x_CLIB_FLASH_U_LOG_SECTORS			(16) // 16 UCBs
//# define TC24x_CLIB_FLASH_H_LOG_SECTORS		(8)  // 8 Hsm Sectors on DF1

# define TC23x_CLIB_FLASH_D_PHYS_SECTORS		(3)  // Not used anyway for Tc23x
# define TC23x_CLIB_FLASH_D_LOG_SECTORS			(16) // 16 EEPROMS
# define TC23x_CLIB_FLASH_U_LOG_SECTORS			(16) // 16 UCBs
# define TC23x_CLIB_FLASH_H_LOG_SECTORS			(8)  // 8 Hsm Sectors on DF1

# define TC22x_CLIB_FLASH_D_PHYS_SECTORS		(3)  // Not used anyway for Tc22x
# define TC22x_CLIB_FLASH_D_LOG_SECTORS			(12) // 12 EEPROMS
# define TC22x_CLIB_FLASH_U_LOG_SECTORS			(16) // 16 UCBs
//# define TC22x_CLIB_FLASH_H_LOG_SECTORS		(8)  // 8 Hsm Sectors on DF1

# define TC21x_CLIB_FLASH_D_PHYS_SECTORS		(3)  // Not used anyway for Tc21x
# define TC21x_CLIB_FLASH_D_LOG_SECTORS			(8)  // 8 EEPROMS
# define TC21x_CLIB_FLASH_U_LOG_SECTORS			(16) // 16 UCBs
//# define TC21x_CLIB_FLASH_H_LOG_SECTORS		(8)  // 8 Hsm Sectors on DF1

//! Performs the "Reset to Read" sequence on all available flash modules
//! \param[in] flash selects the flash (PMU) module
//! \return 0 on success, != 0 on error
//! <B>Example:</B>
//! \code
//! // reset to read mode
//! result |= clib_flash_reset_to_read();
//! \endcode
extern int clib_flash_reset_to_read(int flash);

//! Performs the sequence for entering program page mode.\n
//! \param[in] pageAddr specifies the page beeing written - the command sequence will be varied accordingly
//! \return 0 on success, != 0 if invalid or not available page is selected
extern int clib_flash_enter_page_mode(unsigned pageAddr);

//! performs a load page sequence with a single 64bit access.\n
//! param[in] pageAddr start address of page which should be programmed
//! \return 0 on success, != 0 if invalid or not available page is selected
//! <B>Example:</B>
//! \code
//! // load 64bit into assembly buffer of program flash
//! result |= clib_flash_load_page(CLIB_FLASH_P_START, 0x55555555, 0xaaaaaaaa);
//!
//! // load 64bit into assembly buffer of data flash
//! result |= clib_flash_load_page(CLIB_FLASH_D0_START, 0x55555555, 0xaaaaaaaa);
//! \endcode
extern int clib_flash_load_page(unsigned pageAddr, unsigned wordL, unsigned wordU);

//! works similar to clib_flash_load_page, but performs a load page sequence
//! with two 32bit accesses instead of a single doubleword access.
//! param[in] pageAddr start address of page which should be programmed
//! \return 0 on success, != 0 if invalid or not available page is selected
//! <B>Example:</B>
//! \code
//! // load 2*32bit into assembly buffer of program flash
//! result |= clib_flash_load_page_2x32(CLIB_FLASH_P_START, 0x55555555, 0xaaaaaaaa);
//!
//! // load 2*32bit into assembly buffer of data flash
//! result |= clib_flash_load_page_2x32(CLIB_FLASH_D0_START, 0x55555555, 0xaaaaaaaa);
//! \endcode
extern int clib_flash_load_page_2x32(unsigned int pageAddr, unsigned int wordL, unsigned int wordU);

//! Performs the "Write Page" sequence
//! param[in] pageAddr start address of page which should be programmed
//! \return 0 on success, != 0 if invalid or not available page is selected
//! <B>Example:</B>
//! \code
//! // program the second page of the first sector of the Program Flash
//! result |= clib_flash_write_page(0xa0000100);
//!
//! // alternatively the write could be initiated in this way:
//! result |= clib_flash_write_page(CLIB_FLASH_P_START + 1*CLIB_FLASH_P_PAGE_LENGTH);
//!
//!
//! ///////////////////////////////////////////////////////////////
//! // following example writes a pattern into the first data flash
//! 
//! // erase data flash
//! result |= clib_flash_erase_phys_sector(CLIB_FLASH_D0_START);
//!
//! // wait until unbusy
//! clib_flash_wait_unbusy(CLIB_FLASH_TYPE_D0);
//!
//! // program all pages
//! for(page=0; page<(CLIB_FLASH_D0_END+1-CLIB_FLASH_D0_START)/CLIB_FLASH_D_PAGE_LENGTH; ++page) {
//!   unsigned int pageAddr = CLIB_FLASH_D0_START + page*CLIB_FLASH_D_PAGE_LENGTH;
//!
//!   // enter page mode
//!   result |= clib_flash_enter_page_mode(pageAddr);
//!
//!   // write 128 bytes (16 doublewords) into assembly buffer
//!   for(offset=0; offset<CLIB_FLASH_D_PAGE_LENGTH; offset+=8)
//!     result |= clib_flash_load_page(pageAddr, pageAddr ^ 0xffffffff, offset);
//!
//!   // write page
//!   result |= clib_flash_write_page(pageAddr);
//!
//!   // wait until unbusy
//!   clib_flash_wait_unbusy(CLIB_FLASH_TYPE_D0);
//! }
//! \endcode
extern int clib_flash_write_page(unsigned pageAddr);

//! Performs the "Write Page Once" sequence, similar to write page
//! but performs a program verify after writing.
//! param[in] pageAddr start address of page which should be programmed
//! \return 0 on success, != 0 if invalid or not available page is selected
//! <B>Example:</B>
//! \code
//! // program the second page of the first sector of the Program Flash
//! result |= clib_flash_write_page_once(0xa0000100);
//!
//! // alternatively the write could be initiated in this way:
//! result |= clib_flash_write_page_once(CLIB_FLASH_P_START + 1*CLIB_FLASH_P_PAGE_LENGTH);
//! \endcode
extern int clib_flash_write_page_once(unsigned pageAddr);

//! Performs the "Write Burst" sequence, similar to write page
//! but performs a burst transfer instead of page.Make sure the appropriate
//! amount of data is loaded using load page command 
//! param[in] pageAddr start address of page which should be programmed
//! \return 0 on success, != 0 if invalid or not available page is selected
//! <B>Example:</B>
//! \code
//! // program the second page of the first sector of the Program Flash
//! result |= clib_flash_write_burst(0xa0000100);
//!
//! // alternatively the write could be initiated in this way:
//! result |= clib_flash_write_page_burst(CLIB_FLASH_P_START + 1*CLIB_FLASH_P_PAGE_LENGTH);
//! \endcode
extern int clib_flash_write_burst(unsigned pageAddr);


//! Performs the erase sequence for a sector in program or data flash.\n
//! use clib_flash_erase_physical_sector() for erasing a physical sector in program flash.
//! \param[in] sectorAddr the sector address
//! \return 0 on success, != 0 if invalid or not available sector has been selected
//! <B>Example:</B>
//! \code
//! // erase logical sectors of program flash
//! result |= clib_flash_erase_sector(CLIB_FLASH_P_S0_START);
//! result |= clib_flash_erase_sector(CLIB_FLASH_P_S1_START);
//!
//! // erase all sectors of program flash
//! for(sector=0; sector<CLIB_FLASH_P_LOG_SECTORS; ++sector) {
//!   // get address from predefined table
//!   unsigned int sector_addr = CLIB_FLASH_P_LOG_START[sector];
//!   // log address
//!   LOG_R_VARIABLE(sector_addr);
//!   // erase sector
//!   result |= clib_flash_erase_sector(sector_addr);
//! }
//! \endcode
extern int clib_flash_erase_sector(unsigned int sectorAddr);

//! Performs the erase sequence for a physical sector in program or data flash
//! \return 0 on success, != 0 if invalid or not available sector has been selected
//! <B>Example:</B>
//! \code
//! // erase the first and second data flash
//! result |= clib_flash_erase_phys_sector(CLIB_FLASH_D0_START);
//! result |= clib_flash_erase_phys_sector(CLIB_FLASH_D1_START);
//!
//! // erase the first two physical sectors of program flash (Metis)
//! result |= clib_flash_erase_phys_sector(CLIB_FLASH_P_S0_S3_START);
//! result |= clib_flash_erase_phys_sector(CLIB_FLASH_P_S4_S7_START);
//! \endcode
extern int clib_flash_erase_phys_sector(unsigned int sector_addr);

//! Performs the "Disable Write Protection" sequence.
//! \param[in] flash selects the flash (PMU) module
//! \param[in] ucb selects the user configuration block (0 for UCB0, 1 for UCB1, 5 for UCB_HSMC)
//! \param[in] password pointer to an array of 8 words
extern int clib_flash_dis_write_prot(int flash, int ucb, unsigned password[8]);

//! Performs the "Resume Protection" sequence
//! \param[in] flash selects the flash (PMU) module
//! \return 0 on success, != 0 on errors
extern int clib_flash_resume_prot(int flash);

//! Performs the "Suspend Resume" sequence
//! \param[in] sectorAddr sector address which should be resumed
//! \return 0 on success, != 0 on errors
extern int clib_flash_suspend_resume(unsigned int sectorAddr);

//! Performs the "Erase Verify" sequence
//! \param[in] sectorAddr sector address which should be verified
//! \return 0 on success, != 0 on errors
extern int clib_flash_erase_verify(unsigned int sectorAddr);

//! Performs the "Clear Status" sequence
//! \return 0 on success, != 0 on errors
extern int clib_flash_clear_status(int flash);

//! Polls the selected status flag in flash status register until it turns to 0\n
//! \param[in] flash selects the flash (PMU) module
//! \param[in] flashType selects the flash type:
//!   \li <B>CLIB_FLASH_TYPE_FA:</B> Flash Array
//!   \li <B>CLIB_FLASH_TYPE_P0</B>: program flash #0
//!   \li <B>CLIB_FLASH_TYPE_P1</B>: program flash #1
//!   \li <B>CLIB_FLASH_TYPE_P2</B>: program flash #2
//!   \li <B>CLIB_FLASH_TYPE_P3</B>: program flash #3
//!   \li <B>CLIB_FLASH_TYPE_D0</B>: data flash #0
//!   \li <B>CLIB_FLASH_TYPE_D1</B>: data flash #1
//! \return 0 on success, != 0 on errors
//! <B>Example:</B>
//! \code
//! // wait until program flash is unbusy
//! result |= clib_flash_wait_unbusy(0, CLIB_FLASH_TYPE_P);
//! 
//! // wait until data flash 0 is unbusy
//! result |= clib_flash_wait_unbusy(0, CLIB_FLASH_TYPE_D0);
//! 
//! // wait until data flash 1 is unbusy
//! result |= clib_flash_wait_unbusy(0, CLIB_FLASH_TYPE_D1);
//! 
//! // wait until FSI is unbusy
//! result |= clib_flash_wait_unbusy(0, CLIB_FLASH_TYPE_FSI);
//! \endcode
extern int clib_flash_wait_unbusy(int flash, int flashType);

//! Performs the erase sequence for n  sectors in program or data flash.\n
//! use clib_flash_erase_physical_multiple_sector() for erasing n physical sectors in program flash.
//! \param[in] sectorAddr the sector address
//! \param[in] numSector  the no.of sectors to be operated on
//! \return 0 on success, != 0 if invalid or not available sector has been selected
//! <B>Example:</B>
//! \code
//! // erase logical sectors of program flash
//! result |= clib_flash_erase_multiple_sector(CLIB_FLASH_P_S0_START,2);
//! result |= clib_flash_erase_multiple_sector(CLIB_FLASH_P_S1_START,3);
//!
//! \endcode
extern int clib_flash_erase_multiple_sector(unsigned int sectorAddr, unsigned int numSector);

//! Performs the erase sequence for n physical sector in program or data flash
//! \param[in] sectorAddr the sector address
//! \param[in] numSector  the no.of sectors to be operated on
//! \return 0 on success, != 0 if invalid or not available sector has been selected
//! <B>Example:</B>
//! \code
//! // erase the first and second data flash
//! result |= clib_flash_erase_phys_sector(CLIB_FLASH_D0_START,2);
//! result |= clib_flash_erase_phys_sector(CLIB_FLASH_D1_START,3);
//! \endcode
extern int clib_flash_erase_phys_multiple_sector(unsigned int sector_addr,unsigned int numSector);

//! Performs the "Suspend Resume" sequence for multiple sectors
//! \param[in] sectorAddr the sector address
//! \param[in] numSector  the no.of sectors to be operated on
//! \return 0 on success, != 0 on errors
extern int clib_flash_suspend_resume_multiple(unsigned int sectorAddr,unsigned int numSector);

//! Performs the "Erase Verify" sequence for multiple sectors
//! \param[in] sectorAddr sector address which should be verified
//! \param[in] numSector  the no.of sectors to be operated on
//! \return 0 on success, != 0 on errors
extern int clib_flash_erase_verify_multiple(unsigned int sectorAddr,unsigned int numSector);

//! This function writes a 64 bit word via st.d
//!
//! \param[in] addr the address which should be written (should be 64bit aligned!)
//! \param[in] word_l 32bit variable which contains bit [31:0] of the write word
//! \param[in] word_u 32bit variable which contains bit [63:32] of the write word
//!
//! <B>Usage Example:</B>
//! \code
//!    clib_sys_st_d(0xc0000000, 0x12345678, 0x9abcdef0);
//!
//!    result |= clib_trace(MEM(0xc0000000), 0x12345678);
//!    result |= clib_trace(MEM(0xc0000004), 0x9abcdef0);
//! \endcode
extern void clib_sys_st_d(size_t addr, unsigned int word_l, unsigned int word_u);
extern int clib_flash_type(uint32 sector_addr);

//! this type contains start and end address of sectors
typedef struct clib_flash_sector_t {
  unsigned start;
  unsigned end;
} clib_flash_sector_t;

//! globally available table which defines all physical sectors of PFlash
//extern clib_flash_sector_t clib_flash_table_p_phys[CLIB_FLASH_P_PHYS_SECTORS];

//! globally available table which defines all logical sectors of PFlash
//extern clib_flash_sector_t clib_flash_table_p_log[CLIB_FLASH_P_LOG_SECTORS];

//! globally available table which defines all physical sectors of DFlash
//extern clib_flash_sector_t clib_flash_table_d_phys[CLIB_FLASH_D_PHYS_SECTORS];

//! globally available table which defines all logical sectors of DFlash
//extern clib_flash_sector_t clib_flash_table_d_log[CLIB_FLASH_D_LOG_SECTORS];


//! \}

#elif MCU_PLATFORM == _AURIX_TC39X_ //we need to port it to TC397 // we need to re-define below (or add more) for TC397, right now it's for build only

//! base address for general command sequences like "reset to read"
# define CLIB_FLASH_CMD_BASE					0xaf000000

//! number of flash modules, and offset between modules (only relevant if number > 1)
# define CLIB_FLASH_NUMBER_MODULES      		1 //! number of PMUs

# define CLIB_FLASH_P_OFFSET            		0x00800000 //! offset between PMU PFlash ranges
# define CLIB_FLASH_CMD_BASE_OFFSET     		0x00100000 //! offset between command areas (in DFlash range)

//! program flash
# define CLIB_FLASH_P_START             		0xa0000000
//# define TC26x_TC23x_CLIB_FLASH_P_BANK_OFFSET   0x00100000
//# define TC29x_TC27x_CLIB_FLASH_P_BANK_OFFSET   0x00200000
//# define TC24x_CLIB_FLASH_P_BANK_OFFSET         0x00080000
# define CLIB_FLASH_P_PAGE_LENGTH       		0x20
# define CLIB_FLASH_P_BURST_LENGTH      		0x100

//! data flash
//! Note: UCB and HSM addresses only available in clib_flash_table_d_log and clib_flash_table_phys_log\n
//! Separate defines are not available, as for the final TC27x these data sections will be part of a single sector anyhow!
# define CLIB_FLASH_D_START             		0xaf000000
# define CLIB_FLASH_D_BANKS             		1
# define CLIB_FLASH_D_BANK_OFFSET       		0x00100000
# define CLIB_FLASH_D_PAGE_LENGTH       		0x8
# define CLIB_FLASH_D_BURST_LENGTH      		0x20
# define CLIB_FLASH_D0_START            		(CLIB_FLASH_D_START+0*CLIB_FLASH_D_BANK_OFFSET)
# define CLIB_FLASH_D0_UCB_START                (CLIB_FLASH_D_START+ 1 * CLIB_FLASH_D_BANK_OFFSET)

# define CLIB_FLASH_D_UCB_SIZE                  0x00004000 		//16k

# define TC29x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC29x_CLIB_FLASH_D_SIZE - 1)
# define TC27x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC27x_CLIB_FLASH_D_SIZE - 1)
# define TC26x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC26x_CLIB_FLASH_D_SIZE - 1)
# define TC24x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC24x_CLIB_FLASH_D_SIZE - 1)
# define TC23x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC23x_CLIB_FLASH_D_SIZE - 1)
# define TC22x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC22x_CLIB_FLASH_D_SIZE - 1)
# define TC21x_CLIB_FLASH_D_END             	(CLIB_FLASH_D_START + TC21x_CLIB_FLASH_D_SIZE - 1)
# define CLIB_FLASH_D_UCB_END           	    (CLIB_FLASH_D0_UCB_START + CLIB_FLASH_D_UCB_SIZE - 1)

#endif //endof #if MCU_PLATFORM == _AURIX_TC29X_

#endif /* _CLIB_FLASH_H */




