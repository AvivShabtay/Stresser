#pragma once
#include "IEtwEventHandler.h"
#include "IArtifactSubscriber.h"

class RegistryEventHandler : public IEtwEventHandler, public IArtifactSubscriber
{
public:
	RegistryEventHandler();

	~RegistryEventHandler() override = default;

	void onEventRecord(PEVENT_RECORD record) override;
};
