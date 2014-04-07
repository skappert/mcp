// Hardware.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "camac.h"
#include "Hardware.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void ResetLAMTrigger(int Slot)
{
	SendNAFCamac(Slot,0,10);
}

void EnableLAMTrigger(int Slot)
{
	SendDNAFCamac(Slot,2,0,16);
}

void DisableLAMTrigger(int Slot)
{
	SendDNAFCamac(Slot,0,0,16);
}

void ListArmTrigger(int Slot)
{
	ListDNAFCamac(SENDNAF,(USHORT)0,Slot,1,10);
	ListDNAFCamac(SENDDNAF|WAITINT,(USHORT)2,Slot,0,16);
	ListDNAFCamac(SENDDNAF,(USHORT)0,Slot,0,16);
	ListDNAFCamac(READENCL,0,0,0,0);
	ListDNAFCamac(SENDNAF,(USHORT)0,Slot,1,10);	
}

void ListDeArmTrigger(int Slot)
{
	ListDNAFCamac(SENDDNAF,(USHORT)0,Slot,0,16);
}

void ListLoadTimer(int Slot,double Time)
{
	ULONG muTime;
	muTime = (ULONG)(Time*1e4);
	ListDNAFCamac(SENDDNAF,(USHORT)(muTime),Slot,0,16);
	ListDNAFCamac(SENDDNAF,(USHORT)(muTime>>16),Slot,1,16);
}

void ListLoadTimerWait(int Slot,double Time)
{
	ULONG muTime;
	muTime = (ULONG)(Time*1e4);
	ListDNAFCamac(SENDDNAF,(USHORT)(muTime),Slot,0,16);
	ListDNAFCamac(SENDDNAF|WAITINT,(USHORT)(muTime>>16),Slot,1,16);
}

void ListStartTimer(int Slot)
{
	ListDNAFCamac(SENDNAF|WAITINT,0,Slot,1,26);
	ListDNAFCamac(SENDNAF,0,Slot,0,10);
	ListDNAFCamac(READENCL,0,0,0,0);
}

void ListEnableLAMTimer(int Slot)
{
	ListDNAFCamac(SENDNAF,0,Slot,0,26);
}

void ListDisableLAMTimer(int Slot)
{
	ListDNAFCamac(SENDNAF,0,Slot,0,24);
}

void ListResetLAMTimer(int Slot)
{
	ListDNAFCamac(SENDNAF,0,Slot,0,10);
}

void LoadTimer(int Slot,double Time)
{
	ULONG muTime;
	muTime = (ULONG)(Time*1e4);
	SendDNAFCamac((USHORT)(muTime),Slot,0,16);
	SendDNAFCamac((USHORT)(muTime>>16),Slot,1,16);
}

void StartTimer(int Slot)
{
	SendNAFCamac(Slot,1,26);
}

void EnableLAMTimer(int Slot)
{
	SendNAFCamac(Slot,0,26);
}

void DisableLAMTimer(int Slot)
{
	SendNAFCamac(Slot,0,24);
}

void ResetLAMTimer(int Slot)
{
	SendNAFCamac(Slot,0,10);
	ReadCSRCamac();
	ReadENCLCamac();
	SendNAFCamac(Slot,1,24);
	ReadCSRCamac();
	ReadENCLCamac();
}


USHORT TwoTimesX(USHORT times)
{
	USHORT i;
	USHORT y=1;
	for(i=times;i>0;i--) y=2*y;
	return y;
}

