/**
 * \file IfxGtm_Trig.c
 * \brief GTM TRIG details
 *
 * \version iLLD_1_0_1_9_0
 * \copyright Copyright (c) 2018 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxGtm_Trig.h"
#include "_Utilities/Ifx_Assert.h"
#include "Cpu/Std/IfxCpu_Intrinsics.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Gtm_Trig_Data_Structures
 * \{ */
typedef struct
{
    sint8 config;       /**< \brief Value for SEL0/1. config=0xFF means "not available" */
} IfxGtm_Trig_AdcTrig_Channel;

typedef struct
{
    sint8 config;       /**< \brief Value for SEL0/1. config=0xFF means "not available" */
} IfxGtm_Trig_CanTrig_Channel;

typedef struct
{
    sint8 config;       /**< \brief Value for SELx. config=0xFF means "not available" */
} IfxGtm_Trig_MscTrig_Channel;

/** \} */

/** \addtogroup IfxLld_Gtm_Trig_Data_Structures
 * \{ */
typedef struct
{
    IfxGtm_Trig_AdcTrig_Channel channel[IfxGtm_Trig_AdcTrigChannel_count];
} IfxGtm_Trig_AdcTrig_Source;

typedef struct
{
    IfxGtm_Trig_CanTrig_Channel channel[IfxGtm_Trig_CanTrigChannel_count];
} IfxGtm_Trig_CanTrig_Source;

typedef struct
{
    IfxGtm_Trig_MscTrig_Channel channel[IfxGtm_Trig_MscTrigChannel_count];
} IfxGtm_Trig_MscTrig_Source;

/** \} */

/** \addtogroup IfxLld_Gtm_Trig_Data_Structures
 * \{ */
typedef struct
{
    IfxGtm_Trig_AdcTrig_Source source[IfxGtm_Trig_AdcTrigSource_count];
} IfxGtm_Trig_AdcTrig_Trigger;

typedef struct
{
    IfxGtm_Trig_CanTrig_Source source[IfxGtm_Trig_CanTrigSource_count];
} IfxGtm_Trig_CanTrig_Trigger;

typedef struct
{
    IfxGtm_Trig_MscTrig_Source source[IfxGtm_Trig_MscTrigSource_count];
} IfxGtm_Trig_MscTrig_Set;

/** \} */

/** \addtogroup IfxLld_Gtm_Trig_Data_Structures
 * \{ */
typedef struct
{
    IfxGtm_Trig_AdcTrig_Trigger trigger[IfxGtm_Trig_AdcTrig_count];
} IfxGtm_Trig_AdcTrig_Table;

typedef struct
{
    IfxGtm_Trig_CanTrig_Trigger trigger;
} IfxGtm_Trig_CanTrig_Table;

typedef struct
{
    IfxGtm_Trig_MscTrig_Set set[IfxGtm_Trig_MscTrigSet_count];
} IfxGtm_Trig_MscTrig_Table;

/** \} */

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

