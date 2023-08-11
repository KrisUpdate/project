/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef FLASHCMD_H_
#define FLASHCMD_H_

//#include "Configuration.h"

boolean pflash_write(pchar args, void *data, IfxStdIf_DPipe *io);
boolean pflash_read(pchar args, void *data, IfxStdIf_DPipe *io);
boolean pflash_read_byte(pchar args, void *data, IfxStdIf_DPipe *io);
//boolean pflash_jump(pchar args, void *data, IfxStdIf_DPipe *io);
boolean SwitchToProdFw(pchar args, void *data, IfxStdIf_DPipe *io);
boolean SwitchToUpdFw(pchar args, void *data, IfxStdIf_DPipe *io);
boolean ota_config(pchar args, void *data, IfxStdIf_DPipe *io);
//void pflash_correct_from_ota();
//void pflash_CORRUPT_from_ota();

#endif /* 0_APPSW_TRICORE_FLASH_FLASHCMD_H_ */