#pragma once

class IEventCollector
{
public:
	virtual ~IEventCollector() = default;

	virtual void start() = 0;

	virtual void stop() = 0;
};