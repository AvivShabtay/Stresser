#pragma once

#include <ntifs.h>

class IrpUtils final
{
public:
	/* Check if the input buffer size is not smaller than required. */
	static bool isValidInputBuffer(_In_ PIO_STACK_LOCATION StackLocation, _In_ ULONGLONG validSize);

	/* Check if the output buffer size is not smaller than required. */
	static bool isValidOutputBuffer(_In_ PIO_STACK_LOCATION StackLocation, _In_ ULONGLONG validSize);
};

