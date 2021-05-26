#include "KernelDetector.h"

#include "../Utils/LocalPcUtils.h"
#include "../Utils/DebugPrint.h"
#include "../Utils/PeResource.h"
#include "../Utils/ServiceManager.h"
#include "../Utils/AutoCriticalSection.h"

#include "resource.h"

#include <sstream>


#include "ProcessArtifact.h"
#include "../Utils/StringUtils.h"
#include "../Utils/TimeUtils.h"

KernelDetector::KernelDetector(const EventController& eventController)
	: IStresserDetector(eventController), m_doesTestSigning(false),
	m_onProcessEvent(UM_ON_FAKE_PROCESS_EVENT_NAME, FALSE, FALSE),
	m_stopDetectionThreadEvent(STOP_FETCH_THREAD_EVENT_NAME)
{
}

KernelDetector::~KernelDetector()
{
	try
	{
		if (this->m_doesTestSigning)
		{
			this->stop();
		}
	}
	catch (const std::exception& exception)
	{
		DEBUG_PRINT(exception.what());
	}
	catch (...)
	{
		DEBUG_WPRINT(STRINGIFY(KernelDetector) "Undefined exception was thrown in destructor");
	}
}

void KernelDetector::start()
{
	this->m_doesTestSigning = LocalPcUtils::doesTestSigningEnabled();
	if (!this->m_doesTestSigning)
	{
		DEBUG_WPRINT(STRINGIFY(start) "Running on system without testsigning enabled");
		DEBUG_WPRINT(STRINGIFY(start) "Could not install process detector driver");

		// Continue Stresser without kernel detector
		return;
	}

	try
	{
		KernelDetector::installStresserDriver();

		this->registerFakeProcessIds();
	}
	catch (const std::exception& exception)
	{
		DEBUG_PRINT(exception.what());
	}
}

void KernelDetector::stop()
{
	if (!this->m_doesTestSigning)
	{
		DEBUG_PRINT(STRINGIFY(stop) "Running on system without testsigning enabled");
		return;
	}

	try
	{
		this->stopDetectionThread();

		KernelDetector::uninstallStresserDriver();
	}
	catch (const std::exception& exception)
	{
		DEBUG_PRINT(exception.what());
	}
}

void KernelDetector::setNewArtifacts(const std::vector<std::shared_ptr<IArtifact>>& artifacts)
{
	if (!this->m_doesTestSigning)
	{
		return;
	}

	AutoCriticalSection autoCriticalSection;

	// Set new artifacts:
	this->m_artifactsToReport = artifacts;
	if (this->m_artifactsToReport.empty())
	{
		return;
	}

	try
	{
		this->stopDetectionThread();

		KernelDetector::removeAllRegisteredFakeProcessIds();

		this->registerFakeProcessIds();

		this->startDetectionThread();
	}
	catch (const std::exception& exception)
	{
		DEBUG_PRINT(exception.what());
	}
}

void KernelDetector::installStresserDriver()
{
	const PeResource resource(STRESSER_ENGINE, L"BIN");
	const std::wstring binaryPath(KernelDetector::createTemporaryPath(STRESSER_DRIVER_NAME_WITH_EXTENSION));

	resource.saveResourceToFileSystem(binaryPath);
	DEBUG_WPRINT(STRINGIFY(installStresserDriver) L"Save Stresser driver locally: " + binaryPath);

	ServiceManager serviceManager(STRESSER_DRIVER_SERVICE_NAME, binaryPath, STRESSER_DRIVER_SERVICE_TYPE);

	serviceManager.installAndStart();
	DEBUG_WPRINT(STRINGIFY(installStresserDriver) L"Install and start Stresser driver service: " + STRESSER_DRIVER_SERVICE_NAME);
}

