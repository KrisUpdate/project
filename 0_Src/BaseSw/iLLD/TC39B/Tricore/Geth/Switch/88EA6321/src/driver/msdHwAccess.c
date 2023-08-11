#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"
/********************************************************************************
* msdHwAccess.c
*
* DESCRIPTION:
*       Functions declarations for Hw accessing quarterDeck phy, internal and
*       global registers.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*
*******************************************************************************/
#include "GethBasicDemo.h"
#include "msdHwAccess.h"
#include "msdSem.h"
#include "msdUtils.h"

MSD_STATUS msdRmuRegWait(MSD_QD_DEV* dev, MSD_U8 phyAddr, MSD_U8 regAddr, MSD_U8 bitIdx, MSD_U8 compareValue);
static MSD_STATUS msdRmuRegRead (MSD_QD_DEV* dev, MSD_U8 phyAddr , MSD_U8 regAddr, OUT MSD_U16* value);
static MSD_STATUS msdRmuRegWrite (MSD_QD_DEV* dev, MSD_U8 phyAddr , MSD_U8 regAddr, MSD_U16 value);

static MSD_STATUS msdMultiAddrRead (MSD_QD_DEV* dev, MSD_U8 phyAddr , MSD_U8 regAddr, OUT MSD_U16* value);
static MSD_STATUS msdMultiAddrWrite (MSD_QD_DEV* dev, MSD_U8 phyAddr , MSD_U8 regAddr, MSD_U16 value);

/****************************************************************************/
/* Special Functions for buffered RMU  .                                    */
/****************************************************************************/
MSD_STATUS msdRMUMultiRegAccess(IN  MSD_U8  devNum, INOUT MSD_RegCmd **regCmdList, IN MSD_U32 nCmd);


/*******************************************************************************
* msdRegisterAccessFlush
*
* DESCRIPTION:
*       This routine flushes all pending register accesses. It needs to be used
*       when register access mode was set to asynchronous/critical and RMU is used.
*       It will not return an error code when it was called on wrong interface
*       or chip type - so it can be called in every situation and will do nothing
*       and return MSD_OK in worst case
*
* INPUTS:
*       devNum  - Device number
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success or when nothing has to be done
*       MSD_FAIL  - on error
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdRegisterAccessFlush
(
    IN MSD_U8    devNum
)
{
	MSD_STATUS   retVal;
	MSD_QD_DEV*  dev = sohoDevGet(devNum);
	MSD_U16      i;
	MSD_RegCmd *pRegCmd;

	/* check if RMU buffering suported at all - if no MSD_NOT_SUPPORTED */
	if (!IS_RMU_SUPPORTED(dev))
        return(MSD_OK);
	if (dev->devName != MSD_WILLOW)
        return(MSD_OK);
	/* check if RMU mode is async or critical - if no MSD_OK */
	if (dev->accessMode == MSD_RMU_ACCESS_MODE_SYNCHRONOUS)
		return(MSD_OK);
	/* check if there are buffered operations - if no MSD_OK */
	if (dev->nCmdBufferLevel == 0)
		return(MSD_OK);
	/* check if we are under the critical buffer level - if no MSD_FAIL */
	if ((dev->nCmdBufferLevel > dev->nCmdBufferFlushLevel) || (dev->nCmdBufferLevel > MAX_RMU_TO_BUFFER)) {
		MSD_DBG_ERROR(("Too many RMU cmds buffered %d.\n", dev->nCmdBufferLevel));
		return(MSD_FAIL);
	}

	/* send out the buffered operations using msdRMUMultiRegAccess */
	pRegCmd = &(dev->regCmdBuffer[0]);
	retVal = msdRMUMultiRegAccess(devNum, &pRegCmd, dev->nCmdBufferLevel);
	if (retVal == MSD_OK) {
		/* copy read results */
		for (i = 0; i < dev->nCmdBufferLevel; i++) {
			if (dev->regCmdBuffer[i].opCode == MSD_RMU_REQ_OPCODE_READ) {
				if (dev->regCmdResultPtr[i] != NULL) {
					*dev->regCmdResultPtr[i] = dev->regCmdBuffer[i].data;
				}
				else {
					MSD_DBG_ERROR(("Read operation without result pointer.\n"));
					retVal = MSD_FAIL;
				}
			}
		}
	}
	/* clear buffer and level */
	dev->nCmdBufferLevel = 0;
	/* return the error code you got */
	return retVal;
}

