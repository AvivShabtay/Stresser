#include "EndpointController.h"

#include "../Utils/TimeUtils.h"
#include "../Utils/LocalPcUtils.h"
#include "../Utils/StringUtils.h"

#include <atlstr.h>
#include <Windows.h>

EndpointController::EndpointController(AuthorizedHttpRequest& authorizedHttpRequest)
	: m_authorizedHttpRequest(authorizedHttpRequest) {

#ifdef DEBUG
	/*
	* Use unique value each time while in debug mode because the server keeps
	* endpoint registration for a while before removing it and allowing registering again.
	*/
	std::wstring computerName(LocalPcUtils::getLocalComputerName());
	const std::string currentDateTime(TimeUtils::GetCurrentDateTime());

	computerName += std::wstring(CA2W(currentDateTime.c_str()));
	this->m_computerName = computerName;
#else
	this->m_computerName = LocalPcUtils::getLocalComputerName();
#endif // DEBUG
}

EndpointController& EndpointController::getInstance(AuthorizedHttpRequest& authorizedHttpRequest) {
	// Keep static instance of this class in order to prevent
	// new creations of this class:
	static EndpointController g_instance(authorizedHttpRequest);
	return g_instance;
}


EndpointEntity EndpointController::createEndpoint() const
{
	std::wstring wideIp = LocalPcUtils::getLocalComputerIp();
	std::string ip = StringUtils::wstringToString(wideIp);

	EndpointEntity endpointEntity;
	endpointEntity.SetIPAddress(ip);
	endpointEntity.SetHostname(StringUtils::wstringToString(this->m_computerName));

	const Json jsEndpoint = EndpointEntity::ConvertFromEntity(endpointEntity);
	const std::string targetPath("/endpoint");

	const Json responseJson = this->m_authorizedHttpRequest.sendRequest(http::verb::post, targetPath, jsEndpoint);
	if (responseJson.empty())
	{
		throw std::runtime_error("Server return with no data");
	}

	return EndpointEntity::ConvertFromJson(responseJson);
}

EndpointEntity EndpointController::getEndpoint(const std::string& endpointId) const
{
	const std::string targetPath("/endpoint/" + endpointId);

	const Json responseJson = this->m_authorizedHttpRequest.sendRequest(http::verb::get, targetPath, "");
	if (responseJson.empty())
	{
		throw std::runtime_error("Could not get endpoint data from Server");
	}

	return EndpointEntity::ConvertFromJson(responseJson);
}