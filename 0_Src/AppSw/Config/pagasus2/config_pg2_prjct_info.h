#ifndef CONFIG_PG2_PRJECT_INFO
#define CONFIG_PG2_PRJECT_INFO


#define SW_REVISION_DATE    (0x20220427)            // Software date 0xyyyymmdd, with yyyy the year and mm the month and dd the day

#define SW_REVISION         (0x00000406)            // Software version and revision 0x0000vvrr, with vv the version number and rr the revision number


// Product FW
#define SW_MAJOR                01
#define SW_MINOR                34
//prevent the Xavier OTA automatically from updating Aurix FW, keep it 11 above
#define SW_BUGFIX               99

// Update FW
#define UPDATE_SW_MAJOR         01
#define UPDATE_SW_MINOR         34
//prevent the Xavier OTA automatically from updating Aurix FW, keep it 11 above
#define UPDATE_SW_BUGFIX        99

#define AURIX_A_STEP            0
#define AURIX_B_STEP            0x10

// HW Version
#define DRIVE_AX_A00            0
#define DRIVE_AX_B0x            PG2_PREEVT   //0.9 version and later
#define PG2_PREEVT            1

#define PG2_A00               0
//EVT
#define PG2_A01               1
//DVT
#define PG2_A02               2
//DVT2
#define PG2_A03               3
//DVT3
#define PG2_A04               4

#define PG2_NULL             99

#endif
