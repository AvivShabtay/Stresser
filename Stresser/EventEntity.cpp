#include "EventEntity.h"

#include <utility>

EventEntity::EventEntity(std::string name, std::string type, std::string data, std::string timestamp)
	: m_name(std::move(name)), m_type(std::move(type)), m_data(std::move(data)), m_timestamp(std::move(timestamp))
{
}

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

void EventEntity::setId(const std::string& mId)
{
	m_id = mId;
}

void EventEntity::setName(const std::string& mName)
{
	m_name = mName;
}

void EventEntity::setType(const std::string& mType)
{
	m_type = mType;
}

void EventEntity::setData(const std::string& mData)
{
	m_data = mData;
}

void EventEntity::setHostname(const std::string& mHostname)
{
	m_hostname = mHostname;
}

void EventEntity::setIpAddress(const std::string& mIpAddress)
{
	m_ipAddress = mIpAddress;
}

void EventEntity::setTimestamp(const std::string& mTimestamp)
{
	m_timestamp = mTimestamp;
}

Json EventEntity::ConvertFromEntity(const EventEntity& entity)
{
	Json jsEvent;

	jsEvent["id"] = entity.m_id;
	jsEvent["eventName"] = entity.m_name;
	jsEvent["eventType"] = entity.m_type;
	jsEvent["eventData"] = entity.m_data;
	jsEvent["hostname"] = entity.m_hostname;
	jsEvent["IPAddress"] = entity.m_ipAddress;
	jsEvent["timeStamp"] = entity.m_timestamp;

	return jsEvent;
}

std::ostream& operator<<(std::ostream& out, const EventEntity& eventEntity)
{
	return out
		<< "[Event] ID: " << eventEntity.m_id
		<< ", Name: " << eventEntity.m_name
		<< ", Type: " << eventEntity.m_type
		<< ", Hostname: " << eventEntity.m_hostname
		<< ", IP Address: " << eventEntity.m_ipAddress
		<< ", Timestamp: " << eventEntity.m_timestamp;
}
