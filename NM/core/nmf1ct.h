/*
* Check if information is already included
*/
#ifndef NMF_CT_H
#define NMF_CT_H

/***************************************************************************
* HEADER-FILES (Only those that are needed in this file)
****************************************************************************/

/* System-headerfiles */

/* Inclusion of additional headers should be avoided as much as */
/* possible to reduce dependencies */

/***************************************************************************
* External Interface DEFINITIONS
*
* In this section define
* 
****************************************************************************/
/* Enter into Critical Region, e.g. Disable Interupt */
#define COM_ENTER_PROTECTED_SECTION	
/* Exit from Critical Region, e.g. Enable Interupt */
#define COM_LEAVE_PROTECTED_SECTION	

/***************************************************************************
* GLOBAL TYPE DEFINITIONS
*
* In this section define
* - types which are public for other modules and can be used standalone, i.e.
*   without including the C1 header
****************************************************************************/

/**   Data Type 
*
*/
typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;

typedef unsigned char	boolean;
#define True			(boolean) (1 == 1)
#define False			(boolean) (1 == 0)

 /**  NMF's states
*
*/
enum NMF_enStates{

   /**
    * NM is in not initialized yet.
    * The value 0 was not chosen by mistake since afer reset RAM should be set
    * to 0
    */
    NMF_nenUnInitialized = 0,

   /**
    * NM is in Bus-Sleep
    * Tx off, Rx off, Wake-up enabled
    */
    NMF_nenBusSleep,

   /**
    * NM is in Repeat Message
    * Tx on, Rx on, Wake-up disabled
    */
    NMF_nenRepeatMessage,

   /**
    * NM is in Normal Operation
    * Tx on, Rx on, Wake-up disabled
    */
    NMF_nenNormalOperation,
	
	/**
    * NM is in Ready Sleep
    * Tx off, Rx on, Wake-up disabled
    */
    NMF_nenReadySleep,

	/**
    * NM is in Prepare Bus-Sleep
    * Tx off, Rx off, Wake-up disabled
    */
    NMF_nenPrepareBusSleep
};

typedef enum NMF_enStates NMF_tenStates;


enum NMF_enMsgGroupSts
{
	NMF_nenOffLine,
	NMF_nenOnLine
};

typedef enum NMF_enMsgGroupSts NMF_tenMsgGtoupSts;

/**
*   This should be defined in CANIF module 
*/
typedef struct
{
	uint16 u16Id;
	uint8 u8Dlc;
	uint8* pu8Data;

}CANIF_tstPDUType;

/*
* End of Check if information is already included
*/
#endif /* NMF_CT_H */


