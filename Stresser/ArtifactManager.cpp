#include "ArtifactManager.h"
#include "ArtifactFactory.h"

void ArtifactManager::policyChanged(const std::vector<RuleEntity>& rules)
{
	// Uninstall the old artifacts
	for (auto subscriber : this->m_subscribers)
	{
		subscriber->reset();
	}
	this->m_artifactsVector.clear();

	std::vector<IArtifact*> subscribersArtifacts;

	// Getting the new artifacts
	for (const RuleEntity& ruleEntity : rules)
	{
		auto artifact = ArtifactFactory::BuildArtifact(ruleEntity.getType(), ruleEntity.getName(), ruleEntity.getData());
		this->m_artifactsVector.push_back(std::move(artifact));
		subscribersArtifacts.push_back(artifact.get());
	}

	// Update the subscribers artifacts
	for (const auto subscriber : this->m_subscribers)
	{
		subscriber->setNewArtifacts(subscribersArtifacts);
	}
}

void ArtifactManager::subscribe(IArtifactSubscriber* subscriber)
{
	this->m_subscribers.push_back(subscriber);
}
