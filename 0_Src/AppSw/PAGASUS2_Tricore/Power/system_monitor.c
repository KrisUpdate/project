#include "system_monitor.h"
#include "internal_fw_debug.h"
#include "EvadcAutoScan.h"
#include "Gpio.h"
#include "string.h"
#include "resource.h"
#include "i2crom_drv.h"
#include "I2c.h"

#define SYSTEM_MONITOR_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)

boolean g_PowerDown_flag = FALSE;

typedef struct
{
    uint32 low_threshold_mv; // the low threshold that Aurix will trigger power down
    uint32 low_filter_sample;    // the filter sample for low threshold
    uint32 high_threshold_mv;// the high threshold that Aurix will trigger power up
    uint32 high_filter_ms;    // the filter time for low threshold
}t_monitor_parameter;

typedef enum
{
    HEALTHY,
    POWERING_DOWN,
    BAD,
}e_kl30_state;

static t_monitor_parameter kl30_para=
{
    6500, // low threshold 5600mV
    3,    // 3 sample
    8800, // high threshold 8000mV
    100,   //  5ms
};

static e_kl30_state kl30_state = HEALTHY;
volatile uint32 adc_isr_count = 0;
static uint32 power_on_delay_count = 0;
#if USE_WC_PATTERN == 1
boolean GLOBAL_RUN_WC_PATTERN_TEST = FALSE;
sint32 GLOBAL_WC_PATTERN_TIME_IN_SEC = 1;
boolean GLOBAL_WC_PATTERN_CPU0 = FALSE;
boolean GLOBAL_WC_PATTERN_CPU1 = FALSE;
boolean GLOBAL_WC_PATTERN_CPU2 = FALSE;
boolean GLOBAL_WC_PATTERN_CPU3 = FALSE;
boolean GLOBAL_WC_PATTERN_CPU4 = FALSE;
boolean GLOBAL_WC_PATTERN_CPU5 = FALSE;
#endif
IFX_INTERRUPT(ISR_adc_channel, 0, ISR_PRIORITY_ADC_CHANNEL);

void ISR_adc_channel(void)
{
    IfxCpu_enableInterrupts();
    switch(kl30_state)
    {
    case HEALTHY:
        adc_isr_count++;
        if(check_backup_power() == NO_BACKUP_POWER && adc_isr_count >= kl30_para.low_filter_sample)
        {
            IfxPort_setPinLow(VBAT_TEG_PWR_ON.port, VBAT_TEG_PWR_ON.pinIndex);
            IfxPort_setPinLow(VBAT_SYS_PWR_ON.port, VBAT_SYS_PWR_ON.pinIndex);

            IfxPort_setPinLow(X1_PMIC_POWER_ON_R.port, X1_PMIC_POWER_ON_R.pinIndex);
            IfxPort_setPinLow(X2_PMIC_POWER_ON_R.port, X2_PMIC_POWER_ON_R.pinIndex);
            // disable boundary and we monitor the power up in loops only
            VadcAutoScan_set_kl30_low_thres(0);
            kl30_state = POWERING_DOWN;
        }
        break;
    case POWERING_DOWN:
    case BAD:
    default:
        break;
    }
    VadcAutoScan_clear_kl30_interrupt_flag();
}

static void System_Monitor_Config(t_monitor_parameter* para_ptr)
{
    memcpy(&kl30_para, para_ptr, sizeof(t_monitor_parameter));
    SYSTEM_MONITOR_ALWAYS_PRINTF(("set low threshold to %d mv, filter sample %d\r\n"
            "set high threshold to %d mv, filter time %d ms\r\n",
            kl30_para.low_threshold_mv, kl30_para.low_filter_sample,
            kl30_para.high_threshold_mv, kl30_para.high_filter_ms));

    VadcAutoScan_set_kl30_low_thres(kl30_para.low_threshold_mv);
}

