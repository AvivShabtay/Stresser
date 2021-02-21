#pragma once

#include "../Utils/AutoHandle.h"
#include "../CommunicationManager/IHttpConnection.h"
#include "ITokenListener.h"
#include "HttpConnectionWithTokens.h"

#include <iostream>
#include <thread>

/*
 *
 */
class TokenManager
{
public:
	static TokenManager& getInstance(HttpConnectionWithTokens* httpConnection);

	bool startTokenRefresherThread(const std::string& endpointID, const std::string& initialToken);

	std::string getToken();

	void addListener(ITokenListener* newListener);

private:
	TokenManager(HttpConnectionWithTokens* httpConnection);

	void SetToken(const std::string& newToken);

	void refreshToken(const std::string& endpointID);

	void notifyListeners();

	std::string m_token;
	AutoHandle m_ahTokenRefresherThread;
	std::vector<ITokenListener*> m_listeners;
	std::unique_ptr<HttpConnectionWithTokens> m_httpConnection;
	std::thread m_refreshTokenThread;
};

