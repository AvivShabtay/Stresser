#include "IStresserDetector.h"

IStresserDetector::IStresserDetector(const EventController& eventController)
	:m_eventController(eventController)
{
}

void IStresserDetector::sendEvent(const EventEntity& eventEntity) const
{
	this->m_eventController.sendEvent(eventEntity);
}

