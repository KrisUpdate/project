REM commercial hightec declare mode_t in stat.h file
SET TARGET1=0_Src\AppSw\V31C_Tricore\TegraQspi\cdd_spitp\spi_tp.c
SET TARGET2=0_Src\AppSw\V31C_Tricore\TegraQspi\cdd_spitp\tsync.c

REM replace TARGET1
sed "s://#include <sys/stat.h>:#include <stat.h>:g" %TARGET1% > sed_tmp
move /Y sed_tmp %TARGET1%

REM replace TARGET2
sed "s://#include <sys/stat.h>:#include <stat.h>:g" %TARGET2% > sed_tmp
move /Y sed_tmp %TARGET2%