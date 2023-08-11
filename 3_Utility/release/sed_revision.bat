REM point to target
SET PROJECT=%1
SET TARGET=0_Src\AppSw\Config\%PROJECT%\config_pg2_prjct_info.h

REM get revision
SET EDIT_REV=00000000%2
SET REV=%EDIT_REV:~-8,8%

REM replace revision
REM copy %TARGET% sed_tmp
sed "s/#define SW_REVISION         (0x00000000)/#define SW_REVISION         (0x%REV%)/g" %TARGET% > sed_tmp
move /Y sed_tmp %TARGET%
