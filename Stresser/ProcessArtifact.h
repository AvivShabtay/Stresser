#pragma once
#include "IArtifact.h"
#include "resource.h"
#include "../Utils/PeResource.h"
#include "../Utils/ProcessArtifactUtils.h"


#include <Windows.h>

class ProcessArtifact : public IArtifact
{
public:
	explicit ProcessArtifact(std::string name, std::string data);
	virtual ~ProcessArtifact();

	virtual void install() const;
	virtual void uninstall() const;

private:
	std::wstring filePath;
};

