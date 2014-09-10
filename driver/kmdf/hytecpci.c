
/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

    PciDrv.c

Abstract:

    This is a generic WDM sample driver for Intel 82557/82558
    based PCI Ethernet Adapter (10/100) and Intel compatibles.
    The WDM interface in this sample is based on the Toaster function
    driver, and all the code to access the hardware is taken from
    the E100BEX NDIS miniport sample from the DDK and converted to
    use WDM interfaces instead of NDIS functions.

    This driver can be installed as a standalone driver (genpci.inf)
    for the Intel PCI device.  Please read the PCIDRV.HTM file for 
    more information.

Environment:

    Kernel mode

--*/

#include "precomp.h"


//
// Global debug error level
//
#if !defined(EVENT_TRACING)
ULONG DebugLevel = TRACE_LEVEL_INFORMATION;
ULONG DebugFlag = 0x2f;//0x46;//0x4FF; //0x00000006;
#else
ULONG DebugLevel; // wouldn't be used to control the TRACE_LEVEL_VERBOSE
ULONG DebugFlag;
#endif

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, HytecPCIDeviceAdd)
#pragma alloc_text (PAGE, HytecPCIDeviceContextCleanup)
#pragma alloc_text (PAGE, HytecPCIDevicePrepareHardware)
#pragma alloc_text (PAGE, HytecPCIDeviceReleaseHardware)
#pragma alloc_text (PAGE, HytecPCIDriverContextCleanup)
#pragma alloc_text (PAGE, HytecPCIDeviceSelfManagedIoCleanup)
#pragma alloc_text (PAGE, HytecPCIDeviceSelfManagedIoSuspend)
#pragma alloc_text (PAGE, HytecPCIDeviceWakeArmS0)
#pragma alloc_text (PAGE, HytecPCIDeviceWakeTriggeredS0)
#pragma alloc_text (PAGE, HytecPCIDeviceWakeArmSx)
#pragma alloc_text (PAGE, PciDrvSetPowerPolicy)
#pragma alloc_text (PAGE, PciDrvReadFdoRegistryKeyValue)
#endif


#define PARAMATER_NAME_LEN 80

NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath
    )
/*++

Routine Description:

    Installable driver initialization entry point.
    This entry point is called directly by the I/O system.

Arguments:

    DriverObject - pointer to the driver object

    RegistryPath - pointer to a unicode string representing the path,
                   to driver-specific key in the registry.

Return Value:

    STATUS_SUCCESS if successful,
    STATUS_UNSUCCESSFUL otherwise.

--*/
{
    NTSTATUS               status = STATUS_SUCCESS;
    WDF_DRIVER_CONFIG      config;
    WDF_OBJECT_ATTRIBUTES  attrib;
    WDFDRIVER              driver;

	DbgPrint("Hytec CAMAC controller - Driver Framework Edition \n");

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attrib, DRIVER_CONTEXT);

    //
    // Register a cleanup callback so that we can call WPP_CLEANUP when
    // the framework driver object is deleted during driver unload.
    //
    attrib.EvtCleanupCallback = HytecPCIDriverContextCleanup;

    //
    // Initialize the Driver Config structure..
    //
    WDF_DRIVER_CONFIG_INIT(&config, HytecPCIDeviceAdd);

    //
    // Create a WDFDRIVER object.
    //
    status = WdfDriverCreate(DriverObject,
                             RegistryPath,
                             &attrib,
                             &config,
                             &driver);

    if (!NT_SUCCESS(status)) {
		DbgPrint("WdfDriverCreate failed with status %!STATUS!\n", status);

        return status;
    }

    return status;

}

NTSTATUS
HytecPCIDeviceAdd(
    IN WDFDRIVER        Driver,
    IN PWDFDEVICE_INIT  DeviceInit
    )
