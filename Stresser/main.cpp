#include "Controllers.h"
#include "Entities.h"
#include "ServerDetails.h"
#include "AuthorizedHttpRequest.h"

#include "ArtifactManager.h"
#include "PolicyNotifications.h"

#include "UserModeDetector.h"

#include "../Utils/AutoCriticalSection.h"
#include "../Utils/DebugPrint.h"
#include "../Utils/SehTranslatorGuard.h"
#include "../Utils/WindowsEvent.h"
#include "../Utils/EventsNames.h"

#include "nlohmann/json.hpp"

#include <iostream>

#include <Windows.h>

using Json = nlohmann::json;

// Used to signal events in the main thread:
WindowsEvent g_shutdownEvent(STOP_STRESSER);


BOOL consoleHandler(DWORD signal);

int wmain(int argc, PWCHAR argv[])
{
	try
	{
		SehTranslatorGuard sehTranslatorGuard;

		if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)consoleHandler, TRUE))
		{
			throw std::runtime_error("Could not set console handler");
		}

		// Define application globals:
		ServerDetails serverDetails("Stresser Client / 1.0", 11, "application/json", "stresser-project.herokuapp.com", "80");

		// Define communication object:
		AuthorizedHttpRequest& authorizedHttpRequest = AuthorizedHttpRequest::getInstance(serverDetails, g_shutdownEvent.get());

		// Define application controllers:
		EndpointController& endpointController = EndpointController::getInstance(authorizedHttpRequest);
		PolicyController& policyController = PolicyController::getInstance(authorizedHttpRequest);
		RuleController& ruleController = RuleController::getInstance(authorizedHttpRequest);
		EventController& eventController = EventController::getInstance(authorizedHttpRequest);

		// Register new endpoint in the server:
		// TODO: Move from here !
		EndpointEntity endpoint = endpointController.createEndpoint();
		std::string endpointId = endpoint.GetID();

		// Start token manager:
		authorizedHttpRequest.startTokenRefresherThread(endpointId, endpoint.GetAPIKey());

		PolicyNotifications policyNotifications(endpointId, g_shutdownEvent.get(), endpointController, policyController, ruleController);

		ArtifactManager artifactManager;

		UserModeDetector userModeDetector(eventController);

		policyNotifications.subscribe(&artifactManager);

		artifactManager.subscribe(&userModeDetector);

		userModeDetector.start();

		// Keep the main thread running until user CTRL + C:
		WaitForSingleObject(g_shutdownEvent.get(), INFINITE);
	}
	catch (const std::exception& exception)
	{
		DEBUG_PRINT(exception.what());
	}

	return 0;
}

BOOL consoleHandler(DWORD signal) {

	if (CTRL_C_EVENT == signal)
	{
		AutoCriticalSection autoCriticalSection;

		DEBUG_PRINT("[*] Counter CTRL + C event, signaling to stop");

		g_shutdownEvent.setEvent();

		return TRUE;
	}

	return FALSE;
}
