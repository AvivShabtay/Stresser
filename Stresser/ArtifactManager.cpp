#include "ArtifactManager.h"
#include "ArtifactFactory.h"
#include "../Utils/AutoCriticalSection.h"

void ArtifactManager::policyChanged(const std::vector<RuleEntity>& rules)
{
	{
		AutoCriticalSection autoCriticalSection;

		// Uninstall the old artifacts
		for (auto* subscriber : this->m_subscribers)
		{
			subscriber->reset();
		}

		this->m_artifactsVector.clear();
	}

	// Getting the new artifacts
	for (const RuleEntity& ruleEntity : rules)
	{
		std::shared_ptr<IArtifact> artifact = ArtifactFactory::BuildArtifact(ruleEntity.getType(),
			ruleEntity.getName(), ruleEntity.getData());

		this->m_artifactsVector.push_back(artifact);
	}

	// Update the subscribers artifacts
	for (auto* subscriber : this->m_subscribers)
	{
		subscriber->setNewArtifacts(this->m_artifactsVector);
	}
}

void ArtifactManager::subscribe(IArtifactSubscriber* subscriber)
{
	this->m_subscribers.push_back(subscriber);
}
