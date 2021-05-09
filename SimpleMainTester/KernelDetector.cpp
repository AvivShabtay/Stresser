#include "KernelDetector.h"

#include "../Utils/Win32ErrorCodeException.h"

#include "../StresserEngine/StresserEngineCommon.h"

KernelDetector::KernelDetector()
{
	this->stresserDevice.reset(CreateFile(DEVICE_NAME.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
		nullptr, OPEN_EXISTING, 0, nullptr));

	if (INVALID_HANDLE_VALUE == this->stresserDevice.get())
	{
		throw Win32ErrorCodeException("Could not get Stresser engine device");
	}
}

void KernelDetector::addFakeProcessId(ULONG processId) const
{
	FakeProcessId fakeProcessId{ processId };

	DWORD returnedBytes;
	if (!DeviceIoControl(this->stresserDevice.get(), IOCTL_STRESSER_ENGINE_ADD_FAKE_PID, &fakeProcessId,
		FAKE_PROCESS_ID_SIZE, nullptr, 0, &returnedBytes, nullptr))
	{
		throw Win32ErrorCodeException("Could not commit device control request");
	}
}

void KernelDetector::removeFakeProcessId(ULONG processId) const
{
	FakeProcessId fakeProcessId{ processId };

	DWORD returnedBytes;
	if (!DeviceIoControl(this->stresserDevice.get(), IOCTL_STRESSER_ENGINE_REMOVE_FAKE_PID, &fakeProcessId,
		FAKE_PROCESS_ID_SIZE, nullptr, 0, &returnedBytes, nullptr))
	{
		throw Win32ErrorCodeException("Could not commit device control request");
	}
}

/*
void KernelDetector::registerEvent(HANDLE eventObject) const
{
	RegisterEvent registerEvent{ eventObject };

	DWORD returnedBytes;
	if (!DeviceIoControl(this->stresserDevice.get(), IOCTL_STRESSER_ENGINE_REMOVE_FAKE_PID, &registerEvent,
		REGISTER_EVENT_SIZE, nullptr, 0, &returnedBytes, nullptr))
	{
		throw Win32ErrorCodeException("Could not commit device control request");
	}
}
*/