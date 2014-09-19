/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

    PciDrv.h

Abstract:

    Header file for the PCIDRV driver modules.

Environment:

    Kernel mode

--*/


#if !defined(_PCIDRV_H_)
#define _PCIDRV_H_

//
// Let us use newly introduced (.NET DDK) safe string function to avoid
// security issues related buffer overrun.
// The advantages of the RtlStrsafe functions include:
// 1) The size of the destination buffer is always provided to the
// function to ensure that the function does not write past the end of
// the buffer.
// 2) Buffers are guaranteed to be null-terminated, even if the
// operation truncates the intended result.
//

//
// In this driver we are using a safe version vsnprintf, which is
// RtlStringCbVPrintfA. To use strsafe function on 9x, ME, and Win2K Oses, we
// have to define NTSTRSAFE_LIB before including this header file and explicitly
// link to ntstrsafe.lib. If your driver is just target for XP and above, there is
// no define NTSTRSAFE_LIB and link to the lib.
//
#define NTSTRSAFE_LIB
#include <ntstrsafe.h>

//-----------------------------------------------------------------------------
// 4127 -- Conditional Expression is Constant warning
//-----------------------------------------------------------------------------
#define WHILE(constant) \
__pragma(warning(suppress: 4127)) while(constant)

#define _DRIVER_NAME_ "PCIDRV"

#define PCIDRV_POOL_TAG (ULONG) 'DICP'
#define PCIDRV_FDO_INSTANCE_SIGNATURE (ULONG) 'odFT'

#define MILLISECONDS_TO_100NS   (10000)
#define SECOND_TO_MILLISEC      (1000)
#define SECOND_TO_100NS         (SECOND_TO_MILLISEC * MILLISECONDS_TO_100NS)

//
// CAMAC related defines
//

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

//
// CAMAC related typedefs
//
typedef struct _ILIST
{
	USHORT type;
	ULONG d;
	UCHAR n;
	UCHAR a;
	UCHAR f;
} ILIST;

//
// Bit Flag Macros
//

#define SET_FLAG(Flags, Bit)    ((Flags) |= (Bit))
#define CLEAR_FLAG(Flags, Bit)  ((Flags) &= ~(Bit))
#define TEST_FLAG(Flags, Bit)   (((Flags) & (Bit)) != 0)

//
// The driver context contains global data to the whole driver.
//
typedef struct _DRIVER_CONTEXT {
    //
    // The assumption here is that there is nothing device specific in the lookaside list
    // and hence the same list can be used to do allocations for multiple devices.
    //
    WDFLOOKASIDE            RecvLookaside;

} DRIVER_CONTEXT, * PDRIVER_CONTEXT;
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DRIVER_CONTEXT, GetDriverContext)



//
// Connector Types
//

typedef struct _PCIDRV_WMI_STD_DATA {

    //
    // Current Mac Address of the NIC
    //

    UINT64  MacAddress;

} PCIDRV_WMI_STD_DATA, * PPCIDRV_WMI_STD_DATA;

//
// The device extension for the device object
//
typedef struct _FDO_DATA
{
    ULONG                   Signature;       // must be PCIDRV_FDO_INSTANCE_SIGNATURE
                                             // beneath this device object.
    WDFDEVICE               WdfDevice;

    // Power Management 
	WDF_POWER_DEVICE_STATE      DevicePowerState;   // Current power state of the device(D0 - D3)

    // Wait-Wake
    BOOLEAN                 AllowWakeArming;

    // Idle Detection
    //BOOLEAN                 IdleDetectionEnabled;
    PCIDRV_WMI_STD_DATA     StdDeviceData;

	// Following fields are specific to the hardware
	BUS_INTERFACE_STANDARD  BusInterface;
	WDFQUEUE                IoctlQueue;
	PVOID					PortBase;       // base port address

	WDFINTERRUPT            WdfInterrupt;
	ULONG					InterruptCount;

	// CAMAC interface related
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

}  FDO_DATA, *PFDO_DATA;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(FDO_DATA, FdoGetData)

#define CLRMASK(x, mask)     ((x) &= ~(mask));
#define SETMASK(x, mask)     ((x) |=  (mask));


