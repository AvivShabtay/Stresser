#include "Controllers.h"
#include "ServerDetails.h"
#include "AuthorizedHttpRequest.h"

#include "UserModeDetector.h"

#include "../Utils/AutoCriticalSection.h"
#include "../Utils/DebugPrint.h"
#include "../Utils/SehTranslatorGuard.h"

#include "nlohmann/json.hpp"

#include <iostream>

#include "KernelDetector.h"
#include "StresserApplication.h"

#include <Windows.h>

std::shared_ptr<IStresserApplication> g_application;

BOOL consoleHandler(const DWORD signal)
{
	if (CTRL_C_EVENT == signal)
	{
		AutoCriticalSection autoCriticalSection;

		DEBUG_TRACE(StresserMainThread, "Counter CTRL + C event, signaling to stop all functionality");

		g_application->stop();

		return true;
	}
	return false;
}

int wmain(int argc, PWCHAR argv[])
{
	try
	{
		SehTranslatorGuard sehTranslatorGuard;

		ServerDetails serverDetails("Stresser Client / 1.0", 11, "application/json", "stresser-project.herokuapp.com", "80", "/api");

		g_application = std::make_unique<StresserApplication>(serverDetails);

		if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
		{
			throw std::runtime_error("Could not set console handler");
		}

		g_application->start();

		g_application->waitForShutdown();
	}
	catch (const std::exception &exception)
	{
		DEBUG_TRACE(StresserMainThread, exception.what());
		return 1;
	}
	catch (...)
	{
		DEBUG_TRACE(StresserMainThread, "Unknown exeption");
		return 1;
	}

	DEBUG_TRACE(StresserMainThread, "Stresser shuting down...");
	return 0;
}
