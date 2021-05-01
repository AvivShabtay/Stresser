#pragma once
#include "IArtifact.h"
#include "../Utils/AutoRegistryKeyHandle.h"
#include "../Utils/RegistryArtifactUtils.h"

class RegistryArtifact : public IArtifact
{
public:
	explicit RegistryArtifact(const std::string& name, const std::string& data);
	virtual ~RegistryArtifact();

	virtual void install();
	virtual void uninstall();

private:
	HKEY registryPrefix;
	std::string registrySubKey;
};