/*++
Routine Description:

    EvtDeviceAdd is called by the framework in response to AddDevice
    call from the PnP manager.

Arguments:

    Driver - Handle to a framework driver object created in DriverEntry

    DeviceInit - Pointer to a framework-allocated WDFDEVICE_INIT structure.

Return Value:

    NTSTATUS

--*/
{
    NTSTATUS                        status = STATUS_SUCCESS;
    WDF_PNPPOWER_EVENT_CALLBACKS    pnpPowerCallbacks;
    WDF_POWER_POLICY_EVENT_CALLBACKS powerPolicyCallbacks;
    WDF_OBJECT_ATTRIBUTES           fdoAttributes;
    WDFDEVICE                       device;
    PFDO_DATA                       fdoData = NULL;

    PAGED_CODE();

	DbgPrint("-->HytecPCIDeviceAdd routine. Driver: 0x%p\n", Driver);

    //
    // I/O type is Buffered by default. If required to use something else,
    // call WdfDeviceInitSetIoType with the appropriate type.
    //
	WdfDeviceInitSetIoType(DeviceInit, WdfDeviceIoBuffered);

    //
    // Zero out the PnpPowerCallbacks structure.
    //
    WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpPowerCallbacks);

    //
    // Set Callbacks for any of the functions we are interested in.
    // If no callback is set, Framework will take the default action
    // by itself.  This sample provides many of the possible callbacks,
    // mostly because it's a fairly complex sample that drives full-featured
    // hardware.  Drivers derived from this sample will often be able to
    // provide only some of these.
    //

    //
    // These callback is invoked to tear down all the driver-managed state
    // that is set up in this function.  Many times, this callback won't do
    // much of anything, since many of the things that are set up here will
    // have their lifetimes automatically managed by the Framework.
    //


    //
    // These two callbacks set up and tear down hardware state,
    // specifically that which only has to be done once.
    //

    pnpPowerCallbacks.EvtDevicePrepareHardware = HytecPCIDevicePrepareHardware;
    pnpPowerCallbacks.EvtDeviceReleaseHardware = HytecPCIDeviceReleaseHardware;

    //
    // These two callbacks set up and tear down hardware state that must be
    // done every time the device moves in and out of the D0-working state.
    //

    pnpPowerCallbacks.EvtDeviceD0Entry = HytecPCIDeviceD0Entry;
    pnpPowerCallbacks.EvtDeviceD0Exit  = HytecPCIDeviceD0Exit;

    //
    // These next two callbacks are for doing work at PASSIVE_LEVEL (low IRQL)
    // after all the interrupts are connected and before they are disconnected.
    //
    // Some drivers need to do device initialization and tear-down while the
    // interrupt is connected.  (This is a problem for these devices, since
    // it opens them up to taking interrupts before they are actually ready
    // to handle them, or to taking them after they have torn down too much
    // to be able to handle them.)  While this hardware design pattern is to
    // be discouraged, it is possible to handle it by doing device init and
    // tear down in these routines rather than in EvtDeviceD0Entry and
    // EvtDeviceD0Exit.
    //
    // In this sample these callbacks don't do anything.
    //
#if 0
    pnpPowerCallbacks.EvtDeviceD0EntryPostInterruptsEnabled = NICEvtDeviceD0EntryPostInterruptsEnabled;
    pnpPowerCallbacks.EvtDeviceD0ExitPreInterruptsDisabled = NICEvtDeviceD0ExitPreInterruptsDisabled;
#endif
    //
    // This next group of five callbacks allow a driver to become involved in
    // starting and stopping operations within a driver as the driver moves
    // through various PnP/Power states.  These functions are not necessary
    // if the Framework is managing all the device's queues and there is no
    // activity going on that isn't queue-based.  This sample provides these
    // callbacks because it uses watchdog timer to monitor whether the device
    // is working or not and it needs to start and stop the timer when the device
    // is started or removed. It cannot start and stop the timers in the D0Entry
    // and D0Exit callbacks because if the device is surprise-removed, D0Exit
    // will not be called.
    //
    pnpPowerCallbacks.EvtDeviceSelfManagedIoInit    = HytecPCIDeviceSelfManagedIoInit;
    pnpPowerCallbacks.EvtDeviceSelfManagedIoCleanup = HytecPCIDeviceSelfManagedIoCleanup;
    pnpPowerCallbacks.EvtDeviceSelfManagedIoSuspend = HytecPCIDeviceSelfManagedIoSuspend;
    pnpPowerCallbacks.EvtDeviceSelfManagedIoRestart = HytecPCIDeviceSelfManagedIoRestart;

    //
    // Register the PnP and power callbacks. Power policy related callbacks will be registered
    // later.
    //
    WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpPowerCallbacks);

    //
    // Init the power policy callbacks
    //
    WDF_POWER_POLICY_EVENT_CALLBACKS_INIT(&powerPolicyCallbacks);

    //
    // This group of three callbacks allows this sample driver to manage
    // arming the device for wake from the S0 state.  Networking devices can
    // optionally be put into a low-power state when there is no networking
    // cable plugged into them.  This sample implements this feature.
    //
    powerPolicyCallbacks.EvtDeviceArmWakeFromS0 = HytecPCIDeviceWakeArmS0;
    powerPolicyCallbacks.EvtDeviceDisarmWakeFromS0 = HytecPCIDeviceWakeDisarmS0;
    powerPolicyCallbacks.EvtDeviceWakeFromS0Triggered = HytecPCIDeviceWakeTriggeredS0;

    //
    // This group of three callbacks allows the device to be armed for wake
    // from Sx (S1, S2, S3 or S4.)  Networking devices can optionally be put
    // into a state where a packet sent to them will cause the device's wake
    // signal to be triggered, which causes the machine to wake, moving back
    // into the S0 state.
    //

    powerPolicyCallbacks.EvtDeviceArmWakeFromSx = HytecPCIDeviceWakeArmSx;
    powerPolicyCallbacks.EvtDeviceDisarmWakeFromSx = HytecPCIDeviceWakeDisarmSx;
    powerPolicyCallbacks.EvtDeviceWakeFromSxTriggered = HytecPCIDeviceWakeTriggeredSx;

    //
    // Register the power policy callbacks.
    //
    WdfDeviceInitSetPowerPolicyEventCallbacks(DeviceInit, &powerPolicyCallbacks);

    // Since we are the function driver, we are now the power policy owner
    // for the device according to the default framework rule. We will register
    // our power policy callbacks after finding the wakeup capability of the device.

    //
    // Specify the context type and size for the device we are about to create.
    //
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&fdoAttributes, FDO_DATA);

    //
    // ContextCleanup will be called by the framework when it deletes the device.
    // So you can defer freeing any resources allocated to Cleanup callback in the
    // event AddDevice returns any error after the device is created.
    //
    fdoAttributes.EvtCleanupCallback = HytecPCIDeviceContextCleanup;

    status = WdfDeviceCreate(&DeviceInit, &fdoAttributes, &device);

    if ( !NT_SUCCESS(status)) {
		DbgPrint("WdfDeviceInitialize failed %!STATUS!\n", status);
        return status;
    }

    //
    // Device creation is complete.
    // Get the DeviceExtension and initialize it.
    //
    fdoData = FdoGetData(device);
    fdoData->WdfDevice = device;

	DbgPrint("PDO(0x%p) FDO(0x%p), Lower(0x%p) DevExt (0x%p)\n",
                WdfDeviceWdmGetPhysicalDevice (device),
                WdfDeviceWdmGetDeviceObject (device),
                WdfDeviceWdmGetAttachedDevice(device),
                fdoData);

    //
    // Initialize the device extension and allocate all the software resources
    //
