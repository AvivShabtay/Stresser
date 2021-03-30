#include "PolicyController.h"

#include <sstream>

PolicyController& PolicyController::getInstance(AuthorizedHttpRequest& authorizedHttpRequest)
{
	// Keep static instance of this class in order to prevent
	// new creations of this class:
	static PolicyController g_instance(authorizedHttpRequest);
	return g_instance;
}

PolicyEntity PolicyController::getPolicy(const std::string& policyId) const
{
	std::stringstream targetPath;
	targetPath << "/policy/";
	targetPath << policyId;

	const Json responseJson = this->m_authorizedHttpRequest.sendRequest(http::verb::get, targetPath.str(), "");
	if (responseJson.empty())
	{
		throw std::runtime_error("Server return with no data");
	}

	return PolicyEntity::convertFromJson(responseJson);
}

PolicyController::PolicyController(AuthorizedHttpRequest& authorizedHttpRequest)
	: m_authorizedHttpRequest(authorizedHttpRequest)
{
}
