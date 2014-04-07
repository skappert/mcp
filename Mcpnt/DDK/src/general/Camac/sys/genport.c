// Generic Port I/O driver for NT  VERSION 1.0
//
// Adapted from NT DDK ADLIB driver
//
// Robert R. Howell                 January 8, 1993
//
// Robert B. Nelson (Microsoft)     January 12, 1993
//      Cleaned up comments
//      Enabled and tested resource reporting
//      Added code to retrieve I/O address and port count from the Registry.
//
// Robert B. Nelson (Microsoft)     March 1, 1993
//      Added support for byte, word, and long I/O.
//      Added support for MIPS.
//      Fixed resource reporting.
//
// Robert B. Nelson (Microsoft)     May 1, 1993
//      Fixed port number validation.
//
// Robert B. Nelson (Microsoft)     Oct 25, 1993
//      Fixed MIPS support.
//

#include "genport.h"
#include "stdlib.h"



NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath
    )

/*++

Routine Description:
    This routine is the entry point for the driver.  It is responsible
    for setting the dispatch entry points in the driver object and creating
    the device object.  Any resources such as ports, interrupts and DMA
    channels used must be reported.  A symbolic link must be created between
    the device name and an entry in \DosDevices in order to allow Win32
    applications to open the device.

Arguments:
    
    DriverObject - Pointer to driver object created by the system.

Return Value:

    STATUS_SUCCESS if the driver initialized correctly, otherwise an error
    indicating the reason for failure.

--*/

