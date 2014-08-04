//CAMAC.DLL Ansteuerungslibrary f�r CAMAC unter NT


 
#include "stdafx.h"
#include "MCP for NT.h"
#include "afx.h"
#include <stddef.h>
#include <winioctl.h>
#include "camacdef.h"
#include "camac.h"
#include "hardware.h"

#define bytemask 255
#define wordmask 65535

// *************** HYTEC 1330,1331 : **************/
#define data_low 0
#define data_mid 2
#define data_hi  4
#define CSRPort  6
#define ENCLPort 8
#define aPort    6
#define nPort    8
#define fPort   10
#define repPort 12

static HANDLE	hndCamac;
static BOOLEAN	DriverFound	= FALSE;

//*****************  Die internen Funktionen  ************************//

ULONG ReadLongIO(ULONG adr)
{
   BOOL    IoctlResult;
   ULONG   LongData;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_GPD_READ_PORT_ULONG,
                            &adr,						// Buffer to driver.
                            (DWORD)4,					// Length of buffer in bytes.
                            &LongData,					// Buffer from driver.
                            (DWORD)4,					// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
	return LongData;
}

USHORT SendLongIO(ULONG adr,ULONG data)
{
 
	BOOL				IoctlResult;
    GENPORT_WRITE_INPUT_LONG
						InputBuffer;    // Input buffer for DeviceIoControl
    ULONG               ReturnedLength; // Number of bytes returned in output buffer

	InputBuffer.LongData	= data;
	InputBuffer.PortNumber	= adr;
	
	IoctlResult = DeviceIoControl(
                        hndCamac,						// Handle to device
                        (ULONG)IOCTL_GPD_WRITE_PORT_ULONG,		// IO Control code for Write
                        &InputBuffer,					// Buffer to driver.  Holds port & data.
                        8,  							// Length of buffer in bytes.
                        NULL,							// Buffer from driver.   Not used.
                        0,								// Length of buffer in bytes.
                        &ReturnedLength,				// Bytes placed in outbuf.  Should be 0.
                        NULL							// NULL means wait till I/O completes.
                        );

	return IoctlResult;
}

USHORT ReadWordIO(ULONG adr)
{
   BOOL    IoctlResult;
   USHORT  ShortData;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_GPD_READ_PORT_USHORT,
                            &adr,						// Buffer to driver.
                            4,							// Length of buffer in bytes.
                            &ShortData,					// Buffer from driver.
                            2,							// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
	return ShortData;
}

USHORT SendWordIO(ULONG adr,USHORT data)
{
    BOOL				IoctlResult;
    GENPORT_WRITE_INPUT_SHORT
						InputBuffer;    // Input buffer for DeviceIoControl
    ULONG               ReturnedLength; // Number of bytes returned in output buffer


	InputBuffer.ShortData	= data;
	InputBuffer.PortNumber	= adr;

	IoctlResult = DeviceIoControl(
                        hndCamac,						// Handle to device
                        (ULONG)IOCTL_GPD_WRITE_PORT_USHORT,	// IO Control code for Write
                        &InputBuffer,					// Buffer to driver.  Holds port & data.
                        (ULONG)6,						// Length of buffer in bytes.
                        NULL,							// Buffer from driver.   Not used.
                        (ULONG)0,						// Length of buffer in bytes.
                        &ReturnedLength,				// Bytes placed in outbuf.  Should be 0.
                        NULL							// NULL means wait till I/O completes.
                        );

	return IoctlResult;
}

UCHAR ReadByteIO(ULONG adr)
{
   BOOL    IoctlResult;
   UCHAR   CharData;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_GPD_READ_PORT_UCHAR,
                            &adr,						// Buffer to driver.
                            4,							// Length of buffer in bytes.
                            &CharData,					// Buffer from driver.
                            1,							// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
	return CharData;
}

