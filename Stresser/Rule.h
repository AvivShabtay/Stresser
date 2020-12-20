#pragma once
#ifndef __RULE_H
#define __RULE_H

#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Rule
{
private:
	std::string ruleID;
	std::string ruleName;
	std::string ruleType;
	std::string ruleData;
public:
	Rule(std::string ruleID, std::string ruleName, std::string ruleType, std::string ruleData);
	friend std::ostream& operator << (std::ostream& out, const Rule& rule);
	static Rule ruleFromJson(json ruleJsonData);
};


#endif // !__RULE_H


