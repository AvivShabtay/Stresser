#pragma once
#include <functional>

#include "FileArtifact.h"
#include "ProcessArtifact.h"
#include "RegistryArtifact.h"

class ArtifactFactory 
{
public:
	static std::unique_ptr<IArtifact> BuildArtifact(std::string ruleType, std::string ruleName, std::string data);
};
