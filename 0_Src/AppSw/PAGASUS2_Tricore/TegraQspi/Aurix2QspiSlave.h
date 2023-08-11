#ifndef _AURIX2QSPISLAVE_H_
#define _AURIX2QSPISLAVE_H_

#include "Configuration.h"
#include <Qspi/SpiSlave/IfxQspi_SpiSlave.h>

extern IfxQspi_SpiSlave aurix2_slave_spi3;

void A2_QspiSlave_init(void);
void A2_QspiSlave_loopback(uint16 count);
boolean A2_QspiSlave_read_write(uint32 count, uint32 *pSendData, uint32 *pRecData);

#endif /* _AURIX2QSPISLAVE_H_ */
