#include "ArtifactManager.h"
#include "ArtifactFactory.h"

ArtifactManager::ArtifactManager()
{
}

void ArtifactManager::policyChanged(const std::vector<RuleEntity>& rules)
{
	this->m_artifactsVector.clear();
	
	for (const RuleEntity& ruleEntity : rules)
	{
		auto artifact = ArtifactFactory::BuildArtifact(ruleEntity.getType(), ruleEntity.getName(), ruleEntity.getData());
		this->m_artifactsVector.push_back(std::move(artifact));
	}
}
