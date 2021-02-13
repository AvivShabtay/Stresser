#include "EndpointControllerService.h"

EndpointEntity EndpointControllerService::GetEndpoint(std::string endpointId)
{
	return EndpointEntity();
}

EndpointControllerService::EndpointControllerService(IConnection& connection)
	: IEndpointControllerService(connection) {

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

void EndpointControllerService::RefreshToken(std::string endpointID)
{
	std::wstring uri = L"/endpoint/" + std::wstring(CA2W(endpointID.c_str()));

	auto jsResponse = this->m_connection.SendRequest(L"PUT", uri, "");
	if (jsResponse.size() <= 0) {
		throw std::runtime_error("Server return with no data");
	}

	auto newApiKey = jsResponse["apiKey"].dump();
	newApiKey = StringUtils::RemoveQuotationMarks(newApiKey);

	this->m_connection.SetToken(newApiKey);
}

EndpointControllerService& EndpointControllerService::GetInstance(IConnection& connection) {
	// Keep static instance of this class in order to prevent
	// new creations of this class:
	static EndpointControllerService g_instance(connection);
	return g_instance;
}

EndpointEntity EndpointControllerService::CreateEndpoint()
{
	EndpointEntity endpointEntity;
	endpointEntity.SetIPAddress("10.10.10.10");
	endpointEntity.SetHostname(std::string(CW2A(this->m_computerName.c_str())));

	Json jsEndpoint = EndpointEntity::ConvertFromEntity(endpointEntity);

	auto responseJson = this->m_connection.SendRequest(L"POST", L"/endpoint", jsEndpoint);
	if (responseJson.size() <= 0) {
		throw std::runtime_error("Server return with no data");
	}

	return EndpointEntity::ConvertFromJson(responseJson);
}

bool EndpointControllerService::StartAPIKeyRefresher(std::string endpointID)
{
	struct ThreadParams
	{
		std::string endpointID;
		EndpointControllerService* controller;
	};

	ThreadParams* threadParams = new ThreadParams;
	threadParams->endpointID = endpointID;
	threadParams->controller = this;

	auto threadStartRoutine = [](auto params) {

		ThreadParams* threadParams = reinterpret_cast<ThreadParams*>(params);
		auto endpointID = threadParams->endpointID;
		auto thisEndpoint = threadParams->controller;

		ShutdownSignal& g_signal = ShutdownSignal::GetInstance(L"");
		if (!g_signal.Get()) {
			return static_cast<DWORD>(-1);
		}

		while (::WaitForSingleObject(g_signal.Get(), 500) == WAIT_TIMEOUT) {
			thisEndpoint->RefreshToken(endpointID);
		}

		// TODO: Replace with RAII wrapper:
		delete threadParams;
		return (DWORD)1;
	};

	this->m_ahKeepAliveThread.reset(::CreateThread(nullptr, 0, threadStartRoutine, threadParams, 0, nullptr));
	if (!this->m_ahKeepAliveThread.get()) {
		return true;
	}

	return false;
}

std::wstring EndpointControllerService::GetLocalComputerName() {
	std::vector<WCHAR> buffer(MAX_COMPUTERNAME_LENGTH + 1);
	DWORD dwNameLength = MAX_COMPUTERNAME_LENGTH + 1;

	if (!GetComputerName(&buffer[0], &dwNameLength)) {
		throw ExceptionWithWin32ErrorCode("Could not get local computer name");
	}

	return std::wstring(&buffer[0]);
}