#include <ntifs.h>
#include <ntddk.h>

// Prototypes:
void StresserEngineUnload(_In_ PDRIVER_OBJECT DriverObject);
NTSTATUS StresserEngineCreateClose(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);
//NTSTATUS StresserEngineDeviceControl(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);

// DriverEntry
extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);

	KdPrint(("[StresserEngine] DriverEntry started\n"));

	DriverObject->DriverUnload = StresserEngineUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = StresserEngineCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = StresserEngineCreateClose;
	//DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = StresserEngineDeviceControl;

	// Creating device object for the engine:
	UNICODE_STRING devName = RTL_CONSTANT_STRING(L"\\Device\\StresserEngine");
	PDEVICE_OBJECT DeviceObject;
	NTSTATUS status = IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
	if (!NT_SUCCESS(status)) {
		KdPrint(("[StresserEngine] Failed to create device (0x%08X)\n", status));
		return status;
	}

	UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\StresserEngine");
	status = IoCreateSymbolicLink(&symLink, &devName);
	if (!NT_SUCCESS(status)) {
		KdPrint(("[StresserEngine] Failed to create symbolic link (0x%08X)\n", status));
		IoDeleteDevice(DeviceObject);
		return status;
	}

	KdPrint(("[StresserEngine] StresserEngine DriverEntry completed successfully\n"));

	return STATUS_SUCCESS;
}

void StresserEngineUnload(PDRIVER_OBJECT DriverObject) {
	UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\StresserEngine");

	// Remove symbolic link
	IoDeleteSymbolicLink(&symLink);

	// Remove device object
	IoDeleteDevice(DriverObject->DeviceObject);

	KdPrint(("[StresserEngine] StresserEngine unloaded\n"));
}

NTSTATUS StresserEngineCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}