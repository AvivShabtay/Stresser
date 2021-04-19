#include "Controllers.h"
#include "Entities.h"
#include "ServerDetails.h"
#include "AuthorizedHttpRequest.h"

#include "EventParser.h"
#include "EtwManager.h"

#include "../Utils/AutoCriticalSection.h"
#include "../Utils/DebugPrint.h"
#include "../Utils/SehTranslatorGuard.h"
#include "../Utils/StandardThread.h"
#include "../Utils/WindowsEvent.h"

#include "nlohmann/json.hpp"

#include <iostream>

// Add before any ATL header:
// https://social.msdn.microsoft.com/Forums/vstudio/en-US/371a069b-f428-47b5-8f7b-e9ad85964bce/guidnull-undeclared-identifier?forum=vclanguage
#include <cguid.h>

#include <atlbase.h>
#include <atltime.h>
#include <Windows.h>

using Json = nlohmann::json;

bool g_running = true;

BOOL consoleHandler(DWORD signal);

int wmain(int argc, PWCHAR argv[]) {

	try
	{
		SehTranslatorGuard sehTranslatorGuard;

		if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
		{
			throw std::runtime_error("Could not set console handler");
		}

		// Create application globals:
		ShutdownSignal& shutdownSignal = ShutdownSignal::GetInstance();
		ServerDetails serverDetails("Stresser Client / 1.0", 11, "application/json", "stresser-project.herokuapp.com", "80");
		AuthorizedHttpRequest& authorizedHttpRequest = AuthorizedHttpRequest::getInstance(serverDetails);

		EndpointController& endpointController = EndpointController::getInstance(authorizedHttpRequest);
		PolicyController& policyController = PolicyController::getInstance(authorizedHttpRequest);
		RuleController& ruleController = RuleController::getInstance(authorizedHttpRequest);

		// Register new endpoint in the server:
		EndpointEntity endpoint = endpointController.createEndpoint();

		// Start token manager:
		authorizedHttpRequest.startTokenRefresherThread(endpoint.GetID(), endpoint.GetAPIKey());

		while (g_running)
		{
			EndpointEntity endpointEntity = endpointController.getEndpoint(endpoint.GetID());
			DEBUG_PRINT(endpointEntity);

			PolicyEntity policyEntity = policyController.getPolicy(endpointEntity.GetPolicyID());
			DEBUG_PRINT("Policy ID: " + policyEntity.getId());

			for (RuleEntity& ruleWithOnlyId : policyEntity.getRules())
			{
				RuleEntity ruleEntity = ruleController.getRule(ruleWithOnlyId.getId());
				DEBUG_PRINT(ruleEntity);
			}

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