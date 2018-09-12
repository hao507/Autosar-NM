#define NMF_Interface_C
/***************************************************************************
* HEADER-FILES (Only those that are needed in this file)
****************************************************************************/

/* System-headerfiles */

/* Foreign headerfiles */

/* Own headerfiles */
#include "nmf_cfg_1st.h"
#include "nmf__internal.h"
#include "nmf_interface.h"

#include "nmf1ct.h"
/***************************************************************************
* GLOBAL DEFINITIONS
****************************************************************************/



/***************************************************************************
* FILE LOCAL DEFINITIONS
****************************************************************************/
/*Shall now be defined in corresponding header-file, otherwise they are not 
  visible in design */

extern uint8 NMF__u8PowerFailStatus;

boolean NMF__boEnableRxTx;

/* Old PowerFail status */
static	uint8 u8OldPowerFailStatus = 0;

/******************************************************************************
* FILE LOCAL FUNCTION PROTOTYPES
******************************************************************************/
/*Shall now be defined in corresponding header-file, otherwise they are not 
  visible in design */

/******************************************************************************
* FUNCTION DEFINITIONS
******************************************************************************/

/**
* Initializes the NM package, called when CAN init.
*/
void NMF_vInit (void)
{    
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/
	uint8 u8Index=0;
	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	/* after initialization NM goes in BusSleep state */
	NMF__enCurrentState = NMF_nenBusSleep;
	/* clear all flags */
	NMF__boNetRequest = False;
	NMF__boPassiveStartup = False;
	NMF__boRepeatMessageReq = False;
	NMF__boReceivedNMPdu = False;
	NMF__boTxConfNMPdu = False;
	NMF__boBusOffFlag = False;
	NMF__u8PowerFailStatus = 0;
	
	/* clear all counters */
	NMF__u16BusOffEvents = 0;
	NMF__u16BusOffCounter = 0;
	NMF__u16AckWKCounter = 0;
	NMF__u16TdiagCounter = 0;

	/*set NMF__boEnableRxTx*/
	NMF__boEnableRxTx = True;


	/* clear all counters */
	for(u8Index=0; u8Index < NMF__nenLastTimer; u8Index++)
	{
		NMF__au16Counter[u8Index]=0;
	}
	/* fill TxBuffer with the value of the reserved bits - 0x00 */
	for(u8Index=0; u8Index < NMF_BUFFER_SIZE; u8Index++)
	{
		NMF__au8TxBuffer[u8Index] = NMF_RESERVED_BITS;
		NMF__au8RxBuffer[u8Index] = NMF_RESERVED_BITS;
	}

	/* fill source node ID with IP's ID 0x60*/
	NMF__au8TxBuffer[0] = 0x60;
}   

/**
* Network request, called when Ign On.
*/
void NMF_vNetworkRequest(void)
{
	/* a network request was placed */
	NMF__boNetRequest = True;

	NMF__vNetworkReqCB();
}

/**
* Network release, called when Ign Off.
*/
void NMF_vNetworkRelease(void)
{
	/* network release was requested */
	NMF__boNetRequest = False;

	NMF__vNetworkRelCB();
}

/**
* request Passive Startup£¬may be not used!
*/
void NMF_vPassiveStartup(void)
{
	/* set flag because the passive startup was requested */
	NMF__boPassiveStartup = True;
}


