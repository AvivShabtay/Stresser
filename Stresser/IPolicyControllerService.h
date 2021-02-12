#pragma once
#include <iostream>

#include "PolicyEntity.h"

class IPolicyControllerService
{
public:
	/*
		Gets the policy data from the server using the endpoint API key by the
		corresponding policy ID.
		@param apiKey - Unique key for server request authentication.
		@param policyID - The ID of the policy associated with this endpoint in the server.
		@return The corresponding policy data.
	*/
	virtual PolicyEntity GetPolicy(std::string apiKey, std::string policyId) = 0;

public:
	PolicyEntity policyEntity;
};
