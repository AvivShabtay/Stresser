#include "AuthorizedHttpRequest.h"

#include "../Utils/AutoCriticalSection.h"
#include "../Utils/ShutdownSignal.h"
#include "../Utils/StringUtils.h"
#include "../Utils/DebugPrint.h"
#include "../Utils/SehTranslatorGuard.h"

#include <Windows.h>


AuthorizedHttpRequest& AuthorizedHttpRequest::getInstance(const ServerDetails& server, const HANDLE& shutdownEvent)
{
	static AuthorizedHttpRequest g_tokenManager(server, shutdownEvent);
	return g_tokenManager;
}

AuthorizedHttpRequest::AuthorizedHttpRequest(const ServerDetails& server, const HANDLE& shutdownEvent)
	: m_server(server), m_shutdownEvent(shutdownEvent)
{
}

void AuthorizedHttpRequest::refreshToken()
{
	const std::string targetPath("/endpoint/" + m_endpointId);

	Json jsEndpointData;
	jsEndpointData["id"] = this->m_endpointId;
	jsEndpointData["apiKey"] = this->m_token;

	Json jsResponse = this->sendRequest(http::verb::put, targetPath, jsEndpointData);

	if (jsResponse.empty())
	{
		throw std::runtime_error("Server return with no data");
	}

	const std::string newToken = StringUtils::RemoveQuotationMarks(jsResponse["apiKey"].dump());
	//DEBUG_PRINT("[+] Changing token from: " + this->m_token + ", to: " + newToken);
	this->setToken(newToken);
}

void AuthorizedHttpRequest::setToken(const std::string& newToken)
{
	AutoCriticalSection autoCriticalSection;
	this->m_token = newToken;
}

bool AuthorizedHttpRequest::startTokenRefresherThread(const std::string& endpointId, const std::string& initialToken)
{
	this->m_endpointId = endpointId;
	this->m_token = initialToken;

	auto threadStartRoutine = [](auto params)
	{
		SehTranslatorGuard sehTranslatorGuard;

		auto* const tokenManager = reinterpret_cast<AuthorizedHttpRequest*>(params);

		while (WAIT_TIMEOUT == WaitForSingleObject(tokenManager->m_shutdownEvent, 1000))
		{
			tokenManager->refreshToken();
		}

		return static_cast<DWORD>(0);
	};

	this->m_ahTokenRefresherThread.reset(
		CreateThread(nullptr, 0, threadStartRoutine, this, 0, nullptr));

	if (INVALID_HANDLE_VALUE == this->m_ahTokenRefresherThread.get())
	{
		throw std::runtime_error("Could not create thread for refreshing tokens");
	}

	return true;
}

Json AuthorizedHttpRequest::sendRequest(const http::verb& requestMethod, const std::string& target, const Json& payload) const
{
	std::string path = this->m_server.getApiPrefix() + target;

	if (this->m_token.empty())
	{
		return HttpRequest::sendRequest(this->m_server.getUserAgent(), this->m_server.getHttpVersion(), this->m_server.getContentType(), "",
			requestMethod, this->m_server.getHostname(), path, this->m_server.getPort(), payload);
	}
	else
	{
		return HttpRequest::sendRequest(this->m_server.getUserAgent(), this->m_server.getHttpVersion(), this->m_server.getContentType(),
			this->m_token, requestMethod, this->m_server.getHostname(), path, this->m_server.getPort(), payload);
	}
}