/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "SysSe/Comm/Ifx_Shell.h"
#include <I2c.h>
#include "Tsensor.h"
#include "SysSe/Bsp/Bsp.h"
#include "TsensorCmds.h"
#include "Configuration.h"
#include "board.h"
#include "resource.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
volatile boolean display_temperature = FALSE;
volatile boolean eventFlag_10ms_temp_printing = FALSE;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \brief Handle the 'rdtemp' command
 *
 * \par Syntax
 *  - rdtemp [c/loop] [interval]:  read board temperature for [loop] times or continuously with interval in between read.
 */
void GetTemp(float *temp_1_local, float *temp_1_remote)
{
    *temp_1_local = readTsensorLocal(&i2c_tmp451_1);
    *temp_1_remote = readTsensorRemote(&i2c_tmp451_1);
}

boolean ReadTemp(pchar args, void *data, IfxStdIf_DPipe *io)
{
    pchar temp_args;

    display_temperature_loop_count = 1;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        #if 1
        IfxStdIf_DPipe_print(io, "Syntax: rdtemp [y]"ENDL);
        IfxStdIf_DPipe_print(io, "      > Read board temperature for loop y times. 1s interval between each read."ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: rdtemp c [x]"ENDL);
        IfxStdIf_DPipe_print(io, "      > Read board temperature continuously with 10ms interval x between each read."ENDL);

        #else

        IfxStdIf_DPipe_print(io, "Syntax: rdtemp c [x]"ENDL);
        IfxStdIf_DPipe_print(io, "      > Read board temperature continuously with 10ms interval x between read."ENDL);
        #endif
    }
    else
    {
        temp_args = args;
        if ((Ifx_Shell_matchToken(&args, "c") != FALSE) || (Ifx_Shell_matchToken(&args, "C") != FALSE))
        {
            display_temp_continuous = TRUE;

            if (Ifx_Shell_parseUInt32(&args, &display_temperature_interval, FALSE) == FALSE)
            {
                // Move temperature sensor to STM3 10ms loop, so change it to 100
                display_temperature_interval = 100;  // Tested under 100 will be nothing output always
            }

            if (display_temperature_interval == 0)
            {
                display_temperature_interval = 20;
            }

            IfxStdIf_DPipe_print(io, "Temperature read interval is %d * 10ms"ENDL"Use \"loff\" to stop"ENDL, display_temperature_interval);

            display_temperature = TRUE;
        }
        else
        {

            #if 1
            args = temp_args;
            if (Ifx_Shell_parseUInt32(&args, &display_temperature_loop_count, FALSE) == FALSE)
            {
                display_temperature_loop_count = 1;
            }

            display_temp_continuous= FALSE;

            display_temperature_interval= 100;  // Tested under 100 will be nothing output always
            display_temperature = TRUE;

            #else

            IfxStdIf_DPipe_print(io, "Syntax: rdtemp c [x]"ENDL);
            IfxStdIf_DPipe_print(io, "      > Read board temperature continuously with 10ms interval x between read."ENDL);
            #endif

        }

        //IfxStdIf_DPipe_print(io, "Temperature loop count is %d"ENDL, display_temperature_loop_count);

    }
    return TRUE;
}

