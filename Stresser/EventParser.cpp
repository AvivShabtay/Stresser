#include "EventParser.h"

#include <Windows.h>

EventParser::EventParser(PEVENT_RECORD record) : m_record(record) {
	ULONG size = 0;
	auto error = ::TdhGetEventInformation(record, 0, nullptr, m_info, &size);
	if (error == ERROR_INSUFFICIENT_BUFFER) {
		m_buffer = std::make_unique<BYTE[]>(size);
		m_info = reinterpret_cast<PTRACE_EVENT_INFO>(m_buffer.get());
		error = ::TdhGetEventInformation(record, 0, nullptr, m_info, &size);
	}
	::SetLastError(error);
}

PTRACE_EVENT_INFO EventParser::getEventInfo() const {
	return m_info;
}

PEVENT_RECORD EventParser::getEventRecord() const {
	return m_record;
}

const EVENT_HEADER& EventParser::getEventHeader() const {
	return m_record->EventHeader;
}

const std::vector<EventProperty>& EventParser::getProperties() const {
	if (!m_properties.empty())
		return m_properties;

	m_properties.reserve(m_info->TopLevelPropertyCount);
	auto userDataLength = m_record->UserDataLength;
	BYTE* data = (BYTE*)m_record->UserData;

	for (ULONG i = 0; i < m_info->TopLevelPropertyCount; i++) {
		auto& prop = m_info->EventPropertyInfoArray[i];
		EventProperty property(prop);
		property.Name.assign((WCHAR*)((BYTE*)m_info + prop.NameOffset));
		auto len = prop.length;
		property.Length = len;
		property.Data = data;
		data += len;
		userDataLength -= len;

		m_properties.push_back(std::move(property));
	}

	return m_properties;
}

const EventProperty* EventParser::getProperty(PCWSTR name) const {
	for (auto& prop : getProperties())
		if (prop.Name == name)
			return &prop;
	return nullptr;
}

DWORD EventParser::getProcessId() const {
	return m_record->EventHeader.ProcessId;
}

std::wstring EventParser::getDosNameFromNtName(PCWSTR name) {
	static std::vector<std::pair<std::wstring, std::wstring>> deviceNames;
	static bool first = true;
	if (first) {
		auto drives = ::GetLogicalDrives();
		int drive = 0;
		while (drives) {
			if (drives & 1) {
				// drive exists
				WCHAR driveName[] = L"X:";
				driveName[0] = (WCHAR)(drive + 'A');
				WCHAR path[MAX_PATH];
				if (::QueryDosDevice(driveName, path, MAX_PATH)) {
					deviceNames.push_back({ path, driveName });
				}
			}
			drive++;
			drives >>= 1;
		}
		first = false;
	}

	for (auto& [ntName, dosName] : deviceNames) {
		if (::_wcsnicmp(name, ntName.c_str(), ntName.size()) == 0)
			return dosName + (name + ntName.size());
	}
	return L"";
}

EventProperty::EventProperty(EVENT_PROPERTY_INFO& info) : Info(info) {
}

PCWSTR EventProperty::getUnicodeString() const {
	return (PCWSTR)Data;
}

PCSTR EventProperty::getAnsiString() const {
	return (PCSTR)Data;
}