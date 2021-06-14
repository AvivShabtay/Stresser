#include "EtwManager.h"

#include "../Utils/DebugPrint.h"

EtwManager::EtwManager()
{
	this->initializeGuids();
}

EtwManager::~EtwManager()
{
	try
	{
		this->stop();
	}
	catch (const std::exception& exception)
	{
		DEBUG_PRINT(exception.what());
	}
	catch (...)
	{
		DEBUG_PRINT("Exception was thrown in TraceManager destructor");
	}
}

void EtwManager::start()
{
	DEBUG_WTRACE(EtwManager, "Start ETW manager");

	this->m_autoTraceSession.reset(this->getProperties(), this->getTraceLogFile(), KERNEL_LOGGER_NAME);
}

void EtwManager::stop()
{
	DEBUG_WTRACE(EtwManager, "Stop ETW manager");

	this->m_autoTraceSession.release();
}

void EtwManager::onEventRecord(PEVENT_RECORD record)
{
	for (const auto& eventHandler : this->m_eventsHandlers)
	{
		if (m_etwEventsGuid.at(eventHandler->getType()) == record->EventHeader.ProviderId)
		{
			std::optional<EventEntity> eventEntity = eventHandler->onEventRecord(record);
			if (eventEntity)
			{
				this->m_callback(eventEntity.value());
			}
		}
	}
}

void EtwManager::setCallback(const std::function<void(EventEntity&)>& callback)
{
	m_callback = callback;
}

void EtwManager::registerEventHandle(std::shared_ptr<IEtwEventHandler> eventHandler)
{
	this->m_eventsHandlers.push_back(eventHandler);
}

EVENT_TRACE_PROPERTIES EtwManager::getProperties() const
{
	EVENT_TRACE_PROPERTIES properties = { 0 };

	properties.EnableFlags = this->getEventTypes();

	// Allocating memory for EVENT_TRACE_PROPERTIES + the log file name memory (KERNEL_LOGGER_NAME)
	// See MSDN about EVENT_TRACE_PROPERTIES, LogFileNameOffset filed for more info.
	const auto size = sizeof(EVENT_TRACE_PROPERTIES) + sizeof(KERNEL_LOGGER_NAME);
	properties.Wnode.BufferSize = static_cast<ULONG>(size);

	properties.Wnode.Guid = SystemTraceControlGuid;
	properties.Wnode.Flags = WNODE_FLAG_TRACED_GUID;
	properties.Wnode.ClientContext = 1;
	properties.LogFileMode = EVENT_TRACE_REAL_TIME_MODE;

	// Offset from the start of the allocated memory to the log file name:
	properties.LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);

	return properties;
}

EVENT_TRACE_LOGFILE EtwManager::getTraceLogFile()
{
	EVENT_TRACE_LOGFILE traceLogfile = { 0 };

	// On event, pass this class reference:
	traceLogfile.Context = this;

	traceLogfile.LoggerName = const_cast<LPWSTR>(LOGGER_NAME.c_str());
	traceLogfile.ProcessTraceMode = PROCESS_TRACE_MODE_EVENT_RECORD | PROCESS_TRACE_MODE_REAL_TIME;
	traceLogfile.EventRecordCallback = [](const PEVENT_RECORD record) {
		auto* const etwManager = static_cast<EtwManager*>(record->UserContext);
		etwManager->onEventRecord(record);
	};

	return traceLogfile;
}

ULONG EtwManager::getEventTypes() const
{
	ULONG eventTypes = 0;
	for (const auto& eventHandler : this->m_eventsHandlers)
	{
		eventTypes |= static_cast<ULONG>(eventHandler->getType());
	}

	return eventTypes;
}

void EtwManager::initializeGuids()
{
	this->m_etwEventsGuid =
	{
		{EtwEventTypes::DiskIo, DiskIoGuid},
		{EtwEventTypes::DiskFileIo, FileIoGuid},
		{EtwEventTypes::FileIo, FileIoGuid},
		{EtwEventTypes::FileIoInit, FileIoGuid},
		{EtwEventTypes::Registry, RegistryGuid},
		{EtwEventTypes::Process, ProcessGuid},
		{EtwEventTypes::ProcessCounters, ProcessGuid}
	};
}
