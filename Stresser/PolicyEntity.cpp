#include "PolicyEntity.h"
#include "../Utils/StringUtils.h"

PolicyEntity::PolicyEntity()
	: m_id(""), m_name(""), m_numberOfRules(0), m_rules(std::vector<std::string>()), m_updateCount(0)
{
}

PolicyEntity::PolicyEntity(std::string id, std::string name, int numberOfRules, std::vector<std::string> rules, int updateCount)
	: m_id(id), m_name(name), m_numberOfRules(numberOfRules), m_rules(rules), m_updateCount(updateCount) { }

std::string PolicyEntity::getId() const
{
	return this->m_id;
}

std::string PolicyEntity::getName() const
{
	return this->m_name;
}

int PolicyEntity::getSize() const
{
	return this->m_numberOfRules;
}

std::vector<std::string> PolicyEntity::getRulesIds() const
{
	return this->m_rules;
}

int PolicyEntity::getUpdateCount() const
{
	return this->m_updateCount;
}

PolicyEntity PolicyEntity::convertFromJson(Json jsonPolicy)
{
	const std::string policyID = StringUtils::RemoveQuotationMarks(jsonPolicy["id"].dump());
	const std::string policyName = StringUtils::RemoveQuotationMarks(jsonPolicy["policyName"].dump());
	const int numberOfRules = stoi(StringUtils::RemoveQuotationMarks(jsonPolicy["numberOfRules"].dump()));
	const int updateCount = stoi(StringUtils::RemoveQuotationMarks(jsonPolicy["updateCount"].dump()));

	std::vector<std::string> rules;
	rules.reserve(numberOfRules);

	Json jsRules = jsonPolicy["rules"];
	for (const auto& jsRuleId : jsRules)
		rules.push_back(StringUtils::RemoveQuotationMarks(jsRuleId.dump()));

	return PolicyEntity(policyID, policyName, numberOfRules, rules, updateCount);
}

Json PolicyEntity::convertFromEntity(const PolicyEntity& policyEntity)
{
	Json jsPolicy;
	jsPolicy["id"] = policyEntity.getId();
	jsPolicy["policyName"] = policyEntity.getName();
	jsPolicy["numberOfRules"] = std::string("" + policyEntity.getSize());

	Json jsRules = Json::array();
	auto rules = policyEntity.getRulesIds();

	for (const auto& jsRule : rules)
	{
		jsRules.push_back(jsRule);
	}

	jsPolicy["rules"] = jsRules;
	jsPolicy["updateCount"] = std::string("" + policyEntity.getUpdateCount());

	return jsPolicy;
}

std::ostream& operator<<(std::ostream& out, const PolicyEntity& policyEntity)
{
	out
		<< "[Policy] ID: " << policyEntity.m_id
		<< ", Name: " << policyEntity.m_name
		<< ", Update Count: " << policyEntity.m_updateCount
		<< ", Number of rules: " << policyEntity.m_numberOfRules
		<< std::endl;
	out << "Rules:" << std::endl;

	for (const auto& rule : policyEntity.m_rules)
	{
		out << rule;
	}

	return out;
}

bool operator==(const PolicyEntity& lPolicy, const PolicyEntity& rPolicy)
{
	if (lPolicy.m_id != rPolicy.m_id)
	{
		return false;
	}
	if (lPolicy.m_updateCount != rPolicy.m_updateCount)
	{
		return false;
	}

	return true;
}
