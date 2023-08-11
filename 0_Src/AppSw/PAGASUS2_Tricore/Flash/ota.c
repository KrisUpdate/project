#include "Configuration.h"
#include "Platform_Types.h"
#include "Ifx_Types.h"
#include "IfxStdIf_DPipe.h"
#include "ota.h"
#include "ota_version.h"
#include "board.h"
#include "Clib_flash.h"
#include "resource.h"
static boolean is_ota_enabled = TRUE;
boolean is_ota_start = FALSE;
boolean isDoOTARST = FALSE;
/*******************************************************************************
**                      Update FW VERSION                                     **
*******************************************************************************/
#if ETHERNET_OTA_FW_TYPE == _OTA_UPDATE_FW

/*FW version section to inform linker to locate them at 0x80C7FFF8 */
#if defined(__GNUC__)
#pragma section
#pragma section ".upd_fw_version_data" a
#endif
#if defined(__TASKING__)
#pragma protect on
#pragma section farrom "upd_fw_version_data"
#endif
#if defined(__DCC__)
#pragma section CONST ".upd_fw_version_data" R
#endif
/** \brief FW VERSION DATA
 * FW version data at memory location 0x80C7FFF8.
 */
const uint8 g_upd_fw_version[] = {
    UPDATE_SW_MAJOR,    /* MAJOR NUMBER */
    UPDATE_SW_MINOR,	   /* MINOR NUMBER */
    UPDATE_SW_BUGFIX,  /* REVISION NUMBER */
    0x00,                          /* RESERVED 1 */
#ifdef TC397B
    AURIX_B_STEP,           /* Silicon Step version, indicate what step the binary is for */
#else
    AURIX_A_STEP,           /* Silicon Step version, indicate what step the binary is for */
#endif
    0x00,                          /* RESERVED 3 */
    0x00,                          /* RESERVED 4 */
    0x00                           /* RESERVED 5 */
};

/*reset the sections defined above */
#if defined(__GNUC__)
#pragma section
#endif
#if defined(__TASKING__)
#pragma protect restore
#pragma section farrom restore
#endif
#if defined(__DCC__)
#pragma section CONST
#endif

#else /* Production firmware */

#if PARTITION_LAYOUT == _QPARTITION
/* needs the data stored in flash location 0x80E00000 */
#define UPDATE_FW_VERSION_LOCATION      0xA0C7FFF8
#else
#define UPDATE_FW_VERSION_LOCATION      0xA0E00000
#endif
/* create a pointer to this address                   */
volatile uint8 *ptr_fw_ver = UPDATE_FW_VERSION_LOCATION;

/* IFW SW version : just define the variable */
uint8 g_upd_fw_version[8];

#endif //#if ETHERNET_OTA_FW_TYPE == _OTA_UPDATE_FW

/* Main FW version located at fixed address 0x80C7FFF0 */
#if defined(__GNUC__)
#pragma section
#pragma section ".fw_version_data"
#endif
#if defined(__TASKING__)
#pragma section farrom "fw_version_data"
#endif
#if defined(__DCC__)
#pragma section CONST ".fw_version_data" far-absolute R
#endif
#if ETHERNET_OTA_FW_TYPE == _OTA_PROD_FW
const uint8 g_fw_version[] = {
    SW_MAJOR,                     /* MAJOR NUMBER */
    SW_MINOR,                     /* MINOR NUMBER */
    SW_BUGFIX,                    /* REVISION NUMBER */
    0x00,                              /* RESERVED */
#ifdef TC397B
    AURIX_B_STEP,               /* Silicon Step version, indicate what step the binary is for  */
#else
    AURIX_A_STEP,               /* Silicon Step version, indicate what step the binary is for  */
#endif
    0x00,                              /* RESERVED */
    0x00,                              /* RESERVED */
    0x00                            /* RESERVED */
};
#else   /* UPDATE FW */
/* g_fw_version[] is not used in the update FW */
#endif

/*reset the sections defined above */
#if defined(__GNUC__)
#pragma section
#endif
#if defined(__TASKING__)
#pragma protect restore
#pragma section farrom restore
#endif
#if defined(__DCC__)
#pragma section CONST
#endif

BYTE HeaderBlock[512];
BYTE DataBlock[512];
BYTE Dflash_flag;

DWORD Mcu_PFlash_End_Address;
DWORD Mcu_DFlash_End_Address;
DWORD Mcu_Pflash_Size;
DWORD Mcu_Pflash_P0_End;
DWORD Mcu_Pflash_P1_End;
DWORD Mcu_Pflash_P2_End;
DWORD Mcu_Pflash_P3_End;
DWORD Mcu_Dflash_Size;
DWORD Mcu_Dflash_Logic_Size;

