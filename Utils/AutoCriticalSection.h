#pragma once
#include "CriticalSection.h"

class AutoCriticalSection
{
public:
	AutoCriticalSection();
	~AutoCriticalSection();

	// delete copy ctor, move ctor, assignment
	AutoCriticalSection(const AutoCriticalSection&) = delete;
	AutoCriticalSection& operator=(const AutoCriticalSection&) = delete;
	AutoCriticalSection(AutoCriticalSection&&) = delete;
	AutoCriticalSection& operator=(AutoCriticalSection&&) = delete;

private:
	std::unique_ptr<CriticalSection> m_criticalSection;
};