/*******************************************************************************
* msdRegisterAccessMode
*
* DESCRIPTION:
*       This routine changes the access mode for for all switch register
*       reads and writes between synchronous and asyncronous. This allows
*       using multiple register operations in a single RMU frame.
*
* INPUTS:
*       devNum  - Device number
*       mode    - register acces mode
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdRegisterAccessMode
(
    IN  MSD_U8    devNum,
    IN  MSD_REGISTER_ACCESS_MODE     mode
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);

	/* check support of new mode if - no failure */
	if (!IS_RMU_SUPPORTED(dev))
		return(MSD_NOT_SUPPORTED);
	if (dev->devName != MSD_WILLOW)
		return(MSD_NOT_SUPPORTED);
	/* check if mode actually changes */
	if (dev->accessMode == mode)
		return(MSD_OK);

	/* if the value changes from sync to anything - just store new mode */
	if (dev->accessMode == MSD_RMU_ACCESS_MODE_SYNCHRONOUS) {
		/* no special handling - all done at the end*/
	}
	/* if the value changes from critical to anything */
	else if (dev->accessMode == MSD_RMU_ACCESS_MODE_CRITICAL) {
		/*    check if level is to high - if yes - failure */
		if (dev->nCmdBufferLevel > MAX_RMU_TO_BUFFER) {
			MSD_DBG_ERROR(("Too many cmds buffered in RMU mode critical %d.\n", dev->nCmdBufferLevel));
			return(MSD_FAIL);
		}
		/*    flush buffer - if it fails - failure */
		if (msdRegisterAccessFlush(devNum) != MSD_OK) {
			MSD_DBG_ERROR(("Failed to flush buffered cmds leaving RMU mode critical\n"));
			return(MSD_FAIL);
		}
	}
	/* if the value changes from async to any mode */
	else if (dev->accessMode == MSD_RMU_ACCESS_MODE_ASYNCHRONOUS) {
		/*    flush buffer - if it fails - failure */
		if (msdRegisterAccessFlush(devNum) != MSD_OK) {
			MSD_DBG_ERROR(("Failed to flush buffered cmds leaving RMU mode asynchronous\n"));
			return(MSD_FAIL);
		}
	}
	dev->accessMode = mode;
	dev->nCmdBufferLevel = 0; /* we restart buffering now */
	if (mode == MSD_RMU_ACCESS_MODE_CRITICAL) {
		dev->nCmdBufferFlushLevel = 0xFFFF; /* never flush automatically */
	}
	if (mode == MSD_RMU_ACCESS_MODE_ASYNCHRONOUS) {
		dev->nCmdBufferFlushLevel = MAX_RMU_TO_BUFFER; /* flush auromatically */
	}

	return MSD_OK;
}

/****************************************************************************/
/* Switch Any registers direct R/W functions.                                    */
/****************************************************************************/

/*******************************************************************************
* msdSetAnyReg
*
* DESCRIPTION:
*       This function directly writes to a switch's register.
*
* INPUTS:
*       devAddr - device register.
*       regAddr - The register's address.
*       data    - The data to be written.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdSetAnyReg
(
    IN  MSD_U8    devNum,
    IN  MSD_U8    devAddr,
    IN  MSD_U8    regAddr,
    IN  MSD_U16   data
)
{
    MSD_STATUS   retVal = MSD_OK;
	MSD_U32	index;

	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}
		

	MSD_DBG(("(LOG RW): devAddr 0x%02x, regAddr 0x%02x, ",
              devAddr,regAddr));
	MSD_DBG(("data 0x%04x.\n", data));


    msdSemTake(devNum,dev->multiAddrSem,OS_WAIT_FOREVER);
	

	if (IS_RMU_SUPPORTED(dev)) {
		if (dev->accessMode == MSD_RMU_ACCESS_MODE_SYNCHRONOUS) {
	    retVal = msdRmuRegWrite(dev,devAddr,regAddr,data);
		}
		else {
			index = dev->nCmdBufferLevel;
			dev->regCmdBuffer[index].devAddr = devAddr;
			dev->regCmdBuffer[index].regAddr = regAddr;
			dev->regCmdBuffer[index].data = data;
			dev->regCmdBuffer[index].isWaitOnBit = MSD_RMU_WAIT_ON_BIT_FALSE;
			dev->regCmdBuffer[index].opCode = MSD_RMU_REQ_OPCODE_WRITE;
			/* no return value for write */
			dev->regCmdResultPtr[index] = NULL;
			dev->nCmdBufferLevel++;
			if (dev->nCmdBufferLevel >= dev->nCmdBufferFlushLevel) {
				if ((retVal = msdRegisterAccessFlush(devNum)) != MSD_OK) {
					MSD_DBG_ERROR(("Failed to flush buffered cmds\n"));
				}
			}
		}
	}
    else if(IS_SMI_MULTICHIP_SUPPORTED(dev))
	    retVal = msdMultiAddrWrite(dev,devAddr,regAddr,data);
    else	
	    if (dev->fgtWriteMii)
		    retVal = dev->fgtWriteMii(devNum, devAddr, regAddr, data);
	    else
	    {
		    MSD_DBG_ERROR(("FMSD_WRITE_MII API is NULL.\n"));
		    retVal = MSD_NOT_SUPPORTED;
	    }

	msdSemGive(devNum, dev->multiAddrSem);

    return retVal;
}

/*******************************************************************************
* msdGetAnyReg
*
* DESCRIPTION:
*       This function directly reads a switch's register.
*
* INPUTS:
*       devAddr - device register.
*       regAddr - The register's address.
*
* OUTPUTS:
*       data    - The read register's data.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdGetAnyReg
(
    IN  MSD_U8    devNum,
    IN  MSD_U8    devAddr,
    IN  MSD_U8    regAddr,
    OUT MSD_U16   *data
)
{
    MSD_STATUS   retVal = MSD_OK;
	MSD_U32	index;
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	msdSemTake(devNum, dev->multiAddrSem, OS_WAIT_FOREVER);

	if (IS_RMU_SUPPORTED(dev)) {
		if (dev->accessMode == MSD_RMU_ACCESS_MODE_SYNCHRONOUS) {
	    retVal = msdRmuRegRead(dev,devAddr,regAddr,data);
		}
		else {
			index = dev->nCmdBufferLevel;
			dev->regCmdBuffer[index].devAddr = devAddr;
			dev->regCmdBuffer[index].regAddr = regAddr;
			dev->regCmdBuffer[index].data = 0;
			dev->regCmdBuffer[index].isWaitOnBit = MSD_RMU_WAIT_ON_BIT_FALSE;
			dev->regCmdBuffer[index].opCode = MSD_RMU_REQ_OPCODE_READ;
			/* save pointer for data read from reg */
			dev->regCmdResultPtr[index] = data;
			dev->nCmdBufferLevel++;
			if (dev->nCmdBufferLevel >= dev->nCmdBufferFlushLevel) {
				if ((retVal = msdRegisterAccessFlush(devNum)) != MSD_OK) {
					MSD_DBG_ERROR(("Failed to flush buffered cmds\n"));
				}
			}
		}
	}
    else if(IS_SMI_MULTICHIP_SUPPORTED(dev))
	    retVal = msdMultiAddrRead(dev,devAddr,regAddr,data);
    else
	    if (dev->fgtReadMii)
		    retVal = dev->fgtReadMii(devNum, devAddr, regAddr, data);
	    else
	    {
		    MSD_DBG_ERROR(("FMSD_READ_MII API is NULL.\n"));
		    retVal = MSD_NOT_SUPPORTED;
	    }
	
	msdSemGive(devNum, dev->multiAddrSem);

	MSD_DBG(("(LOG RR): devAddr 0x%02x, regAddr 0x%02x, ",
              devAddr,regAddr));
	MSD_DBG(("data 0x%04x.\n", *data));
    return retVal;
}

