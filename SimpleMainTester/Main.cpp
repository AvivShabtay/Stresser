#include "../Utils/AutoHandle.h"
#include "../Utils/DebugPrint.h"
#include "../Utils/SehTranslatorGuard.h"

#include <exception>

#include <Windows.h>

int main() {

	//SehTranslatorGuard sehTranslatorGuard;

	try
	{

		DEBUG_PRINT("bla");

	}
	catch (std::exception& exception)
	{
		DEBUG_PRINT(exception.what());
		return -1;
	}

	return 0;
}