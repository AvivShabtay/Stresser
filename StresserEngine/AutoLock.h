#pragma once

/*
 * Represent to functions that lock should implement.
 */
struct LockTraits
{
	void lock()
	{
	}

	void unlock()
	{
	}
};

template<typename Lock = LockTraits>
struct AutoLock
{
	AutoLock(Lock& lock)
		: m_lock(lock)
	{
		this->m_lock.lock();
	}

	~AutoLock()
	{
		this->m_lock.unlock();
	}

private:
	Lock& m_lock;
};