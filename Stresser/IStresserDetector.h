#pragma once

#include "EventController.h"

/*
	Represent detector class in the application.
	Using this layer divide between the communication application and the detection application.
 */
class IStresserDetector
{
public:
	explicit IStresserDetector(const EventController& eventController);

	virtual ~IStresserDetector() = default;

	/*
		send event to the server.
		@param eventEntity - Event to send.
	 */
	void sendEvent(const EventEntity& eventEntity) const;

private:
	const EventController& m_eventController;
};