#if 0
    status = NICAllocateSoftwareResources(fdoData);
    if (!NT_SUCCESS (status)){
        TraceEvents(TRACE_LEVEL_ERROR, DBG_PNP,
                    "NICAllocateSoftwareResources failed: %!STATUS!\n",
                    status);
        return status;
    }
#endif

    //
    // Tell the Framework that this device will need an interface so that
    // application can interact with it.
    //
    status = WdfDeviceCreateDeviceInterface(
                 device,
                 (LPGUID) &GUID_DEVINTERFACE_PCIDRV,
                 NULL
             );

    if (!NT_SUCCESS (status)) {
		DbgPrint("WdfDeviceCreateDeviceInterface failed %!STATUS!\n", status);
        return status;
    }

	DbgPrint("<-- HytecPCIDeviceAdd  \n");

    return status;
}

VOID
HytecPCIDeviceContextCleanup (
    WDFOBJECT       Device
    )
/*++

Routine Description:

   EvtDeviceContextCleanup event callback cleans up anything done in
   EvtDeviceAdd, except those things that are automatically cleaned
   up by the Framework.

   In the case of this sample, everything is automatically handled.  In a
   driver derived from this sample, it's quite likely that this function could
   be deleted.

Arguments:

    Device - Handle to a framework device object.

Return Value:

    VOID

--*/
{
    PFDO_DATA               fdoData = NULL;

    PAGED_CODE();

    fdoData = FdoGetData((WDFDEVICE)Device);

	DbgPrint("--> HytecPCIDeviceContextCleanup\n");
#if 0
    NICFreeSoftwareResources(fdoData);
#endif
	DbgPrint("<-- HytecPCIDeviceContextCleanup\n");

}

NTSTATUS
HytecPCIDevicePrepareHardware (
    WDFDEVICE      Device,
    WDFCMRESLIST   Resources,
    WDFCMRESLIST   ResourcesTranslated
    )
/*++

Routine Description:

    EvtDeviceStart event callback performs operations that are necessary
    to make the driver's device operational. The framework calls the driver's
    EvtDeviceStart callback when the PnP manager sends an IRP_MN_START_DEVICE
    request to the driver stack.

Arguments:

    Device - Handle to a framework device object.

    Resources - Handle to a collection of framework resource objects.
                This collection identifies the raw (bus-relative) hardware
                resources that have been assigned to the device.

    ResourcesTranslated - Handle to a collection of framework resource objects.
                This collection identifies the translated (system-physical)
                hardware resources that have been assigned to the device.
                The resources appear from the CPU's point of view.
                Use this list of resources to map I/O space and
                device-accessible memory into virtual address space

Return Value:

    WDF status code

--*/
{
    NTSTATUS     status = STATUS_SUCCESS;
    PFDO_DATA    fdoData = NULL;

    UNREFERENCED_PARAMETER(Resources);
    UNREFERENCED_PARAMETER(ResourcesTranslated);

    PAGED_CODE();

	DbgPrint("--> HytecPCIDevicePrepareHardware\n");

    fdoData = FdoGetData(Device);
#if 0
    status = NICMapHWResources(fdoData, Resources, ResourcesTranslated);
    if (!NT_SUCCESS (status)){
        TraceEvents(TRACE_LEVEL_ERROR, DBG_PNP,
                    "NICMapHWResources failed: %!STATUS!\n", status);
        return status;
    }
#endif
	DbgPrint("<-- HytecPCIDevicePrepareHardware\n");

    return status;

}

NTSTATUS
HytecPCIDeviceReleaseHardware(
    IN  WDFDEVICE    Device,
    IN  WDFCMRESLIST ResourcesTranslated
    )