//BYTE Mcu_Type;
BYTE Mcu_Chip_Family;
BYTE Mcu_Chip_Rev;
BYTE Mcu_Emulation_Ext;
BYTE Mcu_Fsize;
BYTE Mcu_Sec;
BYTE Mcu_Chip_Id;
BYTE SectorAddressValid;

BYTE ota_state = 2;
BYTE data_mode = 0;;
BYTE read_chip_id = 0;

BYTE send_data[EB_MSG_PADDING + 1];
BYTE response_block[EB_MSG_PADDING + OTA_BLOCK_SIZE];

void SetOtaConfig(boolean v){
    is_ota_enabled = v;
    OTA_PRINTF_ALWAYS(("Set OTA Config enable/disable:: %d \r\n", is_ota_enabled));
}

boolean GetOtaConfig(void){
    OTA_PRINTF_ALWAYS(("Get OTA Config enable/disable:: %d \r\n", is_ota_enabled));
    return is_ota_enabled;
}

void Mask_ECC(boolean enable)
{
    uint16 password = IfxScuWdt_getCpuWatchdogPassword();

    IfxScuWdt_clearCpuEndinit(password);
    if(enable)
        CPU4_FLASHCON1.B.MASKUECC = 1u;
    else
        CPU4_FLASHCON1.B.MASKUECC = 0u;
    IfxScuWdt_setCpuEndinit(password);
}

void Get_Aurix_Version(char *pAurixVersion)
{
    char* pProject = AURIX_VERSION_PROJECT;
    while((*pProject) != '\0')
        *pAurixVersion++ = *pProject++;

    char *pBoard = AURIX_VERSION_BOARD_E3550 ;
    if(board.project == P3479){
        pBoard = AURIX_VERSION_BOARD_P3479;
    }
    if(board.project == P3570){
        pBoard = AURIX_VERSION_BOARD_P3570;
    }
    while((*pBoard) != '\0')
        *pAurixVersion++ = *pBoard++;

    char* pType = AURIX_VERSION_TYPE;
    while((*pType) != '\0')
        *pAurixVersion++ = *pType++;

#ifdef TC397B
    char* pChipRev = CHIPREV_STEP_B;
#else
    char* pChipRev = CHIPREV_STEP_A;
#endif
    while((*pChipRev) != '\0')
        *pAurixVersion++ = *pChipRev++;

    char* pSwVersion = CREATE_SW_VERSION(SW_MAJOR,SW_MINOR,SW_BUGFIX);
    while((*pSwVersion) != '\0')
        *pAurixVersion++ = *pSwVersion++;

    *pAurixVersion = '\0';
}

void send_response_block()
{
    UINT i;
    BYTE chksum = 0;
    uint8 status = 0;

    for(i = 1; i < (RESPONSE_BLOCK_SIZE - 1); i++)
    {
        chksum = chksum ^ response_block[EB_MSG_PADDING + i];
    }
    response_block[EB_MSG_PADDING + RESPONSE_BLOCK_SIZE - 1] = chksum;

    OTA_DEBUG_PRINTF(("OTA: sending response block, chksum = 0x%02x\n", chksum));

//#if ETHERNET_OTA_ENABLE == 1
    ota_udp_app_send((uint8*)&response_block, sizeof(response_block));
//#else
//	SpiTp_Transmit(SPITP_CHANNEL_OTA, (uint8*)&response_block, sizeof(response_block), &status);
//#endif
}

void SendByte(BYTE data)
{
    uint8 status = 0;
    send_data[EB_MSG_PADDING] = data;
    OTA_DEBUG_PRINTF(("OTA: sending byte 0x%02x\n", send_data[EB_MSG_PADDING]));

//#if ETHERNET_OTA_ENABLE == 1
    ota_udp_app_send((uint8*)&send_data, sizeof(send_data));
//#else
//	SpiTp_Transmit(SPITP_CHANNEL_OTA, (uint8*)&send_data, sizeof(send_data), &status);
//#endif
}

