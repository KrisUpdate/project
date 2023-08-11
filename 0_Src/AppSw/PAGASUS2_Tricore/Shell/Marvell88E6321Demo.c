#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "IfxGeth_Eth.h"
#include "Ifx_Lwip.h"
#include "msdApiTypes.h"
#include "msdSysConfig.h"
#include "Marvell88E6321Demo.h"
#include "Pearl_msdDrvSwRegs.h"
#include "Willow_msdDrvSwRegs.h"

extern boolean receive_rmu;
extern Ifx_Lwip g_Lwip;
rmu_payload RMU_RX_QUEUE = {0};

int rmu_rx_queue_enqueue(pbuf_t* pbuf) {
	if (pbuf->len > 128) {
		return;
	}
	memcpy(RMU_RX_QUEUE.buf, pbuf->payload + 2, pbuf->len - 2);
	RMU_RX_QUEUE.len = pbuf->len - 2;
	return 0;
}

MSD_STATUS msdBspRmuTxRx
(
	MSD_U8 *req_packet,
	MSD_U32 req_pktlen,
	MSD_U8 **rsp_packet,
	MSD_U32 *rsp_pktlen
)
{
  unsigned payload_size = req_pktlen+2;
  int timeoutCount = 5000;
  err_t err_code = ERR_ARG;
  int i;

  struct pbuf* pbuf = pbuf_alloc(PBUF_LINK, payload_size ,PBUF_RAM);
  pbuf->len = pbuf->tot_len = payload_size;

  for(i=0; i<req_pktlen; i++)
  {
    Ifx_Console_print("0x%X ", req_packet[i]);
  }
  Ifx_Console_print("\r\n");

  memcpy(pbuf->payload+2, ((uint8_t *)req_packet) , payload_size-2);

  receive_rmu = FALSE;

  err_code = ethernet_output_RMU (&g_Lwip.netif, pbuf);

   while (timeoutCount!= 0)
   {
     ifx_netif_input(&g_Lwip.netif);
     if(receive_rmu)
       break;
	timeoutCount--;
     };

  if(receive_rmu==FALSE){
    Ifx_Console_print("QQ receive RMU timeout %d\n\r", err_code);
    pbuf_free(pbuf);
    pbuf = NULL;
    return MSD_FAIL;
  }else{
    Ifx_Console_print("receive RMU ok timeoutCount = %d\n\r",timeoutCount);
    memcpy(*rsp_packet,RMU_RX_QUEUE.buf,RMU_RX_QUEUE.len);
    *rsp_pktlen = RMU_RX_QUEUE.len;
    pbuf_free(pbuf);
    pbuf = NULL;
    return MSD_OK;
  }
}


MSD_STATUS init_88EA6321_RMU(MSD_U8 devNum)
{
	MSD_STATUS status;
	MSD_SYS_CONFIG   cfg;
	MSD_QD_DEV       *dev;
       MSD_U16 id=0;
	
	/* Check if device number our of range, it should be [0,31] */
	if (devNum > 31)
	{
		Ifx_Console_print("devNum %d is our of range, larger than 31. \n", devNum);
		return MSD_FAIL;
	}

	/*
	*  Register all the required functions to Driver.
	*/
	msdMemSet((char*)&cfg, 0, sizeof(MSD_SYS_CONFIG));

	cfg.InterfaceChannel = MSD_INTERFACE_RMU;
	cfg.devNum = devNum ;/* Device Number, 0~31 */
	cfg.rmuMode = MSD_RMU_ETHERT_TYPE_DSA_MODE; /* RMU mode*/
	cfg.eTypeValue = 0x9100;                    /* Ether Type value*/

	cfg.BSPFunctions.readMii = NULL;
	cfg.BSPFunctions.writeMii = NULL;

	cfg.BSPFunctions.rmu_tx_rx = msdBspRmuTxRx;

	if(devNum==MARVELL_88EA6321_DEVICE_NEMBER){
         cfg.baseRegAddr= PEARL_PORT_START_ADDR;
	}else if(devNum==MARVELL_88Q5050_DEVICE_NEMBER){
         cfg.baseRegAddr= WILLOW_PORT_START_ADDR;
       }
#ifdef USE_SEMAPHORE
	cfg.BSPFunctions.semCreate = osSemCreate;
	cfg.BSPFunctions.semDelete = osSemDelete;
	cfg.BSPFunctions.semTake = osSemWait;
	cfg.BSPFunctions.semGive = osSemSignal;
#else
	cfg.BSPFunctions.semCreate = NULL;
	cfg.BSPFunctions.semDelete = NULL;
	cfg.BSPFunctions.semTake = NULL;
	cfg.BSPFunctions.semGive = NULL;
#endif

	if ((status = msdLoadDriver(&cfg)) != MSD_OK)
	{
		Ifx_Console_print("msdLoadDriver return Failed\n\r");
		return status;
	}

	dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		Ifx_Console_print("devNum %d is NULL. Driver load failed. \n\r", devNum);
		return MSD_FAIL;
	}

	Ifx_Console_print("Device ID     : 0x%x\n\r", dev->deviceId);
	Ifx_Console_print("Base Reg Addr : 0x%x\n\r", dev->baseRegAddr);
	Ifx_Console_print("No of Ports   : %d\n\r", dev->numOfPorts);

	Ifx_Console_print("Driver has been started.\n\r");
	Ifx_Console_print("sample_qdStart_SingleDevice_RMU sample code success.\n\r");

	return MSD_OK;
}
#endif
