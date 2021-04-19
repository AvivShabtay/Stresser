#pragma once

// Used for SystemTraceControlGuid:
#define INITGUID

#include "IEventCollector.h"
#include "IEtwEventHandler.h"
#include "AutoEtwTraceSession.h"
#include "EtwEventTypes.h"

#include <functional>
#include <vector>

#include <Windows.h>

// must be after Windows.h:
#include <evntrace.h>
#include <evntcons.h>

// Default logger name:
const std::wstring LOGGER_NAME(KERNEL_LOGGER_NAME);

/*
 * Manage the ETW trace session.
 * Define the default properties and the default logfile for the ETW session
 * and start the trace session, stop it, etc.
 */
class EtwManager : public IEtwEventHandler, IEventCollector
{
public:
	explicit EtwManager(std::vector<EtwEventTypes> eventTypes, std::function<void(PEVENT_RECORD)> callback);

	virtual ~EtwManager();

	/* Start the start the trace session. */
	void start() override;

	/* Stop the start the trace session. */
	void stop() override;

	/* Called whenever new event accepted by the trace session. */
	void onEventRecord(PEVENT_RECORD record) override;

private:
	/* Create default properties structure for the trace session. */
	EVENT_TRACE_PROPERTIES getProperties() const;

	/* Create default logfile structure for the trace session. */
	EVENT_TRACE_LOGFILE getTraceLogFile();

	/* Construct all the event types bits. */
	ULONG getEventTypes() const;

private:
	std::function<void(PEVENT_RECORD)> m_callback;
	std::vector<EtwEventTypes> m_eventTypes;
	AutoEtwTraceSession m_autoTraceSession;
};
