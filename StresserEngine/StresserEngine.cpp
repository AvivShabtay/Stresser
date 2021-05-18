#include "StresserEngine.h"
#include "DriverConfiguration.h"
#include "StresserEngineCommon.h"
#include "HelperMacros.h"
#include "IrpUtils.h"
#include "ObjectNotification.h"
#include "Memory.h"
#include "ProcessUtils.h"

/*
 * Driver entry point.
 * Responsible for creating driver's device object, allocate driver's shared memory,
 * initialize data structures, register to dispatch function, etc.
 */
extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	KdPrint((DRIVER_PREFIX "DriverEntry started\n"));

	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT deviceObject = nullptr;
	PDeviceExtension deviceExtension = nullptr;
	UNICODE_STRING symLink = RTL_CONSTANT_STRING(SYMBOLIC_LINK_PATH);
	bool symbolicLinkCreated = false;

	do
	{
		// Create device object for user-mode communication:
		UNICODE_STRING devName = RTL_CONSTANT_STRING(DEVICE_NAME);
		status = IoCreateDevice(DriverObject, DEVICE_EXTENSION_SIZE, &devName, FILE_DEVICE_UNKNOWN, 0, TRUE, &deviceObject);
		PRINT_STATUS_SUCCESS_FAILURE(status, "Create device object successfully", "Failed to create device");
		BREAK_ON_FAILURE(status);

		// Set the IO communication method:
		deviceObject->Flags |= DO_DIRECT_IO;

		// Get device extensions:
		deviceExtension = static_cast<PDeviceExtension>(deviceObject->DeviceExtension);
		deviceExtension->notificationContext = nullptr;
		deviceExtension->objectNotificationRegistrationHandle = nullptr;

		// Create symbolic link to the device object:
		status = IoCreateSymbolicLink(&symLink, &devName);
		PRINT_STATUS_SUCCESS_FAILURE(status, "Create symbolic link successfully", "Failed to create symbolic link");
		BREAK_ON_FAILURE(status);

		symbolicLinkCreated = true;

		LOG_MESSAGE("allocate memory for notification event");

		// Register to object notifications:
		// TODO: Verify we are on Vista and above, otherwise: "OS version doesn't support Object notifications"

		OB_OPERATION_REGISTRATION objectOperationsRegistration[STRESSER_OBJECT_CALLBACK_COUNT];

		static_assert(STRESSER_PROCESS_OBJECT_TYPE < STRESSER_OBJECT_CALLBACK_COUNT, "Out of bounds");

		// Set object notification callbacks:
		objectOperationsRegistration[STRESSER_PROCESS_OBJECT_TYPE] =
		{
			STRESSER_OBJECT_NOTIFICATION_TYPE,	// ObjectType
			OB_OPERATION_HANDLE_CREATE,			// Operations
			StresserEngineObPreOpCallback,		// PreOperation
			StresserEngineObPostOpCallback		// PostOperation
		};

		const UNICODE_STRING altitude = RTL_CONSTANT_STRING(DRIVER_ALTITUDE);

		// Create shared memory for notifications:
		Value<PVOID, StresserString> result = createNotificationContext(deviceObject);
		if (result.isError())
		{
			LOG_MESSAGE("could not allocate memory for notification context");
			status = STATUS_INSUFFICIENT_RESOURCES;
			BREAK_ON_FAILURE(status);
		}

		// Add shared structure to device extensions:
		deviceExtension->notificationContext = static_cast<PNotificationContext>(result.getValue());

		// Set object notification registration:
		OB_CALLBACK_REGISTRATION objectCallbackRegistration =
		{
			OB_FLT_REGISTRATION_VERSION,		// Version
			STRESSER_OBJECT_CALLBACK_COUNT,		// OperationRegistrationCount
			altitude,							// Altitude
			deviceExtension->notificationContext, // RegistrationContext
			objectOperationsRegistration		// OperationRegistration
		};

		// Register to object notifications:
		status = ObRegisterCallbacks(&objectCallbackRegistration, &deviceExtension->objectNotificationRegistrationHandle);
		PRINT_STATUS_SUCCESS_FAILURE(status, "Register to object notifications successfully", "Failed to register to object notifications");
		BREAK_ON_FAILURE(status);

	} while (false);

	// In case of failure:
	if (!NT_SUCCESS(status))
	{
		if (symbolicLinkCreated)
		{
			IoDeleteSymbolicLink(&symLink);
		}
		if (nullptr != deviceExtension && nullptr != deviceExtension->objectNotificationRegistrationHandle)
		{
			ObUnRegisterCallbacks(deviceExtension->objectNotificationRegistrationHandle);
		}
		if (nullptr != deviceExtension && nullptr != deviceExtension->notificationContext)
		{
			releaseNotificationContext(deviceObject);
		}
		// MUST be after freeing other resources:
		if (nullptr != deviceObject)
		{
			IoDeleteDevice(deviceObject);
		}
	}

	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; ++i)
	{
		DriverObject->MajorFunction[i] = StresserEngineDefaultDispatch;
	}

	DriverObject->DriverUnload = StresserEngineUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = StresserEngineCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = StresserEngineCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = StresserEngineDeviceControl;

	LOG_MESSAGE("DriverEntry completed successfully\n");

	return status;
}