USHORT SendByteIO(ULONG adr,UCHAR data)
{
    BOOL				IoctlResult;
    GENPORT_WRITE_INPUT_BYTE
						InputBuffer;    // Input buffer for DeviceIoControl
    ULONG               ReturnedLength; // Number of bytes returned in output buffer

	InputBuffer.CharData	= data;
	InputBuffer.PortNumber	= adr;

	IoctlResult = DeviceIoControl(
                        hndCamac,						// Handle to device
                        (ULONG)IOCTL_GPD_WRITE_PORT_UCHAR,		// IO Control code for Write
                        &InputBuffer,					// Buffer to driver.  Holds port & data.
                        5,								// Length of buffer in bytes.
                        NULL,							// Buffer from driver.   Not used.
                        0,								// Length of buffer in bytes.
                        &ReturnedLength,				// Bytes placed in outbuf.  Should be 0.
                        NULL							// NULL means wait till I/O completes.
                        );

	return IoctlResult;
}



/****************  Die exportierten Funktionen  *********************/


/****************** List-Befehle  ***********************************/
BOOL	  GPIBReady(void)
{
   BOOL    IoctlResult;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_GPIBREADY,
                            NULL,						// Buffer to driver.
                            (ULONG)0,					// Length of buffer in bytes.
                            NULL,						// Buffer from driver.
                            (ULONG)0,					// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
	return IoctlResult;
}

BOOL    ListReady(void)
{
   BOOL    IoctlResult;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_LISTREADY,
                            NULL,						// Buffer to driver.
                            (ULONG)0,					// Length of buffer in bytes.
                            NULL,						// Buffer from driver.
                            (ULONG)0,					// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
	return IoctlResult;
}

ULONG   ShowStringPtr(void)
{
   BOOL    IoctlResult;
   ULONG   LongData;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_SHOWSTRINGPTR,
                            NULL,						// Buffer to driver.
                            (ULONG)0,					// Length of buffer in bytes.
                            &LongData,					// Buffer from driver.
                            (ULONG)4,					// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
	return LongData;
}

ULONG   ShowActualStringPtr(void)
{
   BOOL    IoctlResult;
   ULONG   LongData;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_SHOWACTUALSTRINGPTR,
                            NULL,						// Buffer to driver.
                            (ULONG)0,					// Length of buffer in bytes.
                            &LongData,					// Buffer from driver.
                            (ULONG)4,					// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
	return LongData;
}

ULONG  ShowListPtr(void)
{
   BOOL    IoctlResult;
   ULONG   LongData;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_SHOWLISTPTR,
                            NULL,						// Buffer to driver.
                            (ULONG)0,					// Length of buffer in bytes.
                            &LongData,					// Buffer from driver.
                            (ULONG)4,					// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
	return LongData;
}

ULONG  ShowActualListPtr(void)
{
   BOOL    IoctlResult;
   ULONG   LongData;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_SHOWACTUALLISTPTR,
                            NULL,						// Buffer to driver.
                            (ULONG)0,					// Length of buffer in bytes.
                            &LongData,					// Buffer from driver.
                            (ULONG)4,					// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
	return LongData;
}

ULONG  ShowDataPtr(void)
{
   BOOL    IoctlResult;
   ULONG   LongData;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_SHOWDATAPTR,
                            NULL,						// Buffer to driver.
                            (ULONG)0,					// Length of buffer in bytes.
                            &LongData,					// Buffer from driver.
                            (ULONG)4,					// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
	return LongData;
}

ULONG  ShowActualDataPtr(void)
{
   BOOL    IoctlResult;
   ULONG   LongData;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_SHOWACTUALDATAPTR,
                            NULL,						// Buffer to driver.
                            (ULONG)0,					// Length of buffer in bytes.
                            &LongData,					// Buffer from driver.
                            (ULONG)4,					// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
	return LongData;
}

