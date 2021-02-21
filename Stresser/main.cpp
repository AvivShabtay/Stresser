
#include "../CommunicationManager/IHttpConnection.h"
#include "../CommunicationManager/HttpConnection.h"
#include "../StresserExceptions/ExceptionWithWin32ErrorCode.h"
#include "../StresserExceptions/UnexpectedHTTPStatusCodeException.h"
#include "IEndpointController.h"
#include "EndpointController.h"
#include "HttpConnectionWithTokens.h"

#include "../Utils/ShutdownSignal.h"
#include "EndpointEntity.h"
#include "TokenManager.h"

#include <iostream>
#include <atlstr.h>

#include "nlohmann/json.hpp"
#include <Windows.h>

#include "../Utils/AutoCriticalSection.h"

using Json = nlohmann::json;

bool g_running = true;

BOOL consoleHandler(DWORD signal);

int wmain(int argc, PWCHAR argv[]) {

	try
	{
		if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
		{
			throw std::runtime_error("Could not set console handler");
		}


		// Create global shutdown event:
		ShutdownSignal& shutdownSignal = ShutdownSignal::GetInstance(L"Shutdown");

		// Create global controllers:
		IEndpointController& endpointController = EndpointController::GetInstance(new HttpConnectionWithTokens());

		// Create endpoint and start token refreshing:
		EndpointEntity endpoint = endpointController.CreateEndpoint();

		// Start token manager:
		TokenManager& g_tokenManager = TokenManager::getInstance(new HttpConnectionWithTokens());

		// Register token listeners:
		g_tokenManager.addListener(&endpointController);

		// Start refreshing tokens:
		const std::string endpointID = endpoint.GetID();
		const std::string endpointInitialToken = endpoint.GetAPIKey();
		g_tokenManager.startTokenRefresherThread(endpointID, endpointInitialToken);

		while (g_running)
		{
			const EndpointEntity endpointEntity = endpointController.GetEndpoint(endpointID);
			std::cout << "\n" << endpointEntity << std::endl;
			Sleep(1000);
		}
	}
	catch (std::exception& exception)
	{
		std::wcout << exception.what() << std::endl;
	}

	return 0;
}

BOOL consoleHandler(DWORD signal) {

	if (signal == CTRL_C_EVENT)
	{
		AutoCriticalSection autoCriticalSection;
		g_running = false;
	}

	return TRUE;
}