#pragma once

#include <thread>
#include <functional>

#include <Windows.h>

class ThreadWrapper
{
public:
	ThreadWrapper() = default;
	explicit ThreadWrapper(std::function<void(LPVOID params)> threadFunction, LPVOID params);

	virtual ~ThreadWrapper();

	ThreadWrapper(const ThreadWrapper&) = delete;
	ThreadWrapper& operator=(const ThreadWrapper&) = delete;

	ThreadWrapper(ThreadWrapper&& other);
	ThreadWrapper& operator=(ThreadWrapper&& other);

	void reset(std::function<void(LPVOID params)> threadFunction, LPVOID params);

private:
	void stop();

	std::thread m_thread;
};

