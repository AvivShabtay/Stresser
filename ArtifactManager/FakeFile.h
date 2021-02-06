#pragma once

#include "FakeArtifact.h"

class FakeFile : public FakeArtifact
{
public:
	FakeFile(ArtifactType::Type type, ArtifactArgs args) : FakeArtifact(type, args) {}
	virtual ~FakeFile();

	/* Creates the fake file in the local computer. */
	virtual bool Install() override;

	/* Removes the fake file from the local computer. */
	virtual bool Uninstall() override;

private:
	std::vector<std::wstring> m_filePaths;
};