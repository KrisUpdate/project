/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef _BOARD_H
#define _BOARD_H

#include "Configuration.h"

#define BOARD_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)

typedef enum
{
	E3550,
	P3479,
	P3570,
}e_project;

typedef enum
{
	MAIN_AURIX,
	SECOND_AURIX,
	MAX_AURIX_NUMBER,
}e_AurixIndex;

typedef struct
{
	uint8 project_char[4];
	uint8 pcb_spin;    //"A", "B", "C" ...
	uint8 pcb_version; // 00, 01, 02 ...
	e_project project;
	uint8 project_string[11];
	e_AurixIndex aurix_index;  // define whether binary is for main Aurix or 2nd Aurix chip
}t_board;

extern t_board board;

void load_board_config(void);
void force_board_config(e_project project);
e_AurixIndex detect_Aurix_index(void);

#endif //endof _BOARD_H