BYTE Sector_Start_Address_Check(DWORD Flash_Start_Address,DWORD dwSectorAddr,DWORD dwSize)
{
    if(dwSectorAddr < Flash_Start_Address+ 0x0020000)		//16k
    {
        if(!(dwSectorAddr & PFLASH_MASK1))
        {
            if(dwSize != 0x4000)
            {
                SendByte(BSL_SIZE_ERROR);
                return 0;
            }
            SectorAddressValid = 1;
        }
    }
    else if(dwSectorAddr< Flash_Start_Address + 0x00060000)	//32k
    {
        if(!(dwSectorAddr & PFLASH_MASK2))
        {
            if(dwSize != 0x8000)
            {
                SendByte(BSL_SIZE_ERROR);
                return 0;
            }
            SectorAddressValid = 1;
        }
    }
    else if(dwSectorAddr< Flash_Start_Address + 0x000A0000)	//64k
    {
        if(!(dwSectorAddr & PFLASH_MASK3))
        {
            if(dwSize != 0x10000)
            {
                SendByte(BSL_SIZE_ERROR);
                return 0;
            }
            SectorAddressValid = 1;
        }
    }
    else if(dwSectorAddr< Flash_Start_Address + 0x00100000)	//128k
    {
        if(!(dwSectorAddr & PFLASH_MASK4))
        {
            if(dwSize != 0x20000)
            {
                SendByte(BSL_SIZE_ERROR);
                return 0;
            }
            SectorAddressValid = 1;
        }
    }
    else													//256k
    {
        if(!(dwSectorAddr & PFLASH_MASK5))
        {
            if(dwSize != 0x40000)
            {
                SendByte(BSL_SIZE_ERROR);
                return 0;
             }
             SectorAddressValid = 1;
        }
    }
    return 1;
}

void EraseSector(DWORD dwSectorAddr, DWORD dwSize)
{
    OTA_DEBUG_PRINTF(("OTA: %s, sector addr: 0x%x, size: 0x%x, not implemented\n", __func__, dwSectorAddr, dwSize));
    OTA_FLASH_ErasePage(dwSectorAddr, dwSize);
    SendByte(BSL_SUCCESS);
}

void VerifyPage(DWORD PageAddr, _Bool PFlash)
{
    uint32 i,j=0;

    //check write pflash data correct
    for(i=0;i<CLIB_FLASH_P_BURST_LENGTH;i++){
        if(DataBlock[2+i]!=PFlashReadByte(PageAddr, j)){
            OTA_PRINTF_ALWAYS(("OTA Flashed and read pflash check result is fail. \n"));
            SendByte(BSL_VERIFICATION_ERROR);
            return 0;
        }
        j++;
        if(j>3){  PageAddr+=4; j=0;  }
    }
    OTA_DEBUG_PRINTF(("OTA verify page pass \n"));
    SendByte(BSL_SUCCESS);
}

_Bool ProgramFlashPage(DWORD dwPageAddr,_Bool PFlash)
{
    //OTA_PRINTF_ALWAYS(("OTA: %s, dwPageAddr: 0x%x\n", __func__, dwPageAddr));

    int uvResult;
    uint32 i;
    uint32 buffer[CLIB_FLASH_P_BURST_LENGTH/4];

    // check if it is a valid page(PFLASH)/burst page(DFLASH) start address,
    // PFLASH : program normal page(32 bytes),
    // DFLASH : program burst page(32 bytes)

    if(dwPageAddr & PFLASH_BURST_PAGE_MASK)
    {
        OTA_PRINTF_ALWAYS(("OTA: %s, BSL_ADDRESS_ERROR\n", __func__));
        SendByte(BSL_ADDRESS_ERROR);
        return 0;
    }
    if(PFlash == 1)
    {
        // load assembly buffer into 32bit buffer
        for(i=0;i<CLIB_FLASH_P_BURST_LENGTH;i+=4)
        {
            buffer[i/4] = (uint32)(DataBlock[2+i] & 0xFF);
            buffer[i/4] += ((uint32)(DataBlock[3+i] & 0xFF) << 8);
            buffer[i/4] += ((uint32)(DataBlock[4+i] & 0xFF) << 16);
            buffer[i/4] += ((uint32)(DataBlock[5+i] & 0xFF) << 24);
        }
        OTA_DEBUG_PRINTF(("OTA_FLASH_ProgramFromBuffer, dwPageAddr: 0x%x\n", dwPageAddr));
        OTA_FLASH_ProgramFromBuffer(dwPageAddr, buffer, CLIB_FLASH_P_BURST_LENGTH);

        //TODO: how to get the result?
        uvResult = 0;
        if(uvResult & FLASH_MASK_ERROR)
        {
            SendByte(BSL_PROGRAM_ERROR);
            return 0;
        }
        else if (uvResult & FLASH_PROTECTION_ERROR_MASK)
        {
            SendByte(BSL_PROTECTION_ERROR);
            return 0;
        }
        else if(uvResult & FLASH_PROGRAM_VERIFY_ERROR)
        {
             //clib_flash_clear_status(0);
             SendByte(BSL_PROGRAM_ERROR);
             return 0;
        }
    }
    else
    {
        OTA_PRINTF_ALWAYS(("OTA: %s, we don't support upgrade DFLASH now\n", __func__));
    }
    SendByte(BSL_SUCCESS);
    return 1;
}

void ProgramSPRAMPage(DWORD dwPageAddr)
{
    SendByte(BSL_SUCCESS);
    OTA_DEBUG_PRINTF(("OTA: %s, not implemented\n", __func__));
}

