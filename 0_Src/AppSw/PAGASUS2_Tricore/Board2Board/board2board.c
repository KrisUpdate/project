/*
 * board2board.c
 *
 *  Created on: May 1, 2019
 *      Author: bradf
 */
#include "configuration.h"
#include "bsp.h"
#include "board2board.h"
#include "EvadcAutoScan.h"
#include "gpioexp.h"
#include "gpio.h"
#include "ethernet.h"
#include "resource.h"
#include <internal_fw_debug.h>
#include "AppAscUart.h"
#include "Platform_Types.h"
#if 1
#define B2B_ALWAYS_PRINTF(x) DEBUG_PRINTF_ALWAYS(x)
#else
#define B2B_ALWAYS_PRINTF(x) 
#endif
static boolean b2b_prst_det1, b2b_prst_det2;
static uint32 b2b_brd_id = 0;
boolean is_b2b_x86_v3nc = FALSE;
boolean is_b2b_pg = FALSE;
boolean g_BMCTempEnable = FALSE;
static sint8 b2b_v3nc_temperature[8]={0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80};
static uint8 b2b_polling_state=B2B_POLLING_EVENT_0;
static uint8 onetime_cmd;
//tern boolean Check_PGoodSignals(uint8 count, ...)
extern boolean IfxEth_B2BPhy_88q2112_init(void);
boolean g_isB2B_power_on=FALSE;
uint8 g_CurrentPath = CURRENT_PATH_CPLD;
uint8 g_CurrentBMCPowerstate = FALSE;
boolean g_isSendCmdByShell = FALSE;
static uint8 g_rev_cmd = 0xFF;
boolean is_b2b_uart_debug = FALSE;
//boolean is_b2b_uart_send = FALSE;
boolean B2B_alive(uint8 f){
    static uint8 alive_cnt=0;
    boolean rtn=FALSE;
    switch (f){
         case B2B_ALIVE_CMD_CLEAR:
                alive_cnt=0;
                rtn=TRUE;
            break;
         case B2B_ALIVE_CMD_ADD:
                alive_cnt++;
                rtn=TRUE;
                //over 30s, can't rev v3nc response
                if(alive_cnt>30) rtn=FALSE;
            break;
    }
    return rtn;
}

boolean b2b_v3nc_monitor_temperature(void){
    boolean cur_state=FALSE;
    if(!g_isB2B_power_on){
        if(b2b_v3nc_temperature[0]>=5&&b2b_v3nc_temperature[1]>=5
            &&b2b_v3nc_temperature[2]>=5&&b2b_v3nc_temperature[3]>=5){
            //show P3570_B2B_TX_SYS_RST_N_EXP
            //B2B_ALWAYS_PRINTF(("Show RST_N Port:\r\n"));
            //AsclinShellInterface_execute_cmd("showportexp", "2 0 7");
            B2B_ALWAYS_PRINTF(("Get B2B temperature over 5 degC and wait power enable \r\n"));
            while(!cur_state){
                SET_GPIOEXP_OUTPUT_LOW(P3570_B2B_TX_PWR_EN_EXP);
                AsclinShellInterface_execute_cmd("showportexp", "2 0 3");
                waitTime(3*TimeConst[TIMER_INDEX_1S]);
                SET_GPIOEXP_OUTPUT_HIGH(P3570_B2B_TX_PWR_EN_EXP);
                AsclinShellInterface_execute_cmd("showportexp", "2 0 3");
                waitTime(3*TimeConst[TIMER_INDEX_1S]);
                cur_state=READ_GPIOEXP_STATE(P3570_B2B_TX_PWR_EN_EXP);
            }
            waitTime(1*TimeConst[TIMER_INDEX_1S]);
            SET_GPIOEXP_OUTPUT_LOW(P3570_B2B_TX_PWR_EN_EXP);
            AsclinShellInterface_execute_cmd("showportexp", "2 0 3");

            g_isB2B_power_on=TRUE;
            b2b_polling_state=B2B_POLLING_EVENT_1;
        }else{
            B2B_ALWAYS_PRINTF(("Get B2B temperature \r\n"));
        }
        
    }
#if 0
    if(g_CurrentPath==CURRENT_PATH_CPLD){
        for (int i=0; i<B2B_V3NC_CMD_4TH_TEMPERATURE;i++)
            B2B_ALWAYS_PRINTF(("T[%d]:%d\r\n",i,b2b_v3nc_temperature[i]));
    }else if(g_CurrentPath==CURRENT_PATH_BMC){
        for (int i=0; i<B2B_V3NC_CMD_8TH_TEMPERATURE;i++)
            B2B_ALWAYS_PRINTF(("T[%d]:%d\r\n",i,b2b_v3nc_temperature[i]));
    }
#endif
    return TRUE;
}

