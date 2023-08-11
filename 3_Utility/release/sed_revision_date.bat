REM point to target
@SET PROJECT=%1
@SET TARGET=0_Src\AppSw\Config\%PROJECT%\config_pg2_prjct_info.h

REM get time info
for /f "skip=1" %%x in ('wmic os get localdatetime') do if not defined MyDate set MyDate=%%x
SET TODAY=%MyDate:~0,4%%MyDate:~4,2%%MyDate:~6,2%

REM replace revision date
REM copy %TARGET% sed_tmp
sed "s/#define SW_REVISION_DATE    (0x00000000)/#define SW_REVISION_DATE    (0x%TODAY%)/g" %TARGET% > sed_tmp
move /Y sed_tmp %TARGET%