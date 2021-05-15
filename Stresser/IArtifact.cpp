#include "IArtifact.h"

IArtifact::IArtifact(std::string name, std::string data, std::string type) :
m_data(std::move(data)), m_name(std::move(name)), m_type(std::move(type))
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

std::string IArtifact::getType() const
{
	return this->m_type;
}
