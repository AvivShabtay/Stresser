#include <iostream>

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
