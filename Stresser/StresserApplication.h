#pragma once
#include "IStresserApplication.h"
#include "ServerDetails.h"
#include "ArtifactManager.h"
#include "PolicyNotifications.h"
#include "KernelDetector.h"
#include "UserModeDetector.h"

#include "../Utils/StandardThread.h"

class StresserApplication : public IStresserApplication
{
public:
	explicit StresserApplication(ServerDetails serverDetails);

	StresserApplication(const StresserApplication&) = delete;

	StresserApplication(StresserApplication&&) = delete;

	StresserApplication& operator=(StresserApplication&&) = delete;

	~StresserApplication() override;

	void start() override;

	void stop() override;

	void pause() override;

	void waitForShutdown() override;

private:
	void initializeEndpoint(AuthorizedHttpRequest& authorizedHttpRequest);
	void initializeDetectors(AuthorizedHttpRequest& authorizedHttpRequest);

	void startNetworkConnectionMonitoringThread();

	void networkConnectionMonitor() const;

	void reconnect() const;

	WindowsEvent m_shutdownEvent;
	WindowsEvent m_lostConnection;
	std::unique_ptr<PolicyNotifications> m_policyNotifications;
	std::unique_ptr<ArtifactManager> m_artifactManager;
	std::vector<std::unique_ptr<IStresserDetector>> m_detectors;
	StandardThread m_networkConnectionMonitorThread;
};