void StresserEngineUnload(PDRIVER_OBJECT DriverObject)
{
	LOG_MESSAGE(STRINGIFY(StresserEngineUnload) " started");

	// Remove symbolic link:
	UNICODE_STRING symLink = RTL_CONSTANT_STRING(SYMBOLIC_LINK_PATH);
	const NTSTATUS status = IoDeleteSymbolicLink(&symLink);
	PRINT_STATUS_SUCCESS_FAILURE(status, "remove symbolic link successfully", "failed to delete symbolic link");

	// Get driver's device object:
	const PDEVICE_OBJECT deviceObject = DriverObject->DeviceObject;
	PRINT_MESSAGE_AND_RETURN_ON_CONDITION(nullptr == deviceObject,
		STRINGIFY(StresserEngineUnload) " device object is null, cannot continue cleanup, returning...");

	// Release notification context memory:
	const Value<bool, StresserString> result = releaseNotificationContext(deviceObject);
	if (result.isError())
	{
		LOG_MESSAGE("could not release notification context memory");
	}

	LOG_MESSAGE("release notification context memory");

	// Get device extensions:
	auto* deviceExtensions = static_cast<PDeviceExtension>(deviceObject->DeviceExtension);
	PRINT_MESSAGE_AND_RETURN_ON_CONDITION(nullptr == deviceExtensions,
		STRINGIFY(StresserEngineUnload) " device extensions is null, cannot continue cleanup, returning...");

	// Remove registered callbacks:
	if (nullptr != deviceExtensions->objectNotificationRegistrationHandle)
	{
		//ObUnRegisterCallbacks(g_registrationHandle);
		ObUnRegisterCallbacks(deviceExtensions->objectNotificationRegistrationHandle);
		LOG_MESSAGE("remove object notification callback successfully");
	}

	// Remove device object
	IoDeleteDevice(deviceObject);
	LOG_MESSAGE("delete device object");

	LOG_MESSAGE(STRINGIFY(StresserEngineUnload) " completed successfully");
}

NTSTATUS StresserEngineCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, 0);
	return STATUS_SUCCESS;
}

NTSTATUS StresserEngineDefaultDispatch(PDEVICE_OBJECT, PIRP Irp)
{
	Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, 0);
	return STATUS_NOT_SUPPORTED;
}

NTSTATUS completeIrp(PIRP Irp, NTSTATUS status, ULONG_PTR info)
{
	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = info;
	IoCompleteRequest(Irp, 0);
	return status;
}

NTSTATUS StresserEngineDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	KdPrint((DRIVER_PREFIX "DeviceControl started\n"));

	const PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	const ULONG controlCode = stack->Parameters.DeviceIoControl.IoControlCode;
	NTSTATUS status = STATUS_NOT_SUPPORTED;

	if (nullptr == Irp)
	{
		return completeIrp(Irp, status);
	}

	// Run the corresponding handler for the request:
	switch (controlCode) {
	case IOCTL_STRESSER_ENGINE_REGISTER_EVENT:
	{
		status = registerEventHandler(DeviceObject, Irp, stack);
		break;
	}
	case IOCTL_STRESSER_ENGINE_ADD_FAKE_PID:
	{
		status = addFakeProcessIdHandler(DeviceObject, Irp, stack);
		break;
	}
	case IOCTL_STRESSER_ENGINE_REMOVE_FAKE_PID:
	{
		status = removeFakeProcessIdHandler(DeviceObject, Irp, stack);
		break;
	}
	case IOCTL_STRESSER_ENGINE_GET_EVENTS:
	{
		status = getEventsHandler(DeviceObject, Irp, stack);
		break;
	}
	default:
	{
		status = STATUS_INVALID_DEVICE_REQUEST;
		break;
	}
	}

	LOG_MESSAGE("DeviceControl completed successfully\n");

	return completeIrp(Irp, status, Irp->IoStatus.Information);
}

