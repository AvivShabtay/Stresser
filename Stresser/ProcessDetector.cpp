#include "ProcessDetector.h"
#include "../Utils/Win32ErrorCodeException.h"

ProcessDetector::ProcessDetector()
{
	this->detectorDevice.reset(CreateFile(DEVICE_NAME.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
		nullptr, OPEN_EXISTING, 0, nullptr));

	if (INVALID_HANDLE_VALUE == this->detectorDevice.get())
	{
		throw Win32ErrorCodeException("Could not get Stresser engine device");
	}
}

void ProcessDetector::addFakeProcessId(ULONG processId) const
{
	FakeProcessId fakeProcessId{ processId };

	DWORD returnedBytes;
	if (!DeviceIoControl(this->detectorDevice.get(), IOCTL_STRESSER_ENGINE_ADD_FAKE_PID, &fakeProcessId,
		FAKE_PROCESS_ID_SIZE, nullptr, 0, &returnedBytes, nullptr))
	{
		throw Win32ErrorCodeException("Could not commit device control request");
	}
}

void ProcessDetector::removeFakeProcessId(ULONG processId) const
{
	FakeProcessId fakeProcessId{ processId };

	DWORD returnedBytes;
	if (!DeviceIoControl(this->detectorDevice.get(), IOCTL_STRESSER_ENGINE_REMOVE_FAKE_PID, &fakeProcessId,
		FAKE_PROCESS_ID_SIZE, nullptr, 0, &returnedBytes, nullptr))
	{
		throw Win32ErrorCodeException("Could not commit device control request");
	}
}

void ProcessDetector::registerEvent(std::wstring eventName) const
{
	if (MAX_EVENT_NAME_LENGTH < eventName.length())
	{
		throw std::runtime_error("Invalid length of event name");
	}

	if (eventName.empty())
	{
		throw std::runtime_error("Invalid event name");
	}

	RegisterEvent registerEvent{ 0 };

	// Make sure for null-terminator:
	memset(registerEvent.eventName, 0, sizeof(WCHAR) * MAX_EVENT_NAME_LENGTH);

	wcscpy_s(registerEvent.eventName, MAX_EVENT_NAME_LENGTH, eventName.c_str());

	DWORD returnedBytes;
	if (!DeviceIoControl(this->detectorDevice.get(), IOCTL_STRESSER_ENGINE_REGISTER_EVENT, &registerEvent,
		REGISTER_EVENT_SIZE, nullptr, 0, &returnedBytes, nullptr))
	{
		throw Win32ErrorCodeException("Could not commit device control request");
	}
}

EventsResult ProcessDetector::receiveEvents(ULONG numberOfEvents) const
{
	if (numberOfEvents < 1)
	{
		throw std::runtime_error("Invalid number of events to receive");
	}

	auto eventsBuffer = std::make_unique<EventInfo[]>(numberOfEvents);
	if (nullptr == eventsBuffer)
	{
		throw std::runtime_error("Could not allocate memory for events buffer");
	}

	// Zero all the memory:
	memset(eventsBuffer.get(), 0, EVENT_INFO_SIZE * numberOfEvents);

	// Prepare the request:
	EventsHeader eventsHeader
	{
		numberOfEvents,
		eventsBuffer.get()
	};

	DWORD returnedBytes;
	if (!DeviceIoControl(this->detectorDevice.get(), IOCTL_STRESSER_ENGINE_GET_EVENTS, nullptr, 0, &eventsHeader,
		EVENTS_HEADER_SIZE, &returnedBytes, nullptr))
	{
		throw Win32ErrorCodeException("Could not commit device control request");
	}

	// There are no available events:
	if (0 == returnedBytes)
	{
		return EventsResult{ 0, nullptr };
	}

	const int size = returnedBytes / EVENT_INFO_SIZE;

	EventsResult result
	{
		size,
		std::move(eventsBuffer)
	};

	return result;
}

void ProcessDetector::unregisterEvent() const
{
	DWORD returnedBytes;
	if (!DeviceIoControl(this->detectorDevice.get(), IOCTL_STRESSER_ENGINE_UNREGISTER_EVENT, nullptr,
		0, nullptr, 0, &returnedBytes, nullptr))
	{
		throw Win32ErrorCodeException("Could not commit device control request");
	}
}

void ProcessDetector::removeAllFakeProcessIds() const
{
	DWORD returnedBytes;
	if (!DeviceIoControl(this->detectorDevice.get(), IOCTL_STRESSER_ENGINE_REMOVE_ALL_FAKE_PROCESS_IDS, nullptr,
		0, nullptr, 0, &returnedBytes, nullptr))
	{
		throw Win32ErrorCodeException("Could not commit device control request");
	}
}
