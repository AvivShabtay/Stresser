#include "EndpointEntity.h"

EndpointEntity::EndpointEntity()
	: m_id(""), m_hostname(""), m_ipAddress(""), m_apiKey(""), m_policyId(""), m_status("") { }

EndpointEntity::EndpointEntity(std::string id, std::string hostname, std::string ipAddress, std::string apiKey,
	std::string policyId, std::string status)
	: m_id(id), m_hostname(hostname), m_ipAddress(ipAddress), m_apiKey(apiKey), m_policyId(policyId), m_status(status) { }

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

const std::string EndpointEntity::GetPolicyID()
{
	return this->m_policyId;
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

EndpointEntity EndpointEntity::ConvertFromJson(Json jsEndpoint)
{
	return EndpointEntity(
		jsEndpoint["endpointId"].dump(),
		jsEndpoint["hostname"].dump(),
		jsEndpoint["IPAddress"].dump(),
		jsEndpoint["apiKey"].dump(),
		jsEndpoint["policyId"].dump(),
		jsEndpoint["status"].dump()
	);
}

Json EndpointEntity::ConvertFromEntity(EndpointEntity entity)
{
	Json jsEndpoint;
	jsEndpoint["endpointId"] = entity.GetID();
	jsEndpoint["hostname"] = entity.GetHostname();
	jsEndpoint["IPAddress"] = entity.GetIPAddress();;
	jsEndpoint["apiKey"] = entity.GetAPIKey();
	jsEndpoint["policyId"] = entity.GetPolicyID();
	jsEndpoint["status"] = entity.GetStatus();

	return jsEndpoint;
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
