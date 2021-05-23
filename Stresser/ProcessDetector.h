#pragma once

#include "../Utils/AutoHandle.h"
#include "../StresserEngine/StresserEngineCommon.h"

#include <string>

const std::wstring DEVICE_NAME = L"\\\\.\\StresserEngine";

/*
	Result of calling for getting events from Driver.
 */
struct EventsResult
{
	int size;								// Number of received events
	std::unique_ptr < EventInfo[]> events;	// Buffer with the received events
};

class ProcessDetector
{
public:
	ProcessDetector();

	// Disable copyable, assignable, movable:
	ProcessDetector(ProcessDetector&) = delete;
	ProcessDetector& operator=(ProcessDetector&) = delete;
	ProcessDetector(ProcessDetector&&) = delete;
	ProcessDetector& operator=(ProcessDetector&&) = delete;

	virtual ~ProcessDetector() = default;

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
	AutoHandle detectorDevice;
};

