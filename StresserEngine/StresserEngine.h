#pragma once

#include <ntifs.h>

/* The Driver's unload function called whenever the kernel unloads the driver. */
DRIVER_UNLOAD StresserEngineUnload;

/* The Driver's default dispatch to all the unimplemented dispatch. */
DRIVER_DISPATCH StresserEngineDefaultDispatch;

/* The Driver's default dispatch for create and close dispatch. */
DRIVER_DISPATCH StresserEngineCreateClose;

/* Dispatch function for device control I\O requests. */
DRIVER_DISPATCH StresserEngineDeviceControl;

/* Helper function to return the IRP to the caller. */
NTSTATUS CompleteIrp(PIRP Irp, NTSTATUS status = STATUS_SUCCESS, ULONG_PTR info = 0);

/* Handler function for registering event  from user mode. */
NTSTATUS RegisterEventHandler(_In_ PIRP Irp, _In_ PIO_STACK_LOCATION StackLocation);

/* Handler function for adding fake process ID to be monitored. */
NTSTATUS AddFakeProcessIdHandler(_In_ PIRP Irp, _In_ PIO_STACK_LOCATION StackLocation);

/* Handler function for removing fake process ID from being monitored. */
NTSTATUS RemoveFakeProcessIdHandler(_In_ PIRP Irp, _In_ PIO_STACK_LOCATION StackLocation);