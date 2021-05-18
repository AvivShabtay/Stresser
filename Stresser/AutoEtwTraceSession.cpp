#include "AutoEtwTraceSession.h"

#include "../Utils/DebugPrint.h"
#include "../Utils/Win32ErrorCodeException.h"

#include <exception>
#include <iostream>

AutoEtwTraceSession::AutoEtwTraceSession()
	: m_traceSessionHandle{ 0 }, m_traceHandle{ 0 }, m_propertiesBuffer(nullptr), m_traceLogfile{ 0 }
{
}

AutoEtwTraceSession::AutoEtwTraceSession(EVENT_TRACE_PROPERTIES properties, EVENT_TRACE_LOGFILEW logfile, const std::wstring& sessionName)
	: m_traceSessionHandle{ 0 }, m_traceHandle{ 0 }, m_propertiesBuffer(nullptr), m_traceLogfile(logfile), m_sessionName(sessionName)
{
	this->setProperties(properties);
	this->m_traceLogfile = logfile;
	this->m_sessionName = sessionName;

	this->startTrace();
}

AutoEtwTraceSession::~AutoEtwTraceSession()
{
	try
	{
		this->stopTrace();
	}
	catch (const std::exception& exception)
	{
		DEBUG_PRINT(exception.what());
	}
	catch (...)
	{
		DEBUG_PRINT("Exception was thrown in AutoTraceManager destructor");
	}
}

void AutoEtwTraceSession::reset(EVENT_TRACE_PROPERTIES properties, EVENT_TRACE_LOGFILEW logfile,
	const std::wstring& sessionName)
{
	this->stopTrace();

	this->setProperties(properties);
	this->m_traceLogfile = logfile;
	this->m_sessionName = sessionName;

	this->startTrace();
}

void AutoEtwTraceSession::release()
{
	this->stopTrace();
}

void AutoEtwTraceSession::startTrace()
{
	// Trace already running:
	if (0 != m_traceSessionHandle || 0 != m_traceHandle)
	{
		return;
	}

	auto* properties = reinterpret_cast<PEVENT_TRACE_PROPERTIES>(this->m_propertiesBuffer.get());

	// If exists stop the trace session with the same name
	EVENT_TRACE_PROPERTIES prop = { sizeof(EVENT_TRACE_PROPERTIES) };
	ULONG error = ControlTrace(NULL, this->m_sessionName.c_str(), &prop, EVENT_TRACE_CONTROL_STOP);
	if (error != ERROR_SUCCESS && error != ERROR_MORE_DATA && error != ERROR_WMI_INSTANCE_NOT_FOUND)
	{
		throw Win32ErrorCodeException("Could not close running ETW trace");
	}

	error = ::StartTrace(&this->m_traceSessionHandle, this->m_sessionName.c_str(), properties);
	if (ERROR_SUCCESS != error)
	{
		// ERROR_ALREADY_EXISTS indicates the trace session is already running:
		if (ERROR_ALREADY_EXISTS != error)
		{
			throw Win32ErrorCodeException("Could not start ETW trace");
		}
	}

	m_traceHandle = ::OpenTrace(&m_traceLogfile);
	if (INVALID_PROCESSTRACE_HANDLE == this->m_traceHandle)
	{
		throw Win32ErrorCodeException("Could not open ETW trace");
	}

	this->m_processTraceThread.reset([](auto params)
		{
			auto* const etwTraceSession = static_cast<AutoEtwTraceSession*>(params);
			const ULONG error = ::ProcessTrace(&etwTraceSession->m_traceHandle, 1, nullptr, nullptr);
			if (ERROR_SUCCESS != error)
			{
				throw Win32ErrorCodeException("ProcessTrace failed");
			}
			return error;
		}, this);

	error = ControlTrace(this->m_traceHandle, this->m_sessionName.c_str(), properties, EVENT_TRACE_CONTROL_FLUSH);
	if (ERROR_SUCCESS != error)
	{
		throw Win32ErrorCodeException("Could not flush the trace session buffers");
	}
}

void AutoEtwTraceSession::stopTrace()
{
	if (0 != m_traceHandle)
	{
		const ULONG error = CloseTrace(m_traceHandle);

		if (ERROR_SUCCESS != error)
		{
			// ERROR_CTX_CLOSE_PENDING indicates successful call and
			// ProcessTrace function will stop after it has processed all real-time events in its buffers.
			if (ERROR_CTX_CLOSE_PENDING != error)
			{
				throw Win32ErrorCodeException("CloseTrace failed");
			}
		}
		m_traceHandle = 0;
	}

	if (0 != m_traceSessionHandle)
	{
		auto* properties = reinterpret_cast<PEVENT_TRACE_PROPERTIES>(this->m_propertiesBuffer.get());

		const ULONG  error = ControlTrace(this->m_traceHandle, this->m_sessionName.c_str(), properties, EVENT_TRACE_CONTROL_STOP);
		if (ERROR_SUCCESS != error)
		{
			// ERROR_MORE_DATA indicates that ETW stop the session before
			// generating this error.
			if (ERROR_MORE_DATA != error)
			{
				throw Win32ErrorCodeException("Could not stop the ETW trace session");
			}
		}

		m_traceSessionHandle = 0;
	}

	// Blocking call:
	this->m_processTraceThread.release();
}

void AutoEtwTraceSession::setProperties(EVENT_TRACE_PROPERTIES properties)
{
	const auto size = properties.Wnode.BufferSize;
	if (0 >= size)
	{
		throw std::runtime_error("Invalid properties buffer size");
	}

	this->m_propertiesBuffer.reset(new BYTE[size]);
	if (nullptr == this->m_propertiesBuffer)
	{
		throw std::runtime_error("Could not allocate memory for properties");
	}

	memset(this->m_propertiesBuffer.get(), 0, size);
	memcpy(this->m_propertiesBuffer.get(), &properties, sizeof(EVENT_TRACE_PROPERTIES));
}

PEVENT_TRACE_PROPERTIES AutoEtwTraceSession::getProperties() const
{
	if (nullptr == this->m_propertiesBuffer)
	{
		throw std::runtime_error("Properties memory is invalid");
	}

	return reinterpret_cast<PEVENT_TRACE_PROPERTIES>(this->m_propertiesBuffer.get());
}