{
    ULONG PortBase;                 // Port location, in NT's address form.
    ULONG PortCount;                // Count of contiguous I/O ports
	ULONG Interrupt;
	USHORT i;
	ULONG sizeOfResourceList;
	PCM_RESOURCE_LIST resourceList;
	PCM_FULL_RESOURCE_DESCRIPTOR nextFrd;
	PCM_PARTIAL_RESOURCE_DESCRIPTOR partial;

	PLOCAL_DEVICE_INFO extension;

    ULONG MappedVector;
	KIRQL irql;
	KAFFINITY Affinity;
	NTSTATUS ioConnectStatus;

	PHYSICAL_ADDRESS PortAddress;

    PLOCAL_DEVICE_INFO pLocalInfo;  // Device extension:
                                    //      local information for each device.
    NTSTATUS Status;
    PDEVICE_OBJECT DeviceObject;

	UNICODE_STRING eventName ;


    //CM_RESOURCE_LIST ResourceList;  // Resource usage list to report to system
    BOOLEAN ResourceConflict;       // This is set true if our I/O ports
                                    //      conflict with another driver

    // Try to retrieve base I/O port and range from the Parameters key of our
    // entry in the Registry.
    // If there isn't anything specified then use the values compiled into
    // this driver.
    {
        static  WCHAR               SubKeyString[] = L"\\Parameters";
        UNICODE_STRING              paramPath;
        RTL_QUERY_REGISTRY_TABLE    paramTable[4];
        ULONG                       DefaultBase		= BASE_PORT;
        ULONG                       DefaultCount	= NUMBER_PORTS;
		ULONG						DefaultInt		= INTERRUPT;
		
	
		

        //
        // Since the registry path parameter is a "counted" UNICODE string, it
        // might not be zero terminated.  For a very short time allocate memory
        // to hold the registry path as well as the Parameters key name zero
        // terminated so that we can use it to delve into the registry.
        //

        paramPath.MaximumLength = RegistryPath->Length + sizeof(SubKeyString);
        paramPath.Buffer = ExAllocatePool(PagedPool, paramPath.MaximumLength);

        if (paramPath.Buffer != NULL)
        {
            RtlMoveMemory(
                paramPath.Buffer, RegistryPath->Buffer, RegistryPath->Length);

            RtlMoveMemory(
                &paramPath.Buffer[RegistryPath->Length / 2], SubKeyString,
                sizeof(SubKeyString));

            paramPath.Length = paramPath.MaximumLength - 2;

            RtlZeroMemory(&paramTable[0], sizeof(paramTable));

            paramTable[0].Flags = RTL_QUERY_REGISTRY_DIRECT;
            paramTable[0].Name = L"IoPortAddress";
            paramTable[0].EntryContext = &PortBase;
            paramTable[0].DefaultType = REG_DWORD;
            paramTable[0].DefaultData = &DefaultBase;
            paramTable[0].DefaultLength = sizeof(ULONG);

            paramTable[1].Flags = RTL_QUERY_REGISTRY_DIRECT;
            paramTable[1].Name = L"IoPortCount";
            paramTable[1].EntryContext = &PortCount;
            paramTable[1].DefaultType = REG_DWORD;
            paramTable[1].DefaultData = &DefaultCount;
            paramTable[1].DefaultLength = sizeof(ULONG);

			paramTable[2].Flags = RTL_QUERY_REGISTRY_DIRECT;
            paramTable[2].Name = L"Interrupt";
            paramTable[2].EntryContext = &Interrupt;
            paramTable[2].DefaultType = REG_DWORD;
            paramTable[2].DefaultData = &DefaultInt;
            paramTable[2].DefaultLength = sizeof(ULONG);

            if (!NT_SUCCESS(RtlQueryRegistryValues(
                RTL_REGISTRY_ABSOLUTE | RTL_REGISTRY_OPTIONAL,
                paramPath.Buffer, &paramTable[0], NULL, NULL)))
            {
                PortBase = DefaultBase;
                PortCount = DefaultCount;
				Interrupt = DefaultInt;
            }
            ExFreePool(paramPath.Buffer);
        }
    }

    //
    // This call will map our IRQ to a system vector. It will also fill
    // in the IRQL (the kernel-defined level at which our ISR will run),
    // and affinity mask (which processors our ISR can run on).
    //
    // We need to do this so that when we connect to the interrupt, we
    // can supply the kernel with this information.
    //
	irql = Interrupt;
	
    MappedVector = HalGetInterruptVector(
			 Isa,	    // Interface type
			 0,	    // Bus number

                         //
                         // This is the IRQ number. The constannt
                         // STAT_IRQ is defined in stat.h
                         //
			 Interrupt, // Bus interrupt level
			 Interrupt, // Bus interrupt vector

			 // These next two parameters are
			 // filled in by the HAL

			 &irql,     // IRQ level
			 &Affinity  // Affinity mask
			 );

    PortAddress.LowPart  = PortBase;
    PortAddress.HighPart = 0;

    //
    // The size of the resource list is going to be one full descriptor
    // which already has one partial descriptor included, plus another
    // partial descriptor. One partial descriptor will be for the
    // interrupt, and the other for the port addresses.
    //

    sizeOfResourceList = sizeof(CM_FULL_RESOURCE_DESCRIPTOR);

    //
    // The full resource descriptor already contains one
    // partial.	Make room for one more.
    //
    // It will hold the irq "prd", and the port "prd".
    //	  ("prd" = partial resource descriptor)
    //

    sizeOfResourceList += sizeof(CM_PARTIAL_RESOURCE_DESCRIPTOR);

    //
    // Now we increment the length of the resource list by field offset
    // of the first frd.   This will give us the length of what preceeds
    // the first frd in the resource list.
    //	 (frd = full resource descriptor)
    //

    sizeOfResourceList += FIELD_OFFSET(
                              CM_RESOURCE_LIST,
                              List[0]
                              );

    resourceList = ExAllocatePool(
                       PagedPool,
                       sizeOfResourceList
                       );

    if (!resourceList) {

        return FALSE;

    }

    //
    // Zero out the list
    //

    RtlZeroMemory(
        resourceList,
        sizeOfResourceList
        );

    resourceList->Count = 1;
    nextFrd = &resourceList->List[0];

    nextFrd->InterfaceType = Isa;
    nextFrd->BusNumber = 0;

    //
    // We are going to report port addresses and interrupt
    //

    nextFrd->PartialResourceList.Count = 2;

    //
    // Now fill in the port data.  We don't wish to share
    // this port range with anyone.
    //
    // Note: the port address we pass in is the one we got
    // back from HalTranslateBusAddress.
    //

    partial = &nextFrd->PartialResourceList.PartialDescriptors[0];

    partial->Type = CmResourceTypePort;
    partial->ShareDisposition = CmResourceShareDriverExclusive;
    partial->Flags = CM_RESOURCE_PORT_IO;
    partial->u.Port.Start = PortAddress;
    partial->u.Port.Length = PortCount;

    partial++;

    //
    // Now fill in the irq stuff.
    //
    // Note: for IoReportResourceUsage, the Interrupt.Level and
    // Interrupt.Vector are bus-specific level and vector, just
    // as we passed in to HalGetInterruptVector, not the mapped
    // system vector we got back from HalGetInterruptVector.
    //

    partial->Type = CmResourceTypeInterrupt;
    partial->u.Interrupt.Level = Interrupt;
    partial->u.Interrupt.Vector = Interrupt;
    partial->ShareDisposition = CmResourceShareDriverExclusive;
    partial->Flags = CM_RESOURCE_INTERRUPT_LATCHED;


    // Report our resource usage and detect conflicts
    Status = IoReportResourceUsage(
                   NULL,
                   DriverObject,
                   resourceList,
                   sizeOfResourceList,
                   NULL,
                   NULL,
                   0,
                   FALSE,
                   &ResourceConflict);

    if (ResourceConflict)
        Status = STATUS_DEVICE_CONFIGURATION_ERROR;

    if (!NT_SUCCESS(Status))
    {
        KdPrint( ("Resource reporting problem %8X", Status) );

        return Status;
    }

    // Initialize the driver object dispatch table.
    // NT sends requests to these routines.

    DriverObject->MajorFunction[IRP_MJ_CREATE]          = GpdDispatch;
    DriverObject->MajorFunction[IRP_MJ_CLOSE]           = GpdDispatch;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]  = GpdDispatch;
    DriverObject->DriverUnload                          = GpdUnload;

    // Create our device.
    Status = GpdCreateDevice(
                    GPD_DEVICE_NAME,
                    GPD_TYPE,
                    DriverObject,
                    &DeviceObject
                    );

    if ( NT_SUCCESS(Status) )
    {
        PHYSICAL_ADDRESS MappedAddress;
        ULONG MemType;

        // Convert the IO port address into a form NT likes.
        MemType = 1;                        // located in IO space
        HalTranslateBusAddress( Isa,
                                0,
                                PortAddress,
                                &MemType,
                                &MappedAddress );


        // Initialize the local driver info for each device object.
        pLocalInfo = (PLOCAL_DEVICE_INFO)DeviceObject->DeviceExtension;

        if (MemType == 0)
        {
            // Port is accessed through memory space - so get a virtual address

            pLocalInfo->PortWasMapped = TRUE;            

            // BUGBUG
            // MmMapIoSpace can fail if we run out of PTEs, we should be
            // checking the return value here

            pLocalInfo->PortBase = MmMapIoSpace(MappedAddress, PortCount, FALSE);
        }
        else
        {
            pLocalInfo->PortWasMapped = FALSE;
            pLocalInfo->PortBase = (PVOID)MappedAddress.LowPart;
        }

        pLocalInfo->DeviceObject    = DeviceObject;
        pLocalInfo->DeviceType      = GPD_TYPE;
        pLocalInfo->PortCount       = PortCount;
        pLocalInfo->PortMemoryType  = MemType;
    }
    else
    {
        //
        // Error creating device - release resources
        //

        RtlZeroMemory(resourceList, sizeOfResourceList);
    
        // Unreport our resource usage
        Status = IoReportResourceUsage(
                       NULL,
                       DriverObject,
                       resourceList,
                       sizeOfResourceList,
                       NULL,
                       NULL,
                       0,
                       FALSE,
                       &ResourceConflict);
    }

	//
	// register DPC routine
	//
	IoInitializeDpcRequest(DeviceObject, GpdDpcRoutine);

	//
	// fill in the device extension
	//
	extension = (PLOCAL_DEVICE_INFO) DeviceObject->DeviceExtension;
	extension->DeviceObject = DeviceObject;
    extension->PortBase		= PortBase;

	// Reset instruction pointer
	extension->WriteInstructionPtr		= 0;
	extension->ReadDataPtr				= 0;
	extension->ActualInstructionPtr		= 0;
	extension->ActualDataPtr			= 0;
	extension->ActualStringPtr			= 0;
	extension->ActualStringPosPtr		= 1;

	extension->GPIBListening			= FALSE;
	extension->GPIBPending				= FALSE;
	extension->ListPending				= FALSE;

	extension->IntInProgress			= FALSE;
	
	//
	// connect the device driver to the IRQ
	//
	ioConnectStatus = IoConnectInterrupt(
				&extension->InterruptObject,
				GpdInterruptServiceRoutine,
				extension->DeviceObject,
				NULL,
				MappedVector,
				irql,
				irql,
				Latched,
				FALSE,
				Affinity,
				FALSE
				);


	// Camac disable interrupts
	RtlInitUnicodeString (&eventName, L"\\BaseNamedObjects\\CamacDataEvt"); 
	extension->Event = IoCreateNotificationEvent (&eventName,&extension->Handle);

	KeClearEvent (extension->Event) ;


	SendF(extension,40);



    return Status;
}

