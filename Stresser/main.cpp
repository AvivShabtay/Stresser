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
		Connection& connection = Connection::GetInstance();
		connection.SetServerURL(hostname);

		// Create global controllers:
		EndpointControllerService& endpointController = EndpointControllerService::GetInstance(connection);

		// Get current endpoint data:
		EndpointEntity endpoint = endpointController.CreateEndpoint();
		endpoint = endpointController.GetEndpoint(endpoint.GetID());

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