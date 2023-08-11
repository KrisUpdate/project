#ifndef CONFIG_TRIBOARD_FEATURE
#define CONFIG_TRIBOARD_FEATURE

#define CFG_ASC_BAUDRATE       (115200.0)                   /**< \brief Define the Baudrate */
#define CFG_ASC_RX_BUFFER_SIZE (512)                        /**< \brief Define the Rx buffer size in byte. */
#define CFG_ASC_TX_BUFFER_SIZE (6 * 1024)                   /**< \brief Define the Tx buffer size in byte. */
#define FEATURE_AVB			1
#if FEATURE_AVB == 1
#define FEATURE_SRP             1
#define FEATURE_1722            1
#define FEATURE_PTP             1
#endif // endif #if FEATURE_AVB  ==1
#define FEATURE_MARVELL_6321           0
#define FEATURE_ERAY_TESTMODE           0
#endif
