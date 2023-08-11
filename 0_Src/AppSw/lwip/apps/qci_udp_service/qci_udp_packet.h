/*
 * qci_udp_packet.h
 *
 *  Created on: 2019¦~2¤ë13¤é
 *      Author: 95111301
 */

#ifndef _QCI_UDP_PACKET_H_
#define _QCI_UDP_PACKET_H_
#include "Ifx_Types.h"
#include "lwip/arch.h"
#include "lwip/udp.h"

#define QCI_UDP_SERVER_PORT 59478
#define QCI_UDP_TOTAL_HEADER_LEN	18
#define QCI_UDP_INFO_LEN	12
#define QCI_UDP_PAYLOAD_SIZE 256
#define QCI_UDP_CAN_PAYLOAD_SIZE 64
#define QCI_UDP_ERAY_PAYLOAD_SIZE 256
#define QCI_UDP_INFO_LEN_NO_PAYLOAD 10

#define AURIX1	0x01
#define AURIX2	0x02
#define XAVIER1	0x03
#define XAVIER2	0x04

typedef enum
{
	TARGET_GATEWAY = 0x0,
	TARGET_SECURITY_GATEWAY = 0x1,
	TARGET_HOST = 0x2
} _qci_target_mcu;

typedef enum
{
	toVIU = 0x14,
	toHOST = 0x28
} _qci_packet_direction;

typedef enum
{
  typeCAN = 0,
  typeLIN = 1,
  typeERAY = 2,
  typeTemperature = 0xA0,
  typeADC = 0xA1,
  typeVersion = 0xA2,
  typeEMC = 0xB1,
  typeFlashInfo = 0xF0,
  typeFlashData = 0xF1,
  typeWWD = 0xF2
} _qci_packet_bus_type;

typedef enum
{
  typeERAY_TESTMODE =      0x01,
  typeERAY_NORMALMODE = 0x10
} _qci_packet_eray_bus_id;

typedef enum
{
  tPFLASH = 0,
  tDFLASH = 1,
} _qci_flash_type;

typedef enum
{
  tUNKNOW = 0,
  tBANKA = 1,
  tBANKB = 2,
} _qci_swap_bank;

typedef enum // VIU Return Info Code to VCU(HOST)
{
    MCU_scheduled_successfully = 0x0000,
    MCU_queue_full = 0xD000,
    MCU_received_from_IP_not_in_channel_configuration = 0xE001,
    MCU_received_invalid_direction_code = 0xE002,
    MCU_received_invalid_VIU_ID = 0xE003,
    MCU_received_invalid_MCU_ID = 0xE004,
    MCU_received_invalid_bus_type = 0xE005,
    MCU_received_invalid_frame_type = 0xE006,
    MCU_received_invalid_return_code = 0xE007,
    MCU_received_invalid_BUS_ID = 0xE008,
    MCU_received_invalid_msg_ID = 0xE009,
    MCU_received_invalid_msg_length = 0xE00A,
    SOTA_COMMAND_RESULT_OK        = 0xF000,
    SOTA_COMMAND_RESULT_BANK_TYPE = 0xF001,
    SOTA_RESULT_ERR_UNKNOWN       = 0xFE01,
    SOTA_RESULT_ERR_CHECKSUM      = 0xFE02
} _qci_packet_info_code;

typedef struct
{
	_qci_packet_direction packet_direct ;
	uint8 target_viu ; //source
	uint8 target_mcu; //target
	//_qci_target_mcu target_mcu;
	_qci_packet_bus_type bus_type;
}qci_udp_packet_header;

PACK_STRUCT_BEGIN
struct qci_udp_packet
{
	PACK_STRUCT_FLD_8(qci_udp_packet_header header);
	PACK_STRUCT_FIELD(uint16 info_len);
	PACK_STRUCT_FIELD(uint16 info_code);
    union
    {
        struct
        {
            PACK_STRUCT_FLD_8(uint8  bus_id);
            PACK_STRUCT_FLD_8(uint8  rs);
            PACK_STRUCT_FIELD(uint16  rs2); //seqnum
            union
            {
                struct{
                    PACK_STRUCT_FIELD(uint32 canid); //message ID
                };
                struct{
                    PACK_STRUCT_FLD_8(uint8 sample); //message ID
                    PACK_STRUCT_FLD_8(uint8 frameid_h); //message ID
                    PACK_STRUCT_FLD_8(uint8 frameid_l); //message ID
                    PACK_STRUCT_FLD_8(uint8 cyclecnt); //message ID
                };
            };
            union{
                struct{
                    PACK_STRUCT_FIELD(uint16  candlc); //length
                };
                struct{
                    PACK_STRUCT_FIELD(uint16  eraylc); //length
                };
            };
            PACK_STRUCT_FLD_8(uint8  payload[QCI_UDP_PAYLOAD_SIZE]);
        };
        struct
        {
            PACK_STRUCT_FIELD(uint16 block_count);
            union
            {
                struct
                {
        	        PACK_STRUCT_FLD_8(uint8  flash_type);
	                PACK_STRUCT_FLD_8(uint8  flash_bank);
                };
                struct
                {
                    PACK_STRUCT_FIELD(uint16 block_index);
                };
            };
	        PACK_STRUCT_FIELD(uint32 phy_address);
            PACK_STRUCT_FIELD(uint16 check_sum);
	        PACK_STRUCT_FLD_8(uint8  flash_data[QCI_UDP_PAYLOAD_SIZE]);
        };
    };
}PACK_STRUCT_STRUCT;
PACK_STRUCT_END
void dump_qci_udp_packet(struct qci_udp_packet *packet);
void parse_qci_udp_packet(ip_addr_t *addr, struct qci_udp_packet *packet);
#endif /* _QCI_UDP_PACKET_H_ */
