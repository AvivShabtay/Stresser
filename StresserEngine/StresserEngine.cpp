#include "StresserEngine.h"
#include "DriverConfiguration.h"
#include "StresserEngineCommon.h"
#include "HelperMacros.h"
#include "IrpUtils.h"
#include "ObjectNotification.h"
#include "Vector.h"
#include "Memory.h"
#include "ProcessUtils.h"

PKEVENT g_notificationEvent = nullptr;
PVOID g_registrationHandle = nullptr;
Vector<FakeProcessId*, DRIVER_TAG>* g_fakeProcessIds = nullptr;

/*
 * Driver entry point.
 * Responsible for register dispatch function, register detection callbacks, etc.
 */
extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	KdPrint((DRIVER_PREFIX "DriverEntry started\n"));

	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT DeviceObject = nullptr;
	UNICODE_STRING symLink = RTL_CONSTANT_STRING(SYMBOLIC_LINK_PATH);
	bool symbolicLinkCreated = false;

	do
	{
		// Create device object for user-mode communication:
		UNICODE_STRING devName = RTL_CONSTANT_STRING(DEVICE_NAME);
		status = IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, 0, TRUE, &DeviceObject);
		PRINT_STATUS_SUCCESS_FAILURE(status, "Create device object successfully", "Failed to create device");
		BREAK_ON_FAILURE(status);

		// Set the IO communication method:
		DeviceObject->Flags |= DO_DIRECT_IO;

		// Create symbolic link to the device object:
		status = IoCreateSymbolicLink(&symLink, &devName);
		PRINT_STATUS_SUCCESS_FAILURE(status, "Create symbolic link successfully", "Failed to create symbolic link");
		BREAK_ON_FAILURE(status);

		symbolicLinkCreated = true;

		// Initialize data structures:
		g_fakeProcessIds = new (NonPagedPool, DRIVER_TAG) Vector<FakeProcessId*, DRIVER_TAG>();
		if (nullptr == g_fakeProcessIds)
		{
			status = STATUS_INSUFFICIENT_RESOURCES;
			BREAK_ON_FAILURE(status);
		}

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

		// Set object notification registration:
		OB_CALLBACK_REGISTRATION objectCallbackRegistration =
		{
			OB_FLT_REGISTRATION_VERSION,	// Version
			STRESSER_OBJECT_CALLBACK_COUNT,	// OperationRegistrationCount
			altitude,						// Altitude
			nullptr,			// RegistrationContext
			objectOperationsRegistration	// OperationRegistration
		};

		status = ObRegisterCallbacks(&objectCallbackRegistration, &g_registrationHandle);
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
		if (nullptr != DeviceObject)
		{
			IoDeleteDevice(DeviceObject);
		}
		if (nullptr != g_registrationHandle)
		{
			ObUnRegisterCallbacks(g_registrationHandle);
		}
		if (nullptr != g_fakeProcessIds)
		{
			delete g_fakeProcessIds;
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

	KdPrint((DRIVER_PREFIX "DriverEntry completed successfully\n"));

	return status;
}

void StresserEngineUnload(PDRIVER_OBJECT DriverObject)
{

	KdPrint((DRIVER_PREFIX "StresserEngineUnloaded started\n"));

	// Remove registered callbacks
	if (nullptr != g_registrationHandle)
	{
		ObUnRegisterCallbacks(g_registrationHandle);
		KdPrint((DRIVER_PREFIX "Remove object notification callback successfully\n"));
	}

	// Free allocated memory for fake process IDs:
	for (auto& item : *g_fakeProcessIds)
	{
		delete item;
	}

	delete g_fakeProcessIds;
	KdPrint((DRIVER_PREFIX "Release memory for holding fake process IDs\n"));

	// Free notification event reference:
	if (nullptr != g_notificationEvent)
	{
		ObDereferenceObject(g_notificationEvent);
		//ExFreePoolWithTag(g_notificationEvent, DRIVER_TAG);
		g_notificationEvent = nullptr;
		KdPrint((DRIVER_PREFIX "Release reference to notification even successfully\n"));
	}

	// Remove symbolic link
	UNICODE_STRING symLink = RTL_CONSTANT_STRING(SYMBOLIC_LINK_PATH);
	const NTSTATUS status = IoDeleteSymbolicLink(&symLink);
	PRINT_STATUS_SUCCESS_FAILURE(status, "Remove symbolic link successfully", "Failed to delete symbolic link");

	// Remove device object
	IoDeleteDevice(DriverObject->DeviceObject);
	KdPrint((DRIVER_PREFIX "Delete device object\n"));

	KdPrint((DRIVER_PREFIX "StresserEngineUnloaded completed successfully\n"));
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

NTSTATUS CompleteIrp(PIRP Irp, NTSTATUS status, ULONG_PTR info)
{
	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = info;
	IoCompleteRequest(Irp, 0);
	return status;
}

NTSTATUS StresserEngineDeviceControl(PDEVICE_OBJECT, PIRP Irp)
{
	KdPrint((DRIVER_PREFIX "DeviceControl started\n"));

	const PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	const ULONG controlCode = stack->Parameters.DeviceIoControl.IoControlCode;
	NTSTATUS status = STATUS_NOT_SUPPORTED;

	if (nullptr == Irp)
	{
		return CompleteIrp(Irp, status);
	}

	// Run the corresponding handler for the request:
	switch (controlCode) {
	case IOCTL_STRESSER_ENGINE_REGISTER_EVENT:
	{
		status = RegisterEventHandler(Irp, stack);
		break;
	}
	case IOCTL_STRESSER_ENGINE_ADD_FAKE_PID:
	{
		status = AddFakeProcessIdHandler(Irp, stack);
		break;
	}

	default:
	{
		status = STATUS_INVALID_DEVICE_REQUEST;
		break;
	}
	}

	KdPrint((DRIVER_PREFIX "DeviceControl completed successfully\n"));

	return CompleteIrp(Irp, status);
}

NTSTATUS RegisterEventHandler(PIRP Irp, PIO_STACK_LOCATION StackLocation)
{
	KdPrint((DRIVER_PREFIX STRINGIFY(RegisterEventHandler) " started\n"));

	if (!IrpUtils::isValidInputBuffer(StackLocation, REGISTER_EVENT_SIZE))
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(RegisterEventHandler) " user buffer is invalid\n"));

		return STATUS_BUFFER_TOO_SMALL;
	}

	auto* const registerEvent = static_cast<RegisterEvent*>(Irp->AssociatedIrp.SystemBuffer);
	if (nullptr == registerEvent)
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(RegisterEventHandler) " Invalid memory\n"));
		STATUS_INVALID_PARAMETER;
	}

	if (nullptr == registerEvent->eventHandle)
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(RegisterEventHandler) " Invalid type for request\n"));
		return STATUS_INVALID_PARAMETER;
	}

	// Get the object pointer from the handle.
	// Note we must be in the context of the process that created the handle
	const NTSTATUS status = ObReferenceObjectByHandle(
		registerEvent->eventHandle,
		SYNCHRONIZE | EVENT_MODIFY_STATE,
		*ExEventObjectType,
		Irp->RequestorMode,
		reinterpret_cast<PVOID*>(g_notificationEvent),
		nullptr
	);

	if (!NT_SUCCESS(status))
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(RegisterEventHandler) " could not get handle for event object status: (0x%08X)\n", status));
		return status;
	}

	KdPrint((DRIVER_PREFIX STRINGIFY(RegisterEventHandler) " completed successfully\n"));

	return STATUS_SUCCESS;
}

