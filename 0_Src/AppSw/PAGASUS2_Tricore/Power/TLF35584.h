 /* COPYRIGHT (C) 2019
 * QUANTA CORPORATION
 * ALL RIGHTS RESERVED

 * THE SOURCE CODE AND ITS RELATED DOCUMENTATION IS PROVIDED "AS IS". QUANTA
 * CORPORATION MAKES NO OTHER WARRANTY OF ANY KIND, WHETHER EXPRESS, IMPLIED OR,
 * STATUTORY AND DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * SATISFACTORY QUALITY, NON INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * THE SOURCE CODE AND DOCUMENTATION MAY INCLUDE ERRORS. QUANTA CORPORATION
 * RESERVES THE RIGHT TO INCORPORATE MODIFICATIONS TO THE SOURCE CODE IN LATER
 * REVISIONS OF IT, AND TO MAKE IMPROVEMENTS OR CHANGES IN THE DOCUMENTATION OR
 * THE PRODUCTS OR TECHNOLOGIES DESCRIBED THEREIN AT ANY TIME.
 * 
 * QUANTA CORPORATION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGE OR LIABILITY ARISING FROM YOUR USE OF THE SOURCE CODE OR
 * ANY DOCUMENTATION, INCLUDING BUT NOT LIMITED TO, LOST REVENUES, DATA OR
 * PROFITS, DAMAGES OF ANY SPECIAL, INCIDENTAL OR CONSEQUENTIAL NATURE, PUNITIVE
 * DAMAGES, LOSS OF PROPERTY OR LOSS OF PROFITS ARISING OUT OF OR IN CONNECTION
 * WITH THIS AGREEMENT, OR BEING UNUSABLE, EVEN IF ADVISED OF THE POSSIBILITY OR
 * PROBABILITY OF SUCH DAMAGES AND WHETHER A CLAIM FOR SUCH DAMAGE IS BASED UPON
 * WARRANTY, CONTRACT, TORT, NEGLIGENCE OR OTHERWISE.					
 */
#ifndef TLF35584_H
#define TLF35584_H

#include <Cpu/Std/Ifx_Types.h>
#include "Platform_Types.h"
#include "Ifx_DateTime.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define	TLF_B_REG_DEVCTRL			0x15
#define	TLF_B_REG_DEVCTRLN			0x16

extern volatile uint32 TLF_B_Step;
extern volatile boolean wwd_enable;
extern volatile boolean eventFlag_10ms_wwd_service;
IFX_EXTERN void IfxTLF35584_init(void);
IFX_EXTERN void IfxTLF35584_unprotect_register(void);
IFX_EXTERN void IfxTLF35584_protect_register(void);
IFX_EXTERN void IfxTLF35584_disable_window_watchdog(void);
IFX_EXTERN void IfxTLF35584_enable_window_watchdog(void);

//void IfxTLF35584_disable_err_pin_monitor(void);
//void IfxTLF35584_protect_register(void);
IFX_EXTERN void IfxTLF35584_disable_err_pin_monitor(void);
IFX_EXTERN void IfxTLF35584_enable_err_pin_monitor(void);
IFX_EXTERN void IfxTLF35584_goto_normal_state(void);
IFX_EXTERN void IfxTLF35584_goto_standby_state(void);
IFX_EXTERN void IfxTLF35584_service_Window_Watchdog(void);
IFX_EXTERN uint32 IfxTLF35584_read_write(uint32 send_data);
IFX_EXTERN uint8 IfxTLF35584_simplified_read_write(boolean write_cmd, uint8 address, uint8 send_data);

#endif
