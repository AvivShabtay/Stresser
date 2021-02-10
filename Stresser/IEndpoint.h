#pragma once

class IEndpoint
{
public:
	/*
	* Send POST request to the server with the local computer information
	* and get in return new API key to communicate with the server.
	* @return True if successfully registered the endpoint, otherwise returns false.
	*/
	virtual bool CreateEndpoint() = 0;

	/*
	* Send PUT request to the server with the endpoint ID and API key
	* for updating the endpoint status.
	* @return True if successfully update the endpoint status, otherwise returns false.
	*/
	virtual bool KeepAlive() = 0;
};