//
// Function prototypes
//
DRIVER_INITIALIZE DriverEntry;

EVT_WDF_DRIVER_DEVICE_ADD HytecPCIDeviceAdd;

EVT_WDF_OBJECT_CONTEXT_CLEANUP HytecPCIDriverContextCleanup;
EVT_WDF_DEVICE_CONTEXT_CLEANUP HytecPCIDeviceContextCleanup;

EVT_WDF_DEVICE_D0_ENTRY HytecPCIDeviceD0Entry;
EVT_WDF_DEVICE_D0_EXIT HytecPCIDeviceD0Exit;
EVT_WDF_DEVICE_PREPARE_HARDWARE HytecPCIDevicePrepareHardware;
EVT_WDF_DEVICE_RELEASE_HARDWARE HytecPCIDeviceReleaseHardware;

EVT_WDF_DEVICE_SELF_MANAGED_IO_CLEANUP HytecPCIDeviceSelfManagedIoCleanup;
EVT_WDF_DEVICE_SELF_MANAGED_IO_INIT HytecPCIDeviceSelfManagedIoInit;
EVT_WDF_DEVICE_SELF_MANAGED_IO_SUSPEND HytecPCIDeviceSelfManagedIoSuspend;
EVT_WDF_DEVICE_SELF_MANAGED_IO_RESTART HytecPCIDeviceSelfManagedIoRestart;

EVT_WDF_DEVICE_ARM_WAKE_FROM_S0 HytecPCIDeviceWakeArmS0;
EVT_WDF_DEVICE_ARM_WAKE_FROM_SX HytecPCIDeviceWakeArmSx;
EVT_WDF_DEVICE_DISARM_WAKE_FROM_S0 HytecPCIDeviceWakeDisarmS0;
EVT_WDF_DEVICE_DISARM_WAKE_FROM_SX HytecPCIDeviceWakeDisarmSx;
EVT_WDF_DEVICE_WAKE_FROM_S0_TRIGGERED HytecPCIDeviceWakeTriggeredS0;
EVT_WDF_DEVICE_WAKE_FROM_SX_TRIGGERED HytecPCIDeviceWakeTriggeredSx;

EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL HytecPCIIoDeviceControl;

VOID SendDNAF(PFDO_DATA fdoData, ULONG d, ULONG n, ULONG a, ULONG f);
VOID SendNAF(PFDO_DATA fdoData, ULONG n, ULONG a, ULONG f);
VOID SendF(PFDO_DATA fdoData, ULONG f);
ULONG ReadD(PFDO_DATA fdoData);
UCHAR ReadENCL(PFDO_DATA fdoData);
UCHAR ReadCSR(PFDO_DATA fdoData);

BOOLEAN
HytecPCIInterruptIsr(
IN WDFINTERRUPT Interrupt,
IN ULONG        MessageID
);

VOID
HytecPCIInterruptDpc(
IN WDFINTERRUPT WdfInterrupt,
IN WDFOBJECT    WdfDevice
);

NTSTATUS
HytecPCIInterruptEnable(
IN WDFINTERRUPT  Interrupt,
IN WDFDEVICE     AssociatedDevice
);

NTSTATUS
HytecPCIInterruptDisable(
IN WDFINTERRUPT  Interrupt,
IN WDFDEVICE     AssociatedDevice
);

NTSTATUS
PciDrvReturnResources (
    IN OUT PFDO_DATA FdoData
    );

NTSTATUS
PciDrvSetPowerPolicy(
    IN PFDO_DATA FdoData
    );

PCHAR
DbgDevicePowerString(
    IN WDF_POWER_DEVICE_STATE Type
    );


BOOLEAN
PciDrvReadFdoRegistryKeyValue(
    _In_  PWDFDEVICE_INIT  DeviceInit,
    _In_  PWSTR            Name,
    _Out_ PULONG           Value
    );

#if defined(WIN2K)

NTKERNELAPI
VOID
ExFreePoolWithTag(
    _In_ PVOID P,
    _In_ ULONG Tag
    );

#endif

#endif  // _PCIDRV_H_


