#define NMF__Internal_C
/***************************************************************************
* HEADER-FILES (Only those that are needed in this file)
****************************************************************************/

/* System-headerfiles */
#include "stdio.h"
/* Foreign headerfiles */

/* Own headerfiles */
#include "nmf_cfg_1st.h"
#include "nmf__internal.h"

#include "nmf1ct.h"
/***************************************************************************
* GLOBAL DEFINITIONS
****************************************************************************/
/**
* NMF current state
*/
NMF_tenStates NMF__enCurrentState = NMF_nenUnInitialized;

/**
* Contains the status of network request
*/ 
boolean NMF__boNetRequest = False;

/**
* Contains the status of passive startup request
*/ 
boolean NMF__boPassiveStartup = False;

/**
* Contains the status of RepeatMessage request
*/ 
boolean NMF__boRepeatMessageReq = False;

/**
* Contains the status of received NM PDU
*/ 
boolean NMF__boReceivedNMPdu = False;

/**
* Contains the status of repeat message bit
*/ 
boolean NMF__boRepeatMsgBit = False;

/**
* Contains the status of Tx confirmation
*/ 
boolean NMF__boTxConfNMPdu = False;

/**
* Contains the BusOff flag
*/ 
boolean NMF__boBusOffFlag = False;

/**
* Counter for BusOff events
*/ 
uint16 NMF__u16BusOffEvents = 0;


/**
* Contains the counters associated to each NM timer
*/ 
uint16 NMF__au16Counter[NMF__nenLastTimer];

/**
* BusOff counter
*/ 
uint16 NMF__u16BusOffCounter = 0;

/**
* Counter for acknowledge frame since wakeup
*/ 
uint16 NMF__u16AckWKCounter = 0;

/**
* Buffer for transmission
*/
uint8 NMF__au8TxBuffer[NMF_BUFFER_SIZE];

/**
* Buffer for reception
*/
uint8 NMF__au8RxBuffer[NMF_BUFFER_SIZE];

/**
* Contains the Power Fail faze status
*/ 
uint8 NMF__u8PowerFailStatus = 0;

/**
* Counter for Tdiag_start
*/ 
uint16 NMF__u16TdiagCounter = 0;

/**
* Indicates for NM RxTx enable
*/ 
extern boolean NMF__boEnableRxTx;

/***************************************************************************
* FILE LOCAL DEFINITIONS
****************************************************************************/

/*Shall now be defined in corresponding header-file, otherwise they are not 
  visible in design */

/******************************************************************************
* FILE LOCAL FUNCTION PROTOTYPES
******************************************************************************/

/*Shall now be defined in corresponding header-file, otherwise they are not 
  visible in design */

void NMF__vSetAllGroups(NMF_tenMsgGtoupSts enGroupSts);

void NMF__vSetApplicationGroups(NMF_tenMsgGtoupSts enGroupSts);

void NMF__vSetNmfGroups(NMF_tenMsgGtoupSts enGroupSts);

/******************************************************************************
* FUNCTION DEFINITIONS
******************************************************************************/
   /*File local scope*/


   /*Package global scope*/

/**
* Transmission function
*/
void NMF__vTransmit(const uint8 *pu8Data)
{
	CANIF_tstPDUType	stPDU;
	uint8 i = 0;

	if(True == NMF__boEnableRxTx)
	{
		/* fill the PDU structure */
		stPDU.u16Id = NMF__nMessageID;
		stPDU.u8Dlc = NMF__nMessageDLC;
		
		stPDU.pu8Data = (uint8*) &pu8Data[0];
		
		/* clear confirmation flag */
		//NMF__boTxConfNMPdu = False;

		/* transmit the NM message */
		printf("[NM - Tx]:");
		for (i = 0; i < NMF_BUFFER_SIZE; i++)
		{
			printf(" %d", stPDU.pu8Data[i]);
		}
		printf("\n");
	}
}

/**
* Transition from BusSleep To RepeatMessage
*/	
void NMF__vBusSleepToRepeatMessage(void)
{
	/* the current state switchs from BusSleep to RepeatMessage state */
	NMF__enCurrentState = NMF_nenRepeatMessage;
	/* load the counter associated to NM-Timeout Timer */
	NMF__au16Counter[NMF__nenTNMTimeout] = NMF_TNM_TIMEOUT;
	/* load the counter associated to RepeatMessage Timer */
	NMF__au16Counter[NMF__nenTRepeatMessage] = NMF_TREPEAT_MESSAGE;
	
	/* set offline all channels */
	NMF_vSetPartOffline(NMF_ALL_GROUPS);
	/* set online the network management channel */
	NMF_vSetPartOnline(NMF_NETMANAGEMENT_GROUP);
	
	/* write the NM state */
	NMF__vSetNMState(NMF_REP_MSG_BUS_SLEEP_BIT);

	/* notify application about the transition 
	* the application could write the wakeup sources, NM Active sources 
	*/
	NMF__vBusSleepToRepeatMessageCB();

	/* call transmit function */
	NMF__vTransmit(&NMF__au8TxBuffer[0]);
	/* start Tx-Timeout counter */
	NMF__au16Counter[NMF__nenTTxTimeout] = NMF_TTX_TIMEOUT;
	
	/* load the counter associated to NMCycle Timer*/
	NMF__au16Counter[NMF__nenTNMCycle] = NMF_TREPEAT_NM_CYCLE;

	/* set online all channels */
	NMF_vSetPartOnline(NMF_ALL_GROUPS);
}

