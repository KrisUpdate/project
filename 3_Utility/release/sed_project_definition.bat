REM point to target
SET PROJECT_DEF=%1
SET TARGET=0_Src\AppSw\Config\Common\Project_definition.h

sed "s/PROJECT_TRIBOARD/%PROJECT_DEF%/g" %TARGET% > sed_tmp
move /Y sed_tmp %TARGET%
