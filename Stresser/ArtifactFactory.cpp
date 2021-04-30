#include "ArtifactFactory.h"

std::unique_ptr<IArtifact> ArtifactFactory::BuildArtifact(const std::string& ruleType, const std::string& ruleName, const std::string& data)
{
	if (ruleType == "Registry")
	{
		return std::make_unique<RegistryArtifact>(ruleName, data);
	}
	if (ruleType == "File")
	{
		return std::make_unique<FileArtifact>(ruleName, data);
	}
	if (ruleType == "Process")
	{
		return std::make_unique<ProcessArtifact>(ruleName, data);
	}

	throw std::runtime_error("Unknown rule type!");
}
