/*
 * message_type.h
 *
 *  Created on: 2019¦~6¤ë12¤é
 *      Author: Alex.Cheng
 */

#ifndef CONFIG_INC_MESSAGE_TYPE_H_
#define CONFIG_INC_MESSAGE_TYPE_H_

#ifndef AVB_MAX_CHANNELS_PER_TALKER_STREAM
#define AVB_MAX_CHANNELS_PER_TALKER_STREAM 8
#endif

#ifndef AVB_MAX_CHANNELS_PER_LISTENER_STREAM
#define AVB_MAX_CHANNELS_PER_LISTENER_STREAM 8
#endif

typedef enum
{
  TALKER_PTP_EVENT,      ///TODO: Alex handle c_ptp
  TALKER_CONTROL_PACKET_RECV_EVENT,
  TALKER_PERIODIC_EVENT,
  TALKER_CAN_PACKET_RECV_EVENT,
  TALKER_LIN_PACKET_RECV_EVENT,
  TALKER_FR_PACKET_RECV_EVENT,
  TALKER_RVF_PACKET_RECV_EVENT,
  TALKER_CVF_PACKET_RECV_EVENT,
} talker_event_t;

/// control interface used by 1722.1 protocol
typedef enum
{
  TALKER_CTL_CMD_AVB1722_CONFIGURE_TALKER_STREAM,
  TALKER_CTL_CMD_AVB1722_DISABLE_TALKER_STREAM,
  TALKER_CTL_CMD_AVB1722_TALKER_GO,
  TALKER_CTL_CMD_AVB1722_TALKER_STOP,
  TALKER_CTL_CMD_AVB1722_SET_PORT,
  TALKER_CTL_CMD_AVB1722_SET_VLAN,
} talker_ctl_cmd_t;

/// FIXME:Alex Design consideration,Check if command data is too long for message passing
typedef struct{
    char stream_num;  /// 1722 stream reservation local ID
    char format;  /// stream format
    unsigned char dest_mac[6]; /// Destination Mac Address
    int source_num; /// Source number
    char num_of_channels; /// stream # of channels
    int fifo_mask; /// FIFO Mask
    int maps[AVB_MAX_CHANNELS_PER_TALKER_STREAM +1]; ///AVB_NUM_MEDIA_OUTPUTS is defined in avb_conf.h, is there a better way to
    char rate; /// rate
    int presentation; /// presentation time
    int src_port; ///source port
    unsigned char vlan; /// Vlan ID
}talker_ctl_cmd_data_t;

typedef struct
{
  unsigned char len;
  unsigned char acf_msg[80];//TBD: Carol
} AVB_Packet_t;

typedef struct {
    talker_event_t event_type;
    talker_ctl_cmd_t cmd; ///talker control command, control interface which is used by 1722.1 protocol stack
    talker_ctl_cmd_data_t cmddata; /// talker command data
    AVB_Packet_t avb_packet;
    //uint16_t len; //bytes of payload
} talker_msg_t;

typedef struct
{
    union {
        unsigned char data[1500];
        struct {
            unsigned char p1722_hdr[24];
            unsigned char acf_payload[1476];
        };
    } pdu;
    unsigned short len;
} P1722_PACKET_t;

typedef enum
{
  LISTENER_BUFFER_IO_EVENT, ///TODO:Alex handle media buffer IO event
  LISTEER_PTP_EVENT,      ///TODO: Alex handle c_ptp
  LISTENER_CONTROL_PACKET_RECV_EVENT,
  LISTENER_PERIODIC_EVENT,
} listener_event_t;

/// control interface used by 1722.1 protocol
typedef enum
{
  LISTENER_CTL_CMD_AVB1722_CONFIGURE_LISTENER_STREAM,
  LISTENER_CTL_CMD_AVB1722_ADJUST_LISTENER_STREAM,
  LISTENER_CTL_CMD_AVB1722_DISABLE_LISTENER_STREAM,
  LISTENER_CTL_CMD_AVB1722_AVB1722_GET_ROUTER_LINK,   ///TODO: Alex this event has return value, shall we exported it as a function
} listener_ctl_cmd_t;

typedef struct{
    int stream_num;  /// 1722 stream local ID
    int media_clk;
    int rate;
    int num_of_channels; /// stream # of channels
    int maps[AVB_MAX_CHANNELS_PER_LISTENER_STREAM +1]; ///AVB_NUM_MEDIA_OUTPUTS is defined in avb_conf.h, is there a better way to
}listener_ctl_cmd_data_t;

typedef struct {
    listener_event_t event_type;
    listener_ctl_cmd_t cmd; /// listener control command, control interface which is used by 1722.1 protocol stack
    listener_ctl_cmd_data_t cmddata; /// control command data
    //uint16_t len; //bytes of payload
}listener_msg_t;

typedef struct {
    unsigned char udp_content[82];
}udp_msg_t;

#endif /* 0_SRC_OS_CONFIG_INC_MESSAGE_TYPE_H_ */
