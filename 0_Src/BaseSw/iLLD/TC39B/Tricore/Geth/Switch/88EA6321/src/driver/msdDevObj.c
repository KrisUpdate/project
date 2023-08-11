#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/********************************************************************************
* msdSysConfig.c
*
* DESCRIPTION:
*       API definitions for system configuration, and enabling.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*
*******************************************************************************/
#define PEARL_BUILD_IN
#define WILLOW_BUILD_IN

#include "msdSysConfig.h"
#include "msdDevObj.h"
#include "msdHwAccess.h"
#include "msdSem.h"
#include "msdUtils.h"
#ifdef TOPAZ_BUILD_IN
#include <topaz/include/Topaz_msdApi.h>
#endif
#ifdef PERIDOT_BUILD_IN
#include <peridot/include/Peridot_msdApi.h>
#endif
#ifdef PEARL_BUILD_IN
#include "Pearl_msdApi.h"
#endif
#ifdef AGATE_BUILD_IN
#include <agate/include/Agate_msdApi.h>
#endif
#ifdef AMETHYST_BUILD_IN
#include <amethyst/include/Amethyst_msdApi.h>
#endif
#ifdef WILLOW_BUILD_IN
#include "Willow_msdApi.h"
#endif

static MSD_BOOL msdReserveFreeQueueSize(MSD_QD_DEV *qdev);
static MSD_BOOL msdPhyTXFIFODepthSet(MSD_QD_DEV *dev);

static MSD_STATUS InitDevObj_Willow(MSD_QD_DEV *dev)
{
#ifdef WILLOW_BUILD_IN
    dev->SwitchDevObj.ATUObj.gfdbAddMacEntry = Willow_gfdbAddMacEntryIntf;
	dev->SwitchDevObj.ATUObj.gfdbGetAtuEntryNext = Willow_gfdbGetAtuEntryNextIntf;
	dev->SwitchDevObj.ATUObj.gfdbFlush = Willow_gfdbFlushIntf;
	dev->SwitchDevObj.ATUObj.gfdbFlushInDB = Willow_gfdbFlushInDBIntf;
	dev->SwitchDevObj.ATUObj.gfdbMove = Willow_gfdbMoveIntf;
	dev->SwitchDevObj.ATUObj.gfdbMoveInDB = Willow_gfdbMoveInDBIntf;
    dev->SwitchDevObj.ATUObj.gfdbPortRemove = Willow_gfdbPortRemoveIntf;
    dev->SwitchDevObj.ATUObj.gfdbPortRemoveInDB = Willow_gfdbPortRemoveInDBIntf;
    dev->SwitchDevObj.ATUObj.gfdbDelAtuEntry = Willow_gfdbDelAtuEntry;
	dev->SwitchDevObj.ATUObj.gfdbGetViolation = Willow_gfdbGetViolationIntf;
	dev->SwitchDevObj.ATUObj.gfdbFindAtuMacEntry = Willow_gfdbFindAtuMacEntryIntf;
	dev->SwitchDevObj.ATUObj.gfdbSetAgingTimeout = Willow_gfdbSetAgingTimeout;
    dev->SwitchDevObj.ATUObj.gfdbGetAgingTimeout = Willow_gfdbGetAgingTimeout;
/*    dev->SwitchDevObj.ATUObj.gfdbSetLearn2All = Willow_gfdbSetLearn2All;
    dev->SwitchDevObj.ATUObj.gfdbGetLearn2All = Willow_gfdbGetLearn2All;*/
    dev->SwitchDevObj.ATUObj.gfdbSetPortLearnLimit = Willow_gfdbSetPortLearnLimit;
    dev->SwitchDevObj.ATUObj.gfdbGetPortLearnLimit = Willow_gfdbGetPortLearnLimit;
    dev->SwitchDevObj.ATUObj.gfdbGetPortLearnCount = Willow_gfdbGetPortLearnCount;
    dev->SwitchDevObj.ATUObj.gfdbGetEntryCount = Willow_gfdbGetEntryCount;
    dev->SwitchDevObj.ATUObj.gfdbGetEntryCountPerFid = Willow_gfdbGetEntryCountPerFid;
    dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCount = Willow_gfdbGetNonStaticEntryCount;
    dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCountPerFid = Willow_gfdbGetNonStaticEntryCountPerFid;
    dev->SwitchDevObj.ATUObj.gfdbDump = Willow_gfdbDump;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetCutThrouthEnable = Willow_gprtSetCutThrouthEnableIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetCutThrouthEnable = Willow_gprtGetCutThrouthEnableIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreFcs = Willow_gprtSetIgnoreFcs;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreFcs = Willow_gprtGetIgnoreFcs;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetAllowBadFcs = Willow_gprtSetAllowBadFcs;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetAllowBadFcs = Willow_gprtGetAllowBadFcs;
/*    dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitOut = Willow_gprtSetPauseLimitOut; //Willow don't support flow control.port reg 0x02
    dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitOut = Willow_gprtGetPauseLimitOut;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitIn = Willow_gprtSetPauseLimitIn;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitIn = Willow_gprtGetPauseLimitIn;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetQueueToPause = Willow_gprtSetQueueToPause;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetQueueToPause = Willow_gprtGetQueueToPause;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetRxPriorityFlowControlEnable = Willow_gprtSetRxPriorityFlowControlEnable;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetRxPriorityFlowControlEnable = Willow_gprtGetRxPriorityFlowControlEnable;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetRxPriorityFlowControlToQueue = Willow_gprtSetRxPriorityFlowControlToQueue;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetRxPriorityFlowControlToQueue = Willow_gprtGetRxPriorityFlowControlToQueue;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetTxPriorityFlowControlEnable = Willow_gprtSetTxPriorityFlowControlEnable;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetTxPriorityFlowControlEnable = Willow_gprtGetTxPriorityFlowControlEnable; */
    dev->SwitchDevObj.PORTCTRLObj.gprtGetOutQueueSize = Willow_gprtGetOutQueueSize;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetMapDA = Willow_gprtSetMapDA;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetMapDA = Willow_gprtGetMapDA;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetPortLocked = Willow_gprtSetPortLocked;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetPortLocked = Willow_gprtGetPortLocked;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetHoldAt1 = Willow_gprtSetHoldAt1;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetHoldAt1 = Willow_gprtGetHoldAt1;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetInt0nAgeOut = Willow_gprtSetInt0nAgeOut;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetInt0nAgeOut = Willow_gprtGetInt0nAgeOut;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetRefreshLocked = Willow_gprtSetRefreshLocked;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetRefreshLocked = Willow_gprtGetRefreshLocked;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreWrongData = Willow_gprtSetIgnoreWrongData;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreWrongData = Willow_gprtGetIgnoreWrongData;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetARPMirror = Willow_gprtSetARPMirror;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetARPMirror = Willow_gprtGetARPMirror;

	dev->SwitchDevObj.VCTObj.gvctGetAdvCableDiag = Willow_gvctGetAdvCableDiagIntf;
	dev->SwitchDevObj.VCTObj.gvctGetAdvExtendedStatus = Willow_gvctGetAdvExtendedStatusIntf;

	dev->SwitchDevObj.STUObj.gstuAddEntry = Willow_gstuAddEntryIntf;
	dev->SwitchDevObj.STUObj.gstuGetEntryNext = Willow_gstuGetEntryNextIntf;
	dev->SwitchDevObj.STUObj.gstuDelEntry = Willow_gstuDelEntryIntf;
	dev->SwitchDevObj.STUObj.gstuFindSidEntry = Willow_gstuFindSidEntryIntf;
	dev->SwitchDevObj.STUObj.gstuGetEntryCount = Willow_gstuGetEntryCountIntf;
	dev->SwitchDevObj.STUObj.gstuDump = Willow_gstuDump;

	dev->SwitchDevObj.VTUObj.gvtuFindVidEntry = Willow_gvtuFindVidEntryIntf;
    dev->SwitchDevObj.VTUObj.gvtuExistVidEntry = Willow_gvtuExistVidEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuFlush = Willow_gvtuFlushIntf;
	dev->SwitchDevObj.VTUObj.gvtuAddEntry = Willow_gvtuAddEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetEntryNext = Willow_gvtuGetEntryNextIntf;
	dev->SwitchDevObj.VTUObj.gvtuDelEntry = Willow_gvtuDelEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuSetMode = Willow_gvtuSetModeIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetMode = Willow_gvtuGetModeIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetViolation = Willow_gvtuGetViolationIntf;
    dev->SwitchDevObj.VTUObj.gvtuGetEntryCount = Willow_gvtuGetEntryCountIntf;
    dev->SwitchDevObj.VTUObj.gvtuSetMemberTag = Willow_gvtuSetMemberTagIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetMemberTag = Willow_gvtuGetMemberTagIntf;
	dev->SwitchDevObj.VTUObj.gvtuDump = Willow_gvtuDump;
	
    dev->SwitchDevObj.TrunkObj.gprtSetTrunkPort = Willow_gprtSetTrunkPort;
    dev->SwitchDevObj.TrunkObj.gprtGetTrunkPort = Willow_gprtGetTrunkPort;
    dev->SwitchDevObj.TrunkObj.gsysSetTrunkMaskTable = Willow_gsysSetTrunkMaskTable;
    dev->SwitchDevObj.TrunkObj.gsysGetTrunkMaskTable = Willow_gsysGetTrunkMaskTable;
    dev->SwitchDevObj.TrunkObj.gsysSetHashTrunk = Willow_gsysSetHashTrunk;
    dev->SwitchDevObj.TrunkObj.gsysGetHashTrunk = Willow_gsysGetHashTrunk;
    dev->SwitchDevObj.TrunkObj.gsysSetTrunkRouting = Willow_gsysSetTrunkRouting;
    dev->SwitchDevObj.TrunkObj.gsysGetTrunkRouting = Willow_gsysGetTrunkRouting;
	
	dev->SwitchDevObj.IMPObj.impRun = Willow_impRunIntf;
	dev->SwitchDevObj.IMPObj.impReset = Willow_impResetIntf;
	dev->SwitchDevObj.IMPObj.impWriteEEPROM = Willow_impWriteEEPROMIntf;
	dev->SwitchDevObj.IMPObj.impLoadToEEPROM = Willow_impLoadToEEPROMIntf;
	dev->SwitchDevObj.IMPObj.impLoadToRAM = Willow_impLoadToRAMIntf;
	dev->SwitchDevObj.IMPObj.impStop = Willow_impStopIntf;

	dev->SwitchDevObj.PHYCTRLObj.gphyReset = Willow_gphyReset;
    dev->SwitchDevObj.PHYCTRLObj.serdesReset = Willow_serdesReset;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortLoopback = Willow_gphySetPortLoopback;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortSpeed = Willow_gphySetPortSpeedIntf;
	dev->SwitchDevObj.PHYCTRLObj.gphyPortPowerDown = Willow_gphyPortPowerDown;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortDuplexMode = Willow_gphySetPortDuplexMode;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortAutoMode = Willow_gphySetPortAutoModeIntf;
    dev->SwitchDevObj.PHYCTRLObj.gphySetEEE = Willow_gphySetEEE;

	dev->SwitchDevObj.PIRLObj.gpirlInitialize = Willow_gpirlInitializeIntf;
	dev->SwitchDevObj.PIRLObj.gpirlInitResource=Willow_gpirlInitResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlCustomSetup=Willow_gpirlCustomSetupIntf;
	dev->SwitchDevObj.PIRLObj.gpirlReadResource=Willow_gpirlReadResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlWriteResource=Willow_gpirlWriteResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlGetResReg=Willow_gpirlGetResRegIntf;
	dev->SwitchDevObj.PIRLObj.gpirlSetResReg = Willow_gpirlSetResRegIntf;
    	dev->SwitchDevObj.PIRLObj.gpirlAdvConfig = Willow_gpirlAdvConfigIntf;
    	dev->SwitchDevObj.PIRLObj.gpirlAdvConfigDisable = Willow_gpirlAdvConfigDisableIntf;

	dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVid = Willow_gvlnSetPortVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVid = Willow_gvlnGetPortVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVlanDot1qMode=Willow_gvlnSetPortVlanDot1qModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVlanDot1qMode=Willow_gvlnGetPortVlanDot1qModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardTagged=Willow_gprtSetDiscardTaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardTagged=Willow_gprtGetDiscardTaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardUntagged=Willow_gprtSetDiscardUntaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardUntagged=Willow_gprtGetDiscardUntaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetUnicastFloodBlock=Willow_gprtSetUnicastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetUnicastFloodBlock=Willow_gprtGetUnicastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMulticastFloodBlock=Willow_gprtSetMulticastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMulticastFloodBlock=Willow_gprtGetMulticastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardBCastMode=Willow_gprtSetDiscardBCastModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardBCastMode=Willow_gprtGetDiscardBCastModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetIGMPSnoopEnable = Willow_gprtSetIGMPSnoopEnable;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetIGMPSnoopEnable = Willow_gprtGetIGMPSnoopEnable;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetHeaderMode = Willow_gprtSetHeaderMode;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetHeaderMode = Willow_gprtGetHeaderMode;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetDropOnLock = Willow_gprtSetDropOnLock;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDropOnLock = Willow_gprtGetDropOnLock;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetJumboMode=Willow_gprtSetJumboModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetJumboMode=Willow_gprtGetJumboModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetLearnEnable=Willow_gprtSetLearnEnableIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetLearnEnable = Willow_gprtGetLearnEnableIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetVTUPriorityOverride = Willow_gprtSetVTUPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetVTUPriorityOverride = Willow_gprtGetVTUPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetSAPriorityOverride = Willow_gprtSetSAPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetSAPriorityOverride = Willow_gprtGetSAPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetDAPriorityOverride = Willow_gprtSetDAPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDAPriorityOverride = Willow_gprtGetDAPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetMessagePort = Willow_gprtSetMessagePort;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetMessagePort = Willow_gprtGetMessagePort;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetEtherType = Willow_gprtSetEtherType;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetEtherType = Willow_gprtGetEtherType;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetAllowVidZero = Willow_gprtSetAllowVidZero;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetAllowVidZero = Willow_gprtGetAllowVidZero;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetFlowCtrl=Willow_gprtSetFlowCtrlIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetFlowCtrl=Willow_gprtGetFlowCtrlIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMonitorSource=Willow_gprtSetEgressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMonitorSource=Willow_gprtGetEgressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIngressMonitorSource=Willow_gprtSetIngressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIngressMonitorSource=Willow_gprtGetIngressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.grcSetEgressRate=Willow_grcSetEgressRateIntf;
	dev->SwitchDevObj.PORTCTRLObj.grcGetEgressRate=Willow_grcGetEgressRateIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplexStatus = Willow_gprtGetDuplexStatusIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetDuplex = Willow_gprtSetDuplexIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplex = Willow_gprtGetDuplexIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetForceLink = Willow_gprtGetForceLinkIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetForceLink = Willow_gprtSetForceLinkIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetLinkState = Willow_gprtGetLinkState;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetForceSpeed = Willow_gprtSetForceSpeedIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetForceSpeed = Willow_gprtGetForceSpeedIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetSpeed = Willow_gprtGetSpeedIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetFrameMode = Willow_gprtSetFrameModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetFrameMode = Willow_gprtGetFrameModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMode = Willow_gprtSetEgressModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMode = Willow_gprtGetEgressModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetVlanPorts = Willow_gprtSetVlanPorts;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetVlanPorts = Willow_gprtGetVlanPorts;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetPortState = Willow_gprtSetPortStateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPortState = Willow_gprtGetPortStateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtRegDump = Willow_gprtRegDump;

    dev->SwitchDevObj.QOSMAPObj.gprtGetIEEEPrioMap = Willow_gprtGetIEEEPrioMapIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtSetIEEEPrioMap = Willow_gprtSetIEEEPrioMapIntf;
    dev->SwitchDevObj.QOSMAPObj.gqosGetPrioMapRule = Willow_gqosGetPrioMapRuleIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtGetInitialPri = Willow_gprtGetInitialPriIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtSetDefQpri = Willow_gprtSetDefQpriIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtGetDefQpri = Willow_gprtGetDefQpriIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtSetDefFpri = Willow_gprtSetDefFpri;
    dev->SwitchDevObj.QOSMAPObj.gprtGetDefFpri = Willow_gprtGetDefFpri;
    dev->SwitchDevObj.QOSMAPObj.gprtSetInitialPri = Willow_gprtSetInitialPriIntf;
    dev->SwitchDevObj.QOSMAPObj.gqosSetPrioMapRule = Willow_gqosSetPrioMapRuleIntf;
  /*  dev->SwitchDevObj.QOSMAPObj.gprtRewriteDscp = Willow_gprtRewriteDscp;  //Don't support on Willow 
    dev->SwitchDevObj.QOSMAPObj.gprtSetCosToQueue = Willow_gprtSetCosToQueue;
    dev->SwitchDevObj.QOSMAPObj.gprtGetCosToQueue = Willow_gprtGetCosToQueue;
    dev->SwitchDevObj.QOSMAPObj.gprtSetCosToDscp = Willow_gprtSetCosToDscp;
    dev->SwitchDevObj.QOSMAPObj.gprtGetCosToDscp = Willow_gprtGetCosToDscp; */

	dev->SwitchDevObj.PORTRMONObj.gstatsFlushAll = Willow_gstatsFlushAll;
	dev->SwitchDevObj.PORTRMONObj.gstatsFlushPort = Willow_gstatsFlushPort;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetPortCounter=Willow_gstatsGetPortCounterIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetPortAllCounters=Willow_gstatsGetPortAllCountersIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetHistogramMode=Willow_gstatsGetHistogramModeIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsSetHistogramMode=Willow_gstatsSetHistogramModeIntf;
	dev->SwitchDevObj.PORTRMONObj.gmibDump = Willow_gmibDump;

	dev->SwitchDevObj.QUEUEObj.gprtSetQueueCtrl = Willow_gprtSetQueueCtrlIntf;
	dev->SwitchDevObj.QUEUEObj.gprtGetQueueCtrl=Willow_gprtGetQueueCtrlIntf;
	dev->SwitchDevObj.QUEUEObj.gprtSetPortSched=Willow_gprtSetPortSchedIntf;
    dev->SwitchDevObj.QUEUEObj.gprtGetPortSched = Willow_gprtGetPortSchedIntf;
    dev->SwitchDevObj.QUEUEObj.gsysSetQoSWeight = Willow_gsysSetQoSWeightIntf;
    dev->SwitchDevObj.QUEUEObj.gsysGetQoSWeight = Willow_gsysGetQoSWeightIntf;

	dev->SwitchDevObj.SYSCTRLObj.gsysSetFloodBC = Willow_gsysSetFloodBC;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetFloodBC = Willow_gsysGetFloodBC;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetMonitorMgmtCtrl = Willow_gsysSetMonitorMgmtCtrl;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetMonitorMgmtCtrl = Willow_gsysGetMonitorMgmtCtrl;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetCPUDest = Willow_gsysSetCPUDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetCPUDest = Willow_gsysGetCPUDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetIngressMonitorDest = Willow_gsysSetIngressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetIngressMonitorDest = Willow_gsysGetIngressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetEgressMonitorDest = Willow_gsysSetEgressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetEgressMonitorDest = Willow_gsysGetEgressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetRsvd2CpuEnables = Willow_gsysSetRsvd2CpuEnables;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetRsvd2CpuEnables = Willow_gsysGetRsvd2CpuEnables;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetMGMTPri = Willow_gsysSetMGMTPri;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetMGMTPri = Willow_gsysGetMGMTPri;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetSMISetup = Willow_gsysSetSMISetup;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetSMISetup = Willow_gsysGetSMISetup;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal1RegDump = Willow_gsysGlobal1RegDump;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal2RegDump = Willow_gsysGlobal2RegDump;

    dev->SwitchDevObj.QAVObj.gqavGetPortQpriXHiLimit = Willow_gqavGetPortQpriXHiLimit;
    dev->SwitchDevObj.QAVObj.gqavSetPortQpriXHiLimit = Willow_gqavSetPortQpriXHiLimit;
    dev->SwitchDevObj.QAVObj.gqavGetPortQpriXRate = Willow_gqavGetPortQpriXRate;
    dev->SwitchDevObj.QAVObj.gqavSetPortQpriXRate = Willow_gqavSetPortQpriXRate;

    dev->SwitchDevObj.PTPObj.gptpSetIntEnable = Willow_gptpSetIntEnableIntf;
    dev->SwitchDevObj.PTPObj.gptpGetIntEnable = Willow_gptpGetIntEnableIntf;
    dev->SwitchDevObj.PTPObj.gptpSetProtPTPEn = Willow_gptpSetProtPTPEn;
    dev->SwitchDevObj.PTPObj.gptpGetProtPTPEn = Willow_gptpGetProtPTPEn;
    dev->SwitchDevObj.PTPObj.gptpSetPortConfig = Willow_gptpSetPortConfigIntf;
    dev->SwitchDevObj.PTPObj.gptpGetPortConfig = Willow_gptpGetPortConfigIntf;
    dev->SwitchDevObj.PTPObj.gptpGetTimeStamp = Willow_gptpGetTimeStampIntf;
    dev->SwitchDevObj.PTPObj.gptpGetTSValidSt = Willow_gptpGetTSValidSt;
    dev->SwitchDevObj.PTPObj.gptpResetTimeStamp = Willow_gptpResetTimeStamp;
    dev->SwitchDevObj.PTPObj.gptpSetGlobalConfig = Willow_gptpSetGlobalConfigIntf;
    dev->SwitchDevObj.PTPObj.gptpGetGlobalConfig = Willow_gptpGetGlobalConfigIntf;
    dev->SwitchDevObj.PTPObj.gptpGetIntStatus = Willow_gptpGetIntStatusIntf;
    dev->SwitchDevObj.PTPObj.gptpSetMeanPathDelay = Willow_gptpSetMeanPathDelay;
    dev->SwitchDevObj.PTPObj.gptpGetMeanPathDelay = Willow_gptpGetMeanPathDelay;
    dev->SwitchDevObj.PTPObj.gptpSetIgrPathDelayAsym = Willow_gptpSetIgrPathDelayAsym;
    dev->SwitchDevObj.PTPObj.gptpGetIgrPathDelayAsym = Willow_gptpGetIgrPathDelayAsym;
    dev->SwitchDevObj.PTPObj.gptpSetEgrPathDelayAsym = Willow_gptpSetEgrPathDelayAsym;
    dev->SwitchDevObj.PTPObj.gptpGetEgrPathDelayAsym = Willow_gptpGetEgrPathDelayAsym;
    dev->SwitchDevObj.PTPObj.gptpSetMode = Willow_gptpSetModeIntf;
    dev->SwitchDevObj.PTPObj.gptpGetMode = Willow_gptpGetModeIntf;
    dev->SwitchDevObj.PTPObj.gptpSetMasterSlave = Willow_gptpSetMasterSlaveIntf;
    dev->SwitchDevObj.PTPObj.gptpGetMasterSlave = Willow_gptpGetMasterSlaveIntf;
    dev->SwitchDevObj.PTPObj.gptpGetTODBusyBitStatus = Willow_gptpGetTODBusyBitStatus;
    dev->SwitchDevObj.PTPObj.gptpTODStoreCompensation = Willow_gptpTODStoreCompensationIntf;
    dev->SwitchDevObj.PTPObj.gptpTODStoreAll = Willow_gptpTODStoreAllIntf;
    dev->SwitchDevObj.PTPObj.gptpTODCaptureAll = Willow_gptpTODCaptureAllIntf;
    dev->SwitchDevObj.PTPObj.gptpSetPulse = Willow_gptpSetPulseIntf;
    dev->SwitchDevObj.PTPObj.gptpGetPulse = Willow_gptpGetPulseIntf;
    dev->SwitchDevObj.PTPObj.gtaiGetPTPGlobalTime = Willow_gtaiGetPTPGlobalTime;

	dev->SwitchDevObj.TCAMObj.gtcamFlushAll = Willow_gtcamFlushAll;
	dev->SwitchDevObj.TCAMObj.gtcamFlushEntry = Willow_gtcamFlushEntry;
	dev->SwitchDevObj.TCAMObj.gtcamLoadEntry = Willow_gtcamLoadEntryIntf;
	dev->SwitchDevObj.TCAMObj.gtcamReadTCAMData = Willow_gtcamReadTCAMDataIntf;
	dev->SwitchDevObj.TCAMObj.gtcamGetNextTCAMData = Willow_gtcamGetNextTCAMDataIntf;
	dev->SwitchDevObj.TCAMObj.gtcamFindEntry = Willow_gtcamFindEntryIntf;
    dev->SwitchDevObj.TCAMObj.gtcamSetMode = Willow_gtcamSetModeIntf;
    dev->SwitchDevObj.TCAMObj.gtcamGetMode = Willow_gtcamGetModeIntf;
    dev->SwitchDevObj.TCAMObj.gtcamAddEntryPort = Willow_gtcamAddEntryPort;
    dev->SwitchDevObj.TCAMObj.gtcamRemoveEntryPort = Willow_gtcamRemoveEntryPort;
	/*dev->SwitchDevObj.TCAMObj.gtcamEgrFlushEntry = Willow_gtcamEgrFlushEntryIntf; //Willow don't support egress tcam
	dev->SwitchDevObj.TCAMObj.gtcamEgrFlushEntryAllPorts = Willow_gtcamEgrFlushEntryAllPortsIntf;
	dev->SwitchDevObj.TCAMObj.gtcamEgrLoadEntry = Willow_gtcamEgrLoadEntryIntf;
	dev->SwitchDevObj.TCAMObj.gtcamEgrGetNextTCAMData = Willow_gtcamEgrGetNextTCAMDataIntf;
	dev->SwitchDevObj.TCAMObj.gtcamEgrReadTCAMData = Willow_gtcamEgrReadTCAMDataIntf; */
	dev->SwitchDevObj.TCAMObj.gtcamDump = Willow_gtcamDump;
   /* dev->SwitchDevObj.TCAMObj.gtcamAdvConfig = Willow_gtcamAdvConfigIntf;*/

	dev->SwitchDevObj.RMUObj.grmuGetID = Willow_msdRmuGetIDIntf;
	dev->SwitchDevObj.RMUObj.grmuAtuDump = Willow_msdRmuAtuDumpIntf;	
	dev->SwitchDevObj.RMUObj.grmuMib2Dump = Willow_msdRmuMib2DumpIntf;
	dev->SwitchDevObj.RMUObj.grmuTCAMDump = Willow_msdRmuTCAMDumpIntf;
	dev->SwitchDevObj.RMUObj.grmuTCAMRead = Willow_msdRmuTCAMReadIntf;
	dev->SwitchDevObj.RMUObj.grmuTCAMWrite = Willow_msdRmuTCAMWriteIntf;
	dev->SwitchDevObj.RMUObj.grmuMultiRegAccess = Willow_msdRmuMultiRegAccessIntf;
    dev->SwitchDevObj.RMUObj.grmuRegDump = Willow_msdRmuRegDump;

    dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIC45PhyReg = Willow_msdGetSMIC45PhyReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIC45PhyReg = Willow_msdSetSMIC45PhyReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyReg = Willow_msdGetSMIPhyReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyReg = Willow_msdSetSMIPhyReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyXMDIOReg = Willow_msdSetSMIPhyXMDIOReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyXMDIOReg = Willow_msdGetSMIPhyXMDIOReg;

/*	dev->SwitchDevObj.EEPROMObj.geepromWrite = Willow_EEPROMWriteIntf;
	dev->SwitchDevObj.EEPROMObj.geepromRead = Willow_EEPROMReadIntf;
*/
    dev->SwitchDevObj.LEDObj.gledForceOn = Willow_gledForceOnIntf;
    dev->SwitchDevObj.LEDObj.gledForceOff = Willow_gledForceOffIntf;
    dev->SwitchDevObj.LEDObj.gledLinkAct = Willow_gledLinkActIntf;
	dev->SwitchDevObj.LEDObj.gledModeSet = Willow_gledModeSetIntf;
	dev->SwitchDevObj.LEDObj.gledModeGet = Willow_gledModeGetIntf;
	dev->SwitchDevObj.LEDObj.gledBlinkRateSet = Willow_gledBlinkRateSetIntf;
	dev->SwitchDevObj.LEDObj.gledBlinkRateGet = Willow_gledBlinkRateGetIntf;
	dev->SwitchDevObj.LEDObj.gledPulseStretchSet = Willow_gledPulseStretchSetIntf;
	dev->SwitchDevObj.LEDObj.gledPulseStretchGet = Willow_gledPulseStretchGetIntf;
	dev->SwitchDevObj.LEDObj.gledSpecialModeSet = Willow_gledSpecialModeSetIntf;
	dev->SwitchDevObj.LEDObj.gledSpecialModeGet = Willow_gledSpecialModeGetIntf;
	dev->SwitchDevObj.LEDObj.gledCtrlSet = Willow_gledCtrlSetIntf;
	dev->SwitchDevObj.LEDObj.gledCtrlGet = Willow_gledCtrlGetIntf;
#endif
    return MSD_OK;
}

