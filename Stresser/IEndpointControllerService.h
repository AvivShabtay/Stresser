#pragma once
#include <iostream>

#include "EndpointEntity.h"

class IEndpointControllerService
{
public:
	/*
		Create new endpoint entity in the server
		@return The new endpoint details.
	*/
	virtual EndpointEntity CreateEndpoint() = 0;

	/*
		Updates endpoint session status by sending  "Hello" request.
		@param apiKey - Unique key for server request authentication.
		@param If operation success.
	*/
	virtual bool KeepAlive(std::string apiKey, EndpointEntity& endpoint) = 0;

	/*
		Gets the endpoint data from the server.
		@param apiKey - Unique key for server request authentication.
		@param endpointId - The ID of the endpoint in the server.
		@return The corresponding endpoint data.
	*/
	virtual EndpointEntity GetEndpoint(std::string apiKey, std::string endpointId) = 0;

private:
	EndpointEntity endpointEntity;
};
