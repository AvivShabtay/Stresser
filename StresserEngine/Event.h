#pragma once

#include "DriverConfiguration.h"
#include "GenericHandle.h"
#include "Value.h"

class Event final
{
public:
	Event();

	explicit Event(StresserString eventName);

	~Event();

	Event(Event&& other) noexcept;
	Event& operator=(Event&& other) noexcept;

	Value<bool, NTSTATUS> open(StresserString eventName);

	Value<bool, NTSTATUS> close();

	Value<bool, NTSTATUS> set() const;

	void release();

	const HANDLE& get() const;

	const StresserString& getEventName() const;

private:
	HANDLE m_event;
	StresserString m_eventName;
};