static MSD_STATUS InitDevObj_Peridot(MSD_QD_DEV *dev)
{
#ifdef PERIDOT_BUILD_IN
    dev->SwitchDevObj.ATUObj.gfdbAddMacEntry = Peridot_gfdbAddMacEntryIntf;
	dev->SwitchDevObj.ATUObj.gfdbGetAtuEntryNext = Peridot_gfdbGetAtuEntryNextIntf;
	dev->SwitchDevObj.ATUObj.gfdbFlush = Peridot_gfdbFlushIntf;
	dev->SwitchDevObj.ATUObj.gfdbFlushInDB = Peridot_gfdbFlushInDBIntf;
	dev->SwitchDevObj.ATUObj.gfdbMove = Peridot_gfdbMoveIntf;
	dev->SwitchDevObj.ATUObj.gfdbMoveInDB = Peridot_gfdbMoveInDBIntf;
    dev->SwitchDevObj.ATUObj.gfdbPortRemove = Peridot_gfdbPortRemoveIntf;
    dev->SwitchDevObj.ATUObj.gfdbPortRemoveInDB = Peridot_gfdbPortRemoveInDBIntf;
    dev->SwitchDevObj.ATUObj.gfdbDelAtuEntry = Peridot_gfdbDelAtuEntry;
	dev->SwitchDevObj.ATUObj.gfdbGetViolation = Peridot_gfdbGetViolationIntf;
	dev->SwitchDevObj.ATUObj.gfdbFindAtuMacEntry = Peridot_gfdbFindAtuMacEntryIntf;
	dev->SwitchDevObj.ATUObj.gfdbSetAgingTimeout = Peridot_gfdbSetAgingTimeout;
    dev->SwitchDevObj.ATUObj.gfdbGetAgingTimeout = Peridot_gfdbGetAgingTimeout;
    dev->SwitchDevObj.ATUObj.gfdbSetLearn2All = Peridot_gfdbSetLearn2All;
    dev->SwitchDevObj.ATUObj.gfdbGetLearn2All = Peridot_gfdbGetLearn2All;
    dev->SwitchDevObj.ATUObj.gfdbSetPortLearnLimit = Peridot_gfdbSetPortLearnLimit;
    dev->SwitchDevObj.ATUObj.gfdbGetPortLearnLimit = Peridot_gfdbGetPortLearnLimit;
    dev->SwitchDevObj.ATUObj.gfdbGetPortLearnCount = Peridot_gfdbGetPortLearnCount;
    dev->SwitchDevObj.ATUObj.gfdbGetEntryCount = Peridot_gfdbGetEntryCount;
    dev->SwitchDevObj.ATUObj.gfdbGetEntryCountPerFid = Peridot_gfdbGetEntryCountPerFid;
    dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCount = Peridot_gfdbGetNonStaticEntryCount;
    dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCountPerFid = Peridot_gfdbGetNonStaticEntryCountPerFid;
    dev->SwitchDevObj.ATUObj.gfdbDump = Peridot_gfdbDump;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetCutThrouthEnable = Peridot_gprtSetCutThrouthEnableIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetCutThrouthEnable = Peridot_gprtGetCutThrouthEnableIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreFcs = Peridot_gprtSetIgnoreFcs;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreFcs = Peridot_gprtGetIgnoreFcs;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetAllowBadFcs = Peridot_gprtSetAllowBadFcs;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetAllowBadFcs = Peridot_gprtGetAllowBadFcs;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitOut = Peridot_gprtSetPauseLimitOut;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitOut = Peridot_gprtGetPauseLimitOut;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitIn = Peridot_gprtSetPauseLimitIn;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitIn = Peridot_gprtGetPauseLimitIn;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetQueueToPause = Peridot_gprtSetQueueToPause;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetQueueToPause = Peridot_gprtGetQueueToPause;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetRxPriorityFlowControlEnable = Peridot_gprtSetRxPriorityFlowControlEnable;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetRxPriorityFlowControlEnable = Peridot_gprtGetRxPriorityFlowControlEnable;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetRxPriorityFlowControlToQueue = Peridot_gprtSetRxPriorityFlowControlToQueue;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetRxPriorityFlowControlToQueue = Peridot_gprtGetRxPriorityFlowControlToQueue;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetTxPriorityFlowControlEnable = Peridot_gprtSetTxPriorityFlowControlEnable;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetTxPriorityFlowControlEnable = Peridot_gprtGetTxPriorityFlowControlEnable;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetOutQueueSize = Peridot_gprtGetOutQueueSize;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetMapDA = Peridot_gprtSetMapDA;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetMapDA = Peridot_gprtGetMapDA;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetPortLocked = Peridot_gprtSetPortLocked;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetPortLocked = Peridot_gprtGetPortLocked;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetHoldAt1 = Peridot_gprtSetHoldAt1;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetHoldAt1 = Peridot_gprtGetHoldAt1;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetInt0nAgeOut = Peridot_gprtSetInt0nAgeOut;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetInt0nAgeOut = Peridot_gprtGetInt0nAgeOut;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetRefreshLocked = Peridot_gprtSetRefreshLocked;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetRefreshLocked = Peridot_gprtGetRefreshLocked;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreWrongData = Peridot_gprtSetIgnoreWrongData;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreWrongData = Peridot_gprtGetIgnoreWrongData;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetARPMirror = Peridot_gprtSetARPMirror;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetARPMirror = Peridot_gprtGetARPMirror;

	dev->SwitchDevObj.VCTObj.gvctGetAdvCableDiag = Peridot_gvctGetAdvCableDiagIntf;
	dev->SwitchDevObj.VCTObj.gvctGetAdvExtendedStatus = Peridot_gvctGetAdvExtendedStatusIntf;

	dev->SwitchDevObj.STUObj.gstuAddEntry = Peridot_gstuAddEntryIntf;
	dev->SwitchDevObj.STUObj.gstuGetEntryNext = Peridot_gstuGetEntryNextIntf;
	dev->SwitchDevObj.STUObj.gstuDelEntry = Peridot_gstuDelEntryIntf;
	dev->SwitchDevObj.STUObj.gstuFindSidEntry = Peridot_gstuFindSidEntryIntf;
	dev->SwitchDevObj.STUObj.gstuGetEntryCount = Peridot_gstuGetEntryCountIntf;
	dev->SwitchDevObj.STUObj.gstuDump = Peridot_gstuDump;

	dev->SwitchDevObj.VTUObj.gvtuFindVidEntry = Peridot_gvtuFindVidEntryIntf;
    dev->SwitchDevObj.VTUObj.gvtuExistVidEntry = Peridot_gvtuExistVidEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuFlush = Peridot_gvtuFlushIntf;
	dev->SwitchDevObj.VTUObj.gvtuAddEntry = Peridot_gvtuAddEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetEntryNext = Peridot_gvtuGetEntryNextIntf;
	dev->SwitchDevObj.VTUObj.gvtuDelEntry = Peridot_gvtuDelEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuSetMode = Peridot_gvtuSetModeIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetMode = Peridot_gvtuGetModeIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetViolation = Peridot_gvtuGetViolationIntf;
    dev->SwitchDevObj.VTUObj.gvtuGetEntryCount = Peridot_gvtuGetEntryCountIntf;
    dev->SwitchDevObj.VTUObj.gvtuSetMemberTag = Peridot_gvtuSetMemberTagIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetMemberTag = Peridot_gvtuGetMemberTagIntf;
	dev->SwitchDevObj.VTUObj.gvtuDump = Peridot_gvtuDump;
	
    dev->SwitchDevObj.TrunkObj.gprtSetTrunkPort = Peridot_gprtSetTrunkPort;
    dev->SwitchDevObj.TrunkObj.gprtGetTrunkPort = Peridot_gprtGetTrunkPort;
    dev->SwitchDevObj.TrunkObj.gsysSetTrunkMaskTable = Peridot_gsysSetTrunkMaskTable;
    dev->SwitchDevObj.TrunkObj.gsysGetTrunkMaskTable = Peridot_gsysGetTrunkMaskTable;
    dev->SwitchDevObj.TrunkObj.gsysSetHashTrunk = Peridot_gsysSetHashTrunk;
    dev->SwitchDevObj.TrunkObj.gsysGetHashTrunk = Peridot_gsysGetHashTrunk;
    dev->SwitchDevObj.TrunkObj.gsysSetTrunkRouting = Peridot_gsysSetTrunkRouting;
    dev->SwitchDevObj.TrunkObj.gsysGetTrunkRouting = Peridot_gsysGetTrunkRouting;
	
	dev->SwitchDevObj.IMPObj.impRun = Peridot_impRunIntf;
	dev->SwitchDevObj.IMPObj.impReset = Peridot_impResetIntf;
	dev->SwitchDevObj.IMPObj.impWriteEEPROM = Peridot_impWriteEEPROMIntf;
	dev->SwitchDevObj.IMPObj.impLoadToEEPROM = Peridot_impLoadToEEPROMIntf;
	dev->SwitchDevObj.IMPObj.impLoadToRAM = Peridot_impLoadToRAMIntf;
	dev->SwitchDevObj.IMPObj.impStop = Peridot_impStopIntf;

	dev->SwitchDevObj.PHYCTRLObj.gphyReset = Peridot_gphyReset;
    dev->SwitchDevObj.PHYCTRLObj.serdesReset = Peridot_serdesReset;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortLoopback = Peridot_gphySetPortLoopback;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortSpeed = Peridot_gphySetPortSpeedIntf;
	dev->SwitchDevObj.PHYCTRLObj.gphyPortPowerDown = Peridot_gphyPortPowerDown;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortDuplexMode = Peridot_gphySetPortDuplexMode;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortAutoMode = Peridot_gphySetPortAutoModeIntf;
    dev->SwitchDevObj.PHYCTRLObj.gphySetEEE = Peridot_gphySetEEE;

	dev->SwitchDevObj.PIRLObj.gpirlInitialize = Peridot_gpirlInitializeIntf;
	dev->SwitchDevObj.PIRLObj.gpirlInitResource=Peridot_gpirlInitResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlCustomSetup=Peridot_gpirlCustomSetup_sr2cIntf;
	dev->SwitchDevObj.PIRLObj.gpirlReadResource=Peridot_gpirlReadResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlWriteResource=Peridot_gpirlWriteResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlGetResReg=Peridot_gpirlGetResRegIntf;
    dev->SwitchDevObj.PIRLObj.gpirlSetResReg = Peridot_gpirlSetResRegIntf;
    dev->SwitchDevObj.PIRLObj.gpirlAdvConfig = Peridot_gpirlAdvConfigIntf;
    dev->SwitchDevObj.PIRLObj.gpirlAdvConfigDisable = Peridot_gpirlAdvConfigDisableIntf;

	dev->SwitchDevObj.PORTCTRLObj.gprtSetForceDefaultVid = Peridot_gprtSetForceDefaultVid;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceDefaultVid = Peridot_gprtGetForceDefaultVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVid = Peridot_gvlnSetPortVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVid = Peridot_gvlnGetPortVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVlanDot1qMode=Peridot_gvlnSetPortVlanDot1qModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVlanDot1qMode=Peridot_gvlnGetPortVlanDot1qModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardTagged=Peridot_gprtSetDiscardTaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardTagged=Peridot_gprtGetDiscardTaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardUntagged=Peridot_gprtSetDiscardUntaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardUntagged=Peridot_gprtGetDiscardUntaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetUnicastFloodBlock=Peridot_gprtSetUnicastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetUnicastFloodBlock=Peridot_gprtGetUnicastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMulticastFloodBlock=Peridot_gprtSetMulticastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMulticastFloodBlock=Peridot_gprtGetMulticastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardBCastMode=Peridot_gprtSetDiscardBCastModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardBCastMode=Peridot_gprtGetDiscardBCastModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetIGMPSnoopEnable = Peridot_gprtSetIGMPSnoopEnable;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetIGMPSnoopEnable = Peridot_gprtGetIGMPSnoopEnable;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetHeaderMode = Peridot_gprtSetHeaderMode;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetHeaderMode = Peridot_gprtGetHeaderMode;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetDropOnLock = Peridot_gprtSetDropOnLock;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDropOnLock = Peridot_gprtGetDropOnLock;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetJumboMode=Peridot_gprtSetJumboModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetJumboMode=Peridot_gprtGetJumboModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetLearnEnable=Peridot_gprtSetLearnEnableIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetLearnEnable = Peridot_gprtGetLearnEnableIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetVTUPriorityOverride = Peridot_gprtSetVTUPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetVTUPriorityOverride = Peridot_gprtGetVTUPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetSAPriorityOverride = Peridot_gprtSetSAPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetSAPriorityOverride = Peridot_gprtGetSAPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetDAPriorityOverride = Peridot_gprtSetDAPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDAPriorityOverride = Peridot_gprtGetDAPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetMessagePort = Peridot_gprtSetMessagePort;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetMessagePort = Peridot_gprtGetMessagePort;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetEtherType = Peridot_gprtSetEtherType;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetEtherType = Peridot_gprtGetEtherType;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetAllowVidZero = Peridot_gprtSetAllowVidZero;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetAllowVidZero = Peridot_gprtGetAllowVidZero;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetFlowCtrl=Peridot_gprtSetFlowCtrlIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetFlowCtrl=Peridot_gprtGetFlowCtrlIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMonitorSource=Peridot_gprtSetEgressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMonitorSource=Peridot_gprtGetEgressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIngressMonitorSource=Peridot_gprtSetIngressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIngressMonitorSource=Peridot_gprtGetIngressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.grcSetEgressRate=Peridot_grcSetEgressRateIntf;
	dev->SwitchDevObj.PORTCTRLObj.grcGetEgressRate=Peridot_grcGetEgressRateIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplexStatus = Peridot_gprtGetDuplexStatusIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetDuplex = Peridot_gprtSetDuplexIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplex = Peridot_gprtGetDuplexIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetForceLink = Peridot_gprtGetForceLinkIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetForceLink = Peridot_gprtSetForceLinkIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetLinkState = Peridot_gprtGetLinkState;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetForceSpeed = Peridot_gprtSetForceSpeedIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetForceSpeed = Peridot_gprtGetForceSpeedIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetSpeed = Peridot_gprtGetSpeedIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetFrameMode = Peridot_gprtSetFrameModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetFrameMode = Peridot_gprtGetFrameModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMode = Peridot_gprtSetEgressModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMode = Peridot_gprtGetEgressModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetVlanPorts = Peridot_gprtSetVlanPorts;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetVlanPorts = Peridot_gprtGetVlanPorts;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetPortState = Peridot_gprtSetPortStateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPortState = Peridot_gprtGetPortStateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtRegDump = Peridot_gprtRegDump;

    dev->SwitchDevObj.QOSMAPObj.gprtGetIEEEPrioMap = Peridot_gprtGetIEEEPrioMapIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtSetIEEEPrioMap = Peridot_gprtSetIEEEPrioMapIntf;
    dev->SwitchDevObj.QOSMAPObj.gqosGetPrioMapRule = Peridot_gqosGetPrioMapRuleIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtGetInitialPri = Peridot_gprtGetInitialPriIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtSetDefQpri = Peridot_gprtSetDefQpriIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtGetDefQpri = Peridot_gprtGetDefQpriIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtSetDefFpri = Peridot_gprtSetDefFpri;
    dev->SwitchDevObj.QOSMAPObj.gprtGetDefFpri = Peridot_gprtGetDefFpri;
    dev->SwitchDevObj.QOSMAPObj.gprtSetInitialPri = Peridot_gprtSetInitialPriIntf;
    dev->SwitchDevObj.QOSMAPObj.gqosSetPrioMapRule = Peridot_gqosSetPrioMapRuleIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtRewriteDscp = Peridot_gprtRewriteDscp;
    dev->SwitchDevObj.QOSMAPObj.gprtSetCosToQueue = Peridot_gprtSetCosToQueue;
    dev->SwitchDevObj.QOSMAPObj.gprtGetCosToQueue = Peridot_gprtGetCosToQueue;
    dev->SwitchDevObj.QOSMAPObj.gprtSetCosToDscp = Peridot_gprtSetCosToDscp;
    dev->SwitchDevObj.QOSMAPObj.gprtGetCosToDscp = Peridot_gprtGetCosToDscp;

	dev->SwitchDevObj.PORTRMONObj.gstatsFlushAll = Peridot_gstatsFlushAll;
	dev->SwitchDevObj.PORTRMONObj.gstatsFlushPort = Peridot_gstatsFlushPort;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetPortCounter=Peridot_gstatsGetPortCounterIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetPortAllCounters=Peridot_gstatsGetPortAllCountersIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetHistogramMode=Peridot_gstatsGetHistogramModeIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsSetHistogramMode=Peridot_gstatsSetHistogramModeIntf;
	dev->SwitchDevObj.PORTRMONObj.gmibDump = Peridot_gmibDump;

	dev->SwitchDevObj.QUEUEObj.gprtSetQueueCtrl = Peridot_gprtSetQueueCtrlIntf;
	dev->SwitchDevObj.QUEUEObj.gprtGetQueueCtrl=Peridot_gprtGetQueueCtrlIntf;
	dev->SwitchDevObj.QUEUEObj.gprtSetPortSched=Peridot_gprtSetPortSchedIntf;
    dev->SwitchDevObj.QUEUEObj.gprtGetPortSched = Peridot_gprtGetPortSchedIntf;
    dev->SwitchDevObj.QUEUEObj.gsysSetQoSWeight = Peridot_gsysSetQoSWeightIntf;
    dev->SwitchDevObj.QUEUEObj.gsysGetQoSWeight = Peridot_gsysGetQoSWeightIntf;

	dev->SwitchDevObj.SYSCTRLObj.gsysSetFloodBC = Peridot_gsysSetFloodBC;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetFloodBC = Peridot_gsysGetFloodBC;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetMonitorMgmtCtrl = Peridot_gsysSetMonitorMgmtCtrl;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetMonitorMgmtCtrl = Peridot_gsysGetMonitorMgmtCtrl;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetCPUDest = Peridot_gsysSetCPUDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetCPUDest = Peridot_gsysGetCPUDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetIngressMonitorDest = Peridot_gsysSetIngressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetIngressMonitorDest = Peridot_gsysGetIngressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetEgressMonitorDest = Peridot_gsysSetEgressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetEgressMonitorDest = Peridot_gsysGetEgressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetRsvd2CpuEnables = Peridot_gsysSetRsvd2CpuEnables;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetRsvd2CpuEnables = Peridot_gsysGetRsvd2CpuEnables;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetMGMTPri = Peridot_gsysSetMGMTPri;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetMGMTPri = Peridot_gsysGetMGMTPri;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetSMISetup = Peridot_gsysSetSMISetup;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetSMISetup = Peridot_gsysGetSMISetup;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal1RegDump = Peridot_gsysGlobal1RegDump;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal2RegDump = Peridot_gsysGlobal2RegDump;

    dev->SwitchDevObj.QAVObj.gqavGetPortQpriXHiLimit = Peridot_gqavGetPortQpriXHiLimit;
    dev->SwitchDevObj.QAVObj.gqavSetPortQpriXHiLimit = Peridot_gqavSetPortQpriXHiLimit;
    dev->SwitchDevObj.QAVObj.gqavGetPortQpriXRate = Peridot_gqavGetPortQpriXRate;
    dev->SwitchDevObj.QAVObj.gqavSetPortQpriXRate = Peridot_gqavSetPortQpriXRate;

    dev->SwitchDevObj.PTPObj.gptpSetIntEnable = Peridot_gptpSetIntEnableIntf;
    dev->SwitchDevObj.PTPObj.gptpGetIntEnable = Peridot_gptpGetIntEnableIntf;
    dev->SwitchDevObj.PTPObj.gptpSetProtPTPEn = Peridot_gptpSetProtPTPEn;
    dev->SwitchDevObj.PTPObj.gptpGetProtPTPEn = Peridot_gptpGetProtPTPEn;
    dev->SwitchDevObj.PTPObj.gptpSetPortConfig = Peridot_gptpSetPortConfigIntf;
    dev->SwitchDevObj.PTPObj.gptpGetPortConfig = Peridot_gptpGetPortConfigIntf;
    dev->SwitchDevObj.PTPObj.gptpGetTimeStamp = Peridot_gptpGetTimeStampIntf;
    dev->SwitchDevObj.PTPObj.gptpGetTSValidSt = Peridot_gptpGetTSValidSt;
    dev->SwitchDevObj.PTPObj.gptpResetTimeStamp = Peridot_gptpResetTimeStamp;
    dev->SwitchDevObj.PTPObj.gptpSetGlobalConfig = Peridot_gptpSetGlobalConfigIntf;
    dev->SwitchDevObj.PTPObj.gptpGetGlobalConfig = Peridot_gptpGetGlobalConfigIntf;
    dev->SwitchDevObj.PTPObj.gptpGetIntStatus = Peridot_gptpGetIntStatusIntf;
    dev->SwitchDevObj.PTPObj.gptpSetMeanPathDelay = Peridot_gptpSetMeanPathDelay;
    dev->SwitchDevObj.PTPObj.gptpGetMeanPathDelay = Peridot_gptpGetMeanPathDelay;
    dev->SwitchDevObj.PTPObj.gptpSetIgrPathDelayAsym = Peridot_gptpSetIgrPathDelayAsym;
    dev->SwitchDevObj.PTPObj.gptpGetIgrPathDelayAsym = Peridot_gptpGetIgrPathDelayAsym;
    dev->SwitchDevObj.PTPObj.gptpSetEgrPathDelayAsym = Peridot_gptpSetEgrPathDelayAsym;
    dev->SwitchDevObj.PTPObj.gptpGetEgrPathDelayAsym = Peridot_gptpGetEgrPathDelayAsym;
    dev->SwitchDevObj.PTPObj.gptpSetMode = Peridot_gptpSetModeIntf;
    dev->SwitchDevObj.PTPObj.gptpGetMode = Peridot_gptpGetModeIntf;
    dev->SwitchDevObj.PTPObj.gptpSetMasterSlave = Peridot_gptpSetMasterSlaveIntf;
    dev->SwitchDevObj.PTPObj.gptpGetMasterSlave = Peridot_gptpGetMasterSlaveIntf;
    dev->SwitchDevObj.PTPObj.gptpGetTODBusyBitStatus = Peridot_gptpGetTODBusyBitStatus;
    dev->SwitchDevObj.PTPObj.gptpTODStoreCompensation = Peridot_gptpTODStoreCompensationIntf;
    dev->SwitchDevObj.PTPObj.gptpTODStoreAll = Peridot_gptpTODStoreAllIntf;
    dev->SwitchDevObj.PTPObj.gptpTODCaptureAll = Peridot_gptpTODCaptureAllIntf;
    dev->SwitchDevObj.PTPObj.gptpSetPulse = Peridot_gptpSetPulseIntf;
    dev->SwitchDevObj.PTPObj.gptpGetPulse = Peridot_gptpGetPulseIntf;
    dev->SwitchDevObj.PTPObj.gtaiGetPTPGlobalTime = Peridot_gtaiGetPTPGlobalTime;

	dev->SwitchDevObj.TCAMObj.gtcamFlushAll = Peridot_gtcamFlushAll;
	dev->SwitchDevObj.TCAMObj.gtcamFlushEntry = Peridot_gtcamFlushEntry;
	dev->SwitchDevObj.TCAMObj.gtcamLoadEntry = Peridot_gtcamLoadEntryIntf;
	dev->SwitchDevObj.TCAMObj.gtcamReadTCAMData = Peridot_gtcamReadTCAMDataIntf;
	dev->SwitchDevObj.TCAMObj.gtcamGetNextTCAMData = Peridot_gtcamGetNextTCAMDataIntf;
	dev->SwitchDevObj.TCAMObj.gtcamFindEntry = Peridot_gtcamFindEntryIntf;
    dev->SwitchDevObj.TCAMObj.gtcamSetMode = Peridot_gtcamSetModeIntf;
    dev->SwitchDevObj.TCAMObj.gtcamGetMode = Peridot_gtcamGetModeIntf;
    dev->SwitchDevObj.TCAMObj.gtcamAddEntryPort = Peridot_gtcamAddEntryPort;
    dev->SwitchDevObj.TCAMObj.gtcamRemoveEntryPort = Peridot_gtcamRemoveEntryPort;
	dev->SwitchDevObj.TCAMObj.gtcamEgrFlushEntry = Peridot_gtcamEgrFlushEntryIntf;
	dev->SwitchDevObj.TCAMObj.gtcamEgrFlushEntryAllPorts = Peridot_gtcamEgrFlushEntryAllPortsIntf;
	dev->SwitchDevObj.TCAMObj.gtcamEgrLoadEntry = Peridot_gtcamEgrLoadEntryIntf;
	dev->SwitchDevObj.TCAMObj.gtcamEgrGetNextTCAMData = Peridot_gtcamEgrGetNextTCAMDataIntf;
	dev->SwitchDevObj.TCAMObj.gtcamEgrReadTCAMData = Peridot_gtcamEgrReadTCAMDataIntf;
	dev->SwitchDevObj.TCAMObj.gtcamDump = Peridot_gtcamDump;
    dev->SwitchDevObj.TCAMObj.gtcamAdvConfig = Peridot_gtcamAdvConfigIntf;

	dev->SwitchDevObj.RMUObj.grmuGetID = Peridot_msdRmuGetIDIntf;
	dev->SwitchDevObj.RMUObj.grmuAtuDump = Peridot_msdRmuAtuDumpIntf;	
	dev->SwitchDevObj.RMUObj.grmuMib2Dump = Peridot_msdRmuMib2DumpIntf;
	dev->SwitchDevObj.RMUObj.grmuTCAMDump = Peridot_msdRmuTCAMDumpIntf;
	dev->SwitchDevObj.RMUObj.grmuTCAMRead = Peridot_msdRmuTCAMReadIntf;
	dev->SwitchDevObj.RMUObj.grmuTCAMWrite = Peridot_msdRmuTCAMWriteIntf;
	dev->SwitchDevObj.RMUObj.grmuMultiRegAccess = Peridot_msdRmuMultiRegAccessIntf;
    dev->SwitchDevObj.RMUObj.grmuRegDump = Peridot_msdRmuRegDump;

    dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIC45PhyReg = Peridot_msdGetSMIC45PhyReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIC45PhyReg = Peridot_msdSetSMIC45PhyReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyReg = Peridot_msdGetSMIPhyReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyReg = Peridot_msdSetSMIPhyReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyXMDIOReg = Peridot_msdSetSMIPhyXMDIOReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyXMDIOReg = Peridot_msdGetSMIPhyXMDIOReg;

	dev->SwitchDevObj.EEPROMObj.geepromWrite = Peridot_EEPROMWriteIntf;
	dev->SwitchDevObj.EEPROMObj.geepromRead = Peridot_EEPROMReadIntf;

    dev->SwitchDevObj.LEDObj.gledForceOn = Peridot_gledForceOnIntf;
    dev->SwitchDevObj.LEDObj.gledForceOff = Peridot_gledForceOffIntf;
    dev->SwitchDevObj.LEDObj.gledLinkAct = Peridot_gledLinkActIntf;
	dev->SwitchDevObj.LEDObj.gledModeSet = Peridot_gledModeSetIntf;
	dev->SwitchDevObj.LEDObj.gledModeGet = Peridot_gledModeGetIntf;
	dev->SwitchDevObj.LEDObj.gledBlinkRateSet = Peridot_gledBlinkRateSetIntf;
	dev->SwitchDevObj.LEDObj.gledBlinkRateGet = Peridot_gledBlinkRateGetIntf;
	dev->SwitchDevObj.LEDObj.gledPulseStretchSet = Peridot_gledPulseStretchSetIntf;
	dev->SwitchDevObj.LEDObj.gledPulseStretchGet = Peridot_gledPulseStretchGetIntf;
	dev->SwitchDevObj.LEDObj.gledSpecialModeSet = Peridot_gledSpecialModeSetIntf;
	dev->SwitchDevObj.LEDObj.gledSpecialModeGet = Peridot_gledSpecialModeGetIntf;
	dev->SwitchDevObj.LEDObj.gledCtrlSet = Peridot_gledCtrlSetIntf;
	dev->SwitchDevObj.LEDObj.gledCtrlGet = Peridot_gledCtrlGetIntf;
#endif
    return MSD_OK;
}

