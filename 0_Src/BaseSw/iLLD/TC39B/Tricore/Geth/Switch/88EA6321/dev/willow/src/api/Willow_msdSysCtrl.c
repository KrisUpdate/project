#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/********************************************************************************
* Willow_msdSysCtrl.c
*
* DESCRIPTION:
*       API definitions for system global control.
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "Willow_msdSysCtrl.h"
#include "Willow_msdApiInternal.h"
#include "Willow_msdHwAccess.h"
#include "Willow_msdDrvSwRegs.h"
#include "msdUtils.h"
#include "msdSem.h"
#include "msdHwAccess.h"
/*******************************************************************************
* Willow_gsysSetTrunkMaskTable
*
* DESCRIPTION:
*       This function sets Trunk Mask for the given Mask Number.
*
* INPUTS:
*       maskNum - Mask Number (0~7).
*       trunkMask - Trunk mask bits. Bit 0 controls trunk masking for port 0,
*                    bit 1 for port 1 , etc.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysSetTrunkMaskTable
(
    IN MSD_QD_DEV    *dev,
    IN MSD_U32       maskNum,
    IN MSD_U32       trunkMask
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_STATUS       retVal2;        /* internal return value.      */
    MSD_U16          data;           /* The register's read data.    */
    MSD_U32            mask;
    int count=0x10;

    MSD_DBG_INFO(("Willow_gsysSetTrunkMaskTable Called.\n"));

	if (maskNum >= WILLOW_MAX_LAG_MASK_NUM)
    {
		MSD_DBG_ERROR(("MSD_BAD_PARAM (maskNum: %u). It should be within [0, %d]\n", (unsigned int)maskNum, (WILLOW_MAX_LAG_MASK_NUM - 1)));
        return MSD_BAD_PARAM;
    }

    mask = (1 << dev->numOfPorts) - 1;
    if(trunkMask > mask)
    {
        MSD_DBG_ERROR(("MSD_BAD_PARAM (trunkMask: 0x%x). It should be within [0, 0x%x]\n", trunkMask, mask));
        return MSD_BAD_PARAM;
    }

    msdSemTake(dev->devNum, dev->tblRegsSem,OS_WAIT_FOREVER);

    /* Check if the register can be accessed. */
    /* in this case we do not replace it with a Willow_msdWaitForBit because the value is used in the next step */
    do
    {
		retVal = msdGetAnyReg(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_TRUNK_MASK_TBL, &data);
        retVal2 = msdRegisterAccessFlush(dev->devNum);
        if ((retVal != MSD_OK) || (retVal2 != MSD_OK))
        {
            if (retVal == MSD_OK)
                retVal = retVal2;
			MSD_DBG_ERROR(("Read G2 trunk mask tbl register busy bit returned: %s.\n", msdDisplayStatus(retVal)));
            msdSemGive(dev->devNum, dev->tblRegsSem);
            return retVal;
        }
		if((count--)==0) 
		{
			MSD_DBG_ERROR(("Wait G2 trunk mask tbl register busy bit time out.\n"));
			msdSemGive(dev->devNum, dev->tblRegsSem);
			return MSD_FAIL;
		}
    } while (data & 0x8000);

    /* Set related register */
	data &= 0x0800; /* maintain existing HasHash bit.*/
	mask = MSD_LPORTVEC_2_PORTVEC(trunkMask);
    data = (MSD_U16)(0x8000 | data |  (maskNum << 12) | mask);   
	retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR,WILLOW_QD_REG_TRUNK_MASK_TBL,data);
    retVal2 = msdRegisterAccessFlush(dev->devNum);

	msdSemGive(dev->devNum, dev->tblRegsSem);

    if ((retVal != MSD_OK) || (retVal2 != MSD_OK))
    {
        if (retVal == MSD_OK)
            retVal = retVal2;
        MSD_DBG_ERROR(("msdSetAnyReg for G2 trunk mask tbl returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gsysSetTrunkMaskTable Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gsysGetTrunkMaskTable
*
* DESCRIPTION:
*       This function Willow_gets Trunk Mask for the given Trunk Number.
*
* INPUTS:
*       maskNum - Mask Number.
*
* OUTPUTS:
*       trunkMask - Trunk mask bits. Bit 0 controls trunk masking for port 0,
*                    bit 1 for port 1 , etc.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given (if maskNum > 0x7)
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysGetTrunkMaskTable
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_U32       maskNum,
    OUT MSD_U32       *trunkMask
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_STATUS       retVal2;        /* internal return value.      */
    MSD_U16          data,data2;     /* The register's read data.    */
    MSD_U32          mask;
    int count=0x10;

    MSD_DBG_INFO(("Willow_gsysGetTrunkMaskTable Called.\n"));

	if (maskNum >= WILLOW_MAX_LAG_MASK_NUM)
	{
		MSD_DBG_ERROR(("MSD_BAD_PARAM (maskNum: %u). It should be within [0, %d]\n", (unsigned int)maskNum, (WILLOW_MAX_LAG_MASK_NUM - 1)));
		return MSD_BAD_PARAM;
	}

    msdSemTake(dev->devNum, dev->tblRegsSem,OS_WAIT_FOREVER);

    /* Check if the register can be accessed. */
    do
    {
		retVal = msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR,WILLOW_QD_REG_TRUNK_MASK_TBL,&data);
        retVal2 = msdRegisterAccessFlush(dev->devNum);
        if ((retVal != MSD_OK) || (retVal2 != MSD_OK))
        {
            if (retVal == MSD_OK)
                retVal = retVal2;
			MSD_DBG_ERROR(("Read G2 trunk mask tbl register busy bit returned: %s.\n", msdDisplayStatus(retVal)));
            msdSemGive(dev->devNum, dev->tblRegsSem);
            return retVal;
        }
		if((count--)==0) 
		{
			MSD_DBG_ERROR(("Wait G2 trunk mask tbl register busy bit time out.\n"));
			msdSemGive(dev->devNum, dev->tblRegsSem);
			return MSD_FAIL;
		}
    } while (data & 0x8000);

	/* Set related register */
    data &= 0x0800;
    data = (MSD_U16)(data | (maskNum << 12));
	retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR,WILLOW_QD_REG_TRUNK_MASK_TBL,data);
    if (retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        MSD_DBG_ERROR(("msdSetAnyReg for G2 trunk mask tbl returned: %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum, dev->tblRegsSem);
		return retVal;
    }

	/* read-back value */
	retVal = msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR,WILLOW_QD_REG_TRUNK_MASK_TBL,&data2);
    retVal2 = msdRegisterAccessFlush(dev->devNum);
    msdSemGive(dev->devNum, dev->tblRegsSem);

    if ((retVal != MSD_OK) || (retVal2 != MSD_OK))
    {
        if (retVal == MSD_OK)
            retVal = retVal2;
        MSD_DBG_ERROR(("msdGetAnyReg for G2 trunk mask tbl returned: %s.\n", msdDisplayStatus(retVal)));
		return retVal;
    }

    mask = (1 << dev->maxPorts) - 1;
    *trunkMask = MSD_PORTVEC_2_LPORTVEC(data2 & mask);
            
    MSD_DBG_INFO(("Willow_gsysGetTrunkMaskTable Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gsysSetHashTrunk
*
* DESCRIPTION:
*       Hash DA & SA for TrunkMask selection. Trunk load balancing is accomplished 
*       by using the frame's DA and SA fields to access one of eight Trunk Masks. 
*       When this bit is set to a one the hashed computed for address table 
*       lookups is used for the TrunkMask selection. When this bit is cleared to 
*       a zero the lower 3 bits of the frame's DA and SA are XOR'ed toWillow_gether to 
*       select the TrunkMask to use.
*
* INPUTS:
*       en - MSD_TRUE to use lookup table, MSD_FALSE to use XOR.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysSetHashTrunk
(
    IN MSD_QD_DEV    *dev,
    IN MSD_BOOL        en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_STATUS       retVal2;        /* internal return value.      */
    MSD_U16            data;

    MSD_DBG_INFO(("Willow_gsysSetHashTrunk Called.\n"));

	MSD_BOOL_2_BIT(en, data);

    /* Set related bit */
	retVal = msdSetAnyRegField(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR,WILLOW_QD_REG_TRUNK_MASK_TBL,11,1,data);
    retVal2 = msdRegisterAccessFlush(dev->devNum);
    if ((retVal != MSD_OK) || (retVal2 != MSD_OK))
    {
        if (retVal == MSD_OK)
            retVal = retVal2;
        MSD_DBG_ERROR(("msdSetAnyRegField for G2 trunk mask tbl returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gsysSetHashTrunk Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gsysGetHashTrunk
*
* DESCRIPTION:
*       Hash DA & SA for TrunkMask selection. Trunk load balancing is accomplished 
*       by using the frame's DA and SA fields to access one of eight Trunk Masks. 
*       When this bit is set to a one the hashed computed for address table 
*       lookups is used for the TrunkMask selection. When this bit is cleared to 
*       a zero the lower 3 bits of the frame's DA and SA are XOR'ed toWillow_gether to 
*       select the TrunkMask to use.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       en - MSD_TRUE to use lookup table, MSD_FALSE to use XOR.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysGetHashTrunk
(
    IN  MSD_QD_DEV    *dev,
    OUT MSD_BOOL      *en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_STATUS       retVal2;        /* internal return value.      */
    MSD_U16          data;           /* The register's read data.    */
    MSD_DBG_INFO(("Willow_gsysGetHashTrunk Called.\n"));

    /* Get related bit */
    retVal = msdGetAnyRegField(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR,WILLOW_QD_REG_TRUNK_MASK_TBL,11,1,&data);
    retVal2 = msdRegisterAccessFlush(dev->devNum);
    if ((retVal != MSD_OK) || (retVal2 != MSD_OK))
    {
        if (retVal == MSD_OK)
            retVal = retVal2;
		MSD_DBG_ERROR(("msdGetAnyRegField for G2 trunk mask tbl returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

	MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gsysGetHashTrunk Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gsysSetTrunkRouting
*
* DESCRIPTION:
*       This function sets routing information for the given Trunk ID.
*
* INPUTS:
*       trunkId - Trunk ID.
*       trunkRoute - Trunk route bits. Bit 0 controls trunk routing for port 0,
*                    bit 1 for port 1 , etc.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given (if trunkId > 0x1F)
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysSetTrunkRouting
(
    IN MSD_QD_DEV    *dev,
    IN MSD_U32       trunkId,
    IN MSD_U32       trunkRoute
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_STATUS       retVal2;        /* internal return value.      */
    MSD_U16          data;           /* The register's read data.    */
    MSD_U32            mask;

    int count=0x10;

    MSD_DBG_INFO(("Willow_gsysSetTrunkRouting Called.\n"));

	/* Check if trunkId is out of range [0,0x1f] */
	if (!WILLOW_IS_TRUNK_ID_VALID(dev, trunkId))
    {
		MSD_DBG_INFO(("MSD_BAD_PARAM (trunkId: %u)\n", (unsigned int)trunkId));
        return MSD_BAD_PARAM;
    }

	/* Check if trunkRoute is our of range [0,0x7ff] */
    mask = (1 << dev->numOfPorts) - 1;
    if(trunkRoute > mask)
    {
		MSD_DBG_ERROR(("MSD_BAD_PARAM (trunkRoute: 0x%x). It should be within [0, 0x%x]\n", trunkRoute, mask));
        return MSD_BAD_PARAM;
    }

    msdSemTake(dev->devNum, dev->tblRegsSem,OS_WAIT_FOREVER);

    /* Check if the register can be accessed. */
    if (!IS_RMU_SUPPORTED(dev))
    {
        do
        {
            retVal = msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_TRUNK_ROUTING, &data);
            if (retVal != MSD_OK)
            {
                MSD_DBG_ERROR(("Read G2 trunk mapping register busy bit returned: %s.\n", msdDisplayStatus(retVal)));
                msdSemGive(dev->devNum, dev->tblRegsSem);
                return retVal;
            }
            if ((count--) == 0)
            {
                MSD_DBG_ERROR(("Wait G2 trunk mapping register busy bit time out.\n"));
                msdSemGive(dev->devNum, dev->tblRegsSem);
                return MSD_FAIL;
            }
        } while (data & 0x8000);
    }
    else
    {
        if (Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_TRUNK_ROUTING, 15, 0) != MSD_OK)
        {
            msdRegisterAccessFlush(dev->devNum);
            MSD_DBG_ERROR(("Failed to wait G2 trunk mapping register busy bit.\n"));
            msdSemGive(dev->devNum, dev->tblRegsSem);
            return MSD_FAIL;
        }
    }
	/* Write TrunkId[4] in Global 2, Misc Register 0x1D, bit 13 */
	data = (MSD_U16)((trunkId&0x10)>>4);
	retVal = msdSetAnyRegField(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_MISC, 13, 1, data);
    /* flush will get called here */
    if (retVal != MSD_OK)
	{
        MSD_DBG_ERROR(("msdSetAnyRegField for G2 misc register returned: %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum, dev->tblRegsSem);
		return retVal;
	}

	/* Set related register */
	mask = MSD_LPORTVEC_2_PORTVEC(trunkRoute);
	data = (MSD_U16)(0x8000 | ((trunkId&0xf) << 11) | mask);
	retVal = msdSetAnyReg(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_TRUNK_ROUTING, data);
    retVal2 = msdRegisterAccessFlush(dev->devNum);
	msdSemGive(dev->devNum, dev->tblRegsSem);

    if ((retVal != MSD_OK) || (retVal2 != MSD_OK))
    {
        if (retVal == MSD_OK)
            retVal = retVal2;
        MSD_DBG_ERROR(("msdSetAnyReg for G2 trunk mapping register returned: %s.\n", msdDisplayStatus(retVal)));
		return retVal;
	}

    MSD_DBG_INFO(("Willow_gsysSetTrunkRouting Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gsysGetTrunkRouting
*
* DESCRIPTION:
*       This function retrieves routing information for the given Trunk ID.
*
* INPUTS:
*       trunkId - Trunk ID.
*
* OUTPUTS:
*       trunkRoute - Trunk route bits. Bit 0 controls trunk routing for port 0,
*                    bit 1 for port 1 , etc.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given (if trunkId > 0x1F)
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysGetTrunkRouting
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_U32       trunkId,
    OUT MSD_U32       *trunkRoute
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_STATUS       retVal2;        /* internal return value.      */
    MSD_U16          data;           /* The register's read data.    */
    MSD_U32            mask;
    int count=0x10;

    MSD_DBG_INFO(("Willow_gsysGetTrunkRouting Called.\n"));

	/* Check if trunkId is out of range [0,0x1f] */
	if (!WILLOW_IS_TRUNK_ID_VALID(dev, trunkId))
	{
		MSD_DBG_INFO(("MSD_BAD_PARAM (trunkId: %u)\n", (unsigned int)trunkId));
		return MSD_BAD_PARAM;
	}

    msdSemTake(dev->devNum, dev->tblRegsSem,OS_WAIT_FOREVER);

    /* Check if the register can be accessed. */
    if (!IS_RMU_SUPPORTED(dev))
    {
        do
        {
		    retVal = msdGetAnyReg(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_TRUNK_ROUTING, &data);
            if(retVal != MSD_OK)
            {
			    MSD_DBG_ERROR(("Read G2 trunk mapping register busy bit returned: %s.\n", msdDisplayStatus(retVal)));
                msdSemGive(dev->devNum, dev->tblRegsSem);
                return retVal;
            }
		    if((count--)==0) 
		    {
			    MSD_DBG_ERROR(("Wait G2 trunk mapping register busy bit time out.\n"));
			    msdSemGive(dev->devNum, dev->tblRegsSem);
			    return MSD_FAIL;
		    }
        } while (data & 0x8000);
    }
    else
    {
        if (Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_TRUNK_ROUTING, 15, 0) != MSD_OK)
        {
            msdRegisterAccessFlush(dev->devNum);
            MSD_DBG_ERROR(("Failed to wait G2 trunk mapping register busy bit.\n"));
            msdSemGive(dev->devNum, dev->tblRegsSem);
            return MSD_FAIL;
        }
    }


	/* Write TrunkId[4] in Global 2, Misc Register 0x1D, bit 13 */
	data = (MSD_U16)((trunkId&0x10)>>4);
	retVal = msdSetAnyRegField(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_MISC, 13, 1, data);
    /* flush was done */
    if(retVal != MSD_OK)
	{
        MSD_DBG_ERROR(("msdSetAnyRegField for G2 misc register returned: %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum, dev->tblRegsSem);
		return retVal;
	}

	/* Set related register */
	data = (MSD_U16)((trunkId&0xf) << 11);
	retVal = msdSetAnyReg(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_TRUNK_ROUTING, data);
    if(retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        MSD_DBG_ERROR(("msdSetAnyReg for G2 trunk mapping register returned: %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum, dev->tblRegsSem);
		return retVal;
    }

	/* Read-back value */
	retVal = msdGetAnyReg(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_TRUNK_ROUTING, &data);
    retVal2 = msdRegisterAccessFlush(dev->devNum);
	msdSemGive(dev->devNum, dev->tblRegsSem);

    if ((retVal != MSD_OK) || (retVal2 != MSD_OK))
    {
        if (retVal == MSD_OK)
            retVal = retVal2;
	    MSD_DBG_ERROR(("msdGetAnyReg for G2 trunk mapping register returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    mask = (1 << dev->maxPorts) - 1;
    *trunkRoute = MSD_PORTVEC_2_LPORTVEC(data & mask);

    MSD_DBG_INFO(("Willow_gsysGetTrunkRouting Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gsysSetFloodBC
*
* DESCRIPTION:
*       Flood Broadcast.
*       When Flood Broadcast is enabled, frames with the Broadcast destination 
*       address will flood out all the ports regardless of the setting of the
*       port's Egress Floods mode (see gprtSetEgressFlood API). VLAN rules and 
*       other switch policy still applies to these Broadcast frames. 
*       When this feature is disabled, frames with the Broadcast destination
*       address are considered Multicast frames and will be affected by port's 
*       Egress Floods mode.
*
* INPUTS:
*       en - MSD_TRUE to enable Flood Broadcast, MSD_FALSE otherwise.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysSetFloodBC
(
    IN MSD_QD_DEV    *dev,
    IN MSD_BOOL        en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U16            data;

    MSD_DBG_INFO(("Willow_gsysSetFloodBC Called.\n"));

	MSD_BOOL_2_BIT(en, data);

    /* Set related bit */
	retVal = msdSetAnyRegField(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_MANAGEMENT, 12, 1, data);
    /* this has called msdRegisterAccessFlush automatically */
    if (retVal != MSD_OK)
    {
		MSD_DBG_ERROR(("msdSetAnyRegField for G2 management register returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gsysSetFloodBC Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gsysGetFloodBC
*
* DESCRIPTION:
*       Flood Broadcast.
*       When Flood Broadcast is enabled, frames with the Broadcast destination 
*       address will flood out all the ports regardless of the setting of the
*       port's Egress Floods mode (see gprtSetEgressFlood API). VLAN rules and 
*       other switch policy still applies to these Broadcast frames. 
*       When this feature is disabled, frames with the Broadcast destination
*       address are considered Multicast frames and will be affected by port's 
*       Egress Floods mode.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       en - MSD_TRUE if Flood Broadcast is enabled, MSD_FALSE otherwise.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysGetFloodBC
(
    IN  MSD_QD_DEV    *dev,
    OUT MSD_BOOL      *en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U16          data;           /* The register's read data.    */
    MSD_DBG_INFO(("Willow_gsysGetFloodBC Called.\n"));

    /* Get related bit */
	retVal = msdGetAnyRegField(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_MANAGEMENT, 12, 1, &data);
    /* this has called msdRegisterAccessFlush automatically */
    if (retVal != MSD_OK)
    {
		MSD_DBG_ERROR(("msdGetAnyRegField for G2 management register returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

	MSD_BIT_2_BOOL(data, *en);
    MSD_DBG_INFO(("Willow_gsysGetFloodBC Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gsysSetMonitorMgmtCtrl
*
* DESCRIPTION:
*       Set Monitor and mgmt control data. 
*         
* INPUTS:
*       point - Pointer to the Monitor and mgmt control register.
*           The Pointer bits are used to access the Index registers as follows:
*              0x00 to 0x01: Rsvd2Cpu Enables for 01:C2:80:00:00:0x
*              0x02 to 0x03: Rsvd2Cpu Enables for 01:C2:80:00:00:2x
*              0x04 to 0x05: Rsvd2IntCpu Enables for 01:C2:80:00:00:0x
*              0x06 to 0x07: Rsvd2IntCpu Enables for 01:C2:80:00:00:2x
*              0x04 to 0x1F: Reserved for future use.
*              0x20 to 0x2F: Mirror Destination Port Settings
*              0x30 to 0x3F: Trap Destination Port Settings
*       data  - Monitor and mgmt Control data written to the register 
*                pointed to by the point above.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Willow_gsysSetMonitorMgmtCtrl
(
    IN  MSD_QD_DEV         *dev,
    IN  MSD_U8             point,
    IN  MSD_U8            data
)
{
    MSD_STATUS    retVal;         /* Functions return value.      */
    MSD_STATUS    retVal2;        /* internal return value.      */
    MSD_U16            tmpData;
    int count=0x10;
    MSD_DBG_INFO(("Willow_gsysSetMonitorMgmtCtrl Called.\n"));

    if (point >= 64)
    {
        MSD_DBG_INFO(("MSD_BAD_PARAM (point: %u). It should be within [0, 63].\n", point));
        return MSD_BAD_PARAM;
    }

    msdSemTake(dev->devNum, dev->tblRegsSem,OS_WAIT_FOREVER);

	/*Wait busy bit*/
    if (!IS_RMU_SUPPORTED(dev))
    {
        tmpData = 1;
        while (tmpData == 1)
        {
            retVal = msdGetAnyRegField(dev->devNum, WILLOW_GLOBAL1_DEV_ADDR, WILLOW_QD_REG_MONITOR_MGMT_CTRL, 15, 1, &tmpData);
            if (retVal != MSD_OK)
            {
                MSD_DBG_ERROR(("Read G1 monitor&mgmt register busy bit returned: %s.\n", msdDisplayStatus(retVal)));
                msdSemGive(dev->devNum, dev->tblRegsSem);
                return retVal;
            }

            if ((count--) == 0)
            {
                MSD_DBG_ERROR(("Wait G1 monitor&mgmt register busy bit time out.\n"));
                msdSemGive(dev->devNum, dev->tblRegsSem);
                return MSD_FAIL;
            }
        }
    }
    else
    {
        if (Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL1_DEV_ADDR, WILLOW_QD_REG_MONITOR_MGMT_CTRL, 15, 0) != MSD_OK)
        {
            msdRegisterAccessFlush(dev->devNum);
            MSD_DBG_ERROR(("Failed to wait G2 trunk mapping register busy bit.\n"));
            msdSemGive(dev->devNum, dev->tblRegsSem);
            return MSD_FAIL;
        }
    }

	tmpData =  (MSD_U16)((1 << 15) | (point << 8) | data);
	retVal = msdSetAnyReg(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR, WILLOW_QD_REG_MONITOR_MGMT_CTRL, tmpData);
    retVal2 = msdRegisterAccessFlush(dev->devNum);
	msdSemGive(dev->devNum, dev->tblRegsSem);

    if ((retVal != MSD_OK) || (retVal2 != MSD_OK))
    {
        if (retVal == MSD_OK)
            retVal = retVal2;
        MSD_DBG_ERROR(("msdSetAnyReg for G1 monitor&mgmt register returned: %s.\n", msdDisplayStatus(retVal)));
		return retVal;
	}

    MSD_DBG_INFO(("Willow_gsysSetMonitorMgmtCtrl Exit.\n"));
	return retVal;
}

/*******************************************************************************
* Willow_gsysGetMonitorMgmtCtrl
*
* DESCRIPTION:
*       Get Monitor and mgmt control data. 
*         
* INPUTS:
*       point - Pointer to the Monitor and mgmt control register.
*           The Pointer bits are used to access the Index registers as follows:
*              0x00 to 0x01: Rsvd2Cpu Enables for 01:C2:80:00:00:0x
*              0x02 to 0x03: Rsvd2Cpu Enables for 01:C2:80:00:00:2x
*              0x04 to 0x05: Rsvd2IntCpu Enables for 01:C2:80:00:00:0x
*              0x06 to 0x07: Rsvd2IntCpu Enables for 01:C2:80:00:00:2x
*              0x04 to 0x1F: Reserved for future use.
*              0x20 to 0x2F: Mirror Destination Port Settings
*              0x30 to 0x3F: Trap Destination Port Settings
*
* OUTPUTS:
*       data  - Monitor and mgmt Control data written to the register 
*                pointed to by the point above.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Willow_gsysGetMonitorMgmtCtrl
(
    IN  MSD_QD_DEV         *dev,
    IN  MSD_U8             point,
    OUT  MSD_U8            *data
)
{
    MSD_STATUS    retVal;         /* Functions return value.      */
    MSD_STATUS    retVal2;        /* internal return value.      */
	MSD_U16	tmpData;
	int count=0x10;
    MSD_DBG_INFO(("Willow_gsysGetMonitorMgmtCtrl Called.\n"));

	if (point >= 64)
	{
		MSD_DBG_INFO(("MSD_BAD_PARAM (point: %u). It should be within [0, 63].\n", point));
		return MSD_BAD_PARAM;
	}

	msdSemTake(dev->devNum, dev->tblRegsSem,OS_WAIT_FOREVER);

    if (!IS_RMU_SUPPORTED(dev))
    {
        do {
		    retVal = msdGetAnyReg(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR, WILLOW_QD_REG_MONITOR_MGMT_CTRL, &tmpData);
            if(retVal != MSD_OK)
		    {
			    MSD_DBG_ERROR(("Read G1 monitor&mgmt register busy bit returned: %s.\n", msdDisplayStatus(retVal)));
			    msdSemGive(dev->devNum, dev->tblRegsSem);
			    return retVal;
            }
		    if((count--)==0) 
		    {
			    MSD_DBG_ERROR(("Wait G1 monitor&mgmt register busy bit time out.\n"));
			    msdSemGive(dev->devNum, dev->tblRegsSem);
			    return MSD_FAIL;
		    }
        } while (tmpData&0x8000);
    }
    else
    {
        if (Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL1_DEV_ADDR, WILLOW_QD_REG_MONITOR_MGMT_CTRL, 15, 0) != MSD_OK)
        {
            msdRegisterAccessFlush(dev->devNum);
            MSD_DBG_ERROR(("Failed to wait G2 trunk mapping register busy bit.\n"));
            msdSemGive(dev->devNum, dev->tblRegsSem);
            return MSD_FAIL;
        }
    }

	tmpData =  (MSD_U16)((point << 8) | 0);
	retVal = msdSetAnyReg(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR, WILLOW_QD_REG_MONITOR_MGMT_CTRL, tmpData);
	if(retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
        MSD_DBG_ERROR(("msdSetAnyReg for G1 monitor&mgmt register returned: %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum, dev->tblRegsSem);
		return retVal;
	}

	retVal = msdGetAnyReg(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR, WILLOW_QD_REG_MONITOR_MGMT_CTRL, &tmpData);
    retVal2 = msdRegisterAccessFlush(dev->devNum);

    if ((retVal != MSD_OK) || (retVal2 != MSD_OK))
    {
        if (retVal == MSD_OK)
            retVal = retVal2; 
        MSD_DBG_ERROR(("msdGetAnyReg for G1 monitor&mgmt register returned: %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum, dev->tblRegsSem);
		return retVal;
    }
	*data = (MSD_U8)(tmpData&0xff);

	msdSemGive(dev->devNum, dev->tblRegsSem);

    MSD_DBG_INFO(("Willow_gsysGetMonitorMgmtCtrl Exit.\n"));
    return retVal;
}


/*******************************************************************************
* Willow_gsysSetCPUDest
*
* DESCRIPTION:
*       This routine sets CPU Destination Port. CPU Destination port indicates the
*       port number on this device where the CPU is connected (either directly or
*       indirectly through another Marvell switch device).
*
*       Many modes of frame processing need to know where the CPU is located.
*       These modes are:
*        1. When IGMP/MLD frame is received and Snooping is enabled
*        2. When the port is configured as a DSA port and it receives a To_CPU frame
*        3. When a Rsvd2CPU frame enters the port
*        4. When the port's SA Filtering mode is Drop to CPU
*        5. When any of the port's Policy Options trap the frame to the CPU
*        6. When the ingressing frame is an ARP and ARP mirroring is enabled in the
*           device
*
*       In all cases, except for ARP, the frames that meet the enabled criteria 
*       are mapped to the CPU Destination port, overriding where the frame would 
*       normally go. In the case of ARP, the frame will be mapped normally and it 
*       will also Willow_get copied to this port.
*       Frames that filtered or discarded will not be mapped to the CPU Destination 
*       port with the exception of the Rsvd2CPU and DSA Tag cases.
*
*       If CPUDest = 0x1F, the remapped frames will be discarded, no ARP mirroring 
*       will occur and ingressing To_CPU frames will be discarded.
*
* INPUTS:
*       port  - the logical port number.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysSetCPUDest
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT        port
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			data;

    MSD_DBG_INFO(("Willow_gsysSetCPUDest Called.\n"));

    /* translate LPORT to hardware port */
	hwPort = MSD_LPORT_2_PORT(port);

    /* translate LPORT to hardware port */
    if(port == 0x1F || port == 0x1E)
        hwPort = (MSD_U8)port;
    else
    {
		hwPort = (MSD_U8)MSD_LPORT_2_PORT(port);
		if (hwPort == MSD_INVALID_PORT)
		{
			MSD_DBG_INFO(("Bad port %u.\n", (unsigned int)port));
			return MSD_BAD_PARAM;
		}
    }

	retVal = Willow_gsysGetMonitorMgmtCtrl(dev,0x30,&data);
    if(retVal != MSD_OK)
    {
		MSD_DBG_ERROR(("Willow_gsysGetMonitorMgmtCtrl returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

	data = ((data & 0xE0) | hwPort);
	retVal = Willow_gsysSetMonitorMgmtCtrl(dev,0x30, (MSD_U8)data);
	if(retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_gsysSetMonitorMgmtCtrl returned: %s.\n", msdDisplayStatus(retVal)));
		return retVal;
	}

	
    MSD_DBG_INFO(("Willow_gsysSetCPUDest Exit.\n"));
	return retVal;
}

/*******************************************************************************
* Willow_gsysGetCPUDest
*
* DESCRIPTION:
*       This routine Willow_gets CPU Destination Port. CPU Destination port indicates the
*       port number on this device where the CPU is connected (either directly or
*       indirectly through another Marvell switch device).
*
*       Many modes of frame processing need to know where the CPU is located.
*       These modes are:
*        1. When IGMP/MLD frame is received and Snooping is enabled
*        2. When the port is configured as a DSA port and it receives a To_CPU frame
*        3. When a Rsvd2CPU frame enters the port
*        4. When the port's SA Filtering mode is Drop to CPU
*        5. When any of the port's Policy Options trap the frame to the CPU
*        6. When the ingressing frame is an ARP and ARP mirroring is enabled in the
*           device
*
*       In all cases, except for ARP, the frames that meet the enabled criteria 
*       are mapped to the CPU Destination port, overriding where the frame would 
*       normally go. In the case of ARP, the frame will be mapped normally and it 
*       will also Willow_get copied to this port.
*       Frames that filtered or discarded will not be mapped to the CPU Destination 
*       port with the exception of the Rsvd2CPU and DSA Tag cases.
*
*       If CPUDest = 0x1F, the remapped frames will be discarded, no ARP mirroring 
*       will occur and ingressing To_CPU frames will be discarded.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       port  - the logical port number.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysGetCPUDest
(
    IN  MSD_QD_DEV    *dev,
    OUT MSD_LPORT      *port
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U16          data;           /* The register's read data.    */
    MSD_U8 datac;

    MSD_DBG_INFO(("Willow_gsysGetCPUDest Called.\n"));

    retVal = Willow_gsysGetMonitorMgmtCtrl(dev,0x30, &datac);
    if(retVal != MSD_OK)
	{
	    MSD_DBG_ERROR(("Willow_gsysGetMonitorMgmtCtrl returned: %s.\n", msdDisplayStatus(retVal)));
	    return retVal;
	}
	data = (datac&0x1F);

    if(data == 0x1F || data == 0x1E)
    {
        *port = (MSD_LPORT)data;
    }
    else
    {
        *port = MSD_PORT_2_LPORT((MSD_U8)data);
    }

    MSD_DBG_INFO(("Willow_gsysGetCPUDest Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gsysSetIngressMonitorDest
*
* DESCRIPTION:
*       This routine sets Ingress Monitor Destination Port. Frames that are 
*       tarWillow_geted toward an Ingress Monitor Destination go out the port number 
*       indicated in these bits. This includes frames received on a Marvell Tag port
*       with the Ingress Monitor type, and frames received on a Network port that 
*       is enabled to be the Ingress Monitor Source Port.
*       If the Ingress Monitor Destination Port resides in this device these bits 
*       should point to the Network port where these frames are to egress. If the 
*       Ingress Monitor Destination Port resides in another device these bits 
*       should point to the Marvell Tag port in this device that is used to Willow_get 
*       to the device that contains the Ingress Monitor Destination Port.
*
* INPUTS:
*       port  - the logical port number.when port = 0x1F,IMD is disabled.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysSetIngressMonitorDest
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT        port
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */

    MSD_DBG_INFO(("Willow_gsysSetIngressMonitorDest Called.\n"));

    /* translate LPORT to hardware port */
	hwPort = MSD_LPORT_2_PORT(port);

    /* translate LPORT to hardware port */
    if(port == 0x1F || port == 0x1E)
        hwPort = (MSD_U8)port;
    else
    {
		hwPort = (MSD_U8)MSD_LPORT_2_PORT(port);
        if (hwPort == MSD_INVALID_PORT)
		{
			MSD_DBG_INFO(("Bad port %u.\n", (unsigned int)port));
			return MSD_BAD_PARAM;
		}
    }

	retVal = Willow_gsysSetMonitorMgmtCtrl(dev,0x20, (MSD_U8)hwPort);
	if(retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_gsysSetMonitorMgmtCtrl returned: %s.\n", msdDisplayStatus(retVal)));
		return retVal;
	}
	
    MSD_DBG_INFO(("Willow_gsysSetIngressMonitorDest Exit.\n"));
	return retVal;
}

/*******************************************************************************
* Willow_gsysGetIngressMonitorDest
*
* DESCRIPTION:
*       This routine Willow_gets Ingress Monitor Destination Port.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       port  - the logical port number.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysGetIngressMonitorDest
(
    IN  MSD_QD_DEV    *dev,
    OUT MSD_LPORT      *port
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U16          data;           /* The register's read data.    */
    MSD_U8 datac;

    MSD_DBG_INFO(("Willow_gsysGetIngressMonitorDest Called.\n"));

    retVal = Willow_gsysGetMonitorMgmtCtrl(dev,0x20, &datac);
    if(retVal != MSD_OK)
	{
        MSD_DBG_ERROR(("Willow_gsysGetMonitorMgmtCtrl returned: %s.\n", msdDisplayStatus(retVal)));
	    return retVal;
	}
	data = datac;

    if(data == 0x1F || data == 0x1E)
    {
        *port = (MSD_LPORT)data;
    }
    else
    {
        *port = MSD_PORT_2_LPORT((MSD_U8)data);
    }

    MSD_DBG_INFO(("Willow_gsysGetIngressMonitorDest Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gsysSetEgressMonitorDest
*
* DESCRIPTION:
*       This routine sets Egress Monitor Destination Port. Frames that are 
*       tarWillow_geted toward an Egress Monitor Destination go out the port number 
*       indicated in these bits. This includes frames received on a Marvell Tag port
*       with the Egress Monitor type, and frames transmitted on a Network port that 
*       is enabled to be the Egress Monitor Source Port.
*       If the Egress Monitor Destination Port resides in this device these bits 
*       should point to the Network port where these frames are to egress. If the 
*       Egress Monitor Destination Port resides in another device these bits 
*       should point to the Marvell Tag port in this device that is used to Willow_get 
*       to the device that contains the Egress Monitor Destination Port.
*
* INPUTS:
*       port  - the logical port number.If port = 0x1F, EMD is disabled
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysSetEgressMonitorDest
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT        port
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */

    MSD_DBG_INFO(("Willow_gsysSetEgressMonitorDest Called.\n"));

    /* translate LPORT to hardware port */
	hwPort = MSD_LPORT_2_PORT(port);

    if(port == 0x1F || port == 0x1E)
        hwPort = (MSD_U8)port;
    else
    {
		hwPort = (MSD_U8)MSD_LPORT_2_PORT(port);
        if (hwPort == MSD_INVALID_PORT)
		{
			MSD_DBG_INFO(("Bad port %u.\n", (unsigned int)port));
			return MSD_BAD_PARAM;
		}
    }

    retVal = Willow_gsysSetMonitorMgmtCtrl(dev,0x21, (MSD_U8)hwPort);
    if(retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_gsysSetMonitorMgmtCtrl returned: %s.\n", msdDisplayStatus(retVal)));
	    return retVal;
	}
	
    MSD_DBG_INFO(("Willow_gsysSetEgressMonitorDest Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Willow_gsysGetEgressMonitorDest
*
* DESCRIPTION:
*       This routine Willow_gets Egress Monitor Destination Port.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       port  - the logical port number.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysGetEgressMonitorDest
(
    IN  MSD_QD_DEV    *dev,
    OUT MSD_LPORT      *port
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U16          data;           /* The register's read data.    */
	MSD_U8 datac;

    MSD_DBG_INFO(("Willow_gsysGetEgressMonitorDest Called.\n"));
		
    retVal = Willow_gsysGetMonitorMgmtCtrl(dev,0x21, &datac);
    if(retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_gsysGetMonitorMgmtCtrl returned: %s.\n", msdDisplayStatus(retVal)));
	  return retVal;
	}
	data = datac;


    if(data == 0x1F || data == 0x1E)
    {
        *port = (MSD_LPORT)data;
    }
    else
    {
        *port = MSD_PORT_2_LPORT((MSD_U8)data);
    }
    MSD_DBG_INFO(("Willow_gsysGetEgressMonitorDest Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gsysSetRsvd2CpuEnables
*
* DESCRIPTION:
*       Reserved DA Enables. When the Rsvd2Cpu(Willow_gsysSetRsvd2Cpu) is set to a one, 
*       the 16 reserved multicast DA addresses, whose bit in this register are 
*       also set to a one, are treadted as MGMT frames. All the reserved DA's 
*       take the form 01:80:C2:00:00:0x. When x = 0x0, bit 0 of this register is 
*       tested. When x = 0x2, bit 2 of this field is tested and so on.
*       If the tested bit in this register is cleared to a zero, the frame will 
*       be treated as a normal (non-MGMT) frame.
*
* INPUTS:
*       enBits - bit vector of enabled Reserved Multicast.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysSetRsvd2CpuEnables
(
    IN MSD_QD_DEV    *dev,
    IN MSD_U16        enBits
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */

    MSD_DBG_INFO(("Willow_gsysSetRsvd2CpuEnables Called.\n"));

	retVal = Willow_gsysSetMonitorMgmtCtrl(dev,0x0, (MSD_U8)(enBits&0xff));
	if(retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_gsysSetMonitorMgmtCtrl for point 0 returned: %s.\n", msdDisplayStatus(retVal)));
		return retVal;
	}
	retVal = Willow_gsysSetMonitorMgmtCtrl(dev,0x1, (MSD_U8)((enBits>>8)&0xff));
	if(retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_gsysSetMonitorMgmtCtrl for point 1 returned: %s.\n", msdDisplayStatus(retVal)));
		return retVal;
	}
	    
	MSD_DBG_INFO(("Willow_gsysSetRsvd2CpuEnables Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Willow_gsysGetRsvd2CpuEnables
*
* DESCRIPTION:
*       Reserved DA Enables. When the Rsvd2Cpu(Willow_gsysSetRsvd2Cpu) is set to a one, 
*       the 16 reserved multicast DA addresses, whose bit in this register are 
*       also set to a one, are treadted as MGMT frames. All the reserved DA's 
*       take the form 01:80:C2:00:00:0x. When x = 0x0, bit 0 of this register is 
*       tested. When x = 0x2, bit 2 of this field is tested and so on.
*       If the tested bit in this register is cleared to a zero, the frame will 
*       be treated as a normal (non-MGMT) frame.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       enBits - bit vector of enabled Reserved Multicast.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysGetRsvd2CpuEnables
(
    IN  MSD_QD_DEV    *dev,
    OUT MSD_U16      *enBits
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U8 data;
	MSD_U16 dataw;

    MSD_DBG_INFO(("Willow_gsysGetRsvd2CpuEnables Called.\n"));

	retVal = Willow_gsysGetMonitorMgmtCtrl(dev,0x0, &data);
	if(retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_gsysGetMonitorMgmtCtrl for point 0 returned: %s.\n", msdDisplayStatus(retVal)));
		return retVal;
	}
	*enBits = data;
	retVal = Willow_gsysGetMonitorMgmtCtrl(dev,0x1, &data);
	if(retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_gsysGetMonitorMgmtCtrl for point 1 returned: %s.\n", msdDisplayStatus(retVal)));
		return retVal;
	}
	dataw=data;
	dataw <<=8;
	*enBits |= (dataw&0xff00);

	MSD_DBG_INFO(("Willow_gsysGetRsvd2CpuEnables Exit.\n"));
	return retVal;
}

/*******************************************************************************
* Willow_gsysSetMGMTPri
*
* DESCRIPTION:
*       These bits are used as the PRI[2:0] bits on Rsvd2CPU frames.
*
* INPUTS:
*       pri - PRI[2:0] bits (should be less than 8)
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given (If pri is not less than 8)
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysSetMGMTPri
(
    IN MSD_QD_DEV    *dev,
    IN MSD_U16        pri
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U8			data;

    MSD_DBG_INFO(("Willow_gsysSetMGMTPri Called.\n"));

    if (pri > 0x7)
    {
        MSD_DBG_INFO(("MSD_BAD_PARAM (pri: %u). It should be within [0, 7].\n", (unsigned int)pri));
        return MSD_BAD_PARAM;
    }

	retVal = Willow_gsysGetMonitorMgmtCtrl(dev,0x30,&data);
    if(retVal != MSD_OK)
    {
		MSD_DBG_ERROR(("Willow_gsysGetMonitorMgmtCtrl returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    /* Set related bit */
	data = (MSD_U8)((data & 0x1F) | ((pri & 0x7)<<5));
    retVal = Willow_gsysSetMonitorMgmtCtrl(dev,0x30,data);
    if(retVal != MSD_OK)
    {
		MSD_DBG_ERROR(("Willow_gsysSetMonitorMgmtCtrl returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gsysSetMGMTPri Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gsysGetMGMTPri
*
* DESCRIPTION:
*       These bits are used as the PRI[2:0] bits on Rsvd2CPU frames.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       pri - PRI[2:0] bits (should be less than 8)
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysGetMGMTPri
(
    IN  MSD_QD_DEV    *dev,
    OUT MSD_U16      *pri
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U8			data;

    MSD_DBG_INFO(("Willow_gsysGetMGMTPri Called.\n"));

    /* Get related bit */
	retVal = Willow_gsysGetMonitorMgmtCtrl(dev,0x30,&data);
    if(retVal != MSD_OK)
    {
		MSD_DBG_ERROR(("Willow_gsysGetMonitorMgmtCtrl returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

	*pri = ((data & 0xE0) >> 5);

    MSD_DBG_INFO(("Willow_gsysGetMGMTPri Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gsysSetSMISetup
*
* DESCRIPTION:
*       This routine is to set SMI PHY Setup
*
* INPUTS:
*       pointer - [9:0] bits (should be less than 0x400)
*		data	- SMI PHY Setup.
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given (If pri is not less than 8)
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysSetSMISetup
(
IN MSD_QD_DEV    *dev,
IN MSD_LPORT  portNum,
IN MSD_U16		  data
)
{
	MSD_STATUS       retVal;         /* Functions return value.      */	
	MSD_U16			 tmpdata;
	MSD_U32    		 timeOut;
	timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */
	MSD_DBG_INFO(("Willow_gsysSetSMISetup Called.\n"));

	if (portNum > 10)
	{
		MSD_DBG_INFO(("MSD_BAD_PARAM (portNum: %u). It should be within [0,10].\n", (unsigned int)portNum));
		return MSD_BAD_PARAM;
	}
	if (data > 0x1f)
	{
		MSD_DBG_INFO(("MSD_BAD_PARAM (regaddress: %u). It should be within [0,31].\n", (unsigned int)data));
		return MSD_BAD_PARAM;
	}
	retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_SMI_PHY_DATA, data);
    if (retVal != MSD_OK) {
        msdRegisterAccessFlush(dev->devNum);
        MSD_DBG_ERROR(("write WILLOW_QD_REG_SMI_PHY_DATA register returned: %s.\n", msdDisplayStatus(MSD_FAIL)));
        return MSD_FAIL;
    }
    if (!IS_RMU_SUPPORTED(dev))
    {
        do
        {
            if (msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_SMI_PHY_CMD, &tmpdata) != MSD_OK)
            {
                MSD_DBG_ERROR(("read WILLOW_QD_REG_SMI_PHY_CMD register returned: %s.\n", msdDisplayStatus(MSD_FAIL)));
                return MSD_FAIL;
            }
            if (timeOut-- < 1)
            {
                MSD_DBG_ERROR(("read SMI Phy WILLOW_QD_REG_SMI_PHY_CMD register Timed Out\n"));
                return MSD_FAIL;
            }
        } while (tmpdata & MSD_SMI_BUSY);
    }
    else
    {
        if (Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_SMI_PHY_CMD, 15, 0) != MSD_OK)
        {
            msdRegisterAccessFlush(dev->devNum);
            MSD_DBG_ERROR(("wait for WILLOW_QD_REG_SMI_PHY_CMD register returned: %s.\n", msdDisplayStatus(MSD_FAIL)));
            return MSD_FAIL;
        }
    }
	tmpdata = (MSD_U16)(0xc000 | 0x0400 | portNum);

	if (msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_SMI_PHY_CMD, tmpdata) != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
		MSD_DBG_ERROR(("Write WILLOW_QD_REG_SMI_PHY_DATA register returned: %s.\n", msdDisplayStatus(MSD_FAIL)));
		return MSD_FAIL;
	}
	
    if (!IS_RMU_SUPPORTED(dev))
    {
        do
	    {
		    if (msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_SMI_PHY_CMD, &tmpdata) != MSD_OK)
		    {
			    MSD_DBG_ERROR(("read WILLOW_QD_REG_SMI_PHY_CMD register returned: %s.\n", msdDisplayStatus(MSD_FAIL)));
			    return MSD_FAIL;
		    }
		    if (timeOut-- < 1)
		    {
			    MSD_DBG_ERROR(("read SMI Phy WILLOW_QD_REG_SMI_PHY_CMD register Timed Out\n"));
			    return MSD_FAIL;
		    }
	    } while (tmpdata & MSD_SMI_BUSY);
    }
    else
    {
        if (Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_SMI_PHY_CMD, 15, 0) != MSD_OK)
        {
            msdRegisterAccessFlush(dev->devNum);
            MSD_DBG_ERROR(("wait for WILLOW_QD_REG_SMI_PHY_CMD register returned: %s.\n", msdDisplayStatus(MSD_FAIL)));
            return MSD_FAIL;
        }
    }
    retVal = msdRegisterAccessFlush(dev->devNum);


	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_gsysSetSMISetup returned: %s.\n", msdDisplayStatus(retVal)));
		return retVal;
	}

	MSD_DBG_INFO(("Willow_gsysSetSMISetup Exit.\n"));
	return MSD_OK;
}

/*******************************************************************************
* Willow_gsysGetSMISetup
*
* DESCRIPTION:
*       This routine is to get SMI PHY Setup
*
* INPUTS:
*       portNum - [9:0] bits (should be less than 0x400)
*		
* OUTPUTS:
*       data	- SMI PHY Setup.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gsysGetSMISetup
(
IN  MSD_QD_DEV		*dev,
IN  MSD_LPORT		portNum,
OUT MSD_U16			*data
)
{
	MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U16			 tmpdata;
	MSD_U32    		 timeOut;
	timeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

	MSD_DBG_INFO(("Willow_gsysGetSMISetup Called.\n"));

	if (portNum > 10)
	{
		MSD_DBG_INFO(("MSD_BAD_PARAM (pointer: %u). It should be within [0,10].\n", (unsigned int)portNum));
		return MSD_BAD_PARAM;
	}
    if (!IS_RMU_SUPPORTED(dev))
    {
        do
        {
            if (msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_SMI_PHY_CMD, &tmpdata) != MSD_OK)
            {
                MSD_DBG_ERROR(("read WILLOW_QD_REG_SMI_PHY_CMD register returned: %s.\n", msdDisplayStatus(MSD_FAIL)));
                return MSD_FAIL;
            }
            if (timeOut-- < 1)
            {
                MSD_DBG_ERROR(("read SMI Phy WILLOW_QD_REG_SMI_PHY_CMD register Timed Out\n"));
                return MSD_FAIL;
            }
        } while (tmpdata & MSD_SMI_BUSY);
    }
    else
    {
        if (Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_SMI_PHY_CMD, 15, 0) != MSD_OK)
        {
            msdRegisterAccessFlush(dev->devNum);
            MSD_DBG_ERROR(("wait for WILLOW_QD_REG_SMI_PHY_CMD register returned: %s.\n", msdDisplayStatus(MSD_FAIL)));
            return MSD_FAIL;
        }
    }

	tmpdata = (MSD_U16)(0xc000 | 0x0800 | portNum);

	if (msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_SMI_PHY_CMD, tmpdata) != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
        MSD_DBG_ERROR(("Write WILLOW_QD_REG_SMI_PHY_DATA register returned: %s.\n", msdDisplayStatus(MSD_FAIL)));
		return MSD_FAIL;
	}

    if (!IS_RMU_SUPPORTED(dev))
    {
        do
        {
            if (msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_SMI_PHY_CMD, &tmpdata) != MSD_OK)
            {
                MSD_DBG_ERROR(("read WILLOW_QD_REG_SMI_PHY_CMD register returned: %s.\n", msdDisplayStatus(MSD_FAIL)));
                return MSD_FAIL;
            }
            if (timeOut-- < 1)
            {
                MSD_DBG_ERROR(("read SMI Phy WILLOW_QD_REG_SMI_PHY_CMD register Timed Out\n"));
                return MSD_FAIL;
            }
        } while (tmpdata & MSD_SMI_BUSY);
    }
    else
    {
        if (Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_SMI_PHY_CMD, 15, 0) != MSD_OK)
        {
            msdRegisterAccessFlush(dev->devNum);
            MSD_DBG_ERROR(("wait for WILLOW_QD_REG_SMI_PHY_CMD register returned: %s.\n", msdDisplayStatus(MSD_FAIL)));
            return MSD_FAIL;
        }
    }

	retVal = msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_SMI_PHY_DATA, data);
    if (retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
        MSD_DBG_ERROR(("msdGetAnyReg returned: %s.\n", msdDisplayStatus(retVal)));
		return retVal;
	}
    retVal = msdRegisterAccessFlush(dev->devNum);

	MSD_DBG_INFO(("Willow_gsysGetSMISetup Exit.\n"));
	return retVal;
}

MSD_STATUS Willow_gsysGlobal1RegDump
(
IN  MSD_QD_DEV    *dev
)
{
	MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U16	data[32];
	MSD_U8 i = 0;
	char desc[32][48] = 
	{
		{ "Switch Global Status(0x0)" },
		{ "ATU FID Register(0x1)" },
		{ "VTU FID Register(0x2)" },
		{ "VTU SID Register(0x3)" },
		{ "Switch Global Control Register(0x4)" },
		{ "VTU Operation Register(0x5)" },
		{ "VTU VID Register(0x6)" },
		{ "VTU/STU Data Register Ports 0 to 7(0x7)" },
		{ "VTU/STU Data Register Ports 8 to 10(0x8)" },
		{ "Reserved(0x9)" },
		{ "ATU Control Register(0xa)" },
		{ "ATU Operation Register(0xb)" },
		{ "ATU Data Register(0xc)" },
		{ "Switch MAC Address Register Bytes 0 & 1(0xd)" },
		{ "Switch MAC Address Register Bytes 2 & 3(0xe)" },
		{ "Switch MAC Address Register Bytes 4 & 5(0xf)" },
		{ "Reserved(0x10)" },
		{ "Reserved(0x11)" },
		{ "Reserved(0x12)" },
		{ "Reserved(0x13)" },
		{ "Reserved(0x14)" },
		{ "Reserved(0x15)" },
		{ "Reserved(0x16)" },
		{ "Reserved(0x17)" },
		{ "Reserved(0x18)" },
		{ "Reserved(0x19)" },
		{ "Monitor & MGMT Control Register(0x1a)" },
		{ "Total Free Counter(0x1b)" },
		{ "Global Control 2(0x1c)" },
		{ "Stats Operation Register(0x1d)" },
		{ "Stats Counter Register Byte 3 & 2(0x1e)" },
		{ "Stats Counter Register Byte 1 & 0(0x1f)" }
	};

	MSD_DBG_INFO(("Willow_gsysGlobal1RegDump Called.\n"));

	MSG(("\n------------------------------------------------------\n"));
	for (i = 0; i < 32; i++)
	{
		retVal = msdGetAnyReg(dev->devNum, WILLOW_GLOBAL1_DEV_ADDR, i, &data[i]);
		if (retVal != MSD_OK)
		{
            msdRegisterAccessFlush(dev->devNum);
            MSD_DBG_ERROR(("msdGetAnyReg returned: %s.\n", msdDisplayStatus(retVal)));
			return retVal;
		}
	}
    retVal = msdRegisterAccessFlush(dev->devNum);
    if (retVal == MSD_OK)
    {
        for (i = 0; i < 32; i++)
        {
            MSG(("%-48s%04x\n", desc[i], data[i]));
        }
    }

	MSD_DBG_INFO(("Willow_gsysGlobal1RegDump Exit.\n"));
	return retVal;
}

MSD_STATUS Willow_gsysGlobal2RegDump
(
IN  MSD_QD_DEV    *dev
)
{
	MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U16	data[32];
	MSD_U8 i = 0;
	char desc[32][48] =
	{
		{ "Interrupt Source Register(0x0)" },
		{ "Interupt Mask Register(0x1)" },
		{ "Reserved(0x2)" },
		{ "Reserved(0x3)" },
		{ "Flow Control Delay Register(0x4)" },
		{ "Switch Management Register(0x5)" },
		{ "Device Mapping Table Register(0x6)" },
		{ "LAG Mask Table Register(0x7)" },
		{ "LAG Mapping Register(0x8)" },
		{ "Ingress Rate Command Register(0x9)" },
		{ "Ingress Rate Data Register(0xa)" },
		{ "Cross Chip Port VLAN Addr Register(0xb)" },
		{ "Cross Chip Port VLAN Data Register(0xc)" },
		{ "Switch MAC/WoL/WoF register(0xd)" },
		{ "ATU Stats(0xe)" },
		{ "Priority Override Table(0xf)" },
		{ "Reserved(0x10)" },
		{ "Reserved(0x11)" },
		{ "Energy Management Register(0x12)" },
		{ "IMP Communication/Debug Register(0x13)" },
		{ "EEPROM Command(0x14)" },
		{ "EEPROM Addr(0x15)" },
		{ "AVB/TSN Command Register(0x16)" },
		{ "AVB/TSN Data Register(0x17)" },
		{ "SMI PHY Command for SMI Access(0x18)" },
		{ "SMI PHY Data Register(0x19)" },
		{ "Scratch and Misc(0x1a)" },
		{ "Watch Dog Control Register(0x1b)" },
		{ "QoS Weights Register(0x1c)" },
		{ "Misc Register(0x1d)" },
		{ "Reserved(0x1e)" },
		{ "Cut Through Control Register(0x1f)" }
	};

	MSD_DBG_INFO(("Willow_gsysGlobal2RegDump Called.\n"));

	MSG(("\n------------------------------------------------\n"));
    for (i = 0; i < 32; i++)
    {
        retVal = msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, i, &data[i]);
        if (retVal != MSD_OK)
        {
            MSD_DBG_ERROR(("msdGetAnyReg returned: %s.\n", msdDisplayStatus(retVal)));
            return retVal;
        }
    }
    retVal = msdRegisterAccessFlush(dev->devNum);
    if (retVal == MSD_OK)
    {
        for (i = 0; i < 32; i++)
        {
            MSG(("%-42s%04x\n", desc[i], data));
        }
    }
    MSD_DBG_INFO(("Willow_gsysGlobal2RegDump Exit.\n"));
	return retVal;
}
#endif
