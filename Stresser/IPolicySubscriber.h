#pragma once
#include "RuleEntity.h"

class IPolicySubscriber
{
public:
	virtual ~IPolicySubscriber() = default;
	virtual void policyChanged(const std::vector<RuleEntity>& rules) = 0;
};
