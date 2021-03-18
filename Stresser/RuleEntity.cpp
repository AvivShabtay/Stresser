#include "RuleEntity.h"

RuleEntity::RuleEntity()
	: m_id(""), m_name(""), m_type(""), m_data("") { }

RuleEntity::RuleEntity(std::string id, std::string name, std::string type, std::string data)
	: m_id(id), m_name(name), m_type(type), m_data(data) { }

std::string RuleEntity::getID() const
{
	return this->m_id;
}

std::string RuleEntity::getName() const
{
	return this->m_name;
}

std::string RuleEntity::getType() const
{
	return this->m_type;
}

std::string RuleEntity::getData() const
{
	return this->m_data;
}

RuleEntity RuleEntity::convertFromJson(Json jsRule)
{
	return RuleEntity(
		jsRule["ruleId"].dump(),
		jsRule["ruleName"].dump(),
		jsRule["ruleType"].dump(),
		jsRule["ruleData"].dump()
	);
}

Json RuleEntity::convertFromEntity(const RuleEntity& ruleEntity)
{
	Json jsRule;
	jsRule["ruleId"] = ruleEntity.getID();
	jsRule["ruleName"] = ruleEntity.getName();
	jsRule["ruleType"] = ruleEntity.getType();;
	jsRule["ruleData"] = ruleEntity.getData();

	return jsRule;
}

std::ostream& operator<<(std::ostream& out, const RuleEntity& ruleEntity)
{
	return out
		<< "[Rule] ID: " << ruleEntity.m_id
		<< ", Name: " << ruleEntity.m_name
		<< ", Type: " << ruleEntity.m_type
		<< ", Data: " << ruleEntity.m_data
		<< std::endl;
}