void KernelDetector::uninstallStresserDriver()
{
	const std::wstring binaryPath(KernelDetector::createTemporaryPath(STRESSER_DRIVER_NAME_WITH_EXTENSION));
	const ServiceManager serviceManager(STRESSER_DRIVER_SERVICE_NAME, binaryPath, STRESSER_DRIVER_SERVICE_TYPE);

	serviceManager.stopAndRemove();
	DEBUG_WPRINT(STRINGIFY(uninstallStresserDriver) L"Stop and uninstall Stresser driver service: " + STRESSER_DRIVER_SERVICE_NAME);

	if (!DeleteFile(binaryPath.c_str()))
	{
		throw Win32ErrorCodeException("Could not delete Stresser driver: " +
			StringUtils::wstringToString(binaryPath));
	}
	DEBUG_WPRINT(STRINGIFY(uninstallStresserDriver) L"Delete Stresser driver binary file");
}

void KernelDetector::registerFakeProcessIds()
{
	if (this->m_artifactsToReport.empty())
	{
		return;
	}

	AutoCriticalSection autoCriticalSection;

	for (const auto& artifact : this->m_artifactsToReport)
	{
		if (ArtifactTypes::Process != artifact->getType())
		{
			return;
		}

		const auto* processArtifact = dynamic_cast<ProcessArtifact*>(artifact.get());

		const ULONG processId = processArtifact->getFakeProcessId();

		const ProcessDetector processDetector;
		processDetector.addFakeProcessId(processId);

		DEBUG_WPRINT(STRINGIFY(startDetection) "Register fake process ID for detection: " + processId);
	}
}

void KernelDetector::removeAllRegisteredFakeProcessIds()
{
	const ProcessDetector processDetector;
	processDetector.removeAllFakeProcessIds();
}

std::wstring KernelDetector::createTemporaryPath(const std::wstring& exeNameWithExtension)
{
	std::wstringstream binPathStream;
	binPathStream << LocalPcUtils::getTempPath();
	binPathStream << "\\";
	binPathStream << exeNameWithExtension;

	return binPathStream.str();
}

void KernelDetector::fetchAndSendEvents(LPVOID params)
{
	auto* kernelDetector = static_cast<KernelDetector*>(params);

	// Check if requested to stop the thread functionality:
	while (!kernelDetector->m_stopDetectionThreadEvent.isSignaled())
	{
		const ProcessDetector processDetector;

		// Check if there are already events:
		const EventsResult eventsResult = processDetector.receiveEvents();
		if (0 < eventsResult.size)
		{
			KernelDetector::onProcessDetectionEvent(eventsResult);
		}
		else
		{
			// Register synchronization event:
			processDetector.registerEvent(KM_ON_FAKE_PROCESS_EVENT_NAME);

			// Wait for event
			if (WAIT_OBJECT_0 == kernelDetector->m_onProcessEvent.wait(WAIT_BETWEEN_FETCH_EVENT_MS))
			{
				const EventsResult eventsResult = processDetector.receiveEvents();
				KernelDetector::onProcessDetectionEvent(eventsResult);
			}
		}
	}
}

void KernelDetector::onProcessDetectionEvent(const EventsResult& eventsResult)
{
	if (0 >= eventsResult.size)
	{
		return;
	}

	const std::unique_ptr<EventInfo[]>& eventInfo = eventsResult.events;

	for (int i = 0; i < eventsResult.size; ++i)
	{
		std::wcout
			<< "~~~~Event~~~~\n"
			<< "Called PID=" << eventInfo[i].processId << "\n"
			<< "Called Filename: " << eventInfo[i].processName << "\n"
			<< "Fake process ID=" << eventInfo[i].fakeProcessId << "\n"
			<< "Event time: " << TimeUtils::systemTimeToTimestamp(eventInfo[i].time, TIME_FORMAT) << "\n"
			<< std::endl;
	}
}

void KernelDetector::startDetectionThread()
{
	this->m_stopDetectionThreadEvent.resetEvent();
	this->m_fetchEventThread.reset([this](auto params)
		{
			KernelDetector::fetchAndSendEvents(params);
		}, this);
}

void KernelDetector::stopDetectionThread()
{
	this->m_stopDetectionThreadEvent.setEvent();
	this->m_fetchEventThread.release();
}