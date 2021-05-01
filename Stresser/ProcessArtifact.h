#pragma once
#include "IArtifact.h"
#include "resource.h"
#include "../Utils/PeResource.h"
#include "../Utils/LocalPcUtils.h"

#include <atlconv.h>


class ProcessArtifact : public IArtifact
{
public:
	explicit ProcessArtifact(const std::string& name, const std::string& data);
	virtual ~ProcessArtifact();

	virtual void install();
	virtual void uninstall();

private:
	std::wstring filePath;
};

