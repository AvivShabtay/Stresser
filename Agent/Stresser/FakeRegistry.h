#pragma once
#ifndef __FAKE_REGISTRY_H
#define __FAKE_REGISTRY_H

#include "FakeArtifact.h"

class FakeRegistry : public FakeArtifact
{
public:
	bool Install();
	bool Uninstall();
};

#endif // !__FAKE_REGISTRY_H