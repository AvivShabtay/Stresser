#pragma once

#include "FakeArtifact.h"

class FakeRegistry : public FakeArtifact
{
public:
	FakeRegistry(ArtifactType::Type type, ArtifactArgs args) : FakeArtifact(type, args) {}
	virtual ~FakeRegistry();

	/* Creates the fake registry in the local computer. */
	virtual bool Install() override;

	/* Removes the fake registry from the local computer. */
	virtual bool Uninstall() override;

private:
	std::vector<std::wstring> m_registryPaths;
};