IFX_STATIC IFX_CONST IfxGtm_Trig_CanTrig_Table IfxGtm_Trig_CanTrig_table0 = {
    .trigger    = {
        .source = {                                                                                                                                                                                                                                             //IfxGtm_CanTrig_0
            //{.channel = {    {Channel_0     }, {Channel_1     }, {Channel_2     }, {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     },  {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }}},
            {.channel= {      {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x08}, {.config = 0x09}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0A}, {.config = 0x0B}}},          //IfxGtm_CanTrigSource_tom0
            {.channel= {      {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x00}, {.config = 0x01}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x02}, {.config = 0x03}, {.config = 0xFF}, {.config = 0xFF}}},          //IfxGtm_CanTrigSource_tom1

            {.channel= {      {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}},          //IfxGtm_CanTrigSource_atom0
            {.channel= {      {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}},          //IfxGtm_CanTrigSource_atom1
            {.channel= {      {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}},          //IfxGtm_CanTrigSource_atom2
        }
    },
};

IFX_STATIC IFX_CONST IfxGtm_Trig_CanTrig_Table IfxGtm_Trig_CanTrig_table1 = {
    .trigger    = {
        .source = {                                                                                                                                                                                                                                             //IfxGtm_CanTrig_1
            //{.channel = {    {Channel_0     }, {Channel_1     }, {Channel_2     }, {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     },  {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }}},
            {.channel= {      {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}},          //IfxGtm_CanTrigSource_tom0
            {.channel= {      {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x00}, {.config = 0x01}, {.config = 0x08}, {.config = 0x09}, {.config = 0x02}, {.config = 0x03}, {.config = 0x0A}, {.config = 0x0B}}},          //IfxGtm_CanTrigSource_tom1

            {.channel= {      {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}},          //IfxGtm_CanTrigSource_atom0
            {.channel= {      {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}},          //IfxGtm_CanTrigSource_atom1
            {.channel= {      {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}},          //IfxGtm_CanTrigSource_atom2
        }
    },
};

IFX_STATIC IFX_CONST IfxGtm_Trig_CanTrig_Table IfxGtm_Trig_CanTrig_table2_3 = {
    .trigger    = {
        .source = {                                                                                                                                                                                                                                      //IfxGtm_CanTrig_2 to 7
            //{.channel = {    {Channel_0     }, {Channel_1     }, {Channel_2     }, {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     },  {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }}},
            {.channel= {      {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}},          //IfxGtm_CanTrigSource_tom0
            {.channel= {      {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x00}, {.config = 0x01}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x02}, {.config = 0x03}, {.config = 0xFF}, {.config = 0xFF}}},          //IfxGtm_CanTrigSource_tom1

            {.channel= {      {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}},          //IfxGtm_CanTrigSource_atom0
            {.channel= {      {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}},          //IfxGtm_CanTrigSource_atom1
            {.channel= {      {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}},          //IfxGtm_CanTrigSource_atom2
        }
    },
};

IFX_STATIC IFX_CONST IfxGtm_Trig_AdcTrig_Table IfxGtm_Trig_EvadcTrig_tableAdc0_1_2 = {
    .trigger = {
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_0
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07}, {.config = 0x08}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0x0D}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x01}, {.config = 0x02}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x03}, {.config = 0x04}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_1
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07}, {.config = 0x08}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x09}, {.config = 0x0A}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x01}, {.config = 0x02}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x03}, {.config = 0x04}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_2
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x07}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0x08}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0E}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_3
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0x01}, {.config = 0xFF}, {.config = 0x02}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0x03}, {.config = 0xFF}, {.config = 0x04}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0x05}, {.config = 0xFF}, {.config = 0x06}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0x07}, {.config = 0x08}, {.config = 0x09}, {.config = 0xFF}, {.config = 0x0A}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0xFF}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0F}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_4
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0xFF}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0xFF}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0x07}, {.config = 0x08}, {.config = 0x09}, {.config = 0xFF}, {.config = 0x0A}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0F}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }}
    }
};

IFX_STATIC IFX_CONST IfxGtm_Trig_AdcTrig_Table IfxGtm_Trig_EvadcTrig_tableAdc3_4 = {
    .trigger = {
        {.source = {                                                                                                                                                                                                               //IfxGtm_AdcTrig_0
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07}, {.config = 0x08}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x01}, {.config = 0x02}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x03}, {.config = 0x04         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x09}, {.config = 0x0A}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0F}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                               //IfxGtm_AdcTrig_1
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07}, {.config = 0x08}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x01}, {.config = 0x02}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x03}, {.config = 0x04 /*0x09*/}, {.config = 0x0A}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0F}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                               //IfxGtm_AdcTrig_2
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0x07}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0x08}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0x0E}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                               //IfxGtm_AdcTrig_3
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0x01}, {.config = 0xFF}, {.config = 0x02}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0x03}, {.config = 0xFF}, {.config = 0x04}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0x05}, {.config = 0xFF}, {.config = 0x06}, {.config = 0xFF}, {.config = 0x07}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0x08}, {.config = 0xFF}, {.config = 0x09}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0x0A}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0xFF}, {.config = 0x0D}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0F         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                               //IfxGtm_AdcTrig_4
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0xFF}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0xFF}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0x07}, {.config = 0x08}, {.config = 0x09}, {.config = 0xFF}, {.config = 0x0A}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0F         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }}
    }
};

