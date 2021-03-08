#pragma once

#include "nlohmann/json.hpp"

#include <iostream>

using Json = nlohmann::json;

/*
 * Represent Rule entity in the application server.
 */
class RuleEntity
{
public:
	RuleEntity();
	explicit RuleEntity(std::string id, std::string name, std::string type, std::string data);
	virtual ~RuleEntity() = default;

	std::string getID() const;
	std::string getName() const;
	std::string getType() const;
	std::string getData() const;

	static RuleEntity convertFromJson(Json jsRule);
	static Json convertFromEntity(const RuleEntity& ruleEntity);

	friend std::ostream& operator << (std::ostream& out, const RuleEntity& ruleEntity);

private:
	std::string m_id;
	std::string m_name;
	std::string m_type;
	std::string m_data;
};

