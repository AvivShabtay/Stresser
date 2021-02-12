#pragma once
#include <iostream>

class RuleEntity
{
	RuleEntity(std::string id, std::string name, std::string type, std::string data);
	~RuleEntity();

	friend std::ostream& operator << (std::ostream& out, const RuleEntity& ruleEntity);

	const std::string GetID();
	const std::string GetName();
	const std::string GetType();
	const std::string GetData();

private:
	std::string m_id;
	std::string m_name;
	std::string m_type;
	std::string m_data;
};

