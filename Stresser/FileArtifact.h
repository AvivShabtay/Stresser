#pragma once
#include "IArtifact.h"
#include "../Utils/AutoHandle.h"
#include "../Utils/Win32ErrorCodeException.h"

class FileArtifact : public IArtifact
{
public:
	explicit FileArtifact(std::string name, std::string data);
	virtual ~FileArtifact();

	virtual void install() const;
	virtual void uninstall() const;
};
