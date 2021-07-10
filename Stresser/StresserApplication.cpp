#include "StresserApplication.h"
#include "Controllers.h"
#include "NetworkConnectionException.h"

#include "../Utils/EventsNames.h"
#include "../Utils/DebugPrint.h"
#include "../Utils/SehTranslatorGuard.h"
#include "../Utils/LocalPcUtils.h"

StresserApplication::StresserApplication(ServerDetails serverDetails)
	: m_shutdownEvent(STOP_STRESSER),
	m_lostConnection(LOST_CONNECTION_EVENT_NAME)
{
	this->m_artifactManager.reset(new ArtifactManager());

	// Define communication object:
	AuthorizedHttpRequest& authorizedHttpRequest = AuthorizedHttpRequest::getInstance(serverDetails, m_shutdownEvent.get());

	this->initializeEndpoint(authorizedHttpRequest);

	this->initializeDetectors(authorizedHttpRequest);

	this->startNetworkConnectionMonitoringThread();
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

void StresserApplication::startNetworkConnectionMonitoringThread()
{
	this->m_networkConnectionMonitorThread = StandardThread([this]()
		{
			this->networkConnectionMonitor();
		});
}

void StresserApplication::networkConnectionMonitor() const
{
	SehTranslatorGuard sehTranslatorGuard;

	constexpr int SLEEP_BETWEEN_RECONNECTION = 30 * 1000;
	constexpr int TRIGGER_EVENTS_COUNT = 2;

	HANDLE triggerEvents[TRIGGER_EVENTS_COUNT] =
	{
		this->m_shutdownEvent.get(),
		this->m_lostConnection.get()
	};

	do
	{
		const DWORD waitResult = WaitForMultipleObjects(TRIGGER_EVENTS_COUNT, triggerEvents, FALSE, INFINITE);
		if (WAIT_OBJECT_0 >= waitResult)
		{
			break;
		}

		if (this->m_shutdownEvent.isSignaled())
		{
			// Signaled to stop the application, exit from the thread
			break;
		}

		if (!this->m_lostConnection.isSignaled())
		{
			// No need to reconnect, network connection available
			continue;
		}

		DEBUG_TRACE(StresserApplication::networkConnectionMonitor, "Try reconnecting to server");

		try
		{
			this->reconnect();
			this->m_lostConnection.resetEvent();

			DEBUG_TRACE(StresserApplication::networkConnectionMonitor, "Successfully reconnected to server");
		}
		catch (const NetworkConnectionException&)
		{
			DEBUG_TRACE(StresserApplication::networkConnectionMonitor, "Could not reconnect yet, network is unavailable");
			Sleep(SLEEP_BETWEEN_RECONNECTION);
		}
		catch (const std::exception& exception)
		{
			DEBUG_TRACE(StresserApplication::networkConnectionMonitor, "Exception thrown in reconnect, Error: ", exception.what());
		}

	} while (true);
}

void StresserApplication::reconnect() const
{
	if (!LocalPcUtils::doesNetworkConnectionAvailable())
	{
		throw NetworkConnectionException("Network connection doesn't available");
	}

	AuthorizedHttpRequest& authorizedHttpRequest = AuthorizedHttpRequest::getInstance();
	EndpointController& endpointController = EndpointController::getInstance(authorizedHttpRequest);

	// Re-register the endpoint in the server:
	EndpointEntity endpoint = endpointController.createEndpoint();
	const std::string endpointId = endpoint.GetID();

	// Restart token refreshing:
	authorizedHttpRequest.startTokenRefresherThread(endpointId, endpoint.GetAPIKey());

	// Reinitialize policy fetching:
	this->m_policyNotifications->resetEndpointId(endpointId);
	this->m_policyNotifications->startFetchPolicyThread();
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
