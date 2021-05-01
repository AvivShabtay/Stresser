#pragma once
#include "IArtifact.h"
#include "../Utils/AutoRegistryKeyHandle.h"
#include "../Utils/RegistryArtifactUtils.h"

class RegistryArtifact : public IArtifact
{
public:
	explicit RegistryArtifact(const std::string& name, const std::string& data);
	virtual ~RegistryArtifact();

	void install() override;
	void uninstall() override;

private:
	HKEY m_registryPrefix;
	std::string m_registrySubKey;
};
