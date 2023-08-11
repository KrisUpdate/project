#ifndef CONFIG_V31C_PRJECT_INFO
#define CONFIG_V31C_PRJECT_INFO

#define SW_REVISION_DATE    (0x20210202)            // Software date 0xyyyymmdd, with yyyy the year and mm the month and dd the day
#define SW_REVISION         (0x00000146)            // Software version and revision 0x0000vvrr, with vv the version number and rr the revision number

// Product FW
#define SW_MAJOR                01
#define SW_MINOR                36
#define SW_BUGFIX               06
#define SPA2_MCU_HIB			98
#define SPA2_MCU_HIA			97

#if SPA2_HIA == 1
#define SPA2_MCU_VAR			SPA2_MCU_HIA
#else
#define SPA2_MCU_VAR			SPA2_MCU_HIB
#endif

// Update FW
#define UPDATE_SW_MAJOR         01
#define UPDATE_SW_MINOR         36
#define UPDATE_SW_BUGFIX        06

// HW Version
#define DRIVE_AX_A00            0
#define DRIVE_AX_B0x            PG2_PREEVT   //0.9 version and later
#define PG2_PREEVT            1
#define PG2_EVT               0
#define PG2_DVT               0   
#define PG2_PVT               0   

#endif
