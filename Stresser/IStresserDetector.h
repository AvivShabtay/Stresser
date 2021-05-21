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

	/* Start the detector */
	virtual void start() = 0;

	/* Stop the detector */
	virtual void stop() = 0;

protected:
	/*
		send event to the server.
		@param eventEntity - Event to send.
	 */
	void sendEvent(const EventEntity& eventEntity) const;

private:
	const EventController& m_eventController;
};
