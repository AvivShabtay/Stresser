#pragma once

#include "Memory.h"

#include <ntifs.h>

template<ULONG Tag, POOL_TYPE PoolType = PagedPool>
class String
{
public:
	String(const PCWCHAR str = nullptr)
		: String(const_cast<PWCHAR>(str), 0)
	{
	}

	/* Construct new empty String object. */
	explicit String(const PWCHAR str)
		: String(str, 0)
	{
	}

	/* Construct new String object with reserved size. */
	explicit String(const PCWCHAR data, ULONG count)
		: String(const_cast<PWCHAR>(data), count)
	{
	}

	/* Construct new String object with reserved size. */
	explicit String(const PWCHAR data, ULONG count)
		: m_data(nullptr), m_length(0), m_capacity(0), m_pool(PoolType), m_tag(Tag)
	{
		if (nullptr != data)
		{
			if (0 == count)
			{
				this->m_length = static_cast<ULONG>(wcslen(data));
			}
			else
			{
				this->m_length = count;
			}

			this->m_capacity = this->m_length + 1; // Null-terminator
			this->m_data = this->allocate(this->m_capacity, data);
			if (nullptr == this->m_data)
			{
				ExRaiseStatus(STATUS_NO_MEMORY);
			}
		}
		else
		{
			this->m_data = nullptr;
			this->m_length = 0;
		}
	}

	explicit String(ULONG reservedSize)
		: m_pool(PoolType), m_tag(Tag)
	{
		this->m_data = String::allocateReservedBuffer(reservedSize);
		this->m_capacity = reservedSize;
		this->m_length = 0;
	}

	/* Copy constructor */
	String(const String& other)
	{
		this->m_length = other.m_length;
		this->m_pool = other.m_pool;
		this->m_tag = other.m_tag;
		this->m_capacity = other.m_capacity;

		if (0 < this->m_length)
		{
			this->m_data = this->allocate(this->m_capacity, other.m_data);
		}
		else
		{
			this->m_data = nullptr;
		}
	}

	/* Construct new String object from UNICODE_STRING type. */
	explicit String(const PUNICODE_STRING unicodeString)
		: m_data(nullptr), m_length(0), m_capacity(0), m_pool(PoolType), m_tag(Tag)
	{
		if (nullptr != unicodeString)
		{
			// UNICODE_STRING length is in bytes and not WCHARs
			this->m_length = unicodeString->Length / sizeof(WCHAR);
			this->m_capacity = m_length + 1;

			this->m_data = this->allocate(this->m_capacity, unicodeString->Buffer);
		}
	}

	/* Assignment operator. */
	String& operator=(const String& other)
	{
		if (&other != this)
		{
			this->release();

			this->m_length = other.m_length;
			this->m_capacity = other.m_capacity;
			this->m_tag = other.m_tag;
			this->m_pool = other.m_pool;

			if (nullptr != other.m_data)
			{
				this->m_data = this->allocate(this->m_capacity, other.m_data);
			}
		}

		return *this;
	}

	/* Move constructor. */
	String(String&& other) noexcept
	{
		this->m_length = other.m_length;
		this->m_capacity = other.m_capacity;
		this->m_pool = other.m_pool;
		this->m_tag = other.m_tag;
		this->m_data = other.m_data;

		other.m_length = 0;
		other.m_data = nullptr;
	}

	/* Move assignment. */
	String& operator=(String&& other) noexcept
	{
		if (&other != this)
		{
			this->release();

			this->m_length = other.m_length;
			this->m_capacity = other.m_capacity;
			this->m_pool = other.m_pool;
			this->m_tag = other.m_tag;
			this->m_data = other.m_data;

			other.m_length = 0;
			other.m_data = nullptr;
		}

		return *this;
	}

	/* Destructor */
	~String()
	{
		this->release();
	}

	/* Append operator. */
	String& operator+=(const String& other)
	{
		return this->append(other.m_data, other.m_length);
	}

	/* Append operator for WCHAR type. */
	String& operator+=(const PCWSTR data)
	{
		this->m_length += static_cast<ULONG>(wcslen(data));
		PWCHAR newBuffer = this->allocate(this->m_length, this->m_data);

		wcscat_s(newBuffer, this->m_length + 1, data);

		this->release();
		this->m_data = newBuffer;

		return *this;
	}

	/* Equality operator. */
	bool operator==(const String& other)
	{
		return
			this->m_data == other.m_data &&
			this->m_pool == other.m_pool &&
			this->m_tag == other.m_tag &&
			this->m_length == other.m_length;
	}

	/*
	 * Compare given String with this one.
	 * Return a zero value if both Strings are considered equal.
	 * otherwise return a value represent which String is greater.
	 */
	int compare(const String& other)
	{
		return wcscmp(this->m_data, other.m_data);
	}

	operator const PWCHAR() const {
		return this->m_data;
	}