static MSD_STATUS InitDevObj_Topaz(MSD_QD_DEV *dev)
{
#ifdef TOPAZ_BUILD_IN
    dev->SwitchDevObj.ATUObj.gfdbAddMacEntry = Topaz_gfdbAddMacEntryIntf;
    dev->SwitchDevObj.ATUObj.gfdbGetAtuEntryNext = Topaz_gfdbGetAtuEntryNextIntf;
    dev->SwitchDevObj.ATUObj.gfdbFlush = Topaz_gfdbFlushIntf;
	dev->SwitchDevObj.ATUObj.gfdbFlushInDB = Topaz_gfdbFlushInDBIntf;
    dev->SwitchDevObj.ATUObj.gfdbMove = Topaz_gfdbMoveIntf;
    dev->SwitchDevObj.ATUObj.gfdbMoveInDB = Topaz_gfdbMoveInDBIntf;
    dev->SwitchDevObj.ATUObj.gfdbPortRemove = Topaz_gfdbPortRemoveIntf;
    dev->SwitchDevObj.ATUObj.gfdbPortRemoveInDB = Topaz_gfdbPortRemoveInDBIntf;
    dev->SwitchDevObj.ATUObj.gfdbDelAtuEntry = Topaz_gfdbDelAtuEntry;
    dev->SwitchDevObj.ATUObj.gfdbGetViolation = Topaz_gfdbGetViolationIntf;
    dev->SwitchDevObj.ATUObj.gfdbFindAtuMacEntry = Topaz_gfdbFindAtuMacEntryIntf;
    dev->SwitchDevObj.ATUObj.gfdbSetAgingTimeout = Topaz_gfdbSetAgingTimeout;
    dev->SwitchDevObj.ATUObj.gfdbGetAgingTimeout = Topaz_gfdbGetAgingTimeout;
    dev->SwitchDevObj.ATUObj.gfdbSetLearn2All = Topaz_gfdbSetLearn2All;
    dev->SwitchDevObj.ATUObj.gfdbGetLearn2All = Topaz_gfdbGetLearn2All;
    dev->SwitchDevObj.ATUObj.gfdbSetPortLearnLimit = Topaz_gfdbSetPortLearnLimit;
    dev->SwitchDevObj.ATUObj.gfdbGetPortLearnLimit = Topaz_gfdbGetPortLearnLimit;
    dev->SwitchDevObj.ATUObj.gfdbGetPortLearnCount = Topaz_gfdbGetPortLearnCount;
    dev->SwitchDevObj.ATUObj.gfdbGetEntryCount = Topaz_gfdbGetEntryCount;
    dev->SwitchDevObj.ATUObj.gfdbGetEntryCountPerFid = Topaz_gfdbGetEntryCountPerFid;
    dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCount = Topaz_gfdbGetNonStaticEntryCount;
    dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCountPerFid = Topaz_gfdbGetNonStaticEntryCountPerFid;
    dev->SwitchDevObj.ATUObj.gfdbDump = Topaz_gfdbDump;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetCutThrouthEnable = Topaz_gprtSetCutThrouthEnableIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetCutThrouthEnable = Topaz_gprtGetCutThrouthEnableIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreFcs = Topaz_gprtSetIgnoreFcs;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreFcs = Topaz_gprtGetIgnoreFcs;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetAllowBadFcs = Topaz_gprtSetAllowBadFcs;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetAllowBadFcs = Topaz_gprtGetAllowBadFcs;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitOut = Topaz_gprtSetPauseLimitOut;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitOut = Topaz_gprtGetPauseLimitOut;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitIn = Topaz_gprtSetPauseLimitIn;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitIn = Topaz_gprtGetPauseLimitIn;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetOutQueueSize = Topaz_gprtGetOutQueueSize;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetMapDA = Topaz_gprtSetMapDA;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetMapDA = Topaz_gprtGetMapDA;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetPortLocked = Topaz_gprtSetPortLocked;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetPortLocked = Topaz_gprtGetPortLocked;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetHoldAt1 = Topaz_gprtSetHoldAt1;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetHoldAt1 = Topaz_gprtGetHoldAt1;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetInt0nAgeOut = Topaz_gprtSetInt0nAgeOut;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetInt0nAgeOut = Topaz_gprtGetInt0nAgeOut;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetRefreshLocked = Topaz_gprtSetRefreshLocked;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetRefreshLocked = Topaz_gprtGetRefreshLocked;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreWrongData = Topaz_gprtSetIgnoreWrongData;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreWrongData = Topaz_gprtGetIgnoreWrongData;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetARPMirror = Topaz_gprtSetARPMirror;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetARPMirror = Topaz_gprtGetARPMirror;

    dev->SwitchDevObj.VCTObj.gvctGetAdvCableDiag = Topaz_gvctGetAdvCableDiagIntf;
    dev->SwitchDevObj.VCTObj.gvctGetAdvExtendedStatus = Topaz_gvctGetAdvExtendedStatusIntf;

    dev->SwitchDevObj.STUObj.gstuAddEntry = Topaz_gstuAddEntryIntf;
    dev->SwitchDevObj.STUObj.gstuDelEntry = Topaz_gstuDelEntryIntf;
	dev->SwitchDevObj.STUObj.gstuFindSidEntry = Topaz_gstuFindSidEntryIntf;
	dev->SwitchDevObj.STUObj.gstuGetEntryNext = Topaz_gstuGetEntryNextIntf;
	dev->SwitchDevObj.STUObj.gstuGetEntryCount = Topaz_gstuGetEntryCountIntf;
	dev->SwitchDevObj.STUObj.gstuDump = Topaz_gstuDump;

    dev->SwitchDevObj.VTUObj.gvtuFindVidEntry = Topaz_gvtuFindVidEntryIntf;
    dev->SwitchDevObj.VTUObj.gvtuExistVidEntry = Topaz_gvtuExistVidEntryIntf;
    dev->SwitchDevObj.VTUObj.gvtuFlush = Topaz_gvtuFlushIntf;
	dev->SwitchDevObj.VTUObj.gvtuAddEntry = Topaz_gvtuAddEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetEntryNext = Topaz_gvtuGetEntryNextIntf;
    dev->SwitchDevObj.VTUObj.gvtuDelEntry = Topaz_gvtuDelEntryIntf;
    dev->SwitchDevObj.VTUObj.gvtuGetViolation = Topaz_gvtuGetViolationIntf;
    dev->SwitchDevObj.VTUObj.gvtuGetEntryCount = Topaz_gvtuGetEntryCountIntf;
    dev->SwitchDevObj.VTUObj.gvtuSetMemberTag = Topaz_gvtuSetMemberTagIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetMemberTag = Topaz_gvtuGetMemberTagIntf;
	dev->SwitchDevObj.VTUObj.gvtuDump = Topaz_gvtuDump;

    dev->SwitchDevObj.QOSMAPObj.gprtGetDefQpri = Topaz_gprtGetDefQpriIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtGetInitialPri = Topaz_gprtGetInitialPriIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtSetDefQpri = Topaz_gprtSetDefQpriIntf;
    dev->SwitchDevObj.QOSMAPObj.gprtSetDefFpri = Topaz_gprtSetDefFpri;
    dev->SwitchDevObj.QOSMAPObj.gprtGetDefFpri = Topaz_gprtGetDefFpri;
    dev->SwitchDevObj.QOSMAPObj.gqosGetPrioMapRule = Topaz_gqosGetPrioMapRule;
    dev->SwitchDevObj.QOSMAPObj.gprtSetInitialPri = Topaz_gprtSetInitialPriIntf;
    dev->SwitchDevObj.QOSMAPObj.gqosSetPrioMapRule = Topaz_gqosSetPrioMapRuleIntf;

    dev->SwitchDevObj.IMPObj.impRun = Topaz_impRunIntf;
    dev->SwitchDevObj.IMPObj.impReset = Topaz_impResetIntf;
    dev->SwitchDevObj.IMPObj.impWriteEEPROM = Topaz_impWriteEEPROMIntf;
    dev->SwitchDevObj.IMPObj.impLoadToEEPROM = Topaz_impLoadToEEPROMIntf;
    dev->SwitchDevObj.IMPObj.impLoadToRAM = Topaz_impLoadToRAMIntf;
    dev->SwitchDevObj.IMPObj.impStop = Topaz_impStopIntf;

    dev->SwitchDevObj.PHYCTRLObj.gphyReset = Topaz_gphyReset;
    dev->SwitchDevObj.PHYCTRLObj.serdesReset = Topaz_serdesReset;
    dev->SwitchDevObj.PHYCTRLObj.gphySetPortLoopback = Topaz_gphySetPortLoopback;
    dev->SwitchDevObj.PHYCTRLObj.gphySetPortSpeed = Topaz_gphySetPortSpeedIntf;
    dev->SwitchDevObj.PHYCTRLObj.gphyPortPowerDown = Topaz_gphyPortPowerDown;
    dev->SwitchDevObj.PHYCTRLObj.gphySetPortDuplexMode = Topaz_gphySetPortDuplexMode;
    dev->SwitchDevObj.PHYCTRLObj.gphySetPortAutoMode = Topaz_gphySetPortAutoModeIntf;
    dev->SwitchDevObj.PHYCTRLObj.gphySetEEE = Topaz_gphySetEEE;

    dev->SwitchDevObj.PIRLObj.gpirlInitialize = Topaz_gpirlInitializeIntf;
    dev->SwitchDevObj.PIRLObj.gpirlInitResource = Topaz_gpirlInitResourceIntf;
    dev->SwitchDevObj.PIRLObj.gpirlCustomSetup = Topaz_gpirlCustomSetupIntf;
    dev->SwitchDevObj.PIRLObj.gpirlReadResource = Topaz_gpirlReadResourceIntf;
    dev->SwitchDevObj.PIRLObj.gpirlWriteResource = Topaz_gpirlWriteResourceIntf;
    dev->SwitchDevObj.PIRLObj.gpirlGetResReg = Topaz_gpirlGetResRegIntf;
    dev->SwitchDevObj.PIRLObj.gpirlSetResReg = Topaz_gpirlSetResRegIntf;
    dev->SwitchDevObj.PIRLObj.gpirlAdvConfig = Topaz_gpirlAdvConfigIntf;
    dev->SwitchDevObj.PIRLObj.gpirlAdvConfigDisable = Topaz_gpirlAdvConfigDisableIntf;

	dev->SwitchDevObj.PORTCTRLObj.gprtSetForceDefaultVid = Topaz_gprtSetForceDefaultVid;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceDefaultVid = Topaz_gprtGetForceDefaultVid;
    dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVid = Topaz_gvlnSetPortVid;
    dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVid = Topaz_gvlnGetPortVid;
    dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVlanDot1qMode = Topaz_gvlnSetPortVlanDot1qModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVlanDot1qMode = Topaz_gvlnGetPortVlanDot1qModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardTagged = Topaz_gprtSetDiscardTaggedIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardTagged = Topaz_gprtGetDiscardTaggedIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardUntagged = Topaz_gprtSetDiscardUntaggedIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardUntagged = Topaz_gprtGetDiscardUntaggedIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetUnicastFloodBlock = Topaz_gprtSetUnicastFloodBlockIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetUnicastFloodBlock = Topaz_gprtGetUnicastFloodBlockIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetMulticastFloodBlock = Topaz_gprtSetMulticastFloodBlockIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetMulticastFloodBlock = Topaz_gprtGetMulticastFloodBlockIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardBCastMode = Topaz_gprtSetDiscardBCastModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardBCastMode = Topaz_gprtGetDiscardBCastModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetIGMPSnoopEnable = Topaz_gprtSetIGMPSnoopEnable;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetIGMPSnoopEnable = Topaz_gprtGetIGMPSnoopEnable;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetHeaderMode = Topaz_gprtSetHeaderMode;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetHeaderMode = Topaz_gprtGetHeaderMode;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetDropOnLock = Topaz_gprtSetDropOnLock;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDropOnLock = Topaz_gprtGetDropOnLock;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetJumboMode = Topaz_gprtSetJumboModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetJumboMode = Topaz_gprtGetJumboModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetLearnEnable = Topaz_gprtSetLearnEnableIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetLearnEnable = Topaz_gprtGetLearnEnableIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetVTUPriorityOverride = Topaz_gprtSetVTUPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetVTUPriorityOverride = Topaz_gprtGetVTUPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetSAPriorityOverride = Topaz_gprtSetSAPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetSAPriorityOverride = Topaz_gprtGetSAPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetDAPriorityOverride = Topaz_gprtSetDAPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDAPriorityOverride = Topaz_gprtGetDAPriorityOverrideIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetMessagePort = Topaz_gprtSetMessagePort;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetMessagePort = Topaz_gprtGetMessagePort;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetEtherType = Topaz_gprtSetEtherType;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetEtherType = Topaz_gprtGetEtherType;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetFlowCtrl = Topaz_gprtSetFlowCtrlIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetFlowCtrl = Topaz_gprtGetFlowCtrlIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMonitorSource = Topaz_gprtSetEgressMonitorSource;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMonitorSource = Topaz_gprtGetEgressMonitorSource;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetIngressMonitorSource = Topaz_gprtSetIngressMonitorSource;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetIngressMonitorSource = Topaz_gprtGetIngressMonitorSource;
    dev->SwitchDevObj.PORTCTRLObj.grcSetEgressRate = Topaz_grcSetEgressRateIntf;
    dev->SwitchDevObj.PORTCTRLObj.grcGetEgressRate = Topaz_grcGetEgressRateIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplexStatus = Topaz_gprtGetDuplexStatusIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetDuplex = Topaz_gprtSetDuplexIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplex = Topaz_gprtGetDuplexIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetForceLink = Topaz_gprtGetForceLinkIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetForceLink = Topaz_gprtSetForceLinkIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetLinkState = Topaz_gprtGetLinkState;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetForceSpeed = Topaz_gprtSetForceSpeedIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetForceSpeed = Topaz_gprtGetForceSpeedIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetSpeed = Topaz_gprtGetSpeedIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetFrameMode = Topaz_gprtSetFrameModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetFrameMode = Topaz_gprtGetFrameModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMode = Topaz_gprtSetEgressModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMode = Topaz_gprtGetEgressModeIntf;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetVlanPorts = Topaz_gprtSetVlanPorts;
    dev->SwitchDevObj.PORTCTRLObj.gprtGetVlanPorts = Topaz_gprtGetVlanPorts;
    dev->SwitchDevObj.PORTCTRLObj.gprtSetPortState = Topaz_gprtSetPortStateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPortState = Topaz_gprtGetPortStateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtRegDump = Topaz_gprtRegDump;

    dev->SwitchDevObj.TrunkObj.gprtSetTrunkPort = Topaz_gprtSetTrunkPort;
    dev->SwitchDevObj.TrunkObj.gprtGetTrunkPort = Topaz_gprtGetTrunkPort;
    dev->SwitchDevObj.TrunkObj.gsysSetTrunkMaskTable = Topaz_gsysSetTrunkMaskTable;
    dev->SwitchDevObj.TrunkObj.gsysGetTrunkMaskTable = Topaz_gsysGetTrunkMaskTable;
    dev->SwitchDevObj.TrunkObj.gsysSetHashTrunk = Topaz_gsysSetHashTrunk;
    dev->SwitchDevObj.TrunkObj.gsysGetHashTrunk = Topaz_gsysGetHashTrunk;
    dev->SwitchDevObj.TrunkObj.gsysSetTrunkRouting = Topaz_gsysSetTrunkRouting;
    dev->SwitchDevObj.TrunkObj.gsysGetTrunkRouting = Topaz_gsysGetTrunkRouting;

    dev->SwitchDevObj.PORTRMONObj.gstatsFlushAll = Topaz_gstatsFlushAll;
    dev->SwitchDevObj.PORTRMONObj.gstatsFlushPort = Topaz_gstatsFlushPort;
    dev->SwitchDevObj.PORTRMONObj.gstatsGetPortCounter = Topaz_gstatsGetPortCounterIntf;
    dev->SwitchDevObj.PORTRMONObj.gstatsGetPortAllCounters = Topaz_gstatsGetPortAllCountersIntf;
    dev->SwitchDevObj.PORTRMONObj.gstatsGetHistogramMode = Topaz_gstatsGetHistogramModeIntf;
    dev->SwitchDevObj.PORTRMONObj.gstatsSetHistogramMode = Topaz_gstatsSetHistogramModeIntf;
	dev->SwitchDevObj.PORTRMONObj.gmibDump = Topaz_gmibDump;

    dev->SwitchDevObj.QUEUEObj.gprtSetQueueCtrl = Topaz_gprtSetQueueCtrlIntf;
    dev->SwitchDevObj.QUEUEObj.gprtGetQueueCtrl = Topaz_gprtGetQueueCtrlIntf;
    dev->SwitchDevObj.QUEUEObj.gprtSetPortSched = Topaz_gprtSetPortSchedIntf;
    dev->SwitchDevObj.QUEUEObj.gprtGetPortSched = Topaz_gprtGetPortSchedIntf;
    dev->SwitchDevObj.QUEUEObj.gsysSetQoSWeight = Topaz_gsysSetQoSWeightIntf;
    dev->SwitchDevObj.QUEUEObj.gsysGetQoSWeight = Topaz_gsysGetQoSWeightIntf;

    dev->SwitchDevObj.SYSCTRLObj.gsysSetFloodBC = Topaz_gsysSetFloodBC;
    dev->SwitchDevObj.SYSCTRLObj.gsysGetFloodBC = Topaz_gsysGetFloodBC;
    dev->SwitchDevObj.SYSCTRLObj.gsysSetMonitorMgmtCtrl = Topaz_gsysSetMonitorMgmtCtrl;
    dev->SwitchDevObj.SYSCTRLObj.gsysGetMonitorMgmtCtrl = Topaz_gsysGetMonitorMgmtCtrl;
    dev->SwitchDevObj.SYSCTRLObj.gsysSetCPUDest = Topaz_gsysSetCPUDest;
    dev->SwitchDevObj.SYSCTRLObj.gsysGetCPUDest = Topaz_gsysGetCPUDest;
    dev->SwitchDevObj.SYSCTRLObj.gsysSetIngressMonitorDest = Topaz_gsysSetIngressMonitorDest;
    dev->SwitchDevObj.SYSCTRLObj.gsysGetIngressMonitorDest = Topaz_gsysGetIngressMonitorDest;
    dev->SwitchDevObj.SYSCTRLObj.gsysSetEgressMonitorDest = Topaz_gsysSetEgressMonitorDest;
    dev->SwitchDevObj.SYSCTRLObj.gsysGetEgressMonitorDest = Topaz_gsysGetEgressMonitorDest;
    dev->SwitchDevObj.SYSCTRLObj.gsysSetRsvd2CpuEnables = Topaz_gsysSetRsvd2CpuEnables;
    dev->SwitchDevObj.SYSCTRLObj.gsysGetRsvd2CpuEnables = Topaz_gsysGetRsvd2CpuEnables;
    dev->SwitchDevObj.SYSCTRLObj.gsysSetMGMTPri = Topaz_gsysSetMGMTPri;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetMGMTPri = Topaz_gsysGetMGMTPri;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetSMISetup = Topaz_gsysSetSMISetup;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetSMISetup = Topaz_gsysGetSMISetup;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal1RegDump = Topaz_gsysGlobal1RegDump;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal2RegDump = Topaz_gsysGlobal2RegDump;

    dev->SwitchDevObj.QAVObj.gqavGetPortQpriXHiLimit = Topaz_gqavGetPortQpriXHiLimit;
    dev->SwitchDevObj.QAVObj.gqavSetPortQpriXHiLimit = Topaz_gqavSetPortQpriXHiLimit;
    dev->SwitchDevObj.QAVObj.gqavGetPortQpriXRate = Topaz_gqavGetPortQpriXRate;
    dev->SwitchDevObj.QAVObj.gqavSetPortQpriXRate = Topaz_gqavSetPortQpriXRate;
    dev->SwitchDevObj.TCAMObj.gtcamSetMode = Topaz_gtcamSetModeIntf;
    dev->SwitchDevObj.TCAMObj.gtcamGetMode = Topaz_gtcamGetModeIntf;
    dev->SwitchDevObj.TCAMObj.gtcamAddEntryPort = Topaz_gtcamAddEntryPort;
    dev->SwitchDevObj.TCAMObj.gtcamRemoveEntryPort = Topaz_gtcamRemoveEntryPort;

    dev->SwitchDevObj.PTPObj.gptpSetIntEnable = Topaz_gptpSetIntEnableIntf;
    dev->SwitchDevObj.PTPObj.gptpGetIntEnable = Topaz_gptpGetIntEnableIntf;
    dev->SwitchDevObj.PTPObj.gptpSetProtPTPEn = Topaz_gptpSetProtPTPEn;
    dev->SwitchDevObj.PTPObj.gptpGetProtPTPEn = Topaz_gptpGetProtPTPEn;
    dev->SwitchDevObj.PTPObj.gptpSetPortConfig = Topaz_gptpSetPortConfigIntf;
    dev->SwitchDevObj.PTPObj.gptpGetPortConfig = Topaz_gptpGetPortConfigIntf;
    dev->SwitchDevObj.PTPObj.gptpGetTimeStamp = Topaz_gptpGetTimeStampIntf;
    dev->SwitchDevObj.PTPObj.gptpGetTSValidSt = Topaz_gptpGetTSValidSt;
    dev->SwitchDevObj.PTPObj.gptpResetTimeStamp = Topaz_gptpResetTimeStamp;
    dev->SwitchDevObj.PTPObj.gptpSetGlobalConfig = Topaz_gptpSetGlobalConfigIntf;
    dev->SwitchDevObj.PTPObj.gptpGetGlobalConfig = Topaz_gptpGetGlobalConfigIntf;
    dev->SwitchDevObj.PTPObj.gptpGetIntStatus = Topaz_gptpGetIntStatusIntf;

	if (dev->deviceId != MSD_88E6141)
    {
        dev->SwitchDevObj.TCAMObj.gtcamFindEntry = Topaz_gtcamFindEntryIntf;
		dev->SwitchDevObj.TCAMObj.gtcamFlushAll = Topaz_gtcamFlushAll;
		dev->SwitchDevObj.TCAMObj.gtcamFlushEntry = Topaz_gtcamFlushEntry;
		dev->SwitchDevObj.TCAMObj.gtcamLoadEntry = Topaz_gtcamLoadEntryIntf;
		dev->SwitchDevObj.TCAMObj.gtcamReadTCAMData = Topaz_gtcamReadTCAMDataIntf;
		dev->SwitchDevObj.TCAMObj.gtcamGetNextTCAMData = Topaz_gtcamGetNextTCAMDataIntf;
		dev->SwitchDevObj.TCAMObj.gtcamDump = Topaz_gtcamDump;
        dev->SwitchDevObj.TCAMObj.gtcamAdvConfig = Topaz_gtcamAdvConfigIntf;
	}
	/* Topaz deviceId MSD_88E6141 doesn't support TCAM and AVB */

	dev->SwitchDevObj.RMUObj.grmuGetID = Topaz_msdRmuGetIDIntf;
	dev->SwitchDevObj.RMUObj.grmuAtuDump = Topaz_msdRmuAtuDumpIntf;	
	dev->SwitchDevObj.RMUObj.grmuMib2Dump = Topaz_msdRmuMib2DumpIntf;
	dev->SwitchDevObj.RMUObj.grmuTCAMDump = Topaz_msdRmuTCAMDumpIntf;
	dev->SwitchDevObj.RMUObj.grmuTCAMRead = Topaz_msdRmuTCAMReadIntf;
	dev->SwitchDevObj.RMUObj.grmuTCAMWrite = Topaz_msdRmuTCAMWriteIntf;
	dev->SwitchDevObj.RMUObj.grmuMultiRegAccess = Topaz_msdRmuMultiRegAccessIntf;
    dev->SwitchDevObj.RMUObj.grmuRegDump = Topaz_msdRmuRegDump;

    dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIC45PhyReg = Topaz_msdGetSMIC45PhyReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIC45PhyReg = Topaz_msdSetSMIC45PhyReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyReg = Topaz_msdGetSMIPhyReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyReg = Topaz_msdSetSMIPhyReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyXMDIOReg = Topaz_msdSetSMIPhyXMDIOReg;
    dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyXMDIOReg = Topaz_msdGetSMIPhyXMDIOReg;

	dev->SwitchDevObj.EEPROMObj.geepromWrite = Topaz_EEPROMWriteIntf;
	dev->SwitchDevObj.EEPROMObj.geepromRead = Topaz_EEPROMReadIntf;

	dev->SwitchDevObj.LEDObj.gledForceOn = Topaz_gledForceOnIntf;
	dev->SwitchDevObj.LEDObj.gledForceOff = Topaz_gledForceOffIntf;
	dev->SwitchDevObj.LEDObj.gledLinkAct = Topaz_gledLinkActIntf;
	dev->SwitchDevObj.LEDObj.gledModeSet = Topaz_gledModeSetIntf;
	dev->SwitchDevObj.LEDObj.gledModeGet = Topaz_gledModeGetIntf;
	dev->SwitchDevObj.LEDObj.gledBlinkRateSet = Topaz_gledBlinkRateSetIntf;
	dev->SwitchDevObj.LEDObj.gledBlinkRateGet = Topaz_gledBlinkRateGetIntf;
	dev->SwitchDevObj.LEDObj.gledPulseStretchSet = Topaz_gledPulseStretchSetIntf;
	dev->SwitchDevObj.LEDObj.gledPulseStretchGet = Topaz_gledPulseStretchGetIntf;
	dev->SwitchDevObj.LEDObj.gledSpecialModeSet = Topaz_gledSpecialModeSetIntf;
	dev->SwitchDevObj.LEDObj.gledSpecialModeGet = Topaz_gledSpecialModeGetIntf;
	dev->SwitchDevObj.LEDObj.gledCtrlSet = Topaz_gledCtrlSetIntf;
	dev->SwitchDevObj.LEDObj.gledCtrlGet = Topaz_gledCtrlGetIntf;
#endif
    return MSD_OK;
}

