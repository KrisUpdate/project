SET TARGET=%1

sed "s/#define ETHERNET_OTA_FW_TYPE    _OTA_PROD_FW/#define ETHERNET_OTA_FW_TYPE _OTA_UPDATE_FW/g" %TARGET% > sed_tmp
move /Y sed_tmp %TARGET%