/**
* Transition from PrepareBusSleep To RepeatMessage
*/
void NMF__vPrepareBusSleepToRepeatMessage(void)
{
	/* the current state switchs from PrepareBusSleep to RepeatMessage state */
	NMF__enCurrentState = NMF_nenRepeatMessage;

	/* load the counter associated to NM-Timeout Timer */
	NMF__au16Counter[NMF__nenTNMTimeout] = NMF_TNM_TIMEOUT;
	/* load the counter associated to RepeatMessage Timer */
	NMF__au16Counter[NMF__nenTRepeatMessage] = NMF_TREPEAT_MESSAGE;
	
	/* set offline all channels */
	NMF_vSetPartOffline(NMF_ALL_GROUPS);
	/* set online the network management channel */
	NMF_vSetPartOnline(NMF_NETMANAGEMENT_GROUP);

	/* write the NM state */
	NMF__vSetNMState(NMF_REP_MSG_PREP_BUS_SLEEP_BIT);

	/* notify application about the transition */
	NMF__vPrepareBusSleepToRepeatMessageCB();

	/* call transmit function */
	NMF__vTransmit(&NMF__au8TxBuffer[0]);
	/* start Tx-Timeout counter */
	NMF__au16Counter[NMF__nenTTxTimeout] = NMF_TTX_TIMEOUT;
	
	/* load the counter associated to NMCycle Timer*/
	NMF__au16Counter[NMF__nenTNMCycle] = NMF_TREPEAT_NM_CYCLE;
}

/**
* Transition from RepeatMessage To NormalOperation
*/
void NMF__vRepeatMessageToNormalOperation(void)
{
	/* the current state switchs from RepeatMessage to NormalOperation state */
	NMF__enCurrentState = NMF_nenNormalOperation;
	/* load the counter associated to NMCycle Timer*/
	NMF__au16Counter[NMF__nenTNMCycle] = NMF_TNM_CYCLE;

	/* write the NM state */
	NMF__vSetNMState(NMF_NORMAL_OP_REP_MSG_BIT);

	/* notify the application about the transition */
	NMF__vRepeatMessageToNormalOperationCB();

	/* clear RepeatMessage request flag */
	NMF__boRepeatMessageReq = False;
}

/**
* Transition from NormalOperation To RepeatMessage
*/
void NMF__vNormalOperationToRepeatMessage(void)
{
	/* the current state switchs from NormalOperation to RepeatMessage state */
	NMF__enCurrentState = NMF_nenRepeatMessage;
	/* load the counter associated to NM-Timeout Timer */
	NMF__au16Counter[NMF__nenTNMTimeout] = NMF_TNM_TIMEOUT;
	/* load the counter associated to RepeatMessage Timer */
	NMF__au16Counter[NMF__nenTRepeatMessage] = NMF_TREPEAT_MESSAGE;
	
	/* check if RepeatMessage request was done */
	if(NMF__boRepeatMessageReq == True)
	{
		/* set Repeat Message Bit */
		NMF_SET_REPEAT_MESAGE_BIT(NMF__au8TxBuffer);
	}

	/* write the NM state */
	NMF__vSetNMState(NMF_REP_MSG_NORMAL_OP_BIT);

	/* notify application about the transition */
	NMF__vNormalOperationToRepeatMessageCB();

	/* call transmit function */
	NMF__vTransmit(&NMF__au8TxBuffer[0]);
	/* start Tx-Timeout counter */
	NMF__au16Counter[NMF__nenTTxTimeout] = NMF_TTX_TIMEOUT;
	
	/* load the counter associated to NMCycle Timer*/
	NMF__au16Counter[NMF__nenTNMCycle] = NMF_TREPEAT_NM_CYCLE;
}

