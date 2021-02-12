#pragma once
#include <iostream>
#include <vector>

class PolicyEntity
{
public:
	PolicyEntity(std::string id, std::string name, int numberOfRules, std::string rules, int updateCount);
	~PolicyEntity();

	friend std::ostream& operator << (std::ostream& out, const PolicyEntity& policyEntity);

	const std::string GetID();
	const std::string GetName();
	const int GetSize();
	const std::vector<int> GetRules();
	const int GetUpdateCount();

private:
	std::string m_id;
	std::string m_name;
	int m_numberOfRules;
	// TODO: Replace with RuleEntity object type:
	std::vector<int> m_rules;
	int m_updateCount;
};

