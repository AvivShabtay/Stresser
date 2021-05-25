#include "FileEventHandler.h"

#include "EtwUtils.h"
#include "EventParser.h"
#include "ArtifactTypes.h"
#include "../Utils/TimeUtils.h"
#include "../Utils/StringUtils.h"
#include "../Utils/LocalPcUtils.h"
#include "../Utils/AutoCriticalSection.h"
#include "boost/algorithm/string.hpp"

FileEventHandler::FileEventHandler(std::vector<std::shared_ptr<IArtifact>>& artifacts)
	: IEtwEventHandler(EtwEventTypes::FileIoInit), m_artifacts(artifacts)
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

	if (FILE_CREATE_OPCODE != eventOpcode)
	{
		return;
	}

	std::string eventType = ArtifactNames[static_cast<size_t>(ArtifactTypes::File)];
	std::wstring wideEventType = StringUtils::stringToWString(eventType);
	const EventProperty* openPathProperty = parser.getProperty(L"OpenPath");
	if (nullptr == openPathProperty)
	{
		return;
	}

	const std::wstring openPath(openPathProperty->getUnicodeString());
	std::wstring filePath;

	try
	{
		filePath = LocalPcUtils::getDosNameFromNtName(openPath);
	}
	catch (...)
	{
		std::wcout << "Cannot convert the ntPath to dosPath, ntPath: " << openPath << std::endl;
		return;
	}

	{
		AutoCriticalSection autoCriticalSection;

		for (const auto& artifact : this->m_artifacts)
		{
			if (artifact->getType() == ArtifactTypes::File)
			{
				const std::wstring artifactFilePath = StringUtils::stringToWString(artifact->getData());
				const std::wstring trimmedArtifactFilePath = StringUtils::trimBackslash(artifactFilePath);
				if (boost::iequals(filePath, artifactFilePath) || boost::iequals(filePath, trimmedArtifactFilePath))
				{
					std::wcout << eventData << ", " << wideEventType << ", Open path= " << openPath << std::endl;
				}
			}
		}
	}
}
