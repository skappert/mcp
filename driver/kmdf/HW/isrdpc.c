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
NICEvtInterruptIsr(
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
    BOOLEAN    InterruptRecognized = FALSE;
    PFDO_DATA  FdoData = NULL;
    //USHORT     IntStatus;

    UNREFERENCED_PARAMETER( MessageID );

	DbgPrint("--> NICEvtInterruptIsr\n");

    FdoData = FdoGetData(WdfInterruptGetDevice(Interrupt));

#if 0
    //
    // We process the interrupt if it's not disabled and it's active
    //
    if (!NIC_INTERRUPT_DISABLED(FdoData) && NIC_INTERRUPT_ACTIVE(FdoData))
    {
        InterruptRecognized = TRUE;

        //
        // Disable the interrupt (will be re-enabled in NICEvtInterruptDpc
        //
        //NICDisableInterrupt(FdoData);

        //
        // Acknowledge the interrupt(s) and get the interrupt status
        //

        NIC_ACK_INTERRUPT(FdoData, IntStatus);

        WdfInterruptQueueDpcForIsr( Interrupt );

    }
#endif
	DbgPrint("<-- NICEvtInterruptIsr\n");

    return InterruptRecognized;
}

VOID
NICEvtInterruptDpc(
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

	DbgPrint("--> NICEvtInterruptDpc\n");

    fdoData = FdoGetData(WdfDevice);
#if 0
    //
    // Re-enable the interrupt (disabled in MPIsr)
    //
    WdfInterruptSynchronize(
        WdfInterrupt,
        NICEnableInterrupt,
        fdoData);
#endif
	DbgPrint("<-- NICEvtInterruptDpc\n");

}

NTSTATUS
NICEvtInterruptEnable(
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

	DbgPrint("--> NICEvtInterruptEnable\n");

    fdoData = FdoGetData(AssociatedDevice);
    //NICEnableInterrupt(Interrupt, fdoData);

	DbgPrint("<-- NICEvtInterruptEnable\n");

    return STATUS_SUCCESS;
}

NTSTATUS
NICEvtInterruptDisable(
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
	DbgPrint("--> NICEvtInterruptDisable\n");

    fdoData = FdoGetData(AssociatedDevice);
    //NICDisableInterrupt(fdoData);

	DbgPrint("<-- NICEvtInterruptDisable\n");

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
