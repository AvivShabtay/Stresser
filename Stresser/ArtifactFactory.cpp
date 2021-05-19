#include "ArtifactFactory.h"
#include "ArtifactTypes.h"

std::unique_ptr<IArtifact> ArtifactFactory::BuildArtifact(const std::string& ruleType, const std::string& ruleName, const std::string& data)
{
	if (ruleType == ArtifactNames[static_cast<size_t>(ArtifactTypes::Registry)])
	{
		return std::make_unique<RegistryArtifact>(ruleName, data);
	}
	if (ruleType == ArtifactNames[static_cast<size_t>(ArtifactTypes::File)])
	{
		return std::make_unique<FileArtifact>(ruleName, data);
	}
	if (ruleType == ArtifactNames[static_cast<size_t>(ArtifactTypes::Process)])
	{
		return std::make_unique<ProcessArtifact>(ruleName, data);
	}

	throw std::runtime_error("Unknown rule type!");
}
