#pragma once
#include "EtwEventTypes.h"
#include "EventEntity.h"

#include <optional>

/*
 * Represent class that have implemented ETW event callback for incoming events.
 */
class IEtwEventHandler
{
public:
	IEtwEventHandler(EtwEventTypes eventType);

	EtwEventTypes getType() const;

	virtual ~IEtwEventHandler() = default;

	virtual std::optional<EventEntity> onEventRecord(PEVENT_RECORD record) = 0;

private:
	EtwEventTypes m_eventType;
};
