#include "FastMutex.h"

FastMutex::FastMutex(const FAST_MUTEX& mutex, bool autoInitialize)
	: m_mutex(mutex)
{
	if (autoInitialize)
	{
		this->init();
	}
}

void FastMutex::init()
{
	ExInitializeFastMutex(&m_mutex);
}

void FastMutex::lock()
{
	ExAcquireFastMutex(&m_mutex);
}

void FastMutex::unlock()
{
	ExReleaseFastMutex(&m_mutex);
}

