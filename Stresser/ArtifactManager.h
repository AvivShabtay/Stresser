#pragma once
#include "IArtifact.h"
#include "IPolicySubscriber.h"

#include <vector>


class ArtifactManager : public IPolicySubscriber
{
public:
	ArtifactManager();
	~ArtifactManager() = default;

	void policyChanged(const std::vector<RuleEntity>& rules) override;

private:
	std::vector<std::unique_ptr<IArtifact>> m_artifactsVector;
};

