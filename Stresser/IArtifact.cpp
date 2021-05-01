#include "IArtifact.h"

IArtifact::IArtifact(std::string name, std::string data) : m_data(std::move(data)), m_name(std::move(name))
{
}

std::string IArtifact::getData() const
{
	return this->m_data;
}

std::string IArtifact::getName() const
{
	return this->m_name;
}

