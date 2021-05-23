#pragma once
#include "IArtifact.h"
#include "IEtwEventHandler.h"

#include <vector>


class RegistryEventHandler : public IEtwEventHandler
{
public:
	explicit RegistryEventHandler(std::vector<IArtifact*>& artifacts);

	~RegistryEventHandler() override = default;

	void onEventRecord(PEVENT_RECORD record) override;

private:
	std::vector<IArtifact*>& m_artifacts;
};
