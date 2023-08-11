#include "resource.h"
#include "IfxCpu_reg.h"
#include <string.h>
#include "bsp.h"
#include "IfxCpu.h"
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "Ifx_Console.h"

#define HOLD_FUNCTION_NAME_SIZE 80
#define WAITING_TIME 1000

static volatile IfxCpu_spinLock sem[SEM_SIZE] ={
    0, /*I2C0*/
    0, /*I2c1*/
    0, /*SEM_SPI_0*/
    0, /*SEM_SPI_1*/
    0, /*SEM_SPI_2*/
    0  /*SEM_ADC*/
};

static volatile IfxCpu_Id take_sem_by_core[SEM_SIZE] ={
    0, /*I2C0*/
    0, /*I2c1*/
    0, /*SEM_SPI_0*/
    0, /*SEM_SPI_1*/
    0, /*SEM_SPI_2*/
    0  /*SEM_ADC*/
};



static char sem_hold_function[SEM_SIZE][HOLD_FUNCTION_NAME_SIZE];

// Create String Table
const char Flash_StrTbl_sem_wait_i2c                       [FLASH_STRTBL_SEM_WAIT_I2C_SIZE         ]= {FLASH_STRTBL_SEM_WAIT_I2C           };
const char Flash_StrTbl_sem_wait_spi                       [FLASH_STRTBL_SEM_WAIT_SPI_SIZE         ]= {FLASH_STRTBL_SEM_WAIT_SPI           };
const char Flash_StrTbl_sem_wait_adc                       [FLASH_STRTBL_SEM_WAIT_ADC_SIZE         ]= {FLASH_STRTBL_SEM_WAIT_ADC           };
const char Flash_StrTbl_sem_occupy_i2c                     [FLASH_STRTBL_SEM_OCCUPY_I2C_SIZE       ]= {FLASH_STRTBL_SEM_OCCUPY_I2C         };
const char Flash_StrTbl_sem_occupy_spi                     [FLASH_STRTBL_SEM_OCCUPY_SPI_SIZE       ]= {FLASH_STRTBL_SEM_OCCUPY_SPI         };
const char Flash_StrTbl_sem_occupy_adc                     [FLASH_STRTBL_SEM_OCCUPY_ADC_SIZE       ]= {FLASH_STRTBL_SEM_OCCUPY_ADC         };


#if 0
static void sem_set(sem_element_t obj, IfxCpu_Id core)
{
    sem[obj] = SET;
    take_sem_by_core[obj] = core;
}

static boolean sem_check(sem_element_t obj)
{
    return sem[obj];
}
#endif

//=======================================================================
void sem_release(sem_element_t obj)
{
    IfxCpu_resetSpinLock(&sem[obj]);
    take_sem_by_core[obj] = 0;
}

void sem_init(void)
{
    memset(sem, 0, 10);
}

void sem_report_status(IfxStdIf_DPipe *io)
{
    #if 0
    int i = 0;
    IfxStdIf_DPipe_print(io, "%s()" ENDL, i2c0_hold_function);
    IfxStdIf_DPipe_print(io, "%s()" ENDL, i2c1_hold_function);
    IfxStdIf_DPipe_print(io, "sem:");
    for(i = 0; i < SEM_SIZE; i++){
        IfxStdIf_DPipe_print(io, "%d:%d ", sem[i], take_sem_by_core[i]);
    }
    IfxStdIf_DPipe_print(io, ENDL);
   
    #else
    
    int i;

    for(i = 0; i < SEM_SIZE; i++)
        if(sem[i] == SET)
            IfxStdIf_DPipe_print(io, "%s"ENDL, sem_hold_function[i]);
        else
            IfxStdIf_DPipe_print(io, "sem%d is release"ENDL, i);
    #endif  // if 0
}

