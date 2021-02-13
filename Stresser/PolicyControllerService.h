#pragma once

#include <iostream>
#include "nlohmann/json.hpp"
#include <vector>

#include "IPolicyControllerService.h"
#include "../CommunicationManager/Connection.h"
#include "Rule.h"

using json = nlohmann::json;

class PolicyControllerService : public IPolicyControllerService
{
public:
	virtual PolicyEntity GetPolicy(std::string apiKey, std::string policyId);

private:
	PolicyControllerService();
};
