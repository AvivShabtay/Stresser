#include "RuleEntity.h"

RuleEntity::RuleEntity()
	: m_id(""), m_name(""), m_type(""), m_data("") { }

RuleEntity::RuleEntity(std::string id, std::string name, std::string type, std::string data)
	: m_id(id), m_name(name), m_type(type), m_data(data) { }

RuleEntity::~RuleEntity() { }

const std::string RuleEntity::GetID()
{
	return this->m_id;
}

const std::string RuleEntity::GetName()
{
	return this->m_name;
}

const std::string RuleEntity::GetType()
{
	return this->m_type;
}

const std::string RuleEntity::GetData()
{
	return this->m_data;
}

RuleEntity RuleEntity::ConvertFromJson(Json jsRule)
{
	return RuleEntity(
		jsRule["ruleId"].dump(),
		jsRule["ruleName"].dump(),
		jsRule["ruleType"].dump(),
		jsRule["ruleData"].dump()
	);
}

Json RuleEntity::ConvertFromEntity(RuleEntity ruleEntity)
{
	Json jsRule;
	jsRule["ruleId"] = ruleEntity.GetID();
	jsRule["ruleName"] = ruleEntity.GetName();
	jsRule["ruleType"] = ruleEntity.GetType();;
	jsRule["ruleData"] = ruleEntity.GetData();

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
