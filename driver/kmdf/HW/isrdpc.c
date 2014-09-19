/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

    ISRDPC.C

Abstract:

    Contains routine to handle interrupts, interrupt DPCs and WatchDogTimer DPC

Environment:

    Kernel mode

--*/

#include "precomp.h"

#if 0
#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, NICEvtDeviceD0ExitPreInterruptsDisabled)
#endif
#endif

BOOLEAN
HytecPCIInterruptIsr(
    IN WDFINTERRUPT Interrupt,
    IN ULONG        MessageID
    )
/*++
Routine Description:

    Interrupt handler for the device.

Arguments:

    Interupt - Address of the framework interrupt object
    MessageID -

Return Value:

     TRUE if our device is interrupting, FALSE otherwise.

--*/
{
    BOOLEAN    InterruptRecognized = TRUE;
    PFDO_DATA  FdoData = NULL;
	UCHAR TheChar;
	UCHAR InstructionType;
	ULONG Index;

    UNREFERENCED_PARAMETER( MessageID );

	//DbgPrint("--> HytecPCIInterruptIsr\n");

    FdoData = FdoGetData(WdfInterruptGetDevice(Interrupt));

	

	//
	// device specific stuff to dismiss the interrupt
	//

	//SendF(FdoData, 40);

	if (FdoData->GPIBListening)
	{
		SendNAF(FdoData, FdoData->GPIBAddress, 0, 0);
		ReadCSR(FdoData);
		TheChar = (UCHAR)ReadD(FdoData);
		ReadENCL(FdoData);


		if (TheChar != 10 && TheChar != 13)FdoData->ReadString[FdoData->ActualStringPtr][FdoData->ActualStringPosPtr] = TheChar;
		else FdoData->ReadString[FdoData->ActualStringPtr][FdoData->ActualStringPosPtr] = 32;

		if (TheChar == 10 || (FdoData->ActualStringPosPtr == STRINGLEN))
		{
			FdoData->ReadString[FdoData->ActualStringPtr][0] = (char)FdoData->ActualStringPosPtr;
			FdoData->GPIBListening = FALSE;
			FdoData->GPIBPending = FALSE;
			FdoData->ActualStringPosPtr = 1;
			FdoData->ActualStringPtr++;
			if (FdoData->ActualStringPtr >= STRINGMEM)FdoData->ActualStringPtr = 0;
		}
		else FdoData->ActualStringPosPtr++;
	}
	if (!FdoData->GPIBListening)
	{
		while ((FdoData->ActualInstructionPtr < FdoData->WriteInstructionPtr) &&
			(FdoData->ActualInstructionPtr < INSTRUCTIONMEM))
		{
			Index = FdoData->ActualInstructionPtr;
			InstructionType = FdoData->InstructionList[Index].type & INSTRUCTIONMASK;
			switch (InstructionType)
			{
			case SENDF:
				SendF(FdoData, FdoData->InstructionList[Index].f);
				ReadCSR(FdoData);
				break;
			case SENDNAF:
				SendNAF(FdoData, FdoData->InstructionList[Index].n,
					FdoData->InstructionList[Index].a,
					FdoData->InstructionList[Index].f);
				ReadCSR(FdoData);
				break;
			case SENDDNAF:
				SendDNAF(FdoData, FdoData->InstructionList[Index].d,
					FdoData->InstructionList[Index].n,
					FdoData->InstructionList[Index].a,
					FdoData->InstructionList[Index].f);
				ReadCSR(FdoData);
				break;
			case READD:
				if (FdoData->ActualDataPtr < DATAMEM)
				{
					FdoData->ReadData[FdoData->ActualDataPtr] = ReadD(FdoData);
					FdoData->ActualDataPtr++;
				}
				break;
			case READCSR:
				ReadCSR(FdoData);
				break;
			case READENCL:
				ReadENCL(FdoData);
				break;
			case READGPIB:
				ReadENCL(FdoData);
				FdoData->GPIBAddress = FdoData->InstructionList[Index].n;
				FdoData->GPIBListening = TRUE;
				break;
			}
			FdoData->ActualInstructionPtr++;
			if (((FdoData->InstructionList[Index].type & WAITINT) != 0) || FdoData->GPIBListening) break;
		}
		if (FdoData->ActualInstructionPtr < FdoData->WriteInstructionPtr)
			FdoData->IntInProgress = TRUE;
		else
		{
			FdoData->IntInProgress = FALSE;
			FdoData->ListPending = FALSE;
		}
	}

	//WdfInterruptQueueDpcForIsr(Interrupt);

	//DbgPrint("<-- HytecPCIInterruptIsr\n");

    return InterruptRecognized;
}

VOID
HytecPCIInterruptDpc(
    IN WDFINTERRUPT WdfInterrupt,
    IN WDFOBJECT    WdfDevice
    )

