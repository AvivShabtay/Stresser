#pragma once

#include "AutoLock.h"
#include "FastMutex.h"

template<typename T, typename Lock = FastMutex>
class LinkedList
{
public:
	void init()
	{
		InitializeListHead(&m_head);
		m_lock.init();
	}

	void insertTail(T* item)
	{
		AutoLock locker(m_lock);
		InsertTailList(&m_head, &item->Entry);
	}

	void insertHead(T* value)
	{
		AutoLock locker(m_lock);
		InsertHeadList(&m_head, &item->Entry);
	}

	T* removeHead()
	{
		AutoLock locker(m_lock);
		const auto entry = RemoveHeadList(&m_head);
		return CONTAINING_RECORD(entry, T, Entry);
	}

	T* getHead()
	{
		AutoLock locker(m_lock);
		const auto entry = m_head.Flink;
		return CONTAINING_RECORD(entry, T, Entry);
	}

private:
	LIST_ENTRY m_head;
	Lock m_lock;
};
