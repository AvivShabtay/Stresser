#pragma once

#include "../Utils/StandardThread.h"

#include <functional>
#include <string>
#include <thread>

#include <Windows.h>
#include <tdh.h>

/*
 * Safe object for ETW trace session.
 * Accept configuration (properties, logfile), initiate the trace session and mange it.
 */
class AutoEtwTraceSession
{
public:
	AutoEtwTraceSession();

	/* Automatically create new trace session. */
	explicit AutoEtwTraceSession(EVENT_TRACE_PROPERTIES properties, EVENT_TRACE_LOGFILE logfile,
		const std::wstring& sessionName);

	virtual ~AutoEtwTraceSession();

	/* Close the current trace session and start new one. */
	void reset(EVENT_TRACE_PROPERTIES properties, EVENT_TRACE_LOGFILE logfile, const std::wstring& sessionName);

	/* Stop the current trace session. */
	void release();

private:
	/* Stops the trace session. */
	void startTrace();

	/* Start the trace session. */
	void stopTrace();

	/* Allocate dynamic memory and copy the structure data into it. */
	void setProperties(EVENT_TRACE_PROPERTIES properties);

	/* Return pointer to the trace properties member. */
	PEVENT_TRACE_PROPERTIES getProperties() const;

	TRACEHANDLE m_traceSessionHandle;
	TRACEHANDLE m_traceHandle;
	std::unique_ptr<BYTE[]> m_propertiesBuffer;
	EVENT_TRACE_LOGFILE m_traceLogfile;
	std::wstring m_sessionName;
	StandardThread m_processTraceThread;
};