static uint8 tx_buffer[B2B_CMD_NUM][B2B_UART_SEND_LENGTH_V3NC]={
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x01,0x01,0x04,UART_PACKET_HEADER2_V3NC},//Get T0
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x01,0x02,0x05,UART_PACKET_HEADER2_V3NC},//Get T1
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x01,0x03,0x06,UART_PACKET_HEADER2_V3NC},//Get T2
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x01,0x04,0x07,UART_PACKET_HEADER2_V3NC},//Get T3
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x01,0x05,0x08,UART_PACKET_HEADER2_V3NC},//Get T4
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x01,0x06,0x09,UART_PACKET_HEADER2_V3NC},//Get T5
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x01,0x07,0x0A,UART_PACKET_HEADER2_V3NC},//Get T6
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x01,0x08,0x0B,UART_PACKET_HEADER2_V3NC},//Get T7
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x05,0x01,0x08,UART_PACKET_HEADER2_V3NC},//Get Power Status
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x06,0x01,0x09,UART_PACKET_HEADER2_V3NC}, //Switch to BMC
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x05,0x02,0x09,UART_PACKET_HEADER2_V3NC},//BMC Get Power Status
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x05,0x03,0x0A,UART_PACKET_HEADER2_V3NC},//BMC power off
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x05,0x04,0x0B,UART_PACKET_HEADER2_V3NC},//BMC power on
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x05,0x05,0x0C,UART_PACKET_HEADER2_V3NC},//BMC power reset
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x05,0x06,0x0D,UART_PACKET_HEADER2_V3NC},//BMC power cycle
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x06,0x00,0x08,UART_PACKET_HEADER2_V3NC},//Get Cucrrent Path
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x06,0x02,0x0A,UART_PACKET_HEADER2_V3NC},//Switch to CPLD
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x05,0x07,0x0E,UART_PACKET_HEADER2_V3NC},//BMC power soft
            {UART_PACKET_HEADER1_V3NC,0x01,0x01,0x08,0x01,0x0B,UART_PACKET_HEADER2_V3NC} //BMC post code
        };

boolean B2B_send_uart_cmd(B2B_SEND_CMD_t cmd){

    //waitTime(5*TimeConst[TIMER_INDEX_1MS]);
    UartApp_Init(UART_BAUD_RATE_19200);
    //if(is_b2b_uart_send){
        //B2B_ALWAYS_PRINTF(("B2B V3NC have no responce. \r\n"));
        //is_b2b_uart_send = FALSE;
    //}else{
        //is_b2b_uart_send = TRUE;
    //}
    for(int i=0;i<B2B_UART_SEND_LENGTH_V3NC;i++){
        UartApp_BlockSend(tx_buffer[cmd][i]);
    }
}