static MSD_STATUS InitDevObj_Pearl(MSD_QD_DEV *dev)
{
#ifdef PEARL_BUILD_IN
	dev->SwitchDevObj.ATUObj.gfdbAddMacEntry = Pearl_gfdbAddMacEntryIntf;
	dev->SwitchDevObj.ATUObj.gfdbGetAtuEntryNext = Pearl_gfdbGetAtuEntryNextIntf;
	dev->SwitchDevObj.ATUObj.gfdbFlush = Pearl_gfdbFlushIntf;
	dev->SwitchDevObj.ATUObj.gfdbFlushInDB = Pearl_gfdbFlushInDBIntf;
	dev->SwitchDevObj.ATUObj.gfdbMove = Pearl_gfdbMoveIntf;
	dev->SwitchDevObj.ATUObj.gfdbMoveInDB = Pearl_gfdbMoveInDBIntf;
	dev->SwitchDevObj.ATUObj.gfdbPortRemove = Pearl_gfdbPortRemoveIntf;
	dev->SwitchDevObj.ATUObj.gfdbPortRemoveInDB = Pearl_gfdbPortRemoveInDBIntf;
	dev->SwitchDevObj.ATUObj.gfdbDelAtuEntry = Pearl_gfdbDelAtuEntry;
	dev->SwitchDevObj.ATUObj.gfdbGetViolation = Pearl_gfdbGetViolationIntf;
	dev->SwitchDevObj.ATUObj.gfdbFindAtuMacEntry = Pearl_gfdbFindAtuMacEntryIntf;
	dev->SwitchDevObj.ATUObj.gfdbSetAgingTimeout = Pearl_gfdbSetAgingTimeout;
	dev->SwitchDevObj.ATUObj.gfdbGetAgingTimeout = Pearl_gfdbGetAgingTimeout;
	dev->SwitchDevObj.ATUObj.gfdbSetLearn2All = Pearl_gfdbSetLearn2All;
	dev->SwitchDevObj.ATUObj.gfdbGetLearn2All = Pearl_gfdbGetLearn2All;
	dev->SwitchDevObj.ATUObj.gfdbSetPortLearnLimit = Pearl_gfdbSetPortLearnLimit;
	dev->SwitchDevObj.ATUObj.gfdbGetPortLearnLimit = Pearl_gfdbGetPortLearnLimit;
	dev->SwitchDevObj.ATUObj.gfdbGetPortLearnCount = Pearl_gfdbGetPortLearnCount;
	dev->SwitchDevObj.ATUObj.gfdbGetEntryCount = Pearl_gfdbGetEntryCount;
	dev->SwitchDevObj.ATUObj.gfdbGetEntryCountPerFid = Pearl_gfdbGetEntryCountPerFid;
	dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCount = Pearl_gfdbGetNonStaticEntryCount;
	dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCountPerFid = Pearl_gfdbGetNonStaticEntryCountPerFid;
	dev->SwitchDevObj.ATUObj.gfdbDump = Pearl_gfdbDump;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreFcs = Pearl_gprtSetIgnoreFcs;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreFcs = Pearl_gprtGetIgnoreFcs;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetAllowBadFcs = Pearl_gprtSetAllowBadFcs;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetAllowBadFcs = Pearl_gprtGetAllowBadFcs;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitOut = Pearl_gprtSetPauseLimitOut;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitOut = Pearl_gprtGetPauseLimitOut;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitIn = Pearl_gprtSetPauseLimitIn;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitIn = Pearl_gprtGetPauseLimitIn;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetOutQueueSize = Pearl_gprtGetOutQueueSize;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMapDA = Pearl_gprtSetMapDA;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMapDA = Pearl_gprtGetMapDA;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPortLocked = Pearl_gprtSetPortLocked;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPortLocked = Pearl_gprtGetPortLocked;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetHoldAt1 = Pearl_gprtSetHoldAt1;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetHoldAt1 = Pearl_gprtGetHoldAt1;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetInt0nAgeOut = Pearl_gprtSetInt0nAgeOut;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetInt0nAgeOut = Pearl_gprtGetInt0nAgeOut;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetRefreshLocked = Pearl_gprtSetRefreshLocked;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetRefreshLocked = Pearl_gprtGetRefreshLocked;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreWrongData = Pearl_gprtSetIgnoreWrongData;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreWrongData = Pearl_gprtGetIgnoreWrongData;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetARPMirror = Pearl_gprtSetARPMirror;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetARPMirror = Pearl_gprtGetARPMirror;

	dev->SwitchDevObj.VCTObj.gvctGetAdvCableDiag = Pearl_gvctGetAdvCableDiagIntf;
	dev->SwitchDevObj.VCTObj.gvctGetAdvExtendedStatus = Pearl_gvctGetAdvExtendedStatusIntf;

	dev->SwitchDevObj.STUObj.gstuAddEntry = Pearl_gstuAddEntryIntf;
	dev->SwitchDevObj.STUObj.gstuDelEntry = Pearl_gstuDelEntryIntf;
	dev->SwitchDevObj.STUObj.gstuFindSidEntry = Pearl_gstuFindSidEntryIntf;
	dev->SwitchDevObj.STUObj.gstuGetEntryNext = Pearl_gstuGetEntryNextIntf;
	dev->SwitchDevObj.STUObj.gstuGetEntryCount = Pearl_gstuGetEntryCountIntf;
	dev->SwitchDevObj.STUObj.gstuDump = Pearl_gstuDump;

	dev->SwitchDevObj.VTUObj.gvtuFindVidEntry = Pearl_gvtuFindVidEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuExistVidEntry = Pearl_gvtuExistVidEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuFlush = Pearl_gvtuFlushIntf;
	dev->SwitchDevObj.VTUObj.gvtuAddEntry = Pearl_gvtuAddEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetEntryNext = Pearl_gvtuGetEntryNextIntf;
	dev->SwitchDevObj.VTUObj.gvtuDelEntry = Pearl_gvtuDelEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetViolation = Pearl_gvtuGetViolationIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetEntryCount = Pearl_gvtuGetEntryCountIntf;
	dev->SwitchDevObj.VTUObj.gvtuSetMemberTag = Pearl_gvtuSetMemberTagIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetMemberTag = Pearl_gvtuGetMemberTagIntf;
	dev->SwitchDevObj.VTUObj.gvtuDump = Pearl_gvtuDump;

	dev->SwitchDevObj.QOSMAPObj.gprtGetDefQpri = Pearl_gprtGetDefQpriIntf;
	dev->SwitchDevObj.QOSMAPObj.gprtGetInitialPri = Pearl_gprtGetInitialPriIntf;
	dev->SwitchDevObj.QOSMAPObj.gprtSetDefQpri = Pearl_gprtSetDefQpriIntf;
	dev->SwitchDevObj.QOSMAPObj.gprtSetDefFpri = Pearl_gprtSetDefFpri;
	dev->SwitchDevObj.QOSMAPObj.gprtGetDefFpri = Pearl_gprtGetDefFpri;
	dev->SwitchDevObj.QOSMAPObj.gqosGetPrioMapRule = Pearl_gqosGetPrioMapRule;
	dev->SwitchDevObj.QOSMAPObj.gprtSetInitialPri = Pearl_gprtSetInitialPriIntf;
	dev->SwitchDevObj.QOSMAPObj.gqosSetPrioMapRule = Pearl_gqosSetPrioMapRuleIntf;

	dev->SwitchDevObj.PHYCTRLObj.gphyReset = Pearl_gphyReset;
    dev->SwitchDevObj.PHYCTRLObj.serdesReset = Pearl_serdesReset;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortLoopback = Pearl_gphySetPortLoopback;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortSpeed = Pearl_gphySetPortSpeedIntf;
	dev->SwitchDevObj.PHYCTRLObj.gphyPortPowerDown = Pearl_gphyPortPowerDown;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortDuplexMode = Pearl_gphySetPortDuplexMode;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortAutoMode = Pearl_gphySetPortAutoModeIntf;
	dev->SwitchDevObj.PHYCTRLObj.gphySetEEE = Pearl_gphySetEEE;

	dev->SwitchDevObj.PIRLObj.gpirlInitialize = Pearl_gpirlInitializeIntf;
	dev->SwitchDevObj.PIRLObj.gpirlInitResource = Pearl_gpirlInitResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlCustomSetup = Pearl_gpirlCustomSetupIntf;
	dev->SwitchDevObj.PIRLObj.gpirlReadResource = Pearl_gpirlReadResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlWriteResource = Pearl_gpirlWriteResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlGetResReg = Pearl_gpirlGetResRegIntf;
	dev->SwitchDevObj.PIRLObj.gpirlSetResReg = Pearl_gpirlSetResRegIntf;
	dev->SwitchDevObj.PIRLObj.gpirlAdvConfig = Pearl_gpirlAdvConfigIntf;
	dev->SwitchDevObj.PIRLObj.gpirlAdvConfigDisable = Pearl_gpirlAdvConfigDisableIntf;

	dev->SwitchDevObj.PORTCTRLObj.gprtSetForceDefaultVid = Pearl_gprtSetForceDefaultVid;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceDefaultVid = Pearl_gprtGetForceDefaultVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVid = Pearl_gvlnSetPortVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVid = Pearl_gvlnGetPortVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVlanDot1qMode = Pearl_gvlnSetPortVlanDot1qModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVlanDot1qMode = Pearl_gvlnGetPortVlanDot1qModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardTagged = Pearl_gprtSetDiscardTaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardTagged = Pearl_gprtGetDiscardTaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardUntagged = Pearl_gprtSetDiscardUntaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardUntagged = Pearl_gprtGetDiscardUntaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetUnicastFloodBlock = Pearl_gprtSetUnicastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetUnicastFloodBlock = Pearl_gprtGetUnicastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMulticastFloodBlock = Pearl_gprtSetMulticastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMulticastFloodBlock = Pearl_gprtGetMulticastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardBCastMode = Pearl_gprtSetDiscardBCastModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardBCastMode = Pearl_gprtGetDiscardBCastModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIGMPSnoopEnable = Pearl_gprtSetIGMPSnoopEnable;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIGMPSnoopEnable = Pearl_gprtGetIGMPSnoopEnable;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetHeaderMode = Pearl_gprtSetHeaderMode;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetHeaderMode = Pearl_gprtGetHeaderMode;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDropOnLock = Pearl_gprtSetDropOnLock;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDropOnLock = Pearl_gprtGetDropOnLock;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetJumboMode = Pearl_gprtSetJumboModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetJumboMode = Pearl_gprtGetJumboModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetLearnEnable = Pearl_gprtSetLearnEnableIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetLearnEnable = Pearl_gprtGetLearnEnableIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetVTUPriorityOverride = Pearl_gprtSetVTUPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetVTUPriorityOverride = Pearl_gprtGetVTUPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetSAPriorityOverride = Pearl_gprtSetSAPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetSAPriorityOverride = Pearl_gprtGetSAPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDAPriorityOverride = Pearl_gprtSetDAPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDAPriorityOverride = Pearl_gprtGetDAPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMessagePort = Pearl_gprtSetMessagePort;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMessagePort = Pearl_gprtGetMessagePort;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEtherType = Pearl_gprtSetEtherType;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEtherType = Pearl_gprtGetEtherType;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetFlowCtrl = Pearl_gprtSetFlowCtrlIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetFlowCtrl = Pearl_gprtGetFlowCtrlIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMonitorSource = Pearl_gprtSetEgressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMonitorSource = Pearl_gprtGetEgressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIngressMonitorSource = Pearl_gprtSetIngressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIngressMonitorSource = Pearl_gprtGetIngressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.grcSetEgressRate = Pearl_grcSetEgressRateIntf;
	dev->SwitchDevObj.PORTCTRLObj.grcGetEgressRate = Pearl_grcGetEgressRateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplexStatus = Pearl_gprtGetDuplexStatusIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDuplex = Pearl_gprtSetDuplexIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplex = Pearl_gprtGetDuplexIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceLink = Pearl_gprtGetForceLinkIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetForceLink = Pearl_gprtSetForceLinkIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetLinkState = Pearl_gprtGetLinkState;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetForceSpeed = Pearl_gprtSetForceSpeedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceSpeed = Pearl_gprtGetForceSpeedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetSpeed = Pearl_gprtGetSpeedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetFrameMode = Pearl_gprtSetFrameModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetFrameMode = Pearl_gprtGetFrameModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMode = Pearl_gprtSetEgressModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMode = Pearl_gprtGetEgressModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetVlanPorts = Pearl_gprtSetVlanPorts;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetVlanPorts = Pearl_gprtGetVlanPorts;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPortState = Pearl_gprtSetPortStateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPortState = Pearl_gprtGetPortStateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtRegDump = Pearl_gprtRegDump;

	dev->SwitchDevObj.TrunkObj.gprtSetTrunkPort = Pearl_gprtSetTrunkPort;
	dev->SwitchDevObj.TrunkObj.gprtGetTrunkPort = Pearl_gprtGetTrunkPort;
	dev->SwitchDevObj.TrunkObj.gsysSetTrunkMaskTable = Pearl_gsysSetTrunkMaskTable;
	dev->SwitchDevObj.TrunkObj.gsysGetTrunkMaskTable = Pearl_gsysGetTrunkMaskTable;
	dev->SwitchDevObj.TrunkObj.gsysSetHashTrunk = Pearl_gsysSetHashTrunk;
	dev->SwitchDevObj.TrunkObj.gsysGetHashTrunk = Pearl_gsysGetHashTrunk;
	dev->SwitchDevObj.TrunkObj.gsysSetTrunkRouting = Pearl_gsysSetTrunkRouting;
	dev->SwitchDevObj.TrunkObj.gsysGetTrunkRouting = Pearl_gsysGetTrunkRouting;

	dev->SwitchDevObj.PORTRMONObj.gstatsFlushAll = Pearl_gstatsFlushAll;
	dev->SwitchDevObj.PORTRMONObj.gstatsFlushPort = Pearl_gstatsFlushPort;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetPortCounter = Pearl_gstatsGetPortCounterIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetPortAllCounters = Pearl_gstatsGetPortAllCountersIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetHistogramMode = Pearl_gstatsGetHistogramModeIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsSetHistogramMode = Pearl_gstatsSetHistogramModeIntf;
	dev->SwitchDevObj.PORTRMONObj.gmibDump = Pearl_gmibDump;

	dev->SwitchDevObj.QUEUEObj.gprtSetPortSched = Pearl_gprtSetPortSchedIntf;
	dev->SwitchDevObj.QUEUEObj.gprtGetPortSched = Pearl_gprtGetPortSchedIntf;
	dev->SwitchDevObj.QUEUEObj.gsysSetQoSWeight = Pearl_gsysSetQoSWeightIntf;
	dev->SwitchDevObj.QUEUEObj.gsysGetQoSWeight = Pearl_gsysGetQoSWeightIntf;

	dev->SwitchDevObj.SYSCTRLObj.gsysSetFloodBC = Pearl_gsysSetFloodBC;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetFloodBC = Pearl_gsysGetFloodBC;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetCPUDest = Pearl_gsysSetCPUDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetCPUDest = Pearl_gsysGetCPUDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetIngressMonitorDest = Pearl_gsysSetIngressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetIngressMonitorDest = Pearl_gsysGetIngressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetEgressMonitorDest = Pearl_gsysSetEgressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetEgressMonitorDest = Pearl_gsysGetEgressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetMGMTPri = Pearl_gsysSetMGMTPri;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetMGMTPri = Pearl_gsysGetMGMTPri;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetRsvd2CpuEnables = Pearl_gsysSetRsvd2CpuEnables;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetRsvd2CpuEnables = Pearl_gsysGetRsvd2CpuEnables;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal1RegDump = Pearl_gsysGlobal1RegDump;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal2RegDump = Pearl_gsysGlobal2RegDump;

	dev->SwitchDevObj.QAVObj.gqavGetPortQpriXHiLimit = Pearl_gqavGetPortQpriXHiLimit;
	dev->SwitchDevObj.QAVObj.gqavSetPortQpriXHiLimit = Pearl_gqavSetPortQpriXHiLimit;
	dev->SwitchDevObj.QAVObj.gqavGetPortQpriXRate = Pearl_gqavGetPortQpriXRate;
	dev->SwitchDevObj.QAVObj.gqavSetPortQpriXRate = Pearl_gqavSetPortQpriXRate;
	dev->SwitchDevObj.TCAMObj.gtcamSetMode = Pearl_gtcamSetModeIntf;
	dev->SwitchDevObj.TCAMObj.gtcamGetMode = Pearl_gtcamGetModeIntf;
	dev->SwitchDevObj.TCAMObj.gtcamAddEntryPort = Pearl_gtcamAddEntryPort;
	dev->SwitchDevObj.TCAMObj.gtcamRemoveEntryPort = Pearl_gtcamRemoveEntryPort;

	dev->SwitchDevObj.PTPObj.gptpSetIntEnable = Pearl_gptpSetIntEnableIntf;
	dev->SwitchDevObj.PTPObj.gptpGetIntEnable = Pearl_gptpGetIntEnableIntf;
	dev->SwitchDevObj.PTPObj.gptpSetProtPTPEn = Pearl_gptpSetProtPTPEn;
	dev->SwitchDevObj.PTPObj.gptpGetProtPTPEn = Pearl_gptpGetProtPTPEn;
	dev->SwitchDevObj.PTPObj.gptpSetPortConfig = Pearl_gptpSetPortConfigIntf;
	dev->SwitchDevObj.PTPObj.gptpGetPortConfig = Pearl_gptpGetPortConfigIntf;
	dev->SwitchDevObj.PTPObj.gptpGetTimeStamp = Pearl_gptpGetTimeStampIntf;
	dev->SwitchDevObj.PTPObj.gptpGetTSValidSt = Pearl_gptpGetTSValidSt;
	dev->SwitchDevObj.PTPObj.gptpResetTimeStamp = Pearl_gptpResetTimeStamp;
	dev->SwitchDevObj.PTPObj.gptpSetGlobalConfig = Pearl_gptpSetGlobalConfigIntf;
	dev->SwitchDevObj.PTPObj.gptpGetGlobalConfig = Pearl_gptpGetGlobalConfigIntf;
	dev->SwitchDevObj.PTPObj.gptpGetIntStatus = Pearl_gptpGetIntStatusIntf;

	if (dev->deviceId != MSD_88E6320)
	{
		dev->SwitchDevObj.TCAMObj.gtcamFindEntry = Pearl_gtcamFindEntryIntf;
		dev->SwitchDevObj.TCAMObj.gtcamFlushAll = Pearl_gtcamFlushAll;
		dev->SwitchDevObj.TCAMObj.gtcamFlushEntry = Pearl_gtcamFlushEntry;
		dev->SwitchDevObj.TCAMObj.gtcamLoadEntry = Pearl_gtcamLoadEntryIntf;
		dev->SwitchDevObj.TCAMObj.gtcamReadTCAMData = Pearl_gtcamReadTCAMDataIntf;
		dev->SwitchDevObj.TCAMObj.gtcamGetNextTCAMData = Pearl_gtcamGetNextTCAMDataIntf;
		dev->SwitchDevObj.TCAMObj.gtcamDump = Pearl_gtcamDump;
		dev->SwitchDevObj.TCAMObj.gtcamAdvConfig = Pearl_gtcamAdvConfigIntf;
		dev->SwitchDevObj.PORTCTRLObj.gprtSetCutThrouthEnable = Pearl_gprtSetCutThrouthEnableIntf;
		dev->SwitchDevObj.PORTCTRLObj.gprtGetCutThrouthEnable = Pearl_gprtGetCutThrouthEnableIntf;
	}
	/* Pearl deviceId MSD_88E6320 doesn't support TCAM and cut thru */

	dev->SwitchDevObj.RMUObj.grmuGetID = Pearl_msdRmuGetIDIntf;
	dev->SwitchDevObj.RMUObj.grmuAtuDump = Pearl_msdRmuAtuDumpIntf;
	dev->SwitchDevObj.RMUObj.grmuMib2Dump = Pearl_msdRmuMib2DumpIntf;
	dev->SwitchDevObj.RMUObj.grmuMultiRegAccess = Pearl_msdRmuMultiRegAccessIntf;
    dev->SwitchDevObj.RMUObj.grmuRegDump = Pearl_msdRmuRegDump;

	dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIC45PhyReg = Pearl_msdGetSMIC45PhyReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIC45PhyReg = Pearl_msdSetSMIC45PhyReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyReg = Pearl_msdGetSMIPhyReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyReg = Pearl_msdSetSMIPhyReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyXMDIOReg = Pearl_msdSetSMIPhyXMDIOReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyXMDIOReg = Pearl_msdGetSMIPhyXMDIOReg;

	dev->SwitchDevObj.EEPROMObj.geepromWrite = Pearl_EEPROMWriteIntf;
	dev->SwitchDevObj.EEPROMObj.geepromRead = Pearl_EEPROMReadIntf;

	dev->SwitchDevObj.LEDObj.gledForceOn = Pearl_gledForceOnIntf;
	dev->SwitchDevObj.LEDObj.gledForceOff = Pearl_gledForceOffIntf;	
	dev->SwitchDevObj.LEDObj.gledModeSet = Pearl_gledModeSetIntf;
	dev->SwitchDevObj.LEDObj.gledModeGet = Pearl_gledModeGetIntf;
	dev->SwitchDevObj.LEDObj.gledBlinkRateSet = Pearl_gledBlinkRateSetIntf;
	dev->SwitchDevObj.LEDObj.gledBlinkRateGet = Pearl_gledBlinkRateGetIntf;
	dev->SwitchDevObj.LEDObj.gledPulseStretchSet = Pearl_gledPulseStretchSetIntf;
	dev->SwitchDevObj.LEDObj.gledPulseStretchGet = Pearl_gledPulseStretchGetIntf;
	dev->SwitchDevObj.LEDObj.gledSpecialModeSet = Pearl_gledSpecialModeSetIntf;
	dev->SwitchDevObj.LEDObj.gledSpecialModeGet = Pearl_gledSpecialModeGetIntf;
	dev->SwitchDevObj.LEDObj.gledCtrlSet = Pearl_gledCtrlSetIntf;
	dev->SwitchDevObj.LEDObj.gledCtrlGet = Pearl_gledCtrlGetIntf;
#endif
	return MSD_OK;
}

