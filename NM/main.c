#include<stdio.h>
#include <windows.h>

#include "nmf1c1.h"


UINT timerid = 0;
DWORD dwThreadId = 0;

uint8 u8Data[8] = { 0 };
CANIF_tstPDUType Test_stPdu = 
{
	0x55,
	0x8,
	u8Data
};

VOID CALLBACK Timer_vCallBack(
	HWND hwnd,     // handle of window for timer messages
	UINT uMsg,     // WM_TIMER message
	UINT idEvent,  // timer identifier
	DWORD dwTime   // current system time
	)
{
#ifdef Test_Log
	static int s_count = 0;
	printf("WM_TIMER in work thread s_count = %d\n", ++s_count);
#endif

	NMF_vCyclicTask();

}

DWORD CALLBACK ThreadRoutine(PVOID pvoid)
{
	unsigned int u32Cmd = 0;

	while (1)
	{
		if(u32Cmd & 0x1)
		{
			NMF_vNetworkRequest();
		}
		else
		{
			NMF_vNetworkRelease();
		}

		if (u32Cmd & 0x2)
		{
			Test_stPdu.pu8Data[1] = 0x01;
			NMF_vRangeRxIndication(&Test_stPdu);
		}
	}
}


int main()
{
	printf("---------------------------------------------------------------\n\n");
	printf("                     Autosar Network Test \n\n");
	printf("---------------------------------------------------------------\n");

	NMF_vInit();

	timerid = SetTimer(NULL, 0, 10, Timer_vCallBack);

	HANDLE hThread = CreateThread(NULL, 0, ThreadRoutine, 0, 0, &dwThreadId);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_TIMER)
		{
			DispatchMessage(&msg);
		}
	}

	while (1);
	return 0;
}


