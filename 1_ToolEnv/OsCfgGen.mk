###############################################################################
#                                                                             #
#       Copyright (C) 2017 Infineon Technologies AG. All rights reserved.     #
#                                                                             #
#                                                                             #
#                              IMPORTANT NOTICE                               #
#                                                                             #
#                                                                             #
# Infineon Technologies AG (Infineon) is supplying this file for use          #
# exclusively with Infineon’s microcontroller products. This file can be      #
# freely distributed within development tools that are supporting such        #
# microcontroller products.                                                   #
#                                                                             #
# THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED #
# OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF          #
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.#
# INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,#
# OR CONSEQUENTIAL DAMAGES, FOR	ANY REASON WHATSOEVER.                        #
#                                                                             #
###############################################################################

-include 1_ToolEnv/0_Build/1_Config/Config.mk

B_OSCFG_GEN_TOOL?=RT-Druid

ifeq ($(strip $(B_OSCFG_GEN_TOOL)),RT-Druid)
B_RT_DRUID_TOOLS_DIR:=$(BINUTILS_PATH)/../OsTools/RT-Druid
B_RT_DRUID_OUT_DIR=0_Src/OS/config
B_RT_DRUID_TEMP_DIR=$(OUT_DIR)/RT_Druid
B_RT_DRUID_CMD=java -jar "$(B_RT_DRUID_TOOLS_DIR)/plugins/org.eclipse.equinox.launcher_1.3.0.v20120522-1813.jar"\
-data $(B_RT_DRUID_TEMP_DIR)/workspace -application com.eu.evidence.rtdruid.oil.standalone.writer 

B_ERIKA_OS_OIL_FILE=1_ToolEnv/0_Build/1_Config/CfgErikaOS.oil
B_ERIKA_OS_CONFIG_FILES=$(B_RT_DRUID_OUT_DIR)/src/eecfg.c

B_RT_DRUID_SED_OPTNS= -e '/\(^ifdef.*\)\|\(^endif.*\)\|\(^ifndef.*\)\|\(^else.*\)\|\(^export.*\)/d' \
                      -e '/\(EEBASE.*\)\|\(ERIKA_FILES.*\)\|\(^CFLAGS.*\)\|\(^ASFLAGS.*\)\|\(^LDFLAGS.*\)\|\(^LDDEPS.*\)\|\(^LIBS.*\)/d' \
                      -e '/\(^APP_SRCS.*\)\|\(APPBASE.*\)\|\(OUTBASE.*\)\|\(eval.*\)/d' \
                      -e '/\(\#.*\)/d'

all: $(B_ERIKA_OS_CONFIG_FILES)

$(B_ERIKA_OS_CONFIG_FILES): $(B_ERIKA_OS_OIL_FILE)
	@-rm -rf $@
	@mkdir -p $(@D) $(@D)/../cfg $(@D)/../inc $(B_RT_DRUID_TEMP_DIR)
	@echo Operating system configurations generated .. 
	@$(B_RT_DRUID_CMD) --inputFile $(B_ERIKA_OS_OIL_FILE) --outputDir $(B_RT_DRUID_TEMP_DIR)
	@sed $(B_RT_DRUID_SED_OPTNS) $(B_RT_DRUID_TEMP_DIR)/makefile >$(B_RT_DRUID_OUT_DIR)/cfg/cfg.mk
	@echo 'EE_SRCS= 0_Src/OS/config/src/eecfg.c' >>$(B_RT_DRUID_OUT_DIR)/cfg/cfg.mk
	@echo 'EE_INCPATH= -I./0_Src/OS/config/inc' >>$(B_RT_DRUID_OUT_DIR)/cfg/cfg.mk
	@echo '' >>$(B_RT_DRUID_OUT_DIR)/cfg/cfg.mk
	@cp $(B_RT_DRUID_TEMP_DIR)/eecfg.h $(B_RT_DRUID_OUT_DIR)/inc
	@cp $(B_RT_DRUID_TEMP_DIR)/eecfg.c $(B_RT_DRUID_OUT_DIR)/src
	@cp $(B_RT_DRUID_TEMP_DIR)/Com.c $(B_RT_DRUID_OUT_DIR)/../pkg/com/com/src
#	@-rm -rf $(B_RT_DRUID_TEMP_DIR)
	@echo ..done
else

endif


