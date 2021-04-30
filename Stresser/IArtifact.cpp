#include "IArtifact.h"

IArtifact::IArtifact(std::string& name, std::string& data) : data(data), name(name)
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

