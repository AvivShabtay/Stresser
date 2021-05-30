#pragma once

#include "AuthorizedHttpRequest.h"
#include "EventEntity.h"

#include "nlohmann/json.hpp"

using Json = nlohmann::json;

/*
	Used to manage all the communication related to events between the client and the server.
*/
class EventController
{
public:
	// Disable: cloneable, assignable, movable:
	EventController(EventController&) = delete;
	void operator=(const EventController&) = delete;
	EventController(EventController&&) = delete;
	void operator=(const EventController&&) = delete;

	virtual ~EventController() = default;

	/*
	* Return the singleton instance of this class.
	* @param authorizedHttpRequest - HTTP request provider.
	* @return Global instance of this class.
	*/
	static EventController& getInstance(AuthorizedHttpRequest& authorizedHttpRequest);

	/*
	 * Send event to the server.
	 * @param eventEntity - Represent the event.
	 */
	void sendEvent(EventEntity& eventEntity) const;

private:
	explicit EventController(AuthorizedHttpRequest& authorizedHttpRequest);

	AuthorizedHttpRequest& m_authorizedHttpRequest;
	std::string m_ipAddress;
	std::string m_hostname;
};

