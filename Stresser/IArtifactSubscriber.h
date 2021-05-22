#pragma once
#include "IArtifact.h"
#include <vector>

class IArtifactSubscriber
{
public:
	virtual ~IArtifactSubscriber() = default;

	void reset();

	void setNewArtifacts(const std::vector<IArtifact*>& artifacts);

protected:
	std::vector<IArtifact*> m_artifactsToReport;
};
