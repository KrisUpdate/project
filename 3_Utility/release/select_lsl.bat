REM default to build full image, "downloader" to build downdoader binary
SET OPTION=%1

SET CONF_DIR="1_ToolEnv\0_Build\1_Config\Config_Tricore_Gnuc"

SET CONF_FILE="Lcf_Gnuc_Tricore_Tc_240k_normal.lsl"
IF "%OPTION%"=="downloader" (
    SET CONF_FILE="Lcf_Gnuc_Tricore_Tc_96k_downloader.lsl"
)

copy /Y "%CONF_DIR%\%CONF_FILE%.*" "%CONF_DIR%\Lcf_Gnuc_Tricore_Tc.lsl" 