BOOL ListSendGPIB(CString Command,int Address,int CamacSlot)
{
	char TheCommand[255];
	USHORT i;

	strcpy(TheCommand,Command);

	ListDNAFCamac(SENDNAF,0,CamacSlot,0,26);
	ListDNAFCamac(SENDNAF,0,CamacSlot,1,26);
	ListDNAFCamac(SENDNAF,0,CamacSlot,4,26);
	ListDNAFCamac(SENDDNAF,8,CamacSlot,13,17);	// Write LAM-Mask
	ListDNAFCamac(SENDF,0,0,0,64+CamacSlot);	// Enable Interrupt from #CamacSlot
	ListDNAFCamac(SENDF,0,0,0,41);				// Enable Interrupts
	ListDNAFCamac(SENDDNAF,32+Address,CamacSlot,0,16);
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDDNAF,64+21,CamacSlot,0,16);
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDNAF,0,CamacSlot,0,24);
	for (i=0;i<strlen(TheCommand);i++)
	{
		ListDNAFCamac(SENDDNAF,TheCommand[i],CamacSlot,0,16);
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	}

	ListDNAFCamac(SENDDNAF,13,CamacSlot,0,16);			// CR
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDDNAF,10,CamacSlot,0,16);			// LF
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDNAF,0,CamacSlot,0,26);
	ListDNAFCamac(SENDDNAF,UNL,CamacSlot,0,16);			// UNLISTEN
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDDNAF,UNT,CamacSlot,0,16);			// UNTALK
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDNAF,0,CamacSlot,0,24);
	ListDNAFCamac(SENDDNAF,0,CamacSlot,13,17);			// Write LAM-Mask (disable all)
	ListDNAFCamac(SENDF,0,0,0,128+CamacSlot);			// Disable Interrupt from #CamacSlot
	ListDNAFCamac(READENCL,0,0,0,0);
	return TRUE;
}

BOOL SendGPIB(CString Command,int Address,int CamacSlot)
{
	char TheCommand[255];
	USHORT i;
	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
	if(!pMyApp->GlobalMeasurementRunning&&pMyApp->CamacOn)
	{
		strcpy(TheCommand,Command);
		while (!GPIBReady()||!ListReady());
		ListReset();
		DataReset();
		
		ListDNAFCamac(SENDNAF,0,CamacSlot,0,26);
		ListDNAFCamac(SENDNAF,0,CamacSlot,1,26);
		ListDNAFCamac(SENDNAF,0,CamacSlot,4,26);

		ListDNAFCamac(SENDDNAF,8,CamacSlot,13,17);	// Write LAM-Mask

		ListDNAFCamac(SENDF,0,0,0,32);				// Disable Inhibit


		ListDNAFCamac(SENDF,0,0,0,64+CamacSlot);	// Enable Interrupt from #CamacSlot

		ListDNAFCamac(SENDF,0,0,0,41);				// Enable Interrupts

		ListDNAFCamac(SENDDNAF,32+Address,CamacSlot,0,16);
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		ListDNAFCamac(SENDDNAF,64+21,CamacSlot,0,16);
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		ListDNAFCamac(SENDNAF,0,CamacSlot,0,24);

		for (i=0;i<strlen(TheCommand);i++)
		{
			ListDNAFCamac(SENDDNAF,TheCommand[i],CamacSlot,0,16);
			ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
		}


		ListDNAFCamac(SENDDNAF,13,CamacSlot,0,16);			// CR
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		ListDNAFCamac(SENDDNAF,10,CamacSlot,0,16);			// LF
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		ListDNAFCamac(SENDNAF,0,CamacSlot,0,26);

		ListDNAFCamac(SENDDNAF,UNL,CamacSlot,0,16);			// UNLISTEN
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		ListDNAFCamac(SENDDNAF,UNT,CamacSlot,0,16);			// UNTALK
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		ListDNAFCamac(SENDNAF,0,CamacSlot,0,24);

		ListDNAFCamac(SENDDNAF,0,CamacSlot,13,17);			// Write LAM-Mask (disable all)
		ListDNAFCamac(SENDF,0,0,0,40);						// Disable Interrupts

		ListStart();
		while (!GPIBReady()||!ListReady());
		ListReset();
		DataReset();
		return TRUE;
	}
	return FALSE;
}

