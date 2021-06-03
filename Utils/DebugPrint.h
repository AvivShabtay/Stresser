#pragma once

#include "TimeUtils.h"
#include "StringUtils.h"

#include <iostream>
#include <utility>

/*
	Helper Macro to print message only in debug mode.
 */
#ifdef _DEBUG
#define DEBUG_PRINT(message)			\
do {									\
  std::cout << (message) << std::endl;	\
} while (0)
#else
#define DEBUG_PRINT(message)	// <-- Empty on purpose to prevent printing message in release mode
#endif

#ifdef _DEBUG
#define DEBUG_WPRINT(message)			\
do {									\
  std::wcout << (message) << std::endl;	\
} while (0)
#else
#define DEBUG_WPRINT(message)	// <-- Empty on purpose to prevent printing message in release mode
#endif

 /*
	 Return the token himself.
	 https://stackoverflow.com/q/55636670
 */
#define STRINGIFY(x) #x
#define STRINGIFY_A(x) #x

 /*
  * Print varargs.
  * https://stackoverflow.com/a/29326784
  */
template<typename ...Args>
void logA(Args&& ...args)
{
	(std::cout << ... << args);
}

template<typename ...Args>
void logW(Args&& ...args)
{
	(std::wcout << ... << args);
}

/*
 *
 */
#ifdef _DEBUG
#define DEBUG_TRACE(symbol, ...)		\
do{										\
	std::cout							\
	<< TimeUtils::getCurrentTimestamp()	\
	<< " "								\
	<< STRINGIFY_A(symbol)				\
	<< " ";								\
	logA(__VA_ARGS__);					\
	std::cout << std::endl;				\
} while (0)
#else
#define debugTrace(message, ...)	// <-- Empty on purpose to prevent printing message in release mode
#endif

 /*
  *
  */
#ifdef _DEBUG
#define DEBUG_WTRACE(symbol, ...)										\
do {																	\
	std::wcout															\
	<< StringUtils::stringToWString(TimeUtils::getCurrentTimestamp())	\
	<< " "																\
	<< STRINGIFY(symbol)												\
	<< " ";																\
	logW(__VA_ARGS__);													\
	std::wcout << std::endl;											\
} while (0)
#else
#define DEBUG_WPRINT(message)	// <-- Empty on purpose to prevent printing message in release mode
#endif