void RunUserCode(void)
{
    SendByte(BSL_SUCCESS);
#if 0
    __asm("movh.a a15,#0xa000\n"			//load flash
    	"\tlea a15,[a15]0x0020\n"			//base address
    	"\tji a15");						//au revoir!
#endif
}

void RunUserCodeSPRAM(void)
{
    SendByte(BSL_SUCCESS);
#if 0
	__asm("movh.a a15,#0xc000\n"			//load SPRAM
    	"\tlea a15,[a15]0x1800\n"			//base address
    	"\tji a15");						//au revoir!
#endif
}

#define POLYVAL 0xB8
//using polynomial  X^8 + X^4 + X^3 +X^2 + 1 */

void update_crc8(unsigned char new, unsigned char *crc)
{
    unsigned char c, i;
    c = *crc;
    for (i = 0; i < 8; i++) {
       if ((c ^ new) & 1) c = (c >> 1 ) ^ POLYVAL;
       else c >>= 1;
       new >>= 1;
    }
 *crc = c;
}

ubyte calc_crc8_core(ubyte *start,unsigned int data_counter)
{
    unsigned char mycrc=0,new;
    unsigned int i;
    unsigned char *start_pos = start;
    for(i=0;i<data_counter;i++)
    {
        new = *start_pos++;
        update_crc8(new, &mycrc);
    }
    return(mycrc);
}

_Bool WaitForDataBlock(uint8* pData, uint16 len)
{
    UINT i;
    BYTE chksum = 0;

    OTA_DEBUG_PRINTF(("OTA: Data received\r\n"));
    memcpy(DataBlock, pData, len);

    if (DataBlock[0] == EOT_BLOCK)
    {
        OTA_DEBUG_PRINTF(("OTA: Data EOT received\r\n"));
        //calculate CRC of data frame
        //chksum = calc_crc8_core(&DataBlock[1],HEADER_BLOCK_SIZE-2);
        for (i = 1; i < HEADER_BLOCK_SIZE-1; i++)
        {
            chksum = chksum ^ DataBlock[i];
        }
        //compare checksums
        if (chksum != DataBlock[HEADER_BLOCK_SIZE-1])
        {
            OTA_DEBUG_PRINTF(("OTA: error wrong checksum\r\n"));
            SendByte(BSL_CHKSUM_ERROR);
            DataBlock[0] = 0xFF; //make block type invalid
            return 0;
        }
        return 0;
    }
    else
    {
        if (DataBlock[0] != DATA_BLOCK)
        {
            OTA_DEBUG_PRINTF(("OTA: error its not data block\r\n"));
            SendByte(BSL_BLOCK_TYPE_ERROR);
            return 0;
        }
        //calculate CRC of data frame
        //chksum = calc_crc8_core(&DataBlock[1],PAGE_SIZE+6);
        for (i = 1; i < DATA_BLOCK_SIZE-1; i++)
        {
            chksum = chksum ^ DataBlock[i];
        }
        //compare the check sum of data frame with calculate checksum
        if (chksum != DataBlock[DATA_BLOCK_SIZE-1])
        {
            OTA_DEBUG_PRINTF(("OTA: error wrong checksum\r\n"));
            SendByte(BSL_CHKSUM_ERROR);
            return 0;
        }
        OTA_DEBUG_PRINTF(("OTA: Data received OK\r\n"));
        return 1;
    }
}

_Bool WaitForHeader(uint8* pData, uint16 len)
{
    UINT i;
    BYTE chksum = 0;

    OTA_DEBUG_PRINTF(("OTA: Header received\r\n"));
    memcpy(HeaderBlock, pData, len);

    if (HeaderBlock[0] != HEADER_BLOCK)
    {
        OTA_DEBUG_PRINTF(("OTA: error its not header block\r\n"));
        SendByte(BSL_BLOCK_TYPE_ERROR);
        return 0;
    }

    if ((HeaderBlock[1]!=BSL_PROGRAM_FLASH) &&
            (HeaderBlock[1]!=BSL_PROGRAM_SPRAM) &&
            (HeaderBlock[1]!=BSL_ERASE_FLASH) &&
            (HeaderBlock[1]!=BSL_RUN_FROM_FLASH) &&
            (HeaderBlock[1]!=BSL_RUN_FROM_SPRAM) &&
            (HeaderBlock[1]!=BSL_PROTECT_FLASH)  &&
            (HeaderBlock[1]!=BSL_BOOT_UPDATE_FW) &&
            (HeaderBlock[1]!=BSL_BOOT_NORMAL_FW) &&
            (HeaderBlock[1]!=BSL_GET_FW_VERSION) &&
            (HeaderBlock[1]!=BSL_END_OF_UPDATE))
    {
        OTA_DEBUG_PRINTF(("OTA: error its not correct mode\r\n"));
        SendByte(BSL_MODE_ERROR);
        return 0;
    }

    //calculate checksum of header frame
    //chksum = calc_crc8_core(&HeaderBlock[1],HEADER_BLOCK_SIZE-2);
    for (i = 1; i < HEADER_BLOCK_SIZE-1; i++)
    {
        chksum = chksum ^ HeaderBlock[i];
    }

    //compare checksum of header frame with the calculate checksum
    if (chksum != HeaderBlock[HEADER_BLOCK_SIZE-1])
    {
        OTA_DEBUG_PRINTF(("OTA: error wrong checksum\r\n"));
        SendByte(BSL_CHKSUM_ERROR);
        return 0;
    }
    OTA_DEBUG_PRINTF(("OTA: Header received OK\r\n"));
    return 1;
}