boolean b2b_v3nc_polling_cmd(void){
    static B2B_SEND_CMD_t polling_cmd=B2B_POLLING_CMD_T0;
    static uint8 B2B_Wait_Cnt = 0;

    if(is_b2b_pg){
        switch (b2b_polling_state){
            case B2B_POLLING_EVENT_0:
                //default value 20
                B2B_send_uart_cmd(polling_cmd);
                polling_cmd=polling_cmd+1;
                if(polling_cmd>B2B_POLLING_CMD_T3)    polling_cmd=B2B_POLLING_CMD_T0;
                break;
            case B2B_POLLING_EVENT_1:
                polling_cmd=B2B_POLLING_CMD_GET_PWRSTATE;
                B2B_Wait_Cnt = B2B_Wait_Cnt+1;
                if(B2B_Wait_Cnt>B2B_WAIT_V3NC_BOOT_TIME){
                    B2B_send_uart_cmd(polling_cmd);
                    B2B_Wait_Cnt=0;
                }
                break;
            case B2B_POLLING_EVENT_2:
                polling_cmd=B2B_POLLING_CMD_SWITCH2BMC;
                B2B_send_uart_cmd(polling_cmd);
                break;
            case B2B_POLLING_EVENT_3:
                polling_cmd=B2B_POLLING_CMD_GET_CURRENTPATH;
                B2B_send_uart_cmd(polling_cmd);
                break;
            case B2B_POLLING_CMD_BMCRUNTIME:
                if(g_isSendCmdByShell){
                    B2B_send_uart_cmd(onetime_cmd);
                    g_isSendCmdByShell=FALSE;
                }else{
                    //if(!B2B_alive(B2B_ALIVE_CMD_ADD)){
                    //    b2b_polling_state=B2B_STATE_ALIVE_FAIL;
                    //    break;
                    //}
                    if(g_BMCTempEnable){
                        B2B_send_uart_cmd(polling_cmd);
                        polling_cmd=polling_cmd+1;
                        if(polling_cmd>B2B_POLLING_CMD_T7)    polling_cmd=B2B_POLLING_CMD_T0;
                    }
                }
                break;
            case B2B_STATE_ALIVE_FAIL:
                B2B_ALWAYS_PRINTF(("B2B V3NC can't responce. \r\n"));
                break;
        }
    }else{
        //B2B_ALWAYS_PRINTF(("B2B Power Good is not ready.\r\n"));
    }
    
    return TRUE;
};