ULONG  ListDNAFCamac(ULONG type,ULONG d,UCHAR n,UCHAR a, UCHAR f)
{
	typedef struct _BUFFER
	{
		ULONG type;
		ULONG d;
		ULONG n;
		ULONG a;
		ULONG f;
	}BUFFER;

	BOOL	IoctlResult;
	BUFFER	InputBuffer;
	ULONG	OutputBuffer;

	InputBuffer.type = type;
	InputBuffer.d = d;
	InputBuffer.n = n;
	InputBuffer.a = a;
	InputBuffer.f = f;
	ULONG ReturnedLength;
	IoctlResult = DeviceIoControl(
                        hndCamac,						// Handle to device
                        (ULONG)IOCTL_LISTDNAF,					// IO Control code for Write
                        &InputBuffer,					// Buffer to driver.  Holds port & data.
                        20,								// Length of buffer in bytes.
                        &OutputBuffer,					// Buffer from driver.   Not used.
                        4,								// Length of buffer in bytes.
                        &ReturnedLength,				// Bytes placed in outbuf.  Should be 0.
                        NULL							// NULL means wait till I/O completes.
                        );	
	return OutputBuffer;
}

USHORT  ListStart(void)
{
	ULONG ReturnedLength;
	return DeviceIoControl(
                        hndCamac,						// Handle to device
                        (ULONG)IOCTL_LISTSTART,				// IO Control code for Write
                        NULL,							// Buffer to driver.  Holds port & data.
                        (ULONG)0,								// Length of buffer in bytes.
                        NULL,							// Buffer from driver.   Not used.
                        (ULONG)0,								// Length of buffer in bytes.
                        &ReturnedLength,				// Bytes placed in outbuf.  Should be 0.
                        NULL							// NULL means wait till I/O completes.
                        );
}

USHORT  ListReset(void)
{
	ULONG ReturnedLength;
	return DeviceIoControl(
                        hndCamac,						// Handle to device
                        (ULONG)IOCTL_LISTPTRRESET,				// IO Control code for Write
                        NULL,							// Buffer to driver.  Holds port & data.
                        (ULONG)0,								// Length of buffer in bytes.
                        NULL,							// Buffer from driver.   Not used.
                        (ULONG)0,								// Length of buffer in bytes.
                        &ReturnedLength,				// Bytes placed in outbuf.  Should be 0.
                        NULL							// NULL means wait till I/O completes.
                        );
}

USHORT  DataReset(void)
{
	ULONG ReturnedLength;
	return DeviceIoControl(
                        hndCamac,						// Handle to device
                        (ULONG)IOCTL_DATAPTRRESET,				// IO Control code for Write
                        NULL,							// Buffer to driver.  Holds port & data.
                        (ULONG)0,								// Length of buffer in bytes.
                        NULL,							// Buffer from driver.   Not used.
                        (ULONG)0,								// Length of buffer in bytes.
                        &ReturnedLength,				// Bytes placed in outbuf.  Should be 0.
                        NULL							// NULL means wait till I/O completes.
                        );
}

ULONG   ListReadS(char* data, USHORT* length)
{
   BOOL    IoctlResult;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_GPIBREADSTRING,
                            NULL,						// Buffer to driver.
                            (ULONG)0,					// Length of buffer in bytes.
                            (LPVOID)data,				// Buffer from driver.
                            (ULONG)50,					// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
   *length = (USHORT)ReturnedLength;
   if(ReturnedLength>1)*(data+ReturnedLength) = 0;
	return IoctlResult;
}


ULONG  ListReadd(ULONG* data)
{
   BOOL    IoctlResult;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_LISTREADD,
                            NULL,						// Buffer to driver.
                            (ULONG)0,					// Length of buffer in bytes.
                            data,						// Buffer from driver.
                            (ULONG)4,					// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
	return IoctlResult;
}



/****************** Ende der List-Befehle  **********************/




ULONG  ReadLongIOCamac(ULONG adr)
{	
	return ReadLongIO(adr);
}

USHORT  SendLongIOCamac(ULONG adr,ULONG data)
{
	return SendLongIO(adr,data);
}

USHORT  ReadWordIOCamac(ULONG adr)
{
	return ReadWordIO(adr);
}

