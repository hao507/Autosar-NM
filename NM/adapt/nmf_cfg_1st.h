
#ifndef NMF_CFG_1ST_H
#define NMF_CFG_1ST_H

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
/* Cyclic NM Task */
#define NMF_CYCLIC_TASK 10
/* NM-Timeout Timer */
#define NMF_TNM_TIMEOUT (5000/NMF_CYCLIC_TASK)
/* Repeat Message Timer  - represent the time for staying in Repreat Message state */
#define NMF_TREPEAT_MESSAGE (500/NMF_CYCLIC_TASK)
/* Wait Bus-Sleep Timer */
#define NMF_TWAIT_BUS_SLEEP (1500/NMF_CYCLIC_TASK)
/* Repeat NM Cycle Timer - represent the NM PDU transmission period while in Repeat Message state*/
#define NMF_TREPEAT_NM_CYCLE (100/NMF_CYCLIC_TASK)
/* NM Cycle Timer - represent the NM PDU transmission period while in Normal Operation state */
#define NMF_TNM_CYCLE (200/NMF_CYCLIC_TASK)
/* Tx Timeout Timer */
#define NMF_TTX_TIMEOUT (2000/NMF_CYCLIC_TASK)
/* Time for Acknowledged frame since CAN WAKEUP   */
#define NMF_TACK_FRAME (1500/NMF_CYCLIC_TASK)

/* BusOff Timer T1 */
#define NMF_T1_BUSOFF (20/NMF_CYCLIC_TASK)
/* BusOff Timer T2 */
#define NMF_T2_BUSOFF (200/NMF_CYCLIC_TASK)

/* Time limit for starting the diagnostic */
#define NMF_TDIAG_START  (500/NMF_CYCLIC_TASK)

#define CANIF_T_OFFLINE		"CANIF: CANIF_T_OFFLINE\n"
#define CANIF_T_STOP		"CANIF: CANIF_T_STOP\n"
#define CANIF_T_START		"CANIF: CANIF_T_START\n"
#define CANIF_T_ONLINE		"CANIF: CANIF_T_ONLINE\n"
#define CANIF_T_RECOVER		"CANIF: CANIF_T_RECOVER\n"

#define CANIF_enSetControllerMode(sts)	printf(sts);

#define COM_memcopy			memcpy

#endif /*#ifndef NMF_CFG_1ST_H*/

