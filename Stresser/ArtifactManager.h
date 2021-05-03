#pragma once
#include "AuthorizedHttpRequest.h"
#include "IArtifact.h"
#include "ArtifactFactory.h"
#include "EndpointController.h"
#include "PolicyController.h"
#include "PolicyEntity.h"
#include "RuleController.h"
#include "../Utils/StandardThread.h"
#include "../Utils/WindowsEvent.h"
#include "../Utils/EventsNames.h"

class ArtifactManager
{
public:
	ArtifactManager(const std::string& endpointId, const HANDLE& shutdownEvent, const EndpointController& endpointController, 
		const PolicyController& policyController, const RuleController& ruleController);
	~ArtifactManager() = default;

	

private:
	void changePolicyThreadFunction(const EndpointController& endpointController, 
		const PolicyController& policyController, const RuleController& ruleController);

	std::string endpointId;
	std::vector<std::unique_ptr<IArtifact>> m_artifactsVector;
	WindowsEvent m_changePolicyEvent;
	HANDLE m_shutdownEvent;
	PolicyEntity m_currentPolicy;
	StandardThread changePolicyThread;
};

