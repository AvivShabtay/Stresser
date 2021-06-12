#include "Controllers.h"
#include "ServerDetails.h"
#include "AuthorizedHttpRequest.h"

#include "UserModeDetector.h"

#include "../Utils/AutoCriticalSection.h"
#include "../Utils/DebugPrint.h"
#include "../Utils/SehTranslatorGuard.h"

#include "nlohmann/json.hpp"

#include <iostream>

#include <Windows.h>

#include "KernelDetector.h"
#include "StresserApplication.h"

using Json = nlohmann::json;

int wmain(int argc, PWCHAR argv[])
{
	try
	{
		SehTranslatorGuard sehTranslatorGuard;

		// Define application globals:
		ServerDetails serverDetails("Stresser Client / 1.0", 11, "application/json", "stresser-project.herokuapp.com", "80", "/api");

		std::unique_ptr<IStresserApplication> application = std::make_unique<StresserApplication>(serverDetails);

		auto consoleHandler = [&application](const DWORD signal) -> BOOL
		{
			if (CTRL_C_EVENT == signal)
			{
				AutoCriticalSection autoCriticalSection;

				DEBUG_TRACE(StresserMainThread, "Counter CTRL + C event, signaling to stop all functionality");

				application->stop();

				return true;
			}
			return false;
		};
		if (!SetConsoleCtrlHandler(reinterpret_cast<PHANDLER_ROUTINE>(&consoleHandler), TRUE))
		{
			throw std::runtime_error("Could not set console handler");
		}

		application->start();

		// Keep the main thread running until user CTRL + C:
		application->waitForShutdown();
	}
	catch (const std::exception& exception)
	{
		DEBUG_PRINT(exception.what());
	}

	return 0;
}
