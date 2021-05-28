#include "RegistryEventHandler.h"

#include "EventParser.h"
#include "ArtifactTypes.h"
#include "../Utils/TimeUtils.h"
#include "../Utils/StringUtils.h"
#include "../Utils/RegistryArtifactUtils.h"
#include "../Utils/AutoCriticalSection.h"
#include "boost/algorithm/string.hpp"

RegistryEventHandler::RegistryEventHandler(std::vector<std::shared_ptr<IArtifact>>& artifacts)
	:IEtwEventHandler(EtwEventTypes::Registry), m_artifacts(artifacts)
{
}

std::optional<EventEntity> RegistryEventHandler::onEventRecord(PEVENT_RECORD record)
{
	if(m_artifacts.empty())
	{
		return std::nullopt;
	}

	const EventParser parser(record);

	const std::wstring timestamp = TimeUtils::systemTimeToTimestamp(parser.getEventHeader().TimeStamp);
	const std::uint32_t processPid = parser.getProcessId();
	const std::wstring eventData = L"Time= " + timestamp + L", PID= " + std::to_wstring(processPid);

	const UCHAR eventOpcode = parser.getEventHeader().EventDescriptor.Opcode;

	if (EVENT_TRACE_TYPE_REGOPEN != eventOpcode)
	{
		return std::nullopt;
	}

	const std::string eventType = ArtifactNames[static_cast<size_t>(ArtifactTypes::Registry)];
	const std::wstring wideEventType = StringUtils::stringToWString(eventType);
	const EventProperty* keyNameProperty = parser.getProperty(L"KeyName");
	if (nullptr == keyNameProperty)
	{
		return std::nullopt;
	}

	const std::wstring keyName(keyNameProperty->getUnicodeString());

	{
		AutoCriticalSection autoCriticalSection;

		for (const auto& artifact : this->m_artifacts)
		{
			if (artifact->getType() == ArtifactTypes::Registry)
			{
				std::string registrySubKey = RegistryArtifactUtils::getRegistrySubKey(artifact->getData());
				std::wstring artifactKey = StringUtils::stringToWString(registrySubKey);
				std::wstring trimmedArtifactKey = StringUtils::trimBackslash(artifactKey);
				if (boost::iequals(keyName, artifactKey) || boost::iequals(keyName, trimmedArtifactKey))
				{
					const std::wstring wideTimestamp = TimeUtils::systemTimeToTimestamp(parser.getEventHeader().TimeStamp);
					const std::string timestamp = StringUtils::wstringToString(wideTimestamp);

					const std::string eventType = ArtifactNames[static_cast<size_t>(ArtifactTypes::Registry)];
					const std::uint32_t processPid = parser.getProcessId();

					const std::wstring eventData = L"PID= " + std::to_wstring(processPid) + L" Key= " + keyName;
					const std::string narrowEventData = StringUtils::wstringToString(eventData);

					std::cout << "Event Found!" << std::endl;

					return EventEntity("Registry Artifact touched", eventType, narrowEventData, timestamp);
				}
			}
		}
	}

	return std::nullopt;
}
