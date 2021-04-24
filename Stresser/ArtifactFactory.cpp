#include "ArtifactFactory.h"

std::unique_ptr<IArtifact> ArtifactFactory::BuildArtifact(std::string ruleType, std::string ruleName, std::string data)
{
	if (ruleType == "Registry")
		return std::make_unique<RegistryArtifact>(ruleName, data);
	else if (ruleType == "File")
		return std::make_unique<FileArtifact>(ruleName, data);
	else if (ruleType == "Process")
		return std::make_unique<ProcessArtifact>(ruleName, data);

	throw std::exception("Unknown rule type!");
}
