#pragma once
#include <iostream>

#include "nlohmann/json.hpp"

using Json = nlohmann::json;

class RuleEntity
{
public:
	RuleEntity();
	RuleEntity(std::string id, std::string name, std::string type, std::string data);
	~RuleEntity();

	const std::string GetID();
	const std::string GetName();
	const std::string GetType();
	const std::string GetData();

	static RuleEntity ConvertFromJson(Json jsRule);
	static Json ConvertFromEntity(RuleEntity ruleEntity);

	friend std::ostream& operator << (std::ostream& out, const RuleEntity& ruleEntity);

private:
	std::string m_id;
	std::string m_name;
	std::string m_type;
	std::string m_data;
};

