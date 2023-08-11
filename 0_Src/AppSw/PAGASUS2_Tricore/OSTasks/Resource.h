#ifndef RESOURCE_H 
#define RESOURCE_H

#include "Ifx_Types.h"
#include "IfxStdIf_DPipe.h"

#define I2C_GET_SEMA_RETRY_COUNT  10
#define FLASH_STRTBL_SEM_WAIT_I2C                               "Semaphore Waiting: (Core%d) %s(): %u, i2c%d"ENDL
#define FLASH_STRTBL_SEM_WAIT_I2C_SIZE                          sizeof(FLASH_STRTBL_SEM_WAIT_I2C)
#define FLASH_STRTBL_SEM_WAIT_SPI                               "Semaphore Waiting: (Core%d) %s(): %u, spi%d"ENDL
#define FLASH_STRTBL_SEM_WAIT_SPI_SIZE                          sizeof(FLASH_STRTBL_SEM_WAIT_SPI)
#define FLASH_STRTBL_SEM_WAIT_ADC                               "Semaphore Waiting: (Core%d) %s(): %u, adc%d"ENDL
#define FLASH_STRTBL_SEM_WAIT_ADC_SIZE                          sizeof(FLASH_STRTBL_SEM_WAIT_ADC)
#define FLASH_STRTBL_SEM_OCCUPY_I2C                             ENDL"Semaphore Occupying: (Core%d) %s(): %u, i2c%d"ENDL
#define FLASH_STRTBL_SEM_OCCUPY_I2C_SIZE                        sizeof(FLASH_STRTBL_SEM_OCCUPY_I2C)
#define FLASH_STRTBL_SEM_OCCUPY_SPI                             ENDL"Semaphore Occupying: (Core%d) %s(): %u, spi%d"ENDL
#define FLASH_STRTBL_SEM_OCCUPY_SPI_SIZE                        sizeof(FLASH_STRTBL_SEM_OCCUPY_SPI)
#define FLASH_STRTBL_SEM_OCCUPY_ADC                             ENDL"Semaphore Occupying: (Core%d) %s(): %u, adc%d"ENDL
#define FLASH_STRTBL_SEM_OCCUPY_ADC_SIZE                        sizeof(FLASH_STRTBL_SEM_OCCUPY_ADC)


typedef enum {
SEM_I2C_0,
SEM_I2C_1,
SEM_SPI_0,
SEM_SPI_1,
SEM_SPI_2,
SEM_ADC,

SEM_SIZE
} sem_element_t;

typedef enum {
RELEASE,
SET,

SEM_STATUS_SIZE
} sem_status_t;

#define FUNC_TAKE_SEM(FUNC, SEM, STRING) \
    do { \
       sem_wait_until_get(SEM, __LINE__, STRING);\
       FUNC();  \
       sem_release(SEM);\
    } while(0)

#define FUNCS_TAKE_SEM(FUNC1, FUNC2, FUNC3, SEM) \
    do { \
       sem_wait_until_get(SEM, __LINE__, __func__);\
       FUNC1();  \
       FUNC2();  \
       FUNC3();  \
       sem_release(SEM);\
    } while(0)



// Create String Table
extern const char Flash_StrTbl_sem_wait_i2c                       [FLASH_STRTBL_SEM_WAIT_I2C_SIZE         ];
extern const char Flash_StrTbl_sem_wait_spi                       [FLASH_STRTBL_SEM_WAIT_SPI_SIZE         ];
extern const char Flash_StrTbl_sem_wait_adc                       [FLASH_STRTBL_SEM_WAIT_ADC_SIZE         ];
extern const char Flash_StrTbl_sem_occupy_i2c                     [FLASH_STRTBL_SEM_OCCUPY_I2C_SIZE       ];
extern const char Flash_StrTbl_sem_occupy_spi                     [FLASH_STRTBL_SEM_OCCUPY_SPI_SIZE       ];
extern const char Flash_StrTbl_sem_occupy_adc                     [FLASH_STRTBL_SEM_OCCUPY_ADC_SIZE       ];

void sem_init(void);
void sem_release(sem_element_t obj);
void sem_report_status(IfxStdIf_DPipe *io);
boolean sem_wait_until_get(sem_element_t obj, unsigned int __line, pchar __function);





#endif /* RESOURCE_H */

