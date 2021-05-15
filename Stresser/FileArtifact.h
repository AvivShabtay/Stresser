#pragma once
#include "IArtifact.h"
#include "../Utils/AutoHandle.h"
#include "../Utils/Win32ErrorCodeException.h"

class FileArtifact : public IArtifact
{
public:
	explicit FileArtifact(const std::string& name, const std::string& data, const std::string type);
	virtual ~FileArtifact();

	void install() override;
	void uninstall() override;
};
