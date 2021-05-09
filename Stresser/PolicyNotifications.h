#pragma once
#include "EndpointController.h"
#include "PolicyController.h"
#include "PolicyEntity.h"
#include "RuleController.h"
#include "../Utils/StandardThread.h"
#include "../Utils/WindowsEvent.h"

#include <string>
#include <vector>

#include "IPolicySubscriber.h"


class PolicyNotifications
{
public:
	explicit PolicyNotifications(const std::string& endpointId, const HANDLE& shutdownEvent, const EndpointController& endpointController,
		const PolicyController& policyController, const RuleController& ruleController);
	~PolicyNotifications() = default;

	void subscribe(IPolicySubscriber* subscriber);

private:
	void changePolicyThreadFunction(const EndpointController& endpointController,
		const PolicyController& policyController, const RuleController& ruleController);

	void notifySubscribers();
	
	std::vector<IPolicySubscriber*> m_subscribers;
	PolicyEntity m_currentPolicy;
	std::string m_endpointId;
	WindowsEvent m_changePolicyEvent;
	HANDLE m_shutdownEvent;
	StandardThread m_changePolicyThread;
	std::vector<RuleEntity> m_currentRules;
};