static MSD_STATUS InitDevObj_Agate(MSD_QD_DEV *dev)
{
#ifdef AGATE_BUILD_IN
	dev->SwitchDevObj.ATUObj.gfdbAddMacEntry = Agate_gfdbAddMacEntryIntf;
	dev->SwitchDevObj.ATUObj.gfdbGetAtuEntryNext = Agate_gfdbGetAtuEntryNextIntf;
	dev->SwitchDevObj.ATUObj.gfdbFlush = Agate_gfdbFlushIntf;
	dev->SwitchDevObj.ATUObj.gfdbFlushInDB = Agate_gfdbFlushInDBIntf;
	dev->SwitchDevObj.ATUObj.gfdbMove = Agate_gfdbMoveIntf;
	dev->SwitchDevObj.ATUObj.gfdbMoveInDB = Agate_gfdbMoveInDBIntf;
	dev->SwitchDevObj.ATUObj.gfdbPortRemove = Agate_gfdbPortRemoveIntf;
	dev->SwitchDevObj.ATUObj.gfdbPortRemoveInDB = Agate_gfdbPortRemoveInDBIntf;
	dev->SwitchDevObj.ATUObj.gfdbDelAtuEntry = Agate_gfdbDelAtuEntry;
	dev->SwitchDevObj.ATUObj.gfdbGetViolation = Agate_gfdbGetViolationIntf;
	dev->SwitchDevObj.ATUObj.gfdbFindAtuMacEntry = Agate_gfdbFindAtuMacEntryIntf;
	dev->SwitchDevObj.ATUObj.gfdbSetAgingTimeout = Agate_gfdbSetAgingTimeout;
	dev->SwitchDevObj.ATUObj.gfdbGetAgingTimeout = Agate_gfdbGetAgingTimeout;
	dev->SwitchDevObj.ATUObj.gfdbSetLearn2All = Agate_gfdbSetLearn2All;
	dev->SwitchDevObj.ATUObj.gfdbGetLearn2All = Agate_gfdbGetLearn2All;
	dev->SwitchDevObj.ATUObj.gfdbSetPortLearnLimit = Agate_gfdbSetPortLearnLimit;
	dev->SwitchDevObj.ATUObj.gfdbGetPortLearnLimit = Agate_gfdbGetPortLearnLimit;
	dev->SwitchDevObj.ATUObj.gfdbGetPortLearnCount = Agate_gfdbGetPortLearnCount;
	dev->SwitchDevObj.ATUObj.gfdbGetEntryCount = Agate_gfdbGetEntryCount;
	dev->SwitchDevObj.ATUObj.gfdbGetEntryCountPerFid = Agate_gfdbGetEntryCountPerFid;
	dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCount = Agate_gfdbGetNonStaticEntryCount;
	dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCountPerFid = Agate_gfdbGetNonStaticEntryCountPerFid;
	dev->SwitchDevObj.ATUObj.gfdbDump = Agate_gfdbDump;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreFcs = Agate_gprtSetIgnoreFcs;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreFcs = Agate_gprtGetIgnoreFcs;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetAllowBadFcs = Agate_gprtSetAllowBadFcs;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetAllowBadFcs = Agate_gprtGetAllowBadFcs;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitOut = Agate_gprtSetPauseLimitOut;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitOut = Agate_gprtGetPauseLimitOut;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitIn = Agate_gprtSetPauseLimitIn;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitIn = Agate_gprtGetPauseLimitIn;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetOutQueueSize = Agate_gprtGetOutQueueSize;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMapDA = Agate_gprtSetMapDA;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMapDA = Agate_gprtGetMapDA;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPortLocked = Agate_gprtSetPortLocked;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPortLocked = Agate_gprtGetPortLocked;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetHoldAt1 = Agate_gprtSetHoldAt1;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetHoldAt1 = Agate_gprtGetHoldAt1;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetInt0nAgeOut = Agate_gprtSetInt0nAgeOut;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetInt0nAgeOut = Agate_gprtGetInt0nAgeOut;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetRefreshLocked = Agate_gprtSetRefreshLocked;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetRefreshLocked = Agate_gprtGetRefreshLocked;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreWrongData = Agate_gprtSetIgnoreWrongData;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreWrongData = Agate_gprtGetIgnoreWrongData;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetARPMirror = Agate_gprtSetARPMirror;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetARPMirror = Agate_gprtGetARPMirror;

	dev->SwitchDevObj.VCTObj.gvctGetAdvCableDiag = Agate_gvctGetAdvCableDiagIntf;
	dev->SwitchDevObj.VCTObj.gvctGetAdvExtendedStatus = Agate_gvctGetAdvExtendedStatusIntf;

	dev->SwitchDevObj.STUObj.gstuAddEntry = Agate_gstuAddEntryIntf;
	dev->SwitchDevObj.STUObj.gstuDelEntry = Agate_gstuDelEntryIntf;
	dev->SwitchDevObj.STUObj.gstuFindSidEntry = Agate_gstuFindSidEntryIntf;
	dev->SwitchDevObj.STUObj.gstuGetEntryNext = Agate_gstuGetEntryNextIntf;
	dev->SwitchDevObj.STUObj.gstuGetEntryCount = Agate_gstuGetEntryCountIntf;
	dev->SwitchDevObj.STUObj.gstuDump = Agate_gstuDump;

	dev->SwitchDevObj.VTUObj.gvtuFindVidEntry = Agate_gvtuFindVidEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuExistVidEntry = Agate_gvtuExistVidEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuFlush = Agate_gvtuFlushIntf;
	dev->SwitchDevObj.VTUObj.gvtuAddEntry = Agate_gvtuAddEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetEntryNext = Agate_gvtuGetEntryNextIntf;
	dev->SwitchDevObj.VTUObj.gvtuDelEntry = Agate_gvtuDelEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetViolation = Agate_gvtuGetViolationIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetEntryCount = Agate_gvtuGetEntryCountIntf;
	dev->SwitchDevObj.VTUObj.gvtuSetMemberTag = Agate_gvtuSetMemberTagIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetMemberTag = Agate_gvtuGetMemberTagIntf;
	dev->SwitchDevObj.VTUObj.gvtuDump = Agate_gvtuDump;

	dev->SwitchDevObj.QOSMAPObj.gprtGetDefQpri = Agate_gprtGetDefQpriIntf;
	dev->SwitchDevObj.QOSMAPObj.gprtGetInitialPri = Agate_gprtGetInitialPriIntf;
	dev->SwitchDevObj.QOSMAPObj.gprtSetDefQpri = Agate_gprtSetDefQpriIntf;
	dev->SwitchDevObj.QOSMAPObj.gprtSetDefFpri = Agate_gprtSetDefFpri;
	dev->SwitchDevObj.QOSMAPObj.gprtGetDefFpri = Agate_gprtGetDefFpri;
	dev->SwitchDevObj.QOSMAPObj.gqosGetPrioMapRule = Agate_gqosGetPrioMapRule;
	dev->SwitchDevObj.QOSMAPObj.gprtSetInitialPri = Agate_gprtSetInitialPriIntf;
	dev->SwitchDevObj.QOSMAPObj.gqosSetPrioMapRule = Agate_gqosSetPrioMapRuleIntf;

	dev->SwitchDevObj.PHYCTRLObj.gphyReset = Agate_gphyReset;
    dev->SwitchDevObj.PHYCTRLObj.serdesReset = Agate_serdesReset;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortLoopback = Agate_gphySetPortLoopback;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortSpeed = Agate_gphySetPortSpeedIntf;
	dev->SwitchDevObj.PHYCTRLObj.gphyPortPowerDown = Agate_gphyPortPowerDown;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortDuplexMode = Agate_gphySetPortDuplexMode;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortAutoMode = Agate_gphySetPortAutoModeIntf;
	dev->SwitchDevObj.PHYCTRLObj.gphySetEEE = Agate_gphySetEEE;

	dev->SwitchDevObj.PIRLObj.gpirlInitialize = Agate_gpirlInitializeIntf;
	dev->SwitchDevObj.PIRLObj.gpirlInitResource = Agate_gpirlInitResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlCustomSetup = Agate_gpirlCustomSetupIntf;
	dev->SwitchDevObj.PIRLObj.gpirlReadResource = Agate_gpirlReadResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlWriteResource = Agate_gpirlWriteResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlGetResReg = Agate_gpirlGetResRegIntf;
	dev->SwitchDevObj.PIRLObj.gpirlSetResReg = Agate_gpirlSetResRegIntf;
	dev->SwitchDevObj.PIRLObj.gpirlAdvConfig = Agate_gpirlAdvConfigIntf;
	dev->SwitchDevObj.PIRLObj.gpirlAdvConfigDisable = Agate_gpirlAdvConfigDisableIntf;

	dev->SwitchDevObj.PORTCTRLObj.gprtSetForceDefaultVid = Agate_gprtSetForceDefaultVid;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceDefaultVid = Agate_gprtGetForceDefaultVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVid = Agate_gvlnSetPortVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVid = Agate_gvlnGetPortVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVlanDot1qMode = Agate_gvlnSetPortVlanDot1qModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVlanDot1qMode = Agate_gvlnGetPortVlanDot1qModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardTagged = Agate_gprtSetDiscardTaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardTagged = Agate_gprtGetDiscardTaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardUntagged = Agate_gprtSetDiscardUntaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardUntagged = Agate_gprtGetDiscardUntaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetUnicastFloodBlock = Agate_gprtSetUnicastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetUnicastFloodBlock = Agate_gprtGetUnicastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMulticastFloodBlock = Agate_gprtSetMulticastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMulticastFloodBlock = Agate_gprtGetMulticastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardBCastMode = Agate_gprtSetDiscardBCastModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardBCastMode = Agate_gprtGetDiscardBCastModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIGMPSnoopEnable = Agate_gprtSetIGMPSnoopEnable;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIGMPSnoopEnable = Agate_gprtGetIGMPSnoopEnable;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetHeaderMode = Agate_gprtSetHeaderMode;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetHeaderMode = Agate_gprtGetHeaderMode;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDropOnLock = Agate_gprtSetDropOnLock;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDropOnLock = Agate_gprtGetDropOnLock;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetJumboMode = Agate_gprtSetJumboModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetJumboMode = Agate_gprtGetJumboModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetLearnEnable = Agate_gprtSetLearnEnableIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetLearnEnable = Agate_gprtGetLearnEnableIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetVTUPriorityOverride = Agate_gprtSetVTUPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetVTUPriorityOverride = Agate_gprtGetVTUPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetSAPriorityOverride = Agate_gprtSetSAPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetSAPriorityOverride = Agate_gprtGetSAPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDAPriorityOverride = Agate_gprtSetDAPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDAPriorityOverride = Agate_gprtGetDAPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMessagePort = Agate_gprtSetMessagePort;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMessagePort = Agate_gprtGetMessagePort;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEtherType = Agate_gprtSetEtherType;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEtherType = Agate_gprtGetEtherType;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetFlowCtrl = Agate_gprtSetFlowCtrlIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetFlowCtrl = Agate_gprtGetFlowCtrlIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMonitorSource = Agate_gprtSetEgressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMonitorSource = Agate_gprtGetEgressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIngressMonitorSource = Agate_gprtSetIngressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIngressMonitorSource = Agate_gprtGetIngressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.grcSetEgressRate = Agate_grcSetEgressRateIntf;
	dev->SwitchDevObj.PORTCTRLObj.grcGetEgressRate = Agate_grcGetEgressRateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplexStatus = Agate_gprtGetDuplexStatusIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDuplex = Agate_gprtSetDuplexIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplex = Agate_gprtGetDuplexIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceLink = Agate_gprtGetForceLinkIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetForceLink = Agate_gprtSetForceLinkIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetLinkState = Agate_gprtGetLinkState;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetForceSpeed = Agate_gprtSetForceSpeedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceSpeed = Agate_gprtGetForceSpeedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetSpeed = Agate_gprtGetSpeedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetFrameMode = Agate_gprtSetFrameModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetFrameMode = Agate_gprtGetFrameModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMode = Agate_gprtSetEgressModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMode = Agate_gprtGetEgressModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetVlanPorts = Agate_gprtSetVlanPorts;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetVlanPorts = Agate_gprtGetVlanPorts;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPortState = Agate_gprtSetPortStateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPortState = Agate_gprtGetPortStateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtRegDump = Agate_gprtRegDump;

	dev->SwitchDevObj.TrunkObj.gprtSetTrunkPort = Agate_gprtSetTrunkPort;
	dev->SwitchDevObj.TrunkObj.gprtGetTrunkPort = Agate_gprtGetTrunkPort;
	dev->SwitchDevObj.TrunkObj.gsysSetTrunkMaskTable = Agate_gsysSetTrunkMaskTable;
	dev->SwitchDevObj.TrunkObj.gsysGetTrunkMaskTable = Agate_gsysGetTrunkMaskTable;
	dev->SwitchDevObj.TrunkObj.gsysSetHashTrunk = Agate_gsysSetHashTrunk;
	dev->SwitchDevObj.TrunkObj.gsysGetHashTrunk = Agate_gsysGetHashTrunk;
	dev->SwitchDevObj.TrunkObj.gsysSetTrunkRouting = Agate_gsysSetTrunkRouting;
	dev->SwitchDevObj.TrunkObj.gsysGetTrunkRouting = Agate_gsysGetTrunkRouting;

	dev->SwitchDevObj.PORTRMONObj.gstatsFlushAll = Agate_gstatsFlushAll;
	dev->SwitchDevObj.PORTRMONObj.gstatsFlushPort = Agate_gstatsFlushPort;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetPortCounter = Agate_gstatsGetPortCounterIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetPortAllCounters = Agate_gstatsGetPortAllCountersIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetHistogramMode = Agate_gstatsGetHistogramModeIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsSetHistogramMode = Agate_gstatsSetHistogramModeIntf;
	dev->SwitchDevObj.PORTRMONObj.gmibDump = Agate_gmibDump;

	dev->SwitchDevObj.QUEUEObj.gprtSetPortSched = Agate_gprtSetPortSchedIntf;
	dev->SwitchDevObj.QUEUEObj.gprtGetPortSched = Agate_gprtGetPortSchedIntf;
	dev->SwitchDevObj.QUEUEObj.gsysSetQoSWeight = Agate_gsysSetQoSWeightIntf;
	dev->SwitchDevObj.QUEUEObj.gsysGetQoSWeight = Agate_gsysGetQoSWeightIntf;

	dev->SwitchDevObj.SYSCTRLObj.gsysSetFloodBC = Agate_gsysSetFloodBC;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetFloodBC = Agate_gsysGetFloodBC;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetCPUDest = Agate_gsysSetCPUDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetCPUDest = Agate_gsysGetCPUDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetIngressMonitorDest = Agate_gsysSetIngressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetIngressMonitorDest = Agate_gsysGetIngressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetEgressMonitorDest = Agate_gsysSetEgressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetEgressMonitorDest = Agate_gsysGetEgressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetRsvd2CpuEnables = Agate_gsysSetRsvd2CpuEnables;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetRsvd2CpuEnables = Agate_gsysGetRsvd2CpuEnables;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetMGMTPri = Agate_gsysSetMGMTPri;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetMGMTPri = Agate_gsysGetMGMTPri;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal1RegDump = Agate_gsysGlobal1RegDump;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal2RegDump = Agate_gsysGlobal2RegDump;

	dev->SwitchDevObj.QAVObj.gqavGetPortQpriXHiLimit = Agate_gqavGetPortQpriXHiLimit;
	dev->SwitchDevObj.QAVObj.gqavSetPortQpriXHiLimit = Agate_gqavSetPortQpriXHiLimit;
	dev->SwitchDevObj.QAVObj.gqavGetPortQpriXRate = Agate_gqavGetPortQpriXRate;
	dev->SwitchDevObj.QAVObj.gqavSetPortQpriXRate = Agate_gqavSetPortQpriXRate;
	dev->SwitchDevObj.TCAMObj.gtcamSetMode = Agate_gtcamSetModeIntf;
	dev->SwitchDevObj.TCAMObj.gtcamGetMode = Agate_gtcamGetModeIntf;
	dev->SwitchDevObj.TCAMObj.gtcamAddEntryPort = Agate_gtcamAddEntryPort;
	dev->SwitchDevObj.TCAMObj.gtcamRemoveEntryPort = Agate_gtcamRemoveEntryPort;

	dev->SwitchDevObj.PTPObj.gptpSetIntEnable = Agate_gptpSetIntEnableIntf;
	dev->SwitchDevObj.PTPObj.gptpGetIntEnable = Agate_gptpGetIntEnableIntf;
	dev->SwitchDevObj.PTPObj.gptpSetProtPTPEn = Agate_gptpSetProtPTPEn;
	dev->SwitchDevObj.PTPObj.gptpGetProtPTPEn = Agate_gptpGetProtPTPEn;
	dev->SwitchDevObj.PTPObj.gptpSetPortConfig = Agate_gptpSetPortConfigIntf;
	dev->SwitchDevObj.PTPObj.gptpGetPortConfig = Agate_gptpGetPortConfigIntf;
	dev->SwitchDevObj.PTPObj.gptpGetTimeStamp = Agate_gptpGetTimeStampIntf;
	dev->SwitchDevObj.PTPObj.gptpGetTSValidSt = Agate_gptpGetTSValidSt;
	dev->SwitchDevObj.PTPObj.gptpResetTimeStamp = Agate_gptpResetTimeStamp;
	dev->SwitchDevObj.PTPObj.gptpSetGlobalConfig = Agate_gptpSetGlobalConfigIntf;
	dev->SwitchDevObj.PTPObj.gptpGetGlobalConfig = Agate_gptpGetGlobalConfigIntf;
	dev->SwitchDevObj.PTPObj.gptpGetIntStatus = Agate_gptpGetIntStatusIntf;

	dev->SwitchDevObj.TCAMObj.gtcamFindEntry = Agate_gtcamFindEntryIntf;
	dev->SwitchDevObj.TCAMObj.gtcamFlushAll = Agate_gtcamFlushAll;
	dev->SwitchDevObj.TCAMObj.gtcamFlushEntry = Agate_gtcamFlushEntry;
	dev->SwitchDevObj.TCAMObj.gtcamLoadEntry = Agate_gtcamLoadEntryIntf;
	dev->SwitchDevObj.TCAMObj.gtcamReadTCAMData = Agate_gtcamReadTCAMDataIntf;
	dev->SwitchDevObj.TCAMObj.gtcamGetNextTCAMData = Agate_gtcamGetNextTCAMDataIntf;
	dev->SwitchDevObj.TCAMObj.gtcamDump = Agate_gtcamDump;
	dev->SwitchDevObj.TCAMObj.gtcamAdvConfig = Agate_gtcamAdvConfigIntf;
	
	dev->SwitchDevObj.EEPROMObj.geepromWrite = Agate_EEPROMWriteIntf;
	dev->SwitchDevObj.EEPROMObj.geepromRead = Agate_EEPROMReadIntf;

	dev->SwitchDevObj.RMUObj.grmuGetID = Agate_msdRmuGetIDIntf;
	dev->SwitchDevObj.RMUObj.grmuAtuDump = Agate_msdRmuAtuDumpIntf;
	dev->SwitchDevObj.RMUObj.grmuMibDump = Agate_msdRmuMibDumpIntf;
	dev->SwitchDevObj.RMUObj.grmuMultiRegAccess = Agate_msdRmuMultiRegAccessIntf;
    dev->SwitchDevObj.RMUObj.grmuRegDump = Agate_msdRmuRegDump;

	dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIC45PhyReg = Agate_msdGetSMIC45PhyReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIC45PhyReg = Agate_msdSetSMIC45PhyReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyReg = Agate_msdGetSMIPhyReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyReg = Agate_msdSetSMIPhyReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyXMDIOReg = Agate_msdSetSMIPhyXMDIOReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyXMDIOReg = Agate_msdGetSMIPhyXMDIOReg;

	dev->SwitchDevObj.LEDObj.gledForceOn = Agate_gledForceOnIntf;
	dev->SwitchDevObj.LEDObj.gledForceOff = Agate_gledForceOffIntf;
	dev->SwitchDevObj.LEDObj.gledModeSet = Agate_gledModeSetIntf;
	dev->SwitchDevObj.LEDObj.gledModeGet = Agate_gledModeGetIntf;
	dev->SwitchDevObj.LEDObj.gledBlinkRateSet = Agate_gledBlinkRateSetIntf;
	dev->SwitchDevObj.LEDObj.gledBlinkRateGet = Agate_gledBlinkRateGetIntf;
	dev->SwitchDevObj.LEDObj.gledPulseStretchSet = Agate_gledPulseStretchSetIntf;
	dev->SwitchDevObj.LEDObj.gledPulseStretchGet = Agate_gledPulseStretchGetIntf;
	dev->SwitchDevObj.LEDObj.gledSpecialModeSet = Agate_gledSpecialModeSetIntf;
	dev->SwitchDevObj.LEDObj.gledSpecialModeGet = Agate_gledSpecialModeGetIntf;
	dev->SwitchDevObj.LEDObj.gledCtrlSet = Agate_gledCtrlSetIntf;
	dev->SwitchDevObj.LEDObj.gledCtrlGet = Agate_gledCtrlGetIntf;
#endif
	return MSD_OK;
}

