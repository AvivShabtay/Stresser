#pragma once

#include "DriverConfiguration.h"

#include <ntifs.h>

/*
	Verify if the given status is NT_SUCCESS and prints to
	corresponding message.
 */
#define PRINT_STATUS_SUCCESS_FAILURE(status, successMsg, failureMsg)	\
do {																	\
	if(NT_SUCCESS(status))												\
	{																	\
		KdPrint((DRIVER_PREFIX successMsg "\n"));						\
	}																	\
	else																\
	{																	\
		KdPrint((DRIVER_PREFIX failureMsg " (0x%08X)\n", status));		\
	}																	\
} while (false);

#define PRINT_ON_STATUS_FAILURE(status, failureMsg)	\
do {												\
	if(!NT_SUCCESS(status))							\
	{												\
		KdPrint((DRIVER_PREFIX failureMsg "\n"));	\
	}												\
} while (false);

 /*
	 Verify if the given status is NT_SUCCESS and prints to
	 corresponding message.
  */
#define PRINT_IF_CONDITION(condition, successMsg, failureMsg)		\
do {																	\
	if(condition)														\
	{																	\
		KdPrint((DRIVER_PREFIX successMsg "\n"));						\
	}																	\
	else																\
	{																	\
		KdPrint((DRIVER_PREFIX failureMsg " (0x%08X)\n", status));		\
	}																	\
} while (false);

  /*
	 Verify is the given status isn't NT_SUCCESS and break.
  */
#define BREAK_ON_FAILURE(status) if(!NT_SUCCESS(status)) { break; }

#define RETURN_ON_STATUS_FAILURE(status, failureMsg)				\
do {																\
	if(!NT_SUCCESS(status))											\
	{																\
		KdPrint((DRIVER_PREFIX failureMsg " (0x%08X)\n", status));	\
		return status;												\
	}																\
} while (false);

  /*

   */
#define RETURN_ON_CONDITION(condition, message, status)			\
do {															\
	if(condition)												\
	{															\
		KdPrint((DRIVER_PREFIX message " (0x%08X)\n", status));	\
		return status;											\
	}															\
} while (false);

   /*
	   Return the token himself.
	   https://stackoverflow.com/q/55636670
   */
#define STRINGIFY(x) #x