#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/********************************************************************************
* msdLed.c
*
* DESCRIPTION:
*       API definitions for LED control
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "msdLed.h"
#include "msdApiTypes.h"
#include "msdUtils.h"

/*******************************************************************************
* msdLedForceOn
*
* DESCRIPTION:
*       This routine is to Force Port LED On
*
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       ledNum  - LED Index 
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
*       For Peridot and Topaz: 
*       If IMP boots, all LEDs are deactive. It's the IMP software's responsibility 
*       to determine when to turn on the LEDs by clearing these bits (Global LED Control,
*       index 0x00, bit[3:0], Skip Columns) to zero. Then we can call this API to 
*       configure LED mode.
*
*******************************************************************************/
MSD_STATUS msdLedForceOn
(
IN  MSD_U8  devNum,
IN  MSD_LPORT portNum,
IN  MSD_U8    ledNum
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }

    if (dev->SwitchDevObj.LEDObj.gledForceOn != NULL)
        return dev->SwitchDevObj.LEDObj.gledForceOn(dev, portNum, ledNum);
    else
        return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdLedForceOff
*
* DESCRIPTION:
*       This routine is to Force Port LED Off
*
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       ledNum  - LED Index
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
*       For Peridot and Topaz:
*       If IMP boots, all LEDs are deactive. It's the IMP software's responsibility
*       to determine when to turn on the LEDs by clearing these bits (Global LED Control,
*       index 0x00, bit[3:0], Skip Columns) to zero. Then we can call this API to
*       configure LED mode.
*
*******************************************************************************/
MSD_STATUS msdLedForceOff
(
IN  MSD_U8  devNum,
IN  MSD_LPORT portNum,
IN  MSD_U8    ledNum
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }

    if (dev->SwitchDevObj.LEDObj.gledForceOff != NULL)
        return dev->SwitchDevObj.LEDObj.gledForceOff(dev, portNum, ledNum);
    else
        return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdLedLinkAct
*
* DESCRIPTION:
*       This routine is to set Port LED mode to link/act. That is:
*           off - no link, on - link, blink = activity.
*
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       ledNum  - LED Index
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
*       For Peridot and Topaz:
*       If IMP boots, all LEDs are deactive. It's the IMP software's responsibility
*       to determine when to turn on the LEDs by clearing these bits (Global LED Control,
*       index 0x00, bit[3:0], Skip Columns) to zero. Then we can call this API to
*       configure LED mode.
*
*******************************************************************************/
MSD_STATUS msdLedLinkAct
(
IN  MSD_U8  devNum,
IN  MSD_LPORT portNum,
IN  MSD_U8    ledNum
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }

    if (dev->SwitchDevObj.LEDObj.gledLinkAct != NULL)
        return dev->SwitchDevObj.LEDObj.gledLinkAct(dev, portNum, ledNum);
    else
        return MSD_NOT_SUPPORTED;
}


