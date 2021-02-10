#include <Windows.h>
#include <iostream>
#include <atlstr.h>

#include "nlohmann/json.hpp"
#include "../CommunicationManager/Connection.h"
#include "../StresserExceptions/ExceptionWithWin32ErrorCode.h"
#include "../StresserExceptions/UnexpectedHTTPStatusCodeException.h"
#include "Endpoint.h"

#include "../Utils/ShutdownSignal.h"

using json = nlohmann::json;

int wmain(int argc, PWCHAR argv[]) {

	std::wstring hostname(L"stresser-project.herokuapp.com");

	try
	{
		ShutdownSignal& shutdownSignal = ShutdownSignal::GetInstance(L"Shutdown");
		Endpoint& endpoint = Endpoint::GetInstance(hostname);
		std::wcout << "API key: " << endpoint.GetAPIKey() << std::endl;

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