	PWCHAR get() const
	{
		return static_cast<const PWCHAR>(this->m_data);
	}

	ULONG length() const
	{
		return this->m_length;
	}

	/*
	 * Return copy of this String with lower case data.
	 */
	String toLower() const
	{
		String temp(this->m_data);
		_wcslwr(temp.m_data);

		return temp;
	}

	/*
	 * Convert data to lower case.
	 */
	String& toLower()
	{
		_wcslwr(this->m_data);
		return *this;
	}

	/*
	 * Return copy of this String with upper case data.
	 */
	String toUpper() const
	{
		String temp(this->m_data);
		_wcsupr(temp.m_data);

		return temp;
	}

	/*
	 * Convert data to upper case.
	 */
	String& toUpper()
	{
		_wcsupr(this->m_data);
		return *this;
	}

	/*
	 * Place null terminator in the String data at given index.
	 */
	String& truncate(ULONG index)
	{
		NT_ASSERT(index >= this->m_length);

		this->m_length = index;
		this->m_data[this->m_length] = L'\0';

		return *this;
	}

	/*
	 * Return character of the data at given index.
	 */
	const WCHAR getAt(const size_t index) const
	{
		NT_ASSERT(index < this->m_length);

		return const_cast<const WCHAR>(this->m_data[index]);
	}

	/*
	 * Return reference for character of the data at given index.
	 */
	WCHAR& getAt(const size_t index)
	{
		NT_ASSERT(index < this->m_length);

		return this->m_data[index];
	}

	/*
	 * Return character of the data at given index using [] operator.
	 */
	const WCHAR operator[](const size_t index) const
	{
		return getAt(index);
	}

	/*
	 * Return reference for character of the data at given index using [] operator.
	 */
	WCHAR& operator[](const size_t index)
	{
		return getAt(index);
	}

	/*
	 * Convert class data to UNICODE_STRING type.
	 */
	UNICODE_STRING* getUnicodeString(const PUNICODE_STRING unicodeString)
	{
		RtlInitUnicodeString(unicodeString, this->m_data);
		return unicodeString;
	}

	/*
	 * Find the first occurrence of this String in other String
	 * and return the pointer to this location.
	 */
	const WCHAR* findOccurrence(const String& other)
	{
		if (0 >= this->m_length || 0 >= other.m_length)
		{
			return nullptr;
		}

		return wcsstr(other.m_data, this->m_data);
	}

	/*
	 * Check if this String is sub-string of other String.
	 */
	bool isSubString(const String& other)
	{
		return nullptr != this->findOccurrence(other);
	}

private:
	/* Allocate memory and copy the data from the source to the new allocated memory. */
	PWCHAR allocate(size_t numOfChars, const PWCHAR source = nullptr) const
	{
		const auto buffer = static_cast<PWCHAR>(ExAllocatePoolWithTag(PoolType, sizeof(WCHAR) * (numOfChars + 1), Tag));
		if (nullptr == buffer)
		{
			KdPrint(("Failed to allocate kstring of length %d chars\n", numOfChars));
			return nullptr;
		}
		else
		{
			const SIZE_T sourceLength = wcslen(source) + 1;
			const SIZE_T size = (sourceLength < numOfChars ? sourceLength : numOfChars);
			wcscpy_s(buffer, size, source);
		}
		return buffer;
	}

	static PWCHAR allocateReservedBuffer(ULONG bufferSize)
	{
		const auto buffer = static_cast<PWCHAR>(ExAllocatePoolWithTag(PoolType, bufferSize, Tag));
		if (nullptr == buffer)
		{
			KdPrint(("Failed to allocate buffer of length %d chars\n", bufferSize));
			return nullptr;
		}
		else
		{
			RtlZeroMemory(buffer, bufferSize);
		}
		return buffer;
	}

	/* Release allocated memory for class data. */
	void release()
	{
		if (nullptr != this->m_data)
		{
			delete this->m_data;
			this->m_data = nullptr;
		}
	}

	/* Concatenate another string to this string. */
	String& append(const PCWSTR data, ULONG len = 0)
	{
		if (0 == len)
		{
			len = static_cast<ULONG>(wcslen(data));
		}

		bool newAlloc = false;
		PWCHAR newBuffer = this->m_data;

		this->m_length += len;

		if (this->m_length + 1 > this->m_capacity)
		{
			this->m_capacity = this->m_length + this->INCREASE_SIZE;
			newBuffer = this->allocate(this->m_capacity, this->m_data);
			newAlloc = true;
		}

		wcsncat_s(newBuffer, this->m_capacity, data, len);

		if (newAlloc)
		{
			this->release();
			this->m_data = newBuffer;
		}
		return *this;
	}

	const ULONG INCREASE_SIZE = 9;

	PWCHAR m_data;
	ULONG m_length;
	ULONG m_capacity;
	POOL_TYPE m_pool;
	ULONG m_tag;
};