#pragma once

#include <vector>
#include "FakeArtifact.h"

class ArtifactManager
{
public:
	ArtifactManager();
	~ArtifactManager();

	/* Add new fake artifact object. */
	bool AddArtifact(FakeArtifact* artifact);

	/* Retrieve the index of given fake artifact. */
	int GetArtifactIndex(FakeArtifact* artifact);

	/* Return the fake artifact object by it's index. */
	FakeArtifact* GetArtifactByIndex(int artifactIndex);

	/* Get index of artifact and removes it. */
	bool RemoveArtifactByIndex(int artifactIndex);

	/* Return the number of artifacts. */
	int Size();

	/* Install all the artifacts in local computer. */
	bool Install();

	/* Uninstall all the artifacts from local computer. */
	bool Uninstall();

private:
	std::vector<FakeArtifact*> m_artifacts;
};
