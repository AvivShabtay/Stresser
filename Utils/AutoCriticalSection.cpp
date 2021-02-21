#include "pch.h"
#include "AutoCriticalSection.h"

AutoCriticalSection::AutoCriticalSection()
{
	this->m_criticalSection.reset(new CriticalSection());
	if (nullptr == this->m_criticalSection.get())
	{
		throw std::runtime_error("Could not create CriticalSection");
	}

	this->m_criticalSection->enter();
}

AutoCriticalSection::~AutoCriticalSection()
{
	this->m_criticalSection->leave();
}
