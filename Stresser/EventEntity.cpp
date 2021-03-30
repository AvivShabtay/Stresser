#include "EventEntity.h"

EventEntity::EventEntity(std::string id, std::string name, std::string type, std::string data,
	std::string hostname, std::string ipAddress, std::string timestamp)
	: m_id(id), m_name(name), m_type(type), m_data(data), m_hostname(hostname), m_ipAddress(ipAddress), m_timestamp(timestamp) { }

const std::string& EventEntity::getId() const
{
	return this->m_id;
}

const std::string& EventEntity::getName() const
{
	return this->m_name;
}

const std::string& EventEntity::getType() const
{
	return this->m_type;
}

const std::string& EventEntity::getData() const
{
	return this->m_data;
}

const std::string& EventEntity::getHostname() const
{
	return this->m_hostname;
}

const std::string& EventEntity::getIpAddress() const
{
	return this->m_ipAddress;
}

const std::string& EventEntity::getTimestamp() const
{
	return this->m_timestamp;
}

std::ostream& operator<<(std::ostream& out, const EventEntity& eventEntity)
{
	return out
		<< "[Event] ID: " << eventEntity.m_id
		<< ", Name: " << eventEntity.m_name
		<< ", Type: " << eventEntity.m_type
		<< ", Hostname: " << eventEntity.m_hostname
		<< ", IP Address: " << eventEntity.m_ipAddress
		<< ", Timestamp: " << eventEntity.m_timestamp
		<< std::endl;
}