boolean b2b_decode_uart_v3nc(uint8 *puart_rx_buffer){
    uint8 n=0;
    //only support return one and two byte
    if(puart_rx_buffer[0]==UART_PACKET_HEADER1_V3NC&&puart_rx_buffer[7]==UART_PACKET_HEADER2_V3NC \
       ||puart_rx_buffer[0]==UART_PACKET_HEADER1_V3NC&&puart_rx_buffer[8]==UART_PACKET_HEADER2_V3NC){

        if(puart_rx_buffer[1]==B2B_V3NC_RESPONSE)
        {
            if(puart_rx_buffer[3]==B2B_V3NC_CMD_TEMPERATURE&&puart_rx_buffer[4]>=B2B_V3NC_CMD_1ST_TEMPERATURE&&puart_rx_buffer[4]<=B2B_V3NC_CMD_8TH_TEMPERATURE){
                if(puart_rx_buffer[5]<0x7F){
                    n=puart_rx_buffer[4]-1;
                    b2b_v3nc_temperature[n]=puart_rx_buffer[5];
                    //B2B_ALWAYS_PRINTF(("get No %d temperature is %d degree\r\n",puart_rx_buffer[3],b2b_v3nc_temperature[n]));
                }else{
                    b2b_v3nc_temperature[n]=(0xFF-puart_rx_buffer[5])*(-1);
                    //B2B_ALWAYS_PRINTF(("get No %d temperature is %d degree\r\n",puart_rx_buffer[3],b2b_v3nc_temperature[n]));
                }
                b2b_v3nc_monitor_temperature();
                if(b2b_polling_state==B2B_POLLING_CMD_BMCRUNTIME){
                     B2B_alive(B2B_ALIVE_CMD_CLEAR);
                }
            }else if(puart_rx_buffer[3]==B2B_V3NC_CMD_GET_POWSTATE&&puart_rx_buffer[4]==B2B_V3NC_CMD_GET_CPLDPOWSTATE){
                B2B_ALWAYS_PRINTF(("B2B Return Power Status is %d \r\n",puart_rx_buffer[5]));
                //Power Statue must be 1
                if(puart_rx_buffer[5]==1){
                    b2b_polling_state=B2B_POLLING_EVENT_2;
                }else if(puart_rx_buffer[5]==0){
                    b2b_polling_state=B2B_POLLING_EVENT_0;
                    g_isB2B_power_on = FALSE;
                }
            }else if(puart_rx_buffer[3]==B2B_V3NC_CMD_PATH&&puart_rx_buffer[4]==B2B_V3NC_CMD_SWITCHTOBMC){
                B2B_ALWAYS_PRINTF(("B2B Return Switch To BMC is %d \r\n",puart_rx_buffer[5]));
                //Switch Statue must be 1, 1 indecate success
                if(puart_rx_buffer[5]==1){
                    b2b_polling_state=B2B_POLLING_CMD_BMCRUNTIME;
                    g_CurrentPath = CURRENT_PATH_BMC;
                }
            }else if(puart_rx_buffer[3]==B2B_V3NC_CMD_PATH&&puart_rx_buffer[4]==B2B_V3NC_CMD_SWITCHTOCPLD){
                B2B_ALWAYS_PRINTF(("B2B Return Switch To CPLD is %d \r\n",puart_rx_buffer[5]));
                //Switch Statue must be 1, 1 indecate success
                if(puart_rx_buffer[5]==1){
                    g_CurrentPath = CURRENT_PATH_CPLD;
                }
            }else if(puart_rx_buffer[3]==B2B_V3NC_CMD_PATH&&puart_rx_buffer[4]==B2B_V3NC_CMD_GET_PATH){
                //Get Current PATH: 2 is BMC
                if(puart_rx_buffer[5]==2){
                    b2b_polling_state=B2B_POLLING_CMD_BMCRUNTIME;
                    g_CurrentPath = CURRENT_PATH_BMC;
                    B2B_ALWAYS_PRINTF(("B2B Return Currnet Path is BMC \r\n"));
                }else if(puart_rx_buffer[5]==1){
                    //b2b_polling_state=B2B_POLLING_CMD_BMCRUNTIME;
                    g_CurrentPath = CURRENT_PATH_CPLD;
                    B2B_ALWAYS_PRINTF(("B2B Return Currnet Path is CPLD \r\n"));
                }else{
                    B2B_ALWAYS_PRINTF(("B2B Return Currnet Path isn't avalible. \r\n"));
                }
            }else if(puart_rx_buffer[3]==B2B_V3NC_CMD_GET_POWSTATE&&puart_rx_buffer[4]==B2B_V3NC_CMD_GET_BMCPOWSTATE){
                g_CurrentBMCPowerstate=puart_rx_buffer[5];
                B2B_ALWAYS_PRINTF(("B2B Return BMC Power State is %d \r\n",puart_rx_buffer[5]));
            }else if(puart_rx_buffer[3]==B2B_V3NC_CMD_GET_DEBUG&&puart_rx_buffer[4]==B2B_V3NC_CMD_GET_POSTCODE){
                //return postcode is two byte
                B2B_ALWAYS_PRINTF(("B2B Return Postcode is %d %d\r\n",puart_rx_buffer[5],puart_rx_buffer[6]));
            }
            for(int i=0; i<B2B_CMD_NUM;i++){
                if(puart_rx_buffer[3]==tx_buffer[i][3]&&puart_rx_buffer[4]==tx_buffer[i][4])
                    g_rev_cmd = i;
            }
        }else{
            B2B_ALWAYS_PRINTF(("b2b uart receive packet over spec\r\n"));
        }
        waitTime(5*TimeConst[TIMER_INDEX_1MS]);
        ClearB2BTest();
        UartApp_Init(UART_BAUD_RATE_19200);
    }
    return TRUE;
}

