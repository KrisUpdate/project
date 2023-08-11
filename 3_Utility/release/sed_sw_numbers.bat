SET SW_MAJOR=%1
SET SW_MINOR=%2
SET SW_BUGFIX=%3
SET TARGET="0_Src\AppSw\Config\pagasus2\config_pg2_prjct_info.h"

sed 's/SW_MAJOR                99/SW_MAJOR %SW_MAJOR%/g;s/SW_MINOR                99/SW_MINOR %SW_MINOR%/g;s/SW_BUGFIX               99/SW_BUGFIX %SW_BUGFIX%/g' %TARGET% > sed_tmp
move /Y sed_tmp %TARGET%

sed 's/UPDATE_SW_MAJOR         99/UPDATE_SW_MAJOR %SW_MAJOR%/g;s/UPDATE_SW_MINOR         99/UPDATE_SW_MINOR %SW_MINOR%/g;s/UPDATE_SW_BUGFIX        99/UPDATE_SW_BUGFIX %SW_BUGFIX%/g' %TARGET% > sed_tmp
move /Y sed_tmp %TARGET%