#pragma once
#include <iostream>

class EndpointEntity
{
public:
	EndpointEntity(std::string id, std::string hostname, std::string ipAddress, std::string apiKey,
		std::string status);
	~EndpointEntity();

	friend std::ostream& operator << (std::ostream& out, const EndpointEntity& endpointEntity);

	const std::string GetID();
	const std::string GetHostname();
	const std::string GetIPAddress();
	const std::string GetAPIKey();
	const std::string GetStatus();
	void SetHostname(const std::string hostname);
	void SetIPAddress(const std::string ipAddress);
	void GetStatus(const std::string status);

private:
	std::string m_id;
	std::string m_hostname;
	std::string m_ipAddress;
	std::string m_apiKey;
	std::string m_status;
};