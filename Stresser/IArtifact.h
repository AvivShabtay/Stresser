#pragma once
#include "ArtifactTypes.h"
#include <string>

class IArtifact
{
public:
	explicit IArtifact(std::string name, std::string data);
	virtual ~IArtifact() = default;

	std::string getData() const;
	std::string getName() const;
	virtual ArtifactTypes getType() const = 0;

	virtual void install() = 0;
	virtual void uninstall() = 0;

protected:
	std::string m_data;
	std::string m_name;
};
