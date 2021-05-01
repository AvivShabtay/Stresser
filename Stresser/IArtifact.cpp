#include "IArtifact.h"

IArtifact::IArtifact(const std::string& name, const std::string& data) : data(data), name(name)
{
}

std::string IArtifact::getData() const
{
	return this->data;
}

std::string IArtifact::getName() const
{
	return this->name;
}

