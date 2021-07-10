#include "PolicyNotifications.h"
#include "ParsingJsonException.h"
#include "InvalidResponseException.h"

#include "../Utils/DebugPrint.h"
#include "../Utils/EventsNames.h"
#include "../Utils/LocalPcUtils.h"
#include "../Utils/SehTranslatorGuard.h"
#include "../Utils/AutoCriticalSection.h"

PolicyNotifications::PolicyNotifications(const std::string& endpointId, const HANDLE& shutdownEvent,
	const EndpointController& endpointController, const PolicyController& policyController, const RuleController& ruleController) :
	m_endpointId(endpointId), m_changePolicyEvent(CHANGE_POLICY), m_shutdownEvent(shutdownEvent),
	m_endpointController(endpointController), m_policyController(policyController), m_ruleController(ruleController)
{
	this->m_lostConnectionEvent.open(LOST_CONNECTION_EVENT_NAME);

	this->startFetchPolicyThread();
}

void PolicyNotifications::subscribe(IPolicySubscriber* subscriber)
{
	if (nullptr == subscriber)
	{
		throw std::runtime_error("Received null pointer");
	}

	this->m_subscribers.push_back(subscriber);
}

void PolicyNotifications::startFetchPolicyThread()
{
	this->m_changePolicyThread = StandardThread([this]()
		{
			this->changePolicyThreadFunction(this->m_endpointController, this->m_policyController, this->m_ruleController);
		});
}

void PolicyNotifications::resetEndpointId(std::string endpointId)
{
	AutoCriticalSection autoCriticalSection;
	this->m_endpointId = endpointId;
}

void PolicyNotifications::changePolicyThreadFunction(const EndpointController& endpointController,
	const PolicyController& policyController, const RuleController& ruleController)
{
	DEBUG_WTRACE(PolicyNotifications, "Start fetching policy updates");

	SehTranslatorGuard sehTranslatorGuard;

	constexpr int CLOSE_THREAD_EVENTS_COUNT = 2;
	HANDLE closeThreadEvents[CLOSE_THREAD_EVENTS_COUNT] =
	{
		this->m_shutdownEvent,
		this->m_lostConnectionEvent.get()
	};

	while (WAIT_TIMEOUT == WaitForMultipleObjects(CLOSE_THREAD_EVENTS_COUNT, closeThreadEvents, FALSE, 30 * 1000))
	{
		try
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
		catch (const InvalidResponseException& exception)
		{
			DEBUG_TRACE(PolicyNotifications, "Invalid response from server, Exception: ", exception.what());
		}
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
