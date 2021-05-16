#pragma once
#include <vector>

#include "IArtifact.h"

class IArtifactSubscriber
{
public:
	explicit IArtifactSubscriber(std::string type);

	virtual ~IArtifactSubscriber() = default;

	void reset();

	void setNewArtifacts(const std::vector<IArtifact*>& artifacts);

	std::string getType() const;

protected:
	std::vector<IArtifact*> m_artifactsToReport;
	std::string m_type;
};
