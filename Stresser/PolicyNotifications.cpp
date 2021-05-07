#include "PolicyNotifications.h"
#include "../Utils/DebugPrint.h"
#include "../Utils/EventsNames.h"

PolicyNotifications::PolicyNotifications(const std::string& endpointId, const HANDLE& shutdownEvent,
	const EndpointController& endpointController, const PolicyController& policyController, const RuleController& ruleController) :
m_endpointId(endpointId), m_changePolicyEvent(CHANGE_POLICY), m_shutdownEvent(shutdownEvent)
{
	this->m_changePolicyThread = StandardThread([this, &endpointController, &policyController, &ruleController]()
		{
			this->changePolicyThreadFunction(endpointController, policyController, ruleController);
		});
}

void PolicyNotifications::subscribe(IPolicySubscriber* subscriber)
{
	this->m_subscribers.push_back(subscriber);
}

void PolicyNotifications::changePolicyThreadFunction(const EndpointController& endpointController,
                                                     const PolicyController& policyController, const RuleController& ruleController)
{
	// Check if there is CTRL + C signal:
	while (WAIT_TIMEOUT == WaitForSingleObject(m_shutdownEvent, 10000))
	{
		EndpointEntity endpointEntity = endpointController.getEndpoint(this->m_endpointId);
		DEBUG_PRINT(endpointEntity);

		PolicyEntity policyEntity = policyController.getPolicy(endpointEntity.GetPolicyID());
		if (this->m_currentPolicy == policyEntity)
		{
			continue;
		}

		this->m_currentRules.clear();
		this->m_currentPolicy = policyEntity;
		DEBUG_PRINT("Policy ID: " + this->m_currentPolicy.getId());

		this->m_changePolicyEvent.setEvent();
		this->m_changePolicyEvent.resetEvent();
		
		for (std::string& ruleId : this->m_currentPolicy.getRulesIds())
		{
			RuleEntity ruleEntity = ruleController.getRule(ruleId);
			this->m_currentRules.push_back(ruleEntity);

			DEBUG_PRINT(ruleEntity);
		}

		this->notifySubscribers();
	}
}

void PolicyNotifications::notifySubscribers()
{
	for (IPolicySubscriber* subscriber : this->m_subscribers)
	{
		subscriber->policyChanged(this->m_currentRules);
	}
}