boolean sem_wait_until_get(sem_element_t obj, unsigned int __line, pchar __function)
{
    #if 0
    volatile uint32 count = 0;    
    IfxCpu_spinLock resourceLock;
    boolean flag = FALSE;

    do{
        flag = IfxCpu_setSpinLock(&sem[obj], 0xFFFF);
        count++;
        if(count % WAITING_TIME == 0){
            Ifx_Console_print("(Core%d) %s(): %u" ENDL, IfxCpu_getCoreId(), __function, __line);
            count = 0;
        }
    }while(flag == FALSE);
    //from here, critical section for the assigned resource
    take_sem_by_core[obj] = IfxCpu_getCoreId();
    if(obj == SEM_I2C_0 || obj == SEM_I2C_1){
        if(IfxCpu_getCoreId() == 0){
            snprintf(i2c0_hold_function, HOLD_FUNCTION_NAME_SIZE,"Core%d:i2c%d:%s", IfxCpu_getCoreId(), (int)obj,__function);
        }
        if(IfxCpu_getCoreId() == 3){
            snprintf(i2c1_hold_function, HOLD_FUNCTION_NAME_SIZE,"Core%d:i2c%d:%s", IfxCpu_getCoreId(), (int)obj,__function);
        }
    }

    #else     // Keep occupy information

    volatile uint32 count = 0;    
    boolean flag = FALSE;
    uint8 ubTmp;

    ubTmp= IfxCpu_getCoreId();

    // If there is occupy resource, keep waiting and dump information
    do{
        flag = IfxCpu_setSpinLock(&sem[obj], 0xFFFF);
        count++;
        //if(count % WAITING_TIME == 0){
        if(count >= WAITING_TIME)
        {
            Ifx_Console_print(sem_hold_function[obj]);

            if((obj == SEM_I2C_0) || (obj == SEM_I2C_1))
                Ifx_Console_print(Flash_StrTbl_sem_wait_i2c, ubTmp, __function, __line, obj);
            else if((obj == SEM_SPI_0) || (obj == SEM_SPI_1) || (obj == SEM_SPI_2))
                Ifx_Console_print(Flash_StrTbl_sem_wait_spi, ubTmp, __function, __line, obj);
            else if(obj == SEM_ADC)
                Ifx_Console_print(Flash_StrTbl_sem_wait_adc, ubTmp, __function, __line, obj);

            count = 0;
        }
    }while(flag == FALSE);

    // Save the occupy resource information
    take_sem_by_core[obj] = ubTmp;

    if((obj == SEM_I2C_0) || (obj == SEM_I2C_1))
        snprintf(sem_hold_function[obj], HOLD_FUNCTION_NAME_SIZE, Flash_StrTbl_sem_occupy_i2c, ubTmp, __function, __line, (int)obj);
    else if((obj == SEM_SPI_0) || (obj == SEM_SPI_1) || (obj == SEM_SPI_2))
        snprintf(sem_hold_function[obj], HOLD_FUNCTION_NAME_SIZE, Flash_StrTbl_sem_occupy_spi, ubTmp, __function, __line, (int)obj);
    else if(obj == SEM_ADC)
        snprintf(sem_hold_function[obj], HOLD_FUNCTION_NAME_SIZE, Flash_StrTbl_sem_occupy_adc, ubTmp, __function, __line, (int)obj);

    #endif    // if 0
    
    return TRUE;
}

#if 0
boolean sem_wait_until_get(sem_element_t obj)
{
  volatile uint32 count = 0;
  
  while(sem_check(obj)){
      waitTime(TimeConst[TIMER_INDEX_1MS]);
      count++;
      if(count % 1000 == 0){
         Ifx_Console_print("wait sem %d, core %d\r\n", obj, IfxCpu_getCoreId());//,__assertion, __line);
      }
  }
  sem_set(obj);
  return TRUE;
}



boolean sem_wait(sem_element_t obj,uint8 timeout)
{
  uint32 count = 0;

  while(sem_check(obj)){
      waitTime(TimeConst[TIMER_INDEX_1MS]);
      if(count++ > timeout){
          IfxStdIf_DPipe_print(io_tmp, ENDL"Core%d, wait %d, No %d resource"ENDL,IfxCpu_getCoreId(), count, obj);
          return FALSE;
      }
  }
  sem_set(obj);
  return TRUE;
}
#endif