USHORT  SendWordIOCamac(ULONG adr,USHORT data)
{
	return SendWordIO(adr,data);
}

UCHAR  ReadByteIOCamac(ULONG adr)
{
	return ReadByteIO(adr);
}

USHORT  SendByteIOCamac(ULONG adr,UCHAR data)
{
	return SendByteIO(adr,data);
}

USHORT  SendDCamac(ULONG d)
{
		   SendLongIO(0,(d & bytemask)+(((d>>8) & bytemask)<<16));
	return SendByteIO(4,(UCHAR)(d>>16));
}

USHORT  SendDNAFCamac(ULONG d,UCHAR n,UCHAR a, UCHAR f)
{
	typedef struct _BUFFER
	{
		ULONG d;
		ULONG n;
		ULONG a;
		ULONG f;
	}BUFFER;

	BUFFER InputBuffer;
	InputBuffer.d = d;
	InputBuffer.n = n;
	InputBuffer.a = a;
	InputBuffer.f = f;
	ULONG ReturnedLength;
	return DeviceIoControl(
                        hndCamac,						// Handle to device
                        (ULONG)IOCTL_SENDDNAF,					// IO Control code for Write
                        &InputBuffer,					// Buffer to driver.  Holds port & data.
                        16,								// Length of buffer in bytes.
                        NULL,							// Buffer from driver.   Not used.
                        0,								// Length of buffer in bytes.
                        &ReturnedLength,				// Bytes placed in outbuf.  Should be 0.
                        NULL							// NULL means wait till I/O completes.
                        );	
}


USHORT  SendNAFCamac(UCHAR n,UCHAR a, UCHAR f)
{
	typedef struct _BUFFER
	{
		ULONG n;
		ULONG a;
		ULONG f;
	}BUFFER;

	BUFFER InputBuffer;
	InputBuffer.n = n;
	InputBuffer.a = a;
	InputBuffer.f = f;
	ULONG ReturnedLength;
	return DeviceIoControl(
                        hndCamac,						// Handle to device
                        (ULONG)IOCTL_SENDNAF,					// IO Control code for Write
                        &InputBuffer,					// Buffer to driver.  Holds port & data.
                        12,								// Length of buffer in bytes.
                        NULL,							// Buffer from driver.   Not used.
                        0,								// Length of buffer in bytes.
                        &ReturnedLength,				// Bytes placed in outbuf.  Should be 0.
                        NULL							// NULL means wait till I/O completes.
                        );
}

USHORT  SendFCamac(UCHAR f)
{
	UCHAR InputBuffer = f;
	ULONG ReturnedLength;
	return DeviceIoControl(
                        hndCamac,						// Handle to device
                        (ULONG)IOCTL_SENDF,					// IO Control code for Write
                        &InputBuffer,					// Buffer to driver.  Holds port & data.
                        1,								// Length of buffer in bytes.
                        NULL,							// Buffer from driver.   Not used.
                        0,								// Length of buffer in bytes.
                        &ReturnedLength,				// Bytes placed in outbuf.  Should be 0.
                        NULL							// NULL means wait till I/O completes.
                        );
}

USHORT  RepeatLastCamac(void)
{
	return SendByteIO(repPort,1);
}

ULONG  ReadDCamac(void)
{
   BOOL    IoctlResult;
   ULONG   LongData;
   ULONG   adr;
   DWORD   ReturnedLength;
   
   IoctlResult = DeviceIoControl(
                            hndCamac,					// Handle to device
                            (ULONG)IOCTL_READD,
                            &adr,						// Buffer to driver.
                            4,					// Length of buffer in bytes.
                            &LongData,					// Buffer from driver.
                            (ULONG)4,					// Length of buffer in bytes.
                            &ReturnedLength,			// Bytes placed in DataBuffer.
                            NULL						// NULL means wait till op. completes.
                            );
	return LongData;
}

UCHAR  ReadENCLCamac(void)
{
	return ReadByteIO(ENCLPort);
}

UCHAR  ReadCSRCamac(void)
{
	return ReadByteIO(CSRPort);
}

