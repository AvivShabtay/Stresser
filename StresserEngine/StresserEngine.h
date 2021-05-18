#pragma once

#include "DriverConfiguration.h"
#include "StresserEngineCommon.h"
#include "Vector.h"
#include "LinkedList.h"
#include "Event.h"

#include <ntifs.h>


/*
	Represent event in the item's linked list.
 */
template<typename T>
struct EventItem
{
	LIST_ENTRY Entry; // Must named with capital 'E'
	T data;
};


/*
	Represent the context each notification callback will receive.
	This data used for the callback logic.
 */
typedef struct _NotificationContext
{
	FastMutex mutex;
	Vector<FakeProcessId*, DRIVER_TAG>* fakeProcessIds;
	LinkedList<EventItem<EventInfo>, FastMutex>* fakeProcessEvents;
	Event* onFakeProcessEvent;
} NotificationContext, * PNotificationContext;


/*
	Represent driver defined data structure used for the driver operations.
 */
typedef struct _DeviceExtension
{
	PNotificationContext notificationContext;
	PVOID objectNotificationRegistrationHandle;
} DeviceExtension, * PDeviceExtension;

constexpr ULONG DEVICE_EXTENSION_SIZE = sizeof(DEVICE_EXTENSION_SIZE);

/* The Driver's unload function called whenever the kernel unloads the driver. */
DRIVER_UNLOAD StresserEngineUnload;

/* The Driver's default dispatch to all the unimplemented dispatch. */
DRIVER_DISPATCH StresserEngineDefaultDispatch;

/* The Driver's default dispatch for create and close dispatch. */
DRIVER_DISPATCH StresserEngineCreateClose;

/* Dispatch function for device control I\O requests. */
DRIVER_DISPATCH StresserEngineDeviceControl;

/* Helper function to return the IRP to the caller. */
NTSTATUS completeIrp(PIRP Irp, NTSTATUS status = STATUS_SUCCESS, ULONG_PTR info = 0);

/* Handler function for registering event  from user mode. */
NTSTATUS registerEventHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp, _In_ PIO_STACK_LOCATION StackLocation);

/* Handler function for adding fake process ID to be monitored. */
NTSTATUS addFakeProcessIdHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp, _In_ PIO_STACK_LOCATION StackLocation);

/* Handler function for removing fake process ID from being monitored. */
NTSTATUS removeFakeProcessIdHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp, _In_ PIO_STACK_LOCATION StackLocation);

/* Handler function for passing event to user mode. */
NTSTATUS getEventsHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp, _In_ PIO_STACK_LOCATION StackLocation);

/* Allocate shared memory used for object notification callbacks. */
Value<PVOID, StresserString> createNotificationContext(PDEVICE_OBJECT DeviceObject);

/* Free shared memory used in object notification callbacks. */
Value<bool, StresserString> releaseNotificationContext(PDEVICE_OBJECT DeviceObject);

/* Signal shared event object. */
bool signalFakeProcessEvent(Event* eventToSignal);