/*******************************************************************************
* msdLedModeSet
*
* DESCRIPTION:
*       This routine is to set led mode
*		For Peridot :
*		Port 1 - 8 :
*			0000 :  Link/Act/Speed by Blink Rate (off = no link, on = link, blink = activity, blink speed = link speed)
*			0001 :  100/Gig Link/Act (off = no link, on = 100 or Gig link, blink = activity)
*			0010 :  Gig Link/Act (off = no link, on = Gig link, blink = activity)
*			0010 :  Link/Act (off = no link, on = link, blink = activity)
*			0100 :  Special LED 3
*			0101 :  Special LED 4
*			0110 :  Duplex/Collision (off = half duplex, on = full duplex, blink = col)
*			0111 :  10/Gig Link/Act (off = no link, on = 10 or Gig link, blink = activity)
*			1000 :  Link (off = no link, on = link)
*			1001 :  10 Link (off = no link, on = 10 link)
*			1010 :  10 Link/Act (off = no link, on = 10 link, blink = activity)
*			1011 :  100/Gig Link (off = no link, on = 100 or Gig link)
*			1100 :  PTP Act (blink on = PTP activity)
*			1101 :  Force Blink
*			1110 :  Force Off
*			1111 :  Force On
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       ledNum  - LED Index
*       mode  - LED mode
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
*       For Peridot and Topaz:
*       If IMP boots, all LEDs are deactive. It's the IMP software's responsibility
*       to determine when to turn on the LEDs by clearing these bits (Global LED Control,
*       index 0x00, bit[3:0], Skip Columns) to zero. Then we can call this API to
*       configure LED mode.
*
*******************************************************************************/
MSD_STATUS msdLedModeSet
(
IN  MSD_U8  devNum,
IN  MSD_LPORT portNum,
IN  MSD_U8    ledNum,
IN  MSD_U8    mode
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.LEDObj.gledModeSet != NULL)
		return dev->SwitchDevObj.LEDObj.gledModeSet(dev, portNum, ledNum, mode);
	else
		return MSD_NOT_SUPPORTED;
}
/*******************************************************************************
* msdLedModeGet
*
* DESCRIPTION:
*       This routine is to get led mode
*		For Peridot :
*		LED mode (Port 1 - 8) :
*			0000 :  Link/Act/Speed by Blink Rate (off = no link, on = link, blink = activity, blink speed = link speed)
*			0001 :  100/Gig Link/Act (off = no link, on = 100 or Gig link, blink = activity)
*			0010 :  Gig Link/Act (off = no link, on = Gig link, blink = activity)
*			0010 :  Link/Act (off = no link, on = link, blink = activity)
*			0100 :  Special LED 3
*			0101 :  Special LED 4
*			0110 :  Duplex/Collision (off = half duplex, on = full duplex, blink = col)
*			0111 :  10/Gig Link/Act (off = no link, on = 10 or Gig link, blink = activity)
*			1000 :  Link (off = no link, on = link)
*			1001 :  10 Link (off = no link, on = 10 link)
*			1010 :  10 Link/Act (off = no link, on = 10 link, blink = activity)
*			1011 :  100/Gig Link (off = no link, on = 100 or Gig link)
*			1100 :  PTP Act (blink on = PTP activity)
*			1101 :  Force Blink
*			1110 :  Force Off
*			1111 :  Force On
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       ledNum  - LED Index
*
* OUTPUTS:
*       mode - Led mode.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       For Peridot and Topaz:
*       If IMP boots, all LEDs are deactive. It's the IMP software's responsibility
*       to determine when to turn on the LEDs by clearing these bits (Global LED Control,
*       index 0x00, bit[3:0], Skip Columns) to zero. Then we can call this API to
*       configure LED mode.
*
*******************************************************************************/
MSD_STATUS msdLedModeGet
(
IN  MSD_U8  devNum,
IN  MSD_LPORT portNum,
IN  MSD_U8    ledNum,
OUT  MSD_U8    *mode
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.LEDObj.gledModeGet != NULL)
		return dev->SwitchDevObj.LEDObj.gledModeGet(dev, portNum, ledNum,mode);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdLedBlinkRateSet
*
* DESCRIPTION:
*       This routine is to set led blink rate
*
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       rate  - LED blink rate
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
*       For Peridot and Topaz:
*       If IMP boots, all LEDs are deactive. It's the IMP software's responsibility
*       to determine when to turn on the LEDs by clearing these bits (Global LED Control,
*       index 0x00, bit[3:0], Skip Columns) to zero. Then we can call this API to
*       configure LED mode.
*
*******************************************************************************/
MSD_STATUS msdLedBlinkRateSet
(
IN  MSD_U8  devNum,
IN  MSD_LPORT portNum,
IN  MSD_LED_BLINK_RATE    rate
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.LEDObj.gledBlinkRateSet != NULL)
		return dev->SwitchDevObj.LEDObj.gledBlinkRateSet(dev, portNum, rate);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdLedBlinkRateGet
*
* DESCRIPTION:
*       This routine is to get led blink rate
*
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*
* OUTPUTS:
*       rate  - LED blink rate
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       For Peridot and Topaz:
*       If IMP boots, all LEDs are deactive. It's the IMP software's responsibility
*       to determine when to turn on the LEDs by clearing these bits (Global LED Control,
*       index 0x00, bit[3:0], Skip Columns) to zero. Then we can call this API to
*       configure LED mode.
*
*******************************************************************************/
MSD_STATUS msdLedBlinkRateGet
(
IN  MSD_U8  devNum,
IN  MSD_LPORT portNum,
OUT  MSD_LED_BLINK_RATE    *rate
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.LEDObj.gledBlinkRateGet != NULL)
		return dev->SwitchDevObj.LEDObj.gledBlinkRateGet(dev, portNum, rate);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdLedPulseStretchSet
*
* DESCRIPTION:
*       This routine is to set led stretch duration
*
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       duration  - LED stretch duration
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
*       For Peridot and Topaz:
*       If IMP boots, all LEDs are deactive. It's the IMP software's responsibility
*       to determine when to turn on the LEDs by clearing these bits (Global LED Control,
*       index 0x00, bit[3:0], Skip Columns) to zero. Then we can call this API to
*       configure LED mode.
*
*******************************************************************************/
MSD_STATUS msdLedPulseStretchSet
(
IN  MSD_U8  devNum,
IN  MSD_LPORT portNum,
IN  MSD_LED_STRETCH_DURATION duration
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.LEDObj.gledPulseStretchSet != NULL)
		return dev->SwitchDevObj.LEDObj.gledPulseStretchSet(dev, portNum, duration);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdLedPulseStretchGet
