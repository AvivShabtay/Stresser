#pragma once

#include <iostream>
#include <vector>
#include "nlohmann/json.hpp"
#include "../CommunicationManager/Connection.h"
#include "../StresserExceptions/ExceptionWithWin32ErrorCode.h"

#include "../Utils/TimeUtils.h"
#include "../Utils/StringUtils.h"

/*
* Represent the local computer as Endpoint in the Stresser security solution.
* Responsible for updating the Stresser server of endpoint status
* and manage the Agent API key for server-client operations.
*/
class Endpoint
{
public:
	/* Disable cloneable*/
	Endpoint(Endpoint& other) = delete;

	/* Disable assignable */
	void operator=(const Endpoint&) = delete;

	/*
	* Return the singleton instance of this class.
	* @param serverURL - the URL of the Stresser server.
	* @return Instance of the Agent's Endpoint class.
	*/
	static Endpoint& GetInstance(const std::wstring serverURL);

	/*
	* Send request to the server with the local computer information
	* and get in return new API key to communicate with the server.
	* @return API key for server-client communication.
	*/
	std::wstring RegisterEndpoint();


private:
	Endpoint(std::wstring serverURL);

	/*
	*
	*/
	bool KeepConnectionAlive();

	/*
	* Returns the API key for this local computer (endpoint).
	* @return String representing the API key of this local computer.
	*/
	std::wstring GetAPIKey();

	/*
	* Return the local computer name.
	* @return The local computer name in string (ASCII) format.
	*/
	std::wstring GetLocalComputerName();

	std::wstring m_computerName;
	std::wstring m_serverURL;
	std::wstring m_apiKey;
};

