#pragma once
#include <string>

class IArtifact 
{
public:
	explicit IArtifact(std::string name, std::string data);
	virtual ~IArtifact() = default;

	std::string getData();
	std::string getName();

	virtual void install() const = 0;
	virtual void uninstall() const = 0;

protected:
	std::string data;
	std::string name;
};