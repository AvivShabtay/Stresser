#pragma once
#include "IArtifact.h"
#include "IPolicySubscriber.h"
#include "IArtifactSubscriber.h"

#include <vector>


class ArtifactManager : public IPolicySubscriber
{
public:
	~ArtifactManager() = default;

	void policyChanged(const std::vector<RuleEntity>& rules) override;

	void subscribe(IArtifactSubscriber* subscriber);

private:
	std::vector<IArtifact*> getArtifactsByType(const std::string& type);

	std::vector<std::unique_ptr<IArtifact>> m_artifactsVector;
	std::vector<IArtifactSubscriber*> m_subscribers;
};
