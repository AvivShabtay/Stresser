#pragma once

#include <string>

#include <Windows.h>

/*
 * Wrapper for Win32 Event objects.
 */
class WindowsEvent
{
public:
	explicit WindowsEvent(const std::wstring& eventName = L"", BOOL initialState = FALSE, BOOL manualReset = TRUE,
		LPSECURITY_ATTRIBUTES eventAttributes = nullptr);

	virtual ~WindowsEvent();

	// Disable: copyable, assignable:
	WindowsEvent(const WindowsEvent& other);

	WindowsEvent& operator=(const WindowsEvent& other);

	WindowsEvent(WindowsEvent&& other);

	WindowsEvent& operator=(WindowsEvent&& other);

	/* Return handle to the event object. */
	const HANDLE& get() const;

	/* Signal the event object. */
	void setEvent() const;

	/* Reset the event object state.  */
	void resetEvent() const;

	/* Get the name of the event object. */
	const std::wstring& getName() const;

	/* Close the event object resource. */
	void release();

	/* If exists, close the current event object resource and create new one. */
	void reset(const std::wstring& eventName = L"", BOOL initialState = FALSE, BOOL manualReset = TRUE,
		LPSECURITY_ATTRIBUTES eventAttributes = nullptr);

	/* Wait on the event object and return the result. */
	DWORD wait(DWORD timeout = INFINITE) const;

	/* Return if the event object signaled. */
	bool isSignaled() const
	{
		return (WAIT_OBJECT_0 == this->wait(0));
	}

	void open(const std::wstring& eventName);

private:
	/* Create event object from by given values. */
	void createEvent(const std::wstring& eventName, BOOL initialState, BOOL manualReset, LPSECURITY_ATTRIBUTES eventAttributes);

	/* If exists, close the current event object resource. */
	void close();

	/* Move the event object resource form other object to this one. */
	void move(WindowsEvent&& other);

	HANDLE m_event;
	std::wstring m_eventName;
};