static MSD_STATUS InitDevObj_Amethyst(MSD_QD_DEV *dev)
{
#ifdef AMETHYST_BUILD_IN
	dev->SwitchDevObj.ATUObj.gfdbAddMacEntry = Amethyst_gfdbAddMacEntryIntf;
	dev->SwitchDevObj.ATUObj.gfdbGetAtuEntryNext = Amethyst_gfdbGetAtuEntryNextIntf;
	dev->SwitchDevObj.ATUObj.gfdbFlush = Amethyst_gfdbFlushIntf;
	dev->SwitchDevObj.ATUObj.gfdbFlushInDB = Amethyst_gfdbFlushInDBIntf;
	dev->SwitchDevObj.ATUObj.gfdbMove = Amethyst_gfdbMoveIntf;
	dev->SwitchDevObj.ATUObj.gfdbMoveInDB = Amethyst_gfdbMoveInDBIntf;
	dev->SwitchDevObj.ATUObj.gfdbPortRemove = Amethyst_gfdbPortRemoveIntf;
	dev->SwitchDevObj.ATUObj.gfdbPortRemoveInDB = Amethyst_gfdbPortRemoveInDBIntf;
	dev->SwitchDevObj.ATUObj.gfdbDelAtuEntry = Amethyst_gfdbDelAtuEntry;
	dev->SwitchDevObj.ATUObj.gfdbGetViolation = Amethyst_gfdbGetViolationIntf;
	dev->SwitchDevObj.ATUObj.gfdbFindAtuMacEntry = Amethyst_gfdbFindAtuMacEntryIntf;
	dev->SwitchDevObj.ATUObj.gfdbSetAgingTimeout = Amethyst_gfdbSetAgingTimeout;
	dev->SwitchDevObj.ATUObj.gfdbGetAgingTimeout = Amethyst_gfdbGetAgingTimeout;
	dev->SwitchDevObj.ATUObj.gfdbSetLearn2All = Amethyst_gfdbSetLearn2All;
	dev->SwitchDevObj.ATUObj.gfdbGetLearn2All = Amethyst_gfdbGetLearn2All;
	dev->SwitchDevObj.ATUObj.gfdbSetPortLearnLimit = Amethyst_gfdbSetPortLearnLimit;
	dev->SwitchDevObj.ATUObj.gfdbGetPortLearnLimit = Amethyst_gfdbGetPortLearnLimit;
	dev->SwitchDevObj.ATUObj.gfdbGetPortLearnCount = Amethyst_gfdbGetPortLearnCount;
	dev->SwitchDevObj.ATUObj.gfdbGetEntryCount = Amethyst_gfdbGetEntryCount;
	dev->SwitchDevObj.ATUObj.gfdbGetEntryCountPerFid = Amethyst_gfdbGetEntryCountPerFid;
	dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCount = Amethyst_gfdbGetNonStaticEntryCount;
	dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCountPerFid = Amethyst_gfdbGetNonStaticEntryCountPerFid;
	dev->SwitchDevObj.ATUObj.gfdbDump = Amethyst_gfdbDump;

	/* Amethyst deviceId MSD_88E6193X and MSD_88E6191X doesn't support Route, TSN, CutThrough */
	if ((dev->deviceId != MSD_88E6191X) && (dev->deviceId != MSD_88E6193X))
	{
		dev->SwitchDevObj.ARPObj.garpLoadMcEntry = Amethyst_garpLoadMcEntryIntf;
		dev->SwitchDevObj.ARPObj.garpLoadUcEntry = Amethyst_garpLoadUcEntryIntf;
		dev->SwitchDevObj.ARPObj.garpFlushEntry = Amethyst_garpFlushEntry;
		dev->SwitchDevObj.ARPObj.garpFlushAll = Amethyst_garpFlushAll;
		dev->SwitchDevObj.ARPObj.garpReadEntry = Amethyst_garpReadEntryIntf;
		dev->SwitchDevObj.ARPObj.garpGetNextEntry = Amethyst_garpGetNextEntryIntf;

		dev->SwitchDevObj.QAVObj.gqavGetPortQpriXHiLimit = Amethyst_gqavGetPortQpriXHiLimit;
		dev->SwitchDevObj.QAVObj.gqavSetPortQpriXHiLimit = Amethyst_gqavSetPortQpriXHiLimit;
		dev->SwitchDevObj.QAVObj.gqavGetPortQpriXRate = Amethyst_gqavGetPortQpriXRate;
		dev->SwitchDevObj.QAVObj.gqavSetPortQpriXRate = Amethyst_gqavSetPortQpriXRate;

		dev->SwitchDevObj.PTPObj.gptpSetIntEnable = Amethyst_gptpSetIntEnableIntf;
		dev->SwitchDevObj.PTPObj.gptpGetIntEnable = Amethyst_gptpGetIntEnableIntf;
		dev->SwitchDevObj.PTPObj.gptpSetProtPTPEn = Amethyst_gptpSetProtPTPEn;
		dev->SwitchDevObj.PTPObj.gptpGetProtPTPEn = Amethyst_gptpGetProtPTPEn;
		dev->SwitchDevObj.PTPObj.gptpSetPortConfig = Amethyst_gptpSetPortConfigIntf;
		dev->SwitchDevObj.PTPObj.gptpGetPortConfig = Amethyst_gptpGetPortConfigIntf;
		dev->SwitchDevObj.PTPObj.gptpGetTimeStamp = Amethyst_gptpGetTimeStampIntf;
		dev->SwitchDevObj.PTPObj.gptpGetTSValidSt = Amethyst_gptpGetTSValidSt;
		dev->SwitchDevObj.PTPObj.gptpResetTimeStamp = Amethyst_gptpResetTimeStamp;
		dev->SwitchDevObj.PTPObj.gptpSetGlobalConfig = Amethyst_gptpSetGlobalConfigIntf;
		dev->SwitchDevObj.PTPObj.gptpGetGlobalConfig = Amethyst_gptpGetGlobalConfigIntf;
		dev->SwitchDevObj.PTPObj.gptpGetIntStatus = Amethyst_gptpGetIntStatusIntf;
		dev->SwitchDevObj.PTPObj.gptpSetMeanPathDelay = Amethyst_gptpSetMeanPathDelay;
		dev->SwitchDevObj.PTPObj.gptpGetMeanPathDelay = Amethyst_gptpGetMeanPathDelay;
		dev->SwitchDevObj.PTPObj.gptpSetIgrPathDelayAsym = Amethyst_gptpSetIgrPathDelayAsym;
		dev->SwitchDevObj.PTPObj.gptpGetIgrPathDelayAsym = Amethyst_gptpGetIgrPathDelayAsym;
		dev->SwitchDevObj.PTPObj.gptpSetEgrPathDelayAsym = Amethyst_gptpSetEgrPathDelayAsym;
		dev->SwitchDevObj.PTPObj.gptpGetEgrPathDelayAsym = Amethyst_gptpGetEgrPathDelayAsym;
		dev->SwitchDevObj.PTPObj.gptpSetMode = Amethyst_gptpSetModeIntf;
		dev->SwitchDevObj.PTPObj.gptpGetMode = Amethyst_gptpGetModeIntf;
		dev->SwitchDevObj.PTPObj.gptpSetMasterSlave = Amethyst_gptpSetMasterSlaveIntf;
		dev->SwitchDevObj.PTPObj.gptpGetMasterSlave = Amethyst_gptpGetMasterSlaveIntf;
		dev->SwitchDevObj.PTPObj.gptpGetTODBusyBitStatus = Amethyst_gptpGetTODBusyBitStatus;
		dev->SwitchDevObj.PTPObj.gptpTODStoreCompensation = Amethyst_gptpTODStoreCompensationIntf;
		dev->SwitchDevObj.PTPObj.gptpTODStoreAll = Amethyst_gptpTODStoreAllIntf;
		dev->SwitchDevObj.PTPObj.gptpTODCaptureAll = Amethyst_gptpTODCaptureAllIntf;
		dev->SwitchDevObj.PTPObj.gptpSetPulse = Amethyst_gptpSetPulseIntf;
		dev->SwitchDevObj.PTPObj.gptpGetPulse = Amethyst_gptpGetPulseIntf;
		dev->SwitchDevObj.PTPObj.gtaiGetPTPGlobalTime = Amethyst_gtaiGetPTPGlobalTime;

		dev->SwitchDevObj.PORTCTRLObj.gprtSetCutThrouthEnable = Amethyst_gprtSetCutThrouthEnableIntf;
		dev->SwitchDevObj.PORTCTRLObj.gprtGetCutThrouthEnable = Amethyst_gprtGetCutThrouthEnableIntf;
	}
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreFcs = Amethyst_gprtSetIgnoreFcs;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreFcs = Amethyst_gprtGetIgnoreFcs;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetAllowBadFcs = Amethyst_gprtSetAllowBadFcs;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetAllowBadFcs = Amethyst_gprtGetAllowBadFcs;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitOut = Amethyst_gprtSetPauseLimitOut;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitOut = Amethyst_gprtGetPauseLimitOut;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitIn = Amethyst_gprtSetPauseLimitIn;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitIn = Amethyst_gprtGetPauseLimitIn;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetQueueToPause = Amethyst_gprtSetQueueToPause;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetQueueToPause = Amethyst_gprtGetQueueToPause;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetRxPriorityFlowControlEnable = Amethyst_gprtSetRxPriorityFlowControlEnable;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetRxPriorityFlowControlEnable = Amethyst_gprtGetRxPriorityFlowControlEnable;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetRxPriorityFlowControlToQueue = Amethyst_gprtSetRxPriorityFlowControlToQueue;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetRxPriorityFlowControlToQueue = Amethyst_gprtGetRxPriorityFlowControlToQueue;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetTxPriorityFlowControlEnable = Amethyst_gprtSetTxPriorityFlowControlEnable;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetTxPriorityFlowControlEnable = Amethyst_gprtGetTxPriorityFlowControlEnable;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetOutQueueSize = Amethyst_gprtGetOutQueueSize;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMapDA = Amethyst_gprtSetMapDA;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMapDA = Amethyst_gprtGetMapDA;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPortLocked = Amethyst_gprtSetPortLocked;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPortLocked = Amethyst_gprtGetPortLocked;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetHoldAt1 = Amethyst_gprtSetHoldAt1;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetHoldAt1 = Amethyst_gprtGetHoldAt1;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetInt0nAgeOut = Amethyst_gprtSetInt0nAgeOut;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetInt0nAgeOut = Amethyst_gprtGetInt0nAgeOut;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetRefreshLocked = Amethyst_gprtSetRefreshLocked;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetRefreshLocked = Amethyst_gprtGetRefreshLocked;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreWrongData = Amethyst_gprtSetIgnoreWrongData;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreWrongData = Amethyst_gprtGetIgnoreWrongData;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetARPMirror = Amethyst_gprtSetARPMirror;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetARPMirror = Amethyst_gprtGetARPMirror;

	dev->SwitchDevObj.VCTObj.gvctGetAdvCableDiag = Amethyst_gvctGetAdvCableDiagIntf;
	dev->SwitchDevObj.VCTObj.gvctGetAdvExtendedStatus = Amethyst_gvctGetAdvExtendedStatusIntf;

	dev->SwitchDevObj.STUObj.gstuAddEntry = Amethyst_gstuAddEntryIntf;
	dev->SwitchDevObj.STUObj.gstuGetEntryNext = Amethyst_gstuGetEntryNextIntf;
	dev->SwitchDevObj.STUObj.gstuDelEntry = Amethyst_gstuDelEntryIntf;
	dev->SwitchDevObj.STUObj.gstuFindSidEntry = Amethyst_gstuFindSidEntryIntf;
	dev->SwitchDevObj.STUObj.gstuGetEntryCount = Amethyst_gstuGetEntryCountIntf;
	dev->SwitchDevObj.STUObj.gstuDump = Amethyst_gstuDump;

	dev->SwitchDevObj.VTUObj.gvtuFindVidEntry = Amethyst_gvtuFindVidEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuExistVidEntry = Amethyst_gvtuExistVidEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuFlush = Amethyst_gvtuFlushIntf;
	dev->SwitchDevObj.VTUObj.gvtuAddEntry = Amethyst_gvtuAddEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetEntryNext = Amethyst_gvtuGetEntryNextIntf;
	dev->SwitchDevObj.VTUObj.gvtuDelEntry = Amethyst_gvtuDelEntryIntf;
	dev->SwitchDevObj.VTUObj.gvtuSetMode = Amethyst_gvtuSetModeIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetMode = Amethyst_gvtuGetModeIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetViolation = Amethyst_gvtuGetViolationIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetEntryCount = Amethyst_gvtuGetEntryCountIntf;
	dev->SwitchDevObj.VTUObj.gvtuSetMemberTag = Amethyst_gvtuSetMemberTagIntf;
	dev->SwitchDevObj.VTUObj.gvtuGetMemberTag = Amethyst_gvtuGetMemberTagIntf;
	dev->SwitchDevObj.VTUObj.gvtuDump = Amethyst_gvtuDump;

	dev->SwitchDevObj.TrunkObj.gprtSetTrunkPort = Amethyst_gprtSetTrunkPort;
	dev->SwitchDevObj.TrunkObj.gprtGetTrunkPort = Amethyst_gprtGetTrunkPort;
	dev->SwitchDevObj.TrunkObj.gsysSetTrunkMaskTable = Amethyst_gsysSetTrunkMaskTable;
	dev->SwitchDevObj.TrunkObj.gsysGetTrunkMaskTable = Amethyst_gsysGetTrunkMaskTable;
	dev->SwitchDevObj.TrunkObj.gsysSetHashTrunk = Amethyst_gsysSetHashTrunk;
	dev->SwitchDevObj.TrunkObj.gsysGetHashTrunk = Amethyst_gsysGetHashTrunk;
	dev->SwitchDevObj.TrunkObj.gsysSetTrunkRouting = Amethyst_gsysSetTrunkRouting;
	dev->SwitchDevObj.TrunkObj.gsysGetTrunkRouting = Amethyst_gsysGetTrunkRouting;

	dev->SwitchDevObj.IMPObj.impRun = Amethyst_impRunIntf;
	dev->SwitchDevObj.IMPObj.impReset = Amethyst_impResetIntf;
	dev->SwitchDevObj.IMPObj.impLoadToRAM = Amethyst_impLoadToRAMIntf;
	dev->SwitchDevObj.IMPObj.impStop = Amethyst_impStopIntf;

	dev->SwitchDevObj.EEPROMObj.geepromWrite = Amethyst_EEPROMWriteIntf;
	dev->SwitchDevObj.EEPROMObj.geepromRead = Amethyst_EEPROMReadIntf;
	dev->SwitchDevObj.EEPROMObj.geepromSetMChipSel = Amethyst_EEPROMChipSelSetIntf;
	dev->SwitchDevObj.EEPROMObj.geepromGetMChipSel = Amethyst_EEPROMChipSelGetIntf;

	dev->SwitchDevObj.PHYCTRLObj.gphyReset = Amethyst_gphyReset;
    dev->SwitchDevObj.PHYCTRLObj.serdesReset = Amethyst_serdesReset;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortLoopback = Amethyst_gphySetPortLoopback;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortSpeed = Amethyst_gphySetPortSpeedIntf;
	dev->SwitchDevObj.PHYCTRLObj.gphyPortPowerDown = Amethyst_gphyPortPowerDown;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortDuplexMode = Amethyst_gphySetPortDuplexMode;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortAutoMode = Amethyst_gphySetPortAutoModeIntf;
	dev->SwitchDevObj.PHYCTRLObj.gphySetEEE = Amethyst_gphySetEEE;

	dev->SwitchDevObj.PIRLObj.gpirlInitialize = Amethyst_gpirlInitializeIntf;
	dev->SwitchDevObj.PIRLObj.gpirlInitResource = Amethyst_gpirlInitResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlCustomSetup = Amethyst_gpirlCustomSetup_sr2cIntf;
	dev->SwitchDevObj.PIRLObj.gpirlReadResource = Amethyst_gpirlReadResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlWriteResource = Amethyst_gpirlWriteResourceIntf;
	dev->SwitchDevObj.PIRLObj.gpirlGetResReg = Amethyst_gpirlGetResRegIntf;
	dev->SwitchDevObj.PIRLObj.gpirlSetResReg = Amethyst_gpirlSetResRegIntf;
	dev->SwitchDevObj.PIRLObj.gpirlAdvConfig = Amethyst_gpirlAdvConfigIntf;
	dev->SwitchDevObj.PIRLObj.gpirlAdvConfigDisable = Amethyst_gpirlAdvConfigDisableIntf;

	dev->SwitchDevObj.PORTCTRLObj.gprtSetForceDefaultVid = Amethyst_gprtSetForceDefaultVid;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceDefaultVid = Amethyst_gprtGetForceDefaultVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVid = Amethyst_gvlnSetPortVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVid = Amethyst_gvlnGetPortVid;
	dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVlanDot1qMode = Amethyst_gvlnSetPortVlanDot1qModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVlanDot1qMode = Amethyst_gvlnGetPortVlanDot1qModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardTagged = Amethyst_gprtSetDiscardTaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardTagged = Amethyst_gprtGetDiscardTaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardUntagged = Amethyst_gprtSetDiscardUntaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardUntagged = Amethyst_gprtGetDiscardUntaggedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetUnicastFloodBlock = Amethyst_gprtSetUnicastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetUnicastFloodBlock = Amethyst_gprtGetUnicastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMulticastFloodBlock = Amethyst_gprtSetMulticastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMulticastFloodBlock = Amethyst_gprtGetMulticastFloodBlockIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardBCastMode = Amethyst_gprtSetDiscardBCastModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardBCastMode = Amethyst_gprtGetDiscardBCastModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIGMPSnoopEnable = Amethyst_gprtSetIGMPSnoopEnable;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIGMPSnoopEnable = Amethyst_gprtGetIGMPSnoopEnable;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetHeaderMode = Amethyst_gprtSetHeaderMode;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetHeaderMode = Amethyst_gprtGetHeaderMode;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDropOnLock = Amethyst_gprtSetDropOnLock;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDropOnLock = Amethyst_gprtGetDropOnLock;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetJumboMode = Amethyst_gprtSetJumboModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetJumboMode = Amethyst_gprtGetJumboModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetLearnEnable = Amethyst_gprtSetLearnEnableIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetLearnEnable = Amethyst_gprtGetLearnEnableIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetVTUPriorityOverride = Amethyst_gprtSetVTUPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetVTUPriorityOverride = Amethyst_gprtGetVTUPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetSAPriorityOverride = Amethyst_gprtSetSAPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetSAPriorityOverride = Amethyst_gprtGetSAPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDAPriorityOverride = Amethyst_gprtSetDAPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDAPriorityOverride = Amethyst_gprtGetDAPriorityOverrideIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMessagePort = Amethyst_gprtSetMessagePort;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMessagePort = Amethyst_gprtGetMessagePort;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEtherType = Amethyst_gprtSetEtherType;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEtherType = Amethyst_gprtGetEtherType;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetAllowVidZero = Amethyst_gprtSetAllowVidZero;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetAllowVidZero = Amethyst_gprtGetAllowVidZero;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetFlowCtrl = Amethyst_gprtSetFlowCtrlIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetFlowCtrl = Amethyst_gprtGetFlowCtrlIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMonitorSource = Amethyst_gprtSetEgressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMonitorSource = Amethyst_gprtGetEgressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIngressMonitorSource = Amethyst_gprtSetIngressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIngressMonitorSource = Amethyst_gprtGetIngressMonitorSource;
	dev->SwitchDevObj.PORTCTRLObj.grcSetEgressRate = Amethyst_grcSetEgressRateIntf;
	dev->SwitchDevObj.PORTCTRLObj.grcGetEgressRate = Amethyst_grcGetEgressRateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplexStatus = Amethyst_gprtGetDuplexStatusIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDuplex = Amethyst_gprtSetDuplexIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplex = Amethyst_gprtGetDuplexIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceLink = Amethyst_gprtGetForceLinkIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetForceLink = Amethyst_gprtSetForceLinkIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetLinkState = Amethyst_gprtGetLinkState;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetForceSpeed = Amethyst_gprtSetForceSpeedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceSpeed = Amethyst_gprtGetForceSpeedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetSpeed = Amethyst_gprtGetSpeedIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetFrameMode = Amethyst_gprtSetFrameModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetFrameMode = Amethyst_gprtGetFrameModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPortECID = Amethyst_gprtSetPortECID;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPortECID = Amethyst_gprtGetPortECID;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMode = Amethyst_gprtSetEgressModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMode = Amethyst_gprtGetEgressModeIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreETag = Amethyst_gprtSetIgnoreETag;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreETag = Amethyst_gprtGetIgnoreETag;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetExtendedPortCtrlReg = Amethyst_gprtSetExtendedPortCtrlReg;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetExtendedPortCtrlReg = Amethyst_gprtGetExtendedPortCtrlReg;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetVlanPorts = Amethyst_gprtSetVlanPorts;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetVlanPorts = Amethyst_gprtGetVlanPorts;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPortState = Amethyst_gprtSetPortStateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPortState = Amethyst_gprtGetPortStateIntf;
	dev->SwitchDevObj.PORTCTRLObj.gprtRegDump = Amethyst_gprtRegDump;

	dev->SwitchDevObj.QOSMAPObj.gprtGetIEEEPrioMap = Amethyst_gprtGetIEEEPrioMapIntf;
	dev->SwitchDevObj.QOSMAPObj.gprtSetIEEEPrioMap = Amethyst_gprtSetIEEEPrioMapIntf;
	dev->SwitchDevObj.QOSMAPObj.gqosGetPrioMapRule = Amethyst_gqosGetPrioMapRuleIntf;
	dev->SwitchDevObj.QOSMAPObj.gprtGetInitialPri = Amethyst_gprtGetInitialPriIntf;
	dev->SwitchDevObj.QOSMAPObj.gprtSetDefQpri = Amethyst_gprtSetDefQpriIntf;
	dev->SwitchDevObj.QOSMAPObj.gprtGetDefQpri = Amethyst_gprtGetDefQpriIntf;
	dev->SwitchDevObj.QOSMAPObj.gprtSetDefFpri = Amethyst_gprtSetDefFpri;
	dev->SwitchDevObj.QOSMAPObj.gprtGetDefFpri = Amethyst_gprtGetDefFpri;
	dev->SwitchDevObj.QOSMAPObj.gprtSetInitialPri = Amethyst_gprtSetInitialPriIntf;
	dev->SwitchDevObj.QOSMAPObj.gqosSetPrioMapRule = Amethyst_gqosSetPrioMapRuleIntf;
	dev->SwitchDevObj.QOSMAPObj.gprtRewriteDscp = Amethyst_gprtRewriteDscp;
	dev->SwitchDevObj.QOSMAPObj.gprtSetCosToQueue = Amethyst_gprtSetCosToQueue;
	dev->SwitchDevObj.QOSMAPObj.gprtGetCosToQueue = Amethyst_gprtGetCosToQueue;
	dev->SwitchDevObj.QOSMAPObj.gprtSetCosToDscp = Amethyst_gprtSetCosToDscp;
	dev->SwitchDevObj.QOSMAPObj.gprtGetCosToDscp = Amethyst_gprtGetCosToDscp;

	dev->SwitchDevObj.PORTRMONObj.gstatsFlushAll = Amethyst_gstatsFlushAll;
	dev->SwitchDevObj.PORTRMONObj.gstatsFlushPort = Amethyst_gstatsFlushPort;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetPortCounter = Amethyst_gstatsGetPortCounterIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetPortAllCounters = Amethyst_gstatsGetPortAllCountersIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetHistogramMode = Amethyst_gstatsGetHistogramModeIntf;
	dev->SwitchDevObj.PORTRMONObj.gstatsSetHistogramMode = Amethyst_gstatsSetHistogramModeIntf;
	dev->SwitchDevObj.PORTRMONObj.gmibDump = Amethyst_gmibDump;

	dev->SwitchDevObj.QUEUEObj.gprtSetQueueCtrl = Amethyst_gprtSetQueueCtrlIntf;
	dev->SwitchDevObj.QUEUEObj.gprtGetQueueCtrl = Amethyst_gprtGetQueueCtrlIntf;
	dev->SwitchDevObj.QUEUEObj.gprtSetPortSched = Amethyst_gprtSetPortSchedIntf;
	dev->SwitchDevObj.QUEUEObj.gprtGetPortSched = Amethyst_gprtGetPortSchedIntf;
	dev->SwitchDevObj.QUEUEObj.gsysSetQoSWeight = Amethyst_gsysSetQoSWeightIntf;
	dev->SwitchDevObj.QUEUEObj.gsysGetQoSWeight = Amethyst_gsysGetQoSWeightIntf;

	dev->SwitchDevObj.SYSCTRLObj.gsysSetFloodBC = Amethyst_gsysSetFloodBC;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetFloodBC = Amethyst_gsysGetFloodBC;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetEgressMonitorDest = Amethyst_gsysSetEgressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetEgressMonitorDest = Amethyst_gsysGetEgressMonitorDest;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetSMISetup = Amethyst_gsysSetSMISetup;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetSMISetup = Amethyst_gsysGetSMISetup;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal1RegDump = Amethyst_gsysGlobal1RegDump;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal2RegDump = Amethyst_gsysGlobal2RegDump;

	/* Amethyst deviceId MSD_88E6191X doesn't support ECID, TCAM */
	if (dev->deviceId != MSD_88E6191X)
	{
		dev->SwitchDevObj.ECIDObj.gBpeEnableSet = Amethyst_gBpeEnableSet;
		dev->SwitchDevObj.ECIDObj.gBpeEnableGet = Amethyst_gBpeEnableGet;
		dev->SwitchDevObj.ECIDObj.gecidAddEntry = Amethyst_gecidAddEntryIntf;
		dev->SwitchDevObj.ECIDObj.gecidDump = Amethyst_gecidDump;
		dev->SwitchDevObj.ECIDObj.gecidFindEcidEntry = Amethyst_gecidFindEcidEntryIntf;
		dev->SwitchDevObj.ECIDObj.gecidFlushAll = Amethyst_gecidFlushAll;
		dev->SwitchDevObj.ECIDObj.gecidFlushEntry = Amethyst_gecidFlushEntry;
		dev->SwitchDevObj.ECIDObj.gecidGetEcidEntryNext = Amethyst_gecidGetEcidEntryNextIntf;
		dev->SwitchDevObj.ECIDObj.gecidGetEntryCount = Amethyst_gecidGetEntryCount;
		dev->SwitchDevObj.ECIDObj.gecidMove = Amethyst_gecidMove;

		dev->SwitchDevObj.TCAMObj.gtcamFlushAll = Amethyst_gtcamFlushAll;
		dev->SwitchDevObj.TCAMObj.gtcamFlushEntry = Amethyst_gtcamFlushEntry;
		dev->SwitchDevObj.TCAMObj.gtcamLoadEntry = Amethyst_gtcamLoadEntryIntf;
		dev->SwitchDevObj.TCAMObj.gtcamReadTCAMData = Amethyst_gtcamReadTCAMDataIntf;
		dev->SwitchDevObj.TCAMObj.gtcamGetNextTCAMData = Amethyst_gtcamGetNextTCAMDataIntf;
		dev->SwitchDevObj.TCAMObj.gtcamFindEntry = Amethyst_gtcamFindEntryIntf;
		dev->SwitchDevObj.TCAMObj.gtcamSetMode = Amethyst_gtcamSetModeIntf;
		dev->SwitchDevObj.TCAMObj.gtcamGetMode = Amethyst_gtcamGetModeIntf;
		dev->SwitchDevObj.TCAMObj.gtcamAddEntryPort = Amethyst_gtcamAddEntryPort;
		dev->SwitchDevObj.TCAMObj.gtcamRemoveEntryPort = Amethyst_gtcamRemoveEntryPort;
		dev->SwitchDevObj.TCAMObj.gtcamEgrFlushEntry = Amethyst_gtcamEgrFlushEntryIntf;
		dev->SwitchDevObj.TCAMObj.gtcamEgrFlushEntryAllPorts = Amethyst_gtcamEgrFlushEntryAllPortsIntf;
		dev->SwitchDevObj.TCAMObj.gtcamEgrLoadEntry = Amethyst_gtcamEgrLoadEntryIntf;
		dev->SwitchDevObj.TCAMObj.gtcamEgrGetNextTCAMData = Amethyst_gtcamEgrGetNextTCAMDataIntf;
		dev->SwitchDevObj.TCAMObj.gtcamEgrReadTCAMData = Amethyst_gtcamEgrReadTCAMDataIntf;
		dev->SwitchDevObj.TCAMObj.gtcamDump = Amethyst_gtcamDump;
		dev->SwitchDevObj.TCAMObj.gtcamAdvConfig = Amethyst_gtcamAdvConfigIntf;

		dev->SwitchDevObj.TCAMRCObj.gtcamRCLoadEntry = Amethyst_gtcamRCLoadEntryIntf;
		dev->SwitchDevObj.TCAMRCObj.gtcamRCFlushAllEntry = Amethyst_gtcamRCFlushAllEntry;
		dev->SwitchDevObj.TCAMRCObj.gtcamRCFlushEntry = Amethyst_gtcamRCFlushEntry;
		dev->SwitchDevObj.TCAMRCObj.gtcamRCReadEntry = Amethyst_gtcamRCReadEntryIntf;
		dev->SwitchDevObj.TCAMRCObj.gtcamRCGetNextEntry = Amethyst_gtcamRCGetNextEntryIntf;
		dev->SwitchDevObj.TCAMRCObj.gtcamRCFindEntry = Amethyst_gtcamRCFindEntryIntf;
		dev->SwitchDevObj.TCAMRCObj.gtcamRCEntryDump = Amethyst_gtcamRCEntryDump;

		dev->SwitchDevObj.TCAMRCCompObj.gtcamRCCompLoad = Amethyst_gtcamRCCompLoadIntf;
		dev->SwitchDevObj.TCAMRCCompObj.gtcamRCCompFlushAll = Amethyst_gtcamRCCompFlushAll;
		dev->SwitchDevObj.TCAMRCCompObj.gtcamRCCompFlushPort = Amethyst_gtcamRCCompFlushPort;
		dev->SwitchDevObj.TCAMRCCompObj.gtcamRCCompRead = Amethyst_gtcamRCCompReadIntf;

		dev->SwitchDevObj.RMUObj.grmuTCAMDump = Amethyst_msdRmuTCAMDumpIntf;
		dev->SwitchDevObj.RMUObj.grmuTCAMRead = Amethyst_msdRmuTCAMReadIntf;
		dev->SwitchDevObj.RMUObj.grmuTCAMWrite = Amethyst_msdRmuTCAMWriteIntf;
		dev->SwitchDevObj.RMUObj.grmuEcidDump = Amethyst_msdRmuEcidDumpIntf;
	}
	dev->SwitchDevObj.RMUObj.grmuGetID = Amethyst_msdRmuGetIDIntf;
	dev->SwitchDevObj.RMUObj.grmuAtuDump = Amethyst_msdRmuAtuDumpIntf;
	dev->SwitchDevObj.RMUObj.grmuMib2Dump = Amethyst_msdRmuMib2DumpIntf;
	dev->SwitchDevObj.RMUObj.grmuMultiRegAccess = Amethyst_msdRmuMultiRegAccessIntf;
    dev->SwitchDevObj.RMUObj.grmuRegDump = Amethyst_msdRmuRegDump;

	dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIC45PhyReg = Amethyst_msdGetSMIC45PhyReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIC45PhyReg = Amethyst_msdSetSMIC45PhyReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyReg = Amethyst_msdGetSMIPhyReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyReg = Amethyst_msdSetSMIPhyReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyXMDIOReg = Amethyst_msdSetSMIPhyXMDIOReg;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyXMDIOReg = Amethyst_msdGetSMIPhyXMDIOReg;

	dev->SwitchDevObj.LEDObj.gledForceOn = Amethyst_gledForceOnIntf;
	dev->SwitchDevObj.LEDObj.gledForceOff = Amethyst_gledForceOffIntf;
	dev->SwitchDevObj.LEDObj.gledLinkAct = Amethyst_gledLinkActIntf;
	dev->SwitchDevObj.LEDObj.gledModeSet = Amethyst_gledModeSetIntf;
	dev->SwitchDevObj.LEDObj.gledModeGet = Amethyst_gledModeGetIntf;
	dev->SwitchDevObj.LEDObj.gledBlinkRateSet = Amethyst_gledBlinkRateSetIntf;
	dev->SwitchDevObj.LEDObj.gledBlinkRateGet = Amethyst_gledBlinkRateGetIntf;
	dev->SwitchDevObj.LEDObj.gledPulseStretchSet = Amethyst_gledPulseStretchSetIntf;
	dev->SwitchDevObj.LEDObj.gledPulseStretchGet = Amethyst_gledPulseStretchGetIntf;
	dev->SwitchDevObj.LEDObj.gledSpecialModeSet = Amethyst_gledSpecialModeSetIntf;
	dev->SwitchDevObj.LEDObj.gledSpecialModeGet = Amethyst_gledSpecialModeGetIntf;
	dev->SwitchDevObj.LEDObj.gledCtrlSet = Amethyst_gledCtrlSetIntf;
	dev->SwitchDevObj.LEDObj.gledCtrlGet = Amethyst_gledCtrlGetIntf;

	dev->SwitchDevObj.POLICYObj.gpolicySetMgmtCtrl = Amethyst_gpolicySetMgmtCtrl;
	dev->SwitchDevObj.POLICYObj.gpolicyGetMgmtCtrl = Amethyst_gpolicyGetMgmtCtrl;
	dev->SwitchDevObj.POLICYObj.gpolicySetCpuDest = Amethyst_gpolicySetCpuDest;
	dev->SwitchDevObj.POLICYObj.gpolicyGetCpuDest = Amethyst_gpolicyGetCpuDest;
	dev->SwitchDevObj.POLICYObj.gpolicySetIngressMonitorDest = Amethyst_gpolicySetIngressMonitorDest;
	dev->SwitchDevObj.POLICYObj.gpolicyGetIngressMonitorDest = Amethyst_gpolicyGetIngressMonitorDest;
	dev->SwitchDevObj.POLICYObj.gpolicySetRsvd2CpuEnables = Amethyst_gpolicySetRsvd2CpuEnables;
	dev->SwitchDevObj.POLICYObj.gpolicyGetRsvd2CpuEnables = Amethyst_gpolicyGetRsvd2CpuEnables;
	dev->SwitchDevObj.POLICYObj.gpolicySetMGMTPri = Amethyst_gpolicySetMGMTPri;
	dev->SwitchDevObj.POLICYObj.gpolicyGetMGMTPri = Amethyst_gpolicyGetMGMTPri;
	dev->SwitchDevObj.POLICYObj.gpolicySetRouteDest = Amethyst_gpolicySetRouteDest;
	dev->SwitchDevObj.POLICYObj.gpolicyGetRouteDest = Amethyst_gpolicyGetRouteDest;
	dev->SwitchDevObj.POLICYObj.gpolicySetMirrorDest = Amethyst_gpolicySetMirrorDest;
	dev->SwitchDevObj.POLICYObj.gpolicyGetMirrorDest = Amethyst_gpolicyGetMirrorDest;
	dev->SwitchDevObj.POLICYObj.gpolicySetIllegalIPAddr = Amethyst_gpolicySetIllegalIPAddr;
	dev->SwitchDevObj.POLICYObj.gpolicyGetIllegalIPAddr = Amethyst_gpolicyGetIllegalIPAddr;
	dev->SwitchDevObj.POLICYObj.gpolicySetDipDaMismatch = Amethyst_gpolicySetDipDaMismatch;
	dev->SwitchDevObj.POLICYObj.gpolicyGetDipDaMismatch = Amethyst_gpolicyGetDipDaMismatch;
	dev->SwitchDevObj.POLICYObj.gpolicySetZeroSIP = Amethyst_gpolicySetZeroSIP;
	dev->SwitchDevObj.POLICYObj.gpolicyGetZeroSIP = Amethyst_gpolicyGetZeroSIP;
	dev->SwitchDevObj.POLICYObj.gpolicySetMcTtlExceeded = Amethyst_gpolicySetMcTtlExceeded;
	dev->SwitchDevObj.POLICYObj.gpolicyGetMcTtlExceeded = Amethyst_gpolicyGetMcTtlExceeded;
	dev->SwitchDevObj.POLICYObj.gpolicySetUcTtlExceeded = Amethyst_gpolicySetUcTtlExceeded;
	dev->SwitchDevObj.POLICYObj.gpolicyGetUcTtlExceeded = Amethyst_gpolicyGetUcTtlExceeded;
	dev->SwitchDevObj.POLICYObj.gprtSetPortTtlThreshold = Amethyst_gprtSetPortTtlThreshold;
	dev->SwitchDevObj.POLICYObj.gprtGetPortTtlThreshold = Amethyst_gprtGetPortTtlThreshold;
	dev->SwitchDevObj.POLICYObj.gpolicySetIpHdrErr = Amethyst_gpolicySetIpHdrErr;
	dev->SwitchDevObj.POLICYObj.gpolicyGetIpHdrErr = Amethyst_gpolicyGetIpHdrErr;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPFinUrgPsh = Amethyst_gpolicySetTCPFinUrgPsh;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPFinUrgPsh = Amethyst_gpolicyGetTCPFinUrgPsh;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPFlagsSeqNum = Amethyst_gpolicySetTCPFlagsSeqNum;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPFlagsSeqNum = Amethyst_gpolicyGetTCPFlagsSeqNum;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPSYN = Amethyst_gpolicySetTCPSYN;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPSYN = Amethyst_gpolicyGetTCPSYN;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPHSize = Amethyst_gpolicySetTCPHSize;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPHSize = Amethyst_gpolicyGetTCPHSize;
	dev->SwitchDevObj.POLICYObj.gprtSetPortTCPMinHdrSize = Amethyst_gprtSetPortTCPMinHdrSize;
	dev->SwitchDevObj.POLICYObj.gprtGetPortTCPMinHdrSize = Amethyst_gprtGetPortTCPMinHdrSize;
	dev->SwitchDevObj.POLICYObj.gpolicySetICMPSize = Amethyst_gpolicySetICMPSize;
	dev->SwitchDevObj.POLICYObj.gpolicyGetICMPSize = Amethyst_gpolicyGetICMPSize;
	dev->SwitchDevObj.POLICYObj.gprtSetPortICMPMaxSize = Amethyst_gprtSetPortICMPMaxSize;
	dev->SwitchDevObj.POLICYObj.gprtGetPortICMPMaxSize = Amethyst_gprtGetPortICMPMaxSize;
	dev->SwitchDevObj.POLICYObj.gpolicySetUDPPort = Amethyst_gpolicySetUDPPort;
	dev->SwitchDevObj.POLICYObj.gpolicyGetUDPPort = Amethyst_gpolicyGetUDPPort;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPPort = Amethyst_gpolicySetTCPPort;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPPort = Amethyst_gpolicyGetTCPPort;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPSynFin = Amethyst_gpolicySetTCPSynFin;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPSynFin = Amethyst_gpolicyGetTCPSynFin;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPFrag = Amethyst_gpolicySetTCPFrag;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPFrag = Amethyst_gpolicyGetTCPFrag;
	dev->SwitchDevObj.POLICYObj.gpolicySetNearNonTPMR = Amethyst_gpolicySetNearNonTPMR;
	dev->SwitchDevObj.POLICYObj.gpolicyGetNearNonTPMR = Amethyst_gpolicyGetNearNonTPMR;
	dev->SwitchDevObj.POLICYObj.gpolicySetETag = Amethyst_gpolicySetETag;
	dev->SwitchDevObj.POLICYObj.gpolicyGetETag = Amethyst_gpolicyGetETag;
	dev->SwitchDevObj.POLICYObj.gpolicySetNonETag = Amethyst_gpolicySetNonETag;
	dev->SwitchDevObj.POLICYObj.gpolicyGetNonETag = Amethyst_gpolicyGetNonETag;

	/*For Amethyst, reserve 16 free queue size*/
	msdReserveFreeQueueSize(dev);
	/*For Amethyst, Set Tx FIFO Depth to 0x0*/
	msdPhyTXFIFODepthSet(dev);
#endif
	return MSD_OK;
}