/*++

Routine Description:

    EvtDeviceReleaseHardware is called by the framework whenever the PnP manager
    is revoking ownership of our resources.  This may be in response to either
    IRP_MN_STOP_DEVICE or IRP_MN_REMOVE_DEVICE.  The callback is made before
    passing down the IRP to the lower driver.

    In this callback, do anything necessary to free those resources.

Arguments:

    Device - Handle to a framework device object.

    ResourcesTranslated - Handle to a collection of framework resource objects.
                This collection identifies the translated (system-physical)
                hardware resources that have been assigned to the device.
                The resources appear from the CPU's point of view.
                Use this list of resources to map I/O space and
                device-accessible memory into virtual address space

Return Value:

    NTSTATUS - Failures will be logged, but not acted on.

--*/
{
    PFDO_DATA  fdoData = NULL;

    UNREFERENCED_PARAMETER(ResourcesTranslated);

    PAGED_CODE();

	DbgPrint("--> HytecPCIDeviceReleaseHardware\n");

    fdoData = FdoGetData(Device);

    //
    // Unmap any I/O ports. Disconnecting from the interrupt will be done
    // automatically by the framework.
    //
#if 0
    NICUnmapHWResources(fdoData);
#endif
	DbgPrint("<-- HytecPCIDeviceReleaseHardware\n");

    return STATUS_SUCCESS;
}

NTSTATUS
HytecPCIDeviceD0Entry(
    IN  WDFDEVICE Device,
    IN  WDF_POWER_DEVICE_STATE PreviousState
    )
/*++

Routine Description:

    EvtDeviceD0Entry event callback must perform any operations that are
    necessary before the specified device is used.  It will be called every
    time the hardware needs to be (re-)initialized.  This includes after
    IRP_MN_START_DEVICE, IRP_MN_CANCEL_STOP_DEVICE, IRP_MN_CANCEL_REMOVE_DEVICE,
    IRP_MN_SET_POWER-D0.

    This function is not marked pageable because this function is in the
    device power up path. When a function is marked pagable and the code
    section is paged out, it will generate a page fault which could impact
    the fast resume behavior because the client driver will have to wait
    until the system drivers can service this page fault.

    This function runs at PASSIVE_LEVEL, even though it is not paged.  A
    driver can optionally make this function pageable if DO_POWER_PAGABLE
    is set.  Even if DO_POWER_PAGABLE isn't set, this function still runs
    at PASSIVE_LEVEL.  In this case, though, the function absolutely must
    not do anything that will cause a page fault.

Arguments:

    Device - Handle to a framework device object.

    PreviousState - Device power state which the device was in most recently.
        If the device is being newly started, this will be
        PowerDeviceUnspecified.

Return Value:

    NTSTATUS

--*/
{
    PFDO_DATA               fdoData;

	DbgPrint("-->HytecPCIDeviceD0Entry - coming from %s\n",
                DbgDevicePowerString(PreviousState));

    fdoData = FdoGetData(Device);

    ASSERT(PowerDeviceD0 != PreviousState);

    fdoData->DevicePowerState = PowerDeviceD0;

	DbgPrint("<--HytecPCIDeviceD0Entry\n");

    return STATUS_SUCCESS;
}


NTSTATUS
HytecPCIDeviceD0Exit(
    IN  WDFDEVICE Device,
    IN  WDF_POWER_DEVICE_STATE TargetState
    )
/*++

Routine Description:

    This routine undoes anything done in EvtDeviceD0Entry.  It is called
    whenever the device leaves the D0 state, which happens when the device is
    stopped, when it is removed, and when it is powered off.

    The device is still in D0 when this callback is invoked, which means that
    the driver can still touch hardware in this routine.

    Note that interrupts have already been disabled by the time that this
    callback is invoked.

   EvtDeviceD0Exit event callback must perform any operations that are
   necessary before the specified device is moved out of the D0 state.  If the
   driver needs to save hardware state before the device is powered down, then
   that should be done here.

   This function runs at PASSIVE_LEVEL, though it is generally not paged.  A
   driver can optionally make this function pageable if DO_POWER_PAGABLE is set.

   Even if DO_POWER_PAGABLE isn't set, this function still runs at
   PASSIVE_LEVEL.  In this case, though, the function absolutely must not do
   anything that will cause a page fault.

Arguments:

    Device - Handle to a framework device object.

    TargetState - Device power state which the device will be put in once this
        callback is complete.

Return Value:

    Success implies that the device can be used.  Failure will result in the
    device stack being torn down.

--*/
{
    PFDO_DATA    fdoData;

    UNREFERENCED_PARAMETER(Device);

	DbgPrint("-->HytecPCIDeviceD0Exit - moving to %s\n",
                DbgDevicePowerString(TargetState));

    fdoData = FdoGetData(Device);

    fdoData->DevicePowerState = TargetState;

    switch (TargetState) {
    case WdfPowerDeviceD1:
    case WdfPowerDeviceD2:
    case WdfPowerDeviceD3:
        DbgPrint("Entering a deeper sleep state\n");
        break;

    case WdfPowerDevicePrepareForHibernation:

        //
        // Fill in any code to save hardware state here.  Do not put in any
        // code to shut the device off.  If this device cannot support being
        // in the paging path (or being a parent or grandparent of a paging
        // path device) then this whole case can be deleted.
        //
        ASSERT(FALSE); // This driver shouldn't get this.
        break;

    case WdfPowerDeviceD3Final:
        //
        // Reset and put the device into a known initial state we're shutting
        // down for the last time.
        //
#if 0
        NICShutdown(fdoData);
#endif
		break;

    default:
        break;
    }


	DbgPrint("<--HytecPCIDeviceD0Exit\n");

    return STATUS_SUCCESS;
}

