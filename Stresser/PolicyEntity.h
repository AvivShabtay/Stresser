#pragma once

#include "RuleEntity.h"

#include "nlohmann/json.hpp"

#include <iostream>
#include <vector>

using Json = nlohmann::json;

/*
 * Represent Policy entity in the application server.
 */
class PolicyEntity
{
public:
	PolicyEntity();
	explicit PolicyEntity(std::string id, std::string name, int numberOfRules, std::vector<std::string> rules, int updateCount);

	virtual ~PolicyEntity() = default;

	std::string getId() const;
	std::string getName() const;
	int getSize() const;
	std::vector<std::string> getRulesIds() const;
	int getUpdateCount() const;

	static PolicyEntity convertFromJson(Json jsonPolicy);
	static Json convertFromEntity(const PolicyEntity& policyEntity);

	friend std::ostream& operator << (std::ostream& out, const PolicyEntity& policyEntity);
	bool operator==(const PolicyEntity& rPolicy) const;

private:
	std::string m_id;
	std::string m_name;
	int m_numberOfRules;
	std::vector<std::string> m_rules;
	int m_updateCount;
};

