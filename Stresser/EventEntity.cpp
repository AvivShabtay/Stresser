#include "EventEntity.h"

EventEntity::EventEntity(std::string id, std::string name, std::string type, std::string data,
	std::string hostname, std::string ipAddress, std::string timestamp)
	: m_id(id), m_name(name), m_type(type), m_data(data), m_hostname(hostname), m_ipAddress(ipAddress), m_timestamp(timestamp) { }

EventEntity::~EventEntity() { }

const std::string EventEntity::GetID()
{
	return this->m_id;
}

const std::string EventEntity::GetName()
{
	return this->m_name;
}

const std::string EventEntity::GetType()
{
	return this->m_type;
}

const std::string EventEntity::GetData()
{
	return this->m_data;
}

const std::string EventEntity::GetHostname()
{
	return this->m_hostname;
}

const std::string EventEntity::GetIPAddress()
{
	return this->m_ipAddress;
}

const std::string EventEntity::GetTimestamp()
{
	return this->m_timestamp;
}

void EventEntity::GetHostname(const std::string hostname)
{
	this->m_hostname = hostname;
}

void EventEntity::GetIPAddress(const std::string ipAddress)
{
	this->m_ipAddress = ipAddress;
}

void EventEntity::GetTimestamp(const std::string timestamp)
{
	this->m_timestamp = timestamp;
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
