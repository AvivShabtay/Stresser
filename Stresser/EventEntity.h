#pragma once
#include <iostream>

class EventEntity
{
public:
	EventEntity(std::string id, std::string name, std::string type, std::string data, std::string hostname,
		std::string ipAddress, std::string timestamp);
	~EventEntity();

	friend std::ostream& operator << (std::ostream& out, const EventEntity& eventEntity);

	const std::string GetID();
	const std::string GetName();
	const std::string GetType();
	const std::string GetData();
	const std::string GetHostname();
	const std::string GetIPAddress();
	const std::string GetTimestamp();
	void GetHostname(const std::string hostname);
	void GetIPAddress(const std::string ipAddress);
	void GetTimestamp(const std::string timestamp);

private:
	std::string m_id;
	std::string m_name;
	std::string m_type;
	std::string m_data;
	std::string m_hostname;
	std::string m_ipAddress;
	std::string m_timestamp;
};