/**
* NMF Cyclic function
*/
void NMF_vCyclicTask (void)
{
	/* local copy of the NetRequest variable */
	boolean boNetReqCpy = False;
	/* local copy of the Passive Startup variable */
	boolean boPassiveStartupCpy = False;
	/* local copy of the ReceivedNMPdu variable */
	boolean boReceivedNMPduCpy = False;
	/* local copy of the RepeatMessageBit variable */
	boolean boRepeatMsgBitCpy = False;
	/* local copy of the TxConfNMPdu variable */
	boolean boTxConfNMPduCpy = False;
	/* local copy of the BusOff flag */
	boolean boBusOffFlag = False;
	/* local copy of the BusOff flag */
	boolean boEnableRxTxCpy = False;
	/* Power Fail status changed */
	boolean boPFChanged = False;

	/* local copy of the CurrentState variable */
	NMF_tenStates enCurrentStateCpy = NMF_nenUnInitialized;
	/* local copy of PowerFail status */
	uint8 u8PowerFailStatus = 0;

	COM_ENTER_PROTECTED_SECTION;
	/* copy the network request status into the local variable */
	boNetReqCpy = NMF__boNetRequest;
	/* copy the passive startup status into the local variable */
	boPassiveStartupCpy = NMF__boPassiveStartup;
	NMF__boPassiveStartup = False;
	/* copy the current state into the local variable */
	enCurrentStateCpy = NMF__enCurrentState;
	/* copy the ReceivedNMPdu variable status into the local variable */
	boReceivedNMPduCpy = NMF__boReceivedNMPdu;
	/* clear received flag after evaluation - prepare for next reception */
	NMF__boReceivedNMPdu = False;
	/* copy the RepeatMsgBit variable status into the local variable */
	boRepeatMsgBitCpy = NMF__boRepeatMsgBit;
	/* copy the BusOff status into the local variable */
	boBusOffFlag = NMF__boBusOffFlag;
	/* copy the TxConfNMPdu variable status into the local variable */
	boTxConfNMPduCpy = NMF__boTxConfNMPdu;
	/* copy PowerFail status into the local variable */
	u8PowerFailStatus = NMF__u8PowerFailStatus;
	if(u8OldPowerFailStatus != u8PowerFailStatus)
	{
		u8OldPowerFailStatus = u8PowerFailStatus;
		boPFChanged = True;
	}
	/* copy the NMF__boEnableRxTx variable status into the local variable */
	boEnableRxTxCpy = NMF__boEnableRxTx;

	COM_LEAVE_PROTECTED_SECTION;
	/* check the state */
	switch(enCurrentStateCpy)
	{
	case NMF_nenBusSleep:
		/* check if a network request or a PassiveStartup was done */
		if((boNetReqCpy == True) || (boPassiveStartupCpy == True))
		{
			/* transition to RepeatMessage state  */
			NMF__vBusSleepToRepeatMessage();
		}
		else
		{/* no request and no PassiveStartup was done */
			
			/* check if any PDU received */
			if (boReceivedNMPduCpy == True)
			{
				/* clear couter */
				NMF__u16AckWKCounter = 0;

				/* inform APPL about succesfully received NM PDU */
				NMF__vWakeupCB();
			}
			else
			{
				/* check if wakeup occured - counter should be different by 0 */
				if (NMF__u16AckWKCounter != 0)
				{
					/* decrement counter */
					NMF__u16AckWKCounter--;

					if(NMF__u16AckWKCounter == 0)
					{
						/* go to sleep again - release Passive */
						NMF__vRelPassiveCB();
					}
				}
			}
		}
		break;
	case NMF_nenRepeatMessage:
		/* check NM-Timeout counter */
		if (NMF__au16Counter[NMF__nenTNMTimeout] != 0)
		{
			/* counter not elapsed - decrement it */
			NMF__au16Counter[NMF__nenTNMTimeout]--;

			/* counter has elapsed */
			if(NMF__au16Counter[NMF__nenTNMTimeout] == 0)
			{
				/* load the counter associated to NM-Timeout Timer */
				NMF__au16Counter[NMF__nenTNMTimeout] = NMF_TNM_TIMEOUT;
				/* notify Application or Diagnose about NM Timeout */
				NMF__vNMTimeout();
			}
		}

		/* check if Tx confirmation */
		if(boTxConfNMPduCpy == False)
		{
			/* check Tx-Timeout counter */
			if (NMF__au16Counter[NMF__nenTTxTimeout] != 0)
			{
				/* counter not elapsed - decrement it */
				NMF__au16Counter[NMF__nenTTxTimeout]--;

				/* counter has elapsed */
				if(NMF__au16Counter[NMF__nenTTxTimeout] == 0)
				{
					/* notify Application */
					NMF__vTxTimeout();
				}
			}
		}

		/* check NM-Timeout counter */
		if (NMF__au16Counter[NMF__nenTRepeatMessage] != 0)
		{
			/* counter not elapsed - decrement it */
			NMF__au16Counter[NMF__nenTRepeatMessage]--;

			/* check NMCycle counter */
			if (NMF__au16Counter[NMF__nenTNMCycle] != 0)
			{
				/* counter not elapsed - decrement it */
				NMF__au16Counter[NMF__nenTNMCycle]--;

				/* counter has elapsed */
				if(NMF__au16Counter[NMF__nenTNMCycle] == 0)
				{
					/* load the counter associated to NMCycle Timer */
					NMF__au16Counter[NMF__nenTNMCycle] = NMF_TREPEAT_NM_CYCLE;
					
					/* call transmit function */
					NMF__vTransmit(&NMF__au8TxBuffer[0]);

					if(boTxConfNMPduCpy == True)
					{
						/* start Tx-Timeout counter */
						NMF__au16Counter[NMF__nenTTxTimeout] = NMF_TTX_TIMEOUT;
					}
				}
			}
			/* counter has elapsed */
			if(NMF__au16Counter[NMF__nenTRepeatMessage] == 0)
			{
				/* clear Repeat Message Bit */
				NMF_CLEAR_REPEAT_MESAGE_BIT(NMF__au8TxBuffer);

				/* check if a network request was done */
				if(boNetReqCpy == True)
				{
					/* transition to NormalOperation state  */
					NMF__vRepeatMessageToNormalOperation();
				}
				else 
				{
					/* transition to ReadySleep state  */
					NMF__vRepeatMessageToReadySleep();
				}
			}
		}
		break;
	case NMF_nenNormalOperation:
		/* check if a network request was done */
		if(boNetReqCpy == True)
		{
			/* check NM-Timeout counter */
			if (NMF__au16Counter[NMF__nenTNMTimeout] != 0)
			{
				/* counter not elapsed - decrement it */
				NMF__au16Counter[NMF__nenTNMTimeout]--;

				/* counter has elapsed */
				if(NMF__au16Counter[NMF__nenTNMTimeout] == 0)
				{
					/* load the counter associated to NM-Timeout Timer */
					NMF__au16Counter[NMF__nenTNMTimeout] = NMF_TNM_TIMEOUT;
					/* notify Application or Diagnose about NM Timeout */
					NMF__vNMTimeout();
				}
			}

			/* check if Tx confirmation */
			if(boTxConfNMPduCpy == False)
			{
				/* check Tx-Timeout counter */
				if (NMF__au16Counter[NMF__nenTTxTimeout] != 0)
				{
					/* counter not elapsed - decrement it */
					NMF__au16Counter[NMF__nenTTxTimeout]--;

					/* counter has elapsed */
					if(NMF__au16Counter[NMF__nenTTxTimeout] == 0)
					{
						/* notify Application */
						NMF__vTxTimeout();
					}
				}
			}

			/* check NMCycle counter */
			if (NMF__au16Counter[NMF__nenTNMCycle] != 0)
			{
				/* counter not elapsed - decrement it */
				NMF__au16Counter[NMF__nenTNMCycle]--;

				/* counter has elapsed */
				if(NMF__au16Counter[NMF__nenTNMCycle] == 0)
				{
					/* load the counter associated to NMCycle Timer */
					NMF__au16Counter[NMF__nenTNMCycle] = NMF_TNM_CYCLE;
					
					/* call transmit function */
					NMF__vTransmit(&NMF__au8TxBuffer[0]);

					if(boTxConfNMPduCpy == True)
					{
						/* start Tx-Timeout counter */
						NMF__au16Counter[NMF__nenTTxTimeout] = NMF_TTX_TIMEOUT;
					}
				}
			}
			
			/* check if any Pdu received and Repeat Message Bit received or Repeat Message requested */
			if(((boReceivedNMPduCpy == True)&&(boRepeatMsgBitCpy)) || (NMF__boRepeatMessageReq))
			{
				/* transition to RepeatMessage state  */
				NMF__vNormalOperationToRepeatMessage();
			}
		}
		else 
		{
			/* transition to ReadySleep state  */
			NMF__vNormalOperationToReadySleep();
		}
		break;
	case NMF_nenReadySleep:
		/* check NM-Timeout counter */
		if (NMF__au16Counter[NMF__nenTNMTimeout] != 0)
		{
			if(True == boEnableRxTxCpy)
			{
				/* counter not elapsed - decrement it */
				NMF__au16Counter[NMF__nenTNMTimeout]--;
			}

			/* check if a network request was done */
			if(boNetReqCpy == True)
			{
				/* transition to NormalOperation state  */
				NMF__vReadySleepToNormalOperation();
			}
			else
			{
				/* check is NMPdu received and Repeat Message Bit received or Repeat Message requested */
				if(((boReceivedNMPduCpy)&&(boRepeatMsgBitCpy)) || (NMF__boRepeatMessageReq))
				{
					/* transition to RepeatMessage state  */
					NMF__vReadySleepToRepeatMessage();
				}
			}

			/* counter has elapsed */
			if(NMF__au16Counter[NMF__nenTNMTimeout] == 0)
			{
				/* transition to PrepareBusSleep state */
				NMF__vReadySleepToPrepareBusSleep();
			}
		}
		break;
	case NMF_nenPrepareBusSleep:
		/* check WaitBusSleep counter */
		if (NMF__au16Counter[NMF__nenTWaitBusSleep] != 0)
		{
			if(True == boEnableRxTxCpy)
			{
				/* counter not elapsed - decrement it */
				NMF__au16Counter[NMF__nenTWaitBusSleep]--;
			}

			/* check if no network request was done and no NM PDU was received */
			if((boNetReqCpy == False)&&(boReceivedNMPduCpy == False))
			{
				/* counter has elapsed */
				if(NMF__au16Counter[NMF__nenTWaitBusSleep] == 0)
				{
					/* transition to BusSleep state */
					NMF__vPrepareBusSleepToBusSleep();
				}
			}
			else
			{/* if a network request was done or a NM PDU received */
			
				/* stop WaitBusSleep counter */
				NMF__au16Counter[NMF__nenTWaitBusSleep] = 0;
				/* transition to RepeatMessage state */
				NMF__vPrepareBusSleepToRepeatMessage();
			}
		}
		break;
	default:break;
	}

	/* check BusOff status */
	if (boBusOffFlag == True)
	{
		/* disable the IL timeouts */
		/*NMF__vDisableILTimeouts();*/

		/* check if BusOff counter has elapsed */
		if(NMF__u16BusOffCounter == 0)
		{
			/* check if this is the first recovery */
			if(NMF__u16BusOffEvents == 0)
			{
				/* load the counter with T1 time */
				NMF__u16BusOffCounter = NMF_T1_BUSOFF;

				CANIF_enSetControllerMode(CANIF_T_OFFLINE);
				CANIF_enSetControllerMode(CANIF_T_RECOVER);

				/* application callback for BusOff Entry */
				NMF__vBusOffEntryCB();

				/* the BusOff event occured */
				NMF__u16BusOffEvents++;
			}
			else
			{
				/* load the counter with T2 time */
				NMF__u16BusOffCounter = NMF_T2_BUSOFF;

				CANIF_enSetControllerMode(CANIF_T_OFFLINE);
				CANIF_enSetControllerMode(CANIF_T_RECOVER);

				/* application callback for BusOff Entry */
				NMF__vBusOffEntryCB();

				/* the BusOff event occured */
				NMF__u16BusOffEvents++;
			}

			/* clear counters - bus off error has higher priority */
			NMF__au16Counter[NMF__nenTNMTimeout] = 0;
			NMF__au16Counter[NMF__nenTTxTimeout] = 0;
		}
		else
		{
			NMF__u16BusOffCounter--;
			
			if(NMF__u16BusOffCounter == 0)
			{
				CANIF_enSetControllerMode(CANIF_T_ONLINE);
				
				/* clear BusOff flag */
				NMF__boBusOffFlag = False;
			}
		}
	}
	else
	{
		/* if successfully transmission and there was a BusOff situation */
		if((boTxConfNMPduCpy == True) && (NMF__u16BusOffEvents != 0))
		{
			/* clear BusOff counter */
			NMF__u16BusOffCounter = 0;

			/* clear BusOff events */
			NMF__u16BusOffEvents = 0;

			/* application callback for BusOff Exit */
			NMF__vBusOffExitCB();

			/* enable IL timeouts */
			/*NMF__vEnableILTimeouts();*/
		}
	}

	/* check the power fail status */
	if(u8PowerFailStatus & NMF_POWERFAIL_NTUV1)
	{
		/* stop Tx monitoring */
		//NMF__vDisableILTimeouts();
		if(boPFChanged)
		{
			NMF__vNormalToUnderVoltage1();
		}
	}

	if(u8PowerFailStatus & NMF_POWERFAIL_NTUV2)
	{
		if(boPFChanged)
		{
			NMF__vNormalToUnderVoltage2();
		}
	}

	if(u8PowerFailStatus & NMF_POWERFAIL_UVTN3)
	{
		if(boPFChanged)
		{
			NMF__vUnderVoltageToNormal3();
		}
	}

	if(u8PowerFailStatus & NMF_POWERFAIL_UVTN4)
	{
		if(boPFChanged)
		{
			NMF__vUnderVoltageToNormal4();
		}
		/* check diag counter */
		if(NMF__u16TdiagCounter)
		{
			NMF__u16TdiagCounter--;

			if(NMF__u16TdiagCounter == 0)
			{
				/* enable Tx monitoring */
				//NMF__vEnableILTimeouts();

				/* callback for APPL - in order to start diagnostic */
				NMF__vStartDiagCB();
			}
		}
	}
	
	if(u8PowerFailStatus & NMF_POWERFAIL_NTOV5)
	{
		if(boPFChanged)
		{
			NMF__vNormalToOverVoltage5();
		}
	}

	if(u8PowerFailStatus & NMF_POWERFAIL_NTOV6)
	{
		/* stop Tx monitoring */
		//NMF__vDisableILTimeouts();
		if(boPFChanged)
		{
			NMF__vNormalToOverVoltage6();
		}
	}

	if(u8PowerFailStatus & NMF_POWERFAIL_OVTN7)
	{
		if(boPFChanged)
		{
			NMF__vOverVoltageToNormal7();
		}
	}

	if(u8PowerFailStatus & NMF_POWERFAIL_OVTN8)
	{
		if(boPFChanged)
		{
			NMF__vOverVoltageNormal8();
		}
		/* check diag counter */
		if(NMF__u16TdiagCounter)
		{
			NMF__u16TdiagCounter--;

			if(NMF__u16TdiagCounter == 0)
			{
				/* enable Tx monitoring */
				//NMF__vEnableILTimeouts();

				/* callback for APPL - in order to start diagnostic */
				NMF__vStartDiagCB();
			}
		}
	}
}

