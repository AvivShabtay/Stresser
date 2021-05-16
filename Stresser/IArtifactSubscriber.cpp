#include "IArtifactSubscriber.h"

#include <utility>

IArtifactSubscriber::IArtifactSubscriber(std::string type) : m_type(std::move(type))
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

std::string IArtifactSubscriber::getType() const
{
	return this->m_type;
}
