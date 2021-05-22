#include "IArtifactSubscriber.h"

void IArtifactSubscriber::reset()
{
	this->m_artifactsToReport.clear();
}

void IArtifactSubscriber::setNewArtifacts(const std::vector<IArtifact*>& artifacts)
{
	this->m_artifactsToReport = artifacts;
}
