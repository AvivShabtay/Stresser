#pragma once
#include <iostream>

#include "RuleEntity.h"

class IRuleControllerService
{
public:
	/*
		Gets the rule data from the server using the endpoint API key by the
		corresponding rule ID.
		@param apiKey - Unique key for server request authentication.
		@param ruleID - The ID of the rule associated with this endpoint in the server.
		@return The corresponding rule data.
	*/
	virtual RuleEntity GetRule(std::string apiKey, std::string policyId) = 0;

public:
	RuleEntity ruleEntity;
};
