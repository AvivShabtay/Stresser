#include "RuleController.h"

RuleController& RuleController::getInstance(AuthorizedHttpRequest& authorizedHttpRequest)
{
	// Keep static instance of this class in order to prevent
	// new creations of this class:
	static RuleController g_instance(authorizedHttpRequest);
	return g_instance;
}

RuleEntity RuleController::getRule(const std::string& ruleId) const
{
	std::stringstream targetPath;
	targetPath << "/rule/";
	targetPath << ruleId;

	const Json responseJson = this->m_authorizedHttpRequest.sendRequest(http::verb::get, targetPath.str(), "");
	if (responseJson.empty())
	{
		throw std::runtime_error("Server return with no data");
	}

	return RuleEntity::convertFromJson(responseJson);
}

RuleController::RuleController(AuthorizedHttpRequest& authorizedHttpRequest)
	:m_authorizedHttpRequest(authorizedHttpRequest)
{
}
