#include "EndpointEntity.h"
#include "AuthorizedHttpRequest.h"
#include "EndpointController.h"
#include "ServerDetails.h"

#include "../Utils/AutoCriticalSection.h"
#include "../Utils/ShutdownSignal.h"
#include "../Utils/DebugPrint.h"

#include "nlohmann/json.hpp"

#include <iostream>
#include <atlstr.h>

#include <Windows.h>

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


		// Create application globals:
		ShutdownSignal& shutdownSignal = ShutdownSignal::GetInstance();
		ServerDetails serverDetails("Stresser Client / 1.0", 11, "application/json", "stresser-project.herokuapp.com", "80");
		AuthorizedHttpRequest& authorizedHttpRequest = AuthorizedHttpRequest::getInstance(serverDetails);
		EndpointController& endpointController = EndpointController::getInstance(authorizedHttpRequest);

		// Register new endpoint in the server:
		EndpointEntity endpoint = endpointController.createEndpoint();

		// Start token manager:
		authorizedHttpRequest.startTokenRefresherThread(endpoint.GetID(), endpoint.GetAPIKey());


		while (g_running)
		{
			const EndpointEntity endpointEntity = endpointController.getEndpoint(endpoint.GetID());
			DEBUG_PRINT(endpointEntity);
			Sleep(500);
		}
	}
	catch (std::exception& exception)
	{
		DEBUG_PRINT(exception.what());
	}

	return 0;
}

BOOL consoleHandler(DWORD signal) {

	if (CTRL_C_EVENT == signal)
	{
		AutoCriticalSection autoCriticalSection;
		g_running = false;
	}

	return TRUE;
}