VOID SendDNAF(PLOCAL_DEVICE_INFO extension,ULONG d,ULONG n,ULONG a,ULONG f)
{
	WRITE_PORT_ULONG((PULONG)((ULONG)extension->PortBase+0),(d & bytemask)+(((d>>8) & bytemask)<<16));
	WRITE_PORT_ULONG((PULONG)((ULONG)extension->PortBase+4),(((d>>16) & bytemask))+(a<<16));
	WRITE_PORT_ULONG((PULONG)((ULONG)extension->PortBase+8),(n & bytemask)+((f & bytemask)<<16));
}

VOID SendNAF(PLOCAL_DEVICE_INFO extension,ULONG n,ULONG a,ULONG f)
{
	WRITE_PORT_ULONG((PULONG)((ULONG)extension->PortBase+6),a);
	WRITE_PORT_ULONG((PUSHORT)((ULONG)extension->PortBase+8),((ULONG)f<<16 )+ (ULONG)n);
}

VOID SendF(PLOCAL_DEVICE_INFO extension,ULONG f)
{
	WRITE_PORT_UCHAR((PUCHAR)((ULONG)extension->PortBase+10),f);
}

ULONG ReadD(PLOCAL_DEVICE_INFO extension)
{
	ULONG data = 0;
	data = READ_PORT_UCHAR((PUCHAR)((ULONG)extension->PortBase + 0));
	data = data + (READ_PORT_UCHAR((PUCHAR)((ULONG)extension->PortBase+2))<<8);
	data = data + (READ_PORT_UCHAR((PUCHAR)((ULONG)extension->PortBase+4))<<16);
	return data;
}

UCHAR ReadENCL(PLOCAL_DEVICE_INFO extension)
{
	return READ_PORT_UCHAR((PUCHAR)((ULONG)extension->PortBase + 8));
}

UCHAR ReadCSR(PLOCAL_DEVICE_INFO extension)
{
	return READ_PORT_UCHAR((PUCHAR)((ULONG)extension->PortBase + 6));
}

NTSTATUS
GpdCreateDevice(
    IN   PWSTR              PrototypeName,
    IN   DEVICE_TYPE        DeviceType,
    IN   PDRIVER_OBJECT     DriverObject,
    OUT  PDEVICE_OBJECT     *ppDevObj
    )

/*++

Routine Description:
    This routine creates the device object and the symbolic link in
    \DosDevices.
    
    Ideally a name derived from a "Prototype", with a number appended at
    the end should be used.  For simplicity, just use the fixed name defined
    in the include file.  This means that only one device can be created.
    
    A symbolic link must be created between the device name and an entry
    in \DosDevices in order to allow Win32 applications to open the device.

Arguments:

    PrototypeName - Name base, # WOULD be appended to this.

    DeviceType - Type of device to create

    DriverObject - Pointer to driver object created by the system.

    ppDevObj - Pointer to place to store pointer to created device object

Return Value:

    STATUS_SUCCESS if the device and link are created correctly, otherwise
    an error indicating the reason for failure.

--*/


{
    NTSTATUS Status;                        // Status of utility calls
    UNICODE_STRING NtDeviceName;
    UNICODE_STRING Win32DeviceName;


    // Get UNICODE name for device.

    RtlInitUnicodeString(&NtDeviceName, PrototypeName);

    Status = IoCreateDevice(                             // Create it.
                    DriverObject,
                    sizeof(LOCAL_DEVICE_INFO),
                    &NtDeviceName,
                    DeviceType,
                    0,
                    TRUE,                      // Exclusive
                    ppDevObj
                    );

    if (!NT_SUCCESS(Status))
        return Status;             // Give up if create failed.

    // Clear local device info memory
    RtlZeroMemory((*ppDevObj)->DeviceExtension, sizeof(LOCAL_DEVICE_INFO));

    //
    // Set up the rest of the device info
    //  These are used for IRP_MJ_READ and IRP_MJ_WRITE which we don't use
    //    
    //  (*ppDevObj)->Flags |= DO_BUFFERED_IO;
    //  (*ppDevObj)->AlignmentRequirement = FILE_BYTE_ALIGNMENT;
    //

    RtlInitUnicodeString(&Win32DeviceName, DOS_DEVICE_NAME);

    Status = IoCreateSymbolicLink( &Win32DeviceName, &NtDeviceName );

    if (!NT_SUCCESS(Status))    // If we we couldn't create the link then
    {                           //  abort installation.
        IoDeleteDevice(*ppDevObj);
    }

    return Status;
}