/**
* Returns the NMF's current state
*/
NMF_tenStates NMF_enGetCurrentState(uint8 u8Network)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	return NMF__enCurrentState;
}

/**
* Notifies NMF package that bus off occured.
* 
*/
void NMF_vBusOffIndication (uint8 u8Network)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	NMF__boBusOffFlag = True;
}

/**
* Notifies NMF package that CAN wake-up occured.
*/
void NMF_vWakeup (uint8 u8Network)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	/* set current state in BusSleep 
	NMF__enCurrentState = NMF_nenBusSleep;*/

	/* load the counter for acknowledged frame since wakeup */
	NMF__u16AckWKCounter = NMF_TACK_FRAME;

	/* request Passive */
	NMF__vReqPassiveCB();
}
/**
* Notifies NMF package that a PDU was received.
*/
void NMF_vRangeRxIndication (CANIF_tstPDUType* pstPdu)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	if(True == NMF__boEnableRxTx)
	{
		/* a NM PDU was received - set the flag */
		NMF__boReceivedNMPdu = True;

		/* load counter because there are nodes on the bus that are keeping the network awake */
		NMF__au16Counter[NMF__nenTNMTimeout] = NMF_TNM_TIMEOUT;
		
		/* get can frame, and CAN id */
		COM_memcopy(&NMF__au8RxBuffer[0], pstPdu->pu8Data, NMF_BUFFER_SIZE);

		/* check the value of the received Repeat Message Bit */
		if(NMF__au8RxBuffer[NMF_CONTROL_BIT_VECTOR] & NMF_REPEAT_MSG_BIT)
		{
			NMF__boRepeatMsgBit = True;
		}
		else
		{
			NMF__boRepeatMsgBit = False;
		}
	}
}

