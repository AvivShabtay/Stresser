#pragma once
#include "EventProvidersGuid.h"
#include <map>

#include <Windows.h>

// must be after Windows.h:
#include <evntrace.h>

enum class EtwEventTypes : ULONG
{
	DiskIo = EVENT_TRACE_FLAG_DISK_IO,
	DiskFileIo = EVENT_TRACE_FLAG_DISK_FILE_IO,
	FileIo = EVENT_TRACE_FLAG_FILE_IO,
	FileIoInit = EVENT_TRACE_FLAG_FILE_IO_INIT,
	Registry = EVENT_TRACE_FLAG_REGISTRY,
	Process = EVENT_TRACE_FLAG_PROCESS,
	ProcessCounters = EVENT_TRACE_FLAG_PROCESS_COUNTERS
};

const std::map<EtwEventTypes, GUID> EtwEventsGuid =
{
	{EtwEventTypes::DiskIo, DiskIoGuid},
	{EtwEventTypes::DiskFileIo, FileIoGuid},
	{EtwEventTypes::FileIo, FileIoGuid},
	{EtwEventTypes::FileIoInit, FileIoGuid},
	{EtwEventTypes::Registry, RegistryGuid},
	{EtwEventTypes::Process, ProcessGuid},
	{EtwEventTypes::ProcessCounters, ProcessGuid}
};
