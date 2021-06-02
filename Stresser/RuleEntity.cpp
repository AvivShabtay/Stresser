#include "RuleEntity.h"

#include "../Utils/StringUtils.h"

RuleEntity::RuleEntity(std::string id, std::string name, std::string type, std::string data)
	: m_id(id), m_name(name), m_type(type), m_data(data) { }

const std::string& RuleEntity::getId() const
{
	return this->m_id;
}

const std::string& RuleEntity::getName() const
{
	return this->m_name;
}

const std::string& RuleEntity::getType() const
{
	return this->m_type;
}

const std::string& RuleEntity::getData() const
{
	return this->m_data;
}

RuleEntity RuleEntity::convertFromJson(Json jsRule)
{
	return RuleEntity(
		StringUtils::RemoveQuotationMarks(jsRule["id"].dump()),
		StringUtils::RemoveQuotationMarks(jsRule["ruleName"].dump()),
		StringUtils::RemoveQuotationMarks(jsRule["ruleType"].dump()),
		StringUtils::RemoveQuotationMarks(jsRule["ruleData"].dump())
	);
}

Json RuleEntity::convertFromEntity(const RuleEntity& ruleEntity)
{
	Json jsRule;
	jsRule["ruleId"] = ruleEntity.getId();
	jsRule["ruleName"] = ruleEntity.getName();
	jsRule["ruleType"] = ruleEntity.getType();
	jsRule["ruleData"] = ruleEntity.getData();

	return jsRule;
}

std::ostream& operator<<(std::ostream& out, const RuleEntity& ruleEntity)
{
	return out
		<< "ID: " << ruleEntity.m_id
		<< ", Name: " << ruleEntity.m_name
		<< ", Type: " << ruleEntity.m_type
		<< ", Data: " << ruleEntity.m_data;
}
