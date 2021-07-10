#include "StresserApplication.h"
#include "Controllers.h"

#include "../Utils/EventsNames.h"
#include "../Utils/DebugPrint.h"

StresserApplication::StresserApplication(const ServerDetails serverDetails)
	: m_shutdownEvent(STOP_STRESSER), m_artifactManager(new ArtifactManager())
{
	// Define communication object:
	AuthorizedHttpRequest& authorizedHttpRequest = AuthorizedHttpRequest::getInstance(serverDetails, m_shutdownEvent.get());

	this->initializeEndpoint(authorizedHttpRequest);

	this->initializeDetectors(authorizedHttpRequest);
}

StresserApplication::~StresserApplication()
{
	try
	{
		this->StresserApplication::stop();
		for (auto& detector : this->m_detectors)
		{
			detector->stop();
		}
	}
	catch (const std::exception& exception)
	{
		DEBUG_TRACE(StresserApplication, "Exception in destructor\n", exception.what());
	}
	catch (...)
	{
		DEBUG_TRACE(StresserApplication, "Undefined exception in destructor");
	}
}

void StresserApplication::initializeEndpoint(AuthorizedHttpRequest& authorizedHttpRequest)
{
	// Define application controllers:
	EndpointController& endpointController = EndpointController::getInstance(authorizedHttpRequest);
	PolicyController& policyController = PolicyController::getInstance(authorizedHttpRequest);
	RuleController& ruleController = RuleController::getInstance(authorizedHttpRequest);

	// Register new endpoint in the server:
	EndpointEntity endpoint = endpointController.createEndpoint();
	std::string endpointId = endpoint.GetID();

	// Start token manager:
	authorizedHttpRequest.startTokenRefresherThread(endpointId, endpoint.GetAPIKey());

	this->m_policyNotifications = std::make_unique<PolicyNotifications>(endpointId, this->m_shutdownEvent.get(), endpointController, policyController, ruleController);

	this->m_policyNotifications->subscribe(this->m_artifactManager.get());
}

void StresserApplication::initializeDetectors(AuthorizedHttpRequest& authorizedHttpRequest)
{
	EventController& eventController = EventController::getInstance(authorizedHttpRequest);

	std::unique_ptr<UserModeDetector> userModeDetector(new UserModeDetector(eventController));
	std::unique_ptr<KernelDetector> kernelDetector(new KernelDetector(eventController));

	this->m_artifactManager->subscribe(userModeDetector.get());
	this->m_artifactManager->subscribe(kernelDetector.get());

	this->m_detectors.push_back(std::move(userModeDetector));
	this->m_detectors.push_back(std::move(kernelDetector));
}

void StresserApplication::start()
{
	try
	{
		for (auto& detector : this->m_detectors)
		{
			detector->start();
		}
	}
	catch (const std::exception& exception)
	{
		DEBUG_TRACE(StresserApplication, "Exception was thrown in stresser start", exception.what());
		throw;
	}
}

void StresserApplication::stop()
{
	try
	{
		this->m_shutdownEvent.setEvent();
	}
	catch (const std::exception& exception)
	{
		DEBUG_TRACE(StresserApplication, "Exception was thrown in stresser stop", exception.what());
		throw;
	}
}

void StresserApplication::pause()
{
	try
	{
		for (auto& detector : this->m_detectors)
		{
			detector->stop();
		}
	}
	catch (const std::exception& exception)
	{
		DEBUG_TRACE(StresserApplication, "Exception was thrown in stresser pause", exception.what());
		throw;
	}
}

void StresserApplication::waitForShutdown()
{
	try
	{
		if (WAIT_OBJECT_0 != this->m_shutdownEvent.wait())
		{
			throw std::runtime_error("Unexpected error with the shutdown wait!");
		}
	}
	catch (const std::exception& exception)
	{
		DEBUG_TRACE(StresserApplication, "Exception was thrown in stresser waitForShutdown", exception.what());
		throw;
	}
}
