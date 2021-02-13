#pragma once

#include <iostream>
#include <vector>

#include "IEndpointControllerService.h"
#include "../CommunicationManager/Connection.h"

#include "../Utils/TimeUtils.h"
#include "../Utils/StringUtils.h"
#include "../Utils/AutoHandle.h"
#include "../Utils/ShutdownSignal.h"

#include "nlohmann/json.hpp"

using Json = nlohmann::json;

/*
* Represent the local computer as Endpoint in the Stresser security solution.
* Responsible for updating the Stresser server of endpoint status
* and manage the Agent API key for server-client operations.
*/
class EndpointControllerService : public IEndpointControllerService
{
public:
	// Disable cloneable, disable assignable:
	EndpointControllerService(EndpointControllerService& other) = delete;
	void operator=(const EndpointControllerService&) = delete;

	/*
	* Return the singleton instance of this class.
	* @param serverURL - the URL of the Stresser server.
	* @return Instance of the Agent's Endpoint class.
	*/
	static EndpointControllerService& GetInstance(IConnection& connection);

	/*
		Create new endpoint entity in the server
		@return The new endpoint details.
	*/
	EndpointEntity CreateEndpoint();

	/*
		Updates endpoint session status by sending  "Hello" request.
		@param apiKey - Unique key for server request authentication.
		@param If operation success.
	* Represent the thread functions.
	* Gets reference to this class ans start sending keep alive messages
	* to the server while the application is running.
	*/
	bool StartAPIKeyRefresher(std::string endpointID);

	/*
		Gets the endpoint data from the server.
		@param apiKey - Unique key for server request authentication.
		@param endpointId - The ID of the endpoint in the server.
		@return The corresponding endpoint data.
	*/
	EndpointEntity GetEndpoint(std::string endpointId);

private:
	EndpointControllerService(IConnection& connection);

	/*
	*
	*/
	void RefreshToken(std::string endpointID);

	/*
	* Return the local computer name.
	* @return The local computer name in string (ASCII) format.
	*/
	std::wstring GetLocalComputerName();

	std::wstring m_computerName;
	AutoHandle m_ahKeepAliveThread;
};