NTSTATUS
HytecPCIDeviceSelfManagedIoInit(
    IN  WDFDEVICE Device
    )
/*++

Routine Description:

    HytecPCIDeviceSelfManagedIoInit is called by the Framework when the device
    enters the D0 state.  Its job is to start any I/O-related actions that the
    Framework isn't managing.  This might include releasing queues that are not
    power-managed, that is, the Framework is not automatically holding and releasing
    them across PnP/Power transitions. (The default behavior for WDFQUEUE is
    auto-managed, so most queues don't need to be dealt with here.)  This might
    also include setting up non-queue-based actions.

    If you allow the Framework to manage most or all of your queues, then when
    you build a driver from this sample, you can probably delete this function.

    In this driver, the SelfManagedIo callbacks are used to implement a watchdog timer.

    This function is not marked pagable because this function is in the
    device power up path. When a function is marked pagable and the code
    section is paged out, it will generate a page fault which could impact
    the fast resume behavior because the client driver will have to wait
    until the system drivers can service this page fault.

Arguments:

    Device - Handle to a framework device object.

Return Value:

    NTSTATUS - Failures will result in the device stack being torn down.

--*/
{
    PFDO_DATA           fdoData = NULL;
	NTSTATUS            status = STATUS_SUCCESS;

	DbgPrint("--> HytecPCIDeviceSelfManagedIoInit\n");

    fdoData = FdoGetData(Device);
#if 0
	//
    // To minimize init-time, create a timer DPC to do link detection.
    // This DPC will also be used to check for hardware hang.
    //
    WDF_TIMER_CONFIG_INIT(&wdfTimerConfig, NICWatchDogEvtTimerFunc);

    WDF_OBJECT_ATTRIBUTES_INIT(&timerAttributes);
    timerAttributes.ParentObject = fdoData->WdfDevice;

    status = WdfTimerCreate(
       &wdfTimerConfig,
       &timerAttributes,
       &fdoData->WatchDogTimer
       );
#endif
    if(!NT_SUCCESS(status) ) {
		DbgPrint("Error: WdfTimerCreate create failed 0x%x\n", status);
       return status;
    }
#if 0
    NICStartWatchDogTimer(fdoData);
#endif
	DbgPrint("<-- HytecPCIDeviceSelfManagedIoInit\n");

    return status;
}

NTSTATUS
HytecPCIDeviceSelfManagedIoSuspend(
    IN  WDFDEVICE Device
    )
/*++

Routine Description:

    EvtDeviceSelfManagedIoSuspend is called by the Framework before the device
    leaves the D0 state.  Its job is to stop any I/O-related actions that the
    Framework isn't managing, and which cannot be handled when the device
    hardware isn't available.  In general, this means reversing anything that
    was done in EvtDeviceSelfManagedIoStart.

    If you allow the Framework to manage most or all of your queues, then when
    you build a driver from this sample, you can probably delete this function.

Arguments:

    Device - Handle to a framework device object.

Return Value:

    NTSTATUS - Failures will result in the device stack being torn down.

--*/
{
    PFDO_DATA   fdoData = NULL;

    PAGED_CODE();

	DbgPrint("--> HytecPCIDeviceSelfManagedIoSuspend\n");

    fdoData = FdoGetData(Device);

    //
    // Stop the watchdog timer and wait for DPC to run to completion if
    // it's already fired.
    //
#if 0
    WdfTimerStop(fdoData->WatchDogTimer, TRUE);
#endif
	DbgPrint("<-- HytecPCIDeviceSelfManagedIoSuspend\n");

    return STATUS_SUCCESS;
}

NTSTATUS
HytecPCIDeviceSelfManagedIoRestart(
    IN  WDFDEVICE Device
    )
/*++

Routine Description:

    EvtDeviceSelfManagedIoRestart is called by the Framework before the device
    is restarted for one of the following reasons:
    a) the PnP resources were rebalanced (framework received
        query-stop and stop IRPS )
    b) the device resumed from a low power state to D0.

    This function is not marked pagable because this function is in the
    device power up path. When a function is marked pagable and the code
    section is paged out, it will generate a page fault which could impact
    the fast resume behavior because the client driver will have to wait
    until the system drivers can service this page fault.

Arguments:

    Device - Handle to a framework device object.

Return Value:

    NTSTATUS - Failure will cause the device stack to be torn down.

--*/
{
    PFDO_DATA   fdoData;

	DbgPrint("--> HytecPCIDeviceSelfManagedIoRestart\n");

    fdoData = FdoGetData(Device);

    //
    // Restart the watchdog timer.
    //
#if 0
    NICStartWatchDogTimer(fdoData);
#endif
	DbgPrint("<-- HytecPCIDeviceSelfManagedIoRestart\n");

    return STATUS_SUCCESS;
}

