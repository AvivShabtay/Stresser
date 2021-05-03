#pragma once

#include "AuthorizedHttpRequest.h"
#include "EndpointEntity.h"

#include "../Utils/AutoHandle.h"
#include "../Utils/StringUtils.h"

#include "nlohmann/json.hpp"

#include <iostream>

using Json = nlohmann::json;

/*
* Represent the local computer as Endpoint in the Stresser security solution.
* Responsible for updating the Stresser server of endpoint status
* and manage the Agent API key for server-client operations.
*/
class EndpointController
{
public:
	// Disable: cloneable, assignable, movable:
	EndpointController(EndpointController&) = delete;
	void operator=(const EndpointController&) = delete;
	EndpointController(EndpointController&&) = delete;
	void operator=(const EndpointController&&) = delete;

	virtual ~EndpointController() = default;

	/*
	* Return the singleton instance of this class.
	* @param authorizedHttpRequest - HTTP request provider.
	* @return Global instance of this class.
	*/
	static EndpointController& getInstance(AuthorizedHttpRequest& authorizedHttpRequest);

	/*
		Create new endpoint entity in the server
		@return The new endpoint details: ID and token.
	*/
	EndpointEntity createEndpoint() const;

	/*
		Return the endpoint data from the server by it's endpoint ID.
		@param endpointId - The associated ID of the endpoint.
		@return The corresponding endpoint data.
	*/
	EndpointEntity getEndpoint(const std::string& endpointId) const;

private:
	EndpointController(AuthorizedHttpRequest& authorizedHttpRequest);

	AuthorizedHttpRequest& m_authorizedHttpRequest;
	std::wstring m_computerName;
};

