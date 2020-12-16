#pragma once
#ifndef __FAKE_REGISTRY_H
#define __FAKE_REGISTRY_H

#include "FakeArtifact.h"

class FakeRegistry : public FakeArtifact
{
public:
	FakeRegistry(ArtifactType type, ArtifactArgs args) : FakeArtifact(type, args) {}
	virtual bool Install() override;
	virtual bool Uninstall() override;
};

#endif // !__FAKE_REGISTRY_H