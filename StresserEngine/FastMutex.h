#pragma once

#include "AutoLock.h"
#include <ntifs.h>

class FastMutex : LockTraits
{
public:
	FastMutex() = default;

	explicit FastMutex(const FAST_MUTEX& mutex, bool autoInitialize = false);

	void init();

	void lock();

	void unlock();

private:
	FAST_MUTEX m_mutex;
};
