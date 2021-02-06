#include <Windows.h>
#include <iostream>
#include <atlstr.h>

#include "nlohmann/json.hpp"
#include "../CommunicationManager/Connection.h"
#include "../StresserExceptions/ExceptionWithWin32ErrorCode.h"
#include "../StresserExceptions/UnexpectedHTTPStatusCodeException.h"
#include "Endpoint.h"

using json = nlohmann::json;

std::wstring RegisiterEndpoint();


int wmain(int argc, PWCHAR argv[]) {

	auto token = RegisiterEndpoint();
	std::wcout << token << std::endl;

	return 0;
}

std::wstring RegisiterEndpoint() {

	std::wstring hostname(L"stresser-project.herokuapp.com");

	try
	{
		Endpoint& endpoint = Endpoint::GetInstance(hostname);
		auto apikey = endpoint.RegisterEndpoint();
		return apikey;
	}
	catch (UnexpectedHTTPStatusCodeException& exception)
	{
		std::wcout << exception.what() << "Status code: " << exception.GetHTTPStatusCode() << std::endl;
		return L"";
	}
	catch (ExceptionWithWin32ErrorCode& exception)
	{
		std::wcout << exception.what() << std::endl;
		return L"";
	}
	catch (std::exception& exception)
	{
		std::wcout << exception.what();

		DWORD dwErrorCode = GetLastError();
		if (dwErrorCode) {
			std::wcout << ", Error code: 0x" << dwErrorCode;
		}

		std::wcout << std::endl;
		return L"";
	}
}