#pragma once
#include <functional>

#include "FileArtifact.h"
#include "ProcessArtifact.h"
#include "RegistryArtifact.h"

class ArtifactFactory 
{
public:
	static std::unique_ptr<IArtifact> BuildArtifact(const std::string& ruleType, const std::string& ruleName, const std::string& data);
};
