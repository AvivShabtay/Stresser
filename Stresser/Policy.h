#pragma once

#ifndef __POLICY_H
#define __POLICY_H

#include <iostream>
#include "nlohmann/json.hpp"
#include <vector>

#include "Rule.h"

using json = nlohmann::json;

class Policy
{
private:
	std::string policyID;
	std::string policyName;
	int numOfRules;
	std::vector<Rule> rules;

public:
	Policy(json policyData);
	friend std::ostream& operator << (std::ostream& out, const Policy& policy);
};


#endif // !__POLICY_H