/*******************************************************************************
* msdSetAnyRegField
*
* DESCRIPTION:
*       This function writes to specified field in a switch's register.
*
* INPUTS:
*       devAddr     - Device Address to write the register for.
*       regAddr     - The register's address.
*       fieldOffset - The field start bit index. (0 - 15)
*       fieldLength - Number of bits to write.
*       data        - Data to be written.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       1.  The sum of fieldOffset & fieldLength parameters must be smaller-
*           equal to 16.
*
*******************************************************************************/
MSD_STATUS msdSetAnyRegField
(
    IN  MSD_U8	 devNum,
    IN  MSD_U8    devAddr,
    IN  MSD_U8    regAddr,
    IN  MSD_U8    fieldOffset,
    IN  MSD_U8    fieldLength,
    IN  MSD_U16   data
)
{
    MSD_U16 mask;
    MSD_U16 tmpData;
    MSD_STATUS   retVal,retVal2;

	retVal = msdGetAnyReg(devNum, devAddr, regAddr, &tmpData);
    if(retVal != MSD_OK)
    {
        return retVal;
    }
	retVal = msdRegisterAccessFlush(devNum);
    if (retVal != MSD_OK)
    {
        return retVal;
    }
    MSD_CALC_MASK(fieldOffset,fieldLength,mask);

    /* Set the desired bits to 0.                       */
    tmpData &= ~mask;
    /* Set the given data into the above reset bits.    */
    tmpData |= ((data << fieldOffset) & mask);
	MSD_DBG(("(LOG RW): devAddr 0x%02x, regAddr 0x%02x, ",
              devAddr,regAddr));
	MSD_DBG(("fOff %d, fLen %d, data 0x%04x.\n", fieldOffset,
              fieldLength,data));

	retVal = msdSetAnyReg(devNum, devAddr, regAddr, tmpData);
	retVal2 = msdRegisterAccessFlush(devNum);
    if (retVal == MSD_OK)
        return retVal2;
    return retVal;
}

/*******************************************************************************
* msdGetAnyRegField
*
* DESCRIPTION:
*       This function reads a specified field from a switch's register.
*
* INPUTS:
*       devAddr     - device address to read the register for.
*       regAddr     - The register's address.
*       fieldOffset - The field start bit index. (0 - 15)
*       fieldLength - Number of bits to read.
*
* OUTPUTS:
*       data        - The read register field.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       1.  The sum of fieldOffset & fieldLength parameters must be smaller-
*           equal to 16.
*
*******************************************************************************/
MSD_STATUS msdGetAnyRegField
(
    IN  MSD_U8	 devNum,
    IN  MSD_U8    devAddr,
    IN  MSD_U8    regAddr,
    IN  MSD_U8    fieldOffset,
    IN  MSD_U8    fieldLength,
    OUT MSD_U16   *data
)
{
    MSD_U16 mask;            /* Bits mask to be read */
    MSD_U16 tmpData;
    MSD_STATUS   retVal;

	retVal = msdGetAnyReg(devNum, devAddr, regAddr, &tmpData);
    if (retVal != MSD_OK)
        return retVal;
	retVal = msdRegisterAccessFlush(devNum);
    if (retVal != MSD_OK)
        return retVal;
    MSD_CALC_MASK(fieldOffset,fieldLength,mask);

    tmpData = (tmpData & mask) >> fieldOffset;
    *data = tmpData;
	MSD_DBG(("(LOG RR): devAddr 0x%02x, regAddr 0x%02x, ",
              devAddr,regAddr));
	MSD_DBG(("fOff %d, fLen %d, data 0x%04x.\n", fieldOffset, fieldLength, *data));

    return MSD_OK;
}

