#pragma once

#ifndef __ARTIFACT_MANAGER_H
#define __ARTIFACT_MANAGER_H

#include <vector>
#include "FakeArtifact.h"

class ArtifactManager
{
private:
	std::vector<FakeArtifact*> artifacts;

public:
	ArtifactManager();
	~ArtifactManager();
	bool AddArtifact(FakeArtifact* artifact);
	int GetArtifactIndex(FakeArtifact* artifact);
	FakeArtifact* GetArtifactByIndex(int artifactIndex);
	bool RemoveArtifactByIndex(int artifactIndex);
	int Size();
};


#endif // !__ARTIFACT_MANAGER_H