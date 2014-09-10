/*++
    Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid for Hytec device class.
// This GUID is used to register (IoRegisterDeviceInterface)
// an instance of an interface so that user application
// can control the toaster device.
//
// {7B2C3242-FD98-4391-8D46-0AD83481674E}
DEFINE_GUID(GUID_DEVINTERFACE_PCIDRV, 
0x7b2c3242, 0xfd98, 0x4391, 0x8d, 0x46, 0xa, 0xd8, 0x34, 0x81, 0x67, 0x4e);