BOOLEAN
GpdInterruptServiceRoutine(
    IN PKINTERRUPT Interrupt,
    IN OUT PVOID Context
    )
{
	UCHAR status;
    PDEVICE_OBJECT DeviceObject;
    PLOCAL_DEVICE_INFO extension;
	UCHAR test;
	UCHAR TheChar;
	UCHAR InstructionType;
	ULONG Index;
	ULONG Wait,Wait2;
    DeviceObject = Context;
    extension = DeviceObject->DeviceExtension;

    //
    // device specific stuff to dismiss the interrupt
    //

	
	if(extension->GPIBListening)
	{
		SendNAF(extension,extension->GPIBAddress,0,0);
		ReadCSR(extension);
		TheChar = (UCHAR)ReadD(extension);
		ReadENCL(extension);
		

		if(TheChar!=10&&TheChar!=13)extension->ReadString[extension->ActualStringPtr][extension->ActualStringPosPtr] = TheChar; 
		else extension->ReadString[extension->ActualStringPtr][extension->ActualStringPosPtr] = 32; 
		
		if(TheChar==10||(extension->ActualStringPosPtr==STRINGLEN))
		{
			extension->ReadString[extension->ActualStringPtr][0] = extension->ActualStringPosPtr;
			extension->GPIBListening = FALSE;
			extension->GPIBPending   = FALSE;
			extension->ActualStringPosPtr = 1;
			extension->ActualStringPtr++;
			if(extension->ActualStringPtr>=STRINGMEM)extension->ActualStringPtr = 0;
		}
		else extension->ActualStringPosPtr++;	
	}
	if(!extension->GPIBListening)
	{
		while ((extension->ActualInstructionPtr < extension->WriteInstructionPtr)&&
				(extension->ActualInstructionPtr < INSTRUCTIONMEM))
		{
			Index = extension->ActualInstructionPtr;
			InstructionType = extension->InstructionList[Index].type & INSTRUCTIONMASK;
			switch (InstructionType)
			{
				case SENDF:
					SendF(extension,extension->InstructionList[Index].f);
					ReadCSR(extension);
				break;
				case SENDNAF:
					SendNAF(extension,extension->InstructionList[Index].n,
									  extension->InstructionList[Index].a,
									  extension->InstructionList[Index].f);
					ReadCSR(extension);
				break;
				case SENDDNAF:
					SendDNAF(extension,extension->InstructionList[Index].d,
									  extension->InstructionList[Index].n,
									  extension->InstructionList[Index].a,
									  extension->InstructionList[Index].f);
					ReadCSR(extension);
				break;
				case READD:
					if (extension->ActualDataPtr < DATAMEM)
					{
						extension->ReadData[extension->ActualDataPtr] = ReadD(extension);
						extension->ActualDataPtr++;
					}
				break;
				case READCSR:
					ReadCSR(extension);
				break;
				case READENCL:
					ReadENCL(extension);
				break;
				case READGPIB:
					ReadENCL(extension);
					extension->GPIBAddress = extension->InstructionList[Index].n;
					extension->GPIBListening = TRUE;
				break;
			}
			extension->ActualInstructionPtr++;
			if (((extension->InstructionList[Index].type & WAITINT)!=0)||extension->GPIBListening) break;
		} 
		if(extension->ActualInstructionPtr < extension->WriteInstructionPtr)
			extension->IntInProgress	= TRUE;
		else 
		{
			extension->IntInProgress	= FALSE;
			extension->ListPending		= FALSE;
		}
	}

	//
    // Note: the CurrentIrp field of the device object gets filled in
    // by IoStartPacket or IoStartNextPacket (which eventually leads
    // calling the drivers StartIo routine, which should lead to the
    // interrupt).
    //
    // If this interrupt was not the result of driver initiated I/O
    // then this field would not be accurate.
    //
    IoRequestDpc(DeviceObject,
		 DeviceObject->CurrentIrp,
		 NULL
		 );

    return TRUE;
}
 
VOID
GpdDpcRoutine(
    IN PKDPC Dpc,
    IN PVOID DeferredContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
    )
{
    PDEVICE_OBJECT DeviceObject;
    PLOCAL_DEVICE_INFO extension;
    PIRP Irp;

    DeviceObject = DeferredContext;
    Irp = DeviceObject->CurrentIrp;
    extension = DeviceObject->DeviceExtension;
    extension->InterruptCount++;
	/*
    if (Irp) {
        //
        // need to fill in this field to get the I/O manager to copy the data
        // back to user address space
        //
        Irp->IoStatus.Information = sizeof(IOCTL_INFORMATION);
        Irp->IoStatus.Status = STATUS_SUCCESS;

        IoStartNextPacket(DeviceObject, FALSE);
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
    }
	*/

	KeSetEvent (extension->Event, 0, FALSE) ; 
    KeClearEvent (extension->Event) ; 

    return;
}
  
NTSTATUS
GpdDispatch(
    IN    PDEVICE_OBJECT pDO,
    IN    PIRP pIrp             
    )

/*++

Routine Description:
    This routine is the dispatch handler for the driver.  It is responsible
    for processing the IRPs.

Arguments:
    
    pDO - Pointer to device object.

    pIrp - Pointer to the current IRP.

Return Value:

    STATUS_SUCCESS if the IRP was processed successfully, otherwise an error
    indicating the reason for failure.

--*/

