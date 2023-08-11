/*
 * board2board.h
 *
 *  Created on: May 1, 2019
 *      Author: bradf
 */

#ifndef _BOARD2BOARD_BOARD2BOARD_H_
#define _BOARD2BOARD_BOARD2BOARD_H_

#include "Configuration.h"

typedef enum{
    BRD_ID_NV_3570         = 0,
    BRD_ID_X86_V3DA       = 1,
    BRD_ID_GPU_BOX        = 3,
    BRD_ID_X86_V3NC       = 4
}BRD_ID_t;

enum{
    B2B_POLLING_EVENT_0         = 0x00,
    B2B_POLLING_EVENT_1         = 0x01,
    B2B_POLLING_EVENT_2         = 0x02,
    B2B_POLLING_EVENT_3         =0x03,
    B2B_POLLING_CMD_BMCRUNTIME = 0x10,
    B2B_STATE_ALIVE_FAIL               = 0x30
};

enum{
    B2B_ALIVE_CMD_CLEAR          = 0x00,
    B2B_ALIVE_CMD_ADD             = 0x01
};

typedef enum{
    B2B_POLLING_CMD_T0                              = 0x00,
    B2B_POLLING_CMD_T1                              = 0x01,
    B2B_POLLING_CMD_T2                              = 0x02,
    B2B_POLLING_CMD_T3                              = 0x03,
    B2B_POLLING_CMD_T4                              = 0x04,
    B2B_POLLING_CMD_T5                              = 0x05,
    B2B_POLLING_CMD_T6                              = 0x06,
    B2B_POLLING_CMD_T7                              = 0x07,
    B2B_POLLING_CMD_GET_PWRSTATE           = 0x08,
    B2B_POLLING_CMD_SWITCH2BMC              = 0x09,
    B2B_POLLING_CMD_BMC_GET_PWRSTATE   = 0x0A,
    B2B_POLLING_CMD_BMC_PWROFF             = 0x0B,
    B2B_POLLING_CMD_BMC_PWRON               = 0x0C,
    B2B_POLLING_CMD_BMC_PWRRESET          = 0x0D,
    B2B_POLLING_CMD_BMC_PWRCYCLE          = 0x0E,
    B2B_POLLING_CMD_GET_CURRENTPATH       = 0x0F,
    B2B_POLLING_CMD_SWITCH2CPLD              = 0x10,
    B2B_POLLING_CMD_BMC_PWRSOFT             = 0x11,
    B2B_POLLING_CMD_BMC_POSTCODE            = 0x12,
    B2B_CMD_NUM                                          =0x13
}B2B_SEND_CMD_t;

#define B2B_UART_VERIFY_LENGTH                 5
#define B2B_UART_SEND_LENGTH_V3NC          7
#define B2B_UART_VERIFY_LENGTH_V3NC       16
//#define B2B_POLLING_CMD                             6

#define B2B_V3NC_RESPONSE                 2
#define B2B_V3NC_LENGTH_ONE_BYTE    1

#define B2B_V3NC_CMD_TEMPERATURE              1
#define B2B_V3NC_CMD_GET_POWSTATE           5
#define B2B_V3NC_CMD_GET_CPLDPOWSTATE           1
#define B2B_V3NC_CMD_GET_BMCPOWSTATE            2
#define B2B_V3NC_CMD_PATH                   6
#define B2B_V3NC_CMD_GET_DEBUG               8
#define B2B_V3NC_CMD_GET_POSTCODE               1

#define B2B_V3NC_CMD_1ST_TEMPERATURE      1
#define B2B_V3NC_CMD_4TH_TEMPERATURE      4
#define B2B_V3NC_CMD_8TH_TEMPERATURE      8

#define B2B_V3NC_CMD_GET_PATH                   0
#define B2B_V3NC_CMD_SWITCHTOBMC                             1
#define B2B_V3NC_CMD_SWITCHTOCPLD                             2

#define B2B_WAIT_V3NC_BOOT_TIME                60

#define CURRENT_PATH_CPLD                         1
#define CURRENT_PATH_BMC                          2

boolean B2b_run(pchar args, void *data, IfxStdIf_DPipe *io);
boolean B2b_cmd(pchar args, void *data, IfxStdIf_DPipe *io);

boolean b2b_is_board_dectected(void);

uint32 b2b_get_board_id(void);

boolean b2b_get_board_powergood_status(void);

boolean b2b_cmd_nc(pchar args, void *data, IfxStdIf_DPipe *io);
boolean b2b_v3nc_monitor_temperature(void);

#endif /* 0_APPSW_TRICORE_BOARD2BOARD_BOARD2BOARD_H_ */
