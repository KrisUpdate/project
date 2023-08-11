
#ifndef __TEGRA_CONSOLE_H_
#define __TEGRA_CONSOLE_H_
#include "udp.h" // for tegra_console_init_udp
#include "Configuration_project.h" // for project separate

#define TEGRA_A_CLIENT_PORT     53000
#define TEGRA_B_CLIENT_PORT     53001
#define TEGRA_A_CLIENT_RETRY_PORT   63000 /* accept extra port for HPA retry (HPA meet "Can't grab 0.0.0.0:53000 with bind") */
#define TEGRA_B_CLIENT_RETRY_PORT   63001 /* accept extra port for HPA retry (HPA meet "Can't grab 0.0.0.0:53000 with bind") */

#define TEGRA_CONSOLE_SERVER_PORT   53002
#define TEGRA_CONSOLE_MONITOR_PORT	53003
#define TEGRA_CONSOLE_MONITOR_BUF_LEN 1500

#if PROJECT == PROJECT_V31C
//heartbeat function and variable
  #if SPA2_HIA == 1
    #define TEGRA_HEARTBEAT_DST_PORT  53010
  #else
    #define TEGRA_HEARTBEAT_DST_PORT  53020
  #endif
void tegra_udp_heartbeat_client_init(void);
void tegra_udp_heartbeat_client_send(uint32 heartbeat);
#endif

// UDP Shell
void tegra_console_init(void);

// UDP Monitor
void UdpMonitor_init(void);
int UdpMonitor_registor(char *src); // return an id
// void UdpMonitor_unregistor(int id);  // TODO: not required yet
void UdpMonitor_notice(int id);
boolean UdpMonitor_is_enabled();

// internal
void tegra_console_init_udp(struct udp_pcb **udp_pcb_ptr, u16_t port, udp_recv_fn recv);

// print UDP Command 
void print_udp_cmd_ctrl(boolean print_ctrl);

#endif  // __TEGRA_CONSOLE_H_
