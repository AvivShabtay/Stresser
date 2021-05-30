#pragma once
#include <string>
#include "nlohmann/json.hpp"

using Json = nlohmann::json;

class EventEntity
{
public:
	EventEntity() = default;

	explicit EventEntity(std::string name, std::string type, std::string data, std::string timestamp);

	virtual ~EventEntity() = default;

	const std::string& getId() const;
	const std::string& getName() const;
	const std::string& getType() const;
	const std::string& getData() const;
	const std::string& getHostname() const;
	const std::string& getIpAddress() const;
	const std::string& getTimestamp() const;

	void setId(const std::string& mId);
	void setName(const std::string& mName);
	void setType(const std::string& mType);
	void setData(const std::string& mData);
	void setHostname(const std::string& mHostname);
	void setIpAddress(const std::string& mIpAddress);
	void setTimestamp(const std::string& mTimestamp);

	static Json ConvertFromEntity(const EventEntity& entity);

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