CString SendReadGPIB(CString Command,int Address,int CamacSlot)
{
	char TheCommand[255];
	char TheAnswer[255];
	CString Answer = "Not successful";
	USHORT length;
	ULONG i;

	strcpy(TheCommand,Command);

	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
	if(!pMyApp->GlobalMeasurementRunning&&pMyApp->CamacOn)
	{
		while (!GPIBReady()||!ListReady());
		ListReset();
		DataReset();

		ListDNAFCamac(SENDNAF,0,CamacSlot,0,26);
		ListDNAFCamac(SENDNAF,0,CamacSlot,1,26);
		ListDNAFCamac(SENDNAF,0,CamacSlot,4,26);

		ListDNAFCamac(SENDDNAF,8,CamacSlot,13,17);	// Write LAM-Mask

		ListDNAFCamac(SENDF,0,0,0,32);				// Disable Inhibit


		ListDNAFCamac(SENDF,0,0,0,64+CamacSlot);	// Enable Interrupt from #CamacSlot

		ListDNAFCamac(SENDF,0,0,0,41);				// Enable Interrupts

		ListDNAFCamac(SENDDNAF,32+Address,CamacSlot,0,16);
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		ListDNAFCamac(SENDDNAF,64+21,CamacSlot,0,16);
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		ListDNAFCamac(SENDNAF,0,CamacSlot,0,24);

		for (i=0;i<strlen(TheCommand);i++)
		{
			ListDNAFCamac(SENDDNAF,TheCommand[i],CamacSlot,0,16);
			ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
		}


		ListDNAFCamac(SENDDNAF,13,CamacSlot,0,16);			// CR
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		ListDNAFCamac(SENDDNAF,10,CamacSlot,0,16);			// LF
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);


		/*************** Now listen to the device *****************/

		ListDNAFCamac(SENDNAF,0,CamacSlot,0,26);

		ListDNAFCamac(SENDDNAF,64+Address,CamacSlot,0,16);	// Device talks
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		ListDNAFCamac(SENDDNAF,32+21,CamacSlot,0,16);		// 3388 is listening
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		

		// this is new and hopefully works
		ListDNAFCamac(SENDDNAF,128,CamacSlot,13,17);		// Write LAM-Mask
		ListDNAFCamac(SENDNAF,0,CamacSlot,0,24);
		ListDNAFCamac(READGPIB,0,CamacSlot,0,0);		

		ListDNAFCamac(SENDDNAF,8,CamacSlot,13,17);			// Write LAM-Mask
	
		ListDNAFCamac(SENDNAF,0,CamacSlot,0,26);

		ListDNAFCamac(SENDDNAF,32+Address,CamacSlot,0,16);
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		ListDNAFCamac(SENDDNAF,64+21,CamacSlot,0,16);
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		
		ListDNAFCamac(SENDDNAF,UNL,CamacSlot,0,16);			// UNLISTEN
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		ListDNAFCamac(SENDDNAF,UNT,CamacSlot,0,16);			// UNTALK
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);

		ListDNAFCamac(SENDNAF,0,CamacSlot,0,24);
		ListDNAFCamac(SENDDNAF,0,CamacSlot,13,17);
		ListDNAFCamac(READENCL,0,0,0,0);
		

		ListStart();

		i=0;
		do i++;
		while (!GPIBReady()&&i<100000);
		if(i<100000)
		{
			ListReadS(&TheAnswer[0],&length);
			if(strlen(TheAnswer)>25)
			{
				strcpy(TheCommand,TheAnswer);
				strcpy(TheAnswer+1,TheCommand);
				strncpy(TheAnswer+25,"\n",1);
				strncpy(TheAnswer,TheCommand,25);
			}
			Answer = TheAnswer;
		}
		else 
		{
			Answer = "No answer";
		}

		ListReset();
		DataReset();
		return Answer;
	}
	return "Device busy";
}

