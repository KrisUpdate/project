#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/*******************************************************************************
* Willow_msdBrgStu.c
*
* DESCRIPTION:
*       API definitions for SID (VTU 802.1s Port State Information Database) 
*       Translation Unit.
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "Willow_msdBrgStu.h"
#include "Willow_msdApiInternal.h"
#include "Willow_msdHwAccess.h"
#include "Willow_msdDrvSwRegs.h"
#include "msdUtils.h"
#include "msdSem.h"
#include "msdHwAccess.h"

/****************************************************************************/
/* Forward function declaration.                                            */
/****************************************************************************/

typedef enum
{
    Willow_LOAD_PURGE_STU_ENTRY = 5,
    Willow_GET_NEXT_STU_ENTRY =6 
} WILLOW_MSD_STU_OPERATION;

static MSD_STATUS Willow_stuOperationPerform
(
    IN      MSD_QD_DEV           *dev,
    IN      WILLOW_MSD_STU_OPERATION    stuOp,
    INOUT   MSD_U8               *valid,
	INOUT   WILLOW_MSD_STU_ENTRY        *stuEntry
);

/*******************************************************************************
* Willow_gstuGetEntryNext
*
* DESCRIPTION:
*       Gets next valid STU entry from the specified SID.
*
* INPUTS:
*       stuEntry - the SID to start the search.
*
* OUTPUTS:
*       stuEntry - next STU entry.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error
*       MSD_NO_SUCH - no more entries.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None
*
*******************************************************************************/
MSD_STATUS Willow_gstuGetEntryNext
(
    IN  MSD_QD_DEV       *dev,
    INOUT WILLOW_MSD_STU_ENTRY  *stuEntry
)
{
    MSD_U8               valid;
    MSD_STATUS           retVal;
    MSD_U8               port; 
    MSD_LPORT            lport; 
    WILLOW_MSD_STU_ENTRY        entry;

    MSD_DBG_INFO(("Willow_gstuGetEntryNext Called.\n"));
	
    if(stuEntry->sid > 0x3F)
    {		
		MSD_DBG_ERROR(("Bad Sid : %d.\n", stuEntry->sid));
        return MSD_BAD_PARAM;
    }

    entry.sid = stuEntry->sid;
    valid = 0;

    retVal = Willow_stuOperationPerform(dev,Willow_GET_NEXT_STU_ENTRY,&valid, &entry);
    if(retVal != MSD_OK)
    {
		MSD_DBG_ERROR(("Willow_stuOperationPerform Willow_GET_NEXT_STU_ENTRY returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    /* retrieve the value from the operation */ 

	if ((entry.sid == 0x3F) && (valid == 0))
	{
		MSD_DBG_INFO(("Willow_gstuGetEntryNext Exit, MSD_NO_SUCH.\n"));
		return MSD_NO_SUCH;
	}

    stuEntry->sid = entry.sid;

    for(lport=0; lport<dev->numOfPorts; lport++)
    {
        port = MSD_LPORT_2_PORT(lport);
        stuEntry->portState[lport]=entry.portState[port];
    }

    MSD_DBG_INFO(("Willow_gstuGetEntryNext Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gstuAddEntry
*
* DESCRIPTION:
*       Creates or update the entry in STU table based on SID.
*
* INPUTS:
*       stuEntry    - stu entry to insert to the STU.
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gstuAddEntry
(
    IN  MSD_QD_DEV       *dev,
    IN  WILLOW_MSD_STU_ENTRY    *stuEntry
)
{
    MSD_U8               valid;
    MSD_STATUS           retVal;
    MSD_U8           port; 
    MSD_LPORT           lport; 
    WILLOW_MSD_STU_ENTRY        entry;

    MSD_DBG_INFO(("Willow_gstuAddEntry Called.\n"));

    if(stuEntry->sid > 0x3F)
    {
		MSD_DBG_ERROR(("Bad Sid : %d.\n", stuEntry->sid));
        return MSD_BAD_PARAM;
    }

    entry.sid = stuEntry->sid;

    valid = 1; /* for load operation */

    for(port=0; port<dev->maxPorts; port++)
    {
        lport = MSD_PORT_2_LPORT(port);
        if (lport == MSD_INVALID_PORT)
            entry.portState[port] = 0;
        else
            entry.portState[port] = stuEntry->portState[lport];
    }

    retVal = Willow_stuOperationPerform(dev,Willow_LOAD_PURGE_STU_ENTRY,&valid, &entry);
    if(retVal != MSD_OK)
    {
		MSD_DBG_ERROR(("Willow_stuOperationPerform add entry returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gstuAddEntry Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gstuDelEntry
*
* DESCRIPTION:
*       Deletes STU entry specified by SID.
*
* INPUTS:
*       stuEntry - the STU entry to be deleted 
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS Willow_gstuDelEntry
(
    IN  MSD_QD_DEV       *dev,
    IN  WILLOW_MSD_STU_ENTRY    *stuEntry
)
{
    MSD_U8               valid;
    MSD_STATUS           retVal;
    WILLOW_MSD_STU_ENTRY        entry;

    MSD_DBG_INFO(("Willow_gstuDelEntry Called.\n"));
    
    if(stuEntry->sid > 0x3F)
    {
		MSD_DBG_ERROR(("Bad Sid : %d.\n", stuEntry->sid));
        return MSD_BAD_PARAM;
    }

    entry.sid = stuEntry->sid;
    valid = 0; /* for delete operation */

    retVal = Willow_stuOperationPerform(dev,Willow_LOAD_PURGE_STU_ENTRY,&valid, &entry);
    if(retVal != MSD_OK)
    {
		MSD_DBG_ERROR(("Willow_stuOperationPerform Delete entry returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }
    MSD_DBG_INFO(("Willow_gstuDelEntry Exit.\n"));
    return MSD_OK;
}

/****************************************************************************/
/* Internal use functions.                                                  */
/****************************************************************************/

static MSD_STATUS Willow_stuSetSTUData
(
    IN    MSD_QD_DEV           *dev,
    IN    WILLOW_MSD_STU_ENTRY        *entry
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U16          data1,data2;           /* Data to be set into the STU data register     */

    data1 = data2 = 0;

	data1 |= ((entry->portState[7] & 3) << 14) | ((entry->portState[6] & 3) << 12) 
		| ((entry->portState[5] & 3) << 10) | ((entry->portState[4] & 3) << 8)
		| ((entry->portState[3] & 3) << 6) | ((entry->portState[2] & 3) << 4)
		| ((entry->portState[1] & 3) << 2) | (entry->portState[0] & 3);

		
	data2 |= ((entry->portState[10] & 3) << 4) | ((entry->portState[9] & 3) << 2)
		| (entry->portState[8] & 3);


	retVal = msdSetAnyReg(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR, WILLOW_QD_REG_VTU_DATA1_REG, data1);
	if(retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
		/*MSD_DBG_ERROR(("Write STU DATA1 Register returned : %s.\n", msdDisplayStatus(retVal)));*/
		return retVal;
	}

	retVal = msdSetAnyRegField(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR,WILLOW_QD_REG_VTU_DATA2_REG, 0, 6, data2);
	if(retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
        /*MSD_DBG_ERROR(("Write STU DATA2 Register returned: %s.\n", msdDisplayStatus(retVal)));*/
		return retVal;
	}
    return msdRegisterAccessFlush(dev->devNum);
}

static MSD_STATUS Willow_stuGetSTUData
(
    IN    MSD_QD_DEV           *dev,
    OUT    WILLOW_MSD_STU_ENTRY       *entry
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U16          data1,data2;           /* Data to be set into the      */

    data1 = data2 = 0;

    msdMemSet((void*)entry->portState,0,sizeof(entry->portState));

    retVal = msdGetAnyReg(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR,WILLOW_QD_REG_VTU_DATA1_REG,&data1);
    if(retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
		/*MSD_DBG_ERROR(("Read STU DATA1 Register returned : %s.\n", msdDisplayStatus(retVal)));*/
        return retVal;
    }

	retVal = msdGetAnyReg(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR,WILLOW_QD_REG_VTU_DATA2_REG,&data2);
    if(retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        /*MSD_DBG_ERROR(("Read STU DATA2 Register returned: %s.\n", msdDisplayStatus(retVal)));*/
        return retVal;
    }
    retVal = msdRegisterAccessFlush(dev->devNum);
    if (retVal != MSD_OK)
    {
        return retVal;
    }
    entry->portState[10] = (data2 >> 4) & 0x3;
	entry->portState[9] = (data2 >> 2) & 0x3;
	entry->portState[8] = data2 & 0x3;
	
	entry->portState[7] = (data1 >> 14) & 0x3;
	entry->portState[6] = (data1 >> 12) & 0x3;
	entry->portState[5] = (data1 >> 10) & 0x3;
	entry->portState[4] = (data1 >> 8) & 0x3;
	entry->portState[3] = (data1 >> 6) & 0x3;
	entry->portState[2] = (data1 >> 4) & 0x3;
	entry->portState[1] = (data1 >> 2) & 0x3;
	entry->portState[0] = data1 & 0x3;

    return MSD_OK;
}

/*******************************************************************************
* Willow_stuOperationPerform
*
* DESCRIPTION:
*       This function is used by all STU control functions, and is responsible
*       to write the required operation into the STU registers.
*
* INPUTS:
*       stuOp       - The STU operation bits to be written into the STU
*                     operation register.
*       sid         - sid
*       valid       - valid bit
*       stuData     - STU Data with port state information
*
* OUTPUTS:
*       sid         - sid
*       valid       - valid bit
*       stuData     - STU Data with port state information
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*		None.
*
*******************************************************************************/
static MSD_STATUS Willow_stuOperationPerform
(
    IN        MSD_QD_DEV           *dev,
    IN      WILLOW_MSD_STU_OPERATION    stuOp,
    INOUT   MSD_U8               *valid,
    INOUT    WILLOW_MSD_STU_ENTRY        *entry
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U16          data,data1;     /* Data to be set into the      */
                                /* register.                    */

    msdSemTake(dev->devNum, dev->vtuRegsSem,OS_WAIT_FOREVER);

    /* Wait until the VTU in ready. */
    retVal = Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL1_DEV_ADDR, WILLOW_QD_REG_VTU_OPERATION, 15, 0);
    if (retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        msdSemGive(dev->devNum, dev->vtuRegsSem);
        return retVal;
    }
    /* Set the VTU data register if Load operation is required. */
    if (stuOp == Willow_LOAD_PURGE_STU_ENTRY)
    {
        if (*valid == 1)
        {
            /* set the Port State for all the ports */
            retVal = Willow_stuSetSTUData(dev,entry);
            /* in this function a flush is triggered */
            if(retVal != MSD_OK)
            {
                msdSemGive(dev->devNum, dev->vtuRegsSem);
                return retVal;
            }

            /* Set the valid bit (WILLOW_QD_REG_VTU_VID_REG) */
            data = (*valid) << 12 ;
            retVal = msdSetAnyReg(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR,(MSD_U8)(WILLOW_QD_REG_VTU_VID_REG),data);
            if(retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->vtuRegsSem);
                return retVal;
            }        
        }
        else
        {
            /* Clear the valid bit (WILLOW_QD_REG_VTU_VID_REG) */
			data = 0;
            retVal = msdSetAnyReg(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR,(MSD_U8)(WILLOW_QD_REG_VTU_VID_REG),data);
            if(retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->vtuRegsSem);
                return retVal;
            }        
        }
    }

    /* Set the SID register (WILLOW_QD_REG_STU_SID_REG) */
    data= (entry->sid) & 0x3F;
    retVal = msdSetAnyReg(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR,(MSD_U8)(WILLOW_QD_REG_STU_SID_REG),data);
    if(retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        msdSemGive(dev->devNum, dev->vtuRegsSem);
        return retVal;
    }
	
    data = (MSD_U16)((1 << 15) | (stuOp << 12));

    retVal = msdSetAnyReg(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR,WILLOW_QD_REG_VTU_OPERATION,data);
    if(retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        msdSemGive(dev->devNum, dev->vtuRegsSem);
        return retVal;
    }

    /* If the operation is a Willow_get next operation wait for the response   */
    if(stuOp == Willow_GET_NEXT_STU_ENTRY)
    {
        /* Wait until the STU is ready. */
        /* Wait until the VTU is ready. */
        retVal = Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL1_DEV_ADDR, WILLOW_QD_REG_VTU_OPERATION, 15, 0);
        if(retVal != MSD_OK)
        {
            msdRegisterAccessFlush(dev->devNum);
            msdSemGive(dev->devNum, dev->vtuRegsSem);
            return retVal;
        }

        /****************** Willow_get the valid bit *******************/
        retVal = msdGetAnyRegField(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR,WILLOW_QD_REG_VTU_VID_REG,12,1,&data);
        if(retVal != MSD_OK)
        {
            msdRegisterAccessFlush(dev->devNum);
            msdSemGive(dev->devNum, dev->vtuRegsSem);
            return retVal;
        }

        /****************** Willow_get the sid *******************/

        retVal = msdGetAnyReg(dev->devNum,  WILLOW_GLOBAL1_DEV_ADDR,WILLOW_QD_REG_STU_SID_REG,&data1);
        if(retVal != MSD_OK)
        {
            msdRegisterAccessFlush(dev->devNum);
            msdSemGive(dev->devNum, dev->vtuRegsSem);
            return retVal;
        }
        retVal = msdRegisterAccessFlush(dev->devNum);
        if (retVal != MSD_OK)
        {
            msdSemGive(dev->devNum, dev->vtuRegsSem);
            return retVal;
        }

        /* new valid bit */
        *valid = (MSD_U8)data;
        /* the sid is bits 0-5 */
        entry->sid = data1 & 0x3F;

        if (*valid)
        {
            /* Willow_get the Port State for all the ports */
            retVal = Willow_stuGetSTUData(dev,entry);
            /* this function triggers a flush operation */
            if(retVal != MSD_OK)
            {
                msdSemGive(dev->devNum, dev->vtuRegsSem);
                return retVal;
            }
        } /* entry is valid */

    } /* end of Willow_get next entry */

    msdSemGive(dev->devNum, dev->vtuRegsSem);
    return MSD_OK;
}

/*******************************************************************************
* Willow_gstuFindSidEntry
*
* DESCRIPTION:
*       Find STU entry for a specific SID, it will return the entry, if found, 
*       along with its associated data 
*
* INPUTS:
*       stuEntry - contains the SID to searche for 
*
* OUTPUTS:
*       found    - MSD_TRUE, if the appropriate entry exists.
*       stuEntry - the entry parameters.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error or entry does not exist.
*       MSD_NO_SUCH - no such entry.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*        Valid SID is 0 ~ 63.
*
*******************************************************************************/
MSD_STATUS Willow_gstuFindSidEntry
(
    IN  MSD_QD_DEV       *dev,
    INOUT WILLOW_MSD_STU_ENTRY  *stuEntry,
    OUT MSD_BOOL         *found
)
{
    MSD_U8               valid;
    MSD_STATUS           retVal;
    MSD_U8               port;
    MSD_LPORT            lport;
    WILLOW_MSD_STU_ENTRY        entry;

    MSD_DBG_INFO(("Willow_gstuFindSidEntry Called.\n"));
	
    if(stuEntry->sid > 0x3F)
    {
		MSD_DBG_ERROR(("Bad Sid : %d.\n", stuEntry->sid));
        return MSD_BAD_PARAM;
    }

	if (NULL == found)
	{
		MSD_DBG_ERROR(("Input param MSD_BOOL found is NULL. \n"));
		return MSD_BAD_PARAM;
	}

    *found = MSD_FALSE;

    /* Decrement 1 from sid */
	if (stuEntry->sid == 0)
	{
		entry.sid = 0x3F;
	}
	else 
	{
		entry.sid   = stuEntry->sid-1;
	}

    valid = 0; /* valid is not used as input in this operation */

    retVal = Willow_stuOperationPerform(dev,Willow_GET_NEXT_STU_ENTRY,&valid, &entry);
    if(retVal != MSD_OK)
    {
		MSD_DBG_ERROR(("Willow_stuOperationPerform Willow_GET_NEXT_STU_ENTRY returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    /* retrive the value from the operation */ 
	if ((entry.sid != stuEntry->sid) || (valid == 0))
	{
		MSD_DBG_INFO(("Willow_gstuFindSidEntry Exit, MSD_NO_SUCH\n"));
		return MSD_OK;
	}

    for(lport=0; lport<dev->numOfPorts; lport++)
    {
        port = MSD_LPORT_2_PORT(lport);
        stuEntry->portState[lport]=entry.portState[port];
    }

    *found = MSD_TRUE;

    MSD_DBG_INFO(("Willow_gstuFindSidEntry Exit.\n"));
    return MSD_OK;
}
MSD_STATUS Willow_gstuDump
(
IN MSD_QD_DEV    *dev
)
{
	MSD_STATUS status;
	WILLOW_MSD_STU_ENTRY entry;
	int temp;

	msdMemSet(&entry, 0, sizeof(WILLOW_MSD_STU_ENTRY));
	entry.sid = 0x3f;

	/* Print out ATU entry field */
	MSG(("\n-------------------------\n"));
	MSG(("SID  PORTSTATE           \n"));
	MSG(("-------------------------\n"));

	temp = 1;
	while (1 == temp)
	{
		status = Willow_gstuGetEntryNext(dev, &entry);
		if (MSD_NO_SUCH == status)
			break;

		if (MSD_OK != status)
		{
			return status;
		}

		MSG(("%-5x%x %x %x %x %x %x %x %x %x %x %x   \n", entry.sid,
			entry.portState[0], entry.portState[1], entry.portState[2], entry.portState[3], entry.portState[4], entry.portState[5],
			entry.portState[6], entry.portState[7], entry.portState[8], entry.portState[9], entry.portState[10]));

		if (entry.sid == 0x3f)
			break;

	}

	MSG(("\n"));
	return MSD_OK;
}
#endif