NTSTATUS registerEventHandler(PDEVICE_OBJECT DeviceObject, PIRP Irp, PIO_STACK_LOCATION StackLocation)
{
	LOG_MESSAGE(STRINGIFY(RegisterEventHandler) " started");

	if (!IrpUtils::isValidInputBuffer(StackLocation, REGISTER_EVENT_SIZE))
	{
		LOG_MESSAGE(STRINGIFY(RegisterEventHandler) " user buffer is invalid");

		return STATUS_BUFFER_TOO_SMALL;
	}

	auto* const registerEvent = static_cast<RegisterEvent*>(Irp->AssociatedIrp.SystemBuffer);
	if (nullptr == registerEvent)
	{
		LOG_MESSAGE(STRINGIFY(RegisterEventHandler) " Invalid memory");
		STATUS_INVALID_PARAMETER;
	}

	const StresserString eventName(registerEvent->eventName);

	auto* deviceExtensions = static_cast<PDeviceExtension>(DeviceObject->DeviceExtension);
	auto* notificationContext = deviceExtensions->notificationContext;
	auto* onFakeProcessEvent = notificationContext->onFakeProcessEvent;

	Value<bool, NTSTATUS> result = onFakeProcessEvent->open(eventName);

	if (result.isError())
	{
		RETURN_ON_STATUS_FAILURE(result.getError(), "could not open event by the given event name");
	}

	LOG_MESSAGE(STRINGIFY(RegisterEventHandler) " completed successfully");

	return STATUS_SUCCESS;
}

NTSTATUS addFakeProcessIdHandler(PDEVICE_OBJECT DeviceObject, PIRP Irp, PIO_STACK_LOCATION StackLocation)
{
	LOG_MESSAGE(STRINGIFY(addFakeProcessIdHandler) " started");

	if (!IrpUtils::isValidInputBuffer(StackLocation, FAKE_PROCESS_ID_SIZE))
	{
		LOG_MESSAGE(STRINGIFY(addFakeProcessIdHandler) " user buffer is invalid");

		return STATUS_BUFFER_TOO_SMALL;
	}

	auto* const fakeProcessId = static_cast<FakeProcessId*>(Irp->AssociatedIrp.SystemBuffer);
	if (nullptr == fakeProcessId)
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(addFakeProcessIdHandler) " Invalid type for request\n"));
		return STATUS_INVALID_PARAMETER;
	}

	if (!ProcessUtils::doesValidProcessId(fakeProcessId->processId))
	{
		LOG_MESSAGE(STRINGIFY(addFakeProcessIdHandler) " Invalid process ID");
		return STATUS_INVALID_PARAMETER;
	}

	auto* processId = new (NonPagedPool, DRIVER_TAG) FakeProcessId;

	processId->processId = fakeProcessId->processId;

	auto* deviceExtensions = static_cast<PDeviceExtension>(DeviceObject->DeviceExtension);
	auto* notificationContext = deviceExtensions->notificationContext;

	{

		AutoLock lock(notificationContext->mutex);

		auto* fakeProcessIds = notificationContext->fakeProcessIds;
		fakeProcessIds->add(processId);
	}

	KdPrint((DRIVER_PREFIX STRINGIFY(addFakeProcessIdHandler)" Add fake process ID=%d\n", processId->processId));
	LOG_MESSAGE(STRINGIFY(addFakeProcessIdHandler) " completed successfully");

	return STATUS_SUCCESS;
}

