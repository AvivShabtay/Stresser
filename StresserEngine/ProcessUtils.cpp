#include "ProcessUtils.h"
#include "HelperMacros.h"
#include "GenericHandle.h"
#include "AutoObjectDereference.h"
#include "AutoBuffer.h"

Value<const ULONG, ProcessUtils::ErrorCodes> ProcessUtils::getPidFromProcess(const PEPROCESS& process)
{
	if (nullptr == process)
	{
		return INVALID_PID_VALUE;
	}

	const HANDLE processHandle = PsGetProcessId(process);
	if (nullptr >= processHandle)
	{
		return INVALID_PID_VALUE;
	}

	const ULONG pid = HandleToULong(processHandle);

	/*
	 * TODO: Verify PID value:
	 * Possible solution: iterates over all the running processes and verify
	 * if the PID exists in the system.
	 */

	return pid;
}

bool ProcessUtils::doesValidProcessId(ULONG processId)
{
	if (ProcessUtils::INVALID_PID_VALUE == processId)
	{
		return false;
	}

	if (ProcessUtils::SYSTEM_PID_VALUE == processId)
	{
		return false;
	}

	/*
	 * TODO: Verify PID value:
	 * Possible solution: iterates over all the running processes and verify
	 * if the PID exists in the system.
	 */

	return true;
}

Value<StresserString, NTSTATUS> ProcessUtils::getProcessFileNameByPid(const ULONG processId)
{
	if (0 == processId)
	{
		return STATUS_INVALID_PARAMETER;
	}

	if (ProcessUtils::SYSTEM_PID_VALUE == processId)
	{
		return StresserString(L"System");
	}

	AutoObjectDereference<PEPROCESS> process;
	const HANDLE pseudoProcessHandle = ULongToHandle(processId);

	NTSTATUS status = PsLookupProcessByProcessId(pseudoProcessHandle, &process.get());
	RETURN_ON_STATUS_FAILURE(status, "PsLookupProcessByProcessId failed");

	HANDLE handle;
	status = ObOpenObjectByPointer(process.get(), OBJ_KERNEL_HANDLE, nullptr, 0, nullptr, KernelMode, &handle);
	RETURN_ON_STATUS_FAILURE(status, "ObOpenObjectByPointer failed");

	// RAII safe handle
	const AutoKernelHandle processHandle(handle);

	const StresserBuffer imageFileNameBuffer(ProcessUtils::FILE_NAME_SIZE);
	if (nullptr == imageFileNameBuffer.get())
	{
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	ULONG returnedSize = 0;

	status = ZwQueryInformationProcess(processHandle, ProcessImageFileName, imageFileNameBuffer.get(),
		FILE_NAME_SIZE - sizeof(WCHAR), &returnedSize);
	RETURN_ON_STATUS_FAILURE(status, "ZwQueryInformationProcess failed");

	return StresserString(static_cast<PUNICODE_STRING>(imageFileNameBuffer.get()));
}