static MSD_STATUS InitObj(MSD_QD_DEV *dev)
{
	dev->SwitchDevObj.ATUObj.gfdbAddMacEntry = NULL;
	dev->SwitchDevObj.ATUObj.gfdbGetAtuEntryNext = NULL;
	dev->SwitchDevObj.ATUObj.gfdbFlush = NULL;
	dev->SwitchDevObj.ATUObj.gfdbMove = NULL;
	dev->SwitchDevObj.ATUObj.gfdbMoveInDB = NULL;
	dev->SwitchDevObj.ATUObj.gfdbPortRemove = NULL;
	dev->SwitchDevObj.ATUObj.gfdbPortRemoveInDB = NULL;
	dev->SwitchDevObj.ATUObj.gfdbDelAtuEntry = NULL;
	dev->SwitchDevObj.ATUObj.gfdbGetViolation = NULL;
	dev->SwitchDevObj.ATUObj.gfdbFindAtuMacEntry = NULL;
	dev->SwitchDevObj.ATUObj.gfdbSetAgingTimeout = NULL;
	dev->SwitchDevObj.ATUObj.gfdbGetAgingTimeout = NULL;
	dev->SwitchDevObj.ATUObj.gfdbSetLearn2All = NULL;
	dev->SwitchDevObj.ATUObj.gfdbGetLearn2All = NULL;
	dev->SwitchDevObj.ATUObj.gfdbSetPortLearnLimit = NULL;
	dev->SwitchDevObj.ATUObj.gfdbGetPortLearnLimit = NULL;
	dev->SwitchDevObj.ATUObj.gfdbGetPortLearnCount = NULL;
	dev->SwitchDevObj.ATUObj.gfdbGetEntryCount = NULL;
	dev->SwitchDevObj.ATUObj.gfdbGetEntryCountPerFid = NULL;
	dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCount = NULL;
	dev->SwitchDevObj.ATUObj.gfdbGetNonStaticEntryCountPerFid = NULL;
	dev->SwitchDevObj.ATUObj.gfdbDump = NULL;

	dev->SwitchDevObj.ECIDObj.gecidAddEntry = NULL;
	dev->SwitchDevObj.ECIDObj.gecidFlushAll = NULL;
	dev->SwitchDevObj.ECIDObj.gecidFlushEntry = NULL;
	dev->SwitchDevObj.ECIDObj.gecidMove = NULL;
	dev->SwitchDevObj.ECIDObj.gecidGetEcidEntryNext = NULL;
	dev->SwitchDevObj.ECIDObj.gecidFindEcidEntry = NULL;
	dev->SwitchDevObj.ECIDObj.gecidGetEntryCount = NULL;
	dev->SwitchDevObj.ECIDObj.gecidDump = NULL;

	dev->SwitchDevObj.PORTCTRLObj.gprtSetCutThrouthEnable = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetCutThrouthEnable = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreFcs = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreFcs = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetAllowBadFcs = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetAllowBadFcs = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitOut = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitOut = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPauseLimitIn = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPauseLimitIn = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetQueueToPause = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetQueueToPause = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetRxPriorityFlowControlEnable = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetRxPriorityFlowControlEnable = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetRxPriorityFlowControlToQueue = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetRxPriorityFlowControlToQueue = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetTxPriorityFlowControlEnable = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetTxPriorityFlowControlEnable = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetOutQueueSize = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMapDA = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMapDA = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPortLocked = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPortLocked = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetHoldAt1 = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetHoldAt1 = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetInt0nAgeOut = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetInt0nAgeOut = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetRefreshLocked = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetRefreshLocked = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreWrongData = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIgnoreWrongData = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetARPMirror = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetARPMirror = NULL;

	dev->SwitchDevObj.POLICYObj.gpolicySetMgmtCtrl = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetMgmtCtrl = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetCpuDest = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetCpuDest = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetIngressMonitorDest = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetIngressMonitorDest = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetRsvd2CpuEnables = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetRsvd2CpuEnables = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetMGMTPri = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetMGMTPri = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetRouteDest = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetRouteDest = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetMirrorDest = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetMirrorDest = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetIllegalIPAddr = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetIllegalIPAddr = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetDipDaMismatch = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetDipDaMismatch = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetZeroSIP = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetZeroSIP = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetMcTtlExceeded = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetMcTtlExceeded = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetUcTtlExceeded = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetUcTtlExceeded = NULL;
	dev->SwitchDevObj.POLICYObj.gprtSetPortTtlThreshold = NULL;
	dev->SwitchDevObj.POLICYObj.gprtGetPortTtlThreshold = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetIpHdrErr = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetIpHdrErr = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPFinUrgPsh = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPFinUrgPsh = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPFlagsSeqNum = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPFlagsSeqNum = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPSYN = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPSYN = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPHSize = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPHSize = NULL;
	dev->SwitchDevObj.POLICYObj.gprtSetPortTCPMinHdrSize = NULL;
	dev->SwitchDevObj.POLICYObj.gprtGetPortTCPMinHdrSize = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetICMPSize = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetICMPSize = NULL;
	dev->SwitchDevObj.POLICYObj.gprtSetPortICMPMaxSize = NULL;
	dev->SwitchDevObj.POLICYObj.gprtGetPortICMPMaxSize = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetUDPPort = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetUDPPort = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPPort = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPPort = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPSynFin = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPSynFin = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetTCPFrag = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetTCPFrag = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetNearNonTPMR = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetNearNonTPMR = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetETag = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetETag = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicySetNonETag = NULL;
	dev->SwitchDevObj.POLICYObj.gpolicyGetNonETag = NULL;

	dev->SwitchDevObj.VCTObj.gvctGetAdvCableDiag = NULL;
	dev->SwitchDevObj.VCTObj.gvctGetAdvExtendedStatus = NULL;

	dev->SwitchDevObj.STUObj.gstuAddEntry = NULL;
	dev->SwitchDevObj.STUObj.gstuGetEntryNext = NULL;
	dev->SwitchDevObj.STUObj.gstuFindSidEntry = NULL;
	dev->SwitchDevObj.STUObj.gstuGetEntryCount = NULL;
	dev->SwitchDevObj.STUObj.gstuDump = NULL;

	dev->SwitchDevObj.VTUObj.gvtuFindVidEntry = NULL;
	dev->SwitchDevObj.VTUObj.gvtuExistVidEntry = NULL;
	dev->SwitchDevObj.VTUObj.gvtuFlush = NULL;
	dev->SwitchDevObj.VTUObj.gvtuAddEntry = NULL;
	dev->SwitchDevObj.VTUObj.gvtuGetEntryNext = NULL;
	dev->SwitchDevObj.VTUObj.gvtuDelEntry = NULL;
	dev->SwitchDevObj.VTUObj.gvtuSetMode = NULL;
	dev->SwitchDevObj.VTUObj.gvtuGetMode = NULL;
	dev->SwitchDevObj.VTUObj.gvtuGetViolation = NULL;
	dev->SwitchDevObj.VTUObj.gvtuGetEntryCount = NULL;
	dev->SwitchDevObj.VTUObj.gvtuSetMemberTag = NULL;
	dev->SwitchDevObj.VTUObj.gvtuGetMemberTag = NULL;
	dev->SwitchDevObj.VTUObj.gvtuDump = NULL;

	dev->SwitchDevObj.IMPObj.impRun = NULL;
	dev->SwitchDevObj.IMPObj.impReset = NULL;
	dev->SwitchDevObj.IMPObj.impWriteEEPROM = NULL;
	dev->SwitchDevObj.IMPObj.impLoadToEEPROM = NULL;
	dev->SwitchDevObj.IMPObj.impLoadToRAM = NULL;
	dev->SwitchDevObj.IMPObj.impStop = NULL;

	dev->SwitchDevObj.EEPROMObj.geepromWrite = NULL;
	dev->SwitchDevObj.EEPROMObj.geepromRead = NULL;
	dev->SwitchDevObj.EEPROMObj.geepromSetMChipSel = NULL;
	dev->SwitchDevObj.EEPROMObj.geepromGetMChipSel = NULL;

	dev->SwitchDevObj.PHYCTRLObj.gphyReset = NULL;
    dev->SwitchDevObj.PHYCTRLObj.serdesReset = NULL;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortLoopback = NULL;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortSpeed = NULL;
	dev->SwitchDevObj.PHYCTRLObj.gphyPortPowerDown = NULL;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortDuplexMode = NULL;
	dev->SwitchDevObj.PHYCTRLObj.gphySetPortAutoMode = NULL;
	dev->SwitchDevObj.PHYCTRLObj.gphySetEEE = NULL;

	dev->SwitchDevObj.PIRLObj.gpirlInitialize = NULL;
	dev->SwitchDevObj.PIRLObj.gpirlInitResource = NULL;
	dev->SwitchDevObj.PIRLObj.gpirlCustomSetup = NULL;
	dev->SwitchDevObj.PIRLObj.gpirlCustomSetup = NULL;
	dev->SwitchDevObj.PIRLObj.gpirlReadResource = NULL;
	dev->SwitchDevObj.PIRLObj.gpirlWriteResource = NULL;
	dev->SwitchDevObj.PIRLObj.gpirlGetResReg = NULL;
	dev->SwitchDevObj.PIRLObj.gpirlSetResReg = NULL;
	dev->SwitchDevObj.PIRLObj.gpirlAdvConfig = NULL;
	dev->SwitchDevObj.PIRLObj.gpirlAdvConfigDisable = NULL;

	dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVid = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVid = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gvlnSetPortVlanDot1qMode = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gvlnGetPortVlanDot1qMode = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardTagged = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardTagged = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardUntagged = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardUntagged = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetUnicastFloodBlock = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetUnicastFloodBlock = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMulticastFloodBlock = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMulticastFloodBlock = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDiscardBCastMode = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDiscardBCastMode = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIGMPSnoopEnable = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIGMPSnoopEnable = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetHeaderMode = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetHeaderMode = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDropOnLock = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDropOnLock = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetJumboMode = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetJumboMode = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetLearnEnable = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetLearnEnable = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetVTUPriorityOverride = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetVTUPriorityOverride = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetSAPriorityOverride = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetSAPriorityOverride = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDAPriorityOverride = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDAPriorityOverride = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetMessagePort = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetMessagePort = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEtherType = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEtherType = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetAllowVidZero = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetAllowVidZero = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetFlowCtrl = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetFlowCtrl = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMonitorSource = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMonitorSource = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIngressMonitorSource = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetIngressMonitorSource = NULL;
	dev->SwitchDevObj.PORTCTRLObj.grcSetEgressRate = NULL;
	dev->SwitchDevObj.PORTCTRLObj.grcGetEgressRate = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetDuplex = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplex = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetDuplex = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceLink = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceLink = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetLinkState = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetForceSpeed = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceSpeed = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetSpeed = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetFrameMode = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetFrameMode = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPortECID = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPortECID = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetEgressMode = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetEgressMode = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreETag = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetIgnoreETag = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetExtendedPortCtrlReg = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetExtendedPortCtrlReg = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetVlanPorts = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetVlanPorts = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetForceDefaultVid = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetForceDefaultVid = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtSetPortState = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtGetPortState = NULL;
	dev->SwitchDevObj.PORTCTRLObj.gprtRegDump = NULL;

	dev->SwitchDevObj.TrunkObj.gprtSetTrunkPort = NULL;
	dev->SwitchDevObj.TrunkObj.gprtGetTrunkPort = NULL;
	dev->SwitchDevObj.TrunkObj.gsysSetTrunkMaskTable = NULL;
	dev->SwitchDevObj.TrunkObj.gsysGetTrunkMaskTable = NULL;
	dev->SwitchDevObj.TrunkObj.gsysSetHashTrunk = NULL;
	dev->SwitchDevObj.TrunkObj.gsysGetHashTrunk = NULL;
	dev->SwitchDevObj.TrunkObj.gsysSetTrunkRouting = NULL;
	dev->SwitchDevObj.TrunkObj.gsysGetTrunkRouting = NULL;

	dev->SwitchDevObj.PORTRMONObj.gstatsFlushAll = NULL;
	dev->SwitchDevObj.PORTRMONObj.gstatsFlushPort = NULL;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetPortCounter = NULL;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetPortAllCounters = NULL;
	dev->SwitchDevObj.PORTRMONObj.gstatsGetHistogramMode = NULL;
	dev->SwitchDevObj.PORTRMONObj.gstatsSetHistogramMode = NULL;
	dev->SwitchDevObj.PORTRMONObj.gmibDump = NULL;

	dev->SwitchDevObj.QUEUEObj.gprtSetQueueCtrl = NULL;
	dev->SwitchDevObj.QUEUEObj.gprtGetQueueCtrl = NULL;
	dev->SwitchDevObj.QUEUEObj.gprtSetPortSched = NULL;
	dev->SwitchDevObj.QUEUEObj.gprtGetPortSched = NULL;
	dev->SwitchDevObj.QUEUEObj.gsysSetQoSWeight = NULL;
	dev->SwitchDevObj.QUEUEObj.gsysGetQoSWeight = NULL;

	dev->SwitchDevObj.SYSCTRLObj.gsysSetFloodBC = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetFloodBC = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetMonitorMgmtCtrl = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetMonitorMgmtCtrl = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetCPUDest = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetCPUDest = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetIngressMonitorDest = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetIngressMonitorDest = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetEgressMonitorDest = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetEgressMonitorDest = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetRsvd2CpuEnables = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetRsvd2CpuEnables = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetMGMTPri = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetMGMTPri = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysSetSMISetup = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysGetSMISetup = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal1RegDump = NULL;
	dev->SwitchDevObj.SYSCTRLObj.gsysGlobal2RegDump = NULL;

	dev->SwitchDevObj.QOSMAPObj.gprtGetIEEEPrioMap = NULL;
	dev->SwitchDevObj.QOSMAPObj.gprtSetIEEEPrioMap = NULL;
	dev->SwitchDevObj.QOSMAPObj.gqosGetPrioMapRule = NULL;
	dev->SwitchDevObj.QOSMAPObj.gprtGetInitialPri = NULL;
	dev->SwitchDevObj.QOSMAPObj.gprtSetDefQpri = NULL;
	dev->SwitchDevObj.QOSMAPObj.gprtGetDefQpri = NULL;
	dev->SwitchDevObj.QOSMAPObj.gprtSetInitialPri = NULL;
	dev->SwitchDevObj.QOSMAPObj.gqosSetPrioMapRule = NULL;
	dev->SwitchDevObj.QOSMAPObj.gprtRewriteDscp = NULL;
	dev->SwitchDevObj.QOSMAPObj.gprtSetCosToQueue = NULL;
	dev->SwitchDevObj.QOSMAPObj.gprtGetCosToQueue = NULL;
	dev->SwitchDevObj.QOSMAPObj.gprtSetCosToDscp = NULL;
	dev->SwitchDevObj.QOSMAPObj.gprtGetCosToDscp = NULL;

	dev->SwitchDevObj.QAVObj.gqavGetPortQpriXHiLimit = NULL;
	dev->SwitchDevObj.QAVObj.gqavSetPortQpriXHiLimit = NULL;
	dev->SwitchDevObj.QAVObj.gqavGetPortQpriXRate = NULL;
	dev->SwitchDevObj.QAVObj.gqavSetPortQpriXRate = NULL;

	dev->SwitchDevObj.PTPObj.gptpSetIntEnable = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetIntEnable = NULL;
	dev->SwitchDevObj.PTPObj.gptpSetProtPTPEn = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetProtPTPEn = NULL;
	dev->SwitchDevObj.PTPObj.gptpSetPortConfig = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetPortConfig = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetTimeStamp = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetTSValidSt = NULL;
	dev->SwitchDevObj.PTPObj.gptpResetTimeStamp = NULL;
	dev->SwitchDevObj.PTPObj.gptpSetGlobalConfig = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetGlobalConfig = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetIntStatus = NULL;
	dev->SwitchDevObj.PTPObj.gptpSetMeanPathDelay = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetMeanPathDelay = NULL;
	dev->SwitchDevObj.PTPObj.gptpSetIgrPathDelayAsym = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetIgrPathDelayAsym = NULL;
	dev->SwitchDevObj.PTPObj.gptpSetEgrPathDelayAsym = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetEgrPathDelayAsym = NULL;
	dev->SwitchDevObj.PTPObj.gptpSetMode = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetMode = NULL;
	dev->SwitchDevObj.PTPObj.gptpSetMasterSlave = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetMasterSlave = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetTODBusyBitStatus = NULL;
	dev->SwitchDevObj.PTPObj.gptpTODStoreCompensation = NULL;
	dev->SwitchDevObj.PTPObj.gptpTODStoreAll = NULL;
	dev->SwitchDevObj.PTPObj.gptpTODCaptureAll = NULL;
	dev->SwitchDevObj.PTPObj.gptpSetPulse = NULL;
	dev->SwitchDevObj.PTPObj.gptpGetPulse = NULL;
	dev->SwitchDevObj.PTPObj.gtaiGetPTPGlobalTime = NULL;

	dev->SwitchDevObj.TCAMObj.gtcamFlushAll = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamFlushEntry = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamLoadEntry = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamReadTCAMData = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamGetNextTCAMData = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamFindEntry = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamSetMode = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamGetMode = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamAddEntryPort = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamRemoveEntryPort = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamEgrFlushEntry = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamEgrFlushEntryAllPorts = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamEgrLoadEntry = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamEgrGetNextTCAMData = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamEgrReadTCAMData = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamDump = NULL;
	dev->SwitchDevObj.TCAMObj.gtcamAdvConfig = NULL;

	dev->SwitchDevObj.TCAMRCObj.gtcamRCLoadEntry = NULL;
	dev->SwitchDevObj.TCAMRCObj.gtcamRCFlushEntry = NULL;
	dev->SwitchDevObj.TCAMRCObj.gtcamRCReadEntry = NULL;
	dev->SwitchDevObj.TCAMRCObj.gtcamRCGetNextEntry = NULL;
	dev->SwitchDevObj.TCAMRCObj.gtcamRCFindEntry = NULL;
	dev->SwitchDevObj.TCAMRCObj.gtcamRCEntryDump = NULL;

	dev->SwitchDevObj.TCAMRCCompObj.gtcamRCCompLoad = NULL;
	dev->SwitchDevObj.TCAMRCCompObj.gtcamRCCompFlushAll = NULL;
	dev->SwitchDevObj.TCAMRCCompObj.gtcamRCCompFlushPort = NULL;
	dev->SwitchDevObj.TCAMRCCompObj.gtcamRCCompRead = NULL;

	dev->SwitchDevObj.ARPObj.garpLoadMcEntry = NULL;
	dev->SwitchDevObj.ARPObj.garpLoadUcEntry = NULL;
	dev->SwitchDevObj.ARPObj.garpFlushEntry = NULL;
	dev->SwitchDevObj.ARPObj.garpFlushAll = NULL;
	dev->SwitchDevObj.ARPObj.garpReadEntry = NULL;
	dev->SwitchDevObj.ARPObj.garpGetNextEntry = NULL;

	dev->SwitchDevObj.RMUObj.grmuGetID = NULL;
	dev->SwitchDevObj.RMUObj.grmuAtuDump = NULL;
	dev->SwitchDevObj.RMUObj.grmuEcidDump = NULL;
	dev->SwitchDevObj.RMUObj.grmuMibDump = NULL;
	dev->SwitchDevObj.RMUObj.grmuMib2Dump = NULL;
	dev->SwitchDevObj.RMUObj.grmuTCAMDump = NULL;
	dev->SwitchDevObj.RMUObj.grmuTCAMRead = NULL;
	dev->SwitchDevObj.RMUObj.grmuTCAMWrite = NULL;
	dev->SwitchDevObj.RMUObj.grmuMultiRegAccess = NULL;
    dev->SwitchDevObj.RMUObj.grmuRegDump = NULL;

	dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIC45PhyReg = NULL;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIC45PhyReg = NULL;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyReg = NULL;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyReg = NULL;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyXMDIOReg = NULL;
	dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyXMDIOReg = NULL;

	dev->SwitchDevObj.LEDObj.gledForceOn = NULL;
	dev->SwitchDevObj.LEDObj.gledForceOff = NULL;
	dev->SwitchDevObj.LEDObj.gledLinkAct = NULL;
	dev->SwitchDevObj.LEDObj.gledModeSet = NULL;
	dev->SwitchDevObj.LEDObj.gledModeGet = NULL;
	dev->SwitchDevObj.LEDObj.gledBlinkRateSet = NULL;
	dev->SwitchDevObj.LEDObj.gledBlinkRateGet = NULL;
	dev->SwitchDevObj.LEDObj.gledPulseStretchSet = NULL;
	dev->SwitchDevObj.LEDObj.gledPulseStretchGet = NULL;
	dev->SwitchDevObj.LEDObj.gledSpecialModeSet = NULL;
	dev->SwitchDevObj.LEDObj.gledSpecialModeGet = NULL;
	dev->SwitchDevObj.LEDObj.gledCtrlSet = NULL;
	dev->SwitchDevObj.LEDObj.gledCtrlGet = NULL;

	return MSD_OK;
}

