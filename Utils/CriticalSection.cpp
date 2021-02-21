#include "pch.h"
#include "CriticalSection.h"
#include "../StresserExceptions/ExceptionWithWin32ErrorCode.h"

CriticalSection::CriticalSection(DWORD spinCount, DWORD flags)
{
	if (!InitializeCriticalSectionEx(&this->m_criticalSection, spinCount, flags))
	{
		throw ExceptionWithWin32ErrorCode("Could not initialize critical section");
	}
}

CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&this->m_criticalSection);
}

void CriticalSection::enter()
{
	EnterCriticalSection(&this->m_criticalSection);
}

bool CriticalSection::tryEnter()
{
	return TryEnterCriticalSection(&this->m_criticalSection);
}

void CriticalSection::leave()
{
	LeaveCriticalSection(&this->m_criticalSection);
}
