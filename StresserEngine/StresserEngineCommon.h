#pragma once

//#include <ntdef.h>

/* Define globals types for User to Kernel communication. */

#define STRESSER_ENGINE_DEVICE 0x8000

/* Register shared event by it's name for signaling from kernel mode to user mode. */
#define IOCTL_STRESSER_ENGINE_REGISTER_EVENT CTL_CODE(STRESSER_ENGINE_DEVICE, \
	0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Register PID of fake process to be monitored. */
#define IOCTL_STRESSER_ENGINE_ADD_FAKE_PID CTL_CODE(STRESSER_ENGINE_DEVICE, \
	0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Remove PID of fake process from being monitored. */
#define IOCTL_STRESSER_ENGINE_REMOVE_FAKE_PID CTL_CODE(STRESSER_ENGINE_DEVICE, \
	0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Send buffer and receive the current recorded events. */
#define IOCTL_STRESSER_ENGINE_GET_EVENTS CTL_CODE(STRESSER_ENGINE_DEVICE, \
	0x803, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

// Sizes of fields shared by user mode and kernel mode:
const int MAX_EVENT_NAME_LENGTH = 100;
const int MAX_PROCESS_NAME_LENGTH = 300;

/*
	Used for sending event handle throw IOCTL.
*/
struct RegisterEvent
{
	WCHAR eventName[MAX_EVENT_NAME_LENGTH + 1];
};

/*
	Used for sending fake process ID throw IOCTL.
*/
struct FakeProcessId
{
	ULONG processId;
};

/*
	Represent process detection event.
	Fires whenever process try to open handle to fake process.
*/
struct EventInfo
{
	ULONG processId;								// ID of calling process
	WCHAR processName[MAX_PROCESS_NAME_LENGTH + 1];	// Filename path of calling process
	ULONG fakeProcessId;							// Called for this fake process
	LARGE_INTEGER time;								// Timestamp of the call
};

/*

 */
struct EventsHeader
{
	ULONG size;
	EventInfo* events;
};

constexpr SIZE_T REGISTER_EVENT_SIZE = sizeof(RegisterEvent);
constexpr SIZE_T FAKE_PROCESS_ID_SIZE = sizeof(FakeProcessId);
constexpr SIZE_T EVENT_INFO_SIZE = sizeof(EventInfo);
constexpr SIZE_T EVENTS_HEADER_SIZE = sizeof(EventsHeader);