/**
* Notifies NMF package that the PDU was succesfully transmitted.
*/
void NMF_vTxConfirmation (void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	//NMF__boTxConfNMPdu = True;

	/* load counter because there are nodes on the bus that are keeping the network awake */
	NMF__au16Counter[NMF__nenTNMTimeout] = NMF_TNM_TIMEOUT;

	/* clear the TxTimeout counter - the Pdu was sent successfuly */
	NMF__au16Counter[NMF__nenTTxTimeout] = 0;
}

/**
* Notifies NMF package that a RepeatMessage request was performed
* @param none
* 
*/
void NMF_vRepeatMessageReq (void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	
	/* check if NM current state is NormalOperation or ReadySleep */
	if((NMF__enCurrentState == NMF_nenReadySleep) || (NMF__enCurrentState == NMF_nenNormalOperation))
	{
		/* set the flag */
		NMF__boRepeatMessageReq = True;
	}
}

/**
* When battery voltage is showing decreasing trend  and voltage comes to
* the operating point1 (Ubat_low reached) the diagnostic should be deactivated 
*/
void NMF_vNormalToUnderVoltage1 (void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	COM_ENTER_PROTECTED_SECTION;
    NMF__u8PowerFailStatus = NMF_POWERFAIL_NTUV1;
	COM_LEAVE_PROTECTED_SECTION;
}

