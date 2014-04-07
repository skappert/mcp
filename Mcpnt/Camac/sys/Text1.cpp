IO_RESOURCE_DESCRIPTOR ioResource = { 
                    IO_RESOURCE_PREFERRED, 
                    CmResourceTypeMemory, 
                    CmResourceShareDeviceExclusive, 
                    0, 
                    CM_RESOURCE_MEMORY_READ_WRITE, 
                    0, 
                    { 
                      NEW_MMIO_WINDOW_SIZE,         // Length 
                      NEW_MMIO_WINDOW_SIZE,         // Alignment 
                      { NEW_MMIO_WINDOW_SIZE, 0 },  // Minimum start address 
                      { 0xffffffff, 0}              // Maximum end address 
                    } 
                }; 



NTSTATUS 
	IoAssignResources(
		IN PUNICODE_STRING  RegistryPath,
		IN PUNICODE_STRING  DriverClassName,		/* optional */
		IN PDRIVER_OBJECT  DriverObject,
		IN PDEVICE_OBJECT  DeviceObject,			/* optional */
		IN PIO_RESOURCE_REQUIREMENTS_LIST  RequestedResources,
		IN OUT PCM_RESOURCE_LIST  *AllocatedResources
		);


NTSTATUS 
	IoReportResourceUsage(
		IN PUNICODE_STRING  DriverClassName,	/* optional */
		IN PDRIVER_OBJECT  DriverObject,
		IN PCM_RESOURCE_LIST  DriverList,		/* optional */
		IN ULONG  DriverListSize,			/* optional */
		IN PDEVICE_OBJECT  DeviceObject,
		IN PCM_RESOURCE_LIST  DeviceList,	/* optional */
		IN ULONG  DeviceListSize,			/* optional */
		IN BOOLEAN  OverrideConflict,
		OUT PBOOLEAN  ConflictDetected
		);

