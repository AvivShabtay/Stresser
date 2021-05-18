#include "KernelDetector.h"

#include "../Utils/AutoHandle.h"
#include "../Utils/DebugPrint.h"
#include "../Utils/SehTranslatorGuard.h"
#include "../Utils/Win32ErrorCodeException.h"
#include "../Utils/StringUtils.h"
#include "../Utils/TimeUtils.h"

#include <exception>
#include <iostream>
#include <sstream>
#include  <iomanip>

#include <conio.h>
#include <stdio.h>

#include <Windows.h>

const std::wstring EVENT_NAME(L"OnFakeProcess");
const std::wstring UM_GLOBAL_FAKE_PROCESS_EVENT(L"Global\\" + EVENT_NAME);
const std::wstring KM_FAKE_PROCESS_EVENT(L"\\BaseNamedObjects\\" + EVENT_NAME);
const ULONG FAKE_PROCESS_ID_FOR_TESTING = 15;
const std::wstring TIME_FORMAT(L"%d/%m/%Y - %H:%M:%S");
constexpr int NUMBER_OF_EVENTS_TO_CONSUME = 10;

void printUsage();
std::string convertSystemTimeToString(const LARGE_INTEGER& time);
void addFakeProcessId(ULONG processId);
void removeFakeProcessId(ULONG processId);
void registerNotificationEventAndWait();
void unregisterNotificationEvent();
void consumeEvents();
void printEvents(const EventsResult& eventsResult);

int main(int argc, PWCHAR argv[])
{

	SehTranslatorGuard sehTranslatorGuard;

	if (argc < 2)
	{
		printUsage();
		return -1;
	}

	try
	{
		const int choice = _wtoi(argv[1]);

		switch (choice)
		{
		case 1:
		{
			// Add fake process ID
			if (argc < 3)
			{
				printUsage();
				return -1;
			}

			const ULONG processId = atoi(reinterpret_cast<PCHAR>(argv[2]));
			std::cout << "Process ID: " << processId << std::endl;

			addFakeProcessId(processId);
			break;
		}
		case 2:
		{
			// Remove fake process ID
			if (argc < 3)
			{
				printUsage();
				return -1;
			}

			const ULONG processId = atoi(reinterpret_cast<PCHAR>(argv[2]));
			std::cout << "Process ID: " << processId << std::endl;

			removeFakeProcessId(processId);
			break;
		}
		case 3:
		{
			// Register notification event

			registerNotificationEventAndWait();
			break;
		}
		case 4:
		{
			// Unregister notification event

			unregisterNotificationEvent();
			break;
		}
		case 5:
		{
			// Get collected events

			consumeEvents();
			break;
		}
		default:
		{
			throw std::runtime_error("Invalid user choice");
		}
		}
	}
	catch (std::exception& exception)
	{
		DEBUG_PRINT(exception.what());
		return -1;
	}

	return 0;
}

void printUsage()
{
	std::cout << "Usage: SimpleMainTester.exe < 1 | 2 | 3 > [parameters]" << std::endl;
	std::cout << "\t1 [pid] - add fake process ID" << std::endl;
	std::cout << "\t2 [pid] - remove fake process ID" << std::endl;
	std::cout << "\t3 - register notification event and wait for signal" << std::endl;
	std::cout << "\t4 - unregister any existing notification event" << std::endl;
	std::cout << "\t5 - consume events" << std::endl;
}

void addFakeProcessId(ULONG processId)
{
	const KernelDetector kernelDetector;
	kernelDetector.addFakeProcessId(processId);

	std::cout << "Add fake process ID: " << FAKE_PROCESS_ID_FOR_TESTING << std::endl;
}

void removeFakeProcessId(ULONG processId)
{
	const KernelDetector kernelDetector;
	kernelDetector.removeFakeProcessId(processId);

	std::cout << "Remove fake process ID: " << FAKE_PROCESS_ID_FOR_TESTING << std::endl;
}

void registerNotificationEventAndWait()
{
	const AutoHandle objectNotificationEvent(CreateEvent(nullptr, TRUE, FALSE, UM_GLOBAL_FAKE_PROCESS_EVENT.c_str()));
	if (!objectNotificationEvent.get())
	{
		throw Win32ErrorCodeException("Could not create global event");
	}

	std::cout << "Event handle: " << std::hex << objectNotificationEvent.get() << std::endl;

	const KernelDetector kernelDetector;

	// Close any existing event:
	kernelDetector.unregisterEvent();

	// Register new event:
	kernelDetector.registerEvent(KM_FAKE_PROCESS_EVENT);

	if (WAIT_OBJECT_0 != WaitForSingleObject(objectNotificationEvent.get(), INFINITE))
	{
		throw Win32ErrorCodeException("Wait for signal from Driver failed");
	}

	// Consume and print events:
	const EventsResult eventsResult = kernelDetector.receiveEvents(NUMBER_OF_EVENTS_TO_CONSUME);
	printEvents(eventsResult);

	// Close the newly shared event:
	kernelDetector.unregisterEvent();
}

void unregisterNotificationEvent()
{
	const KernelDetector kernelDetector;

	// Close any existing event:
	kernelDetector.unregisterEvent();
}

void consumeEvents()
{
	const KernelDetector kernelDetector;

	const EventsResult eventsResult = kernelDetector.receiveEvents(NUMBER_OF_EVENTS_TO_CONSUME);

	printEvents(eventsResult);
}

void printEvents(const EventsResult& eventsResult)
{
	if (0 == eventsResult.size)
	{
		std::cout << "There are no available events" << std::endl;
		return;
	}

	if (nullptr == eventsResult.events)
	{
		throw std::runtime_error("Invalid events buffer");
	}

	for (int i = 0; i < eventsResult.size; ++i)
	{
		const auto& eventInfo = eventsResult.events.get()[i];

		std::wcout
			<< "~~~~Event~~~~\n"
			<< "Called PID=" << eventInfo.processId << "\n"
			<< "Called Filename: " << eventInfo.processName << "\n"
			<< "Fake process ID=" << eventInfo.fakeProcessId << "\n"
			<< "Event time: " << TimeUtils::systemTimeToTimestamp(eventInfo.time, TIME_FORMAT) << "\n"
			<< std::endl;
	}
}

std::string convertSystemTimeToString(const LARGE_INTEGER& time)
{
	//SYSTEMTIME systemTime;
	//FileTimeToSystemTime((FILETIME*)&time, &systemTime);

	FILETIME localTime{ 0 };
	FileTimeToLocalFileTime((FILETIME*)&time, &localTime);

	SYSTEMTIME systemTime;
	FileTimeToSystemTime(&localTime, &systemTime);

	std::stringstream timeStream;
	timeStream << std::setfill('0') << std::setw(2) << systemTime.wDay;
	timeStream << "/";
	timeStream << std::setfill('0') << std::setw(2) << systemTime.wMonth;
	timeStream << "/";
	timeStream << std::setfill('0') << std::setw(4) << systemTime.wYear;
	timeStream << "-";
	timeStream << std::setfill('0') << std::setw(2) << systemTime.wHour;
	timeStream << ":";
	timeStream << std::setfill('0') << std::setw(2) << systemTime.wMinute;
	timeStream << ":";
	timeStream << std::setfill('0') << std::setw(2) << systemTime.wSecond;
	timeStream << ".";
	timeStream << std::setfill('0') << std::setw(3) << systemTime.wMilliseconds;

	std::string timeString(timeStream.str());
	return timeString;
}