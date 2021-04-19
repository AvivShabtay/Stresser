#include <Windows.h>

// must be after Windows.h:
#include <evntrace.h>

enum EtwEventTypes : ULONG
{
	DiskIo = EVENT_TRACE_FLAG_DISK_IO,
	DiskFileIo = EVENT_TRACE_FLAG_DISK_FILE_IO,
	FileIo = EVENT_TRACE_FLAG_FILE_IO,
	FileIoInit = EVENT_TRACE_FLAG_FILE_IO_INIT,
	Registry = EVENT_TRACE_FLAG_REGISTRY,
	Process = EVENT_TRACE_FLAG_PROCESS,
	ProcessCounters = EVENT_TRACE_FLAG_PROCESS_COUNTERS
};