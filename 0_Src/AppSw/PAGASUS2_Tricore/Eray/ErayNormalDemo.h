#include "Configuration.h"

#ifndef ERAYDEMO_H
#define ERAYDEMO_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Ifx_Types.h>
#include <Eray/Eray/IfxEray_Eray.h>
#include "IfxEray_Eray.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define DATA_PAYLOAD_LENGTH                  24 //10//24//13   /* 13 two byte words = 26 bytes data payload 	*/
#define NUMBER_ERAY_COMM_CYCLES 	        64// 64 * Number of tested communication cycles		*/
#define DEFAULT_NUM_S1                              1//5    /* Number of slots configured					*/
#define DEFAULT_NUM_S2                              1//5    /* Number of slots configured	*/
#define NUM_OF_INTEGERS                           7    /* Number of 32bit words; this value is calculated as: ceil[payload length (in bytes)/ size of int (array contains 32 bit values)]*/
#define NUM_OF_SLOT_MAX_S2                   8// 3
#define NUM_OF_SLOT_MAX_S1                   8// 3
#define NUM_OF_SLOT_MAX                        8// 3
#define NUM_OF_SLOT_DATA_FIELD             32//127
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef enum{
    ERAY_APP_STATE_IDLE                                       =0x00,
    ERAY_APP_STATE_IDLE_TO_INIT                          =0x01,
    ERAY_APP_STATE_WAIT_ACTIVE_OR_PASSIVE      =0x02,
    ERAY_APP_STATE_ACTIVE_OR_PASSIVE               =0x03,
    ERAY_APP_STATE_FINISH_COMMUNICATION         =0x04,
}eray_app_state;

typedef struct{
    uint32               frameId;
    uint16               txdata[NUM_OF_SLOT_DATA_FIELD];
}eraytxdata_t;

typedef struct{
    boolean                    istx;
    uint32                   frameId;
    uint16                   list_index;
    uint32                      cycle;
    IfxEray_BufferDirection   bufferDirection;
    uint16                   txdata[NUM_OF_SLOT_DATA_FIELD];
} erayconfig_t;

typedef struct
{
    boolean        enable;
    uint16        frameId;
    uint16       bufferindex;
}rxheader_list_t;

typedef struct
{
    boolean                                                 enable;
    uint16                                                 frameId;
    uint16                                            bufferindex;
    uint16            txdata[NUM_OF_SLOT_DATA_FIELD];
}txheader_list_t;

typedef struct
{
    IfxEray_Eray            eray;                                     /**< \brief Eray handle */
    IfxEray_Eray_Config     erayModuleConfig;                         /**< \brief Eray module Configuration structure */
    IfxEray_Eray_NodeConfig nodeConfig;                               /**< \brief Eray Node Configuration structure */
    volatile unsigned       ndatCount;                                /**< \brief received data (new data) */
    volatile unsigned       errors;                                   /**< \brief error interrupt counter */
    uint16                  receivedData[NUMBER_ERAY_COMM_CYCLES][NUM_OF_SLOT_MAX_S2][NUM_OF_SLOT_DATA_FIELD]; /**< \brief received data in slots */
    boolean                  isUpdate;
} App_Eray;

typedef enum
{
    ERAY_NORMAL_MODE_TEST_S1 = 0,
    ERAY_NORMAL_MODE_TEST_S2,
    ERAY_TEST_MODE                   =0x10
}Eray_run_mode;

typedef enum
{
    ERAY_S1 = 0,
    ERAY_S2,
    ERAY_SAMPLE_CONFIG
}Eray_Sample_Config;
/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN void eray_poc_state_monitor(void);
IFX_EXTERN App_Eray g_Eray;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void IfxErayDemo_init(void);
IFX_EXTERN void IfxErayDemo_run(void);

#endif

