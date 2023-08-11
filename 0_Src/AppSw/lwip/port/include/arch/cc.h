#ifndef IFX_LWIP_CC_H
#define IFX_LWIP_CC_H

#include <stdlib.h>
#include <Cpu/Std/Ifx_Types.h>
#include "lwipopts.h"
#include "Ifx_Console.h"

typedef uint8  u8_t;
typedef uint16 u16_t;
typedef uint32 u32_t;
typedef sint8  s8_t;
typedef sint16 s16_t;
typedef sint32 s32_t;
typedef u32_t  mem_ptr_t;
typedef u32_t sys_prot_t;

/* printf formatters for data types */
#define U16_F              "u"
#define S16_F              "d"
#define X16_F              "x"
#define U32_F              "u"
#define S32_F              "d"
#define X32_F              "x"
#define SZT_F              "zu"

#define BYTE_ORDER         LITTLE_ENDIAN

#if defined(__GNUC__) || defined(__DCC__)
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT __attribute__ ((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x
#elif __TASKING__
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x
#endif

#define LWIP_PROVIDE_ERRNO

#ifdef LWIP_DEBUG
#define printf Ifx_Console_print
#define LWIP_PLATFORM_ASSERT(msg)                                                           \
		printf("Assertion \"%s\" failed at line %d in %s\n", msg, __LINE__, __FILE__); \
    abort()
#define LWIP_PLATFORM_DIAG(msg) Ifx_Console_print msg
#else
#define LWIP_PLATFORM_ASSERT(msg) ((void)0)
#define LWIP_PLATFORM_DIAG(msg)   ((void)0)
#error
#endif

#endif  //__LWIP_EE_CC_TRICORE_H__
