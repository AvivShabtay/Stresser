#pragma once
#include <memory>

#include "IArtifact.h"
#include <vector>

class IArtifactSubscriber
{
public:
	IArtifactSubscriber() = default;

	virtual ~IArtifactSubscriber() = default;

	void reset();

	virtual void setNewArtifacts(const std::vector<std::shared_ptr<IArtifact>>& artifacts);

protected:
	std::vector<std::shared_ptr<IArtifact>> m_artifactsToReport;
};
