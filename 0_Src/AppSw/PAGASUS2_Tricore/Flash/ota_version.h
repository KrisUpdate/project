#define AURIX_VERSION_PROJECT "DRIVE-v5.1.3-"
#define AURIX_VERSION_BOARD_E3550 "E3550-" //For IFW since there is only 1 FW for different boards, this should be read again at runtime
#define AURIX_VERSION_BOARD_P3570 "P3570-" //For IFW since there is only 1 FW for different boards, this should be read again at runtime
#define AURIX_VERSION_BOARD_P3479 "P3479-" //For IFW since there is only 1 FW for different boards, this should be read again at runtime

#if ETHERNET_OTA_FW_TYPE == _OTA_PROD_FW
  #ifdef HYPERION
    #define AURIX_VERSION_TYPE "NV-Aurix-IFW-ForHyperion-"
  #else
    #define AURIX_VERSION_TYPE "NV-Aurix-IFW-"
  #endif
#else
  #ifdef HYPERION
    #define AURIX_VERSION_TYPE "NV-Aurix-UPDATE-ForHyperion-"
  #else
    #define AURIX_VERSION_TYPE "NV-Aurix-UPDATE-"
  #endif
#endif

#define CHIPREV_STEP_B     "StepB-"
#define CHIPREV_STEP_A     "StepA-"

/* creating SW version from the MAJOR and MINOR versions existing in configuration.h*/
// this should be similar to - #define SW_VERSION  "1.30.00"
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x) // two level of preprocessor directive
// required to replace the macro with the actual value while concatenating the strings

#define CREATE_SW_VERSION(X,Y,Z) STR(X) "." STR(Y) "." STR(Z)
