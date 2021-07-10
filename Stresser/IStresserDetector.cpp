#include "IStresserDetector.h"
#include "InvalidResponseException.h"

#include "../Utils/DebugPrint.h"

IStresserDetector::IStresserDetector(const EventController& eventController)
	:m_eventController(eventController)
{
}

void IStresserDetector::sendEvent(EventEntity& eventEntity) const
{
	try
	{
		this->m_eventController.sendEvent(eventEntity);
	}
	catch (const InvalidResponseException& exception)
	{
		DEBUG_TRACE(IStresserDetector, "Could not send event to server");
	}
}

