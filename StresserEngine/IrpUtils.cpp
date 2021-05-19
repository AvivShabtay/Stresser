#include "IrpUtils.h"

bool IrpUtils::isValidInputBuffer(PIO_STACK_LOCATION StackLocation, ULONGLONG validSize)
{
	auto& deviceIoControl = StackLocation->Parameters.DeviceIoControl;

	if (validSize > deviceIoControl.InputBufferLength)
	{
		return false;
	}

	return true;
}

bool IrpUtils::isValidOutputBuffer(PIO_STACK_LOCATION StackLocation, ULONGLONG validSize)
{
	auto& deviceIoControl = StackLocation->Parameters.DeviceIoControl;

	if (validSize > deviceIoControl.OutputBufferLength)
	{
		return false;
	}

	return true;
}
