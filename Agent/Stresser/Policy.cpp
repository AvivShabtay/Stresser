#include "Policy.h"

Policy::Policy(json policyData) {
	this->policyID = policyData["policyID"];
	this->policyName = policyData["policyName"];
	this->numOfRules = policyData["numOfRules"];

	auto rules = policyData["rules"];
	for (auto rule : rules) {
		json ruleJson(rule);
		Rule r = Rule::ruleFromJson(ruleJson);
		this->rules.push_back(r);
	}
}

std::ostream& operator<<(std::ostream& out, const Policy& policy)
{
	out << "[Policy] ID: " << policy.policyID << ", Name: " << policy.policyName
		<< ", Number of rules: " << policy.numOfRules << std::endl;
	out << "Rules:" << std::endl;

	for (auto r : policy.rules)
		out << r;

	return out;
}