BOOL ListSendReadGPIB(CString Command,int Address,int CamacSlot)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	USHORT i;
	char TheCommand[255];

	strcpy(TheCommand,Command);

	ListDNAFCamac(SENDNAF,0,CamacSlot,0,26);
	ListDNAFCamac(SENDNAF,0,CamacSlot,1,26);
	ListDNAFCamac(SENDNAF,0,CamacSlot,4,26);
	ListDNAFCamac(SENDDNAF,8,CamacSlot,13,17);	// Write LAM-Mask
	ListDNAFCamac(SENDF,0,0,0,32);				// Disable Inhibit
	ListDNAFCamac(SENDF,0,0,0,64+CamacSlot);	// Enable Interrupt from #CamacSlot
	ListDNAFCamac(SENDF,0,0,0,41);				// Enable Interrupts
	ListDNAFCamac(SENDDNAF,32+Address,CamacSlot,0,16);
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDDNAF,64+21,CamacSlot,0,16);
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDNAF,0,CamacSlot,0,24);
	for (i=0;i<strlen(TheCommand);i++)
	{
		ListDNAFCamac(SENDDNAF,TheCommand[i],CamacSlot,0,16);
		ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	}
	ListDNAFCamac(SENDDNAF,13,CamacSlot,0,16);			// CR
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDDNAF,10,CamacSlot,0,16);			// LF
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	/*************** Now listen to the device *****************/
	ListDNAFCamac(SENDNAF,0,CamacSlot,0,26);
	ListDNAFCamac(SENDDNAF,64+Address,CamacSlot,0,16);	// Device talks
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDDNAF,32+21,CamacSlot,0,16);		// 3388 is listening
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	// this is new and hopefully works
	ListDNAFCamac(SENDNAF,0,CamacSlot,0,0);
	ListDNAFCamac(SENDNAF,0,CamacSlot,0,24);
	ListDNAFCamac(SENDDNAF,128,CamacSlot,13,17);		// Write LAM-Mask
	ListDNAFCamac(READGPIB,0,CamacSlot,0,0);		
	ListDNAFCamac(SENDDNAF,8,CamacSlot,13,17);			// Write LAM-Mask
	ListDNAFCamac(SENDDNAF,32+Address,CamacSlot,0,16);
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDDNAF,64+21,CamacSlot,0,16);
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDNAF,0,CamacSlot,0,26);
	ListDNAFCamac(SENDDNAF,UNL,CamacSlot,0,16);			// UNLISTEN
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDDNAF,UNT,CamacSlot,0,16);			// UNTALK
	ListDNAFCamac(WAITINT | READENCL,0,0,0,0);
	ListDNAFCamac(SENDNAF,0,CamacSlot,0,24);
	ListDNAFCamac(SENDDNAF,0,CamacSlot,13,17);			// Write LAM-Mask (disable all)
	ListDNAFCamac(SENDF,0,0,0,128+CamacSlot);			// Disable Interrupt from #CamacSlot
	ListDNAFCamac(READENCL,0,0,0,0);
	return TRUE;
}

void OnBit(USHORT Slot,USHORT Bit)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(Slot==0)
	{
		pApp->OutRegister1 = pApp->OutRegister1|TwoTimesX(Bit);
		SendDNAFCamac(pApp->OutRegister1,pApp->GelbeboxSlot,0,16);
	}
	else
	{
		pApp->OutRegister2 = pApp->OutRegister2|TwoTimesX(Bit);
		SendDNAFCamac(pApp->OutRegister2,pApp->GelbeboxSlot,1,16);
	}
}

void OffBit(USHORT Slot,USHORT Bit)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(Slot==0)
	{
		pApp->OutRegister1 = pApp->OutRegister1&~TwoTimesX(Bit);
		SendDNAFCamac(pApp->OutRegister1,pApp->GelbeboxSlot,0,16);
	}
	else
	{
		pApp->OutRegister2 = pApp->OutRegister2&~TwoTimesX(Bit);
		SendDNAFCamac(pApp->OutRegister2,pApp->GelbeboxSlot,1,16);
	}
}

void ToggleBit(USHORT Slot,USHORT Bit)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(Slot==0)
	{
		pApp->OutRegister1 = pApp->OutRegister1^TwoTimesX(Bit);
		SendDNAFCamac(pApp->OutRegister1,pApp->GelbeboxSlot,0,16);
	}
	else
	{
		pApp->OutRegister2 = pApp->OutRegister2^TwoTimesX(Bit);
		SendDNAFCamac(pApp->OutRegister2,pApp->GelbeboxSlot,1,16);
	}
}

void ListOnBit(USHORT Slot,USHORT Bit)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(Slot==0)
	{
		pApp->OutRegister1 = pApp->OutRegister1|TwoTimesX(Bit);
		ListDNAFCamac(SENDDNAF,pApp->OutRegister1,pApp->GelbeboxSlot,0,16);
	}
	else
	{
		pApp->OutRegister2 = pApp->OutRegister2|TwoTimesX(Bit);
		ListDNAFCamac(SENDDNAF,pApp->OutRegister2,pApp->GelbeboxSlot,1,16);
	}
}

