#include "EndpointEntity.h"

EndpointEntity::EndpointEntity(std::string id, std::string hostname, std::string ipAddress, std::string apiKey,
	std::string status)
	: m_id(id), m_hostname(hostname), m_ipAddress(ipAddress), m_apiKey(apiKey), m_status(status) { }

EndpointEntity::~EndpointEntity() { }

const std::string EndpointEntity::GetID()
{
	return this->m_id;
}

const std::string EndpointEntity::GetHostname()
{
	return this->m_hostname;
}

const std::string EndpointEntity::GetIPAddress()
{
	return this->m_ipAddress;
}

const std::string EndpointEntity::GetAPIKey()
{
	return this->m_apiKey;
}

const std::string EndpointEntity::GetStatus()
{
	return this->m_status;
}

void EndpointEntity::SetHostname(const std::string hostname)
{
	this->m_hostname = hostname;
}

void EndpointEntity::SetIPAddress(const std::string ipAddress)
{
	this->m_ipAddress = ipAddress;
}

void EndpointEntity::GetStatus(const std::string status)
{
	this->m_status = status;
}

std::ostream& operator<<(std::ostream& out, const EndpointEntity& endpointEntity)
{
	return out
		<< "[Entity] ID: " << endpointEntity.m_id
		<< ", Hostname: " << endpointEntity.m_hostname
		<< ", IP Address: " << endpointEntity.m_ipAddress
		<< ", API Key: " << endpointEntity.m_apiKey
		<< ", Status: " << endpointEntity.m_status
		<< std::endl;
}