static void ota_dump_packet(uint8* pData, uint16 len)
{
    int i=0;

    OTA_DEBUG_PRINTF(("OTA : packet data : len = %d\r\n", len));
    for(i=0; i < len; i++)
    {
        OTA_DEBUG_PRINTF(("\t0x%02x\r\n", pData[i]));
    }
}

uint8 check_for_bmhd0(uint32 addr)
{
    uint32 start_addr;
    uint32 bmhd_index;
#if MCU_PLATFORM == _AURIX_TC29X_
    if(addr == CLIB_FLASH_P_START)
    {
        start_addr  = (DataBlock[2] & 0xFF);
        start_addr |= ((DataBlock[3] & 0xFF) << 8);
        start_addr |= ((DataBlock[4] & 0xFF) << 16);
        start_addr |= ((DataBlock[5] & 0xFF) << 24);
        bmhd_index  = (DataBlock[6] & 0xFF);
        bmhd_index |= ((DataBlock[7] & 0xFF) << 8);
        bmhd_index |= ((DataBlock[8] & 0xFF) << 16);
        bmhd_index |= ((DataBlock[9] & 0xFF) << 24);

        OTA_PRINTF_ALWAYS(("OTA: start_addr = 0x%08x\r\n", start_addr));
        OTA_PRINTF_ALWAYS(("OTA: bmhd_index = 0x%08x\r\n", bmhd_index));

        if( (start_addr == (CLIB_FLASH_P_START + 0x20)) &&
            (((bmhd_index >> 16) & 0xFFFF) == BMHD_MAGIC) &&
            ((bmhd_index & 0xFF) == BMHD_MODE)  )
            return 1;
        else
            return 0;
        }
    else
        return 1;
#elif MCU_PLATFORM == _AURIX_TC39X_
    return 1;
#endif
}

