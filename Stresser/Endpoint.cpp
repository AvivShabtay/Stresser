#include "Endpoint.h"

Endpoint::Endpoint(std::wstring serverURL) {
	if (serverURL.size() <= 0) {
		throw std::runtime_error("The server URL is invalid.");
	}

	this->m_serverURL = serverURL;

#ifdef DEBUG
	/*
	* Use unique value each time while in debug mode because the server keeps
	* endpoint registration for a while before removing it and allowing registering again.
	*/
	auto computerName = this->GetLocalComputerName();
	auto currentDateTime = TimeUtils::GetCurrentDateTime();
	computerName = computerName + std::wstring(CA2W(currentDateTime.c_str()));
	this->m_computerName = computerName;
#else
	this->m_computerName = this->GetLocalComputerName();
#endif // DEBUG


	this->CreateEndpoint();
	this->RunKeepAliveThread();
}

Endpoint& Endpoint::GetInstance(const std::wstring serverURL) {
	// Keep static instance of this class in order to prevent
	// new creations of this class:
	static Endpoint m_instance(serverURL);
	return m_instance;
}

bool Endpoint::CreateEndpoint()
{
	json endpoint;
	endpoint["endpointId"] = 0;
	endpoint["IPAddress"] = "10.10.10.10";
	endpoint["hostname"] = std::string(CW2A(this->m_computerName.c_str()));
	endpoint["apiKey"] = "";
	endpoint["policyId"] = 0;
	endpoint["status"] = "";

	Connection createEndpoint(this->m_serverURL);
	auto responseJson = createEndpoint.SendRequest(L"POST", L"/endpoint", endpoint);
	if (responseJson.size() <= 0) {
		throw std::runtime_error("Server return with no data");
	}

	auto apiKey = responseJson["apiKey"].dump();
	auto endpointID = responseJson["endpointId"].dump();

	std::wstring apiKeyW(CA2W(apiKey.c_str()));
	std::wstring endpointIDW(CA2W(endpointID.c_str()));

	apiKeyW = StringUtils::RemoveQuotationMarks(apiKeyW);
	endpointIDW = StringUtils::RemoveQuotationMarks(endpointIDW);

	this->m_apiKey = apiKeyW;
	this->m_endpoindID = endpointIDW;

	return true;
}

bool Endpoint::KeepAlive()
{
	json endpoint;
	endpoint["apiKey"] = std::string(CW2A(this->m_apiKey.c_str()));

	Connection sendKeepAlive(this->m_serverURL);
	std::wstring uri = L"/endpoint/" + this->m_endpoindID;
	auto responseJson = sendKeepAlive.SendRequest(L"PUT", uri, endpoint);
	if (responseJson.size() <= 0) {
		throw std::runtime_error("Server return with no data");
	}

	std::wcout << "[KEEP ALIVE] " << std::endl;

	return true;
}

std::wstring Endpoint::GetEndpointID()
{
	return this->m_endpoindID;
}

std::wstring Endpoint::GetAPIKey()
{
	return this->m_apiKey;
}

void Endpoint::RunKeepAliveThread()
{
	if (this->m_apiKey.empty()) {
		throw std::runtime_error("Could not start keep alive thread before having API Key");
	}

	AutoHandle hThread(::CreateThread(nullptr, 0,
		[](LPVOID params) {

			auto thisEndpoint = reinterpret_cast<Endpoint*>(params);

			ShutdownSignal& g_signal = ShutdownSignal::GetInstance(L"");
			if (!g_signal.Get()) {
				return static_cast<DWORD>(-1);
			}

			DWORD dwResult = 0;

			while (::WaitForSingleObject(g_signal.Get(), 500) == WAIT_TIMEOUT) {
				dwResult = thisEndpoint->KeepAlive();
				if (!dwResult) {
					break;
				}
			}

			return dwResult;
		},
		this, 0, nullptr));

	this->m_ahKeepAliveThread = std::move(hThread);
}

std::wstring Endpoint::GetLocalComputerName() {
	std::vector<WCHAR> buffer(MAX_COMPUTERNAME_LENGTH + 1);
	DWORD dwNameLength = MAX_COMPUTERNAME_LENGTH + 1;

	if (!GetComputerName(&buffer[0], &dwNameLength)) {
		throw ExceptionWithWin32ErrorCode("Could not get local computer name");
	}

	return std::wstring(&buffer[0]);
}