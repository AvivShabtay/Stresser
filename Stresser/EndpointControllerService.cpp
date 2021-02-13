#include "EndpointControllerService.h"

EndpointEntity EndpointControllerService::GetEndpoint(std::string endpointId)
{
	return EndpointEntity();
}

EndpointControllerService::EndpointControllerService(IConnection& connection)
	: IEndpointControllerService(connection) {

	this->m_connection = Connection::GetInstance();

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
	this->StartAPIKeyRefresher();
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

bool EndpointControllerService::KeepAlive(EndpointEntity& endpoint)
{
	std::wstring uri = L"/endpoint/" + std::wstring(CA2W(endpoint.GetID().c_str()));

	auto responseJson = this->m_connection.SendRequest(L"PUT", uri, "");
	if (responseJson.size() <= 0) {
		throw std::runtime_error("Server return with no data");
	}

	std::wcout << "[KEEP ALIVE] " << std::endl;

	return true;
}

void EndpointControllerService::StartAPIKeyRefresher()
{
	auto threadStartRoutine = [](auto params) {

		auto thisEndpoint = reinterpret_cast<EndpointControllerService*>(params);

		ShutdownSignal& g_signal = ShutdownSignal::GetInstance(L"");
		if (!g_signal.Get()) {
			return static_cast<DWORD>(-1);
		}

		DWORD dwResult = 0;

		while (::WaitForSingleObject(g_signal.Get(), 500) == WAIT_TIMEOUT) {
			//dwResult = thisEndpoint->KeepAlive();
			if (!dwResult) {
				break;
			}
		}

		return dwResult;
	};

	this->m_ahKeepAliveThread.reset(::CreateThread(nullptr, 0, threadStartRoutine, this, 0, nullptr));
}

std::wstring EndpointControllerService::GetLocalComputerName() {
	std::vector<WCHAR> buffer(MAX_COMPUTERNAME_LENGTH + 1);
	DWORD dwNameLength = MAX_COMPUTERNAME_LENGTH + 1;

	if (!GetComputerName(&buffer[0], &dwNameLength)) {
		throw ExceptionWithWin32ErrorCode("Could not get local computer name");
	}

	return std::wstring(&buffer[0]);
}