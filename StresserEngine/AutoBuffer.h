#pragma once

#include <ntifs.h>

template<ULONG Tag, POOL_TYPE PoolType = PagedPool>
class AutoBuffer final
{
public:
	explicit AutoBuffer(const SIZE_T size)
		: m_size(size)
	{
		this->allocateBuffer();
	}

	// Disable copyable, movable, assignable:
	AutoBuffer(AutoBuffer&) = delete;
	AutoBuffer& operator=(AutoBuffer&) = delete;

	// Allow movable:
	AutoBuffer(AutoBuffer&& other)
	{
		this->m_data = other.m_data;
		this->m_size = other.m_size;

		other.m_data = nullptr;
		other.m_size = 0;
	}

	AutoBuffer& operator=(AutoBuffer&& other)
	{
		this->release();

		this->m_data = other.m_data;
		this->m_size = other.m_size;

		other.m_data = nullptr;
		other.m_size = 0;
	}

	/* Destructor */
	~AutoBuffer()
	{
		__try
		{
			this->release();
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KdPrint(("Exception was thrown in AutoUnicodeString destructor\n"));
		}
	}

	/* Return the data member. */
	PVOID get() const
	{
		return this->m_data;
	}

	/* Return the size of the allocated buffer for the data member. */
	SIZE_T size() const
	{
		return this->size();
	}

private:
	/* Allocate memory for the data member. */
	void allocateBuffer()
	{
		if (0 >= this->m_size)
		{
			return;
		}

		this->m_data = ExAllocatePoolWithTag(PoolType, this->m_size, Tag);
		if (nullptr == this->m_data)
		{
			KdPrint(("Could not allocate memory in size %d\n", this->m_size));
			return;
		}

		RtlZeroMemory(this->m_data, this->m_size);
	}

	/* Release memory used for the data member. */
	void release()
	{
		if (nullptr != this->m_data)
		{
			ExFreePoolWithTag(this->m_data, Tag);

			this->m_data = nullptr;
			this->m_size = 0;
		}
	}

	SIZE_T m_size;
	PVOID m_data;
};
