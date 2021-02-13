#include <Windows.h>
#include <iostream>
#include <atlstr.h>

#include "nlohmann/json.hpp"
#include "../CommunicationManager/Connection.h"
#include "../StresserExceptions/ExceptionWithWin32ErrorCode.h"
#include "../StresserExceptions/UnexpectedHTTPStatusCodeException.h"
#include "EndpointControllerService.h"

#include "../Utils/ShutdownSignal.h"
#include "EndpointEntity.h"

using json = nlohmann::json;

int wmain(int argc, PWCHAR argv[]) {

	std::wstring hostname(L"stresser-project.herokuapp.com");

	try
	{
		// Create global shutdown event:
		ShutdownSignal& shutdownSignal = ShutdownSignal::GetInstance(L"Shutdown");

		// Create global connection instance:
		IConnection& connection = Connection::GetInstance(hostname);

		// Create global controllers:
		EndpointControllerService& endpointController = EndpointControllerService::GetInstance(connection);

		// Create endpoint and start token refreshing:
		EndpointEntity endpoint = endpointController.CreateEndpoint();
		bool result = endpointController.StartAPIKeyRefresher(endpoint.GetID());

		auto token = StringUtils::RemoveQuotationMarks(endpoint.GetAPIKey());
		std::cout << token << std::endl;

		// TODO: Remove latter:
		::Sleep(5000);
	}
	catch (UnexpectedHTTPStatusCodeException& exception)
	{
		std::wcout << exception.what() << "Status code: " << exception.GetHTTPStatusCode() << std::endl;
	}
	catch (ExceptionWithWin32ErrorCode& exception)
	{
		std::wcout << exception.what() << std::endl;
	}
	catch (std::exception& exception)
	{
		std::wcout << exception.what();

		DWORD dwErrorCode = GetLastError();
		if (dwErrorCode) {
			std::wcout << ", Error code: 0x" << dwErrorCode;
		}

		std::wcout << std::endl;
	}

	return 0;
}