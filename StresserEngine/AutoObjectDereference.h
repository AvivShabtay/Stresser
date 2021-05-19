#pragma once

#include <ntifs.h>

template<typename Object>
class AutoObjectDereference
{
public:
	explicit AutoObjectDereference(Object object = nullptr)
		: m_object(object)
	{
	}

	// Disable copyable:
	AutoObjectDereference(AutoObjectDereference& other) = delete;
	AutoObjectDereference& operator=(AutoObjectDereference& other) = delete;

	/* Move constructor */
	AutoObjectDereference(AutoObjectDereference&& other) noexcept
	{
		this->m_object = other.m_object;
		other.m_object = nullptr;
	}

	/* Move assignment */
	AutoObjectDereference& operator=(AutoObjectDereference&& other) noexcept
	{
		this->dereference();

		this->m_object = other.m_object;
		other.m_object = nullptr;
		return *this;
	}

	/* Destructor */
	~AutoObjectDereference()
	{
		__try
		{

			this->dereference();
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KdPrint(("Exception was thrown in ObDereferenceObject destructor"));
		}
	}

	/* Return the object member. */
	Object& get()
	{
		return this->m_object;
	}

	/* Return if the object member address is valid. */
	bool isValid()
	{
		return nullptr != this->m_object;
	}

private:
	/* Decrease the reference count to the object member. */
	void dereference()
	{
		if (nullptr != this->m_object)
		{
			ObDereferenceObject(m_object);
		}
	}

	Object m_object;
};