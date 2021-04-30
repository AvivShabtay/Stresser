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

	virtual void install() const;
	virtual void uninstall() const;

private:
	std::wstring filePath;
};