boolean System_Monitor_Config_Cmd(pchar args, void *data, IfxStdIf_DPipe *io)
{
    t_monitor_parameter input;
    uint32 value;
    (void)data;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : kl30config x1 t1 x2 t2"ENDL);
        IfxStdIf_DPipe_print(io, "           > config the parameter for kl30 voltage monitor"ENDL);
        IfxStdIf_DPipe_print(io, "           > x1: low threshold voltage in mV "ENDL);
        IfxStdIf_DPipe_print(io, "           > t1: low threshold filter sample "ENDL);
        IfxStdIf_DPipe_print(io, "           > x2: high threshold voltage in mV "ENDL);
        IfxStdIf_DPipe_print(io, "           > t2: high threshold filter time in ms "ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid low threshold voltage %s"ENDL, args);
            return TRUE;
        }
        input.low_threshold_mv = value;

        if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid low threshold filter sample %s"ENDL, args);
            return TRUE;
        }
        input.low_filter_sample = value;

        if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid high threshold voltage %s"ENDL, args);
            return TRUE;
        }
        input.high_threshold_mv = value;

        if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid high threshold filter time %s"ENDL, args);
            return TRUE;
        }
        input.high_filter_ms = value;

        System_Monitor_Config(&input);
    }

    return TRUE;
}

backup_power_state backup_power_up = 0;
unsigned char check_backup_power(void)
{
    return backup_power_up;
}

void set_backup_power(backup_power_state flag)
{
    backup_power_up = flag;
}


void System_Monitor_KL30_Init(void)
{
    VadcAutoScan_set_kl30_low_thres(kl30_para.low_threshold_mv);
    power_on_delay_count = 0;
    adc_isr_count = 0;
    kl30_state = HEALTHY;
}