{
    PLOCAL_DEVICE_INFO extension;
    PIO_STACK_LOCATION pIrpStack;
    NTSTATUS Status;
	PULONG pIOBuffer;           // Pointer to transfer buffer
	UCHAR InstructionType;
	ULONG Index;

	USHORT i,j;
    //  Initialize the irp info field.
    //      This is used to return the number of bytes transfered.

    pIrp->IoStatus.Information = 0;
    extension = (PLOCAL_DEVICE_INFO)pDO->DeviceExtension;    // Get local info struct

    pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

    //  Set default return status
    Status = STATUS_NOT_IMPLEMENTED;

    // Dispatch based on major fcn code.

    switch (pIrpStack->MajorFunction)
    {
        case IRP_MJ_CREATE:
        case IRP_MJ_CLOSE:
            // We don't need any special processing on open/close so we'll
            // just return success.
            Status = STATUS_SUCCESS;
            break;

        case IRP_MJ_DEVICE_CONTROL:
            //  Dispatch on IOCTL
			pIOBuffer     = (PULONG)pIrp->AssociatedIrp.SystemBuffer;

            switch (pIrpStack->Parameters.DeviceIoControl.IoControlCode)
            {
			case IOCTL_SENDF:
				WRITE_PORT_UCHAR((PUCHAR)((ULONG)extension->PortBase + 10),
                *(PUCHAR)pIOBuffer);

				Status = STATUS_SUCCESS;
			break;
			
			case IOCTL_SENDNAF:
				WRITE_PORT_UCHAR((PUCHAR)((ULONG)extension->PortBase + 8),
                *(PUCHAR)pIOBuffer++);
				WRITE_PORT_UCHAR((PUCHAR)((ULONG)extension->PortBase + 6),
                *(PUCHAR)pIOBuffer++);
				WRITE_PORT_UCHAR((PUCHAR)((ULONG)extension->PortBase + 10),
                *(PUCHAR)pIOBuffer);

				Status = STATUS_SUCCESS;
			break;
			
			case IOCTL_SENDDNAF:
				WRITE_PORT_UCHAR((PULONG)((ULONG)extension->PortBase + 0),
                *(PULONG)pIOBuffer);
				WRITE_PORT_UCHAR((PULONG)((ULONG)extension->PortBase + 2),
                (*(PULONG)pIOBuffer)>>8);
				WRITE_PORT_UCHAR((PULONG)((ULONG)extension->PortBase + 4),
                (*(PULONG)pIOBuffer++)>>16);

				WRITE_PORT_UCHAR((PUCHAR)((ULONG)extension->PortBase + 8),
                *(PUCHAR)pIOBuffer++);
				WRITE_PORT_UCHAR((PUCHAR)((ULONG)extension->PortBase + 6),
                *(PUCHAR)pIOBuffer++);
				WRITE_PORT_UCHAR((PUCHAR)((ULONG)extension->PortBase + 10),
                *(PUCHAR)pIOBuffer);

				Status = STATUS_SUCCESS;
			break;
			
			case IOCTL_READD:
				*(PULONG)pIOBuffer = ReadD(extension);
				pIrp->IoStatus.Information = sizeof(ULONG);
				Status = STATUS_SUCCESS;
			break;
			
			case IOCTL_READCSR:
				*(PUCHAR)pIOBuffer = READ_PORT_UCHAR(
                            (PUCHAR)((ULONG)extension->PortBase + 6));
				pIrp->IoStatus.Information = sizeof(UCHAR);

				Status = STATUS_SUCCESS;
			break;
			
			case IOCTL_READENCL:
				*(PUCHAR)pIOBuffer = READ_PORT_UCHAR(
                            (PUCHAR)((ULONG)extension->PortBase + 8));
				pIrp->IoStatus.Information = sizeof(UCHAR);

				Status = STATUS_SUCCESS;
			break;
			
			case IOCTL_LISTDNAF:
				Status = STATUS_UNSUCCESSFUL;
				if (extension->WriteInstructionPtr < INSTRUCTIONMEM)
				{
					extension->InstructionList[extension->WriteInstructionPtr].type = *(PUSHORT)pIOBuffer++;
					extension->InstructionList[extension->WriteInstructionPtr].d = *(PULONG)pIOBuffer++;
					extension->InstructionList[extension->WriteInstructionPtr].n = *(PUCHAR)pIOBuffer++;
					extension->InstructionList[extension->WriteInstructionPtr].a = *(PUCHAR)pIOBuffer++;
					extension->InstructionList[extension->WriteInstructionPtr].f = *(PUCHAR)pIOBuffer;
					if(extension->InstructionList[extension->WriteInstructionPtr].type == READGPIB)
					{
						extension->GPIBPending = TRUE;
					}
					extension->WriteInstructionPtr = extension->WriteInstructionPtr + 1;

				
					Status = STATUS_SUCCESS;
					pIOBuffer     = (PULONG)pIrp->AssociatedIrp.SystemBuffer;
					*(PULONG)pIOBuffer = extension->WriteInstructionPtr;
					pIrp->IoStatus.Information = sizeof(ULONG);
				}
			break;


			case IOCTL_SHOWLISTPTR:
				Status = STATUS_SUCCESS;
				pIOBuffer     = (PULONG)pIrp->AssociatedIrp.SystemBuffer;
				*(PULONG)pIOBuffer = extension->WriteInstructionPtr;
				pIrp->IoStatus.Information = sizeof(ULONG);
			break;

			case IOCTL_SHOWDATAPTR:
				Status = STATUS_SUCCESS;
				pIOBuffer     = (PULONG)pIrp->AssociatedIrp.SystemBuffer;
				*(PULONG)pIOBuffer = extension->ReadDataPtr;
				pIrp->IoStatus.Information = sizeof(ULONG);
			break;

			case IOCTL_SHOWSTRINGPTR:
				Status = STATUS_SUCCESS;
				pIOBuffer     = (PULONG)pIrp->AssociatedIrp.SystemBuffer;
				*(PULONG)pIOBuffer = extension->ReadStringPtr;
				pIrp->IoStatus.Information = sizeof(ULONG);
			break;

			case IOCTL_SHOWACTUALLISTPTR:
				Status = STATUS_SUCCESS;
				pIOBuffer     = (PULONG)pIrp->AssociatedIrp.SystemBuffer;
				*(PULONG)pIOBuffer = extension->ActualInstructionPtr;
				pIrp->IoStatus.Information = sizeof(ULONG);
			break;

			case IOCTL_SHOWACTUALDATAPTR:
				Status = STATUS_SUCCESS;
				pIOBuffer     = (PULONG)pIrp->AssociatedIrp.SystemBuffer;
				*(PULONG)pIOBuffer = extension->ActualDataPtr;
				pIrp->IoStatus.Information = sizeof(ULONG);
			break;
			
			case IOCTL_SHOWACTUALSTRINGPTR:
				Status = STATUS_SUCCESS;
				pIOBuffer     = (PULONG)pIrp->AssociatedIrp.SystemBuffer;
				*(PULONG)pIOBuffer = extension->ActualStringPtr;
				pIrp->IoStatus.Information = sizeof(ULONG);
			break;

			case IOCTL_LISTSTART:
				extension->ActualInstructionPtr = 0;
				extension->ListPending			= TRUE;
				while ((extension->ActualInstructionPtr < extension->WriteInstructionPtr)&&
						(extension->ActualInstructionPtr < INSTRUCTIONMEM))
				{
					Index = extension->ActualInstructionPtr;
					InstructionType = extension->InstructionList[Index].type & INSTRUCTIONMASK;
					switch (InstructionType)
					{
						case SENDF:
							SendF(extension,extension->InstructionList[Index].f);
							ReadCSR(extension);
						break;
						case SENDNAF:
							SendNAF(extension,extension->InstructionList[Index].n,
											extension->InstructionList[Index].a,
											extension->InstructionList[Index].f);
							ReadCSR(extension);
						break;
						case SENDDNAF:
							SendDNAF(extension,extension->InstructionList[Index].d,
											extension->InstructionList[Index].n,
											extension->InstructionList[Index].a,
											extension->InstructionList[Index].f);
							ReadCSR(extension);
						break;
						case READD:
							extension->ReadData[extension->ActualDataPtr] = ReadD(extension);
							extension->ActualDataPtr++;
						break;
						case READCSR:
							ReadCSR(extension);
						break;
						case READENCL:
							ReadENCL(extension);
						break;
						case READGPIB:
							ReadENCL(extension);
							extension->GPIBAddress = extension->InstructionList[Index].n;
							extension->GPIBListening = TRUE;
						break;
					}
					extension->ActualInstructionPtr++;
					if (((extension->InstructionList[Index].type & WAITINT)!=0)||extension->GPIBListening) break;
				}
				if(extension->ActualInstructionPtr == extension->WriteInstructionPtr)extension->ListPending = FALSE;
				Status = STATUS_SUCCESS;
			break;
			
			case IOCTL_LISTREADD:
				Status = STATUS_UNSUCCESSFUL;
				if ((extension->ReadDataPtr < DATAMEM)&&(extension->ReadDataPtr < extension->ActualDataPtr))
				{
					*(PULONG)pIOBuffer = extension->ReadData[extension->ReadDataPtr];
					extension->ReadDataPtr++;
					pIrp->IoStatus.Information = sizeof(ULONG);
					Status = STATUS_SUCCESS;
				}
			break;
			
			case IOCTL_LISTPTRRESET:
				
				extension->WriteInstructionPtr	= 0;
				extension->GPIBListening		= FALSE;
				extension->GPIBPending			= FALSE;
				extension->ListPending			= FALSE;
				Status = STATUS_SUCCESS;
			break;

			case IOCTL_DATAPTRRESET:
				extension->ActualDataPtr		= 0;
				extension->ActualInstructionPtr = 0;
				extension->ReadDataPtr			= 0;
				extension->ReadStringPtr		= 0;
				extension->ActualStringPtr		= 0;
				extension->ActualStringPosPtr	= 1;
				Status = STATUS_SUCCESS;
			break;

			case IOCTL_GPIBREADY:
				if(!extension->GPIBPending)Status = STATUS_SUCCESS;
				else Status = STATUS_UNSUCCESSFUL;
				pIrp->IoStatus.Information = 0;
			break;

			case IOCTL_LISTREADY:
				if(!extension->ListPending)Status = STATUS_SUCCESS;
				else Status = STATUS_UNSUCCESSFUL;
				pIrp->IoStatus.Information = 0;
			break;

			case IOCTL_GPIBREADSTRING:
				j = extension->ReadString[extension->ReadStringPtr][0];
				for(i=1;i<50;i++)
				{
					*(((PUCHAR)pIOBuffer)+i-1) = (UCHAR)extension->ReadString[extension->ReadStringPtr][i];
				}
				extension->ReadStringPtr++;
				if (extension->ReadStringPtr>=STRINGMEM)extension->ReadStringPtr = 0;
				pIrp->IoStatus.Information = j;
				Status = STATUS_SUCCESS;
			break;

            case IOCTL_GPD_READ_PORT_UCHAR:
            case IOCTL_GPD_READ_PORT_USHORT:
            case IOCTL_GPD_READ_PORT_ULONG:
                Status = GpdIoctlReadPort(
                            extension,
                            pIrp,
                            pIrpStack,
                            pIrpStack->Parameters.DeviceIoControl.IoControlCode
                            );
                break;

            case IOCTL_GPD_WRITE_PORT_UCHAR:
            case IOCTL_GPD_WRITE_PORT_USHORT:
            case IOCTL_GPD_WRITE_PORT_ULONG:
                Status = GpdIoctlWritePort(
                            extension, 
                            pIrp,
                            pIrpStack,
                            pIrpStack->Parameters.DeviceIoControl.IoControlCode
                            );
                break;
            }
            break;
    }

    // We're done with I/O request.  Record the status of the I/O action.
    pIrp->IoStatus.Status = Status;

    // Don't boost priority when returning since this took little time.
    IoCompleteRequest(pIrp, IO_NO_INCREMENT );

    return Status;
}


