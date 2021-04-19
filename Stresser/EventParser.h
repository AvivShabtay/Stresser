#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Windows.h>
#include <tdh.h>

#pragma comment(lib, "tdh")

struct EventProperty {
	EventProperty(EVENT_PROPERTY_INFO& info);

	std::wstring Name;
	BYTE* Data;
	ULONG Length;
	EVENT_PROPERTY_INFO& Info;

	template<typename T>
	T getValue() const {
		static_assert(std::is_pod<T>() && !std::is_pointer<T>());
		return *(T*)Data;
	}

	PCWSTR getUnicodeString() const;
	PCSTR getAnsiString() const;
};

class EventParser {
public:
	EventParser(PEVENT_RECORD record);

	PTRACE_EVENT_INFO getEventInfo() const;
	PEVENT_RECORD getEventRecord() const;
	const EVENT_HEADER& getEventHeader() const;
	const std::vector<EventProperty>& getProperties() const;
	const EventProperty* getProperty(PCWSTR name) const;

	DWORD getProcessId() const;

	static std::wstring getDosNameFromNtName(PCWSTR name);

private:
	std::unique_ptr<BYTE[]> m_buffer;
	PTRACE_EVENT_INFO m_info{ nullptr };
	PEVENT_RECORD m_record;
	mutable std::vector<EventProperty> m_properties;
};