void Print_TemperatureMsg(IfxStdIf_DPipe *io)
{
    float localtemp, localtemp2, localtemp3, localtemp4, localtemp5;
    //float remotetemp, remotetemp2, remotetemp3;
    float remotetemp;

    static uint32 loop_count=0;
    static uint32 interval_count=0;
    eventFlag_10ms_temp_printing = TRUE;
//    IfxStdIf_DPipe_print(io, "eventFlag_10ms_temp_printing %d"ENDL, eventFlag_10ms_temp_printing);
//    IfxStdIf_DPipe_print(io, "display_temperature %d"ENDL, display_temperature);
//    IfxStdIf_DPipe_print(io, "display_temp_continuous %d"ENDL, display_temp_continuous);
    if(display_temperature)
    {
        if(eventFlag_10ms_temp_printing)
        {
            eventFlag_10ms_temp_printing = FALSE;
            if (interval_count >= display_temperature_interval)
            {
                localtemp = readTsensorLocal(&i2c_tmp451_1);
                localtemp2 = readTsensorLocal(&i2c_tmp451_2);
                localtemp3 = readTsensorLocal(&i2c_tmp451_3);

                IfxStdIf_DPipe_print(io, "Reading Temperature as below:"ENDL);
                if(board.aurix_index == MAIN_AURIX)
                {
                    remotetemp = readTsensorRemote(&i2c_tmp451_1);
#ifdef float_support
                   
                    IfxStdIf_DPipe_print(io, "Sensor1(U51 Local): %.2f degC\r\n", localtemp);
                    IfxStdIf_DPipe_print(io, "Sensor2(U725 Local): %.2f degC\r\n", localtemp2);
                    IfxStdIf_DPipe_print(io, "Sensor3(U726 Local): %.2f degC\r\n", localtemp3);
                    IfxStdIf_DPipe_print(io, "Sensor1(Q44 Remote): %.2f degC\r\n", remotetemp);
#else
                    IfxStdIf_DPipe_print(io, ENDL"Sensor1(U51 Local): %d.%02d degC", (sint32)localtemp, localtemp>0 ? (sint32)(localtemp*100)%100 : -1*(sint32)(localtemp*100)%100 );
                    IfxStdIf_DPipe_print(io, ENDL"Sensor2(U725 Local): %d.%02d degC", (sint32)localtemp2, localtemp2>0 ? (sint32)(localtemp2*100)%100 : -1*(sint32)(localtemp2*100)%100 );
                    IfxStdIf_DPipe_print(io, ENDL"Sensor3(U726 Local): %d.%02d degC", (sint32)localtemp3, localtemp3>0 ? (sint32)(localtemp3*100)%100 : -1*(sint32)(localtemp3*100)%100 );
                    IfxStdIf_DPipe_print(io, ENDL"Sensor1(Q44 Remote): %d.%02d degC", (sint32)remotetemp, remotetemp>0 ? (sint32)(remotetemp*100)%100 : -1*(sint32)(remotetemp*100)%100 );
#endif
                }else{
                    //i2cbus 0, anrix2 only
                    localtemp4 = readTsensorLocal(&i2c_tmp451_4);
                    localtemp5 = readTsensorLocal(&i2c_tmp451_5);
#ifdef float_support
                    IfxStdIf_DPipe_print(io, "Sensor1(U9 Local): %.2f degC\r\n", localtemp);
                    IfxStdIf_DPipe_print(io, "Sensor2(U22 Local): %.2f degC\r\n", localtemp2);
                    IfxStdIf_DPipe_print(io, "Sensor3(U30 Local): %.2f degC\r\n", localtemp3);
                    IfxStdIf_DPipe_print(io, "Sensor4(U6  Local): %.2f degC\r\n", localtemp4);
                    IfxStdIf_DPipe_print(io, "Sensor5(U10 Local): %.2f degC\r\n", localtemp5);
#else
                    IfxStdIf_DPipe_print(io, ENDL"Sensor1(U9 Local): %d.%02d degC", (sint32)localtemp, localtemp>0 ? (sint32)(localtemp*100)%100 : -1*(sint32)(localtemp*100)%100 );
                    IfxStdIf_DPipe_print(io, ENDL"Sensor2(U22 Local): %d.%02d degC", (sint32)localtemp2, localtemp2>0 ? (sint32)(localtemp2*100)%100 : -1*(sint32)(localtemp2*100)%100 );
                    IfxStdIf_DPipe_print(io, ENDL"Sensor3(U30 Local): %d.%02d degC", (sint32)localtemp3, localtemp3>0 ? (sint32)(localtemp3*100)%100 : -1*(sint32)(localtemp3*100)%100 );
                    IfxStdIf_DPipe_print(io, ENDL"Sensor4(U6  Local): %d.%02d degC", (sint32)localtemp4, localtemp4>0 ? (sint32)(localtemp4*100)%100 : -1*(sint32)(localtemp4*100)%100 );
                    IfxStdIf_DPipe_print(io, ENDL"Sensor5(U10 Local): %d.%02d degC", (sint32)localtemp5, localtemp5>0 ? (sint32)(localtemp5*100)%100 : -1*(sint32)(localtemp5*100)%100 );
#endif
                }
                interval_count = 0;

                if(!display_temp_continuous)
                {
                    loop_count++;
                    if (loop_count >= display_temperature_loop_count)
                    {
                        display_temperature=FALSE;
                        loop_count=0;
                        IfxStdIf_DPipe_print(io, ENDL);
                    }
                }
            }
            interval_count++;
        }

    }
}

