#include "IArtifactSubscriber.h"

#include <utility>

IArtifactSubscriber::IArtifactSubscriber(ArtifactTypes type) :
	m_type(std::move(type))
{
}

void IArtifactSubscriber::reset()
{
	this->m_artifactsToReport.clear();
}

void IArtifactSubscriber::setNewArtifacts(const std::vector<IArtifact*>& artifacts)
{
	this->m_artifactsToReport = artifacts;
}

ArtifactTypes IArtifactSubscriber::getType() const
{
	return this->m_type;
}