*
* DESCRIPTION:
*       This routine is to get led stretch duration
*
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*
* OUTPUTS:
*       duration  - LED stretch duration
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       For Peridot and Topaz:
*       If IMP boots, all LEDs are deactive. It's the IMP software's responsibility
*       to determine when to turn on the LEDs by clearing these bits (Global LED Control,
*       index 0x00, bit[3:0], Skip Columns) to zero. Then we can call this API to
*       configure LED mode.
*
*******************************************************************************/
MSD_STATUS msdLedPulseStretchGet
(
IN  MSD_U8  devNum,
IN  MSD_LPORT portNum,
OUT  MSD_LED_STRETCH_DURATION    *duration
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.LEDObj.gledPulseStretchGet != NULL)
		return dev->SwitchDevObj.LEDObj.gledPulseStretchGet(dev, portNum, duration);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdLedSpecialModeSet
*
* DESCRIPTION:
*       This routine is to set Special led portvect
*
*
* INPUTS:
*       devNum  - physical devie number
*       mode  - Special led number
*		portVect - Special led bit mask
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
*       For Peridot and Topaz:
*       If IMP boots, all LEDs are deactive. It's the IMP software's responsibility
*       to determine when to turn on the LEDs by clearing these bits (Global LED Control,
*       index 0x00, bit[3:0], Skip Columns) to zero. Then we can call this API to
*       configure LED mode.
*
*******************************************************************************/
MSD_STATUS msdLedSpecialModeSet
(
IN  MSD_U8  devNum,
IN  MSD_LED_SPECIAL_MODE	mode,
IN  MSD_U16  portVect
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.LEDObj.gledSpecialModeSet != NULL)
		return dev->SwitchDevObj.LEDObj.gledSpecialModeSet(dev, mode, portVect);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdLedSpecialModeGet
*
* DESCRIPTION:
*       This routine is to get Special led portvect
*
*
* INPUTS:
*       devNum  - physical devie number
*       mode  - Special led number
*
* OUTPUTS:
*		portVect - Special led bit mask
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       For Peridot and Topaz:
*       If IMP boots, all LEDs are deactive. It's the IMP software's responsibility
*       to determine when to turn on the LEDs by clearing these bits (Global LED Control,
*       index 0x00, bit[3:0], Skip Columns) to zero. Then we can call this API to
*       configure LED mode.
*
*******************************************************************************/
MSD_STATUS msdLedSpecialModeGet
(
IN  MSD_U8  devNum,
IN  MSD_LED_SPECIAL_MODE	mode,
OUT  MSD_U16	*portVect
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.LEDObj.gledSpecialModeGet != NULL)
		return dev->SwitchDevObj.LEDObj.gledSpecialModeGet(dev, mode, portVect);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdLedCtrlSet
*
* DESCRIPTION:
*       This routine is to set led control register
*
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       pointer - LED register pointer
*       data  - LED register data
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
*       For Peridot and Topaz:
*       If IMP boots, all LEDs are deactive. It's the IMP software's responsibility
*       to determine when to turn on the LEDs by clearing these bits (Global LED Control,
*       index 0x00, bit[3:0], Skip Columns) to zero. Then we can call this API to
*       configure LED mode.
*
*******************************************************************************/
MSD_STATUS msdLedCtrlSet
(
IN  MSD_U8  devNum,
IN  MSD_LPORT portNum,
IN  MSD_U8 	pointer,
IN  MSD_U16	data
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.LEDObj.gledCtrlSet != NULL)
		return dev->SwitchDevObj.LEDObj.gledCtrlSet(dev, portNum, pointer, data);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdLedCtrlGet
*
* DESCRIPTION:
*       This routine is to get led control register
*
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       pointer - LED register pointer
*
* OUTPUTS:
*       data  - LED register data
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       For Peridot and Topaz:
*       If IMP boots, all LEDs are deactive. It's the IMP software's responsibility
*       to determine when to turn on the LEDs by clearing these bits (Global LED Control,
*       index 0x00, bit[3:0], Skip Columns) to zero. Then we can call this API to
*       configure LED mode.
*
*******************************************************************************/
MSD_STATUS msdLedCtrlGet
(
IN  MSD_U8  devNum,
IN  MSD_LPORT portNum,
IN  MSD_U8 	pointer,
OUT MSD_U16	*data
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.LEDObj.gledCtrlGet != NULL)
		return dev->SwitchDevObj.LEDObj.gledCtrlGet(dev, portNum, pointer, data);
	else
		return MSD_NOT_SUPPORTED;
}
#endif /*#if FEATURE_MARVELL_6321*/
