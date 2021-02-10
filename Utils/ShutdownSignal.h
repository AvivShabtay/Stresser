#pragma once

#include <Windows.h>
#include <iostream>

#include "AutoHandle.h"

/*
* Wrapper class for Win32 event object.
* Auto initiating name event object and managing the access to this object
* as singleton instance fot the entire application.
*/
class ShutdownSignal
{
public:
	// Disable: cloneable, assignable:
	ShutdownSignal(ShutdownSignal& other) = delete;
	void operator=(const ShutdownSignal&) = delete;

	/*
	* Return the singleton instance of this class.
	* @param signalName - the name of the Win32 event.
	* @return Instance of ShutdownSignal.
	*/
	static ShutdownSignal& GetInstance(const std::wstring signalName);

	~ShutdownSignal();

	/*
	* Returns constant instance of this class event handle.
	* @return Win32 handle for event used to signal application shutdown.
	*/
	const HANDLE Get();

private:
	ShutdownSignal(std::wstring signalName);
	std::wstring m_eventName;
	AutoHandle ahShutdownEvent;
};

