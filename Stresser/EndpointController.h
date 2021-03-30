#pragma once

#include "AuthorizedHttpRequest.h"
#include "EndpointEntity.h"

#include "../Utils/AutoHandle.h"

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
	* @param serverURL - the URL of the Stresser server.
	* @return Instance of the Agent's Endpoint class.
	*/
	static EndpointController& getInstance(AuthorizedHttpRequest& authorizedHttpRequest);

	/*
		Create new endpoint entity in the server
		@return The new endpoint details.
	*/
	EndpointEntity createEndpoint() const;

	/*
		Gets the endpoint data from the server.
		@param apiKey - Unique key for server request authentication.
		@param endpointId - The ID of the endpoint in the server.
		@return The corresponding endpoint data.
	*/
	EndpointEntity getEndpoint(const std::string& endpointId) const;

private:
	EndpointController(AuthorizedHttpRequest& authorizedHttpRequest);

	AuthorizedHttpRequest& m_authorizedHttpRequest;
	std::wstring m_computerName;
	AutoHandle m_ahKeepAliveThread;
};