/**
* When battery voltage is showing decreasing trend  and voltage comes to
* the operating point2 (Ubat_min reached) the transmission and reception should be stopped
*/
void NMF_vNormalToUnderVoltage2 (void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	COM_ENTER_PROTECTED_SECTION;
	NMF__u8PowerFailStatus = NMF_POWERFAIL_NTUV2;
	COM_LEAVE_PROTECTED_SECTION;
}

/**
* When battery voltage starts powering up and comes to operating point3 (Ubat_min reached),
* the transmission and reception should be started.
*/
void NMF_vUnderVoltageToNormal3 (void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	COM_ENTER_PROTECTED_SECTION;
	NMF__u8PowerFailStatus = NMF_POWERFAIL_UVTN3;
	COM_LEAVE_PROTECTED_SECTION;
}
/**
* When battery voltage comes to operating point4 (Ubat_low reached), the Tdiag_start timer should be started.
* After this timer elapsed the diagnostic should be started.
*/
void NMF_vUnderVoltageToNormal4 (void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	COM_ENTER_PROTECTED_SECTION;
	NMF__u8PowerFailStatus = NMF_POWERFAIL_UVTN4;
	COM_LEAVE_PROTECTED_SECTION;
}

/**
* When battery voltage is showing increasing trend and voltage comes to the
* operating point5 (Ubat_high is reached) , the diagnostic should be stopped,
* transmission and reception is working.
*/
void NMF_vNormalToOverVoltage5 (void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	COM_ENTER_PROTECTED_SECTION;
	NMF__u8PowerFailStatus = NMF_POWERFAIL_NTOV5;
	COM_LEAVE_PROTECTED_SECTION;
}