VOID
HytecPCIDeviceSelfManagedIoCleanup(
    IN  WDFDEVICE Device
    )
/*++

Routine Description:

    EvtDeviceSelfManagedIoCleanup is called by the Framework when the device is
    being torn down, either in response to the WDM IRP_MN_REMOVE_DEVICE
    It will be called only once.  Its job is to stop all outstanding I/O in the driver
    that the Framework is not managing.

Arguments:

    Device - Handle to a framework device object.

Return Value:

    None

--*/
{
	UNREFERENCED_PARAMETER(Device);

    PAGED_CODE();

	DbgPrint("--> HytecPCIDeviceSelfManagedIoCleanup\n");

	DbgPrint("<-- HytecPCIDeviceSelfManagedIoCleanup\n");
}


VOID
HytecPCIIoDeviceControl(
    IN WDFQUEUE    Queue,
    IN WDFREQUEST  Request,
    IN size_t      OutputBufferLength,
    IN size_t      InputBufferLength,
    IN ULONG       IoControlCode
    )
/*++

Routine Description:

    This event is called when the framework receives IRP_MJ_DEVICE_CONTROL
    requests from the system.

Arguments:

    Queue - Handle to the framework queue object that is associated
            with the I/O request.
    Request - Handle to a framework request object.

    OutputBufferLength - length of the request's output buffer,
                        if an output buffer is available.
    InputBufferLength - length of the request's input buffer,
                        if an input buffer is available.

    IoControlCode - the driver-defined or system-defined I/O control code
                    (IOCTL) that is associated with the request.
Return Value:

    VOID

--*/
{
    //NTSTATUS                status= STATUS_SUCCESS;
    PFDO_DATA               fdoData = NULL;
    WDFDEVICE               hDevice;
    WDF_REQUEST_PARAMETERS  params;

    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

	DbgPrint("HytecPCIIoDeviceControl called %p\n", Request);

    hDevice = WdfIoQueueGetDevice(Queue);
    fdoData = FdoGetData(hDevice);

    WDF_REQUEST_PARAMETERS_INIT(&params);

    WdfRequestGetParameters(
        Request,
        &params
        );

    switch (IoControlCode)
    {
         default:
			ASSERT((IoControlCode & 0x3) != METHOD_BUFFERED);
            ASSERTMSG(FALSE, "Invalid IOCTL request\n");
            WdfRequestComplete(Request, STATUS_INVALID_DEVICE_REQUEST);
            break;
    }

    return;
}

NTSTATUS
HytecPCIDeviceWakeArmS0(
    IN WDFDEVICE Device
    )
/*++

Routine Description:

    EvtDeviceWakeArmS0 is called when the Framework arms the device for
    wake in the S0 state.  If there is any device-specific initialization
    that needs to be done to arm internal wake signals, or to route internal
    interrupt signals to the wake logic, it should be done here.  The device
    will be moved out of the D0 state soon after this callback is invoked.

    In this sample, wake from S0 involves waking on packet arrival, as does
    wake from Sx.  A more common NIC implementation might wake on cable
    insertion.

    This function is pageable and it will run at PASSIVE_LEVEL.

Arguments:

    Device - Handle to a Framework device object.

Return Value:

    NTSTATUS - Failure will result in the device remaining in the D0 state.

--*/
{
	NTSTATUS status = STATUS_SUCCESS;
    PFDO_DATA           fdoData;

    PAGED_CODE();

	DbgPrint("--> HytecPCIDeviceWakeArmS0\n");

    fdoData = FdoGetData(Device);

    //
    // Add pattern before sending wait-wake
    //
#if 0
    status = NICConfigureForWakeUp(fdoData, TRUE);
#endif
	DbgPrint("<-- HytecPCIDeviceWakeArmS0 %x\n", status);

    return status;
}

NTSTATUS
HytecPCIDeviceWakeArmSx(
    IN WDFDEVICE Device
    )
