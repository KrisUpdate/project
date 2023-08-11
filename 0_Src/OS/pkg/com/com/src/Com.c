#include "com/com/inc/ee_cominit.h"


/***************************************************************************
 *
 * COM application modes
 *
 **************************************************************************/
COMApplicationModeType EE_com_validmodes[EE_COM_N_MODE] = {EE_COM_MODE_A};

/* Flags */
EE_COM_DEFINE_FLAG(data_in);


/* Transmission */
EE_COM_DEFINE_INTERNAL_MESSAGE(senderTalkerMsg, 8U*sizeof(talker_msg_t), receiverTalkerMsg);
EE_COM_DEFINE_INTERNAL_MESSAGE(senderListenerMsg, 8U*sizeof(listener_msg_t), receiverListenerMsg);
EE_COM_DEFINE_INTERNAL_MESSAGE(senderUDPMsg, 8U*sizeof(udp_msg_t), receiverUDPMsg);

/* Receiving senderTalkerMsg */
EE_COM_DEFINE_INTERNAL_QUEUED_MESSAGE(receiverTalkerMsg, 8U*sizeof(talker_msg_t), F_Always, NULL,
                                      EE_COM_F_OK, &data_in, 10, EE_COM_NULL);


/* Receiving senderListenerMsg */
EE_COM_DEFINE_INTERNAL_QUEUED_MESSAGE(receiverListenerMsg, 8U*sizeof(listener_msg_t), F_Always, NULL,
                                      EE_COM_F_OK, &data_in, 3, EE_COM_NULL);


/* Receiving senderUDPMsg */
EE_COM_DEFINE_INTERNAL_QUEUED_MESSAGE(receiverUDPMsg, 8U*sizeof(udp_msg_t), F_Always, NULL,
                                      EE_COM_F_OK, &data_in, 30, EE_COM_NULL);


/* Messages (RAM) */
struct EE_com_msg_RAM_TYPE * EE_com_msg_RAM[EE_COM_N_MSG] = {
    EE_com_msg_RAM(senderTalkerMsg),
    EE_com_msg_RAM(receiverTalkerMsg),
    EE_com_msg_RAM(senderListenerMsg),
    EE_com_msg_RAM(receiverListenerMsg),
    EE_com_msg_RAM(senderUDPMsg),
    EE_com_msg_RAM(receiverUDPMsg)
};

/* Messages (ROM) */
const struct EE_com_msg_ROM_TYPE * EE_com_msg_ROM[EE_COM_N_MSG] = {
    EE_com_msg_ROM(senderTalkerMsg),
    EE_com_msg_ROM(receiverTalkerMsg),
    EE_com_msg_ROM(senderListenerMsg),
    EE_com_msg_ROM(receiverListenerMsg),
    EE_com_msg_ROM(senderUDPMsg),
    EE_com_msg_ROM(receiverUDPMsg)
};

/* Messages initial Value (ROM) */
const EE_UINT64 EE_com_msg_init_val[EE_COM_N_MSG] = {
    0,
    0,
    0,
    0,
    0,
    0
};

