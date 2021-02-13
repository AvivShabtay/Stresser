#include "PolicyEntity.h"

PolicyEntity::PolicyEntity()
	: m_id(""), m_name(""), m_numberOfRules(0), m_rules(std::vector<RuleEntity>()), m_updateCount(0) { }

PolicyEntity::PolicyEntity(std::string id, std::string name, int numberOfRules, std::vector<RuleEntity> rules, int updateCount)
	: m_id(id), m_name(name), m_numberOfRules(numberOfRules), m_rules(rules), m_updateCount(updateCount) { }

PolicyEntity::~PolicyEntity() { }

const std::string PolicyEntity::GetID()
{
	return this->m_id;
}

const std::string PolicyEntity::GetName()
{
	return this->m_name;
}

const int PolicyEntity::GetSize()
{
	return this->m_numberOfRules;
}

const std::vector<RuleEntity> PolicyEntity::GetRules()
{
	return this->m_rules;
}

const int PolicyEntity::GetUpdateCount()
{
	return this->m_updateCount;
}

PolicyEntity PolicyEntity::ConvertFromJson(Json jsonPolicy)
{
	auto policyID = jsonPolicy["policyId"].dump();
	auto policyName = jsonPolicy["policyName"].dump();
	auto numberOfRules = stoi(jsonPolicy["numberOfRules"].dump());
	auto updateCount = stoi(jsonPolicy["updateCount"].dump());

	std::vector<RuleEntity> rules(numberOfRules);

	auto jsRules = jsonPolicy["rules"].array();
	for (int i = 0; i < numberOfRules; i++)
	{
		RuleEntity rule = RuleEntity::ConvertFromJson(jsRules[i]);
		rules.push_back(rule);
	}

	return PolicyEntity(policyID, policyName, numberOfRules, rules, updateCount);
}

Json PolicyEntity::ConvertFromEntity(PolicyEntity policyEntity)
{
	Json jsPolicy;
	jsPolicy["policyId"] = policyEntity.GetID();
	jsPolicy["policyName"] = policyEntity.GetName();
	jsPolicy["numberOfRules"] = std::string("" + policyEntity.GetSize());

	Json jsRules = Json::array();
	auto rules = policyEntity.GetRules();

	for (int i = 0; i < policyEntity.GetSize(); i++)
	{
		auto jsRule = RuleEntity::ConvertFromEntity(rules.at(i));
		jsRules.push_back(jsRule);
	}

	jsPolicy["rules"] = jsRules;
	jsPolicy["updateCount"] = std::string("" + policyEntity.GetUpdateCount());

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
