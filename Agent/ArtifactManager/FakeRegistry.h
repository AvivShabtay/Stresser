#pragma once
#ifndef __FAKE_REGISTRY_H
#define __FAKE_REGISTRY_H

#include "FakeArtifact.h"

class FakeRegistry : public FakeArtifact
{
private:
	std::vector<std::wstring> registryPaths;
public:
	FakeRegistry(ArtifactType::Type type, ArtifactArgs args) : FakeArtifact(type, args) {}
	~FakeRegistry();
	virtual bool Install() override;
	virtual bool Uninstall() override;
};

#endif // !__FAKE_REGISTRY_H