/****************************************************************************/
/* SMI PHY Registers indirect R/W functions.                                         */
/****************************************************************************/
/*****************************************************************************
* msdSetSMIC45PhyReg
*
* DESCRIPTION:
*       This function indirectly write internal SERDES register through SMI PHY command.
*
* INPUTS:
*       devAddr - The device address.
*       phyAddr - The PHY address.
*       regAddr - The register address.
*       data - data to be written
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdSetSMIC45PhyReg
(
    IN MSD_U8    devNum,
    IN MSD_U8    devAddr,
    IN MSD_U8    phyAddr,
    IN MSD_U16   regAddr,
    IN MSD_U16   data
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }

    if (dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIC45PhyReg != NULL)
        return dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIC45PhyReg(dev, devAddr, phyAddr, regAddr, data);
    else
        return MSD_NOT_SUPPORTED;
}

/*****************************************************************************
* msdGetSMIC45PhyReg
*
* DESCRIPTION:
*       This function indirectly read internal SERDES register through SMI PHY command.
*
* INPUTS:
*       devAddr - The device address.
*       phyAddr - The PHY address.
*       regAddr  - The register address to read.
*
* OUTPUTS:
*       data     - The storage where register data to be saved.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdGetSMIC45PhyReg
(
    IN  MSD_U8   devNum,
    IN  MSD_U8   devAddr,
    IN  MSD_U8   phyAddr,
    IN  MSD_U16  regAddr,
    OUT MSD_U16  *data
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }

    if (dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIC45PhyReg != NULL)
        return dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIC45PhyReg(dev, devAddr, phyAddr, regAddr, data);
    else
        return MSD_NOT_SUPPORTED;
}

/*****************************************************************************
* msdSetSMIPhyReg
*
* DESCRIPTION:
*       This function indirectly write internal PHY register through SMI PHY command.
*
* INPUTS:
*       devAddr - The PHY address.
*       regAddr - The register address.
*       data - data to be written
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdSetSMIPhyReg
(
    IN  MSD_U8   devNum,
    IN  MSD_U8   devAddr,
    IN  MSD_U8   regAddr,
    IN  MSD_U16  data
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }

    if (dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyReg != NULL)
        return dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyReg(dev, devAddr, regAddr, data);
    else
        return MSD_NOT_SUPPORTED;
}

/*****************************************************************************
* msdGetSMIPhyReg
*
* DESCRIPTION:
*       This function indirectly read internal PHY register through SMI PHY command.
*
* INPUTS:
*       devAddr  - The PHY address to be read.
*       regAddr  - The register address to read.
*
* OUTPUTS:
*       data     - The storage where register date to be saved.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdGetSMIPhyReg
(
    IN  MSD_U8   devNum,
    IN  MSD_U8   devAddr,
    IN  MSD_U8   regAddr,
    OUT MSD_U16  *data
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }

    if (dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyReg != NULL)
        return dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyReg(dev, devAddr, regAddr, data);
    else
        return MSD_NOT_SUPPORTED;
}

/*****************************************************************************
* msdSetSMIPhyXMDIOReg
*
* DESCRIPTION:
*       This function indirectly set internal PHY XMDIO register through SMI PHY command.
*
* INPUTS:
*       portNum  - The logic port number
*       devAddr  - The Clause45 device address to be write.
*       regAddr  - The register address to write.
*       data     - The data to be write
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdSetSMIPhyXMDIOReg
(
    IN MSD_U8  devNum,
    IN MSD_U8  portNum,
    IN MSD_U8  devAddr,
    IN MSD_U16  regAddr,
    IN MSD_U16 data
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }

    if (dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyXMDIOReg != NULL)
        return dev->SwitchDevObj.SmiPhyRegAccessObj.msdSetSMIPhyXMDIOReg(dev, portNum, devAddr, regAddr, data);
    else
        return MSD_NOT_SUPPORTED;
}

/*****************************************************************************
* msdGetSMIPhyXMDIOReg
*
* DESCRIPTION:
*       This function indirectly read internal PHY XMDIO register through SMI PHY command.
*
* INPUTS:
*       portNum  - The logic port number
*       devAddr  - The Clause45 device address to be write.
*       regAddr  - The register address to write.
*
* OUTPUTS:
*       data     - The storage where register date to be saved.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdGetSMIPhyXMDIOReg
(
    IN MSD_U8  devNum,
    IN MSD_U8  portNum,
    IN MSD_U8  devAddr,
    IN MSD_U16  regAddr,
    OUT MSD_U16 *data
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }

    if (dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyXMDIOReg != NULL)
        return dev->SwitchDevObj.SmiPhyRegAccessObj.msdGetSMIPhyXMDIOReg(dev, portNum, devAddr, regAddr, data);
    else
        return MSD_NOT_SUPPORTED;
}

#define MSD_RMU_END_OF_FRAME	0xFFFFFFFF
#define MSD_RMU_TCAM_END_OF_LIST	0x0000
#define MSD_RMU_LEN_TYPE		0x0800


#define MSD_RMU_REQ_CODE_GETID			0x0000
#define MSD_RMU_REQ_CODE_DUMP_ATU		0x1000
#define MSD_RMU_REQ_CODE_DUMP_MIB		0x1020
#define MSD_RMU_REQ_CODE_DUMP_MIB2		0x1020
#define MSD_RMU_REQ_CODE_DUMP_TCAM		0x1030
#define MSD_RMU_REQ_CODE_MULTI_REGRW	0x2000
#define MSD_RMU_REQ_CODE_MULTI_TCAM		0x2030

/*
#define RMU_REQ_OPCODE_READ  2
#define RMU_REQ_OPCODE_WRITE 1
#define RMU_WAIT_ON_BIT_FALSE 0
#define RMU_WAIT_ON_BIT_TRUE 1
#define RMU_WAIT_ON_BIT_VAL0 0
#define RMU_WAIT_ON_BIT_VAL1 3
*/

/*#define MSD_RMU_PACKET_PREFIX_SIZE 28*/ 	/* DA(6)+SA(6)+ETYPE(4)+DSA(4)+Type(2)+ReqFMT(2)+Pad(2)+ReqCode(2) */
/*#define RMU_REGCMD_WORD_SIZE 4*/

static MSD_U32 msdCreateDsaTag
(
	IN	MSD_QD_DEV	*dev
);
/*#endif*/

/****************************************************************************/
/* Internal functions.                                                  */
/****************************************************************************/

MSD_STATUS msdRmuTxRxPkt
(
	IN  MSD_QD_DEV* dev,
	IN  MSD_U8	*reqPkt,
	IN  MSD_U32	reqPktLen,
	OUT MSD_U8	**rspPkt,
	OUT MSD_U32	*rspPktLen
)
{
	if (dev->rmu_tx_rx)
		return dev->rmu_tx_rx(reqPkt, reqPktLen, rspPkt, rspPktLen);
	else
	{
		MSD_DBG_ERROR(("RMU_TX_RX API is NULL.\n"));
		return MSD_NOT_SUPPORTED;
	}
}