IFX_STATIC IFX_CONST IfxGtm_Trig_AdcTrig_Table IfxGtm_Trig_EvadcTrig_tableAdc5_6_7 = {
    .trigger = {
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_0
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0x05}, {.config = 0x06}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0x07}, {.config = 0x08}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x09}, {.config = 0x0A}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x03}, {.config = 0x04}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x01}, {.config = 0x02}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_1
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x05}, {.config = 0x06}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x08}, {.config = 0x09}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x09}, {.config = 0x0A}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x03}, {.config = 0x04}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x01}, {.config = 0x02}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_2
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0x07}, {.config = 0x08}, {.config = 0x09}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0xFF}, {.config = 0x0C}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_3
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0x01}, {.config = 0xFF}, {.config = 0x02}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0x03}, {.config = 0xFF}, {.config = 0x04}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0x05}, {.config = 0xFF}, {.config = 0x06}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0x07}, {.config = 0xFF}, {.config = 0x08}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0x09}, {.config = 0xFF}, {.config = 0x0A}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0x0B}, {.config = 0xFF}, {.config = 0x0C}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0x0D}, {.config = 0x0D}, {.config = 0xFF}, {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_4
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0x03}, {.config = 0xFF}, {.config = 0x04}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0x05}, {.config = 0xFF}, {.config = 0x06}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0x09}, {.config = 0xFF}, {.config = 0x0A}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0B}, {.config = 0xFF}, {.config = 0x0C}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0D}, {.config = 0xFF}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x01}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0x0F}, {.config = 0x07}, {.config = 0xFF}, {.config = 0x08}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x02}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }}
    }
};

IFX_STATIC IFX_CONST IfxGtm_Trig_AdcTrig_Table IfxGtm_Trig_EvadcTrig_tableAdc8_9_10_11 = {
    .trigger = {
        {.source = {                                                                                                                                                                                                                        //IfxGtm_AdcTrig_0
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07         }, {.config = 0x08         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x01 /*0x09*/}, {.config = 0x02 /*0x0A*/}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x03}, {.config = 0x04}, {.config = 0x0F}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                                        //IfxGtm_AdcTrig_1
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07         }, {.config = 0x08         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0D         }, {.config = 0x0E         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0B         }, {.config = 0x0C         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x09}, {.config = 0x0A}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x01         }, {.config = 0x02         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x03}, {.config = 0x04}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                                        //IfxGtm_AdcTrig_2
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0x07}, {.config = 0x08}, {.config = 0x09}, {.config = 0xFF         }, {.config = 0x0A         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0xFF         }, {.config = 0x0E         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                                        //IfxGtm_AdcTrig_3
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0x01}, {.config = 0xFF}, {.config = 0x02}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0x03}, {.config = 0xFF}, {.config = 0x04}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0x05}, {.config = 0x06}, {.config = 0x07}, {.config = 0xFF         }, {.config = 0x08         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0x09}, {.config = 0xFF}, {.config = 0x0A}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0x0B}, {.config = 0xFF}, {.config = 0x0C}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0xFF         }, {.config = 0x0F         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                                        //IfxGtm_AdcTrig_4
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0x04}, {.config = 0x05}, {.config = 0xFF         }, {.config = 0x06         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0x07}, {.config = 0x08}, {.config = 0xFF         }, {.config = 0x09         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0xFF         }, {.config = 0x0C         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x01}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x02}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0xFF         }, {.config = 0x0F         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x03}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF         }, {.config = 0xFF         }, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }}
    }
};