NTSTATUS
GpdIoctlReadPort(
    IN PLOCAL_DEVICE_INFO extension,
    IN PIRP pIrp,
    IN PIO_STACK_LOCATION IrpStack,
    IN ULONG IoctlCode  )


/*++

Routine Description:
    This routine processes the IOCTLs which read from the ports.

Arguments:
    
    extension        - our local device data
    pIrp        - IO request packet
    IrpStack    - The current stack location
    IoctlCode   - The ioctl code from the IRP

Return Value:
    STATUS_SUCCESS           -- OK

    STATUS_INVALID_PARAMETER -- The buffer sent to the driver
                                was too small to contain the
                                port, or the buffer which
                                would be sent back to the driver
                                was not a multiple of the data size.

    STATUS_ACCESS_VIOLATION  -- An illegal port number was given.

--*/

{
                                // NOTE:  Use METHOD_BUFFERED ioctls.
    PULONG pIOBuffer;           // Pointer to transfer buffer
                                //      (treated as an array of longs).
    ULONG InBufferSize;         // Amount of data avail. from caller.
    ULONG OutBufferSize;        // Max data that caller can accept.
    ULONG nPort;                // Port number to read
    ULONG DataBufferSize;

    // Size of buffer containing data from application
    InBufferSize  = IrpStack->Parameters.DeviceIoControl.InputBufferLength;

    // Size of buffer for data to be sent to application
    OutBufferSize = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;

    // NT copies inbuf here before entry and copies this to outbuf after
    // return, for METHOD_BUFFERED IOCTL's.
    pIOBuffer     = (PULONG)pIrp->AssociatedIrp.SystemBuffer;

    // Check to ensure input buffer is big enough to hold a port number and
    // the output buffer is at least as big as the port data width.
    //
    switch (IoctlCode)
    {
    default:                    // There isn't really any default but 
	/* FALL THRU */         // this will quiet the compiler.
    case IOCTL_GPD_READ_PORT_UCHAR:
        DataBufferSize = sizeof(UCHAR);
        break;
    case IOCTL_GPD_READ_PORT_USHORT:
        DataBufferSize = sizeof(USHORT);
        break;
    case IOCTL_GPD_READ_PORT_ULONG:
        DataBufferSize = sizeof(ULONG);
        break;
    }

    if ( InBufferSize != sizeof(ULONG) || OutBufferSize < DataBufferSize )
    {
        return STATUS_INVALID_PARAMETER;
    }

    // Buffers are big enough.

    nPort = *pIOBuffer;             // Get the I/O port number from the buffer.
	
	/*
    if (nPort >= extension->PortCount ||
        (nPort + DataBufferSize) > extension->PortCount ||
        (((ULONG)extension->PortBase + nPort) & (DataBufferSize - 1)) != 0)
    {
        return STATUS_ACCESS_VIOLATION;   // It was not legal.
    }
    */

    if (extension->PortMemoryType == 1)
    {
        // Address is in I/O space
        
        switch (IoctlCode)
        {
        case IOCTL_GPD_READ_PORT_UCHAR:
            *(PUCHAR)pIOBuffer = READ_PORT_UCHAR(
                            (PUCHAR)((ULONG)extension->PortBase + nPort) );
            break;
        case IOCTL_GPD_READ_PORT_USHORT:
            *(PUSHORT)pIOBuffer = READ_PORT_USHORT(
                            (PUSHORT)((ULONG)extension->PortBase + nPort) );
            break;
        case IOCTL_GPD_READ_PORT_ULONG:
            *(PULONG)pIOBuffer = READ_PORT_ULONG(
                            (PULONG)((ULONG)extension->PortBase + nPort) );
            break;
        }
    } else {
        // Address is in Memory space
        
        switch (IoctlCode)
        {
        case IOCTL_GPD_READ_PORT_UCHAR:
            *(PUCHAR)pIOBuffer = READ_REGISTER_UCHAR(
                            (PUCHAR)((ULONG)extension->PortBase + nPort) );
            break;
        case IOCTL_GPD_READ_PORT_USHORT:
            *(PUSHORT)pIOBuffer = READ_REGISTER_USHORT(
                            (PUSHORT)((ULONG)extension->PortBase + nPort) );
            break;
        case IOCTL_GPD_READ_PORT_ULONG:
            *(PULONG)pIOBuffer = READ_REGISTER_ULONG(
                            (PULONG)((ULONG)extension->PortBase + nPort) );
            break;
        }
    }
    
    // Indicate # of bytes read
    //
    
    pIrp->IoStatus.Information = DataBufferSize;

    return STATUS_SUCCESS;
}