void ListOffBit(USHORT Slot,USHORT Bit)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(Slot==0)
	{
		pApp->OutRegister1 = pApp->OutRegister1&~TwoTimesX(Bit);
		ListDNAFCamac(SENDDNAF,pApp->OutRegister1,pApp->GelbeboxSlot,0,16);
	}
	else
	{
		pApp->OutRegister2 = pApp->OutRegister2&~TwoTimesX(Bit);
		ListDNAFCamac(SENDDNAF,pApp->OutRegister2,pApp->GelbeboxSlot,1,16);
	}
}

void ListToggleBit(USHORT Slot,USHORT Bit)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(Slot==0)
	{
		pApp->OutRegister1 = pApp->OutRegister1^TwoTimesX(Bit);
		ListDNAFCamac(SENDDNAF,pApp->OutRegister1,pApp->GelbeboxSlot,0,16);
	}
	else
	{
		pApp->OutRegister2 = pApp->OutRegister2^TwoTimesX(Bit);
		ListDNAFCamac(SENDDNAF,pApp->OutRegister2,pApp->GelbeboxSlot,1,16);
	}
}

void CamacReset()
{
	int i;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	ReadENCLCamac();
	SendFCamac(40);
	ReadENCLCamac();
	SendFCamac(33);
	ReadENCLCamac();
	SendFCamac(35);
	ReadENCLCamac();
	ReadCSRCamac();

	SendFCamac(41);
	ReadENCLCamac();
	SendFCamac(32);
	ReadENCLCamac();
	ReadCSRCamac();
	SendDNAFCamac((ULONG)~0,pApp->HpSlot,0,16);
	ReadCSRCamac();
	ReadENCLCamac();
	SendDNAFCamac((ULONG)~0,pApp->HpSlot,1,16);
	ReadCSRCamac();
	ReadENCLCamac();
	//long Data = (long)8388608;
	//SendNAFCamac(pApp->HvSlot,(UCHAR)1,(UCHAR)10);
	//ReadCSRCamac();
	//ReadENCLCamac();
	//SendDNAFCamac(Data,pApp->HvSlot,(UCHAR)0,(UCHAR)16);
	//ReadCSRCamac();
	//ReadENCLCamac();
	
	for(i=0;i<25;i++)
	{
		SendFCamac(128+i);
		ReadENCLCamac();
	}

	pApp->OutRegister1 = 0;
	SendDNAFCamac(pApp->OutRegister1,pApp->GelbeboxSlot,0,16);
	ReadCSRCamac();
	ReadENCLCamac();
	pApp->OutRegister2 = 0;
	SendDNAFCamac(pApp->OutRegister2,pApp->GelbeboxSlot,1,16);
	ReadCSRCamac();
	ReadENCLCamac();
	SendNAFCamac(pApp->GpibSlot,13,17);			// Write LAM-Mask (disable all)
	ReadENCLCamac();
	SendDNAFCamac(pApp->Gelbebox2Slot,0,0,16);
	ReadENCLCamac();
	ResetLAMTimer(pApp->PresetSlot);
	ReadENCLCamac();
	SendNAFCamac(pApp->Gelbebox2Slot,1,10);
	ReadENCLCamac();
	SendNAFCamac(pApp->PresetSlot,1,24);
	ReadENCLCamac();
	SendNAFCamac(pApp->PresetSlot,2,24);
	ReadENCLCamac();
	SendNAFCamac(pApp->HvSlot,(UCHAR)11,(UCHAR)28);
	ReadCSRCamac();
	ReadENCLCamac();
	SendNAFCamac(pApp->HvSlot,(UCHAR)8,(UCHAR)30);
	ReadCSRCamac();
	ReadENCLCamac();
	SendDNAFCamac(pApp->HvSlot,0,(UCHAR)0,(UCHAR)16);
	ReadCSRCamac();
	ReadENCLCamac();
	LoadTimer(pApp->PresetSlot,1.);
	StartTimer(pApp->PresetSlot);
}

void DelayCamac(USHORT delay)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	while (!GPIBReady()||!ListReady());
	ListReset();
	DataReset();

	SendFCamac(41);
	SendFCamac(32);
	
	SendFCamac(64+pApp->PresetSlot);
	EnableLAMTimer(pApp->PresetSlot);
	
	ListDelayCamac(pApp->PresetSlot,delay);
	ListStart();
	while (!ListReady());
}

