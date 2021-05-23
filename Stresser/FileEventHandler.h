#pragma once
#include "IArtifact.h"
#include "IEtwEventHandler.h"

#include <vector>

class FileEventHandler : public IEtwEventHandler
{
public:
	explicit FileEventHandler(std::vector<IArtifact*>& artifacts);

	~FileEventHandler() override = default;

	void onEventRecord(PEVENT_RECORD record) override;

private:
	std::vector<IArtifact*>& m_artifacts;
};
