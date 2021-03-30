#pragma once
#include "RuleEntity.h"
#include "AuthorizedHttpRequest.h"

#include <iostream>

class RuleController
{
public:
	// Disable: cloneable, assignable, movable:
	RuleController(RuleController&) = delete;
	void operator=(const RuleController&) = delete;
	RuleController(RuleController&&) = delete;
	void operator=(const RuleController&&) = delete;

	virtual ~RuleController() = default;

	/*
	* Return the singleton instance of this class.
	* @param authorizedHttpRequest - HTTP request provider.
	* @return Global instance of this class.
	*/
	static RuleController& getInstance(AuthorizedHttpRequest& authorizedHttpRequest);

	/*
		Return the rule data from the server by it's rule ID.
		@param ruleID - The associated ID of the rule.
		@return The corresponding rule data.
	*/
	RuleEntity getRule(const std::string& ruleId) const;

private:
	explicit RuleController(AuthorizedHttpRequest& authorizedHttpRequest);

	AuthorizedHttpRequest& m_authorizedHttpRequest;
};