/*++

Routine Description:

    DPC callback for ISR.

Arguments:

    WdfInterrupt - Handle to the framework interrupt object

    WdfDevice - Associated device object.

Return Value:

--*/
{
    PFDO_DATA fdoData = NULL;

	UNREFERENCED_PARAMETER(WdfInterrupt);

	DbgPrint("--> HytecPCIInterruptDpc\n");

    fdoData = FdoGetData(WdfDevice);

    //
    // Re-enable the interrupt (disabled in MPIsr)
    //
#if 0
    WdfInterruptSynchronize(
        WdfInterrupt,
		HytecPCIEnableInterrupt,
        fdoData);
#endif

	DbgPrint("<-- HytecPCIInterruptDpc\n");

}

NTSTATUS
HytecPCIInterruptEnable(
    IN WDFINTERRUPT  Interrupt,
    IN WDFDEVICE     AssociatedDevice
    )
/*++

Routine Description:

    This event is called when the Framework moves the device to D0, and after
    EvtDeviceD0Entry.  The driver should enable its interrupt here.

    This function will be called at the device's assigned interrupt
    IRQL (DIRQL.)

Arguments:

    Interrupt - Handle to a Framework interrupt object.

    AssociatedDevice - Handle to a Framework device object.

Return Value:

    BOOLEAN - TRUE indicates that the interrupt was successfully enabled.

--*/
{
    PFDO_DATA           fdoData;

	UNREFERENCED_PARAMETER(Interrupt);

	DbgPrint("--> HytecPCIInterruptEnable\n");

    fdoData = FdoGetData(AssociatedDevice);
    //NICEnableInterrupt(Interrupt, fdoData);

	DbgPrint("<-- HytecPCIInterruptEnable\n");

    return STATUS_SUCCESS;
}

NTSTATUS
HytecPCIInterruptDisable(
    IN WDFINTERRUPT  Interrupt,
    IN WDFDEVICE     AssociatedDevice
    )
/*++

Routine Description:

    This event is called before the Framework moves the device to D1, D2 or D3
    and before EvtDeviceD0Exit.  The driver should disable its interrupt here.

    This function will be called at the device's assigned interrupt
    IRQL (DIRQL.)

Arguments:

    Interrupt - Handle to a Framework interrupt object.

    AssociatedDevice - Handle to a Framework device object.

Return Value:

    STATUS_SUCCESS -  indicates success.

--*/
{
    PFDO_DATA           fdoData;

    UNREFERENCED_PARAMETER(Interrupt);
	DbgPrint("--> HytecPCIInterruptDisable\n");

    fdoData = FdoGetData(AssociatedDevice);
    //NICDisableInterrupt(fdoData);

	DbgPrint("<-- HytecPCIInterruptDisable\n");

    return STATUS_SUCCESS;
}

NTSTATUS
NICEvtDeviceD0EntryPostInterruptsEnabled(
    IN WDFDEVICE     Device,
    IN WDF_POWER_DEVICE_STATE PreviousState
    )
/*++

Routine Description:

    This event is called so that driver can do PASSIVE_LEVEL work after
    the interrupt is connected and enabled. Here we start the watchdog timer.
    Watch dog timer is used to do the initial link detection during
    start and then used to make sure the device is not stuck for any reason.

    This function is not marked pageable because this function is in the
    device power up path. When a function is marked pagable and the code
    section is paged out, it will generate a page fault which could impact
    the fast resume behavior because the client driver will have to wait
    until the system drivers can service this page fault.

Arguments:

    Interrupt - Handle to a Framework interrupt object.

    AssociatedDevice - Handle to a Framework device object.

Return Value:

   STATUS_SUCCESS -  indicates success.

--*/
{
    PFDO_DATA  fdoData;

    UNREFERENCED_PARAMETER( PreviousState );

	DbgPrint("--> NICEvtDeviceD0EntryPostInterruptsEnabled\n");

    fdoData = FdoGetData(Device);

	DbgPrint("<-- NICEvtDeviceD0EntryPostInterruptsEnabled\n");

    return STATUS_SUCCESS;

}

NTSTATUS
NICEvtDeviceD0ExitPreInterruptsDisabled(
    IN WDFDEVICE     Device,
    IN WDF_POWER_DEVICE_STATE TargetState
    )
/*++

Routine Description:

    This event is called so that driver can do PASSIVE_LEVEL work before
    the interrupt is disconnected and disabled.

Arguments:

    Interrupt - Handle to a Framework interrupt object.

    AssociatedDevice - Handle to a Framework device object.

Return Value:

    STATUS_SUCCESS -  indicates success.

--*/
{
    PFDO_DATA           fdoData;

    UNREFERENCED_PARAMETER(TargetState);

    PAGED_CODE();

	DbgPrint("--> NICEvtDeviceD0ExitPreInterruptsDisabled\n");

    fdoData = FdoGetData(Device);

	DbgPrint("<-- NICEvtDeviceD0ExitPreInterruptsDisabled\n");

    return STATUS_SUCCESS;

}