/* This is called every 1ms */
void System_Monitor_KL30_Routine(void)
{
    volatile uint32 kl30bat1_mv;
    volatile uint32 kl30bat2_mv;
    volatile uint8 vbat2_filter_sample;
    uint8 i;

    kl30bat2_mv = VadcAutoScan_getVoltage_mV(10)*(49.9+10)/10;
    kl30bat1_mv = VadcAutoScan_getVoltage_mV(0)*(49.9+10)/10;

    switch(kl30_state)
    {
    case HEALTHY:
        if(kl30bat2_mv > kl30_para.low_threshold_mv){
            set_backup_power(BACKUP_POWER);
        }else{
            //turn the debug message on to check if backup is abnormal
            for(i = 0; i < 10; i++){
                kl30bat2_mv = VadcAutoScan_getVoltage_mV(10)*(49.9+10)/10;
                if(kl30bat2_mv < kl30_para.low_threshold_mv)
                {
                    vbat2_filter_sample++;
                    //SYSTEM_MONITOR_ALWAYS_PRINTF(("\r\n , %d:current VBAT1: %d, VBAT2: %d\r\n", bat2_filter_sample, kl30bat1_mv, kl30bat2_mv));
 
                }else{
                    vbat2_filter_sample = 0;
                    break;
                } 
            }
            if(vbat2_filter_sample == 10){
                set_backup_power(NO_BACKUP_POWER);
                vbat2_filter_sample = 0;
            }
        }
        adc_isr_count = 0;
        break;
    case POWERING_DOWN:
        // handle the power off here
        g_PowerDown_flag = TRUE;
        System_PowerOff();
        kl30bat1_mv = VadcAutoScan_getVoltage_mV(0)*(49.9+10)/10;
        kl30bat2_mv = VadcAutoScan_getVoltage_mV(10)*(49.9+10)/10;
        SYSTEM_MONITOR_ALWAYS_PRINTF(("\r\n Detect KL30 voltage go below %d mV for %d sample, current VBAT1: %d, VBAT2: %d\r\n",
                                       kl30_para.low_threshold_mv, adc_isr_count, kl30bat1_mv, kl30bat2_mv));
        SYSTEM_MONITOR_ALWAYS_PRINTF(("System is powered off successfully because of KL30 goes down\r\n"));

        kl30_state = BAD;
        adc_isr_count = 0;
        break;
    case BAD:
        kl30bat1_mv = VadcAutoScan_getVoltage_mV(0)*(49.9+10)/10;
        kl30bat2_mv = VadcAutoScan_getVoltage_mV(10)*(49.9+10)/10;
        if(kl30bat1_mv == 0 && kl30bat2_mv == 0)
        {
            return;
        }

        if(kl30bat1_mv >= kl30_para.high_threshold_mv || kl30bat2_mv >= kl30_para.high_threshold_mv)
        {
//            SYSTEM_MONITOR_ALWAYS_PRINTF(("\r\n , > current VBAT1: %d, VBAT2: %d, power_on_delay_count=%d\r\n", kl30bat1_mv, kl30bat2_mv, power_on_delay_count));
            power_on_delay_count++;
        }
        else
        {
            //kl30bat2_mv need to be double checked, due to sometimes it will be 0
            if(kl30bat2_mv == 0){
                for(i = 0; i < 7; i++){
                    kl30bat2_mv = VadcAutoScan_getVoltage_mV(10)*(49.9+10)/10;
                    if(kl30bat2_mv > kl30_para.high_threshold_mv)
                    {
                        power_on_delay_count++;
                        waitTime(3*TimeConst[TIMER_INDEX_1MS]);
//                        SYSTEM_MONITOR_ALWAYS_PRINTF(("\r\n , 1 double check:current VBAT1: %d, VBAT2: %d, power_on_delay_count: %d\r\n", kl30bat1_mv, kl30bat2_mv, power_on_delay_count)); 
                        return;                        
                    } 
                }
            }

            //kl30bat1_mv need to be double checked, due to sometimes it will be 0
            if(kl30bat1_mv == 0){
                for(i = 0; i < 7; i++){
                    kl30bat1_mv = VadcAutoScan_getVoltage_mV(0)*(49.9+10)/10;
                    if(kl30bat1_mv > kl30_para.high_threshold_mv)
                    {
                        power_on_delay_count++;
                        //SYSTEM_MONITOR_ALWAYS_PRINTF(("\r\n , 2:V1: %d, V2: %d, cnt: %d\r\n", kl30bat1_mv, kl30bat2_mv, power_on_delay_count)); 
                        waitTime(3*TimeConst[TIMER_INDEX_1MS]);                        
                        return;                        
                    }else{
                        if(kl30bat1_mv == 0){
//                            SYSTEM_MONITOR_ALWAYS_PRINTF(("\r\n , 3 V1: %d, V2: %d\r\n", kl30bat1_mv, kl30bat2_mv)); 
                           waitTime(3*TimeConst[TIMER_INDEX_1MS]);                        
                        }
                    }                    
                }
            }

            if(power_on_delay_count != 0){
                SYSTEM_MONITOR_ALWAYS_PRINTF(("\r\n , < V1: %d, V2: %d, cnt=%d\r\n", kl30bat1_mv, kl30bat2_mv, power_on_delay_count));
            }
            power_on_delay_count = 0;
        }

        if(power_on_delay_count >= kl30_para.high_filter_ms)
        {
            SYSTEM_MONITOR_ALWAYS_PRINTF(("kl30_para.high_filter_ms = %d times\r\n", kl30_para.high_filter_ms));
            System_PowerOn(TRUE);
            SYSTEM_MONITOR_ALWAYS_PRINTF(("\r\n Detect KL30_VBAT1 voltage is above %d mV for %d ms, current value is %d mv\r\n",
                                           kl30_para.high_threshold_mv, kl30_para.high_filter_ms, kl30bat1_mv));
            SYSTEM_MONITOR_ALWAYS_PRINTF(("\r\n Detect KL30_VBAT2 voltage is above %d mV for %d ms, current value is %d mv\r\n",
                                           kl30_para.high_threshold_mv, kl30_para.high_filter_ms, kl30bat2_mv));

            SYSTEM_MONITOR_ALWAYS_PRINTF(("Power up the system as KL30 is up and stable\r\n"));

            VadcAutoScan_set_kl30_low_thres(kl30_para.low_threshold_mv);
            power_on_delay_count = 0;
            adc_isr_count = 0;
            kl30_state = HEALTHY;
        }
        break;
    default:
        break;
    }

}
#if USE_WC_PATTERN == 1
// Worst Case Pattern with a high IPC Rate, can be executed by each CPU.
IFX_EXTERN boolean Start_WC_Pattern(pchar args, void *data, IfxStdIf_DPipe *io)
{
    (void)data;
    uint32 value = 1;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : wc_pattern no_of_seconds"ENDL);
        IfxStdIf_DPipe_print(io, "           > Starts Worst Case Pattern with a high IPC Rate on all CPUs for approximate number of seconds"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
        {
            GLOBAL_RUN_WC_PATTERN_TEST = FALSE;
            IfxStdIf_DPipe_print(io, "Unrecognized option"ENDL);
            IfxStdIf_DPipe_print(io, "Syntax     : wc_pattern no_of_seconds"ENDL);
            IfxStdIf_DPipe_print(io, "           > Starts Worst Case Pattern with a high IPC Rate on all CPUs for approximate number of seconds"ENDL);
            IfxStdIf_DPipe_print(io, "Command Fail "ENDL);;
            return TRUE;
        }
        IfxStdIf_DPipe_print(io, "Debug     : no_of_seconds = %ld "ENDL, value);
        GLOBAL_WC_PATTERN_TIME_IN_SEC = value;
        GLOBAL_RUN_WC_PATTERN_TEST = TRUE;
        GLOBAL_WC_PATTERN_CPU5 = GLOBAL_WC_PATTERN_CPU4 = GLOBAL_WC_PATTERN_CPU3 = GLOBAL_WC_PATTERN_CPU2 = TRUE;
        GLOBAL_WC_PATTERN_CPU1 = GLOBAL_WC_PATTERN_CPU0 = TRUE;
        IfxStdIf_DPipe_print(io, "Command Executed"ENDL);
    }
    return TRUE;
}
#endif