boolean b2b_default_config(uint32 id){

        switch(id)
        {
            case BRD_ID_X86_V3NC:
                SET_GPIOEXP_OUTPUT_HIGH(P3570_B2B_GPO_1_EXP);
                SET_GPIOEXP_CONFIG_OUTPUT(P3570_B2B_GPO_1_EXP);
                SET_GPIOEXP_OUTPUT_HIGH(P3570_B2B_GPO_2_EXP);
                SET_GPIOEXP_CONFIG_OUTPUT(P3570_B2B_GPO_2_EXP);
                SET_GPIOEXP_CONFIG_INPUT(P3570_B2B_RX_SYS_RST_N_EXP);
                SET_GPIOEXP_CONFIG_INPUT(P3570_B2B_GPI_1_EXP);
                SET_GPIOEXP_CONFIG_INPUT(P3570_B2B_GPI_2_EXP);
                SET_GPIOEXP_OUTPUT_LOW(P3570_B2B_PG_OUT);
                B2B_ALWAYS_PRINTF(("Done X86 V3NC default config \r\n"));
                break;
            default:
                break;
        }
return TRUE;
}
#if 0
boolean b2b_cmd_nc(pchar args, void *data, IfxStdIf_DPipe *io){
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : b2bcmd temp"ENDL);
        IfxStdIf_DPipe_print(io, "           > Show the temperature of board to board side"ENDL);
        IfxStdIf_DPipe_print(io, "           > H_locate"ENDL);
        IfxStdIf_DPipe_print(io, "           > H_remote"ENDL);
        IfxStdIf_DPipe_print(io, "           > A_locate"ENDL);
        IfxStdIf_DPipe_print(io, "           > A_remote"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : b2bcmd power s0/s5"ENDL);
        return TRUE;
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "temp") != FALSE)
        {
            for(int i=0;i<4;i++)
                IfxStdIf_DPipe_print(io, "%d"ENDL,b2b_v3nc_temperature[i]);
        }else if (Ifx_Shell_matchToken(&args, "power") != FALSE)
        {
            if (Ifx_Shell_matchToken(&args, "s0") != FALSE){
                //b2b_polling_state = B2B_POLLING_CMD_S0;
                IfxStdIf_DPipe_print(io, "send s0 cmd"ENDL);
            }else if (Ifx_Shell_matchToken(&args, "s5") != FALSE){
                //b2b_polling_state = B2B_POLLING_CMD_S5;
                IfxStdIf_DPipe_print(io, "send s5 cmd"ENDL);
            }
        }
    }
    return TRUE;
}
#endif
boolean B2b_run(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if(!is_b2b_x86_v3nc){
        IfxStdIf_DPipe_print(io, "V3NC board isn't connected."ENDL);
        return TRUE;
    }

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : b2brun temp"ENDL);
        IfxStdIf_DPipe_print(io, "           > Show the temperature of board to board side"ENDL);

        return TRUE;
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "cpldtemp") != FALSE){
            if(g_CurrentPath==CURRENT_PATH_CPLD){
                IfxStdIf_DPipe_print(io, "Current repoter is CPLD."ENDL);
                //for(int i=0;i<4;i++)
                //    IfxStdIf_DPipe_print(io, "%d"ENDL,b2b_v3nc_temperature[i]);
                IfxStdIf_DPipe_print(io, "Sensor1(U59): %d degC"ENDL,b2b_v3nc_temperature[0]);
                IfxStdIf_DPipe_print(io, "Sensor2(Q59): %d degC"ENDL,b2b_v3nc_temperature[1]);
                IfxStdIf_DPipe_print(io, "Sensor3(U30): %d degC"ENDL,b2b_v3nc_temperature[2]);
                IfxStdIf_DPipe_print(io, "Sensor4(Q112): %d degC"ENDL,b2b_v3nc_temperature[3]);
            }else{
                IfxStdIf_DPipe_print(io, "Current path is BMC."ENDL);
            }
        }else if (Ifx_Shell_matchToken(&args, "bmctemp") != FALSE){
            if(g_CurrentPath==CURRENT_PATH_BMC){
                if(g_BMCTempEnable){
                    IfxStdIf_DPipe_print(io, "Current repoter is BMC."ENDL);
                    //for(int i=0;i<8;i++)
                    //    IfxStdIf_DPipe_print(io, "%d"ENDL,b2b_v3nc_temperature[i]);
                    IfxStdIf_DPipe_print(io, "Sensor1(U59): %d degC"ENDL,b2b_v3nc_temperature[0]);
                    IfxStdIf_DPipe_print(io, "Sensor2(Q59): %d degC"ENDL,b2b_v3nc_temperature[1]);
                    IfxStdIf_DPipe_print(io, "Sensor3(U30): %d degC"ENDL,b2b_v3nc_temperature[2]);
                    IfxStdIf_DPipe_print(io, "Sensor4(Q112): %d degC"ENDL,b2b_v3nc_temperature[3]);
                    IfxStdIf_DPipe_print(io, "Sensor5(U12): %d degC"ENDL,b2b_v3nc_temperature[4]);
                    IfxStdIf_DPipe_print(io, "Sensor6(Q104): %d degC"ENDL,b2b_v3nc_temperature[5]);
                    IfxStdIf_DPipe_print(io, "Sensor7(U33): %d degC"ENDL,b2b_v3nc_temperature[6]);
                    IfxStdIf_DPipe_print(io, "Sensor8(Q102): %d degC"ENDL,b2b_v3nc_temperature[7]);
                }else{ IfxStdIf_DPipe_print(io, "BMC Temp is disable, please enable BMC Temp."ENDL); }
            }else{
                IfxStdIf_DPipe_print(io, "Current path is CPLD."ENDL);
            }
        }else if (Ifx_Shell_matchToken(&args, "enablebmctemp") != FALSE){
            IfxStdIf_DPipe_print(io, "run b2brun enable bmc temp."ENDL);
            if(g_CurrentPath==CURRENT_PATH_BMC){
                g_BMCTempEnable=TRUE;
            }else{
                IfxStdIf_DPipe_print(io, "Current path is CPLD."ENDL);
            }
        }else if (Ifx_Shell_matchToken(&args, "disablebmctemp") != FALSE){
            IfxStdIf_DPipe_print(io, "run b2brun disable bmc temp."ENDL);
            if(g_CurrentPath==CURRENT_PATH_BMC){
                g_BMCTempEnable=FALSE;
            }else{
                IfxStdIf_DPipe_print(io, "Current path is CPLD."ENDL);
            }
        }else if (Ifx_Shell_matchToken(&args, "poweroff") != FALSE){
            if(g_BMCTempEnable){
                IfxStdIf_DPipe_print(io, "Please disable BMC Temp first."ENDL);
            }else{
                IfxStdIf_DPipe_print(io, "run b2brun poweroff."ENDL);
                onetime_cmd=B2B_POLLING_CMD_BMC_PWROFF;
                g_isSendCmdByShell=TRUE;
            }
        }else if (Ifx_Shell_matchToken(&args, "poweron") != FALSE){
            if(g_BMCTempEnable){
                IfxStdIf_DPipe_print(io, "Please disable BMC Temp first."ENDL);
            }else{
                IfxStdIf_DPipe_print(io, "run b2brun poweron."ENDL);
                onetime_cmd=B2B_POLLING_CMD_BMC_PWRON;
                g_isSendCmdByShell=TRUE;
            }
        }else if (Ifx_Shell_matchToken(&args, "powerreset") != FALSE){
            if(g_BMCTempEnable){
                IfxStdIf_DPipe_print(io, "Please disable BMC Temp first."ENDL);
            }else{
                IfxStdIf_DPipe_print(io, "run b2brun powerreset."ENDL);
                onetime_cmd=B2B_POLLING_CMD_BMC_PWRRESET;
                g_isSendCmdByShell=TRUE;
            }
        }else if (Ifx_Shell_matchToken(&args, "powercycle") != FALSE){
            if(g_BMCTempEnable){
                IfxStdIf_DPipe_print(io, "Please disable BMC Temp first."ENDL);
            }else{
                IfxStdIf_DPipe_print(io, "run b2brun powercycle."ENDL);
                onetime_cmd=B2B_POLLING_CMD_BMC_PWRCYCLE;
                g_isSendCmdByShell=TRUE;
            }
         }else if (Ifx_Shell_matchToken(&args, "powersoft") != FALSE){
            if(g_BMCTempEnable){
                IfxStdIf_DPipe_print(io, "Please disable BMC Temp first."ENDL);
            }else{
                IfxStdIf_DPipe_print(io, "run b2brun powersoft."ENDL);
                onetime_cmd=B2B_POLLING_CMD_BMC_PWRSOFT;
                g_isSendCmdByShell=TRUE;
            }
        }else if (Ifx_Shell_matchToken(&args, "getpath") != FALSE){
            IfxStdIf_DPipe_print(io, "run b2brun getpath."ENDL);
            onetime_cmd=B2B_POLLING_CMD_GET_CURRENTPATH;
            g_isSendCmdByShell=TRUE;
        }else if (Ifx_Shell_matchToken(&args, "postcode") != FALSE){
            IfxStdIf_DPipe_print(io, "run b2brun postcode."ENDL);
            onetime_cmd=B2B_POLLING_CMD_BMC_POSTCODE;
            g_isSendCmdByShell=TRUE;
        }else if (Ifx_Shell_matchToken(&args, "getbmcpowerstate") != FALSE){
            IfxStdIf_DPipe_print(io, "run b2brun getpowerstate."ENDL);
            if(g_CurrentPath==CURRENT_PATH_BMC)
                onetime_cmd=B2B_POLLING_CMD_BMC_GET_PWRSTATE;
            g_isSendCmdByShell=TRUE;
        }else if (Ifx_Shell_matchToken(&args, "switch2bmc") != FALSE){
            IfxStdIf_DPipe_print(io, "run b2brun switch2bmc."ENDL);
            onetime_cmd=B2B_POLLING_CMD_SWITCH2BMC;
            g_isSendCmdByShell=TRUE;
        }else if (Ifx_Shell_matchToken(&args, "switch2cpld") != FALSE){
            if(g_BMCTempEnable){
                IfxStdIf_DPipe_print(io, "Please disable BMC Temp first."ENDL);
            }else{
                IfxStdIf_DPipe_print(io, "run b2brun switch2cpld."ENDL);
                onetime_cmd=B2B_POLLING_CMD_SWITCH2CPLD;
                g_isSendCmdByShell=TRUE;
            }
        }else if (Ifx_Shell_matchToken(&args, "enabledebug") != FALSE){
            is_b2b_uart_debug = TRUE;
        }else if (Ifx_Shell_matchToken(&args, "disabledebug") != FALSE){
            is_b2b_uart_debug = FALSE;
        }else{
            IfxStdIf_DPipe_print(io, "b2brun can't find avaliable command."ENDL);
        }
        //if(g_isSendCmdByShell==TRUE){
            //waitTime(2*TimeConst[TIMER_INDEX_1S]);
            //if(g_rev_cmd==onetime_cmd){
                //IfxStdIf_DPipe_print(io, "send command complete."ENDL);
                //if(onetime_cmd==B2B_POLLING_CMD_GET_CURRENTPATH){
                    //IfxStdIf_DPipe_print(io, "currentpath is %d.",g_CurrentPath,ENDL);
                //}else if(onetime_cmd==B2B_POLLING_CMD_BMC_GET_PWRSTATE){
                    //IfxStdIf_DPipe_print(io, "bmc power state is %d.",g_CurrentBMCPowerstate,ENDL);
                //}
                //g_rev_cmd=0xff;
            //}else
                //IfxStdIf_DPipe_print(io, "send command and no response."ENDL);
        //}
    }
    return TRUE;
}