USHORT  LAMEnableCamac(UCHAR a)
{
	return SendByteIO(fPort,64+a);
}

USHORT  LAMDisableCamac(UCHAR a)
{
	return SendByteIO(fPort,128+a);
}

UCHAR  GetLAMCamac(UCHAR a)
{
	SendByteIO(fPort,192+a);
	return ReadByteIO(ENCLPort);
}

ULONG   ListDelayCamac(USHORT slot, USHORT delay)
{
	ListResetLAMTimer(slot);
	ListDNAFCamac(READENCL,0,0,0,0);

	ListLoadTimer(slot,delay);
	ListStartTimer(slot);
	
	ListResetLAMTimer(slot);
	ListDNAFCamac(READENCL,0,0,0,0);
	return TRUE;
}

/***************** Device Testfunctions **********************/

BOOL  DriverIsExisting(void)
{
	const UCHAR Test = 55;
	SendDNAFCamac(Test,Test,Test,Test);
	if(Test==ReadByteIO((ULONG)10)) return DriverFound;
	else return FALSE;
}


/**********************    Main (Start und Ende der DLL *****************/

BOOL  __stdcall DllMain(HINSTANCE hinstDll,DWORD fwdReason,LPVOID lpvReserved)
{
	switch (fwdReason)
	{
	case DLL_PROCESS_ATTACH:
		hndCamac = CreateFile(
					"\\\\.\\GpdDev",                    // Open the Device "file"
					GENERIC_READ|GENERIC_WRITE,
					0,
	                NULL,
		            OPEN_EXISTING,
			        0,
				    NULL
					);
               
		if (hndCamac == INVALID_HANDLE_VALUE)        // Was the device opened?
		{	
			hndCamac	= NULL;
			DriverFound = FALSE;
		}
		else
		{
			DriverFound = TRUE;
		}
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_PROCESS_DETACH:
		if (DriverFound)
		{
		  if (!CloseHandle(hndCamac))                  // Close the Device "file".
			{
			    hndCamac=NULL;
				exit(1);
			}
		}
		break;

	case DLL_THREAD_DETACH:
		break;

	};
	return TRUE;
}


/* Old Timer
ULONG   ListDelayCamac(USHORT slot, USHORT delay)
{
	double ticks;
	ULONG  divisor;
	ULONG  fac;

	ticks = 10e3*delay;
	ticks = ticks/65536;
	
	if(ticks>32768)
	{
		divisor = 262144;
		fac		= 64;
	}
	if(ticks<=32768 && ticks>4096)
	{
		divisor = 32768;
		fac		= 32;
	}
	if(ticks<=4096 && ticks>512)
	{
		divisor = 4096;
		fac		= 16;
	}
	if(ticks<=512 && ticks>64)
	{
		divisor = 512;
		fac		= 8;
	}
	if(ticks<=64 && ticks>8)
	{
		divisor = 64;
		fac		= 4;
	}
	if(ticks<=8 && ticks>1)
	{
		divisor = 8;
		fac		= 2;
	}
	if(ticks<=1)
	{
		divisor = 1;
		fac		= 1;
	}
	ListDNAFCamac(SENDDNAF,fac,(UCHAR)slot,(UCHAR)1,(UCHAR)16);
	
	ListDNAFCamac(SENDNAF,0,(UCHAR)slot,(UCHAR)0,(UCHAR)10);
	ListDNAFCamac(READENCL,0,0,0,0);

	ListDNAFCamac(SENDDNAF,(USHORT)(delay*10e3/divisor),(UCHAR)slot,(UCHAR)0,(UCHAR)16);
	ListDNAFCamac(SENDNAF|WAITINT,0,(UCHAR)slot,(UCHAR)0,(UCHAR)28);
	
	ListDNAFCamac(SENDNAF,0,(UCHAR)slot,(UCHAR)0,(UCHAR)10);
	ListDNAFCamac(READENCL,0,0,0,0);
	return TRUE;
}
*/