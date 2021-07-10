#include "AutoSetLostConnectionEvent.h"

#include "../Utils/EventsNames.h"
#include "../Utils/WindowsEvent.h"
#include "../Utils/DebugPrint.h"
#include "../Utils/StringUtils.h"

AutoSetLostConnectionEvent::AutoSetLostConnectionEvent()
{
	try
	{
		WindowsEvent lostConnectionEvent;
		lostConnectionEvent.open(LOST_CONNECTION_EVENT_NAME);
		lostConnectionEvent.setEvent();
	}
	catch (const std::exception& exception)
	{
		DEBUG_TRACE(AutoSetLostConnectionEvent, "Exception when trying to open: ", StringUtils::wstringToString(LOST_CONNECTION_EVENT_NAME),
			" connection: ", exception.what());

		throw exception;
	}
}
