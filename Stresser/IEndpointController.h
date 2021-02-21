#pragma once

#include "EndpointEntity.h"
#include "HttpConnectionWithTokens.h"
#include "ITokenListener.h"

#include <iostream>

class IEndpointController : public ITokenListener
{
public:
	IEndpointController(HttpConnectionWithTokens* httpConnection)
		: m_httpConnection(httpConnection) { }

	/*
		Create new endpoint entity in the server
		@return The new endpoint details.
	*/
	virtual EndpointEntity CreateEndpoint() = 0;

	/*
		Updates endpoint session status by sending  "Hello" request.
		@param If operation success.
	*/
	//virtual bool StartAPIKeyRefresher(std::string endpointID) = 0;

	/*
		Gets the endpoint data from the server.
		@param endpointId - The ID of the endpoint in the server.
		@return The corresponding endpoint data.
	*/
	virtual EndpointEntity GetEndpoint(std::string endpointId) = 0;

	void updateToken(const std::string& newToken) = 0;

protected:
	std::unique_ptr<HttpConnectionWithTokens> m_httpConnection;
};
