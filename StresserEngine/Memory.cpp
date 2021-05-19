#include "Memory.h"
#include "DriverConfiguration.h"

void* __cdecl operator new(size_t size, POOL_TYPE type, ULONG tag)
{
	PVOID buffer = nullptr;

	if (0 == tag)
	{
		// Kernel allocating default tag:
		buffer = ExAllocatePool(type, size);
	}
	else
	{
		buffer = ExAllocatePoolWithTag(type, size, tag);
	}

	if (nullptr == buffer)
	{
		KdPrint((DRIVER_PREFIX "Failed to allocate %u bytes\n", size));
	}

	return buffer;

	// Calls the object constructor automatically
}

void* __cdecl operator new(size_t size, PVOID pointer)
{
	UNREFERENCED_PARAMETER(size);

	return pointer;

	// Calls the object constructor automatically
}

void operator delete(PVOID pointer, size_t size)
{
	UNREFERENCED_PARAMETER(size);
	ExFreePool(pointer);
}