/**
* Transition from RepeatMessage To ReadySleep
*/
void NMF__vRepeatMessageToReadySleep(void)
{
	/* the current state switchs from RepeatMessage to ReadySleep state */
	NMF__enCurrentState = NMF_nenReadySleep;
	/* in this state the NM PDUs are not transmitted */
	NMF_vSetPartOffline(NMF_NETMANAGEMENT_GROUP);
	/* stop TxTimeout counter */
	NMF__au16Counter[NMF__nenTTxTimeout] = 0;

	/* clear RepeatMessage request flag */
	NMF__boRepeatMessageReq = False;

	/* notify application about the transition */
	NMF__vRepeatMessageToReadySleepCB();
}

/**
* Transition from ReadySleep To RepeatMessage
*/
void NMF__vReadySleepToRepeatMessage(void)
{
	/* the current state switchs from ReadySleep to RepeatMessage state */
	NMF__enCurrentState = NMF_nenRepeatMessage;
	/* load the counter associated to NM-Timeout Timer */
	NMF__au16Counter[NMF__nenTNMTimeout] = NMF_TNM_TIMEOUT;
	/* load the counter associated to RepeatMessage Timer */
	NMF__au16Counter[NMF__nenTRepeatMessage] = NMF_TREPEAT_MESSAGE;
	
	/* check if RepeatMessage request was done */
	if(NMF__boRepeatMessageReq == True)
	{
		/* set Repeat Message Bit */
		NMF_SET_REPEAT_MESAGE_BIT(NMF__au8TxBuffer);
	}

	/* write the NM state */
	NMF__vSetNMState(NMF_REP_MSG_READY_SLEEP_BIT);

	/* notify application about this transition */
	NMF__vReadySleepToRepeatMessageCB();

	/* set online the network management channel */
	NMF_vSetPartOnline(NMF_NETMANAGEMENT_GROUP);

	/* call transmit function */
	NMF__vTransmit(&NMF__au8TxBuffer[0]);
	/* start Tx-Timeout counter */
	NMF__au16Counter[NMF__nenTTxTimeout] = NMF_TTX_TIMEOUT;
	
	/* load the counter associated to NMCycle Timer*/
	NMF__au16Counter[NMF__nenTNMCycle] = NMF_TREPEAT_NM_CYCLE;
}

/**
* Transition from NormalOperation To ReadySleep
*/
void NMF__vNormalOperationToReadySleep(void)
{
	/* the current state switchs from NormalOperation to ReadySleep state */
	NMF__enCurrentState = NMF_nenReadySleep;
	/* in this state the NM PDUs are not transmitted */
	NMF_vSetPartOffline(NMF_NETMANAGEMENT_GROUP);
	/* stop TxTimeout counter */
	NMF__au16Counter[NMF__nenTTxTimeout] = 0;

	/* notify application about this transition */
	NMF__vNormalOperationToReadySleepCB();
}

/**
* Transition from ReadySleep To NormalOperation
*/
void NMF__vReadySleepToNormalOperation(void)
{
	/* the current state switchs from ReadySleep to NormalOperation state */
	NMF__enCurrentState = NMF_nenNormalOperation;

	/* in this state the NM PDUs are transmitted */
	NMF_vSetPartOnline(NMF_NETMANAGEMENT_GROUP);

	/* write the NM state */
	NMF__vSetNMState(NMF_NORMAL_OP_READY_SLEEP_BIT);

	/* notify application about this transition */
	NMF__vReadySleepToNormalOperationCB();

	/* call transmit function */
	NMF__vTransmit(&NMF__au8TxBuffer[0]);
	
	/* start Tx-Timeout counter */
	NMF__au16Counter[NMF__nenTTxTimeout] = NMF_TTX_TIMEOUT;

	/* load the counter associated to NMCycle Timer*/
	NMF__au16Counter[NMF__nenTNMCycle] = NMF_TNM_CYCLE;
}

/**
* Transition from ReadySleep To PrepareBusSleep
*/
void NMF__vReadySleepToPrepareBusSleep(void)
{
	/* the current state switchs from ReadySleep to PrepareBusSleep state */
	NMF__enCurrentState = NMF_nenPrepareBusSleep;
	
	/* no activity on the bus - only the queued messages are transmitted */
	NMF_vSetPartOffline(NMF_ALL_GROUPS);

	/* start wait bus sleep counter */
	NMF__au16Counter[NMF__nenTWaitBusSleep] = NMF_TWAIT_BUS_SLEEP;

	/* callback to prepare bus sleep mode */
	NMF__vReadySleepToPrepareBusSleepCB();
}

/**
* Transition from PrepareBusSleep To BusSleep
*/
void NMF__vPrepareBusSleepToBusSleep(void)
{
	/* the current state switchs from PrepareBusSleep to BusSleep state */
	NMF__enCurrentState = NMF_nenBusSleep;
	
	/* callback to bus sleep mode */
	NMF__vPrepareBusSleepToBusSleepCB();
}

