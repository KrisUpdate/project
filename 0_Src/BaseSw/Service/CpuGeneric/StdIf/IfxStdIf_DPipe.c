/**
 * \file IfxStdIf_DPipe.c
 * \brief Standard interface: Data Pipe
 * \ingroup IfxStdIf
 *
 * \copyright Copyright (c) 2013 Infineon Technologies AG. All rights reserved.
 *
 * $Date: 2014-02-27 20:08:23 GMT$
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 */

#include "IfxStdIf_DPipe.h"
#include "_Utilities/Ifx_Assert.h"
#include "configuration.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#if PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V31C || PROJECT == PROJECT_V3NA
#include "ethernet_config.h"
#include "udp.h"
#endif
#if PROJECT == PROJECT_TRIBOARD
#include "cores_rpc.h"
#endif
void IfxStdIf_DPipe_print(IfxStdIf_DPipe *stdif, pchar format, ...)
{
    if (!stdif->txDisabled)
    {
        char      message[STDIF_DPIPE_MAX_PRINT_SIZE + 1];
        Ifx_SizeT count;
        va_list   args;
        va_start(args, format);
        vsprintf((char *)message, format, args);
        va_end(args);
        count = (Ifx_SizeT)strlen(message);
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, count < STDIF_DPIPE_MAX_PRINT_SIZE);
        //return
#if PROJECT == PROJECT_TRIBOARD
        rpc_print(stdif, (void *)message, &count);
#else
        IfxStdIf_DPipe_write(stdif, (void *)message, &count, TIME_INFINITE);
#endif
#if PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V31C || PROJECT == PROJECT_V3NA
        //Added by NVIDIA
		//Also Redirect output to UDP port if connection exists
		if(tegra_console_server_ptr != NULL && cmd_src_ip.addr != 0 && cmd_src_port != 0){
			tegra_console_server_send(tegra_console_server_ptr, &cmd_src_ip, cmd_src_port, (uint8*)message, count);
		   }
#endif
    }
    else
    {
        //return TRUE;
    }
}
