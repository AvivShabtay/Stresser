#pragma once
#include "IArtifact.h"
#include "IEtwEventHandler.h"

#include <memory>
#include <vector>


class RegistryEventHandler : public IEtwEventHandler
{
public:
	explicit RegistryEventHandler(std::vector<std::shared_ptr<IArtifact>>& artifacts);

	~RegistryEventHandler() override = default;

	void onEventRecord(PEVENT_RECORD record) override;

private:
	std::vector<std::shared_ptr<IArtifact>>& m_artifacts;
};
