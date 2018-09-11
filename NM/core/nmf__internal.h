#ifndef NMF__Internal_H
#define NMF__Internal_H
/****************************************************************************
** Header Files (Only those that are needed in this file)
****************************************************************************/

/* System Header Files */

/* Foreign headerfiles */


/* Own headerfiles */
#include "nmf1ct.h"

/***************************************************************************
* Definition of exported global data used by Class
* Macros, Enumerations, Types,  Forward declarations
***************************************************************************/

/**
* NMF current state.
*  context task (SINI & CAM)
*  protection none since SINI and CAM container should not interrupt each other.
* @range  see enumaration description
* @values 0 -> the package is not initialized
*/
extern NMF_tenStates NMF__enCurrentState;

/**
* Contains the status of the network request
*  context task - handled inside the task
*  protection none
* @range  TRUE - FALSE
* @values TRUE network was required 
*	     FALSE network was not required
*/ 
extern boolean NMF__boNetRequest;

/**
* Contains the status of RepeatMessage request
*  context task - handled inside the task
*  protection none
* @range  TRUE - FALSE
* @values TRUE network was required 
*	     FALSE network was not required
*/ 
extern boolean NMF__boRepeatMessageReq;

/**
* Contains the status of passive startup request
*  context task - handled inside the task
*  protection none
* @range  TRUE - FALSE
* @values TRUE passive startup was required 
*	     FALSE passive startup was not required
*/ 
extern boolean NMF__boPassiveStartup;

/**
* Contains the status of received NM PDU
*  context task - handled inside the task
*  protection none
* @range  TRUE - FALSE
* @values TRUE NM PDU received
*	     FALSE NM PDU was not received
*/ 
extern boolean NMF__boReceivedNMPdu;

/**
* Contains the status of repeat message bit
*  context task and ISR
*  protection none
* @range  TRUE - FALSE
* @values TRUE NM PDU received with RepeatMsg set
*	     FALSE NM PDU with RepeatMsg not set
*/ 
extern boolean NMF__boRepeatMsgBit;

/**
* Contains the status of a successfully NM PDU transmission
*  context task - handled inside the task
*  protection none
* @range  TRUE - FALSE
* @values TRUE Tx confirmation
*	     FALSE no successfully transmission
*/ 
extern boolean NMF__boTxConfNMPdu;

/**
* Contains the status of BusOff flag
*  context task - handled inside the task
*  protection none
* @range  TRUE - FALSE
* @values TRUE BusOff occured
*	     FALSE BusOff doen't occurend
*/ 
extern boolean NMF__boBusOffFlag;

/**
* Buffer for transmission
*  context task - handled inside the task
*  protection none
* @range  
* @values 
*/
extern uint8 NMF__au8TxBuffer[];

/**
* Buffer for reception
*  context task - handled inside the task
*  protection none
* @range  
* @values 
*/
extern uint8 NMF__au8RxBuffer[];

/**
* Contains the counters associated to each NM timer
*  context task - handled inside the task
*  protection none
* @range  0 - MAX(uint16)
* @values 0 counter is stopped
*	     1 - MAX(uint16) counter Started
*/
extern uint16 NMF__au16Counter[];

/**
* Contains the counter associated to BusOff timer
*  context task - handled inside the task
*  protection none
* @range  0 - MAX(uint16)
* @values 0 counter is stopped
*	     1 - MAX(uint16) counter Started
*/
extern uint16 NMF__u16BusOffCounter;

/**
* Counter for acknowledge frame since wakeup
*  context task - handled inside the task
*  protection none
* @range  0 - MAX(uint16)
* @values 0 counter is stopped
*	     1 - MAX(uint16) counter Started
*/ 
extern uint16 NMF__u16AckWKCounter;

/**
* Contains the counter associated to BusOff events
*  context task - handled inside the task
*  protection none
* @range  0 - MAX(uint16)
* @values 0 counter is stopped
*	     1 - MAX(uint16) counter Started
*/
extern uint16 NMF__u16BusOffEvents;

/**
* Contains the counter associated to diagnostic timer
*  context task - handled inside the task
*  protection none
* @range  0 - MAX(uint16)
* @values 0 counter is stopped
*	     1 - MAX(uint16) counter Started
*/
extern uint16 NMF__u16TdiagCounter;

/**  NMF list of timers
*
*/
enum NMF__enTimer {

	/**
	* NM Timeout Timer
	*/
	NMF__nenTNMTimeout = 0,

	/**
	* Repeat Message Timer
	*/
	NMF__nenTRepeatMessage,

	/**
	* Wait Bus Sleep Timer
	*/
	NMF__nenTWaitBusSleep,

	/**
	* NM Cycle Timer - used for RepeatNMCycle and NMCycle
	*/
	NMF__nenTNMCycle,

	/**
	* Tx Timeout Timer
	*/
	NMF__nenTTxTimeout,

	/**
	* Last Timer. Do not add timers after this entry
	*/
	NMF__nenLastTimer
};

typedef enum NMF__enTimer NMF__tenTimer;

  

/* Package Global Scope */

/* Size of a NM frame */
#define NMF_BUFFER_SIZE 8

/* Control byte */
#define NMF_CONTROL_BIT_VECTOR 0x01

/* NM State byte */
#define NMF_NM_STATE 0x02

/* Wakeup Sources byte */
#define NMF_WAKEUP_SOURCES 0x03

/* NM Active Sources byte */
#define NMF_NM_ACTIVE_SOURCES 0x04

/* reserved bits value */
#define NMF_RESERVED_BITS 0x00

