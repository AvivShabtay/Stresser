#pragma once

#include "AutoLock.h"
#include "FastMutex.h"

/*
	Linked list data structure.
	Accept allocated item and mange it under the linked list data structure.
	@note Assuming item in the linked list contains field with LIST_ENTRY member named "Entry".
	@note Make sure to free the memory.
 */
template<typename T, typename Lock = FastMutex>
class LinkedList
{
public:
	LinkedList()
		: m_head{ nullptr, nullptr }, m_size(0)
	{
		this->init();
	}

	/* Initialize the linked list and the private lock. */
	void init()
	{
		InitializeListHead(&m_head);
		m_lock.init();
	}

	/* Insert allocated item to tail of the linked list. */
	void insertTail(T* item)
	{
		AutoLock locker(m_lock);
		InsertTailList(&m_head, &item->Entry);
		this->increaseSize();
	}

	/* Insert allocated memory to the head of the linked list. */
	void insertHead(T* item)
	{
		AutoLock locker(m_lock);
		InsertHeadList(&m_head, &item->Entry);
		this->increaseSize();
	}

	/* Remove item from the linked list, without free it's memory. */
	T* removeHead()
	{
		AutoLock locker(m_lock);
		auto* const entry = RemoveHeadList(&m_head);
		this->decreaseSize();
		return CONTAINING_RECORD(entry, T, Entry);
	}

	/* Get the item in the head of the linked list. */
	T* getHead()
	{
		AutoLock locker(m_lock);
		auto* const entry = m_head.Flink;
		return CONTAINING_RECORD(entry, T, Entry);
	}

	ULONG size()
	{
		AutoLock locker(m_lock);
		return this->m_size;
	}

private:
	void increaseSize()
	{
		++this->m_size;
	}

	void decreaseSize()
	{
		this->m_size = (this->m_size > 0 ? --this->m_size : 0);
	}

	LIST_ENTRY m_head;
	Lock m_lock;
	ULONG m_size;
};
