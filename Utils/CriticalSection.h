#pragma once

#include <Windows.h>

/*
 * Wrapper class for Win32 CRITICAL_SECTION type.
 */
class CriticalSection
{
public:
	CriticalSection(DWORD spinCount = 0, DWORD flags = 0);
	~CriticalSection();

	void enter();
	bool tryEnter();
	void leave();

private:
	CRITICAL_SECTION m_criticalSection;
};