NTSTATUS
GpdIoctlWritePort(
    IN PLOCAL_DEVICE_INFO extension, 
    IN PIRP pIrp, 
    IN PIO_STACK_LOCATION IrpStack,
    IN ULONG IoctlCode
    )

/*++

Routine Description:
    This routine processes the IOCTLs which write to the ports.

Arguments:
    
    extension        - our local device data
    pIrp        - IO request packet
    IrpStack    - The current stack location
    IoctlCode   - The ioctl code from the IRP

Return Value:
    STATUS_SUCCESS           -- OK

    STATUS_INVALID_PARAMETER -- The buffer sent to the driver
                                was too small to contain the
                                port, or the buffer which
                                would be sent back to the driver
                                was not a multiple of the data size.

    STATUS_ACCESS_VIOLATION  -- An illegal port number was given.

--*/

{
                                // NOTE:  Use METHOD_BUFFERED ioctls.
    PULONG pIOBuffer;           // Pointer to transfer buffer
                                //      (treated as array of longs).
    ULONG InBufferSize ;        // Amount of data avail. from caller.
    ULONG OutBufferSize ;       // Max data that caller can accept.
    ULONG nPort;                // Port number to read or write.
    ULONG DataBufferSize;

    // Size of buffer containing data from application
    InBufferSize  = IrpStack->Parameters.DeviceIoControl.InputBufferLength;

    // Size of buffer for data to be sent to application
    OutBufferSize = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;

    // NT copies inbuf here before entry and copies this to outbuf after return,
    // for METHOD_BUFFERED IOCTL's.
    pIOBuffer     = (PULONG) pIrp->AssociatedIrp.SystemBuffer;

    // We don't return any data on a write port.
    pIrp->IoStatus.Information = 0;
    
    // Check to ensure input buffer is big enough to hold a port number as well
    // as the data to write.
    //
    // The relative port # is a ULONG, and the data is the type appropriate to
    // the IOCTL.
    //

    switch (IoctlCode)
    {
    default:                    // There isn't really any default but 
	/* FALL THRU */         // this will quiet the compiler.
    case IOCTL_GPD_WRITE_PORT_UCHAR:
        DataBufferSize = sizeof(UCHAR);
        break;
    case IOCTL_GPD_WRITE_PORT_USHORT:
        DataBufferSize = sizeof(USHORT);
        break;
    case IOCTL_GPD_WRITE_PORT_ULONG:
        DataBufferSize = sizeof(ULONG);
        break;
    }

    if ( InBufferSize < (sizeof(ULONG) + DataBufferSize) )
    {
        return STATUS_INVALID_PARAMETER;
    }

    nPort = *pIOBuffer++;
	
	/*
    if (nPort >= extension->PortCount ||
        (nPort + DataBufferSize) > extension->PortCount ||
        (((ULONG)extension->PortBase + nPort) & (DataBufferSize - 1)) != 0)
    {
        return STATUS_ACCESS_VIOLATION;   // Illegal port number
    }
	*/

    if (extension->PortMemoryType == 1)
    {
        // Address is in I/O space
        
        switch (IoctlCode)
        {
        case IOCTL_GPD_WRITE_PORT_UCHAR:
            WRITE_PORT_UCHAR(
                (PUCHAR)((ULONG)extension->PortBase + nPort),
                *(PUCHAR)pIOBuffer );
            break;
        case IOCTL_GPD_WRITE_PORT_USHORT:
            WRITE_PORT_USHORT(
                (PUSHORT)((ULONG)extension->PortBase + nPort),
                *(PUSHORT)pIOBuffer );
            break;
        case IOCTL_GPD_WRITE_PORT_ULONG:
            WRITE_PORT_ULONG(
                (PULONG)((ULONG)extension->PortBase + nPort),
                *(PULONG)pIOBuffer );
            break;
        }
    } else {
        // Address is in Memory space
        
        switch (IoctlCode)
        {
        case IOCTL_GPD_WRITE_PORT_UCHAR:
            WRITE_REGISTER_UCHAR( 
                    (PUCHAR)((ULONG)extension->PortBase + nPort),
                    *(PUCHAR)pIOBuffer );
            break;
        case IOCTL_GPD_WRITE_PORT_USHORT:
            WRITE_REGISTER_USHORT(
                    (PUSHORT)((ULONG)extension->PortBase + nPort),
                    *(PUSHORT)pIOBuffer );
            break;
        case IOCTL_GPD_WRITE_PORT_ULONG:
            WRITE_REGISTER_ULONG(
                    (PULONG)((ULONG)extension->PortBase + nPort),
                    *(PULONG)pIOBuffer );
            break;
        }
    }

    return STATUS_SUCCESS;
}


