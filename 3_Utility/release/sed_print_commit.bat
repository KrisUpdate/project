REM point to target
SET PROJECT_DIR=%1
SET TARGET=0_Src\AppSw\%PROJECT_DIR%\Main\Cpu0_Main.c

REM get commit
FOR /F "delims=" %%i IN ('git rev-parse HEAD') DO set COMMIT=%%i
ECHO %COMMIT%

REM replace to print
sed 's+// ###PRINT COMMIT###+printf("\\\r\\\nBUILD COMMIT: %COMMIT%\\\r\\\n");+g' %TARGET% > sed_tmp
move /Y sed_tmp %TARGET%
