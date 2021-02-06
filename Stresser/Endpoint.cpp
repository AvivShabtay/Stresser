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

}

Endpoint& Endpoint::GetInstance(const std::wstring serverURL) {
	// Keep static instance of this class in order to prevent
	// new creations of this class:
	static Endpoint m_instance(serverURL);
	return m_instance;
}

std::wstring Endpoint::RegisterEndpoint() {
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

	auto apiKey = responseJson["apiKey"];
	std::wstring apiKeyW(CA2W(apiKey.dump().c_str()));

	apiKeyW = StringUtils::RemoveQuotationMarks(apiKeyW);
	return apiKeyW;
}

bool Endpoint::KeepConnectionAlive() {
	return false;
}

std::wstring Endpoint::GetAPIKey() {
	return this->m_apiKey;
}

std::wstring Endpoint::GetLocalComputerName() {
	std::vector<WCHAR> buffer(MAX_COMPUTERNAME_LENGTH + 1);
	DWORD dwNameLength = MAX_COMPUTERNAME_LENGTH + 1;

	if (!GetComputerName(&buffer[0], &dwNameLength)) {
		throw ExceptionWithWin32ErrorCode("Could not get local computer name");
	}

	return std::wstring(&buffer[0]);
}