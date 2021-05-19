#pragma once
#include "ArtifactTypes.h"
#include "IArtifact.h"
#include <vector>

class IArtifactSubscriber
{
public:
	explicit IArtifactSubscriber(ArtifactTypes type);

	virtual ~IArtifactSubscriber() = default;

	void reset();

	void setNewArtifacts(const std::vector<IArtifact*>& artifacts);

	ArtifactTypes getType() const;

protected:
	std::vector<IArtifact*> m_artifactsToReport;
	ArtifactTypes m_type;
};
