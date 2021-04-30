#include "Controllers.h"
#include "Entities.h"
#include "ServerDetails.h"
#include "AuthorizedHttpRequest.h"

#include "EventParser.h"
#include "EtwManager.h"
#include "EtwUtils.h"

#include "../Utils/AutoCriticalSection.h"
#include "../Utils/DebugPrint.h"
#include "../Utils/SehTranslatorGuard.h"
#include "../Utils/StandardThread.h"
#include "../Utils/WindowsEvent.h"
#include "../Utils/TimeUtils.h"
#include "../Utils/EventsNames.h"

#include "nlohmann/json.hpp"

#include "ArtifactFactory.h"

#include <iostream>

#include <Windows.h>

using Json = nlohmann::json;

// Used to signal events in the main thread:
WindowsEvent g_shutdownEvent(STOP_STRESSER);
WindowsEvent g_stopPolicy(STOP_POLICY);

BOOL consoleHandler(DWORD signal);

/*
 * Handler function for ETW events.
 * TODO: Move from here !
 */
void OnEvent(PEVENT_RECORD rec);

int wmain(int argc, PWCHAR argv[])
{
	std::vector<std::unique_ptr<IArtifact>> artifactsVector;

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

		// Register new endpoint in the server:
		// TODO: Move from here !
		EndpointEntity endpoint = endpointController.createEndpoint();
		std::string endpointId = endpoint.GetID();

		// Start token manager:
		authorizedHttpRequest.startTokenRefresherThread(endpointId, endpoint.GetAPIKey());

		// Define ETW event types to be collected:
		// TODO: Move from here !
		std::vector<EtwEventTypes> eventTypes =
		{
			EtwEventTypes::DiskIo,
			EtwEventTypes::DiskFileIo,
			EtwEventTypes::FileIo,
			EtwEventTypes::FileIoInit
		};

		//Create ETW collector:
		// TODO: Move from here !
		EtwManager mgr(eventTypes, OnEvent);
		mgr.start();

		// Create background thread to fetch endpoint configuration from the server:
		// TODO: Move from here !
		StandardThread communication([&endpointController, &policyController, &endpointId, &ruleController, &artifactsVector]()
			{
				EndpointEntity endpointEntity = endpointController.getEndpoint(endpointId);
				DEBUG_PRINT(endpointEntity);

				// Check if there is CTRL + C signal:
				while (WAIT_TIMEOUT == WaitForSingleObject(g_shutdownEvent.get(), 10000))
				{
					PolicyEntity policyEntity = policyController.getPolicy(endpointEntity.GetPolicyID());
					DEBUG_PRINT("Policy ID: " + policyEntity.getId());
					
					g_stopPolicy.setEvent();
					artifactsVector.clear();
					g_stopPolicy.resetEvent();

					for (std::string& ruleId : policyEntity.getRulesIds())
					{
						RuleEntity ruleEntity = ruleController.getRule(ruleId);				

						auto artifact = ArtifactFactory::BuildArtifact(ruleEntity.getType(), ruleEntity.getName(), ruleEntity.getData());
						artifactsVector.push_back(std::move(artifact));
						
						DEBUG_PRINT(ruleEntity);
					}
				}
			});

		// Keep the main thread running until user CTRL + C:
		WaitForSingleObject(g_shutdownEvent.get(), INFINITE);
	}
	catch (const std::exception& exception)
	{
		DEBUG_PRINT(exception.what());
	}

	return 0;
}

void OnEvent(PEVENT_RECORD rec) {

	const EventParser parser(rec);

	const std::wstring timestamp = TimeUtils::systemTimeToTimestamp(parser.getEventHeader().TimeStamp);
	const std::uint32_t processPid = parser.getProcessId();
	const std::wstring eventData = L"Time= " + timestamp + L", PID= " + std::to_wstring(processPid);
	const std::wstring FAKE_FILE(L"StresserTest.txt");

	const UCHAR eventOpcode = parser.getEventHeader().EventDescriptor.Opcode;

	// FileIo_Name, [EventType{0, 32, 35, 36}, EventTypeName{"Name", "FileCreate", "FileDelete", "FileRundown"}]
	if (0 == eventOpcode || 32 == eventOpcode || 35 == eventOpcode || 36 == eventOpcode)
	{
		std::wstring eventType = EtwUtils::getFileIoNameEventTypes()[eventOpcode];
		const EventProperty* fileNameProperty = parser.getProperty(L"FileName");
		if (nullptr != fileNameProperty)
		{
			const std::wstring fileName(fileNameProperty->getUnicodeString());
			if (std::wstring::npos != fileName.find(FAKE_FILE))
			{
				std::wcout << "[!!!] " << eventData << ", " << eventType << ", File name= " << fileName << std::endl;
			}
		}
	}

	// FileIo_Create, [EventType{64}, EventTypeName{"Create"}]
	if (64 == eventOpcode)
	{
		std::wstring eventType = L"FileIo_Create";
		const EventProperty* openPathProperty = parser.getProperty(L"OpenPath");
		if (nullptr != openPathProperty)
		{
			const std::wstring openPath(openPathProperty->getUnicodeString());
			if (std::wstring::npos != openPath.find(FAKE_FILE))
			{
				std::wcout << eventData << ", " << eventType << ", Open path= " << openPath << std::endl;
			}
		}
	}

	// FileIo_DirEnum, [EventType{72, 77}, EventTypeName{"DirEnum", "DirNotify"}]
	if (72 == eventOpcode || 77 == eventOpcode)
	{
		std::wstring eventType = EtwUtils::getFileIoNameEventTypes()[eventOpcode];
		const EventProperty* fileNameProperty = parser.getProperty(L"FileName");
		if (nullptr != fileNameProperty)
		{
			const std::wstring fileName(fileNameProperty->getUnicodeString());
			if (std::wstring::npos != fileName.find(FAKE_FILE))
			{
				std::wcout << eventData << ", " << eventType << ", File name= " << fileName << std::endl;
			}
		}
	}

	if (EVENT_TRACE_TYPE_REGOPEN == eventOpcode)
	{
		const std::wstring eventType = L"Open key event";
		const EventProperty* keyNameProperty = parser.getProperty(L"KeyName");
		if (nullptr != keyNameProperty)
		{
			const std::wstring keyName(keyNameProperty->getUnicodeString());
			std::wcout << eventData << ", " << eventType << ", Open path= " << keyName << std::endl;
		}
	}
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
