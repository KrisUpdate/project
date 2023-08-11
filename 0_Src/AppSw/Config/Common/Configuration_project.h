#ifndef CONFIGURATION_PROJECT_H
#define CONFIGURATION_PROJECT_H

#define PROJECT_TRIBOARD      0
#define PROJECT_PAGASUS2      1
#define PROJECT_V31C          2
#define PROJECT_V3NA          3
#define PROJECT_V3AA          4

#include "Project_definition.h"

/* config the project feature */
#if PROJECT == PROJECT_TRIBOARD
#include "ConfigurationIsr_triboard.h"
#include "config_triboard_feasures.h"

#elif PROJECT == PROJECT_PAGASUS2
#include "ConfigurationIsr_pagasus2.h"
#include "config_pg2_prjct_info.h"
#include "config_pg2_feasures.h"
#include "config_pg2_pins_def.h"
#include "config_pg2_aurix2_pins_def.h"

#elif PROJECT == PROJECT_V31C
#define SPA2_HIA    1
#if SPA2_HIA == 1
#define SPA2_HIB 0
#else
#define SPA2_HIB 1
#endif
#include "ConfigurationIsr_v31c.h"
#include "config_v31c_prjct_info.h"
#include "config_v31c_feasures.h"
#include "config_v31c_pins_def.h"

#elif PROJECT == PROJECT_V3NA
#include "ConfigurationIsr_v3na.h"
#include "config_v3na_proj_info.h"
#include "config_v3na_feasures.h"
#include "config_v3na_pins_def.h"

#elif PROJECT == PROJECT_V3AA
#include "ConfigurationIsr_v3aa.h"
#include "config_v3aa_feasures.h"

#else
#error "Please define the correct project in Project_definition.h!"
#endif
#endif