MSD_U32 msdCreateDsaTag
(
	IN	MSD_QD_DEV* dev
)
{
	MSD_U32	dsaTag = 0;
	MSD_U8	trg_dev = dev->devNum;
	MSD_U8	seq_num = dev->reqSeqNum;

	MSD_U8	pri = 6;

	dsaTag |= (1 << 30);
	dsaTag |= ((trg_dev & 0x1f) << 24);
	dsaTag |= (0x3E << 18);
	dsaTag |= (1 << 17);
	dsaTag |= ((pri & 0x7) << 13);
	dsaTag |= (0xf << 8);
	dsaTag |= (seq_num & 0xff);

	dev->reqSeqNum += 1;

	return dsaTag;
}

MSD_STATUS msdRmuReqPktCreate
(
	IN	MSD_QD_DEV* dev,
	IN	MSD_RMU_CMD cmd,
	OUT MSD_Packet* packet
)
{
	MSD_U8 DA[6] = { 0x01, 0x50, 0x43, 0x00, 0x00, 0x00 };
	MSD_U8 SA[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };	/* will be override with corrent cpu address by upper application. */
	int i = 0;
	/* MSD_U8 SA[6] = { 0x28, 0xD2, 0x44, 0x8C, 0xF9, 0xF3 };	// will be override with corrent cpu address by upper application. */
	for (i = 0; i < 6; i++) {
		SA[i]  = ethAddr.addr[i];
	}
	
	msdMemSet(packet, 0, sizeof(MSD_Packet));

	msdMemCpy(packet->DA, DA, sizeof(DA));
	msdMemCpy(packet->SA, SA, sizeof(SA));
    packet->rmuMode = dev->rmuMode;
	packet->etherType = (dev->eTypeValue << 16);
	packet->dsaTag = msdCreateDsaTag(dev);
	packet->lenType = MSD_RMU_LEN_TYPE;
	switch (cmd)
	{
		case MSD_DumpATU:
			packet->reqFmt = 0x0001;
			packet->reqCode = MSD_RMU_REQ_CODE_DUMP_ATU;
			/* packet->reqData._reqData = 0; // starting address */
			break;
		case MSD_DumpMIB:
		case MSD_DumpMIBClear:
			packet->reqFmt = 0x0001;
			packet->reqCode = MSD_RMU_REQ_CODE_DUMP_MIB;
			/* packet->reqData._reqData = 0; // 0x000p */
			break;
		case MSD_DumpMIB2:
		case MSD_DumpMIB2Clear:
			packet->reqFmt = 0x0002;
			packet->reqCode = MSD_RMU_REQ_CODE_DUMP_MIB2;
			/* packet->reqData._reqData = 0;	// 0x000p */
			break;
		case MSD_DumpTCAM:
			packet->reqFmt = 0x0001;
			packet->reqCode = MSD_RMU_REQ_CODE_DUMP_TCAM;
			/* packet->reqData._reqData = 0;	// starting address */
			break;
		case MSD_RegRW:
			packet->reqFmt = 0x0001;
			packet->reqCode = MSD_RMU_REQ_CODE_MULTI_REGRW;
			packet->reqData._regRWData.eof = MSD_RMU_END_OF_FRAME;
			break;
		case MSD_TCAMRW:
			packet->reqFmt = 0x0001;
			packet->reqCode = MSD_RMU_REQ_CODE_MULTI_TCAM;
			packet->reqData._tcamRWData.eol = MSD_RMU_TCAM_END_OF_LIST;
			break;
		case MSD_GetID:
			packet->reqFmt = 0x0;
			packet->reqCode = MSD_RMU_REQ_CODE_GETID;
			packet->reqData._reqData = 0x0;
			break;
		default:
			return MSD_NOT_SUPPORTED;
	}
	return MSD_OK;
}

static void msdU32VauleCpy(MSD_U8 *ptr, MSD_U32 value)
{
	*ptr = (MSD_U8)((value >> 24) & 0xff);
	*(ptr + 1) = (MSD_U8)((value >> 16) & 0xff);
	*(ptr + 2) = (MSD_U8)((value >> 8) & 0xff);
	*(ptr + 3) = (MSD_U8)((value >> 0) & 0xff);
}
static void msdU16VauleCpy(MSD_U8 *ptr, MSD_U16 value)
{
	*ptr = (MSD_U8)((value >> 8) & 0xff);
	*(ptr + 1) = (MSD_U8)((value >> 0) & 0xff);
}

