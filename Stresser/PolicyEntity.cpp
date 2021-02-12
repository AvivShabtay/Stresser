#include "PolicyEntity.h"

PolicyEntity::PolicyEntity(std::string id, std::string name, int numberOfRules, std::string rules, int updateCount)
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

const std::vector<int> PolicyEntity::GetRules()
{
	return this->m_rules;
}

const int PolicyEntity::GetUpdateCount()
{
	return this->m_updateCount;
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
