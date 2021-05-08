#pragma once

#include <ntifs.h>

/* Represent the valid traits for using GenericHandle object. */
struct CommonTraits
{
	static void close(const HANDLE handle)
	{
		UNREFERENCED_PARAMETER(handle);
	}

	static bool isValid(const HANDLE handle)
	{
		UNREFERENCED_PARAMETER(handle);
		return false;
	}

	static HANDLE invalidHandle()
	{
		return nullptr;
	}
};

/*
 * Define object for managing handle safely.
 * Credit for @Zodiacon: https://github.com/zodiacon/ndcoslo2019/tree/master/CppKernel
 */
template<typename T, typename Traits = CommonTraits>
class GenericHandle final
{
public:
	explicit GenericHandle(T handle = Traits::invalidHandle(), const bool owner = true)
		: m_handle(handle), m_owner(owner)
	{
	}

	/* Destructor */
	~GenericHandle()
	{
		__try
		{
			this->reset();
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			KdPrint(("Exception accrued in GenericHandle destructor\n"));
		}
	}

	// Disable copyable:
	GenericHandle(GenericHandle&) = delete;
	GenericHandle& operator=(const GenericHandle&) = delete;

	/* Move constructor */
	GenericHandle(GenericHandle&& other) noexcept
	{
		this->m_handle = other.m_handle;
		this->m_owner = other.m_owner;
		other.m_handle = nullptr;
	}

	/* Move assignment */
	GenericHandle& operator=(GenericHandle&& other) noexcept
	{
		if (this != &other)
		{
			this->reset();
			this->m_handle = other.m_handle;
			this->m_owner = other.m_owner;
			other.m_handle = nullptr;
		}
		return *this;
	}

	/* Equality operator. */
	bool operator==(const GenericHandle& other)
	{
		return this->m_handle == other.m_handle;
	}

	/* Function like call to return the object handle member. */
	operator T() const
	{
		return this->get();
	}

	/* Return the handle member. */
	T get() const
	{
		return this->m_handle;
	}

	/* Verify if the handle member is valid. */
	operator bool const()
	{
		return Traits::isValid(this->m_handle);
	}

	/* Return the address of the handle member. */
	T* getAddressOf()
	{
		NT_ASSERT(!Traits::isValid(this->m_handle));
		return &this->m_handle;
	}

	/* Close the current handle and manage another one. */
	void reset(T handle = nullptr, bool owner = true)
	{
		if (this->m_owner && this->m_handle)
		{
			Traits::close(this->m_handle);
		}

		this->m_handle = handle;
		this->m_owner = owner;
	}

	/*
	 * Stop manage the handle member.
	 * @note The handle would not be release in class destruction.
	 */
	T detach()
	{
		auto handle = this->m_handle;
		this->m_handle = nullptr;
		return handle;
	}

private:
	T m_handle;
	bool m_owner;
};


/* Represent traits for simple kernel handle. */
struct KernelHandleTraits : CommonTraits
{
	static void close(const HANDLE handle)
	{
		if (nullptr != handle)
		{
			ZwClose(handle);
		}
	}

	static bool isValid(const HANDLE handle)
	{
		return handle != nullptr;
	}

	static HANDLE invalidHandle()
	{
		return nullptr;
	}
};

/* Define smart pointer for kernel space. */
using AutoKernelHandle = GenericHandle<HANDLE, KernelHandleTraits>;