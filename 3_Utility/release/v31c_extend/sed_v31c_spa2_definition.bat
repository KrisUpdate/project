SET TARGET=0_Src\AppSw\Config\Common\Configuration_project.h

IF "%1"=="HIB" (
  sed "s/#define SPA2_HIA    1/#define SPA2_HIA    0/g" %TARGET% > sed_tmp
  move /Y sed_tmp %TARGET%
)
