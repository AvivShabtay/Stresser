#pragma once

#include "../Utils/AutoHandle.h"
#include "../StresserEngine/StresserEngineCommon.h"

#include <Windows.h>

const std::wstring DEVICE_NAME = L"\\\\.\\StresserEngine";

/*
	Result of calling for getting events from Driver.
 */
struct EventsResult
{
	int size;								// Number of received events
	std::unique_ptr < EventInfo[]> events;	// Buffer with the received events
};

/*
	Interface for communication with Stresser Driver through I\O calls.
 */
class StresserEngine
{
public:
	StresserEngine();

	virtual ~StresserEngine() = default;

	// Delete copyable, movable:
	StresserEngine(const StresserEngine&) = delete;
	StresserEngine& operator=(const StresserEngine&) = delete;
	StresserEngine(StresserEngine&&) = delete;
	StresserEngine& operator=(StresserEngine&&) = delete;

	/* Add process ID to be monitored. */
	void addFakeProcessId(ULONG processId) const;

	/* Remove process ID from being monitored. */
	void removeFakeProcessId(ULONG processId) const;

	/*
		Register shared event with the Driver for synchronization when
		there are available events to consume.
	*/
	void registerEvent(std::wstring eventName) const;

	/* Fetch the events from the Driver. */
	EventsResult receiveEvents(ULONG numberOfEvents = 10) const;

	void unregisterEvent() const;

private:
	AutoHandle stresserDevice;
};