NTSTATUS removeFakeProcessIdHandler(PDEVICE_OBJECT DeviceObject, PIRP Irp, PIO_STACK_LOCATION StackLocation)
{
	KdPrint((DRIVER_PREFIX STRINGIFY(removeFakeProcessIdHandler) " started\n"));

	if (!IrpUtils::isValidInputBuffer(StackLocation, FAKE_PROCESS_ID_SIZE))
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(RemoveFakeProcessIdHandler) " user buffer is invalid\n"));

		return STATUS_BUFFER_TOO_SMALL;
	}

	auto* const fakeProcessId = static_cast<FakeProcessId*>(Irp->AssociatedIrp.SystemBuffer);
	if (nullptr == fakeProcessId)
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(removeFakeProcessIdHandler) " invalid type for request\n"));
		return STATUS_INVALID_PARAMETER;
	}

	if (!ProcessUtils::doesValidProcessId(fakeProcessId->processId))
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(removeFakeProcessIdHandler) " invalid process ID\n"));
		return STATUS_INVALID_PARAMETER;
	}

	auto* deviceExtensions = static_cast<PDeviceExtension>(DeviceObject->DeviceExtension);
	auto* notificationContext = deviceExtensions->notificationContext;

	{
		AutoLock lock(notificationContext->mutex);

		auto* fakeProcessIds = notificationContext->fakeProcessIds;

		for (ULONG i = 0; i < fakeProcessIds->size(); ++i)
		{
			const FakeProcessId* current = fakeProcessIds->getAt(i);
			if (fakeProcessId->processId == current->processId)
			{
				KdPrint((DRIVER_PREFIX STRINGIFY(removeFakeProcessIdHandler)" remove fake process ID=%d\n", current->processId));

				fakeProcessIds->removeAt(i);
				delete current;

				KdPrint((DRIVER_PREFIX STRINGIFY(removeFakeProcessIdHandler) " completed successfully\n"));
				return STATUS_SUCCESS;
			}
		}
	}

	KdPrint((DRIVER_PREFIX STRINGIFY(removeFakeProcessIdHandler) " could not remove process ID=%d\n", fakeProcessId->processId));
	return STATUS_INVALID_PARAMETER;
}

NTSTATUS getEventsHandler(PDEVICE_OBJECT DeviceObject, PIRP Irp, PIO_STACK_LOCATION StackLocation)
{
	LOG_MESSAGE(STRINGIFY(getEventsHandler) " started");

	// Validates input data:
	RETURN_STATUS_ON_CONDITION(!IrpUtils::isValidOutputBuffer(StackLocation, EVENTS_HEADER_SIZE),
		STRINGIFY(getEventsHandler) " events header is too small", STATUS_BUFFER_TOO_SMALL);

	// Get data from the request:
	auto* eventsHeader = static_cast<EventsHeader*>(MmGetSystemAddressForMdlSafe(Irp->MdlAddress, NormalPagePriority));
	RETURN_STATUS_ON_CONDITION(nullptr == eventsHeader,
		STRINGIFY(getEventsHandler) " events buffer is too small", STATUS_INVALID_DEVICE_REQUEST);

	// Validates minimum output buffer size:
	const ULONG eventsBufferSize = eventsHeader->size;
	RETURN_STATUS_ON_CONDITION(eventsBufferSize < 1,
		STRINGIFY(getEventsHandler) " events buffer is too small", STATUS_BUFFER_TOO_SMALL);

	// Validates output buffer:
	auto* const eventsBuffer = reinterpret_cast<EventInfo*>(eventsHeader->events);
	RETURN_STATUS_ON_CONDITION(nullptr == eventsBuffer,
		STRINGIFY(getEventsHandler) " invalid events buffer", STATUS_INVALID_PARAMETER);

	// TODO: Validates output buffer size, not by given user buffer size

	auto* deviceExtensions = static_cast<PDeviceExtension>(DeviceObject->DeviceExtension);
	auto* notificationContext = deviceExtensions->notificationContext;

	{
		AutoLock lock(notificationContext->mutex);

		auto* fakeProcessEvents = notificationContext->fakeProcessEvents;

		// Verify if the list contains any events:
		const ULONG listSize = fakeProcessEvents->size();
		RETURN_STATUS_ON_CONDITION(0 == listSize,
			STRINGIFY(getEventsHandler) " empty events list", STATUS_SUCCESS);

		const ULONG maxIterations = min(listSize, eventsBufferSize);

		for (ULONG i = 0; i < maxIterations; ++i)
		{
			const auto* eventItem = fakeProcessEvents->removeHead();
			const EventInfo& eventInfo = eventItem->data;

			memcpy(&eventsBuffer[i], &eventInfo, EVENT_INFO_SIZE);

			delete eventItem;
		}

		// Pass how much size of buffer have been used:
		Irp->IoStatus.Information = maxIterations * EVENT_INFO_SIZE;
	}

	LOG_MESSAGE(STRINGIFY(getEventsHandler) " completed successfully");
	return STATUS_SUCCESS;
}

