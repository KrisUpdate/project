#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/********************************************************************************
* Pearl_msdPhyCtrl.c
*
* DESCRIPTION:
*       API implementation for switch internal Copper PHY control.
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "Pearl_msdPhyCtrl.h"
#include "Pearl_msdHwAccess.h"
#include "Pearl_msdApiInternal.h"
#include "Pearl_msdDrvSwRegs.h"
#include "msdUtils.h"
#include "msdSem.h"

/*
* This routine set Auto-Negotiation Ad Register for Copper
*/
static
MSD_STATUS Pearl_gigCopperSetAutoMode
(
	IN MSD_QD_DEV *dev,
	IN MSD_U8 hwPort,
	IN PEARL_MSD_PHY_AUTO_MODE mode
)
{
	MSD_U16             u16Data, u16Data1;

	if (Pearl_msdReadPagedPhyReg(dev, hwPort, 0, PEARL_QD_PHY_AUTONEGO_AD_REG, &u16Data) != MSD_OK)
	{
        return MSD_FAIL;
	}

	/* Mask out all auto mode related bits. */
	u16Data &= ~PEARL_QD_PHY_MODE_AUTO_AUTO;

	if (Pearl_msdReadPagedPhyReg(dev, hwPort, 0, PEARL_QD_PHY_AUTONEGO_1000AD_REG, &u16Data1) != MSD_OK)
	{
        return MSD_FAIL;
	}

	/* Mask out all auto mode related bits. */
	u16Data1 &= ~(PEARL_QD_GIGPHY_1000T_FULL | PEARL_QD_GIGPHY_1000T_HALF);

	switch (mode)
	{
		case PEARL_SPEED_AUTO_DUPLEX_AUTO:
			u16Data |= PEARL_QD_PHY_MODE_AUTO_AUTO;
		case PEARL_SPEED_1000_DUPLEX_AUTO:
			u16Data1 |= PEARL_QD_GIGPHY_1000T_FULL | PEARL_QD_GIGPHY_1000T_HALF;
			break;
		case PEARL_SPEED_AUTO_DUPLEX_FULL:
			u16Data |= PEARL_QD_PHY_MODE_AUTO_FULL;
			u16Data1 |= PEARL_QD_GIGPHY_1000T_FULL;
			break;
		case PEARL_SPEED_1000_DUPLEX_FULL:
			u16Data1 |= PEARL_QD_GIGPHY_1000T_FULL;
			break;
		case PEARL_SPEED_1000_DUPLEX_HALF:
			u16Data1 |= PEARL_QD_GIGPHY_1000T_HALF;
			break;
		case PEARL_SPEED_AUTO_DUPLEX_HALF:
			u16Data |= PEARL_QD_PHY_MODE_AUTO_HALF;
			u16Data1 |= PEARL_QD_GIGPHY_1000T_HALF;
			break;
		case PEARL_SPEED_100_DUPLEX_AUTO:
			u16Data |= PEARL_QD_PHY_MODE_100_AUTO;
			break;
		case PEARL_SPEED_10_DUPLEX_AUTO:
			u16Data |= PEARL_QD_PHY_MODE_10_AUTO;
			break;
		case PEARL_SPEED_100_DUPLEX_FULL:
			u16Data |= PEARL_QD_PHY_100_FULL;
			break;
		case PEARL_SPEED_100_DUPLEX_HALF:
			u16Data |= PEARL_QD_PHY_100_HALF;
			break;
		case PEARL_SPEED_10_DUPLEX_FULL:
			u16Data |= PEARL_QD_PHY_10_FULL;
			break;
		case PEARL_SPEED_10_DUPLEX_HALF:
			u16Data |= PEARL_QD_PHY_10_HALF;
			break;
		default:
            return MSD_BAD_PARAM;
	}

	/* Write to Phy AutoNegotiation Advertisement Register.  */
	if (Pearl_msdWritePagedPhyReg(dev, hwPort, 0, PEARL_QD_PHY_AUTONEGO_AD_REG, u16Data) != MSD_OK)
	{
        return MSD_FAIL;
	}

	/* Write to Phy AutoNegotiation 1000B Advertisement Register.  */
	if (Pearl_msdWritePagedPhyReg(dev, hwPort, 0, PEARL_QD_PHY_AUTONEGO_1000AD_REG, u16Data1) != MSD_OK)
	{
        return MSD_FAIL;
	}

	return MSD_OK;
}

