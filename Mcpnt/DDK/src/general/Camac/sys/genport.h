// genport.h    Include file for Generic Port I/O Example Driver
//
// Robert R. Howell         January 6, 1993
// Robert B. Nelson (MS)    January 12, 1993
//      Modified comments prior to posting to Compuserve
//      Changed Wkd* to Gpd*
// Robert B. Nelson (MS)    March 1, 1993
//      Cleanup and bug fixes for Beta 2
//
#include <ntddk.h>
#include <string.h>
#include <devioctl.h>
#include "gpioctl.h"        // Get IOCTL interface definitions

/* Default base port, and # of ports */
#define BASE_PORT       0x300
#define NUMBER_PORTS       16
#define INTERRUPT			5

#define bytemask 255
#define wordmask 65535

#define SENDF		(UCHAR)0x01
#define SENDNAF		(UCHAR)0x02
#define SENDDNAF	(UCHAR)0x03
#define READD		(UCHAR)0x04
#define READCSR		(UCHAR)0x05
#define READENCL	(UCHAR)0x06
#define READGPIB	(UCHAR)0x07
#define WAITINT		(UCHAR)0x10

#define COMPAREEQUAL		(UCHAR)0x20
#define COMPAREATLEAST		(UCHAR)0x40
#define COMPARELESSTHAN		(UCHAR)0x60

#define INSTRUCTIONMASK	(UCHAR)0x0f
#define INTERRUPTMASK	(UCHAR)0x10
#define COMPAREMASK		(UCHAR)0x60

#define DATAMEM			(ULONG) 32000
#define INSTRUCTIONMEM	(ULONG) 64000 
#define STRINGMEM		(ULONG) 100
#define STRINGLEN		(ULONG) 50

// NT device name
#define GPD_DEVICE_NAME L"\\Device\\Gpd0"

// File system device name.   When you execute a CreateFile call to open the
// device, use "\\.\GpdDev", or, given C's conversion of \\ to \, use
// "\\\\.\\GpdDev"

#define DOS_DEVICE_NAME L"\\DosDevices\\GpdDev"

typedef struct _ILIST
{
	USHORT type;
	ULONG d;
	UCHAR n;
	UCHAR a;
	UCHAR f;
} ILIST;

// driver local data structure specific to each device object
typedef struct _LOCAL_DEVICE_INFO {
    ULONG               DeviceType;     // Our private Device Type
    BOOLEAN             PortWasMapped;  // If TRUE, we have to unmap on unload
	BOOLEAN				IntWasMapped;
    PVOID               PortBase;       // base port address
    ULONG               PortCount;      // Count of I/O addresses used.
    ULONG               PortMemoryType; // HalTranslateBusAddress MemoryType
    PDEVICE_OBJECT      DeviceObject;   // The Gpd device object.
	PKINTERRUPT			InterruptObject;
	ULONG				InterruptCount;

	ULONG				ReadData[DATAMEM];
	ILIST				InstructionList[INSTRUCTIONMEM];
	char				ReadString[STRINGMEM][STRINGLEN];
	ULONG				WriteInstructionPtr;
	ULONG				ReadDataPtr;
	ULONG				ReadStringPtr;
	BOOLEAN				GPIBListening;
	BOOLEAN				GPIBPending;
	BOOLEAN				ListPending;

	ULONG				ActualInstructionPtr;
	ULONG				ActualDataPtr;
	ULONG				ActualStringPtr;
	ULONG				ActualStringPosPtr;
	USHORT				GPIBAddress;

	BOOLEAN				IntInProgress;
	HANDLE				Handle; 
	PKEVENT				Event;

} LOCAL_DEVICE_INFO, *PLOCAL_DEVICE_INFO;

/********************* function prototypes ***********************************/
//
UCHAR		ReadENCL(		   PLOCAL_DEVICE_INFO extension );
 
UCHAR		ReadCSR(		   PLOCAL_DEVICE_INFO extension );

ULONG		ReadD(			   PLOCAL_DEVICE_INFO extension );

VOID		SendDNAF(		   PLOCAL_DEVICE_INFO extension,
							   ULONG d,
							   ULONG n,
							   ULONG a,
							   ULONG f	);

VOID		SendNAF(		   PLOCAL_DEVICE_INFO extension,
							   ULONG n,
							   ULONG a,
							   ULONG f  );

VOID		SendF(			   PLOCAL_DEVICE_INFO extension,
							   ULONG f	);

VOID		GpdDpcRoutine(	   IN PKDPC Dpc,
							   IN PVOID DeferredContext,
							   IN PVOID SystemArgument1,
							   IN PVOID SystemArgument2    );

BOOLEAN		GpdInterruptServiceRoutine( 
							   IN PKINTERRUPT Interrupt,
							   IN OUT PVOID Context    );

NTSTATUS    DriverEntry(       IN  PDRIVER_OBJECT DriverObject,
                               IN  PUNICODE_STRING RegistryPath );

NTSTATUS    GpdCreateDevice(   IN  PWSTR szPrototypeName,
                               IN  DEVICE_TYPE DeviceType,
                               IN  PDRIVER_OBJECT DriverObject,
                               OUT PDEVICE_OBJECT *ppDevObj     );

NTSTATUS    GpdDispatch(       IN  PDEVICE_OBJECT pDO,
                               IN  PIRP pIrp                    );

NTSTATUS    GpdIoctlReadPort(  IN  PLOCAL_DEVICE_INFO pLDI,
                               IN  PIRP pIrp,
                               IN  PIO_STACK_LOCATION IrpStack,
                               IN  ULONG IoctlCode              );

NTSTATUS    GpdIoctlWritePort( IN  PLOCAL_DEVICE_INFO pLDI,
                               IN  PIRP pIrp,
                               IN  PIO_STACK_LOCATION IrpStack,
                               IN  ULONG IoctlCode              );

VOID        GpdUnload(         IN  PDRIVER_OBJECT DriverObject );

