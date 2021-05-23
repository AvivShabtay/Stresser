#include "RegistryEventHandler.h"

#include "EventParser.h"
#include "../Utils/TimeUtils.h"
#include "../Utils/StringUtils.h"
#include "../Utils/RegistryArtifactUtils.h"
#include "EventEntity.h"

RegistryEventHandler::RegistryEventHandler(std::vector<IArtifact*>& artifacts) :
IEtwEventHandler(EtwEventTypes::Registry), m_artifacts(artifacts)
{
}

void RegistryEventHandler::onEventRecord(PEVENT_RECORD record)
{
	if(m_artifacts.empty())
	{
		return;
	}

	const EventParser parser(record);

	const std::wstring timestamp = TimeUtils::systemTimeToTimestamp(parser.getEventHeader().TimeStamp);
	const std::uint32_t processPid = parser.getProcessId();
	const std::wstring eventData = L"Time= " + timestamp + L", PID= " + std::to_wstring(processPid);

	const UCHAR eventOpcode = parser.getEventHeader().EventDescriptor.Opcode;

	if (EVENT_TRACE_TYPE_REGOPEN != eventOpcode)
	{
		return;
	}

	const std::wstring eventType = L"Open key event";
	const EventProperty* keyNameProperty = parser.getProperty(L"KeyName");
	if (nullptr == keyNameProperty)
	{
		return;
	}

	const std::wstring keyName(keyNameProperty->getUnicodeString());
	for (const auto& artifact : this->m_artifacts)
	{
		std::string registrySubKey = RegistryArtifactUtils::getRegistrySubKey(artifact->getData());
		std::wstring artifactKey = StringUtils::stringToWString(registrySubKey);
		std::wstring trimmedArtifactKey = StringUtils::trimBackslash(artifactKey);
		if (keyName == artifactKey || keyName == trimmedArtifactKey)
		{
			std::wcout << eventData << ", " << eventType << ", Open path= " << keyName << std::endl;
		}
	}
}
