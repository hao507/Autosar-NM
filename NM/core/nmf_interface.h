#ifndef NMF_Interface_H
#define NMF_Interface_H
/****************************************************************************
** Header Files (Only those that are needed in this file)
****************************************************************************/

/* System Header Files */

/* Foreign headerfiles */

/* Own headerfiles */
#include "nmf1ct.h"



/* System Wide Scope */
void NMF_vInit(void);

void NMF_vNetworkRequest(void);

void NMF_vNetworkRelease(void);

void NMF_vCyclicTask(void);

NMF_tenStates NMF_enGetCurrentState(uint8 u8Network);

void NMF_vBusOffIndication(uint8 u8Network);

void NMF_vWakeup(uint8 u8Network);

void NMF_vPassiveStartup(void);

void NMF_vRangeRxIndication(CANIF_tstPDUType* pstPdu);

void NMF_vTxConfirmation(void);

void NMF_vRepeatMessageReq (void);

void NMF_vNormalToUnderVoltage1 (void);

void NMF_vNormalToUnderVoltage2 (void);

void NMF_vUnderVoltageToNormal3 (void);

void NMF_vUnderVoltageToNormal4 (void);

void NMF_vNormalToOverVoltage5 (void);

void NMF_vNormalToOverVoltage6 (void);

void NMF_vOverVoltageToNormal7 (void);

void NMF_vOverVoltageToNormal8 (void);

void NMF_vSetWakeupSource(uint8 u8Value);

void NMF_vSetNMActiveSource(uint8 u8Value);

uint8 NMF_vGetWakeupSource(void);

uint8 NMF_vGetNMActiveSource(void);

uint8 NMF_vGetNMState(void);

void NMF_vEnableRxTx(void);

void NMF_vDisableRxTx(void);

void NMF_vSetGWRequestBit(void);

void NMF_vClearGWRequestBit(void);


#endif /* NMF_Interface_H */