MSD_STATUS InitDevObj(MSD_QD_DEV *dev)
{
	InitObj(dev);
    if (dev->devName == MSD_TOPAZ)
        InitDevObj_Topaz(dev);
    else if (dev->devName == MSD_PERIDOT)
        InitDevObj_Peridot(dev);
    else if (dev->devName == MSD_WILLOW)
        InitDevObj_Willow(dev);
	if (dev->devName == MSD_PEARL)
		InitDevObj_Pearl(dev);
	else if (dev->devName == MSD_AGATE)
		InitDevObj_Agate(dev);
	if (dev->devName == MSD_AMETHYST)
		InitDevObj_Amethyst(dev);
    return MSD_OK;
}

/*******************************************************************************
* msdReserveFreeQueueSize
*
* DESCRIPTION:
*       workaround for QC WatchDog trigger issue: reserve 16 free queue size, total 1024. Set threshold *       for the free pool FIFO to 0x10. This means the recycle pointer will not be reused immediately, *       16 pointers will be given to per-port module before this pointer can be used.
*
* INPUTS:
*        dev - pointer to the structure for device.
*
* OUTPUTS:
*        None.
*
* RETURNS:
*       MSD_TRUE, if input is valid. MSD_FALSE, otherwise.
*
*******************************************************************************/
static MSD_BOOL msdReserveFreeQueueSize(MSD_QD_DEV *qdev)
{
	MSD_U16 reg27, tempData;
	MSD_STATUS retVal;

	/*read back G1 offset 0x1b register value*/
	retVal = msdGetAnyReg(qdev->devNum, 0x1B, 0x1B, &reg27);
	if (retVal != 0)
	{
		MSD_DBG_ERROR(("msdGetAnyReg return failed, retval = %d.\n", retVal));
		return MSD_FAIL;
	}
	/*Change the hidden block to QC Hidden registers, Hidden Block: 0x2*/
	retVal = msdSetAnyReg(qdev->devNum, 0x1B, 0x1B, 0x2000);
	if (retVal != 0)
	{
		MSD_DBG_ERROR(("msdSetAnyReg return failed, retval = %d.\n", retVal));
		return MSD_FAIL;
	}
	/*wait QCHiddenCtrl register busy bit self clear*/
	do
	{
		retVal = msdGetAnyReg(qdev->devNum, 0x1B, 0x1B, &tempData);
		if (retVal != 0)
		{
			MSD_DBG_ERROR(("msdGetAnyReg return failed, retval = %d.\n", retVal));
			return MSD_FAIL;
		}
	} while ((tempData & 0x8000) == 0x8000);

	/*write data to QC Hidden Data, Device Offset 0x1B, Register Offset: 0x1A, reserve 16 free queue size*/
	retVal = msdSetAnyReg(qdev->devNum, 0x1B, 0x1A, 0x8400);
	if (retVal != 0)
	{
		MSD_DBG_ERROR(("msdSetAnyReg return failed, retval = %d.\n", retVal));
		return MSD_FAIL;
	}
	/*write cmd register, QC Hidden Control, Device Offset 0x1B, Register Offset: 0x19*/
	tempData = (0xE000 | (0x1F << 8) | 0xC);

	retVal = msdSetAnyReg(qdev->devNum, 0x1B, 0x19, tempData);
	if (retVal != 0)
	{
		MSD_DBG_ERROR(("msdSetAnyReg return failed, retval = %d.\n", retVal));
		return MSD_FAIL;
	}

	/*restore the Hidden block*/
	retVal = msdSetAnyReg(qdev->devNum, 0x1B, 0x1B, reg27);
	if (retVal != 0)
	{
		MSD_DBG_ERROR(("msdSetAnyReg return failed, retval = %d.\n", retVal));
		return MSD_FAIL;
	}

	return MSD_TRUE;
}

/*******************************************************************************
* msdPhyTXFIFODepthSet
*
* DESCRIPTION:
*       workaround for CQ-7589 issue: PTP - Peer to Peer one-step and two-step *       Pdresp egress time stamp inconsistent, boundary sync egress time stamp *       failed for both one-step and two-step. * SOLUTION:*       Set the PHY FIFO to be 0 which has the smallest FIFO depth
*
* INPUTS:
*        dev - pointer to the structure for device.
*
* OUTPUTS:
*        None.
*
* RETURNS:
*       MSD_TRUE, if input is valid. MSD_FALSE, otherwise.
*
*******************************************************************************/
static MSD_BOOL msdPhyTXFIFODepthSet(MSD_QD_DEV *dev)
{
	MSD_STATUS       retVal = 0;     /* Functions return value.      */
	MSD_U8           hwPort, port;   /* the physical port number     */
	MSD_U16  pageReg, data;

	MSD_DBG_INFO(("msdPhyTXFIFODepthSet Called.\n"));

	/* translate LPORT to hardware port */
	for (port = 1; port < 9; port++)
	{
		hwPort = MSD_LPORT_2_PHY(port);
		if (hwPort == MSD_INVALID_PHY)
		{
			MSD_DBG_ERROR(("Bad Port: %u.\n", (unsigned int)port));
			return MSD_BAD_PARAM;
		}

		msdSemTake(dev->devNum, dev->phyRegsSem, OS_WAIT_FOREVER);

		if ((retVal = msdGetSMIPhyReg(dev->devNum, hwPort, 22, &pageReg)) != MSD_OK)
		{
			MSD_DBG_ERROR(("read Phy Page Register return %s.\n", msdDisplayStatus(retVal)));
			msdSemGive(dev->devNum, dev->phyRegsSem);
			return retVal;
		}

		/* change page 2*/
		if ((retVal = msdSetSMIPhyReg(dev->devNum, hwPort, 22, 2)) != MSD_OK)
		{
			MSD_DBG_ERROR(("write back Phy Page Register %s.\n", msdDisplayStatus(retVal)));
			msdSemGive(dev->devNum, dev->phyRegsSem);
			return retVal;
		}

		/* read register 16 bit 15:14 field */
		if ((retVal = msdGetSMIPhyReg(dev->devNum, hwPort, 16, &data)) != MSD_OK)
		{
			MSD_DBG_ERROR(("read back Phy Page Register %s.\n", msdDisplayStatus(retVal)));
			msdSemGive(dev->devNum, dev->phyRegsSem);
			return retVal;
		}
		/* write register 16 bit 15:14 field to 0x0 */
		data = data & 0x3FFF;
		if ((retVal = msdSetSMIPhyReg(dev->devNum, hwPort, 16, data)) != MSD_OK)
		{
			MSD_DBG_ERROR(("write back Phy Page Register %s.\n", msdDisplayStatus(retVal)));
			msdSemGive(dev->devNum, dev->phyRegsSem);
			return retVal;
		}

		/* change page 0*/
		if ((retVal = msdSetSMIPhyReg(dev->devNum, hwPort, 22, 0)) != MSD_OK)
		{
			MSD_DBG_ERROR(("write back Phy Page Register %s.\n", msdDisplayStatus(retVal)));
			msdSemGive(dev->devNum, dev->phyRegsSem);
			return retVal;
		}

		/* read register 0 */
		if ((retVal = msdGetSMIPhyReg(dev->devNum, hwPort, 0, &data)) != MSD_OK)
		{
			MSD_DBG_ERROR(("read back Phy Page Register %s.\n", msdDisplayStatus(retVal)));
			msdSemGive(dev->devNum, dev->phyRegsSem);
			return retVal;
		}
		/* write register 0 bit 15 field to 0x1 */
		data = (data & 0x7FFF) | (1 << 15);
		if ((retVal = msdSetSMIPhyReg(dev->devNum, hwPort, 0, data)) != MSD_OK)
		{
			MSD_DBG_ERROR(("write back Phy Page Register %s.\n", msdDisplayStatus(retVal)));
			msdSemGive(dev->devNum, dev->phyRegsSem);
			return retVal;
		}

		if ((retVal = msdSetSMIPhyReg(dev->devNum, hwPort, 22, pageReg)) != MSD_OK)
		{
			MSD_DBG_ERROR(("write back Phy Page Register %s.\n", msdDisplayStatus(retVal)));
			msdSemGive(dev->devNum, dev->phyRegsSem);
			return retVal;
		}

		msdSemGive(dev->devNum, dev->phyRegsSem);
	}

	MSD_DBG_INFO(("msdPhyTXFIFODepthSet Exit.\n"));
	return retVal;
}
#endif /*#if FEATURE_MARVELL_6321*/