IFX_STATIC IFX_CONST IfxGtm_Trig_MscTrig_Table IfxGtm_Trig_MscTrig_table = {
    .set = {
        {.source = {                                                                                                                                                             //IfxGtm_MsccSet_1
                               //{.channel = {   {Channel_0     }, {Channel_1     }, {Channel_2     }, {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     },
                               //                {Channel_8     }, {Channel_9     }, {Channel_10    }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},

                               {.channel = {     {.config = 0x00}, {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07},
                                                 {.config = 0x08}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0x0F}}}, //IfxGtm_MscTrigSource_tom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom5
                               {.channel = {     {.config = 0x10}, {.config = 0x11}, {.config = 0x12}, {.config = 0x13}, {.config = 0x14}, {.config = 0x15}, {.config = 0x16}, {.config = 0x17},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom1
                               {.channel = {     {.config = 0x18}, {.config = 0x19}, {.config = 0x1A}, {.config = 0x1B}, {.config = 0x1C}, {.config = 0x1D}, {.config = 0x1E}, {.config = 0x1F},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom6
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom7
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom8
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom9
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom10
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom11
                           }},
        {.source = {                                                                                                                                                                               //IfxGtm_MsccSet_2
                               //{.channel = {   {Channel_0     }, {Channel_1     }, {Channel_2     }, {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     },
                               //                {Channel_8     }, {Channel_9     }, {Channel_10    }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},

                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom0
                               {.channel = {     {.config = 0x00}, {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07},
                                                 {.config = 0x08}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0x0F}}}, //IfxGtm_MscTrigSource_tom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom0
                               {.channel = {     {.config = 0x10}, {.config = 0x11}, {.config = 0x12}, {.config = 0x13}, {.config = 0x14}, {.config = 0x15}, {.config = 0x16}, {.config = 0x17},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom2
                               {.channel = {     {.config = 0x18}, {.config = 0x19}, {.config = 0x1A}, {.config = 0x1B}, {.config = 0x1C}, {.config = 0x1D}, {.config = 0x1E}, {.config = 0x1F},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom6
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom7
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom8
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom9
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom10
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom11
                           }},
        {.source = {                                                                                                                                                                               //IfxGtm_MsccSet_3
                               //{.channel = {   {Channel_0     }, {Channel_1     }, {Channel_2     }, {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     },
                               //                {Channel_8     }, {Channel_9     }, {Channel_10    }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},

                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom1
                               {.channel = {     {.config = 0x00}, {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07},
                                                 {.config = 0x08}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0x0F}}}, //IfxGtm_MscTrigSource_tom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom2
                               {.channel = {     {.config = 0x18}, {.config = 0x19}, {.config = 0x1A}, {.config = 0x1B}, {.config = 0x1C}, {.config = 0x1D}, {.config = 0x1E}, {.config = 0x1F},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom3
                               {.channel = {     {.config = 0x10}, {.config = 0x11}, {.config = 0x12}, {.config = 0x13}, {.config = 0x14}, {.config = 0x15}, {.config = 0x16}, {.config = 0x17},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom6
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom7
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom8
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom9
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom10
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom11
                           }},
        {.source = {                                                                                                                                                                               //IfxGtm_MsccSet_4
                               //{.channel = {   {Channel_0     }, {Channel_1     }, {Channel_2     }, {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     },
                               //                {Channel_8     }, {Channel_9     }, {Channel_10    }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},

                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom5
                               {.channel = {     {.config = 0x00}, {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom0
                               {.channel = {     {.config = 0x08}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0x0F},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom1
                               {.channel = {     {.config = 0x10}, {.config = 0x11}, {.config = 0x12}, {.config = 0x13}, {.config = 0x14}, {.config = 0x15}, {.config = 0x16}, {.config = 0x17},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom4
                               {.channel = {     {.config = 0x18}, {.config = 0x19}, {.config = 0x1A}, {.config = 0x1B}, {.config = 0x1C}, {.config = 0x1D}, {.config = 0x1E}, {.config = 0x1F},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom6
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom7
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom8
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom9
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom10
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom11
                           }},
        {.source = {                                                                                                                                                                               //IfxGtm_MsccSet_5
                               //{.channel = {   {Channel_0     }, {Channel_1     }, {Channel_2     }, {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     },
                               //                {Channel_8     }, {Channel_9     }, {Channel_10    }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},

                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom2
                               {.channel = {     {.config = 0x00}, {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07},
                                                 {.config = 0x08}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0x0F}}}, //IfxGtm_MscTrigSource_tom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom4
                               {.channel = {     {.config = 0x10}, {.config = 0x11}, {.config = 0x12}, {.config = 0x13}, {.config = 0x14}, {.config = 0x15}, {.config = 0x16}, {.config = 0x17},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom5
                               {.channel = {     {.config = 0x18}, {.config = 0x19}, {.config = 0x1A}, {.config = 0x1B}, {.config = 0x1C}, {.config = 0x1D}, {.config = 0x1E}, {.config = 0x1F},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom6
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom7
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom8
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom9
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom10
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom11
                           }},
        {.source = {                                                                                                                                                                               //IfxGtm_MsccSet_6
                               //{.channel = {   {Channel_0     }, {Channel_1     }, {Channel_2     }, {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     },
                               //                {Channel_8     }, {Channel_9     }, {Channel_10    }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},

                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom3
                               {.channel = {     {.config = 0x00}, {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom5
                               {.channel = {     {.config = 0x08}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0x0F},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom6
                               {.channel = {     {.config = 0x10}, {.config = 0x11}, {.config = 0x12}, {.config = 0x13}, {.config = 0x14}, {.config = 0x15}, {.config = 0x16}, {.config = 0x17},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom7
                               {.channel = {     {.config = 0x18}, {.config = 0x19}, {.config = 0x1A}, {.config = 0x1B}, {.config = 0x1C}, {.config = 0x1D}, {.config = 0x1E}, {.config = 0x1F},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom8
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom9
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom10
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom11
                           }},
        {.source = {                                                                                                                                                                               //IfxGtm_MsccSet_7
                               //{.channel = {   {Channel_0     }, {Channel_1     }, {Channel_2     }, {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     },
                               //                {Channel_8     }, {Channel_9     }, {Channel_10    }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},

                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom3
                               {.channel = {     {.config = 0x00}, {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07},
                                                 {.config = 0x08}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0x0F}}}, //IfxGtm_MscTrigSource_tom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom4
                               {.channel = {     {.config = 0x18}, {.config = 0x19}, {.config = 0x1A}, {.config = 0x1B}, {.config = 0x1C}, {.config = 0x1D}, {.config = 0x1E}, {.config = 0x1F},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom6
                               {.channel = {     {.config = 0x10}, {.config = 0x11}, {.config = 0x12}, {.config = 0x13}, {.config = 0x14}, {.config = 0x15}, {.config = 0x16}, {.config = 0x17},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom7
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom8
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom9
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom10
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom11
                           }},
        {.source = {                                                                                                                                                                               //IfxGtm_MsccSet_8
                               //{.channel = {   {Channel_0     }, {Channel_1     }, {Channel_2     }, {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     },
                               //                {Channel_8     }, {Channel_9     }, {Channel_10    }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},

                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom4
                               {.channel = {     {.config = 0x00}, {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07},
                                                 {.config = 0x08}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0x0F}}}, //IfxGtm_MscTrigSource_tom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom6
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom7
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom8
                               {.channel = {     {.config = 0x10}, {.config = 0x11}, {.config = 0x12}, {.config = 0x13}, {.config = 0x14}, {.config = 0x15}, {.config = 0x16}, {.config = 0x17},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom9
                               {.channel = {     {.config = 0x18}, {.config = 0x19}, {.config = 0x1A}, {.config = 0x1B}, {.config = 0x1C}, {.config = 0x1D}, {.config = 0x1E}, {.config = 0x1F},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom10
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom11
                           }},
        {.source = {                                                                                                                                                                               //IfxGtm_MsccSet_9
                               //{.channel = {   {Channel_0     }, {Channel_1     }, {Channel_2     }, {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     },
                               //                {Channel_8     }, {Channel_9     }, {Channel_10    }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},

                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_tom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom0
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom1
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom2
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom3
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom4
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom5
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom6
                               {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom7
                               {.channel = {     {.config = 0x18}, {.config = 0x19}, {.config = 0x1A}, {.config = 0x1B}, {.config = 0x1C}, {.config = 0x1D}, {.config = 0x1E}, {.config = 0x1F},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom8
                               {.channel = {     {.config = 0x10}, {.config = 0x11}, {.config = 0x12}, {.config = 0x13}, {.config = 0x14}, {.config = 0x15}, {.config = 0x16}, {.config = 0x17},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom9
                               {.channel = {     {.config = 0x08}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0x0E}, {.config = 0x0F},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom10
                               {.channel = {     {.config = 0x00}, {.config = 0x01}, {.config = 0x02}, {.config = 0x03}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07},
                                                 {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_MscTrigSource_atom11
                           }},
    }
};

IFX_STATIC IFX_CONST IfxGtm_Trig_AdcTrig_Table IfxGtm_Trig_SentTrig_table12_13_14_15 = {
    .trigger = {
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_0
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0x08}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x00}, {.config = 0x01}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x02}, {.config = 0x03}, {.config = 0x0F}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_1
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0x04}, {.config = 0x05}, {.config = 0x06}, {.config = 0x07}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0x08}, {.config = 0x09}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x00}, {.config = 0x01}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x02}, {.config = 0x03}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_2
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x08}, {.config = 0x09}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0A}, {.config = 0x0B}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0C}, {.config = 0x0D}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0E}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x00}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x01}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x02}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x03}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x04}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x05}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x06}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x07}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_3
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x01}, {.config = 0x02}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x03}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x00}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }},
        {.source = {                                                                                                                                                                                                      //IfxGtm_AdcTrig_4
                  //{.channel = {     {Channel_3     }, {Channel_4     }, {Channel_5     }, {Channel_6     }, {Channel_7     }, {Channel_11    }, {Channel_12    }, {Channel_13    }, {Channel_14    }, {Channel_15    }}},
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom5
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom6
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom7
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom8
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom9
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom10
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_atom11

                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom0
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom1
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom2
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom3
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0x0F}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom4
                  {.channel = {     {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}, {.config = 0xFF}}}, //IfxGtm_AdcTrigSource_tom5
              }}
    }
};

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

boolean IfxGtm_Trig_fromEdsadc(Ifx_GTM *gtm, uint32 edsadcChannel, IfxGtm_Tim tim, IfxGtm_Tim_Ch timChannel)
{
    boolean result = __checkrange(edsadcChannel, 2, 13);

    if (result == FALSE)
    {
        // do nothing
    }
    else
    {
        uint32 mask = 0xFU << (timChannel * 4);
        __ldmst_c(&(gtm->DSADCINSEL[tim].U), mask, edsadcChannel << (timChannel * 4));
    }

    return result;
}


boolean IfxGtm_Trig_fromMsc(Ifx_GTM *gtm, uint32 msc, IfxGtm_Trig_MscTrigInput input, IfxGtm_Trig_MscTrigOutput output, IfxGtm_Trig_MscTrigSignal signal)
{
    uint32  shift  = signal * 2;
    uint32  mask   = 0x3U << shift;
    uint32  value  = output << shift;
    boolean result = TRUE;

    switch (input)
    {
    case IfxGtm_Trig_MscTrigInput_low:
        __ldmst_c(&(gtm->MSC.MSCQ[msc].INLCON.U), mask, value);
        break;
    case IfxGtm_Trig_MscTrigInput_lowExtended:
        __ldmst_c(&(gtm->MSC.MSCQ[msc].INLEXTCON.U), mask, value);
        break;
    case IfxGtm_Trig_MscTrigInput_high:
        __ldmst_c(&(gtm->MSC.MSCQ[msc].INHCON.U), mask, value);
        break;
    default: result = FALSE;
        break;
    }

    return result;
}


boolean IfxGtm_Trig_toCan(Ifx_GTM *gtm, IfxGtm_Trig_CanTrig canTrig, IfxGtm_Trig_CanDevice canDevice, IfxGtm_Trig_CanTrigSource source, IfxGtm_Trig_CanTrigChannel channel)
{
    IFX_CONST IfxGtm_Trig_CanTrig_Table *table  = NULL_PTR;
    uint8                                config;
    boolean                              result = 0;
    uint32                               shift;
    uint32                               mask;
    uint32                               value;

    switch (canTrig)
    {
    case IfxGtm_Trig_CanTrig_0:
        table = &IfxGtm_Trig_CanTrig_table0;
        break;
    case IfxGtm_Trig_CanTrig_1:
        table = &IfxGtm_Trig_CanTrig_table1;
        break;
    case IfxGtm_Trig_CanTrig_2:
    case IfxGtm_Trig_CanTrig_3:
        table = &IfxGtm_Trig_CanTrig_table2_3;
        break;
    default:
        break;
    }

    if (table != NULL_PTR)
    {
        config = table->trigger.source[source].channel[channel].config;
        result = config != 0xFF;

        switch (canDevice)
        {
        case IfxGtm_Trig_CanDevice_0:
        case IfxGtm_Trig_CanDevice_1:
            shift = (canTrig * 4) + (canDevice * 16);
            mask  = 0xFU << shift;
            value = config << shift;
            __ldmst_c(&(gtm->CANOUTSEL0.U), mask, value);
            break;
        case IfxGtm_Trig_CanDevice_2:
            shift = canTrig * 4;
            mask  = 0xFU << shift;
            value = config << shift;
            __ldmst_c(&(gtm->CANOUTSEL1.U), mask, value);
            break;
        default:
            break;
        }
    }

    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, result);

    return result;
}


boolean IfxGtm_Trig_toEVadc(Ifx_GTM *gtm, IfxGtm_Trig_AdcGroup adcGroup, IfxGtm_Trig_AdcTrig adcTrig, IfxGtm_Trig_AdcTrigSource source, IfxGtm_Trig_AdcTrigChannel channel)
{
    IFX_CONST IfxGtm_Trig_AdcTrig_Table *table = NULL_PTR;
    uint8                                config;

    boolean                              result = 0;

    switch (adcGroup)
    {
    case IfxGtm_Trig_AdcGroup_0:
    case IfxGtm_Trig_AdcGroup_1:
    case IfxGtm_Trig_AdcGroup_2:
        table = &IfxGtm_Trig_EvadcTrig_tableAdc0_1_2;
        break;
    case IfxGtm_Trig_AdcGroup_3:
    case IfxGtm_Trig_AdcGroup_4:
        table = &IfxGtm_Trig_EvadcTrig_tableAdc3_4;
        break;
    case IfxGtm_Trig_AdcGroup_5:
    case IfxGtm_Trig_AdcGroup_6:
    case IfxGtm_Trig_AdcGroup_7:
        table = &IfxGtm_Trig_EvadcTrig_tableAdc5_6_7;
        break;
    case IfxGtm_Trig_AdcGroup_8:
    case IfxGtm_Trig_AdcGroup_9:
    case IfxGtm_Trig_AdcGroup_10:
    case IfxGtm_Trig_AdcGroup_11:
        table = &IfxGtm_Trig_EvadcTrig_tableAdc8_9_10_11;
        break;
    default:
        break;
    }

    if (table != NULL_PTR)
    {
        config = table->trigger[adcTrig].source[source].channel[channel].config;
        result = config != 0xFF;

        uint32 shift = (adcGroup % 8) * 4;
        uint32 mask  = 0xFU << shift;
        uint32 value = config << shift;

        if ((adcGroup / 8) == 0)
        {
            __ldmst_c(&(gtm->ADCTRIG[adcTrig].OUT0.U), mask, value);
        }
        else
        {
            __ldmst_c(&(gtm->ADCTRIG[adcTrig].OUT1.U), mask, value);
        }
    }

    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, result);

    return result;
}


void IfxGtm_Trig_toEVadcClear(Ifx_GTM *gtm, IfxGtm_Trig_AdcGroup adcGroup, IfxGtm_Trig_AdcTrig adcTrig)
{
    uint8  config = 0x0;
    uint32 shift  = (adcGroup % 8) * 4;
    uint32 mask   = 0xFU << shift;
    uint32 value  = config << shift;

    if ((adcGroup / 8) == 0)
    {
        __ldmst_c(&(gtm->ADCTRIG[adcTrig].OUT0.U), mask, value);
    }
    else
    {
        __ldmst_c(&(gtm->ADCTRIG[adcTrig].OUT1.U), mask, value);
    }
}


void IfxGtm_Trig_toEdsadc(Ifx_GTM *gtm, uint32 edsadcChannel, IfxGtm_Trig_EdsadcTrig edsadcTrig, IfxGtm_Trig_EdsadcTrigSource sel)
{
    uint32 shift = (edsadcChannel % 8) * 4;
    uint32 mask  = 0xFU << shift;
    uint32 value = (sel % 16) << shift;

    if ((edsadcChannel / 8) == 0)
    {
        __ldmst_c(&(gtm->DSADC[edsadcTrig].OUTSEL0.U), mask, value);
    }
    else
    {
        __ldmst_c(&(gtm->DSADC[edsadcTrig].OUTSEL1.U), mask, value);
    }
}


boolean IfxGtm_Trig_toMsc(Ifx_GTM *gtm, IfxGtm_Trig_MscTrigSignal signal, IfxGtm_Trig_MscTrigSet mscSet, IfxGtm_Trig_MscTrigSource source, IfxGtm_Trig_MscTrigChannel channel)
{
    IFX_CONST IfxGtm_Trig_MscTrig_Table *table = NULL_PTR;
    table = &IfxGtm_Trig_MscTrig_table;
    uint8                                config;

    boolean                              result = 0;

    {
        config = table->set[mscSet].source[source].channel[channel].config;
        result = config != 0xFF;
        uint32 shift = (signal % 4) * 8;
        uint32 mask  = 0x1FU << shift;
        uint32 value = config << shift;

        if (mscSet < IfxGtm_Trig_MscTrigSet_count)
        {
            switch (signal / 4)
            {
            case 0:
                __ldmst_c(&(gtm->MSC.SET[mscSet].CON0.U), mask, value);
                break;
            case 1:
                __ldmst_c(&(gtm->MSC.SET[mscSet].CON1.U), mask, value);
                break;
            case 2:
                __ldmst_c(&(gtm->MSC.SET[mscSet].CON2.U), mask, value);
                break;
            case 3:
                __ldmst_c(&(gtm->MSC.SET[mscSet].CON3.U), mask, value);
                break;
            default:
                break;
            }
        }
        else
        {
            result = FALSE;
        }
    }

    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, result);

    return result;
}


void IfxGtm_Trig_toPsi5(Ifx_GTM *gtm, uint32 psi5Channel, IfxGtm_Trig_Psi5TrigSource sel)
{
    uint32 shift = (psi5Channel * 4);
    uint32 mask  = 0xFU << shift;
    uint32 value = sel << shift;
    __ldmst_c(&(gtm->PSI5OUTSEL.U), mask, value);
}


void IfxGtm_Trig_toPsi5s(Ifx_GTM *gtm, uint32 psi5sChannel, IfxGtm_Trig_Psi5sTrigSource sel)
{
    uint32 shift = (psi5sChannel * 4);
    uint32 mask  = 0xFU << shift;
    uint32 value = sel << shift;

    __ldmst_c(&(gtm->PSI5SOUTSEL.U), mask, value);
}


boolean IfxGtm_Trig_toSent(Ifx_GTM *gtm, IfxGtm_Trig_SentGroup sentGroup, IfxGtm_Trig_SentTrig sentTrig, IfxGtm_Trig_SentTrigSource source, IfxGtm_Trig_SentTrigChannel channel)
{
    IFX_CONST IfxGtm_Trig_AdcTrig_Table *table = NULL_PTR;
    uint8                                config;
    uint32                               shift, mask, value;

    boolean                              result = 0;

    switch (sentGroup)
    {
    case IfxGtm_Trig_SentGroup_0:
    case IfxGtm_Trig_SentGroup_1:
    case IfxGtm_Trig_SentGroup_2:
        table = &IfxGtm_Trig_EvadcTrig_tableAdc0_1_2;
        break;
    case IfxGtm_Trig_SentGroup_3:
    case IfxGtm_Trig_SentGroup_4:
        table = &IfxGtm_Trig_EvadcTrig_tableAdc3_4;
        break;
    case IfxGtm_Trig_SentGroup_5:
    case IfxGtm_Trig_SentGroup_6:
    case IfxGtm_Trig_SentGroup_7:
        table = &IfxGtm_Trig_EvadcTrig_tableAdc5_6_7;
        break;
    case IfxGtm_Trig_SentGroup_8:
    case IfxGtm_Trig_SentGroup_9:
    case IfxGtm_Trig_SentGroup_10:
    case IfxGtm_Trig_SentGroup_11:
        table = &IfxGtm_Trig_EvadcTrig_tableAdc8_9_10_11;
        break;
    case IfxGtm_Trig_SentGroup_12:
    case IfxGtm_Trig_SentGroup_13:
    case IfxGtm_Trig_SentGroup_14:
    case IfxGtm_Trig_SentGroup_15:
        table = &IfxGtm_Trig_SentTrig_table12_13_14_15;
        break;
    default:
        break;
    }

    if (table != NULL_PTR)
    {
        config = table->trigger[sentTrig].source[source].channel[channel].config;
        result = config != 0xFF;

        if ((sentGroup / 12) == 0) // write to EVADC registers
        {
            shift = (sentGroup % 8) * 4;
        }
        else            // write EDSADC registers
        {
            shift = (sentGroup % 12) * 4;
        }

        mask  = 0xFU << shift;
        value = config << shift;

        if ((sentGroup / 12) == 0) // write to EVADC registers
        {
            if ((sentGroup / 8) == 0)
            {
                __ldmst_c(&(gtm->ADCTRIG[sentTrig].OUT0.U), mask, value);
            }
            else
            {
                __ldmst_c(&(gtm->ADCTRIG[sentTrig].OUT1.U), mask, value);
            }
        }
        else
        {
            __ldmst_c(&(gtm->DSADC[sentTrig].OUTSEL0.U), mask, value);
        }
    }

    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, result);

    return result;
}
