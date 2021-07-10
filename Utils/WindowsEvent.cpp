#include "pch.h"
#include "WindowsEvent.h"
#include "DebugPrint.h"
#include "Win32ErrorCodeException.h"
#include "AutoCriticalSection.h"

#include <exception>

WindowsEvent::WindowsEvent(const std::wstring& eventName, BOOL initialState, BOOL manualReset,
	LPSECURITY_ATTRIBUTES eventAttributes)
	: m_event(nullptr), m_eventName(eventName)
{
	this->createEvent(eventName, initialState, manualReset, eventAttributes);
}

WindowsEvent::~WindowsEvent()
{
	try
	{
		this->close();
	}
	catch (const std::exception& exception)
	{
		DEBUG_PRINT(exception.what());
	}
	catch (...)
	{
		DEBUG_PRINT("Exception was thrown in WindowsEvent destructor");
	}
}

WindowsEvent::WindowsEvent(const WindowsEvent& other)
{
	this->m_event = other.m_event;
	this->m_eventName = other.m_eventName;
}

WindowsEvent& WindowsEvent::operator=(const WindowsEvent& other)
{
	this->close();

	this->m_event = other.m_event;
	this->m_eventName = other.m_eventName;

	return *this;
}

WindowsEvent::WindowsEvent(WindowsEvent&& other)
{
	this->move(std::move(other));
}

WindowsEvent& WindowsEvent::operator=(WindowsEvent&& other)
{
	this->close();

	this->move(std::move(other));

	return *this;
}

const HANDLE& WindowsEvent::get() const
{
	return this->m_event;
}

void WindowsEvent::setEvent() const
{
	if (!SetEvent(this->m_event))
	{
		throw Win32ErrorCodeException("Could not set the event");
	}
}

void WindowsEvent::resetEvent() const
{
	if (!ResetEvent(this->m_event))
	{
		throw Win32ErrorCodeException("Could not set the event");
	}
}

const std::wstring& WindowsEvent::getName() const
{
	return this->m_eventName;
}

void WindowsEvent::release()
{
	this->close();
}

void WindowsEvent::reset(const std::wstring& eventName, BOOL initialState, BOOL manualReset,
	LPSECURITY_ATTRIBUTES eventAttributes)
{
	this->close();

	this->createEvent(eventName, initialState, manualReset, eventAttributes);
}

DWORD WindowsEvent::wait(DWORD timeout) const
{
	return WaitForSingleObject(this->m_event, timeout);
}

void WindowsEvent::open(const std::wstring& eventName)
{
	if (eventName.empty())
	{
		throw std::runtime_error("Invalid event name to open");
	}

	this->release();

	this->m_event = OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, eventName.c_str());
	if (nullptr == this->m_event)
	{
		throw Win32ErrorCodeException("Could not open event object");
	}
}

void WindowsEvent::createEvent(const std::wstring& eventName, BOOL initialState, BOOL manualReset,
	LPSECURITY_ATTRIBUTES eventAttributes)
{
	this->m_event = CreateEvent(eventAttributes, manualReset, initialState, eventName.c_str());
	if (nullptr == this->m_event)
	{
		throw Win32ErrorCodeException("Could not create event object");
	}
}

void WindowsEvent::close()
{
	if (nullptr != this->m_event)
	{
		if (!CloseHandle(this->m_event))
		{
			throw Win32ErrorCodeException("Could not close the event handle");
		}
	}

	this->m_eventName = L"";
}

void WindowsEvent::move(WindowsEvent&& other)
{
	this->m_event = other.m_event;
	this->m_eventName = other.m_eventName;

	other.m_event = nullptr;
	other.m_eventName = L"";
}
