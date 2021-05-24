#include "KernelDetector.h"
#include "ProcessDetector.h"

#include "../Utils/LocalPcUtils.h"
#include "../Utils/DebugPrint.h"
#include "../Utils/PeResource.h"
#include "../Utils/ServiceManager.h"
#include "../Utils/AutoCriticalSection.h"

#include "resource.h"

#include <sstream>


#include "ProcessArtifact.h"
#include "../Utils/StringUtils.h"

KernelDetector::KernelDetector(const EventController& eventController)
	: IStresserDetector(eventController), m_doesTestSigning(false)
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

	try
	{
		// TODO: Stop detection thread

		// TODO: Send I\O to stop detection of all the processes
		this->removeAllRegisteredFakeProcessIds();

		// TODO: Consume all available events

		// TODO: Send I\O with all the new process IDs
		this->registerFakeProcessIds();

		// TODO: start detection thread
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

	{
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
