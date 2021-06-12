#pragma once
class IStresserApplication
{
public:
	virtual ~IStresserApplication() = default;

	virtual void start() = 0;

	virtual void stop() = 0;

	virtual void pause() = 0;

	virtual void waitForShutdown() = 0;
};