NTSTATUS AddFakeProcessIdHandler(_In_ PIRP Irp, _In_ PIO_STACK_LOCATION StackLocation)
{
	KdPrint((DRIVER_PREFIX STRINGIFY(AddFakeProcessIdHandler) " started\n"));

	if (!IrpUtils::isValidInputBuffer(StackLocation, FAKE_PROCESS_ID_SIZE))
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(AddFakeProcessIdHandler) " user buffer is invalid\n"));

		return STATUS_BUFFER_TOO_SMALL;
	}

	auto* const fakeProcessId = static_cast<FakeProcessId*>(Irp->AssociatedIrp.SystemBuffer);
	if (nullptr == fakeProcessId)
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(AddFakeProcessIdHandler) " Invalid type for request\n"));
		return STATUS_INVALID_PARAMETER;
	}

	if (!ProcessUtils::doesValidProcessId(fakeProcessId->processId))
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(RegisterEventHandler) " Invalid process ID\n"));
		return STATUS_INVALID_PARAMETER;
	}

	auto* processId = new (NonPagedPool, DRIVER_TAG) FakeProcessId;

	processId->processId = fakeProcessId->processId;

	g_fakeProcessIds->add(processId);
	KdPrint((DRIVER_PREFIX STRINGIFY(AddFakeProcessIdHandler)" Add fake process ID=%d\n", processId->processId));

	KdPrint((DRIVER_PREFIX STRINGIFY(AddFakeProcessIdHandler) " completed successfully\n"));

	return STATUS_SUCCESS;
}

NTSTATUS RemoveFakeProcessIdHandler(PIRP Irp, PIO_STACK_LOCATION StackLocation)
{
	KdPrint((DRIVER_PREFIX STRINGIFY(RemoveFakeProcessIdHandler) " started\n"));

	if (!IrpUtils::isValidInputBuffer(StackLocation, FAKE_PROCESS_ID_SIZE))
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(RemoveFakeProcessIdHandler) " user buffer is invalid\n"));

		return STATUS_BUFFER_TOO_SMALL;
	}

	auto* const fakeProcessId = static_cast<FakeProcessId*>(Irp->AssociatedIrp.SystemBuffer);
	if (nullptr == fakeProcessId)
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(RemoveFakeProcessIdHandler) " invalid type for request\n"));
		return STATUS_INVALID_PARAMETER;
	}

	if (!ProcessUtils::doesValidProcessId(fakeProcessId->processId))
	{
		KdPrint((DRIVER_PREFIX STRINGIFY(RemoveFakeProcessIdHandler) " invalid process ID\n"));
		return STATUS_INVALID_PARAMETER;
	}

	for (ULONG i = 0; i < g_fakeProcessIds->size(); ++i)
	{
		const FakeProcessId* current = g_fakeProcessIds->getAt(i);
		if (fakeProcessId->processId == current->processId)
		{
			KdPrint((DRIVER_PREFIX STRINGIFY(RemoveFakeProcessIdHandler)" remove fake process ID=%d\n", current->processId));

			g_fakeProcessIds->removeAt(i);
			delete current;

			KdPrint((DRIVER_PREFIX STRINGIFY(RemoveFakeProcessIdHandler) " completed successfully\n"));
			return STATUS_SUCCESS;
		}
	}

	KdPrint((DRIVER_PREFIX STRINGIFY(RemoveFakeProcessIdHandler) " could not remove process ID=%d\n", fakeProcessId->processId));
	return STATUS_INVALID_PARAMETER;
}