void ReadTsensor(IfxStdIf_DPipe *io)
{
    float localtemp, localtemp2, localtemp3, localtemp4, localtemp5;
    //float remotetemp, remotetemp2, remotetemp3;
    float remotetemp;

    localtemp = readTsensorLocal(&i2c_tmp451_1);
    localtemp2 = readTsensorLocal(&i2c_tmp451_2);
    localtemp3 = readTsensorLocal(&i2c_tmp451_3);
    if(board.aurix_index == MAIN_AURIX)
    {
        remotetemp = readTsensorRemote(&i2c_tmp451_1);
#ifdef float_support

        IfxStdIf_DPipe_print(io, "Sensor1(U51 Local): %.2f degC\r\n", localtemp);
        IfxStdIf_DPipe_print(io, "Sensor2(U725 Local): %.2f degC\r\n", localtemp2);
        IfxStdIf_DPipe_print(io, "Sensor3(U726 Local): %.2f degC\r\n", localtemp3);
        IfxStdIf_DPipe_print(io, "Sensor1(Q44 Remote): %.2f degC\r\n", remotetemp);
#else
        IfxStdIf_DPipe_print(io, ENDL"Sensor1(U51 Local): %d.%02d degC", (sint32)localtemp, localtemp>0 ? (sint32)(localtemp*100)%100 : -1*(sint32)(localtemp*100)%100 );
        IfxStdIf_DPipe_print(io, ENDL"Sensor2(U725 Local): %d.%02d degC", (sint32)localtemp2, localtemp2>0 ? (sint32)(localtemp2*100)%100 : -1*(sint32)(localtemp2*100)%100 );
        IfxStdIf_DPipe_print(io, ENDL"Sensor3(U726 Local): %d.%02d degC", (sint32)localtemp3, localtemp3>0 ? (sint32)(localtemp3*100)%100 : -1*(sint32)(localtemp3*100)%100 );
        IfxStdIf_DPipe_print(io, ENDL"Sensor1(Q44 Remote): %d.%02d degC", (sint32)remotetemp, remotetemp>0 ? (sint32)(remotetemp*100)%100 : -1*(sint32)(remotetemp*100)%100 );
#endif
    }else{
        localtemp4 = readTsensorLocal(&i2c_tmp451_4);
        localtemp5 = readTsensorLocal(&i2c_tmp451_5);
#ifdef float_support
        IfxStdIf_DPipe_print(io, "Sensor1(U9 Local): %.2f degC\r\n", localtemp);
        IfxStdIf_DPipe_print(io, "Sensor2(U22 Local): %.2f degC\r\n", localtemp2);
        IfxStdIf_DPipe_print(io, "Sensor3(U30 Local): %.2f degC\r\n", localtemp3);
        IfxStdIf_DPipe_print(io, "Sensor4(U6  Local): %.2f degC\r\n", localtemp4);
        IfxStdIf_DPipe_print(io, "Sensor5(U10 Local): %.2f degC\r\n", localtemp5);
#else
        IfxStdIf_DPipe_print(io, ENDL"Sensor1(U9 Local): %d.%02d degC", (sint32)localtemp, localtemp>0 ? (sint32)(localtemp*100)%100 : -1*(sint32)(localtemp*100)%100 );
        IfxStdIf_DPipe_print(io, ENDL"Sensor2(U22 Local): %d.%02d degC", (sint32)localtemp2, localtemp2>0 ? (sint32)(localtemp2*100)%100 : -1*(sint32)(localtemp2*100)%100 );
        IfxStdIf_DPipe_print(io, ENDL"Sensor3(U30 Local): %d.%02d degC", (sint32)localtemp3, localtemp3>0 ? (sint32)(localtemp3*100)%100 : -1*(sint32)(localtemp3*100)%100 );
        IfxStdIf_DPipe_print(io, ENDL"Sensor4(U6  Local): %d.%02d degC", (sint32)localtemp4, localtemp4>0 ? (sint32)(localtemp4*100)%100 : -1*(sint32)(localtemp4*100)%100 );
        IfxStdIf_DPipe_print(io, ENDL"Sensor5(U10 Local): %d.%02d degC", (sint32)localtemp5, localtemp5>0 ? (sint32)(localtemp5*100)%100 : -1*(sint32)(localtemp5*100)%100 );
#endif
    }
}



