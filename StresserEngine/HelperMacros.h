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
	  Verify if the OS version is above vista, otherwise print message and break.
   */
   /*
   #define VISTA_AND_ABOVE(failureMsg)					\
   do {												\
	   if(!(NTDDI_VERSION >= NTDDI_VISTA))				\
	   {												\
		   KdPrint((DRIVER_PREFIX failureMsg "\n"));	\
		   break;										\
	   }												\
   } while (false);
   */

#define DO_NOTHING (void)0