/**
* When battery voltage is showing increasing trend and voltage
* until voltage comes to operating point6 (Ubat_max is reached), the diagnostic should be stopped,
* transmission and reception should be stopped.
*/
void NMF_vNormalToOverVoltage6 (void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	COM_ENTER_PROTECTED_SECTION;
	NMF__u8PowerFailStatus = NMF_POWERFAIL_NTOV6;
	COM_LEAVE_PROTECTED_SECTION;
}

/**
* When battery voltage starts falling down and comes to operating point7 (Ubat_max is reached)
* the diagnostic should be stopped, transmission and reception is working.
*/
void NMF_vOverVoltageToNormal7 (void)
{
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	COM_ENTER_PROTECTED_SECTION;
	NMF__u8PowerFailStatus = NMF_POWERFAIL_OVTN7;
	COM_LEAVE_PROTECTED_SECTION;
}
/**
* When battery voltage comes to operating point8 (Ubat_high is reached), 
* the diagnostic should be started after tdiag_start, transmission and reception is working.
*/
void NMF_vOverVoltageToNormal8 (void){
	/******************************************************************************
	* FUNCTION LOCAL DEFINITIONS
	******************************************************************************/

	/******************************************************************************
	* FUNCTION CODE
	******************************************************************************/
	COM_ENTER_PROTECTED_SECTION;
	NMF__u8PowerFailStatus = NMF_POWERFAIL_OVTN8;
	COM_LEAVE_PROTECTED_SECTION;
}

