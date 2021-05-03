#include "ArtifactManager.h"

ArtifactManager::ArtifactManager(const std::string& endpointId, const HANDLE& shutdownEvent, const EndpointController& endpointController,
		const PolicyController& policyController, const RuleController& ruleController) :
endpointId(endpointId), m_changePolicyEvent(CHANGE_POLICY), m_shutdownEvent(shutdownEvent)
{
	this->changePolicyThread = StandardThread([this, &endpointController, &policyController, &ruleController]()
	{
		this->changePolicyThreadFunction(endpointController, policyController, ruleController);
	});
}

void ArtifactManager::changePolicyThreadFunction(const EndpointController& endpointController,
	const PolicyController& policyController, const RuleController& ruleController)
{
	// Check if there is CTRL + C signal:
	while (WAIT_TIMEOUT == WaitForSingleObject(m_shutdownEvent, 10000))
	{
		EndpointEntity endpointEntity = endpointController.getEndpoint(this->endpointId);
		DEBUG_PRINT(endpointEntity);
		
		PolicyEntity policyEntity = policyController.getPolicy(endpointEntity.GetPolicyID());
		if (this->m_currentPolicy == policyEntity)
		{
			continue;
		}

		this->m_currentPolicy = policyEntity;
		DEBUG_PRINT("Policy ID: " + this->m_currentPolicy.getId());

		this->m_changePolicyEvent.setEvent();
		this->m_artifactsVector.clear();
		this->m_changePolicyEvent.resetEvent();

		for (std::string& ruleId : this->m_currentPolicy.getRulesIds())
		{
			RuleEntity ruleEntity = ruleController.getRule(ruleId);

			auto artifact = ArtifactFactory::BuildArtifact(ruleEntity.getType(), ruleEntity.getName(), ruleEntity.getData());
			this->m_artifactsVector.push_back(std::move(artifact));

			DEBUG_PRINT(ruleEntity);
		}
	}
}
