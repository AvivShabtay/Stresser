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
	if (nullptr == subscriber)
	{
		throw std::runtime_error("Received null pointer");
	}

	this->m_subscribers.push_back(subscriber);
}

void PolicyNotifications::changePolicyThreadFunction(const EndpointController& endpointController,
	const PolicyController& policyController, const RuleController& ruleController)
{
	DEBUG_WTRACE(PolicyNotifications, "Start fetching policy updates");

	// Check if there is CTRL + C signal:
	while (WAIT_TIMEOUT == WaitForSingleObject(m_shutdownEvent, 30 * 1000))
	{
		EndpointEntity endpointEntity = endpointController.getEndpoint(this->m_endpointId);
		DEBUG_TRACE(changePolicyThreadFunction, "Fetch policy data for endpoint ID=", endpointEntity.GetID());

		PolicyEntity policyEntity = policyController.getPolicy(endpointEntity.GetPolicyID());
		if (this->m_currentPolicy == policyEntity)
		{
			continue;
		}

		this->m_currentRules.clear();
		this->m_currentPolicy = policyEntity;

		DEBUG_TRACE(changePolicyThreadFunction, "New policy data, ID=", this->m_currentPolicy.getId(),
			", Version=", this->m_currentPolicy.getUpdateCount());

		this->m_changePolicyEvent.setEvent();
		this->m_changePolicyEvent.resetEvent();

		for (std::string& ruleId : this->m_currentPolicy.getRulesIds())
		{
			RuleEntity ruleEntity = ruleController.getRule(ruleId);
			this->m_currentRules.push_back(ruleEntity);

			DEBUG_TRACE(changePolicyThreadFunction, "Rule: ", ruleEntity.getName(), "ID=", ruleEntity.getId());
		}

		this->notifySubscribers();
	}

	DEBUG_WTRACE(PolicyNotifications, "Stop fetching policy updates");
}

void PolicyNotifications::notifySubscribers()
{
	for (IPolicySubscriber* subscriber : this->m_subscribers)
	{
		subscriber->policyChanged(this->m_currentRules);
	}
}
