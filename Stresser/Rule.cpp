#include "Rule.h"

std::ostream& operator<<(std::ostream& out, const Rule& rule)
{
	out << "[Rule] ID: " << rule.ruleID << ", Name: " << rule.ruleName
		<< ", Type: " << rule.ruleType << ", Data: " << rule.ruleData << std::endl;

	return out;
}

Rule Rule::ruleFromJson(json ruleJsonData)
{
	std::string ruleID = ruleJsonData["ruleID"];
	std::string ruleName = ruleJsonData["ruleName"];
	std::string ruleType = ruleJsonData["ruleType"];
	std::string ruleData = ruleJsonData["ruleData"];

	return { ruleID, ruleName, ruleType, ruleData };
}

Rule::Rule(std::string ruleID, std::string ruleName, std::string ruleType, std::string ruleData) {
	this->ruleID = ruleID;
	this->ruleName = ruleName;
	this->ruleType = ruleType;
	this->ruleData = ruleData;
}