MSD_STATUS msdRmuPackEthReqPkt
(
	IN	MSD_Packet	*packet,
	IN	MSD_RMU_CMD	cmd,
	OUT MSD_U8	*buf
)
{
	/**/
	/* MSD_U8 ethPacket[512]; */
	MSD_U8 *pktPtr = NULL;
	MSD_U32 tmp32;
	MSD_U16 tmp16;

	int i, j;

	pktPtr = buf;

	msdMemCpy(pktPtr, packet->DA, sizeof(packet->DA));
	pktPtr += sizeof(packet->DA);

	msdMemCpy(pktPtr, packet->SA, sizeof(packet->SA));
	pktPtr += sizeof(packet->SA);

    if (packet->rmuMode == MSD_RMU_ETHERT_TYPE_DSA_MODE)
    {
        tmp32 = packet->etherType;
        msdU32VauleCpy(pktPtr, tmp32);
        pktPtr += 4;
    }

	tmp32 = packet->dsaTag;
	msdU32VauleCpy(pktPtr, tmp32);
	pktPtr += 4;

	tmp16 = packet->lenType;
	msdU16VauleCpy(pktPtr, tmp16);
	pktPtr += 2;

	tmp16 = packet->reqFmt;
	msdU16VauleCpy(pktPtr, tmp16);
	pktPtr += 2;

	tmp16 = packet->pad;
	msdU16VauleCpy(pktPtr, tmp16);
	pktPtr += 2;

	tmp16 = packet->reqCode;
	msdU16VauleCpy(pktPtr, tmp16);
	pktPtr += 2;

	switch (cmd)
	{
		case MSD_RegRW:
			if (packet->reqData._regRWData.nCmd > MSD_RMU_MAX_REGCMDS)
				return MSD_BAD_PARAM;
			for (i = 0; i < (int)(packet->reqData._regRWData.nCmd); i++){
				MSD_U32 cmdWord = 0;/* = rmuEncodeRegCmd(&packet->regCmd[i], packet->reqCode); */
				cmdWord |= ((packet->reqData._regRWData.regCmd[i].isWaitOnBit & 0x1) << 28);
				cmdWord |= (packet->reqData._regRWData.regCmd[i].opCode & 0x3) << 26;
				cmdWord |= (((packet->reqData._regRWData.regCmd[i].devAddr >> 3) & 0x3) << 24) |
							((packet->reqData._regRWData.regCmd[i].devAddr & 0x7) << 21);
				cmdWord |= (packet->reqData._regRWData.regCmd[i].regAddr & 0x1f) << 16;
				if (packet->reqData._regRWData.regCmd[i].isWaitOnBit) {
					cmdWord |= (packet->reqData._regRWData.regCmd[i].data & 0xf) << 8;
				}
				else {
					cmdWord |= (packet->reqData._regRWData.regCmd[i].data & 0xff) |
						(((packet->reqData._regRWData.regCmd[i].data >> 8) & 0xff) << 8);
				}

				tmp32 = cmdWord;
				msdU32VauleCpy(pktPtr, tmp32);
				pktPtr += 4;
			}

			msdMemCpy(pktPtr, &packet->reqData._regRWData.eof, sizeof(packet->reqData._regRWData.eof));
			break;
		case MSD_TCAMRW:
			/* RW */
			if (packet->reqData._tcamRWData.nEntry > MSD_RMU_MAX_TCAMS)
				return MSD_BAD_PARAM;
			for (i = 0; i < (int)(packet->reqData._tcamRWData.nEntry); i++)
			{
				for (j = 0; j < (MSD_RMU_TCAM_ENTRY_OP_SIZE/2); j++)
				{
					tmp16 = *((packet->reqData._tcamRWData.tcamEntry[i]).dataArray + j);
					msdU16VauleCpy(pktPtr, tmp16);
					pktPtr += 2;
				}
			}
			msdMemCpy(pktPtr, &packet->reqData._tcamRWData.eol, sizeof(packet->reqData._tcamRWData.eol));
			break;
        case MSD_GetID:
		case MSD_DumpATU:
		case MSD_DumpMIB:
		case MSD_DumpMIBClear:
		case MSD_DumpMIB2:
		case MSD_DumpMIB2Clear:
		case MSD_DumpTCAM:
			tmp16 = packet->reqData._reqData;
			msdU16VauleCpy(pktPtr, tmp16);
			break;
		default: 
			return MSD_NOT_SUPPORTED;
	}
	return MSD_OK;
}

/*****************************************************************************
* rmuRegRead
*
* DESCRIPTION:
*       This function reads data from a device using RMU access bus.
*
* INPUTS:
*       phyAddr     - The PHY address to be read.
*       regAddr     - The register address to read.
*       value       - The storage where register date to be saved.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static MSD_STATUS msdRmuRegRead (MSD_QD_DEV* dev, MSD_U8 phyAddr , MSD_U8 regAddr, OUT MSD_U16* value)
{
	MSD_Packet ReqPkt;
	/* Packet RspPkt; */
	MSD_U32 nCmd = 1;
	MSD_U8 reqEthPacket[512];
	MSD_U8 rspEthPacket[512];
	MSD_U32 req_pktlen, rsp_pktlen;
	MSD_STATUS retVal = 0;
    MSD_U8 delta;

	MSD_RMU_CMD cmd = MSD_RegRW;

	MSD_U8 *rspEthPacketPtr = &(rspEthPacket[0]);

    if (dev->rmuMode == MSD_RMU_ETHERT_TYPE_DSA_MODE)
        delta = 0;
    else if (dev->rmuMode == MSD_RMU_DSA_MODE)
        delta = 4;
    else
        delta = 0;

	/*Request Packet*/
	msdRmuReqPktCreate(dev, cmd, &ReqPkt);

	ReqPkt.reqData._regRWData.nCmd = nCmd;
	ReqPkt.reqData._regRWData.regCmd[0].isWaitOnBit = MSD_RMU_WAIT_ON_BIT_FALSE;
	ReqPkt.reqData._regRWData.regCmd[0].opCode = MSD_RMU_REQ_OPCODE_READ;
	ReqPkt.reqData._regRWData.regCmd[0].devAddr = phyAddr;
	ReqPkt.reqData._regRWData.regCmd[0].regAddr = regAddr;
	ReqPkt.reqData._regRWData.regCmd[0].data = 0x0;

	msdMemSet(reqEthPacket, 0, sizeof(reqEthPacket));
	msdRmuPackEthReqPkt(&ReqPkt, cmd, reqEthPacket);
    req_pktlen = MSD_RMU_PACKET_PREFIX_SIZE - delta + (nCmd + 1)*MSD_RMU_REGCMD_WORD_SIZE;

	/* dump_packet(&reqEthPacket, req_pktlen, "Read Register Request Packet"); */
	retVal = msdRmuTxRxPkt(dev, reqEthPacket, req_pktlen,
		&rspEthPacketPtr, &rsp_pktlen);
	if (retVal != MSD_OK || rsp_pktlen == 0) {
		MSD_DBG_ERROR(("rmu_tx_rx returned: %s with rsp_pktLen %d.\n", msdDisplayStatus(retVal), (int)rsp_pktlen));
		return MSD_FAIL;
	}

	/* Receive Packet */
	if (rsp_pktlen >= req_pktlen) {
		/* dump_packet(&rspEthPacket, req_pktlen,
			"Read Register Response Packet");
		rmuParseEthPacket(&rspEthPacket, RspPkt); */
        *value = ((*(rspEthPacketPtr + 30 - delta) & 0xff) << 8) | (*(rspEthPacketPtr + 31 - delta) & 0xff);
	}
	else {
		MSD_DBG_ERROR(("response_pktlen [%d] < request_pktlen [%d]\n",
			rsp_pktlen, req_pktlen));
		return MSD_FAIL;
	}

	/* *value = RspPkt.reqData._regRWData.regCmd[0].data;*/

	return MSD_OK;
}

