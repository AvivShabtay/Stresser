#include "EventController.h"
#include "../Utils/StringUtils.h"
#include "../Utils/LocalPcUtils.h"

EventController& EventController::getInstance(AuthorizedHttpRequest& authorizedHttpRequest)
{
	// Keep static instance of this class in order to prevent
	// new creations of this class:
	static EventController g_instance(authorizedHttpRequest);
	return g_instance;
}

void EventController::sendEvent(EventEntity& eventEntity) const
{
	const std::string targetPath("/event");

	eventEntity.setIpAddress(this->m_ipAddress);
	eventEntity.setHostname(this->m_hostname);

	const Json jsEvent = EventEntity::ConvertFromEntity(eventEntity);

	const Json responseJson = this->m_authorizedHttpRequest.sendRequest(http::verb::post, targetPath, jsEvent);
	if (responseJson.empty())
	{
		throw std::runtime_error("Server return with no data");
	}
}

EventController::EventController(AuthorizedHttpRequest& authorizedHttpRequest)
	: m_authorizedHttpRequest(authorizedHttpRequest)
{
	const std::wstring wideHostname = LocalPcUtils::getLocalComputerName();
	const std::wstring wideIp = LocalPcUtils::getLocalComputerIp();

	this->m_hostname = StringUtils::wstringToString(wideHostname);
	this->m_ipAddress = StringUtils::wstringToString(wideIp);
}
