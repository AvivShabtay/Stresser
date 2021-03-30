#pragma once
#include <iostream>

class EventEntity
{
public:
	explicit EventEntity(std::string id, std::string name, std::string type, std::string data, std::string hostname,
		std::string ipAddress, std::string timestamp);

	virtual ~EventEntity() = default;

	const std::string& getId() const;
	const std::string& getName() const;
	const std::string& getType() const;
	const std::string& getData() const;
	const std::string& getHostname() const;
	const std::string& getIpAddress() const;
	const std::string& getTimestamp() const;

	friend std::ostream& operator << (std::ostream& out, const EventEntity& eventEntity);

private:
	std::string m_id;
	std::string m_name;
	std::string m_type;
	std::string m_data;
	std::string m_hostname;
	std::string m_ipAddress;
	std::string m_timestamp;
};

