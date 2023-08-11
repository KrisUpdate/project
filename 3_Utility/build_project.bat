@echo on

set TRICORE_TOOLS=C:\HIGHTEC\toolchains\tricore\v4.9.1.0-infineon-2.0
set BINUTILS_PATH=C:\Tools\BifacesWin64\bin

if not exist "%TRICORE_TOOLS%" goto missing_TRICORE_TOOLS

set EXIT_CODE=0

:: check for command line parameters
if [%2]==[clean] goto call_clean
if [%1]==[make] goto call_build


:call_build
echo invoke command and terminate
cmd /c %TRICORE_TOOLS%\bin\make.exe -j8 all
set EXIT_CODE=%ERRORLEVEL%
goto finish

:call_clean
:: invoke command and terminate
cmd /c %TRICORE_TOOLS%\bin\make.exe -j8 clean
set EXIT_CODE=%ERRORLEVEL%
goto finish

:missing_TRICORE_TOOLS
echo.
echo    The variable TRICORE_TOOLS was not set correctly:
echo    %TRICORE_TOOLS%.
echo    Please specify this variable
echo.
set EXIT_CODE=1
if [%1]==[] pause
goto :finish


:finish
exit /B %EXIT_CODE%