/*****************************************************************************
* msdRmuRegWait
*
* DESCRIPTION:
*       This function waits for a bit in a register to get a specific value.
*
* INPUTS:
*       devAddr - device register.
*       regAddr - The register's address.
*       bitIndex- bit to be checked (0..15)
*       compareValue - expected value of bit (0..1)
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdRmuRegWait(MSD_QD_DEV* dev, MSD_U8 phyAddr, MSD_U8 regAddr, MSD_U8 bitIdx, MSD_U8 compareValue)
{
	MSD_Packet ReqPkt;
	/* Packet RspPkt; */
	MSD_U32 nCmd = 1;
	MSD_U8 reqEthPacket[512];
	MSD_U8 rspEthPacket[512];
	MSD_U32 req_pktlen, rsp_pktlen;
	MSD_STATUS retVal = 0;
	MSD_U8 delta;

	MSD_RMU_CMD cmd = MSD_RegRW;

	MSD_U8 *rspEthPacketPtr = &(rspEthPacket[0]);

	if (dev->rmuMode == MSD_RMU_ETHERT_TYPE_DSA_MODE)
		delta = 0;
	else if (dev->rmuMode == MSD_RMU_DSA_MODE)
		delta = 4;
	else
		delta = 0;

	/*Request Packet*/
	msdRmuReqPktCreate(dev, cmd, &ReqPkt);

	ReqPkt.reqData._regRWData.nCmd = nCmd;
	ReqPkt.reqData._regRWData.regCmd[0].isWaitOnBit = MSD_RMU_WAIT_ON_BIT_TRUE;
	if (compareValue == 0) {
		ReqPkt.reqData._regRWData.regCmd[0].opCode = MSD_RMU_WAIT_ON_BIT_VAL0;
	}
	else {
		ReqPkt.reqData._regRWData.regCmd[0].opCode = MSD_RMU_WAIT_ON_BIT_VAL1;
	}
	ReqPkt.reqData._regRWData.regCmd[0].devAddr = phyAddr;
	ReqPkt.reqData._regRWData.regCmd[0].regAddr = regAddr;
	ReqPkt.reqData._regRWData.regCmd[0].data = bitIdx;

	msdMemSet(reqEthPacket, 0, sizeof(reqEthPacket));
	msdRmuPackEthReqPkt(&ReqPkt, cmd, reqEthPacket);
	req_pktlen = MSD_RMU_PACKET_PREFIX_SIZE - delta + (nCmd + 1)*MSD_RMU_REGCMD_WORD_SIZE;

	/* dump_packet(&reqEthPacket, req_pktlen, "Read Register Request Packet"); */
	retVal = msdRmuTxRxPkt(dev, reqEthPacket, req_pktlen,
		&rspEthPacketPtr, &rsp_pktlen);
	if (retVal != MSD_OK || rsp_pktlen == 0) {
		MSD_DBG_ERROR(("rmu_tx_rx returned: %s with rsp_pktLen %d.\n", msdDisplayStatus(retVal), (int)rsp_pktlen));
		return MSD_FAIL;
	}

	/* Receive Packet */
	if (rsp_pktlen < req_pktlen) {
		MSD_DBG_ERROR(("response_pktlen [%d] < request_pktlen [%d]\n",
			rsp_pktlen, req_pktlen));
		return MSD_FAIL;
	}
	return MSD_OK;
}


