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

	// Getting the new artifacts
	for (const RuleEntity& ruleEntity : rules)
	{
		auto artifact = ArtifactFactory::BuildArtifact(ruleEntity.getType(), ruleEntity.getName(), ruleEntity.getData());
		this->m_artifactsVector.push_back(std::move(artifact));
	}

	// Update the subscribers artifacts
	for (const auto subscriber : this->m_subscribers)
	{
		subscriber->setNewArtifacts(this->getArtifactsByType(subscriber->getType()));
	}
}

std::vector<IArtifact*> ArtifactManager::getArtifactsByType(const std::string& type)
{
	std::vector<IArtifact*> result;

	for (const auto& artifact :  this->m_artifactsVector)
	{
		if (artifact->getType() == type)
		{
			result.push_back(artifact.get());
		}
	}

	return result;
}

void ArtifactManager::subscribe(IArtifactSubscriber* subscriber)
{
	this->m_subscribers.push_back(subscriber);
}
