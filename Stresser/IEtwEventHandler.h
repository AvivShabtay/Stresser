#pragma once

#include <Windows.h>

// must be after Windows.h:
#include <evntrace.h>

/*
 * Represent class that have implemented ETW event callback for incoming events.
 */
class IEtwEventHandler
{
public:
	virtual ~IEtwEventHandler() = default;

	virtual void onEventRecord(PEVENT_RECORD record) = 0;
};