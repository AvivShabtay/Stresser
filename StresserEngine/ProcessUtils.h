#pragma once

#include "DriverConfiguration.h"
#include "Value.h"

#include <ntifs.h>

extern "C" NTSTATUS ZwQueryInformationProcess(
	_In_ HANDLE ProcessHandle,
	_In_ PROCESSINFOCLASS ProcessInformationClass,
	_Out_ PVOID ProcessInformation,
	_In_ ULONG ProcessInformationLength,
	_Out_opt_ PULONG ReturnLength);

class ProcessUtils
{
public:
	enum ErrorCodes
	{
		INVALID_PID_VALUE = 0,
		INVALID_PEPROCESS,
		COULD_NOT_FIND_EPROCESS,
		COULD_NOT_CREATE_OBJECT_REFERENCE,
		COULD_NOT_QUERY_PROCESS_INFORMATION
	};

	static constexpr ULONG SYSTEM_PID_VALUE = 4;
	static constexpr SIZE_T FILE_NAME_SIZE = 300;

	static Value<const ULONG, ErrorCodes> getPidFromProcess(const PEPROCESS& process);

	static Value<StresserString, NTSTATUS> getProcessFileNameByPid(const ULONG processId);
};

