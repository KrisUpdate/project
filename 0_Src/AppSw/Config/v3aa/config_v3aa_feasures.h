#ifndef CONFIG_V3AA_FEATURE
#define CONFIG_V3AA_FEATURE

//#define TC397B

#define _OTA_PROD_FW                    0x01
#define _OTA_UPDATE_FW                0x02
#define ETHERNET_OTA_FW_TYPE    _OTA_PROD_FW

#define _DEFAULT                             0x01
#define _QPARTITION                        0x02
#define PARTITION_LAYOUT              _DEFAULT

#define CFG_ASC_BAUDRATE       (115200.0)                   /**< \brief Define the Baudrate */
#define CFG_ASC_RX_BUFFER_SIZE (512)                        /**< \brief Define the Rx buffer size in byte. */
#define CFG_ASC_TX_BUFFER_SIZE (6 * 1024)                   /**< \brief Define the Tx buffer size in byte. */
#define FEATURE_AVB			0
#if FEATURE_AVB == 1
#define FEATURE_SRP             1
#define FEATURE_1722            1
#define FEATURE_PTP             1
#endif // endif #if FEATURE_AVB  ==1

#define V3AA_BRINGUP 1    /* If you would like to use triboard, please set 0 */
#define V3AA_CANBUS 1     /* If you would like to use triboard, please set 0 */
#define V3NP_COMPLIANCE 0    /* If you would like to use v3np, please set 1 */


#define USE_TRIBOARD_ETH 1
#if USE_TRIBOARD_ETH
#define USE_TRIBOARD_LED 0
#endif

#define FEATURE_MARVELL_6321           0
#define FEATURE_ERAY_TESTMODE           0

#define IFXETH_SWITCH_88Q6113_ADDR     0x3
#define IFXETH_SWITCH_88Q5072_ADDR     0x2

#endif
