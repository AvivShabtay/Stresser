#pragma once

#include "ServerDetails.h"
#include "HttpRequest.h"
#include "../Utils/AutoHandle.h"

#include "nlohmann/json.hpp"

#include <boost/beast/http.hpp>

#include <string>

using Json = nlohmann::json;

/*
 * Used to provide communication with the application server by managing unique token
 * representing authorized endpoint.
 */
class AuthorizedHttpRequest
{
public:
	/*
	 * Return instance of this class for the given server configuration.
	 * @param server - Current server configuration.
	 * @shutdownEvent - Handle to signal object representing the application to stop.
	 * @note - This class is NOT responsible to close the signale object.
	 */
	static AuthorizedHttpRequest& getInstance(ServerDetails server = ServerDetails(), HANDLE shutdownEvent = 0);

	/*
	 * Start sending request to the server to keep the endpoint token valid.
	 * @param endpointId - Unique ID of the endpoint that received from the server.
	 * @param initialToken - Token value that was received from the server before.
	 */
	bool startTokenRefresherThread(const std::string& endpointId, const std::string& initialToken);

	/*
	 * Simplified HTTP request that automatically inject the valid token value.
	 * @param requestMethod - Boost	type represent request type.
	 * @param target - Represent the URI path in the server.
	 * @param payload - The data to be send in the HTTP request.
	 */
	Json sendRequest(const http::verb& requestMethod, const std::string& target, const Json& payload) const;

private:
	/* Keep class as singleton. */
	AuthorizedHttpRequest(ServerDetails server, HANDLE shutdownEvent);

	/*
	 * Send HTTP request the expected arguments to the server for refreshing the
	 * endpoint token.
	 * @note Using Endpoint data such as ID.
	 */
	void refreshToken();

	/*
	 * Thread safe setter for token value.
	 * @param newToken - The new token value.
	 */
	void setToken(const std::string& newToken);

	ServerDetails m_server;
	std::string m_endpointId;
	std::string m_token;
	AutoHandle m_ahTokenRefresherThread;
	std::thread m_refreshTokenThread;
	HANDLE m_shutdownEvent;
};

