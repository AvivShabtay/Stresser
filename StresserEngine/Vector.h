#pragma once

#include "FastMutex.h"
#include "AutoLock.h"

#include <ntifs.h>

template<typename T, ULONG Tag, POOL_TYPE PoolType = PagedPool>
class Vector
{
	static_assert(Tag != 0, "Tag cannot be zero");

public:
	explicit Vector(const ULONG capacity = 4)
		: m_items(nullptr), m_size(0), m_capacity(capacity)
	{
		this->m_items = this->allocate(capacity);
		this->m_lock.init();
	}

	~Vector()
	{
		__try
		{
			this->free();
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KdPrint(("Exception was thrown in Vector destructor\n"));
		}
	}

	// Delete copy constructor, assignment operator, move constructor, move operator:
	Vector(const Vector&) = delete;
	Vector& operator=(const Vector&) = delete;
	Vector(Vector&&) = delete;
	Vector& operator=(Vector&&) = delete;

	/*
	 * Return the number of items in the vector.
	 */
	const ULONG size() const
	{
		return this->m_size;
	}

	/*
	 * Return the maximum number of items that could be stored in the vector.
	 */
	size_t capacity() const
	{
		return this->m_capacity;
	}

	/*
	 * Add new item to the vector, update the capacity (if needed) and the size of the vector.
	 */
	void add(const T& item)
	{
		AutoLock<FastMutex> lock(this->m_lock);

		if (this->m_size == this->m_capacity)
		{
			this->resize(this->m_capacity * 2);
		}

		this->m_items[this->m_size] = item;
		++this->m_size;
	}

	/*
	 * Return item by it's index.
	 */
	T getAt(const ULONG index)
	{
		AutoLock<FastMutex> lock(this->m_lock);

		if (this->isValidIndexCapacity(index))
		{
			return this->m_items[index];
		}

		return nullptr;
	}

	/*
	 * Return reference to item by it's index.
	 */
	const T getAt(const ULONG index) const
	{
		AutoLock<FastMutex> lock(this->m_lock);

		if (this->isValidIndexCapacity(index))
		{
			return this->m_items[index];
		}


		return nullptr;
	}

	/*
	 * Return item by it's index using the [] operator.
	 */
	T& operator[](const size_t index)
	{
		return this->getAt(index);
	}

	/*
	 * Return reference to item by it's index using the[] operator.
	 */
	const T& operator[](const size_t index) const
	{
		return this->getAt(index);
	}

	/*
	 * Set item by it's index.
	 */
	void setAt(ULONG index, const T& item)
	{
		AutoLock<FastMutex> lock(this->m_lock);

		if (this->isValidIndexSize(index))
		{
			this->m_items[index] = item;
		}
	}

	/*
	 * Remove item by it's index.
	 */
	void removeAt(ULONG index)
	{
		AutoLock<FastMutex> lock(this->m_lock);

		if (!this->isValidIndexSize(index))
		{
			return;
		}

		// The item is not the last item:
		if (index < this->m_size - 1)
		{
			// Shift all the items one index backward:
			memcpy(this->m_items + index, this->m_items + index + 1, sizeof(T) * (this->m_size - index - 1));
		}

		// The item is the last item:
		--this->m_size;
	}

	/*
	 * Return pointer to the begin of the data.
	 */
	T* begin()
	{
		return this->m_items;
	}

	/*
	 * Return constant pointer to the begin of the data.
	 */
	const T* begin() const
	{
		return this->m_items;
	}

	/*
	 * Return pointer to the end of the data.
	 */
	T* end()
	{
		return this->m_items + this->m_size;
	}

	/*
	 * Return constant pointer to the end of the data.
	 */
	const T* end() const
	{
		return m_items + this->m_size;
	}

	/*
	 * Change the maximum number of items the vector could store.
	 */
	void resize(const ULONG capacity)
	{
		if (!this->isValidIndexCapacity(capacity))
		{
			return;
		}

		this->m_capacity = capacity;

		T* items = this->allocate(this->m_capacity);
		if (nullptr == items)
		{
			return;
		}

		memcpy(items, this->m_items, sizeof(T) * this->m_size);

		this->m_items = items;
	}

	/* Release all the allocated memory and zero sizes. */
	void clear()
	{
		AutoLock<FastMutex> lock(this->m_lock);

		memset(this->m_items, 0, this->m_capacity);

		this->m_size = 0;
	}

private:
	/*
	 * Allocate continues memory in size of the given size.
	 */
	T* allocate(const ULONG size)
	{
		if (!this->isValidIndexCapacity(size))
		{
			return nullptr;
		}

		auto* buffer = static_cast<T*>(ExAllocatePoolWithTag(PoolType, sizeof(T) * size, Tag));
		if (nullptr == buffer)
		{
			KdPrint(("Could not resize the vector, invalid memory\n"));
			return nullptr;
		}

		RtlZeroMemory(buffer, sizeof(T) * size);
		return buffer;
	}

	/* Free all the allocated memory */
	void free()
	{
		if (nullptr != this->m_items)
		{
			ExFreePoolWithTag(this->m_items, Tag);
		}
	}

	bool isValidIndexCapacity(ULONG index)
	{
		return (0 <= index && index <= this->m_capacity);
	}

	bool isValidIndexSize(ULONG index)
	{
		return (0 <= index && index <= this->m_size);
	}

	T* m_items;			// Pointer to the items in the vector
	ULONG m_size;		// The actual number of items in the vector
	ULONG m_capacity;	// The maximum number of items in the vector
	FastMutex m_lock{};
};