bool signalFakeProcessEvent(Event* eventToSignal)
{
	Value<bool, NTSTATUS> result = eventToSignal->set();
	if (result.isError())
	{
		const NTSTATUS status = result.getError();
		PRINT_ON_STATUS_FAILURE(status, "could not open event by the given event name");
		return false;
	}

	LOG_MESSAGE("signal fake process event");

	return true;
}

Value<PVOID, StresserString> createNotificationContext(PDEVICE_OBJECT DeviceObject)
{
	LOG_MESSAGE(STRINGIFY(createNotificationContext) " called");

	auto* notificationContext = new (NonPagedPool, DRIVER_TAG) NotificationContext;
	if (nullptr == notificationContext)
	{
		StresserString errorMessage(L"Could not allocate memory for " STRINGIFY(NotificationContext));
		return errorMessage;
	}

	notificationContext->fakeProcessIds = new (NonPagedPool, DRIVER_TAG) Vector<FakeProcessId*, DRIVER_TAG>();
	if (nullptr == notificationContext->fakeProcessIds)
	{
		StresserString errorMessage(L"Could not allocate memory for " STRINGIFY(Vector));
		return errorMessage;
	}

	notificationContext->fakeProcessEvents = new (NonPagedPool, DRIVER_TAG) LinkedList<EventItem<EventInfo>, FastMutex>();
	if (nullptr == notificationContext->fakeProcessEvents)
	{
		StresserString errorMessage(L"Could not allocate memory for " STRINGIFY(LinkedList));
		return errorMessage;
	}

	notificationContext->onFakeProcessEvent = new (NonPagedPool, DRIVER_TAG) Event();
	if (nullptr == notificationContext->onFakeProcessEvent)
	{
		StresserString errorMessage(L"Could not allocate memory for " STRINGIFY(Event));
		return errorMessage;
	}

	// Initialize global lock object:
	notificationContext->mutex.init();

	LOG_MESSAGE(STRINGIFY(createNotificationContext) " ended");

	auto* deviceExtensions = static_cast<PDeviceExtension>(DeviceObject->DeviceExtension);
	deviceExtensions->notificationContext = notificationContext;

	return notificationContext;
}

Value<bool, StresserString> releaseNotificationContext(PDEVICE_OBJECT DeviceObject)
{
	LOG_MESSAGE(STRINGIFY(releaseNotificationContext) " called");

	auto* deviceExtensions = static_cast<PDeviceExtension>(DeviceObject->DeviceExtension);
	auto* notificationContext = deviceExtensions->notificationContext;

	if (nullptr == notificationContext)
	{
		StresserString errorMessage(L"Notification context pointer already null");
		return errorMessage;
	}

	{
		AutoLock(notificationContext->mutex);

		auto* fakeProcessIds = notificationContext->fakeProcessIds;
		if (nullptr != fakeProcessIds)
		{
			for (auto& fakeProcessId : *fakeProcessIds)
			{
				delete fakeProcessId;
			}
		}

		auto* events = notificationContext->fakeProcessEvents;
		if (nullptr != events || nullptr != events->getHead())
		{
			const ULONG numberOfEvents = events->size();

			for (ULONG i = 0; i < numberOfEvents; ++i)
			{
				EventItem<EventInfo>* removedItem = events->removeHead();
				delete removedItem;
			}
		}

		auto* onFakeProcessEvent = notificationContext->onFakeProcessEvent;
		if (nullptr != onFakeProcessEvent)
		{
			delete onFakeProcessEvent;
		}
	}

	delete notificationContext;

	LOG_MESSAGE(STRINGIFY(releaseNotificationContext) " ended");

	return true;
}