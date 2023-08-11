SET TOOLCHAIN=%1

SET TARGET1=1_ToolEnv\0_Build\1_Config\Config_Tricore_Gnuc\Config_Gnuc.mk
SET TARGET2=1_ToolEnv\0_Build\1_Config\Config_Tricore_Gnuc_QPartition\Config_Gnuc.mk
SET TARGET3=3_Utility\build_project.bat


sed "s/v4.9.1.0-infineon-2.0/%TOOLCHAIN%/g" %TARGET1% > sed_tmp
move /Y sed_tmp %TARGET1%

sed "s/v4.9.1.0-infineon-2.0/%TOOLCHAIN%/g" %TARGET2% > sed_tmp
move /Y sed_tmp %TARGET2%

sed "s/v4.9.1.0-infineon-2.0/%TOOLCHAIN%/g" %TARGET3% > sed_tmp
move /Y sed_tmp %TARGET3%