VOID
GpdUnload(
    PDRIVER_OBJECT DriverObject
    )

/*++

Routine Description:
    This routine prepares our driver to be unloaded.  It is responsible
    for freeing all resources allocated by DriverEntry as well as any 
    allocated while the driver was running.  The symbolic link must be
    deleted as well.

Arguments:
    
    DriverObject - Pointer to driver object created by the system.

Return Value:

    None

--*/

{
    PLOCAL_DEVICE_INFO extension;
    CM_RESOURCE_LIST NullResourceList;
    BOOLEAN ResourceConflict;
    UNICODE_STRING Win32DeviceName;

    // Find our global data
    extension = (PLOCAL_DEVICE_INFO)DriverObject->DeviceObject->DeviceExtension;
	ZwClose (extension->Handle) ;
    // Unmap the ports

    if (extension->PortWasMapped)
    {
        MmUnmapIoSpace(extension->PortBase, extension->PortCount);
    }

    // Report we're not using any hardware.  If we don't do this
    // then we'll conflict with ourselves (!) on the next load

	IoDisconnectInterrupt(extension->InterruptObject);

    RtlZeroMemory((PVOID)&NullResourceList, sizeof(NullResourceList));

    IoReportResourceUsage(
              NULL,
              DriverObject,
              &NullResourceList,
              sizeof(ULONG),
              NULL,
              NULL,
              0,
              FALSE,
              &ResourceConflict );

    // Assume all handles are closed down.
    // Delete the things we allocated - devices, symbolic links

    RtlInitUnicodeString(&Win32DeviceName, DOS_DEVICE_NAME);

    IoDeleteSymbolicLink(&Win32DeviceName);

    IoDeleteDevice(extension->DeviceObject);
}
