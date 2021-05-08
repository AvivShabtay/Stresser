#pragma once

#include <ntdef.h>

/* Define globals types for User to Kernel communication. */

#define STRESSER_ENGINE_DEVICE 0x8000

/* Register event object for signaling from kernel mode to user mode. */
#define IOCTL_STRESSER_ENGINE_REGISTER_EVENT CTL_CODE(STRESSER_ENGINE_DEVICE, \
	0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_STRESSER_ENGINE_REMOVE_PATH CTL_CODE(STRESSER_ENGINE_DEVICE, \
	0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_STRESSER_ENGINE_GET_PATHS_LEN CTL_CODE(STRESSER_ENGINE_DEVICE, \
	0x802, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_STRESSER_ENGINE_GET_PATHS CTL_CODE(STRESSER_ENGINE_DEVICE, \
	0x801, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

/* Used for sending event handle throw IOCTL. */
struct RegisterEvent
{
	HANDLE eventHandle;
};

constexpr SIZE_T REGISTER_EVENT_SIZE = sizeof(RegisterEvent);