/*****************************************************************************
* msdRmuRegWrite
*
* DESCRIPTION:
*       This function writes data to the device using RMU access bus.
*
* INPUTS:
*       phyAddr     - The PHY address to be read.
*       regAddr     - The register address to read.
*       value       - The data to be written into the register.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static MSD_STATUS msdRmuRegWrite (MSD_QD_DEV* dev, MSD_U8 phyAddr , MSD_U8 regAddr, MSD_U16 value)
{
	MSD_Packet ReqPkt;
	/* Packet RspPkt; */
	MSD_U32 nCmd = 1;
	MSD_U8 reqEthPacket[512];
	MSD_U8 rspEthPacket[512];
	MSD_U32 req_pktlen, rsp_pktlen;
	MSD_STATUS retVal = 0;
    MSD_U8 delta;

	MSD_RMU_CMD cmd = MSD_RegRW;

	MSD_U8 *rspEthPacketPtr = &(rspEthPacket[0]);

    if (dev->rmuMode == MSD_RMU_ETHERT_TYPE_DSA_MODE)
        delta = 0;
    else if (dev->rmuMode == MSD_RMU_DSA_MODE)
        delta = 4;
    else
        delta = 0;

	/*Request Packet*/
	msdRmuReqPktCreate(dev, cmd, &ReqPkt);

	ReqPkt.reqData._regRWData.nCmd = nCmd;
	ReqPkt.reqData._regRWData.regCmd[0].isWaitOnBit = MSD_RMU_WAIT_ON_BIT_FALSE;
	ReqPkt.reqData._regRWData.regCmd[0].opCode = MSD_RMU_REQ_OPCODE_WRITE;
	ReqPkt.reqData._regRWData.regCmd[0].devAddr = phyAddr;
	ReqPkt.reqData._regRWData.regCmd[0].regAddr = regAddr;
	ReqPkt.reqData._regRWData.regCmd[0].data = value;

	msdRmuPackEthReqPkt(&ReqPkt, cmd, reqEthPacket);
	req_pktlen = MSD_RMU_PACKET_PREFIX_SIZE - delta + (nCmd + 1)*MSD_RMU_REGCMD_WORD_SIZE;

	/* dump_packet(&reqEthPacket, req_pktlen, "Write Register Request Packet"); */
	retVal = msdRmuTxRxPkt(dev, reqEthPacket, req_pktlen,
		&rspEthPacketPtr, &rsp_pktlen);
	if (retVal != MSD_OK || rsp_pktlen == 0) {
		MSD_DBG_ERROR(("rmu_tx_rx returned: %s with rsp_pktLen %d.\n", msdDisplayStatus(retVal), (int)rsp_pktlen));
		return MSD_FAIL;
	}

	if (rsp_pktlen < req_pktlen) {
		/* printf("Error: response_pktlen [%d] < request_pktlen [%d]\n",
			rsp_pktlen, req_pktlen); */
		MSD_DBG_ERROR(("response_pktlen [%d] < request_pktlen [%d]\n",
			rsp_pktlen, req_pktlen));
		return MSD_FAIL;
	}

	/* dump_packet(&rspEthPacket, req_pktlen,
		"Write Register Response Packet"); */

	return MSD_OK;
}

/*****************************************************************************
* msdMultiAddrRead
*
* DESCRIPTION:
*       This function reads data from a device in the secondary MII bus.
*
* INPUTS:
*       phyAddr     - The PHY address to be read.
*       regAddr     - The register address to read.
*       value       - The storage where register date to be saved.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static MSD_STATUS msdMultiAddrRead (MSD_QD_DEV* dev, MSD_U8 phyAddr , MSD_U8 regAddr, OUT MSD_U16* value)
{
    MSD_U16 smiReg;
    volatile unsigned int timeOut; /* in 100MS units */

    timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do 
    {
        if((dev->fgtReadMii(dev->devNum,dev->phyAddr,MSD_REG_SMI_COMMAND, &smiReg)) != MSD_OK)
        {
            return MSD_FAIL;
        }
		if(timeOut-- < 1 ) 
        {
            return MSD_FAIL;
        }
    } while (smiReg & MSD_SMI_BUSY);

    smiReg =  MSD_SMI_BUSY | (phyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_READ_22 << MSD_SMI_OP_BIT) | 
            (regAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE22 << MSD_SMI_MODE_BIT);

	if((dev->fgtWriteMii(dev->devNum,dev->phyAddr,MSD_REG_SMI_COMMAND, smiReg)) != MSD_OK)
    {
        return MSD_FAIL;
    }
	
    timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do 
    {
		if((dev->fgtReadMii(dev->devNum,dev->phyAddr,MSD_REG_SMI_COMMAND, &smiReg)) != MSD_OK)
        {
            return MSD_FAIL;
        }
		if(timeOut-- < 1 ) 
        {
            return MSD_FAIL;
        }
    } while (smiReg & MSD_SMI_BUSY);

	if((dev->fgtReadMii(dev->devNum,dev->phyAddr,MSD_REG_SMI_DATA, &smiReg)) != MSD_OK)
    {
        return MSD_FAIL;
    }
    *value = smiReg;
    
    return MSD_OK;
}

/*****************************************************************************
* msdMultiAddrWrite
*
* DESCRIPTION:
*       This function writes data to the device in the secondary MII bus.
*
* INPUTS:
*       phyAddr     - The PHY address to be read.
*       regAddr     - The register address to read.
*       value       - The data to be written into the register.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static MSD_STATUS msdMultiAddrWrite (MSD_QD_DEV* dev, MSD_U8 phyAddr , MSD_U8 regAddr, MSD_U16 value)
{
    MSD_U16 smiReg;
    volatile unsigned int timeOut; /* in 100MS units */

    timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do 
    {
		if((dev->fgtReadMii(dev->devNum,dev->phyAddr,MSD_REG_SMI_COMMAND, &smiReg)) != MSD_OK)
        {
            return MSD_FAIL;
        }
		if(timeOut-- < 1 ) 
        {
            return MSD_FAIL;
        }
    } while (smiReg & MSD_SMI_BUSY);

	if((dev->fgtWriteMii(dev->devNum,dev->phyAddr,MSD_REG_SMI_DATA, value)) != MSD_OK)
    {
        return MSD_FAIL;
    }
    smiReg = MSD_SMI_BUSY | (phyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_WRITE << MSD_SMI_OP_BIT) | 
            (regAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE22 << MSD_SMI_MODE_BIT);

	if((dev->fgtWriteMii(dev->devNum,dev->phyAddr,MSD_REG_SMI_COMMAND, smiReg)) != MSD_OK)
    {
        return MSD_FAIL;
    }

    return MSD_OK;
}
#endif /*#if FEATURE_MARVELL_6321*/
