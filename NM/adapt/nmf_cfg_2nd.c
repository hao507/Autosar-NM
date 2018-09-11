/****************************************************************************
** Header Files (Only those that are needed in this file)
****************************************************************************/

/* System Header Files */

/* Own headerfiles */
#include "nmf__internal.h"

/* Foreign headerfiles */

#include "NMF1C1.h"

/***************************************************************************
* GLOBAL DEFINITIONS:
****************************************************************************/


/*The ID of the NM Tx message */
const uint16 NMF__u16MessageID = (0x0660u) ;

/*The DLC of the NM Tx message */
const uint8 NMF__u8MessageDLC = (0x8u) ;

/***************************************************************************
* FILE LOCAL DEFINITIONS
****************************************************************************/

/** Notify that a network request was done 
*/
void NMF__vNetworkReqCB(void)
{
	/* Fill if possible. */
}

/** Notify that a network release was done 
*/
void NMF__vNetworkRelCB(void)
{
	/* Fill if possible */
}

/* Notify that Passive Startup was requested
*/
void NMF__vReqPassiveCB(void)
{

}

/* Notify that Passive Startup was released
*/
void NMF__vRelPassiveCB(void)
{

}

/* Notify that a wakeup occured 
*/
void NMF__vWakeupCB(void)
{

}

/* Notify that the transition from BusSleep To RepeatMessage will be performed 
*/
void NMF__vBusSleepToRepeatMessageCB(void)
{
	/*** Enable App Message to transmit ***/
}

/* Notify that the transition from RepeatMessage To NormalOperation will be performed 
*/
void NMF__vRepeatMessageToNormalOperationCB(void)
{

}

/* Notify that the transition from NormalOperation To RepeatMessage will be performed 
*/
void NMF__vNormalOperationToRepeatMessageCB(void)
{

}

/* Notify that the transition from RepeatMessage To ReadySleep will be performed
*/
void NMF__vRepeatMessageToReadySleepCB(void)
{

}

/* Notify that the transition from ReadySleep To RepeatMessage will be performed 
*/
void NMF__vReadySleepToRepeatMessageCB(void)
{

}

/* Notify that the transition from ReadySleep To NormalOperation will be performed 
*/
void NMF__vReadySleepToNormalOperationCB(void)
{

}

/* Notify that the transition from NormalOperation To ReadySleep will be performed 
*/
void NMF__vNormalOperationToReadySleepCB(void)
{

}

/* Notify that the transition from ReadySleep To PrepareBusSleep will be performed
*/
void NMF__vReadySleepToPrepareBusSleepCB(void)
{
	/*** Disable App message to transmit ***/
}

/* Notify that the transition from PrepareBusSleep To BusSleep will be performed 
*/
void NMF__vPrepareBusSleepToBusSleepCB(void)
{

}

/* Notify that the transition from PrepareBusSleep To RepeatMessage will be performed 
*/
void NMF__vPrepareBusSleepToRepeatMessageCB(void)
{
	NMF_vSetPartOnline(NMF_ALL_GROUPS);
//	IL_CyclicTxEnableIL();
}

/* Notify about Tx Timeout
*/
void NMF__vTxTimeout(void)
{

}

/* Notify about NM Timeout 
*/
void NMF__vNMTimeout(void)
{

}

/* The Diagnostic should be started
*/
void NMF__vStartDiagCB(void)
{

}

/* The Diagnostic should be stopped
*/
void NMF__vStopDiagCB(void)
{

}

/**
* Set all PDUs on the NMF network to a specific mode.
*/
void NMF__vSetAllGroups(NMF_tenMsgGtoupSts enGroupSts)
{    
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	
	
		
}


/**
* Set Application PDUs on the NMF network to a specific mode.
*/
void NMF__vSetApplicationGroups(NMF_tenMsgGtoupSts enGroupSts)
{    
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	
	
		
}

/**
* Set NMF's PDUs on the NMF network to a specific mode.
*/
void NMF__vSetNmfGroups(NMF_tenMsgGtoupSts enGroupSts)
{    
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	
	
		
}

/**
* Enable Timeout monitoring for all IL PDUs.
*/
void NMF__vEnableILTimeouts(void)
{    
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	
}

/**
* Disable Timeout monitoring for all IL PDUs.
*/
void NMF__vDisableILTimeouts(void)
{    
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE 
	******************************************************************************/
	
}


/**
* Bus Off has been detected.
*/
void NMF__vBusOffEntryCB(void)
{
	
}

/**
* Bus Off has been recovered.
*/
void NMF__vBusOffExitCB(void)
{
	
}