/*++

Routine Description:

    EvtDeviceWakeArmSx is called when the Framework arms the device for
    wake from the S1, S2, S3 or S4 states.  If there is any device-specific
    initialization that needs to be done to arm internal wake signals, or to
    route internal interrupt signals to the wake logic, it should be done here.
    The device will be moved out of the D0 state soon after this callback is
    invoked.

    In this sample, wake from Sx involves arming for wake on packet arrival.
    Cable insertion should not be enabled, as nobody would want their machine
    to wake up simply because they plugged the cable in.

    This function runs at PASSIVE_LEVEL.  Whether it is pageable or not depends
    on whether the device has set DO_POWER_PAGABLE.

Arguments:

    Device - Handle to a Framework device object.

Return Value:

    NTSTATUS - Failure will result in the device not being armed for wake
    while the system is in Sx.

--*/
{
	NTSTATUS   status = STATUS_SUCCESS;
    PFDO_DATA  fdoData;

    PAGED_CODE();

	DbgPrint("--> HytecPCIDeviceWakeArmSx\n");

    fdoData = FdoGetData(Device);
    //
    // Add pattern before sending wait-wake
    //
#if 0
    status = NICConfigureForWakeUp(fdoData, TRUE);
#endif
	DbgPrint("<-- HytecPCIDeviceWakeArmSx %x\n", status);

    return status;
}

VOID
HytecPCIDeviceWakeDisarmS0(
    IN WDFDEVICE Device
    )
/*++

Routine Description:

    EvtDeviceWakeDisarmS0 reverses anything done in EvtDeviceWakeArmS0.

    This function is not marked pageable because this function is in the
    device power up path. When a function is marked pagable and the code
    section is paged out, it will generate a page fault which could impact
    the fast resume behavior because the client driver will have to wait
    until the system drivers can service this page fault.

Arguments:

    Device - Handle to a Framework device object.

Return Value:

    VOID.

--*/
{
	NTSTATUS status = STATUS_SUCCESS;
    PFDO_DATA           fdoData;

	DbgPrint("--> HytecPCIDeviceWakeDisarmS0\n");

    fdoData = FdoGetData(Device);
#if 0
    status = NICConfigureForWakeUp(fdoData, FALSE);
#endif
	DbgPrint("<-- HytecPCIDeviceWakeDisarmS0 %x\n", status);

    return ;
}

VOID
HytecPCIDeviceWakeDisarmSx(
    IN WDFDEVICE Device
    )
/*++

Routine Description:

    EvtDeviceWakeDisarmSx reverses anything done in EvtDeviceWakeArmSx.

    This function is not marked pageable because this function is in the
    device power up path. When a function is marked pagable and the code
    section is paged out, it will generate a page fault which could impact
    the fast resume behavior because the client driver will have to wait
    until the system drivers can service this page fault.

    This function runs at PASSIVE_LEVEL, even though it is not paged.  A
    driver can optionally make this function pageable if DO_POWER_PAGABLE
    is set.  Even if DO_POWER_PAGABLE isn't set, this function still runs
    at PASSIVE_LEVEL.  In this case, though, the function absolutely must
    not do anything that will cause a page fault.

Arguments:

    Device - Handle to a Framework device object.

Return Value:

    VOID

--*/
{
	NTSTATUS   status = STATUS_SUCCESS;
    PFDO_DATA  fdoData;

    UNREFERENCED_PARAMETER(Device);

	DbgPrint("--> HytecPCIDeviceWakeDisarmSx\n");

    fdoData = FdoGetData(Device);
#if 0
    status = NICConfigureForWakeUp(fdoData, FALSE);
#endif
	DbgPrint("<-- HytecPCIDeviceWakeDisarmSx %x\n", status);

    return;
}

VOID
HytecPCIDeviceWakeTriggeredS0(
    IN WDFDEVICE Device
    )
/*++

Routine Description:

    EvtDeviceWakeTriggeredS0 will be called whenever the device triggers its
    wake signal after being armed for wake from S0.

    This function is pageable and runs at PASSIVE_LEVEL.

Arguments:

    Device - Handle to a Framework device object.

Return Value:

    VOID

--*/
{
    UNREFERENCED_PARAMETER(Device);

    PAGED_CODE();

	DbgPrint("--> HytecPCIDeviceWakeTriggeredS0\n");


	DbgPrint("<-- HytecPCIDeviceWakeTriggeredS0\n");
}

VOID
HytecPCIDeviceWakeTriggeredSx(
    IN WDFDEVICE Device
    )
/*++

Routine Description:

    EvtDeviceWakeTriggeredSx will be called whenever the device triggers its
    wake signal after being armed for wake from Sx.

    This function is not marked pageable because this function is in the
    device power up path. When a function is marked pagable and the code
    section is paged out, it will generate a page fault which could impact
    the fast resume behavior because the client driver will have to wait
    until the system drivers can service this page fault.

    This function runs at PASSIVE_LEVEL, even though it is not paged.  A
    driver can optionally make this function pageable if DO_POWER_PAGABLE
    is set.  Even if DO_POWER_PAGABLE isn't set, this function still runs
    at PASSIVE_LEVEL.  In this case, though, the function absolutely must
    not do anything that will cause a page fault.

Arguments:

    Device - Handle to a Framework device object.

Return Value:

    VOID

--*/
{
    UNREFERENCED_PARAMETER(Device);

	DbgPrint("--> HytecPCIDeviceWakeTriggeredSx");


	DbgPrint("<-- HytecPCIDeviceWakeTriggeredSx");

    return;
}

VOID
HytecPCIDriverContextCleanup(
    IN WDFOBJECT Driver
    )
