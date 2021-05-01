#pragma once
#include <string>

class IArtifact 
{
public:
	explicit IArtifact(const std::string& name, const std::string& data);
	virtual ~IArtifact() = default;

	std::string getData() const;
	std::string getName() const;

	virtual void install() = 0;
	virtual void uninstall() = 0;

protected:
	std::string data;
	std::string name;
};