void ota_handle_state(uint8* pData, uint16 len)
{
    Dflash_flag = 0;
    DWORD addr, prev_addr;
    unsigned int sec;
    unsigned int i = 0;
    uint8 check_bmhd0 = 0;
    uint32 fw_version = 0;
    char aurix_ver[100];
    // OTA_DEBUG_PRINTF(("OTA: ota_handle_state len %d\r\n", len));
    DEBUG_PRINTF_ALWAYS(("OTA: ota_handle_state len %d\r\n", len));
    Get_Aurix_Version(aurix_ver);
    //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,aurix_ver);
    char *ptr_aurix_ver;

    if(len < EB_MSG_PADDING)
    {
        DEBUG_PRINTF_ALWAYS(("OTA: message byte less than %d\r\n", EB_MSG_PADDING));
        return;
    }

    //ota_dump_packet(pData, len);
    pData = pData + EB_MSG_PADDING;
    len = len - EB_MSG_PADDING;

    if(read_chip_id == 0)
    {
        OTA_DEBUG_PRINTF(("OTA: reading chip ids\r\n"));
        Read_CHIPID();
        read_chip_id = 1;
    }

    //in case of break in communication with tegra, reset ota state
    if(pData[0] == HEADER_BLOCK)
    {
        OTA_DEBUG_PRINTF(("OTA: resetting to wait_for_header state\r\n"));
        ota_state = OTA_STATE_WAIT_FOR_HEADER;
    }
    // OTA_DEBUG_PRINTF(("OTA: ota_state = %d, len = %d\r\n", ota_state, len));
    DEBUG_PRINTF_ALWAYS(("OTA: ota_state = %d, len = %d\r\n", ota_state, len));
    switch(ota_state)
    {
        // This state is not used currently
        case OTA_STATE_INIT:
            OTA_DEBUG_PRINTF(("[OTA: OTA_STATE_INIT] \r\n"));
            OTA_DEBUG_PRINTF(("OTA: sending initial ack byte\r\n"));
            //SendByte(0x55); // send ackn for header
            ota_state = OTA_STATE_WAIT_FOR_HEADER;
            break;

        case OTA_STATE_WAIT_FOR_HEADER:
            OTA_DEBUG_PRINTF(("[OTA: OTA_STATE_WAIT_FOR_HEADER] \r\n"));
            OTA_DEBUG_PRINTF(("OTA: getting the Header\r\n"));
            if (WaitForHeader(pData, len))
            {
                DWORD dwSize;
                DWORD dwAddress;
                dwAddress  = ((HeaderBlock[2] & 0xFF) << 24);
                dwAddress |= ((HeaderBlock[3] & 0xFF) << 16);
                dwAddress |= ((HeaderBlock[4] & 0xFF) << 8);
                dwAddress |= ( HeaderBlock[5] & 0xFF);
                switch (HeaderBlock[1])
                {
                    case BSL_PROGRAM_FLASH:
                        DEBUG_PRINTF_ALWAYS(("OTA: start flash\r\n"));
                        OTA_DEBUG_PRINTF(("[OTA: BSL_PROGRAM_FLASH] \r\n"));
                        OTA_DEBUG_PRINTF(("OTA: Flash program.\r\n"));
                        OTA_DEBUG_PRINTF(("OTA: Flash Addr is 0x%08x\r\n", dwAddress));
                        SendByte(BSL_SUCCESS); // send ackn for header
                        _Bool validRange=0;
                        _Bool PFlash=1;
#if MCU_PLATFORM == _AURIX_TC29X_
                         //check if user start address is in boundary
                         if ((dwAddress >= CLIB_FLASH_P_START) && (dwAddress < Mcu_PFlash_End_Address))
                         {
                             validRange=1;
                         }
                         else if ((dwAddress >= CLIB_FLASH_D_START) && (dwAddress < Mcu_DFlash_End_Address))
                         {
                             validRange=1;
                             PFlash = 0;
                             Dflash_flag = 1;
                         }
                         else if ((dwAddress >= CLIB_FLASH_D0_UCB_START) && (dwAddress < CLIB_FLASH_D_UCB_END))
                         {
                             validRange=1;
                             PFlash = 0;
                             Dflash_flag = 1;
                         }
#elif MCU_PLATFORM == _AURIX_TC39X_ //TODO: define the address here
                         validRange=1;
#endif
                         if (validRange) {
                             prev_addr = dwAddress;
                             data_mode = FLASH_MODE;
                             ota_state = OTA_STATE_WAIT_FOR_DATA;
                         }
                         else
                         {
                             SendByte(BSL_ADDRESS_ERROR);
                         }
                         DEBUG_PRINTF_ALWAYS(("OTA: flash finish\r\n"));
                         waitTime(1 * TimeConst[TIMER_INDEX_1MS]);
                         break;
                    case BSL_END_OF_UPDATE:
                        OTA_DEBUG_PRINTF(("[OTA: BSL_END_OF_UPDATE] \r\n"));
                        //DEBUG_PRINTF_ALWAYS(("OTA: end of fw update\n"));
                        SendByte(BSL_SUCCESS); // send ackn for header
                        break;
                    case BSL_GET_FW_VERSION:
                        OTA_DEBUG_PRINTF(("[OTA: BSL_GET_FW_VERSION] \r\n"));
                        DEBUG_PRINTF_ALWAYS(("OTA: get fw version %s\r\n",
                        HeaderBlock[2] == TYPE_EB ?"FW" : "UPDATE FW"));
                        response_block[EB_MSG_PADDING] = 0x03;
#if ETHERNET_OTA_FW_TYPE == _OTA_PROD_FW
                         /* we need to response both f/w version in prod F/W */
                        if(HeaderBlock[2] == TYPE_EB)
                        {
                            response_block[EB_MSG_PADDING+1] = g_fw_version[0];
                            response_block[EB_MSG_PADDING+2] = g_fw_version[1];
                            response_block[EB_MSG_PADDING+3] = g_fw_version[2];
                        }
                        else
                        {
                            Mask_ECC(TRUE);
                            g_upd_fw_version[0] = ptr_fw_ver[0];  // update the address from the pointer to g_upd_fw_version
                            g_upd_fw_version[1] = ptr_fw_ver[1];
                            g_upd_fw_version[2] = ptr_fw_ver[2];
                            g_upd_fw_version[3] = ptr_fw_ver[3];
                            g_upd_fw_version[4] = ptr_fw_ver[4];
                            g_upd_fw_version[5] = ptr_fw_ver[5];
                            g_upd_fw_version[6] = ptr_fw_ver[6];
                            g_upd_fw_version[7] = ptr_fw_ver[7];

                            response_block[EB_MSG_PADDING+1] = g_upd_fw_version[0];
                            response_block[EB_MSG_PADDING+2] = g_upd_fw_version[1];
                            response_block[EB_MSG_PADDING+3] = g_upd_fw_version[2];
                            Mask_ECC(FALSE);
                        }
                        response_block[EB_MSG_PADDING+5] = HeaderBlock[2];
#else
                        response_block[EB_MSG_PADDING+1] = g_upd_fw_version[0];
                        response_block[EB_MSG_PADDING+2] = g_upd_fw_version[1];
                        response_block[EB_MSG_PADDING+3] = g_upd_fw_version[2];
                        response_block[EB_MSG_PADDING+5] = TYPE_UPDATE;
#endif
                        response_block[EB_MSG_PADDING+4] = 0x0;

                        /*Sending back the chip revision value here*/
                        uint8 Mcu_Chip_Rev = SCU_CHIPID.B.CHREV;
                        response_block[EB_MSG_PADDING+6] = Mcu_Chip_Rev;

                        ptr_aurix_ver = aurix_ver;
                        for(i = HEADER_BLOCK_SIZE; i < OTA_BLOCK_SIZE ; i++)
                        {
                            response_block[i] = *ptr_aurix_ver++;
                            if (response_block[i] == '\0')
                                break;
                        }

                        send_response_block();
                        break;

                    case BSL_BOOT_UPDATE_FW:
                        OTA_DEBUG_PRINTF(("[OTA: BSL_BOOT_UPDATE_FW] \r\n"));
                        //DEBUG_PRINTF_ALWAYS(("is_OTA_config value is: %d\r\n", GetOtaConfig());
                        if(!GetOtaConfig())
                        {
                            DEBUG_PRINTF_ALWAYS(("OTA is disable, stop doing OTA. Run \"ota ?\" for detail \r\n"));
                            break;
                        }
                        DEBUG_PRINTF_ALWAYS(("OTA: Boot update fw\r\n"));
#if ETHERNET_OTA_FW_TYPE == _OTA_UPDATE_FW
                        SendByte(BSL_UPDATE_FW_ERROR);
#else  //ETHERNET_OTA_FW_TYPE == _OTA_PROD_FW
                        sec = HeaderBlock[2];

                        isDoOTARST=TRUE;
                        pflash_CORRUPT_from_ota();
                        SendByte(BSL_SUCCESS);

                        DEBUG_PRINTF_ALWAYS(("OTA: waiting for %d sec\r\n", sec));
                        waitTime(sec*1000*TimeConst[TIMER_INDEX_1MS]);

                        DEBUG_PRINTF_ALWAYS(("OTA: resetting Aurix \r\n"));
                        sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
                        ota_aurixrst();
                        sem_release(SEM_SPI_2);
#endif
                        break;

                    case BSL_BOOT_NORMAL_FW:
                        OTA_DEBUG_PRINTF(("[OTA: BSL_BOOT_NORMAL_FW] \r\n"));
                        DEBUG_PRINTF_ALWAYS(("OTA: Boot Prod fw\r\n"));
                        sec = HeaderBlock[2];
						
                        isDoOTARST=TRUE;
                        pflash_correct_from_ota();
                        SendByte(BSL_SUCCESS);

                        DEBUG_PRINTF_ALWAYS(("OTA: waiting for %d sec\r\n", sec));
                        waitTime(sec*1000*TimeConst[TIMER_INDEX_1MS]);

                        DEBUG_PRINTF_ALWAYS(("OTA: resetting Aurix"));
                        sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
                        ota_aurixrst();
                        sem_release(SEM_SPI_2);
                        break;

                    case BSL_PROGRAM_SPRAM:
                        OTA_DEBUG_PRINTF(("[OTA: BSL_PROGRAM_SPRAM] \r\n"));
                        DEBUG_PRINTF_ALWAYS(("OTA: BSL_PROGRAM_SPRAM\r\n"));
                        SendByte(BSL_SUCCESS); // send ackn for header
                        if ((dwAddress >= 0xC0001800) && (dwAddress < 0xC000A000))
                        {
                            prev_addr = dwAddress;
                            data_mode = SPRAM_MODE;
                            ota_state = OTA_STATE_WAIT_FOR_DATA;
                        }
                        else
                        {
                            SendByte(BSL_ADDRESS_ERROR);
                        }
                        break;

                    case BSL_ERASE_FLASH:
                        DEBUG_PRINTF_ALWAYS(("OTA: ERASE Start\r\n"));
                        is_ota_start = TRUE;
                        waitTime(1 * TimeConst[TIMER_INDEX_1MS]);
                        OTA_DEBUG_PRINTF(("[OTA: BSL_ERASE_FLASH] \r\n"));
                        DEBUG_PRINTF_ALWAYS(("OTA: Flash Erase\r\n"));
                        waitTime(1 * TimeConst[TIMER_INDEX_1MS]);
                        DEBUG_PRINTF_ALWAYS(("OTA: Flash Addr is 0x%08x\r\n", dwAddress));
                        waitTime(1 * TimeConst[TIMER_INDEX_1MS]);
                        // is_ota_start = TRUE;
                        ota_state = OTA_STATE_WAIT_FOR_HEADER;
                        //Read the sector size additionally to the sector address
                        //P33_OUT.B.P6 = 0;
                        dwSize  = ((HeaderBlock[6] & 0xFF) << 24);
                        dwSize |= ((HeaderBlock[7] & 0xFF) << 16);
                        dwSize |= ((HeaderBlock[8] & 0xFF) << 8);
                        dwSize |= ( HeaderBlock[9] & 0xFF);

                        DEBUG_PRINTF_ALWAYS(("OTA: erase size is 0x%08x\r\n", dwSize));
                        waitTime(1 * TimeConst[TIMER_INDEX_1MS]);
                        EraseSector(dwAddress, dwSize);
                        waitTime(1 * TimeConst[TIMER_INDEX_1MS]);
                        //SendByte(BSL_SUCCESS);
                        DEBUG_PRINTF_ALWAYS(("OTA: ERASE end\r\n"));
                        break;

                    case BSL_RUN_FROM_FLASH:
                        OTA_DEBUG_PRINTF(("[OTA: BSL_RUN_FROM_FLASH] \r\n"));
                        ota_state = OTA_STATE_WAIT_FOR_HEADER;
                        RunUserCode();
                        break;
                    case BSL_RUN_FROM_SPRAM:
                        OTA_DEBUG_PRINTF(("[OTA: BSL_RUN_FROM_SPRAM] \r\n"));
                        ota_state = OTA_STATE_WAIT_FOR_HEADER;
                        RunUserCodeSPRAM();
                        break;
                    case BSL_PROTECT_FLASH:
                        OTA_DEBUG_PRINTF(("[OTA: BSL_RUN_FROM_SPRAM] \r\n"));
                        ota_state = OTA_STATE_WAIT_FOR_HEADER;
                        break;
                    default:
                        break;
                 }
            }
           break;

       case OTA_STATE_WAIT_FOR_DATA:
            // OTA_DEBUG_PRINTF(("[OTA: OTA_STATE_WAIT_FOR_DATA] \r\n"));
            DEBUG_PRINTF_ALWAYS(("[OTA: OTA_STATE_WAIT_FOR_DATA] \r\n"));
            //DWORD addr;
            //_Bool PFlash = 1;
            if(data_mode == FLASH_MODE)
            {
                if (WaitForDataBlock(pData, len))
                {
                    addr  = ((DataBlock[DATA_BLOCK_SIZE-6] & 0xFF) << 24);
                    addr |= ((DataBlock[DATA_BLOCK_SIZE-5] & 0xFF) << 16);
                    addr |= ((DataBlock[DATA_BLOCK_SIZE-4] & 0xFF) << 8);
                    addr |= ( DataBlock[DATA_BLOCK_SIZE-3] & 0xFF);

                    OTA_DEBUG_PRINTF(("OTA: Program Flash Addr is 0x%08x\r\n", addr));
                    check_bmhd0 = check_for_bmhd0(addr);
                    if(check_bmhd0)
                    {
                        DEBUG_PRINTF_ALWAYS(("OTA: ProgramFlashPage\r\n"));
                        ProgramFlashPage(addr,1);
                        // DEBUG_PRINTF_ALWAYS(("OTA: VerifyPage\r\n"));
                        // VerifyPage(addr,1);
                        DEBUG_PRINTF_ALWAYS(("OTA: Program finish\r\n"));
                        //SendByte(BSL_SUCCESS);
                    }
                    else
                    {
                        OTA_PRINTF_ALWAYS(("OTA: Trying to program wrong BMHD \r\n"));
                        SendByte(BSL_PROGRAM_ERROR);
                    }
                }
                if (DataBlock[0] == EOT_BLOCK)
                {
                    SendByte(BSL_SUCCESS);
                    ota_state = OTA_STATE_WAIT_FOR_HEADER;
                }
            }
            if(data_mode == SPRAM_MODE)
            {
                if (WaitForDataBlock(pData, len))
                {
                    ProgramSPRAMPage(prev_addr);
                    prev_addr += PAGE_SIZE;
                }
                if (DataBlock[0] == EOT_BLOCK)
                {
                    SendByte(BSL_SUCCESS);
                    ota_state = OTA_STATE_WAIT_FOR_HEADER;
                }
            }
    }
}

BYTE Read_CHIPID(void)
{

}
