#include "IEtwEventHandler.h"

IEtwEventHandler::IEtwEventHandler(EtwEventTypes eventType) :
	m_eventType(eventType)
{
}

EtwEventTypes IEtwEventHandler::getType() const
{
	return m_eventType;
}
