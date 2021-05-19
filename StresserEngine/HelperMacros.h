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
		KdPrint((DRIVER_PREFIX CURRENT_FILE ": " successMsg "\n"));						\
	}																	\
	else																\
	{																	\
		KdPrint((DRIVER_PREFIX CURRENT_FILE failureMsg " (0x%08X)\n", status));		\
	}																	\
} while (false);

#define PRINT_ON_STATUS_FAILURE(status, failureMsg)	\
do {												\
	if(!NT_SUCCESS(status))							\
	{												\
		KdPrint((DRIVER_PREFIX CURRENT_FILE failureMsg "\n"));	\
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
		KdPrint((DRIVER_PREFIX CURRENT_FILE successMsg "\n"));						\
	}																	\
	else																\
	{																	\
		KdPrint((DRIVER_PREFIX CURRENT_FILE failureMsg " (0x%08X)\n", status));		\
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
		KdPrint((DRIVER_PREFIX CURRENT_FILE failureMsg " (0x%08X)\n", status));	\
		return status;												\
	}																\
} while (false);

  /*

   */
#define RETURN_STATUS_ON_CONDITION(condition, message, status)			\
do {															\
	if(condition)												\
	{															\
		KdPrint((DRIVER_PREFIX CURRENT_FILE message " (0x%08X)\n", status));	\
		return status;											\
	}															\
} while (false);

   /*

	*/
#define RETURN_ON_CONDITION(condition)	\
do {									\
	if(condition)						\
	{									\
		return;							\
	}									\
} while (false);

	/*

	 */
#define PRINT_MESSAGE_AND_RETURN_ON_CONDITION(condition, message)	\
do {									\
	if(condition)						\
	{									\
		KdPrint((DRIVER_PREFIX CURRENT_FILE message "\n"));	\
		return;							\
	}									\
} while (false);

	 /*
	  *
	  */
#define CURRENT_FILE __FILE__

	  /*
		  Return the token himself.
		  https://stackoverflow.com/q/55636670
	  */
#define STRINGIFY(x) #x

	  /*
	   *
	   */
#define LOG_MESSAGE(msg)							\
do {												\
	KdPrint((DRIVER_PREFIX CURRENT_FILE ": " msg "\n"));	\
} while (false);

	   /*
		*
		*/
#define MIN(a,b) (((a) < (b)) ? (a) : (b))