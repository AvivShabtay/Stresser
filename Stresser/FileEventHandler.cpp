#include "FileEventHandler.h"

#include <iostream>
#include <regex>

#include "EtwUtils.h"
#include "EventParser.h"
#include "../Utils/TimeUtils.h"
#include "../Utils/StringUtils.h"
#include "../Utils/LocalPcUtils.h"

FileEventHandler::FileEventHandler(std::vector<IArtifact*>& artifacts) : IEtwEventHandler(EtwEventTypes::FileIoInit), m_artifacts(artifacts)
{
}

void FileEventHandler::onEventRecord(PEVENT_RECORD record)
{
	if (m_artifacts.empty())
	{
		return;
	}

	const EventParser parser(record);

	const std::wstring timestamp = TimeUtils::systemTimeToTimestamp(parser.getEventHeader().TimeStamp);
	const std::uint32_t processPid = parser.getProcessId();
	const std::wstring eventData = L"Time= " + timestamp + L", PID= " + std::to_wstring(processPid);

	const UCHAR eventOpcode = parser.getEventHeader().EventDescriptor.Opcode;

	if (64 != eventOpcode)
	{
		return;
	}

	std::wstring eventType = L"FileIo_Create";
	const EventProperty* openPathProperty = parser.getProperty(L"OpenPath");
	if (nullptr == openPathProperty)
	{
		return;
	}

	const std::wstring openPath(openPathProperty->getUnicodeString());
	const std::wstring filePath = LocalPcUtils::getDosNameFromNtName(openPath);
	for (const auto& artifact : this->m_artifacts)
	{
		const std::wstring artifactFilePath = StringUtils::stringToWString(artifact->getData());
		const std::wstring trimmedArtifactFilePath = StringUtils::trimBackslash(artifactFilePath);

		if (filePath == artifactFilePath || filePath == trimmedArtifactFilePath)
		{
			std::wcout << eventData << ", " << eventType << ", Open path= " << openPath << std::endl;
		}
	}
}
