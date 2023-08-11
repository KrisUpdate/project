/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#include "Configuration.h"
#include "string.h"
#include "stdio.h"
#include "board.h"
#include "internal_fw_debug.h"
#include "board_I2C_devices.h"
#include "I2cRom.h"
#include "i2crom_drv.h"
#include "error_report.h"

/* These define the default projects if I2C ROM reading is fail */
#define DEFAULT_PROJECT         E3550
#define DEFAULT_PROJECT_CHAR    "3550"
#define DEFAULT_PCB_SPIN        "B"
#define DEFAULT_PCB_VERSION     1

/* Enable this Macro if we need to force the board regardless of Info ROM reading
 * This is for code debugging and board bring up ONLY */
#define IS_RROJECT_FORCED_ENABLED  1
#define DEBUG_PROJECT         P3570
#define DEBUG_PROJECT_CHAR    "3570"
#define DEBUG_PCB_SPIN        "A"
#define DEBUG_PCB_VERSION     0
#define DEBUG_AURIX_INDEX     MAX_AURIX_NUMBER


t_board board =
{
        .project = E3550,
        .aurix_index = MAIN_AURIX,
};

static void update_project_string(void);

e_AurixIndex detect_Aurix_index(void)
{
    e_AurixIndex aurixIndex = MAIN_AURIX;
    //P3570_AURIX_ID: LOW: MAIN Aurix
    //                HIGH: Aurix 2
    if(IfxPort_getPinState(P3570_AURIX_ID.port, P3570_AURIX_ID.pinIndex) == FALSE)
    {
        aurixIndex = MAIN_AURIX;
    }
    else
    {
        aurixIndex = SECOND_AURIX;
    }
    return aurixIndex;
}

static t_rom_object info_pcb_ROM = {0, 0x54, 224, 224};
void load_board_config(void)
{
    uint8 buffer[30];
    uint8 buffer_pcb[4];
    uint8 name_id[4]="rwk";
    uint8 output_buffer[2];
    uint8 pcb_buffer_nv[2];


    memset(buffer, 0, sizeof(buffer));
    memset(buffer_pcb, 0, sizeof(buffer_pcb));
    BOARD_ALWAYS_PRINTF(("Set board to 3570 by default\r\n"));

    memset(buffer, 0, sizeof(buffer));
    i2crom_user_info_read(buffer, name_id, 0);
    i2crom_drv_read(&info_pcb_ROM, 68, pcb_buffer_nv, 2); /*Read PCB REV start address*/

    if (pcb_buffer_nv[1]==0x41){
        BOARD_ALWAYS_PRINTF(("Read Customer PCB_REV\r\n"));
        if(pcb_buffer_nv[0]==0x01){
            board.pcb_version = PG2_A01;
        }
        else if(pcb_buffer_nv[0]==0x02){
            board.pcb_version = PG2_A02;
        }
        else if(pcb_buffer_nv[0]==0x00){
            board.pcb_version = PG2_A00;
        }
        else if(pcb_buffer_nv[0]>=0x03){
            board.pcb_version = pcb_buffer_nv[0];
        }
        else{
            BOARD_ALWAYS_PRINTF(("WARNING: Inforom has no any data!\r\n"));
            board.pcb_version = PG2_NULL;
        }
        if(buffer[0]==0x0A)
            BOARD_ALWAYS_PRINTF(("Customer PCB_REV and Quanta PCB coexist!\r\n"));
    }
    else{
        BOARD_ALWAYS_PRINTF(("Read Quanta PCB_REV\r\n"));
        if(buffer[0]==0x0A && buffer[1]==0x01){
            board.pcb_version = PG2_A01;
            i2crom_user_info_clear();
            output_buffer[0] = buffer[1];
            output_buffer[1] = 0x41; //A ASCII code
            i2crom_user_info_flash(output_buffer, 68, 2); /*Write PCB REV start address*/
        }
        else if(buffer[0]==0x0A && buffer[1]==0x02){
            board.pcb_version = PG2_A02;
            i2crom_user_info_clear();
            output_buffer[0] = buffer[1];
            output_buffer[1] = 0x41; //A ASCII code
            i2crom_user_info_flash(output_buffer, 68, 2); /*Write PCB REV start address*/
        }
        else if(buffer[0]==0x0A && buffer[1]==0x00){
            board.pcb_version = PG2_A00;
            i2crom_user_info_clear();
            output_buffer[0] = buffer[1];
            output_buffer[1] = 0x41; //A ASCII code
            i2crom_user_info_flash(output_buffer, 68, 2); /*Write PCB REV start address*/
        }
        else if(buffer[0]==0x0A && buffer[1]>=0x03){
            board.pcb_version = buffer[1];
            i2crom_user_info_clear();
            output_buffer[0] = buffer[1];
            output_buffer[1] = 0x41; //A ASCII code
            i2crom_user_info_flash(output_buffer, 68, 2); /*Write PCB REV start address*/
        }
        else{
            BOARD_ALWAYS_PRINTF(("WARNING: Inforom has no any data!\r\n"));
            board.pcb_version = PG2_NULL;
        }
    }


    if(board.pcb_version != PG2_NULL){
		if(board.pcb_version <= 0x09 && board.pcb_version >= 0x00)
	        BOARD_ALWAYS_PRINTF(("A0%d\r\n",board.pcb_version));
		else
			BOARD_ALWAYS_PRINTF(("A%d\r\n",board.pcb_version));
    }




/*
    for(i=0;i<30;i++)
      BOARD_ALWAYS_PRINTF(("%X \r\n", buffer1[i]));
*/
#if IS_RROJECT_FORCED_ENABLED == 1
    memcpy(board.project_char, DEBUG_PROJECT_CHAR, 4);
    memcpy(&board.pcb_spin, DEBUG_PCB_SPIN, 1);
    board.project = DEBUG_PROJECT;
    //set_error_flag(INVALID_ROM_CONTENT_ERR);

    if(board.project == P3570)
    {
        board.aurix_index = detect_Aurix_index();
    }
    else
    {
        board.aurix_index = MAIN_AURIX;
    }
#endif

    update_project_string();

    /* Update the i2c device address based on project in run time */
    load_i2c_devices_address();
}

void force_board_config(e_project project)
{
    board.project = project;
}

static void update_project_string(void)
{
    uint8 buffer[6];
    memset(board.project_string,  0, sizeof(board.project_string));
    if((board.project == P3479) || (board.project == P3570))
    {
        memcpy(&board.project_string[0], "P", 1);
    }
    else
    {
        memcpy(&board.project_string[0], "E", 1);
    }
    memcpy(&board.project_string[1], board.project_char, 4);
    memcpy(&board.project_string[5], "-", 1);
    memcpy(&board.project_string[6], &board.pcb_spin, 1);
//  sprintf((char*)buffer, "%02d\r\n",board.pcb_version);
    memcpy(&board.project_string[7], buffer, 2);
//  BOARD_ALWAYS_PRINTF(("Project is %s\r\n", board.project_string));
}
