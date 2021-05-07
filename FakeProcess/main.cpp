#include "../Utils/AutoHandle.h"
#include "../Utils/EventsNames.h"
#include "../Utils/SehTranslatorGuard.h"

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
	try
	{
		SehTranslatorGuard sehTranslatorGuard;
		
		AutoHandle events[2]{
			AutoHandle(OpenEvent(SYNCHRONIZE, FALSE, CHANGE_POLICY)),
			AutoHandle(OpenEvent(SYNCHRONIZE, FALSE, STOP_STRESSER))
		};

		HANDLE hEvents[]{ events[0].get(), events[1].get() };

		WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
	}
	catch (...)
	{
	}

    return 0;
}