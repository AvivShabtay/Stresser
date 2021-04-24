#include "IArtifact.h"

IArtifact::IArtifact(std::string name, std::string data) : data(data), name(name) 
{}

std::string IArtifact::getData()
{
	return this->data;
}

std::string IArtifact::getName()
{
	return this->name;
}

