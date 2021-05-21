#include "EventController.h"

EventController& EventController::getInstance(AuthorizedHttpRequest& authorizedHttpRequest)
{
	// Keep static instance of this class in order to prevent
	// new creations of this class:
	static EventController g_instance(authorizedHttpRequest);
	return g_instance;
}

void EventController::sendEvent(const EventEntity& eventEntity) const
{
	// TODO
	throw std::runtime_error("Not implemented method, TODO !");
}

EventController::EventController(AuthorizedHttpRequest& authorizedHttpRequest)
	: m_authorizedHttpRequest(authorizedHttpRequest)
{
}
