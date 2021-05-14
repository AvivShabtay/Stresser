#include "Event.h"
#include "HelperMacros.h"
#include "Move.h"

Event::Event()
	: m_event(nullptr)
{
}

Event::Event(StresserString eventName)
	: m_event(nullptr), m_eventName(move(eventName))
{
	Value<bool, NTSTATUS> result = this->open(eventName);
	if (result.isError())
	{
		PRINT_ON_STATUS_FAILURE(result.getError(), "Could not open event");
	}
}

Event::~Event()
{
	__try
	{
		auto result = this->close();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_MESSAGE("exception was thrown in destructor");
	}
}

Event::Event(Event&& other) noexcept
{
	this->m_event = other.m_event;
	this->m_eventName = move(other.m_eventName);

	other.m_event = nullptr;
	other.m_eventName.release();
}

Event& Event::operator=(Event&& other) noexcept
{
	Value<bool, NTSTATUS> result = this->close();
	if (result.isError())
	{
		PRINT_ON_STATUS_FAILURE(result.getError(), "could not close current event object");
		return *this;
	}

	this->m_event = other.m_event;
	this->m_eventName = move(other.m_eventName);

	other.m_event = nullptr;
	other.m_eventName.release();

	return *this;
}

Value<bool, NTSTATUS> Event::open(StresserString eventName)
{
	this->m_eventName = move(eventName);
	RETURN_ON_CONDITION(this->m_eventName.empty(), "got empty event name", STATUS_INVALID_PARAMETER);

	const StresserBuffer eventNameBuffer(sizeof(UNICODE_STRING));

	const PUNICODE_STRING eventNameUnicodeString =
		this->m_eventName.getUnicodeString(static_cast<PUNICODE_STRING>(eventNameBuffer.get()));

	OBJECT_ATTRIBUTES eventAttribute{ 0 };
	InitializeObjectAttributes(
		&eventAttribute,
		eventNameUnicodeString,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		nullptr,
		nullptr
	);

	const NTSTATUS status = ZwOpenEvent(&this->m_event, EVENT_MODIFY_STATE, &eventAttribute);
	RETURN_ON_STATUS_FAILURE(status, "could not open handle to event object");

	if (nullptr == this->m_event)
	{
		return false;
	}

	return true;
}

Value<bool, NTSTATUS> Event::close() const
{
	if (nullptr != this->m_event)
	{
		return false;
	}

	const NTSTATUS status = ZwClose(this->m_event);
	RETURN_ON_STATUS_FAILURE(status, "could not close event handle");

	return true;
}

Value<bool, NTSTATUS> Event::set() const
{
	if (nullptr == this->m_event)
	{
		return false;
	}

	LONG previousStatus = 0;
	const NTSTATUS status = ZwSetEvent(this->m_event, &previousStatus);
	RETURN_ON_STATUS_FAILURE(status, "could not set the event");

	return true;
}

void Event::release()
{
	this->m_event = nullptr;
}

const HANDLE& Event::get() const
{
	return this->m_event;
}

const StresserString& Event::getEventName() const
{
	return this->m_eventName;
}
