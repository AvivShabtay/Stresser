#pragma once

#include <iostream>
#include <vector>
#include "nlohmann/json.hpp"
#include "../CommunicationManager/Connection.h"

#include "IEndpoint.h"
#include "../Utils/TimeUtils.h"
#include "../Utils/StringUtils.h"
#include "../Utils/AutoHandle.h"
#include "../Utils/ShutdownSignal.h"

/*
* Represent the local computer as Endpoint in the Stresser security solution.
* Responsible for updating the Stresser server of endpoint status
* and manage the Agent API key for server-client operations.
*/
class Endpoint : public IEndpoint
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
	* Registers this local computer in the application server
	* and get API key to access server services.
	*/
	virtual bool CreateEndpoint() override;

	/*
	* Send data to the application server indicating the this
	* endpoint still running the Stresser application.
	*/
	virtual bool KeepAlive() override;

	/*
	* Returns the current endpoint ID gave by the server when the endpoint registered.
	* @return String representing the endpoint ID value.
	*/
	std::wstring GetEndpointID();

	/*
	* Returns the API key for this local computer (endpoint).
	* @return String representing the API key of this local computer.
	*/
	std::wstring GetAPIKey();


private:
	/*
	* Represent the thread functions.
	* Gets reference to this class ans start sending keep alive messages
	* to the server while the application is running.
	*/
	void RunKeepAliveThread();

	Endpoint(std::wstring serverURL);

	/*
	* Return the local computer name.
	* @return The local computer name in string (ASCII) format.
	*/
	std::wstring GetLocalComputerName();

	std::wstring m_computerName;
	std::wstring m_serverURL;
	std::wstring m_apiKey;
	std::wstring m_endpoindID;
	AutoHandle m_ahKeepAliveThread;
};

