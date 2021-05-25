#pragma once
#include "IArtifactSubscriber.h"
#include "IStresserDetector.h"
#include "EtwManager.h"

class UserModeDetector : public IStresserDetector, public IArtifactSubscriber
{
public:
	explicit UserModeDetector(const EventController& eventController);

	void start() override;
	void stop() override;

private:
	std::unique_ptr<EtwManager> m_etwManager;
};

