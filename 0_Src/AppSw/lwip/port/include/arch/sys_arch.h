/*
 * sys_arch.h
 *
 *  Created on: 2018¦~12¤ë20¤é
 *      Author: 95111301
 */

#ifndef LWIP_ARCH_SYS_ARCH_H
#define LWIP_ARCH_SYS_ARCH_H

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "os.h"
#include "lwipopts.h"
#if (NO_SYS == 0)
struct _sys_mut {
  void *mut;
};
typedef struct _sys_mut sys_mutex_t;
#define sys_mutex_valid_val(mutex)   ((mutex).mut != NULL)
#define sys_mutex_valid(mutex)       (((mutex) != NULL) && sys_mutex_valid_val(*(mutex)))
#define sys_mutex_set_invalid(mutex) ((mutex)->mut = NULL)

struct _sys_mbox {
  void *mbx;
};
typedef struct _sys_mbox sys_mbox_t;
#define sys_mbox_valid_val(mbox)   ((mbox).mbx != NULL)
#define sys_mbox_valid(mbox)       (((mbox) != NULL) && sys_mbox_valid_val(*(mbox)))
#define sys_mbox_set_invalid(mbox) ((mbox)->mbx = NULL)

struct _sys_thread {
  void *thread_handle;
};
typedef struct _sys_thread sys_thread_t;


typedef struct SemType sys_sem_t;
#else
#define SYS_MBOX_NULL   NULL
#define SYS_SEM_NULL    NULL

typedef void * sys_prot_t;

typedef void * sys_sem_t;

typedef void * sys_mbox_t;

typedef void * sys_thread_t;
#endif
void sys_arch_msleep(u32_t delay_ms);
#define sys_msleep(ms) sys_arch_msleep(ms)


#endif /* LWIP_ARCH_SYS_ARCH_H */
