#pragma once

#include <Windows.h>

class AutoServiceHandle final
{
public:
	explicit AutoServiceHandle(const SC_HANDLE& handle);

	~AutoServiceHandle();

	// Disable: copyable, assignable, movable:
	AutoServiceHandle(const AutoServiceHandle&) = delete;
	AutoServiceHandle& operator=(const AutoServiceHandle&) = delete;
	AutoServiceHandle(AutoServiceHandle&&) = delete;
	AutoServiceHandle& operator=(AutoServiceHandle&&) = delete;

	/* Set the SC_HANDLE member, close the old one if exists. */
	void reset(const SC_HANDLE& handle);

	/* Return the SC_HANDLE member. */
	SC_HANDLE get() const;

protected:
	/* Try to close the SC_HANDLE, throw exception if failed. */
	void serviceHandleDeleter() const;

	SC_HANDLE m_handle;
};