/** \brief Handle the 'ThermTest' command
 *
 * \par Syntax
 *  - thermtest [loop]: Test thermal sensor alert/shutdown for [loop] times, default loop = 1
 */
boolean ThermTest(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 loop = 0;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : thermtest [loop] "ENDL);
        IfxStdIf_DPipe_print(io, "           > test thermal sensor alert/shutdown for [loop] times"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &loop, FALSE) == FALSE)
        {
            loop = 1;
        }
        if (loop == 0)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid loop %d, needs to be larger than 0"ENDL, loop);
            return TRUE;
        }

        {
            IfxStdIf_DPipe_print(io, "Start testing thermal sensor1 0x4C "ENDL);
            testTsensor(&i2c_tmp451_1, loop);

            IfxStdIf_DPipe_print(io, "\r\nStart testing thermal sensor2 0x49 "ENDL);
            testTsensor(&i2c_tmp451_2, loop);

            IfxStdIf_DPipe_print(io, "\r\nStart testing thermal sensor3 0x4E "ENDL);
            testTsensor(&i2c_tmp451_3, loop);
        }
    }
    return TRUE;
}

/** \brief Handle the 'ThermTest' command
 *
 * \par Syntax
 *  - thermtest [loop]: Test thermal sensor alert/shutdown for [loop] times, default loop = 1
 */
boolean Aurix2ThermTest(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 loop = 0;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : thermtest [loop] "ENDL);
        IfxStdIf_DPipe_print(io, "           > test thermal sensor alert/shutdown for [loop] times"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &loop, FALSE) == FALSE)
        {
            loop = 1;
        }
        if (loop == 0)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid loop %d, needs to be larger than 0"ENDL, loop);
            return TRUE;
        }

        IfxStdIf_DPipe_print(io, "Start testing thermal sensor1 i2c1 0x4C "ENDL);
        testAurix2Tsensor(&i2c_tmp451_1, loop);

        IfxStdIf_DPipe_print(io, "\r\nStart testing thermal sensor2 i2c1 0x49 "ENDL);
        testAurix2Tsensor(&i2c_tmp451_2, loop);

        IfxStdIf_DPipe_print(io, "\r\nStart testing thermal sensor3 i2c1 0x4E "ENDL);
        testAurix2Tsensor(&i2c_tmp451_3, loop);

        IfxStdIf_DPipe_print(io, "Start testing thermal sensor4 i2c0 0x4C "ENDL);
        testAurix2Tsensor(&i2c_tmp451_4, loop);

        IfxStdIf_DPipe_print(io, "\r\nStart testing thermal sensor5 i2c0 0x49 "ENDL);
        testAurix2Tsensor(&i2c_tmp451_5, loop);
    }
    return TRUE;
}