/**
* Function used to start the transmission of a specific message group
*/
void NMF_vSetPartOnline(uint8 u8Group )
{
	switch(u8Group)
	{
	case NMF_ALL_GROUPS:
		NMF__vSetAllGroups(NMF_nenOnLine);
		break;
	case NMF_APPLICANTION_GROUP:
		NMF__vSetApplicationGroups(NMF_nenOnLine);
		break;
	case NMF_NETMANAGEMENT_GROUP:
		NMF__vSetNmfGroups(NMF_nenOnLine);
		break;
	default:
		break;
	}
	
	u8Group ^= u8Group;
}

/**
* Function used to stop the transmission of a specific message group
*/
void NMF_vSetPartOffline(uint8 u8Group )
{
	switch(u8Group)
	{
	case NMF_ALL_GROUPS:
		NMF__vSetAllGroups(NMF_nenOffLine);
		break;
	case NMF_APPLICANTION_GROUP:
		NMF__vSetApplicationGroups(NMF_nenOffLine);
		break;
	case NMF_NETMANAGEMENT_GROUP:
		NMF__vSetNmfGroups(NMF_nenOffLine);
		break;
	default:
		break;
	}
	
	u8Group ^= u8Group;
}

/**
* Actions related from Normal to Under Voltage1 transition.
*/	
void NMF__vNormalToUnderVoltage1(void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	/* callback for APPL - in order to stop the diagnostic */
	NMF__vStopDiagCB();
}

/**
* Actions related from Normal to Under Voltage2 transition.
*/	
void NMF__vNormalToUnderVoltage2(void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	/* stop rx, tx */
	CANIF_enSetControllerMode(CANIF_T_OFFLINE);
	NMF__vDisableILTimeouts();
	/*CANIF_enSetControllerMode(CANIF_T_STOP);*/
}

/**
* Actions related from Under Voltage3 to Normal transition.
*/	
void NMF__vUnderVoltageToNormal3(void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	/* start rx, tx */
	/*CANIF_enSetControllerMode(CANIF_T_START);*/
	NMF__vEnableILTimeouts();
	CANIF_enSetControllerMode(CANIF_T_ONLINE);
}

/**
* Actions related from Under Voltage4 to Normal transition.
*/	
void NMF__vUnderVoltageToNormal4(void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	if(NMF__u16TdiagCounter == 0)
	{
		/* start diag counter  - +1 because the counter is decremented in the same task*/
		NMF__u16TdiagCounter = NMF_TDIAG_START + 1;
		
	}
}


/**
* Actions related from Normal to Over Voltage5 transition.
*/	
void NMF__vNormalToOverVoltage5(void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	/* callback for APPL - in order to stop the diagnostic */
	NMF__vStopDiagCB();
}

/**
* Actions related from Normal to Over Voltage6 transition.
*/	
void NMF__vNormalToOverVoltage6(void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	/* stop rx, tx */
	CANIF_enSetControllerMode(CANIF_T_OFFLINE);
	NMF__vDisableILTimeouts();
	/*CANIF_enSetControllerMode(CANIF_T_STOP);*/
}

/**
* Actions related from Over Voltage7 to Normal transition.
*/	
void NMF__vOverVoltageToNormal7(void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	/* start rx, tx */
	/*CANIF_enSetControllerMode(CANIF_T_START);*/
	NMF__vEnableILTimeouts();
	CANIF_enSetControllerMode(CANIF_T_ONLINE);
}

/**
* Actions related from Over Voltage8 to Normal transition.
*/	
void NMF__vOverVoltageNormal8(void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	if(NMF__u16TdiagCounter == 0)
	{
		/* start diag counter  - +1 because the counter is decremented in the same task*/
		NMF__u16TdiagCounter = NMF_TDIAG_START + 1;
	}
}

/**
* Set NM state value
* @param u8Value- the current NM state
* valid values: 
* 0x01 for Repeat Message State(from Bus Sleep Mode)
* 0x02 for Repeat Message State(from Prepare Bus Sleep Mode)
* 0x04 for Repeat Message State(from Normal Operation State)
* 0x08 for Repeat Message State(from Ready Sleep State)
* 0x10 for Normal Operation State(from Repeat Message State)
* 0x20 for Normal Operation State(from Ready Sleep Mode)
* 0x40 for Wakeup(from Gateway Evaluates)
* 0x80 Reserved
*/
void NMF__vSetNMState(uint8 u8Value)
{
	/* write in the tx buffer the NM State */
	NMF__au8TxBuffer[NMF_NM_STATE] = u8Value&0x7F|(NMF__au8TxBuffer[NMF_NM_STATE]&0x80);
}

