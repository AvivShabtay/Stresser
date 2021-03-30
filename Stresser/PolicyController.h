#pragma once

#include "AuthorizedHttpRequest.h"
#include "PolicyEntity.h"

#include <iostream>

class PolicyController
{
public:
	// Disable: cloneable, assignable, movable:
	PolicyController(PolicyController&) = delete;
	void operator=(const PolicyController&) = delete;
	PolicyController(PolicyController&&) = delete;
	void operator=(const PolicyController&&) = delete;

	virtual ~PolicyController() = default;

	/*
	* Return the singleton instance of this class.
	* @param authorizedHttpRequest - HTTP request provider.
	* @return Global instance of this class.
	*/
	static PolicyController& getInstance(AuthorizedHttpRequest& authorizedHttpRequest);

	/*
		Return the policy data from the server by it's policy ID.
		@param policyID - The associated ID of the policy.
		@return The corresponding policy data.
	*/
	PolicyEntity getPolicy(const std::string& policyId) const;

public:
	explicit PolicyController(AuthorizedHttpRequest& authorizedHttpRequest);

	AuthorizedHttpRequest& m_authorizedHttpRequest;
};
