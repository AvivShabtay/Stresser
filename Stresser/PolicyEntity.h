#pragma once
#include <iostream>
#include <vector>

#include "RuleEntity.h"
#include "nlohmann/json.hpp"

using Json = nlohmann::json;

class PolicyEntity
{
public:
	PolicyEntity();
	PolicyEntity(std::string id, std::string name, int numberOfRules, std::vector<RuleEntity> rules, int updateCount);
	~PolicyEntity();

	const std::string GetID();
	const std::string GetName();
	const int GetSize();
	const std::vector<RuleEntity> GetRules();
	const int GetUpdateCount();

	static PolicyEntity ConvertFromJson(Json jsonPolicy);
	static Json ConvertFromEntity(PolicyEntity policyEntity);

	friend std::ostream& operator << (std::ostream& out, const PolicyEntity& policyEntity);

private:
	std::string m_id;
	std::string m_name;
	int m_numberOfRules;
	std::vector<RuleEntity> m_rules;
	int m_updateCount;
};