/**
* Set Wakeup Sources
* @param u8Value- the current Wakeup Source
* valid values: from 0 to 31
* 0x00 for CAN Wakeup, 0x01 for KL15
* from 0x02 to 0x31 other Wakeup Sources
* 0x31 reserved
*/
void NMF_vSetWakeupSource(uint8 u8Value)
{
	/* write in the tx buffer the Wakeup Sources */
	NMF__au8TxBuffer[NMF_WAKEUP_SOURCES] = u8Value;
}

/**
* Get Wakeup Sources
*/
uint8 NMF_vGetWakeupSource(void)
{
	/* get the Wakeup Sources from rx buffer */
	return ((uint8)NMF__au8RxBuffer[NMF_WAKEUP_SOURCES]);
}

/**
* Set NM Active Sources
* @param u8Value- the current NM Active Source
* valid values: 0x00 for KL15, 0x01 for Diagnose,
* 0x02 for Repeat Message Timer has not expired
* 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 - for Other Active Sources
*/
void NMF_vSetNMActiveSource(uint8 u8Value)
{
	/* write in the tx buffer the NM Active source */
	NMF__au8TxBuffer[NMF_NM_ACTIVE_SOURCES] = u8Value;
}


/**
* Get NM Active Sources
*/
uint8 NMF_vGetNMActiveSource(void)
{
	/* get the NM Active source from rx buffer */
	return ((uint8)NMF__au8RxBuffer[NMF_NM_ACTIVE_SOURCES]);
}

/**
* Get NM State
*/
uint8 NMF_vGetNMState(void)
{
	/* get the NMState from Tx buffer */
	return ((uint8)NMF__au8TxBuffer[NMF_NM_STATE]);
}

/**
* Set NM RxTx enable
*/
void NMF_vEnableRxTx(void)
{
	NMF__boEnableRxTx = True;
}

/**
* Set NM RxTx disable
*/
void NMF_vDisableRxTx(void)
{
	NMF__boEnableRxTx = False;
}

/**
* Set GWRequestBit
*/
void NMF_vSetGWRequestBit(void)
{
	/* Set GWRequestBit */
	NMF__au8TxBuffer[NMF_NM_STATE] |= 0x80;
}

/**
* Clear GWRequestBit
*/
void NMF_vClearGWRequestBit(void)
{
	/* Set GWRequestBit */
	NMF__au8TxBuffer[NMF_NM_STATE] &= ~0x80;
}