/*++
Routine Description:

    Free all the resources allocated in DriverEntry.

Arguments:

    Driver - handle to a WDF Driver object.

Return Value:

    VOID.

--*/
{
    PDRIVER_CONTEXT driverContext;

	DbgPrint("--> HytecPCIDriverContextCleanup\n");
    PAGED_CODE ();

    driverContext = GetDriverContext((WDFDRIVER)Driver);

    if (driverContext->RecvLookaside) {
        WdfObjectDelete(driverContext->RecvLookaside);
    }

}

NTSTATUS
PciDrvSetPowerPolicy(
        IN PFDO_DATA FdoData
    )
{
    WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS idleSettings;
    WDF_DEVICE_POWER_POLICY_WAKE_SETTINGS wakeSettings;
    NTSTATUS    status = STATUS_SUCCESS;

	DbgPrint("--> PciDrvSetPowerPolicy\n");

    PAGED_CODE();

    //
    // Init the idle policy structure.
    //
    WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS_INIT(&idleSettings, IdleCanWakeFromS0);
    idleSettings.IdleTimeout = 10000; // 10-sec

    status = WdfDeviceAssignS0IdleSettings(FdoData->WdfDevice, &idleSettings);
    if ( !NT_SUCCESS(status)) {
		DbgPrint("WdfDeviceSetPowerPolicyS0IdlePolicy failed %x\n", status);
        return status;
    }

    //
    // Init wait-wake policy structure.
    //
    WDF_DEVICE_POWER_POLICY_WAKE_SETTINGS_INIT(&wakeSettings);

    status = WdfDeviceAssignSxWakeSettings(FdoData->WdfDevice, &wakeSettings);
    if (!NT_SUCCESS(status)) {
		DbgPrint("WdfDeviceAssignSxWakeSettings failed %x\n", status);
        return status;
    }


    //
    // Functions that program wakeup patterns on the device
    // check this variable to see whether the NDIS edge has enabled
    // wakeup on this device. If there is no ndis edge, this variable and all
    // the checks can be removed because framework as a power policy owner
    // it knows when to call the driver to arm/disarm for wakeup.
    //
    FdoData->AllowWakeArming = TRUE;

	DbgPrint("<-- PciDrvSetPowerPolicy\n");

    return status;
}

PCHAR
DbgDevicePowerString(
    IN WDF_POWER_DEVICE_STATE Type
    )
/*++

Updated Routine Description:
    DbgDevicePowerString does not change in this stage of the function driver.

--*/
{
    switch (Type)
    {
    case WdfPowerDeviceInvalid:
        return "WdfPowerDeviceInvalid";
    case WdfPowerDeviceD0:
        return "WdfPowerDeviceD0";
    case WdfPowerDeviceD1:
        return "WdfPowerDeviceD1";
    case WdfPowerDeviceD2:
        return "WdfPowerDeviceD2";
    case WdfPowerDeviceD3:
        return "WdfPowerDeviceD3";
    case WdfPowerDeviceD3Final:
        return "WdfPowerDeviceD3Final";
    case WdfPowerDevicePrepareForHibernation:
        return "WdfPowerDevicePrepareForHibernation";
    case WdfPowerDeviceMaximum:
        return "WdfPowerDeviceMaximum";
    default:
        return "UnKnown Device Power State";
    }
}


#if !defined(EVENT_TRACING)

VOID
TraceEvents    (
    IN ULONG   TraceEventsLevel,
    IN ULONG   TraceEventsFlag,
    IN PCCHAR  DebugMessage,
    ...
    )

/*++

Routine Description:

    Debug print for the sample driver.

Arguments:

    TraceEventsLevel - print level between 0 and 3, with 3 the most verbose

Return Value:

    None.

 --*/
 {
#if DBG
#define     TEMP_BUFFER_SIZE        512
    va_list    list;
    CHAR       debugMessageBuffer[TEMP_BUFFER_SIZE];
    NTSTATUS   status;

    va_start(list, DebugMessage);

    if (DebugMessage) {

        //
        // Using new safe string functions instead of _vsnprintf.
        // This function takes care of NULL terminating if the message
        // is longer than the buffer.
        //
        status = RtlStringCbVPrintfA( debugMessageBuffer,
                                      sizeof(debugMessageBuffer),
                                      DebugMessage,
                                      list );
        if(!NT_SUCCESS(status)) {

            DbgPrint (_DRIVER_NAME_": RtlStringCbVPrintfA failed %x\n",
                      status);
            return;
        }
        if (TraceEventsLevel <= TRACE_LEVEL_INFORMATION ||
            (TraceEventsLevel <= DebugLevel &&
             ((TraceEventsFlag & DebugFlag) == TraceEventsFlag))) {
            DbgPrint(debugMessageBuffer);
        }
    }
    va_end(list);

    return;
#else
    UNREFERENCED_PARAMETER(TraceEventsLevel);
    UNREFERENCED_PARAMETER(TraceEventsFlag);
    UNREFERENCED_PARAMETER(DebugMessage);
#endif
}

#endif

