#pragma once
#include <iostream>

#include "nlohmann/json.hpp"

using Json = nlohmann::json;

class EndpointEntity
{
public:
	EndpointEntity();
	EndpointEntity(std::string id, std::string hostname, std::string ipAddress, std::string apiKey,
		std::string policyId, std::string status);
	~EndpointEntity();

	const std::string GetID();
	const std::string GetHostname();
	const std::string GetIPAddress();
	const std::string GetAPIKey();
	const std::string GetPolicyID();
	const std::string GetStatus();
	void SetHostname(const std::string hostname);
	void SetIPAddress(const std::string ipAddress);
	void GetStatus(const std::string status);

	static EndpointEntity ConvertFromJson(Json jsonEndpointEntity);
	static Json ConvertFromEntity(EndpointEntity entity);

	friend std::ostream& operator << (std::ostream& out, const EndpointEntity& endpointEntity);

private:
	std::string m_id;
	std::string m_hostname;
	std::string m_ipAddress;
	std::string m_apiKey;
	std::string m_policyId;
	std::string m_status;
};