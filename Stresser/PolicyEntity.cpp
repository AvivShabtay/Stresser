#include "PolicyEntity.h"

PolicyEntity::PolicyEntity()
	: m_id(""), m_name(""), m_numberOfRules(0), m_rules(std::vector<RuleEntity>()), m_updateCount(0)
{
}

PolicyEntity::PolicyEntity(std::string id, std::string name, int numberOfRules, std::vector<RuleEntity> rules, int updateCount)
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

std::vector<RuleEntity> PolicyEntity::getRules() const
{
	return this->m_rules;
}

int PolicyEntity::getUpdateCount() const
{
	return this->m_updateCount;
}

PolicyEntity PolicyEntity::convertFromJson(Json jsonPolicy)
{
	const std::string policyID = jsonPolicy["policyId"].dump();
	const std::string policyName = jsonPolicy["policyName"].dump();
	const int numberOfRules = stoi(jsonPolicy["numberOfRules"].dump());
	const int updateCount = stoi(jsonPolicy["updateCount"].dump());

	std::vector<RuleEntity> rules(numberOfRules);

	auto jsRules = jsonPolicy["rules"].array();
	for (int i = 0; i < numberOfRules; i++)
	{
		RuleEntity rule = RuleEntity::convertFromJson(jsRules[i]);
		rules.push_back(rule);
	}

	return PolicyEntity(policyID, policyName, numberOfRules, rules, updateCount);
}

Json PolicyEntity::convertFromEntity(const PolicyEntity& policyEntity)
{
	Json jsPolicy;
	jsPolicy["policyId"] = policyEntity.getId();
	jsPolicy["policyName"] = policyEntity.getName();
	jsPolicy["numberOfRules"] = std::string("" + policyEntity.getSize());

	Json jsRules = Json::array();
	auto rules = policyEntity.getRules();

	for (int i = 0; i < policyEntity.getSize(); i++)
	{
		auto jsRule = RuleEntity::convertFromEntity(rules.at(i));
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

	for (auto rule : policyEntity.m_rules)
		out << rule;

	return out;
}