/*
* This routine sets Auto Mode and Reset the phy
*/
static
MSD_STATUS Pearl_phySetAutoMode
(
	IN MSD_QD_DEV *dev,
	IN MSD_U8 hwPort,
	IN PEARL_MSD_PHY_AUTO_MODE mode
)
{
	MSD_U16         u16Data;
	MSD_STATUS    status;

	if ((status = Pearl_gigCopperSetAutoMode(dev, hwPort, mode)) != MSD_OK)
	{
		return status;
	}

	/* Read to Phy Control Register.  */
	if (Pearl_msdReadPagedPhyReg(dev, hwPort, 0, PEARL_QD_PHY_CONTROL_REG, &u16Data) != MSD_OK)
		return MSD_FAIL;

	u16Data |= PEARL_QD_PHY_AUTONEGO;

	/* Write to Phy Control Register.  */
	if (Pearl_msdWritePagedPhyReg(dev, hwPort, 0, PEARL_QD_PHY_CONTROL_REG, u16Data) != MSD_OK)
		return MSD_FAIL;

	return Pearl_msdPhyReset(dev, hwPort, 0, 0xFF);
}


/*******************************************************************************
* Pearl_gphyReset
*
* DESCRIPTION:
*       This routine preforms PHY reset.
*       After reset, phy will be in Autonegotiation mode.
*
* INPUTS:
* 		port - The logical port number, unless SERDES device is accessed
*        	   The physical address, if SERDES device is accessed
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
* 		data sheet register 0.15 - Reset
* 		data sheet register 0.13 - Speed(LSB)
* 		data sheet register 0.12 - Autonegotiation
* 		data sheet register 0.8  - Duplex Mode
*		data sheet register 0.6  - Speed(MSB)
* 
*       If DUT is in power down or loopback mode, port will link down, 
*       in this case, no need to do software reset to restart AN.
*       When port recover from link down, AN will restart automatically.
*
*******************************************************************************/
MSD_STATUS Pearl_gphyReset
(
    IN MSD_QD_DEV *dev,
    IN MSD_LPORT  port
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U16  pageReg;

    MSD_DBG_INFO(("Pearl_gphyReset Called.\n"));
    
    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PHY(port);
	if (hwPort == MSD_INVALID_PHY)
	{
        MSD_DBG_ERROR(("Bad Port: %u.\n", (unsigned int)port));
		return MSD_BAD_PARAM;
	}
	
    msdSemTake(dev->devNum, dev->phyRegsSem,OS_WAIT_FOREVER);
	if ((retVal = Pearl_msdGetSMIPhyRegField(dev, hwPort, 22, 0, 8, &pageReg)) != MSD_OK)
    {
        MSD_DBG_ERROR(("read Phy Page Register return %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}

    /* set Auto Negotiation AD Register */
    retVal = Pearl_phySetAutoMode(dev, hwPort, PEARL_SPEED_AUTO_DUPLEX_AUTO);
    if(retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Pearl_phySetAutoMode PEARL_SPEED_AUTO_DUPLEX_AUTO returned: %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
    }

	if ((retVal = Pearl_msdSetSMIPhyRegField(dev, hwPort, 22, 0, 8, pageReg)) != MSD_OK)
    {
        MSD_DBG_ERROR(("write back Phy Page Register %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}

    msdSemGive(dev->devNum, dev->phyRegsSem);

	MSD_DBG_INFO(("Pearl_gphyReset Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Pearl_serdesReset
*
* DESCRIPTION:
*       This routine preforms internal serdes reset.
*       different devcie have different register location for the reset bit
*
* INPUTS:
*       devNum  - physical devie number
* 		portNum - The logical PHY port number
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
*       Pearl:   Port0 and Port1, Page1 Register0 (two serdes, port0 and port1, phyAddr is 0xc and 0xd)
*
*******************************************************************************/
MSD_STATUS Pearl_serdesReset
(
    IN MSD_QD_DEV *dev,
    IN MSD_LPORT  port
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16  data;

    MSD_DBG_INFO(("Pearl_serdesReset Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_SERDES(port);
    if (hwPort == MSD_INVALID_PHY)
    {
        MSD_DBG_ERROR(("Bad Port: %u.\n", (unsigned int)port));
        return MSD_BAD_PARAM;
    }

    msdSemTake(dev->devNum, dev->phyRegsSem, OS_WAIT_FOREVER);
    if ((retVal = Pearl_msdReadPagedPhyReg(dev, hwPort, 1, 0, &data)) != MSD_OK)
    {
        MSD_DBG_ERROR(("read 1000BASE-X/SGMI Control register error %s.\n", msdDisplayStatus(retVal)));
        msdSemGive(dev->devNum, dev->phyRegsSem);
        return retVal;
    }

    data = data | 0x8000;

    if ((retVal = Pearl_msdWritePagedPhyReg(dev, hwPort, 1, 0, data)) != MSD_OK)
    {
        MSD_DBG_ERROR(("write 1000BASE-X/SGMI Control register error %s.\n", msdDisplayStatus(retVal)));
        msdSemGive(dev->devNum, dev->phyRegsSem);
        return retVal;
    }

    msdSemGive(dev->devNum, dev->phyRegsSem);

    MSD_DBG_INFO(("Pearl_serdesReset Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Pearl_gphySetPortLoopback
*
* DESCRIPTION:
*       Enable/Disable Internal Port Loopback. 
*       For 10/100 Fast Ethernet PHY, speed of Loopback is determined as follows:
*         If Auto-Negotiation is enabled, this routine disables Auto-Negotiation and 
*         forces speed to be 10Mbps.
*         If Auto-Negotiation is disabled, the forced speed is used.
*         Disabling Loopback simply clears bit 14 of control register(0.14). Therefore,
*         it is recommended to call gprtSetPortAutoMode for PHY configuration after 
*         Loopback test.
*       For 10/100/1000 Gigagbit Ethernet PHY, speed of Loopback is determined as follows:
*         If Auto-Negotiation is enabled and Link is active, the current speed is used.
*         If Auto-Negotiation is disabled, the forced speed is used.
*         All other cases, default MAC Interface speed is used. Please refer to the data
*         sheet for the information of the default MAC Interface speed.
*   
*
* INPUTS:
*       port - The logical port number, unless SERDES device is accessed
*              The physical address, if SERDES device is accessed
*       enable - If MSD_TRUE, enable loopback mode
*       		 If MSD_FALSE, disable loopback mode
*
* OUTPUTS:
* 		  None.
*
* RETURNS:
* 		MSD_OK - on success
* 		MSD_FAIL - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
* 		data sheet register 0.14 - Loop_back
*
*******************************************************************************/
MSD_STATUS Pearl_gphySetPortLoopback
(
    IN MSD_QD_DEV *dev,
    IN MSD_LPORT  port,
    IN MSD_BOOL   enable
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16             u16Data;
	MSD_U16  pageReg;

    MSD_DBG_INFO(("Pearl_gphySetPortLoopback Called.\n"));
    
    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PHY(port);
	if (hwPort == MSD_INVALID_PHY)
	{
        MSD_DBG_ERROR(("Bad Port: %u.\n", (unsigned int)port));
		return MSD_BAD_PARAM;
	}
	
    msdSemTake(dev->devNum, dev->phyRegsSem,OS_WAIT_FOREVER);
	if ((retVal = Pearl_msdGetSMIPhyRegField(dev, hwPort, 22, 0, 8, &pageReg)) != MSD_OK)
    {
        MSD_DBG_ERROR(("read Phy Page Register %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}

    MSD_BOOL_2_BIT(enable,u16Data);

    /* Write to Phy Control Register.  */
	retVal = Pearl_msdSetPagedPhyRegField(dev, hwPort, 0, PEARL_QD_PHY_CONTROL_REG, 14, 1, u16Data);
    if(retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("write Phy Control Register  %s.\n", msdDisplayStatus(retVal))); 
        msdSemGive(dev->devNum, dev->phyRegsSem);
		return retVal;
    }
   
	if ((retVal = Pearl_msdSetSMIPhyRegField(dev, hwPort, 22, 0, 8, pageReg)) != MSD_OK)
    {
        MSD_DBG_ERROR(("write back Phy Page Register %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}

    msdSemGive(dev->devNum, dev->phyRegsSem);
	MSD_DBG_INFO(("Pearl_gphySetPortLoopback Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Pearl_gphySetPortSpeed
*
* DESCRIPTION:
*       Sets speed for a specific logical port. This function will keep the duplex 
*       mode and loopback mode to the previous value, but disable others, such as 
*       Autonegotiation.
*
* INPUTS:
*       port -  The logical port number, unless SERDES device is accessed
*               The physical address, if SERDES device is accessed
*       speed - port speed.
*               PEARL_PHY_SPEED_10_MBPS for 10Mbps
*               PEARL_PHY_SPEED_100_MBPS for 100Mbps
*               PEARL_PHY_SPEED_1000_MBPS for 1000Mbps
*		
* OUTPUTS:
* 		None.
*
* RETURNS:
* 		MSD_OK - on success
* 		MSD_FAIL - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
* 		data sheet register 0.13 - Speed Selection (LSB)
* 		data sheet register 0.6  - Speed Selection (MSB)
*
*       If DUT is in power down or loopback mode, port will link down, 
*       in this case, no need to do software reset to force take effect .
*       When port recover from link down, configure will take effect automatically.
*
*******************************************************************************/
MSD_STATUS Pearl_gphySetPortSpeed
(
	IN MSD_QD_DEV *dev,
	IN MSD_LPORT  port,
	IN PEARL_MSD_PHY_SPEED speed
)
{
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16             u16Data;
    MSD_STATUS        retVal;
	MSD_U16  pageReg;

    MSD_DBG_INFO(("Pearl_gphySetPortSpeed Called.\n"));
    
    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PHY(port);
	if (hwPort == MSD_INVALID_PHY)
	{
        MSD_DBG_ERROR(("Bad Port: %u.\n", (unsigned int)port));
		return MSD_BAD_PARAM;
	}
	
    msdSemTake(dev->devNum, dev->phyRegsSem,OS_WAIT_FOREVER);
	if ((retVal = Pearl_msdGetSMIPhyRegField(dev, hwPort, 22, 0, 8, &pageReg)) != MSD_OK)
    {
        MSD_DBG_ERROR(("read Phy Page Register %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}

	if (Pearl_msdReadPagedPhyReg(dev, hwPort, 0, PEARL_QD_PHY_CONTROL_REG, &u16Data) != MSD_OK)
    {

        MSD_DBG_ERROR(("read Phy Control Register %s.\n", msdDisplayStatus(retVal))); 
        msdSemGive(dev->devNum, dev->phyRegsSem);
        return MSD_FAIL;
    }

    switch(speed)
    {
    case PEARL_PHY_SPEED_10_MBPS:
            u16Data = u16Data & (PEARL_QD_PHY_LOOPBACK | PEARL_QD_PHY_DUPLEX);
            break;
    case PEARL_PHY_SPEED_100_MBPS:
            u16Data = (u16Data & (PEARL_QD_PHY_LOOPBACK | PEARL_QD_PHY_DUPLEX)) | PEARL_QD_PHY_SPEED;
            break;
    case PEARL_PHY_SPEED_1000_MBPS:
            u16Data = (u16Data & (PEARL_QD_PHY_LOOPBACK | PEARL_QD_PHY_DUPLEX)) | PEARL_QD_PHY_SPEED_MSB;
            break;
        default:
            msdSemGive(dev->devNum, dev->phyRegsSem);
            MSD_DBG_ERROR(("Failed (Bad speed).\n"));
            return MSD_BAD_PARAM;
    }

	retVal = Pearl_msdPhyReset(dev, hwPort, 0, u16Data);
	if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Pearl_msdPhyReset returned: %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}
    
	if ((retVal = Pearl_msdSetSMIPhyRegField(dev, hwPort, 22, 0, 8, pageReg)) != MSD_OK)
    {
        MSD_DBG_ERROR(("write back Phy Page Register %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}

	msdSemGive(dev->devNum, dev->phyRegsSem);
	MSD_DBG_INFO(("Pearl_gphySetPortSpeed Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Pearl_gprtPortPowerDown
*
* DESCRIPTION:
*        Enable/disable (power down) on specific logical port.
*        Phy configuration remains unchanged after Power down.
*
* INPUTS:
*       port -  The logical port number, unless SERDES device is accessed
*               The physical address, if SERDES device is accessed
*       state - MSD_TRUE: power down
*                MSD_FALSE: normal operation
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       data sheet register 0.11 - Power Down
*
*******************************************************************************/
MSD_STATUS Pearl_gphyPortPowerDown
(
	IN MSD_QD_DEV *dev,
	IN MSD_LPORT  port,
	IN MSD_BOOL   state
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          u16Data;
	MSD_U16  pageReg;

    MSD_DBG_INFO(("Pearl_gprtPortPowerDown Called.\n"));
    
    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PHY(port);
	if (hwPort == MSD_INVALID_PHY)
	{
        MSD_DBG_ERROR(("Bad Port: %u.\n", (unsigned int)port));
		return MSD_BAD_PARAM;
	}
	
    msdSemTake(dev->devNum, dev->phyRegsSem,OS_WAIT_FOREVER);
	if ((retVal = Pearl_msdGetSMIPhyRegField(dev, hwPort, 22, 0, 8, &pageReg)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Not able to read Phy Page Register.\n"));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}

    MSD_BOOL_2_BIT(state,u16Data);

	if ((retVal = Pearl_msdSetPagedPhyRegField(dev, hwPort, 0, PEARL_QD_PHY_CONTROL_REG, 11, 1, u16Data)) != MSD_OK)
    {
        MSD_DBG_ERROR(("read Phy Control Register %s.\n", msdDisplayStatus(retVal))); 
        msdSemGive(dev->devNum, dev->phyRegsSem);
        return retVal;
    }

	if ((retVal = Pearl_msdSetSMIPhyRegField(dev, hwPort, 22, 0, 8, pageReg)) != MSD_OK)
    {
        MSD_DBG_ERROR(("write back Phy Page Register %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}

    msdSemGive(dev->devNum, dev->phyRegsSem);
	MSD_DBG_INFO(("Pearl_gprtPortPowerDown Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Pearl_gphySetPortDuplexMode
*
* DESCRIPTION:
*		Sets duplex mode for a specific logical port. This function will keep 
*		the speed and loopback mode to the previous value, but disable others,
*		such as Autonegotiation.
*
* INPUTS:
*		port -  The logical port number, unless SERDES device is accessed
*                The physical address, if SERDES device is accessed
*		dMode - dulpex mode
*
* OUTPUTS:
*		None.
*
* RETURNS:
*		MSD_OK  - on success
*		MSD_FAIL - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
* 		data sheet register 0.8 - Duplex Mode
*
*       If DUT is in power down or loopback mode, port will link down, 
*       in this case, no need to do software reset to force take effect .
*       When port recover from link down, configure will take effect automatically.
*
*******************************************************************************/
MSD_STATUS Pearl_gphySetPortDuplexMode
(
	IN MSD_QD_DEV *dev,
	IN MSD_LPORT  port,
	IN MSD_BOOL   dMode
)
{
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16             u16Data;
    MSD_STATUS        retVal;
	MSD_U16  pageReg;

    MSD_DBG_INFO(("Pearl_gphySetPortDuplexMode Called.\n"));
    
    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PHY(port);
	if (hwPort == MSD_INVALID_PHY)
	{
        MSD_DBG_ERROR(("Bad Port: %u.\n", (unsigned int)port));
		return MSD_BAD_PARAM;
	}
	
    msdSemTake(dev->devNum, dev->phyRegsSem,OS_WAIT_FOREVER);
	if ((retVal = Pearl_msdGetSMIPhyRegField(dev, hwPort, 22, 0, 8, &pageReg)) != MSD_OK)
    {
        MSD_DBG_ERROR(("read Phy Page Register %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}

	if (Pearl_msdReadPagedPhyReg(dev, hwPort, 0, PEARL_QD_PHY_CONTROL_REG, &u16Data) != MSD_OK)
    {
        MSD_DBG_ERROR(("read Phy Control Register %s.\n", msdDisplayStatus(retVal)));
        msdSemGive(dev->devNum, dev->phyRegsSem);
        return MSD_FAIL;
    }

    if(dMode)
    {
        u16Data = (u16Data & (PEARL_QD_PHY_LOOPBACK | PEARL_QD_PHY_SPEED | PEARL_QD_PHY_SPEED_MSB)) | PEARL_QD_PHY_DUPLEX;
    }
    else
    {
        u16Data = u16Data & (PEARL_QD_PHY_LOOPBACK | PEARL_QD_PHY_SPEED | PEARL_QD_PHY_SPEED_MSB);
    }

    /* Write to Phy Control Register.  */
	retVal = Pearl_msdPhyReset(dev, hwPort, 0, u16Data);
	if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Pearl_msdPhyReset returned: %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}

	if ((retVal = Pearl_msdSetSMIPhyRegField(dev, hwPort, 22, 0, 8, pageReg)) != MSD_OK)
    {
        MSD_DBG_ERROR(("write back Phy Page Register %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}
    msdSemGive(dev->devNum, dev->phyRegsSem);
	MSD_DBG_INFO(("Pearl_gphySetPortDuplexMode Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Pearl_gphySetPortAutoMode
*
* DESCRIPTION:
*		This routine sets up the port with given Auto Mode.
*		Supported mode is as follows:
*        - Auto for both speed and duplex.
*        - Auto for speed only and Full duplex.
*        - Auto for speed only and Half duplex.
*        - Auto for duplex only and speed 1000Mbps.
*        - Auto for duplex only and speed 100Mbps.
*        - Auto for duplex only and speed 10Mbps.
*        - Speed 1000Mbps and Full duplex.
*        - Speed 1000Mbps and Half duplex.
*        - Speed 100Mbps and Full duplex.
*        - Speed 100Mbps and Half duplex.
*        - Speed 10Mbps and Full duplex.
*        - Speed 10Mbps and Half duplex.
*        
*
* INPUTS:
*		port -	The logical port number, unless SERDES device is accessed
*               The physical address, if SERDES device is accessed
*		mode - 	Auto Mode to be written
*
* OUTPUTS:
*		None.
*
* RETURNS:
*		MSD_OK   - on success
*		MSD_FAIL - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		data sheet register 4.6, 4.5 Autonegotiation Advertisement for 10BT
*		data sheet register 4.8, 4.7 Autonegotiation Advertisement for 100BT
*		data sheet register 9.9, 9.8 Autonegotiation Advertisement for 1000BT
*
*       If DUT is in power down or loopback mode, port will link down, 
*       in this case, no need to do software reset to restart AN.
*       When port recover from link down, AN will restart automatically.
*
*******************************************************************************/
MSD_STATUS Pearl_gphySetPortAutoMode
(
    IN MSD_QD_DEV *dev,
    IN MSD_LPORT  port,
    IN PEARL_MSD_PHY_AUTO_MODE mode
)
{

    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U16  pageReg;
	
    MSD_DBG_INFO(("Pearl_gphySetPortAutoMode Called.\n"));
    
    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PHY(port);
	if (hwPort == MSD_INVALID_PHY)
	{
        MSD_DBG_ERROR(("Bad Port: %u.\n", (unsigned int)port));
		return MSD_BAD_PARAM;
	}

    msdSemTake(dev->devNum, dev->phyRegsSem,OS_WAIT_FOREVER);
	if ((retVal = Pearl_msdGetSMIPhyRegField(dev, hwPort, 22, 0, 8, &pageReg)) != MSD_OK)
    {
        MSD_DBG_ERROR(("read Phy Page Register %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}

	retVal = Pearl_phySetAutoMode(dev, hwPort, mode);
	if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Pearl_phySetAutoMode PEARL_SPEED_AUTO_DUPLEX_AUTO returned: %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}

	if ((retVal = Pearl_msdSetSMIPhyRegField(dev, hwPort, 22, 0, 8, pageReg)) != MSD_OK)
    {
        MSD_DBG_ERROR(("write back Phy Page Register %s.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->phyRegsSem);
		return retVal;
	}
	
    msdSemGive(dev->devNum, dev->phyRegsSem);
	MSD_DBG_INFO(("Pearl_gphySetPortAutoMode Exit.\n"));
    return retVal;

}

MSD_STATUS Pearl_gphySetEEE
(
    IN MSD_QD_DEV *dev,
    MSD_LPORT portNum,
    MSD_BOOL en
)
{
    MSD_STATUS retVal;
    MSD_U16 data;
    MSD_U8 hwPort;

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PHY(portNum);
    if (hwPort == MSD_INVALID_PHY)
    {
        MSD_DBG_ERROR(("Bad Port: %u.\n", (unsigned int)portNum));
        return MSD_BAD_PARAM;
    }
    switch (en)
    {
    case MSD_TRUE:
        data = 0x06;
        break;
    case MSD_FALSE:
        data = 0;
        break;
    default:
        data = 0x06;;
        break;
    }

    msdSemTake(dev->devNum, dev->phyRegsSem, OS_WAIT_FOREVER);

    /*enable/Disable the EEE for 100Base-T and 1000Base-T*/
    retVal = Pearl_msdSetSMIPhyXMDIOReg(dev, (MSD_U8)hwPort, 7, 60, data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("enable/Disable the EEE for 100Base-T and 1000Base-T.\n", msdDisplayStatus(retVal)));
        msdSemGive(dev->devNum, dev->phyRegsSem);
        return retVal;
    }
    /*Software reset*/
    retVal = Pearl_msdReadPagedPhyReg(dev, (MSD_U8)hwPort, 0, 0, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("get status.\n", msdDisplayStatus(retVal)));
        msdSemGive(dev->devNum, dev->phyRegsSem);
        return retVal;
    }
    data = data | 0x8000;
    retVal = Pearl_msdWritePagedPhyReg(dev, (MSD_U8)hwPort, 0, 0, data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Software reset.\n", msdDisplayStatus(retVal)));
        msdSemGive(dev->devNum, dev->phyRegsSem);
        return retVal;
    }

    msdSemGive(dev->devNum, dev->phyRegsSem);
    return retVal;
}
#endif /*#if FEATURE_MARVELL_6321*/
