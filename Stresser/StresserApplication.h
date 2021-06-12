#pragma once
#include "IStresserApplication.h"
#include "ServerDetails.h"
#include "ArtifactManager.h"
#include "PolicyNotifications.h"
#include "KernelDetector.h"
#include "UserModeDetector.h"

class StresserApplication : public IStresserApplication
{
public:
	explicit StresserApplication(const ServerDetails serverDetails);

	~StresserApplication() override;

	void start() override;

	void stop() override;

	void pause() override;

	void waitForShutdown() override;

private:
	void initializeEndpoint(AuthorizedHttpRequest& authorizedHttpRequest);
	void initializeDetectors(AuthorizedHttpRequest& authorizedHttpRequest);

	WindowsEvent m_shutdownEvent;
	std::unique_ptr<PolicyNotifications> m_policyNotifications;
	std::unique_ptr<ArtifactManager> m_artifactManager;
	std::vector<std::unique_ptr<IStresserDetector>> m_detectors;
};

