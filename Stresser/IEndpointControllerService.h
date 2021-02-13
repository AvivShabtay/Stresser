#pragma once
#include <iostream>

#include "EndpointEntity.h"
#include "../CommunicationManager/IConnection.h"

class IEndpointControllerService
{
public:
	IEndpointControllerService(IConnection& connection) : m_connection(connection) { }

	/*
		Create new endpoint entity in the server
		@return The new endpoint details.
	*/
	virtual EndpointEntity CreateEndpoint() = 0;

	/*
		Updates endpoint session status by sending  "Hello" request.
		@param If operation success.
	*/
	virtual bool StartAPIKeyRefresher(std::string endpointID) = 0;

	/*
		Gets the endpoint data from the server.
		@param endpointId - The ID of the endpoint in the server.
		@return The corresponding endpoint data.
	*/
	virtual EndpointEntity GetEndpoint(std::string endpointId) = 0;

protected:
	IConnection& m_connection;
};
