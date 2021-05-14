#pragma once
#include "EtwEventTypes.h"

/*
 * Represent class that have implemented ETW event callback for incoming events.
 */
class IEtwEventHandler
{
public:
	IEtwEventHandler(EtwEventTypes eventType);

	virtual ~IEtwEventHandler() = default;

	virtual void onEventRecord(PEVENT_RECORD record) = 0;

	EtwEventTypes eventType;
};
