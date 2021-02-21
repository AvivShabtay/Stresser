#include "TokenManager.h"
#include "../Utils/AutoCriticalSection.h"
#include "../Utils/ShutdownSignal.h"
#include "../Utils/StringUtils.h"
#include "../CommunicationManager/HttpConnection.h"

#include "nlohmann/json.hpp"

#include <atlstr.h>
#include <thread>
#include <Windows.h>

using Json = nlohmann::json;

TokenManager& TokenManager::getInstance(HttpConnectionWithTokens* httpConnection)
{
	static TokenManager g_tokenManager(httpConnection);
	return g_tokenManager;
}

TokenManager::TokenManager(HttpConnectionWithTokens* httpConnection)
	: m_httpConnection(httpConnection)
{
}

void TokenManager::refreshToken(const std::string& endpointID)
{
	const std::wstring uri = L"/endpoint/" + std::wstring(CA2W(endpointID.c_str()));

	Json jsResponse = this->m_httpConnection->SendRequest(L"PUT", uri, "");
	if (jsResponse.empty())
	{
		throw std::runtime_error("Server return with no data");
	}

	std::string newApiKey = jsResponse["apiKey"].dump();
	newApiKey = StringUtils::RemoveQuotationMarks(newApiKey);

	this->SetToken(newApiKey);
}

bool TokenManager::startTokenRefresherThread(const std::string& endpointID, const std::string& initialToken)
{
	this->m_token = initialToken;
	this->m_httpConnection->setToken(initialToken);

	struct ThreadParams
	{
		std::string endpointID;
		TokenManager* tokenManager;
	};

	ThreadParams* threadParams = new ThreadParams;
	threadParams->endpointID = endpointID;
	threadParams->tokenManager = this;

	auto threadStartRoutine = [](auto params) {

		std::unique_ptr<ThreadParams> threadParams(reinterpret_cast<ThreadParams*>(params));
		auto endpointID = threadParams->endpointID;
		auto tokenManager = threadParams->tokenManager;

		ShutdownSignal& g_signal = ShutdownSignal::GetInstance(L"");
		if (!g_signal.Get()) {
			return static_cast<DWORD>(-1);
		}

		while (WAIT_TIMEOUT == WaitForSingleObject(g_signal.Get(), 10000))
		{
			tokenManager->refreshToken(endpointID);
		}

		// TODO: Replace with RAII wrapper:
		return static_cast<DWORD>(1);
	};

	this->m_ahTokenRefresherThread.reset(CreateThread(nullptr, 0, threadStartRoutine, threadParams, 0, nullptr));
	if (INVALID_HANDLE_VALUE == this->m_ahTokenRefresherThread.get())
	{
		return false;
	}

	return true;
}

std::string TokenManager::getToken()
{
	return this->m_token;
}

void TokenManager::SetToken(const std::string& newToken)
{
	{
		AutoCriticalSection autoCriticalSection;
		this->m_token = newToken;
	}

	this->notifyListeners();
}

void TokenManager::addListener(ITokenListener* newListener)
{
	if (nullptr != newListener)
	{
		this->m_listeners.push_back(newListener);
	}
}

void TokenManager::notifyListeners()
{
	if (!this->m_token.empty())
	{
		for (auto& tokenListener : this->m_listeners)
		{
			tokenListener->updateToken(this->m_token);
		}
	}
}