static boolean System_Monitor_KL15_check(void)
{
    boolean result;
    uint32 kl15_mv;

    kl15_mv = VadcAutoScan_getVoltage_mV(2)*(49.9+10)/10;
    if(kl15_mv > 8500){
        result = TRUE;
    }
    else if(kl15_mv < 500){
        result = FALSE;
    }
    return result;
}


static boolean ignition_booting_status_check(void)
{
    boolean result;
    uint8 bytesleft;
    uint8 inforom_ignition_booting_status;

    // no need to have semaphore because other cores is not ready
    //bytesleft = i2c_read_bytes(INFOROM_I2C_NUM, INFOROM_DEV_ADDR, INFOROM_ADDR_TBL_IGNCHKFG, 1, , 1);
    bytesleft = IgnitionChkEngage_ReadValue(&inforom_ignition_booting_status);
    if(!bytesleft)
    {
        //successful
        Ifx_Console_print("The Ignition Check Feature Enablement Setting is = %d."ENDL, inforom_ignition_booting_status);
    }
    else
    {
        //failed
        Ifx_Console_print("Read value is failed."ENDL);
    }


    if(inforom_ignition_booting_status == TRUE){
        result = TRUE;
    }
    else{
        result = FALSE;
    }
    return result;
}

void booting_system(void)
{
    uint32 i = 0;

    if(ignition_booting_status_check() == TRUE){
        //Blocking system until ignition is on
        while(System_Monitor_KL15_check() == FALSE){
            i++;
            waitTime(3*TimeConst[TIMER_INDEX_1MS]);
            if(i % 1000 == 0){
                Ifx_Console_print("Waiting for ignition" ENDL);
                i = 0;
            }
        }        
    }
    System_PowerOn(FALSE);
}
