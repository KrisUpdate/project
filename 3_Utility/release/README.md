# Automation for build project

Here provides tools to modify project config or source code, making build information can be applied to image automatically.

## Requirements
* Windows platform for executing batch script
* sed gnuwin32 (http://gnuwin32.sourceforge.net/packages/sed.htm)

## Descriptions
* Config build project
    * Modify 0_Src\AppSw\Config\Common\Project_definition.h
    * eg: set to V3NP
        ```
        cd {CODEBASE_ROOT}
        3_Utility\release\sed_project_definition.bat PROJECT_PAGASUS2
        ```

* Config to build OTA image
    * Replace macro ETHERNET_OTA_FW_TYPE from _OTA_PROD_FW to _OTA_UPDATE_FW
    * Replace lsl file to Lcf_Gnuc_Tricore_Tc_96k_downloader.lsl
    * note: only support pagasus2 currently since file path is hard coded in script
    * eg:
        ```
        cd {CODEBASE_ROOT}
        3_Utility\release\sed_ota_definition.bat 0_Src\AppSw\Config\pagasus2\config_pg2_feasures.h
        3_Utility\release\select_lsl.bat downloader
        ```

* Modify revision
    * Modify 0_Src\AppSw\Config\XXX\config_pg2_prjct_info.h
    * eg: set revision as 1.01
        ```
        cd {CODEBASE_ROOT}
        3_Utility\release\sed_revision.bat pagasus2 101
        ```

* Modify revision
    * Modify 0_Src\AppSw\Config\XXX\config_pg2_prjct_info.h
    * eg: set revision as 1.01
        ```
        cd {CODEBASE_ROOT}
        3_Utility\release\sed_revision.bat pagasus2 101
        ```

* Modify revision date
    * Modify 0_Src\AppSw\Config\XXX\config_pg2_prjct_info.h
    * eg: set today as revision date
        ```
        cd {CODEBASE_ROOT}
        3_Utility\release\sed_revision_date.bat pagasus2
        ```

* Modify SW numbers
    * Modify 0_Src\AppSw\Config\pagasus2\config_pg2_prjct_info.h
    * note: only support pagasus2 currently since file path is hard coded in script
    * eg: set SW_MAJOR,UPDATE_SW_MAJOR to 11. SW_MINOR,UPDATE_SW_MINOR to 22. SW_BUGFIX,UPDATE_SW_BUGFIX to 33
        ```
        cd {CODEBASE_ROOT}
        3_Utility\release\sed_sw_numbers.bat 11 22 33
        ```

* Print commit hash
    * replace comment "// ###COMMIT PRINT###" to print string in Cpu0_Main.c
    * eg: 
        ```
        cd {CODEBASE_ROOT}
        3_Utility\release\sed_print_commit.bat PAGASUS2_Tricore
        ```

* Use HIGHTEC complier v4.9.3.0
    * Replace scripts about compiler setup
    * eg:
        ```
        cd {CODEBASE_ROOT}
        3_Utility\release\use_hightec_v4.9.3.0\replace.bat
        ```
* Build
    * eg:
        ```
        cd {CODEBASE_ROOT}
        3_Utility\release\make_all.bat
        ```