/* mask for Repeat Message Bit */
#define NMF_REPEAT_MSG_BIT 0x01

/* mask for Repeat Message State(from Bus Sleep Mode) */
#define NMF_REP_MSG_BUS_SLEEP_BIT 0x01

/* mask for Repeat Message State(from Prepare Bus Sleep Mode) */
#define NMF_REP_MSG_PREP_BUS_SLEEP_BIT 0x02

/* mask for Repeat Message State(from Normal Operation State) */
#define NMF_REP_MSG_NORMAL_OP_BIT 0x04

/* mask for Repeat Message State(from Ready Sleep State) */
#define NMF_REP_MSG_READY_SLEEP_BIT 0x08

/* mask for Normal Operation State(from Repeat Message State) */
#define NMF_NORMAL_OP_REP_MSG_BIT 0x10

/* mask for Normal Operation State(from Ready Sleep Mode) */
#define NMF_NORMAL_OP_READY_SLEEP_BIT 0x20

/* mask for Wakeup(from Gateway Evaluates) */
#define NMF_WAKEUP_BIT 0x40

/* mask for Active Sources - CAN WAKEUP  */
#define NMF_ACTIVE_SOURCE_CAN_WK 0x00

/* mask for Active Sources - KL15 */
#define NMF_ACTIVE_SOURCE_KL_15 0x01


extern const uint16 NMF__u16MessageID;
#define NMF__nMessageID  NMF__u16MessageID 

extern const uint8 NMF__u8MessageDLC;
#define NMF__nMessageDLC NMF__u8MessageDLC

/* set Repeat Message Bit */
#define NMF_SET_REPEAT_MESAGE_BIT(tx_buf)\
{\
	(tx_buf)[NMF_CONTROL_BIT_VECTOR] |= NMF_REPEAT_MSG_BIT;\
}

/* clear Repeat Message Bit */
#define NMF_CLEAR_REPEAT_MESAGE_BIT(tx_buf)\
{\
	(tx_buf)[NMF_CONTROL_BIT_VECTOR] &= (uint8)(~NMF_REPEAT_MSG_BIT);\
}

#define NMF_ALL_GROUPS 0x01
#define NMF_APPLICANTION_GROUP 0x02
#define NMF_NETMANAGEMENT_GROUP 0x03

/* define masks for PowerFail */
#define NMF_POWERFAIL_NTUV1 0x01
#define NMF_POWERFAIL_NTUV2 0x02
#define NMF_POWERFAIL_UVTN3 0x04
#define NMF_POWERFAIL_UVTN4 0x08
#define NMF_POWERFAIL_NTOV5 0x10
#define NMF_POWERFAIL_NTOV6 0x20
#define NMF_POWERFAIL_OVTN7 0x40
#define NMF_POWERFAIL_OVTN8 0x80


#ifdef NMF__Internal_C


#endif

/***************************************************************************
*   Class Operations
***************************************************************************/

/* Note: System Wide Scope Functions should only be contained in interface classes */

void NMF__vTransmit(const uint8 *pu8Data);

void NMF__vBusSleepToRepeatMessage(void);
   
void NMF__vPrepareBusSleepToRepeatMessage(void);

void NMF__vRepeatMessageToNormalOperation(void);

void NMF__vNormalOperationToRepeatMessage(void);

void NMF__vRepeatMessageToReadySleep(void);

void NMF__vReadySleepToRepeatMessage(void);

void NMF__vNormalOperationToReadySleep(void);

void NMF__vReadySleepToNormalOperation(void);

void NMF__vReadySleepToPrepareBusSleep(void);

void NMF__vPrepareBusSleepToBusSleep(void);
 
void NMF_vSetPartOnline(uint8 u8Group );

void NMF_vSetPartOffline(uint8 u8Group );

void NMF__vNormalToUnderVoltage1(void);

void NMF__vNormalToUnderVoltage2(void);

void NMF__vUnderVoltageToNormal3(void);

void NMF__vUnderVoltageToNormal4(void);

void NMF__vNormalToOverVoltage5(void);

void NMF__vNormalToOverVoltage6(void);

void NMF__vOverVoltageToNormal7(void);

void NMF__vOverVoltageNormal8(void);

void NMF__vSetNMState(uint8 u8Value);

/* NMF callbacks */

void NMF__vNetworkReqCB(void);

void NMF__vNetworkRelCB(void);

void NMF__vWakeupCB(void);

void NMF__vReqPassiveCB(void);

void NMF__vRelPassiveCB(void);

void NMF__vBusSleepToRepeatMessageCB(void);

void NMF__vRepeatMessageToNormalOperationCB(void);

void NMF__vNormalOperationToRepeatMessageCB(void);

void NMF__vRepeatMessageToReadySleepCB(void);

void NMF__vReadySleepToRepeatMessageCB(void);

void NMF__vReadySleepToNormalOperationCB(void);

void NMF__vNormalOperationToReadySleepCB(void);

void NMF__vReadySleepToPrepareBusSleepCB(void);

void NMF__vPrepareBusSleepToBusSleepCB(void);

void NMF__vPrepareBusSleepToRepeatMessageCB(void);

void NMF__vNMTimeout(void);

void NMF__vTxTimeout(void);

void NMF__vEnableILTimeouts(void);

void NMF__vDisableILTimeouts(void);

void NMF__vBusOffEntryCB(void);

void NMF__vBusOffExitCB(void);

void NMF__vStartDiagCB(void);

void NMF__vStopDiagCB(void);


#ifdef NMF__Internal_C
  
#endif

#endif /* NMF__Internal_H */