boolean B2b_cmd(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 board_id = 0;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax     : b2b status"ENDL);
		IfxStdIf_DPipe_print(io, "Syntax     : b2b config"ENDL);
		IfxStdIf_DPipe_print(io, "           > Show the status of board to board connection"ENDL);
		return TRUE;
	}
	else
	{
		if (Ifx_Shell_matchToken(&args, "status") != FALSE)
		{
			if(b2b_is_board_dectected())
			{
				IfxStdIf_DPipe_print(io, "2nd Board is connected:"ENDL);
				board_id = b2b_get_board_id();
				if(board_id == BRD_ID_NV_3570)
				{
					IfxStdIf_DPipe_print(io, "board id is P3570"ENDL);
				}
				else if (board_id == BRD_ID_X86_V3DA)
				{
					IfxStdIf_DPipe_print(io, "board id is X86 For V3DA"ENDL);
				}
                          else if (board_id == BRD_ID_X86_V3NC)
				{
					IfxStdIf_DPipe_print(io, "board id is X86 For V3NC"ENDL);
				}
				else
				{
					IfxStdIf_DPipe_print(io, "Unknown Board ID 0x%x"ENDL, board_id);
				}
				if(b2b_get_board_powergood_status())
				{
					IfxStdIf_DPipe_print(io, "2nd board power is up"ENDL);
				}
				else
				{
					IfxStdIf_DPipe_print(io, "2nd board power is NOT up"ENDL);
				}
			}
			else
			{
				IfxStdIf_DPipe_print(io, "2nd Board is NOT connected"ENDL);
			}
		}
		else if (Ifx_Shell_matchToken(&args, "config") != FALSE)
		{
			if(b2b_is_board_dectected())
			{
				IfxStdIf_DPipe_print(io, "0905 2nd Board is connected:"ENDL);
				board_id = b2b_get_board_id();
				if(board_id == BRD_ID_NV_3570)
				{
					IfxStdIf_DPipe_print(io, "board id is P3570"ENDL);
					AsclinShellInterface_execute_cmd("setport", "0 6 0");
					//AsclinShellInterface_execute_cmd("setport", "2 9 0");
					//AsclinShellInterface_execute_cmd("setport", "0 11 0");
				}
				else if (board_id == BRD_ID_X86_V3DA)
				{
					if(check_pin_status() == TRUE)
					{
#if 1					
						IfxStdIf_DPipe_print(io, "board id is X86"ENDL);
					    SET_GPIOEXP_OUTPUT_HIGH(P3570_B2B_TX_PWR_EN_EXP);
						waitTime(5*TimeConst[TIMER_INDEX_1S]);
						SET_GPIOEXP_OUTPUT_LOW(P3570_B2B_TX_PWR_EN_EXP);
						waitTime(1*TimeConst[TIMER_INDEX_1S]);
						SET_GPIOEXP_OUTPUT_HIGH(P3570_B2B_TX_PWR_EN_EXP);
						IfxStdIf_DPipe_print(io, "board id is X86 GPU"ENDL);
						waitTime(6*TimeConst[TIMER_INDEX_1S]);
						IfxStdIf_DPipe_print(io, "Phy init: Detect X86 is connected\r\n");
						//AURIX_B2B_GPO_1
						IfxStdIf_DPipe_print(io, "Phy init: Set RST to Lo\r\n");
						waitTime(1*TimeConst[TIMER_INDEX_1S]);
						waitTime(50*TimeConst[TIMER_INDEX_1MS]);
						//wait 100ms
						waitTime(100*TimeConst[TIMER_INDEX_1MS]);
						IfxStdIf_DPipe_print(io, "~~~~~~ Do b2b phy init ~~~~ \r\n");
						/* we change it to static ip  10.42.0.146 at start up */
                        IfxEth_B2BPhy_88q2112_init();
                        B2B_Ethernet_Phy_Re_Initial();
                        Ethernet_Devices_Initial(2);
#endif						
						}else{

                        }
				}
				else
				{
					IfxStdIf_DPipe_print(io, "Unknown Board ID 0x%x"ENDL, board_id);
				}
				if(b2b_get_board_powergood_status())
				{
					IfxStdIf_DPipe_print(io, "2nd board power is up"ENDL);
				}
				else
				{
					IfxStdIf_DPipe_print(io, "2nd board power is NOT up"ENDL);
				}
			}
			else
			{
				IfxStdIf_DPipe_print(io, "2nd Board is NOT connected"ENDL);
			}
		}
		else
		{
			IfxStdIf_DPipe_print(io, "Error : invalid parameter %s"ENDL, args);
			return TRUE;
		}
	}
}

boolean b2b_is_board_dectected(void)
{
	//if (B2B_PRST_DET_N == 00) //use ADC to check GPIO status.
	b2b_prst_det1 = VadcAutoScan_getGPIOState("B2B_PRST_DET1_N", 0);
	b2b_prst_det2 = VadcAutoScan_getGPIOState("B2B_PRST_DET2_N", 0);
	if((b2b_prst_det1 == FALSE) && (b2b_prst_det2 == FALSE))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

uint32 b2b_get_board_id(void)
{
	 // Read board ID using ADC
	if(VadcAutoScan_getGPIOState("B2B_BRD_ID0_RX", 0))
		b2b_brd_id |= 0x1;
	if(VadcAutoScan_getGPIOState("B2B_BRD_ID1_RX", 0))
		b2b_brd_id |= 0x2;
	if(VadcAutoScan_getGPIOState("B2B_BRD_ID2_RX", 0))
		b2b_brd_id |= 0x4;
	return b2b_brd_id;
}

boolean b2b_get_board_powergood_status(void)
{
	return READ_GPIOEXP_STATE(P3570_B2B_PG_in);
}
