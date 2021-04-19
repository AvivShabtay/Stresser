#pragma once

#include <thread>
#include <functional>

#include <Windows.h>

/*
 * Wrapper for std::thread.
 */
class StandardThread
{
public:
	StandardThread() = default;
	explicit StandardThread(std::function<void()> threadFunction);
	explicit StandardThread(std::function<void(LPVOID params)> threadFunction, LPVOID params);

	virtual ~StandardThread();

	StandardThread(const StandardThread&) = delete;
	StandardThread& operator=(const StandardThread&) = delete;

	StandardThread(StandardThread&& other);
	StandardThread& operator=(StandardThread&& other);

	/* Stop the current running thread and start a new one. */
	void reset(std::function<void(LPVOID params)> threadFunction, LPVOID params);

	/* Stop the current running thread. */
	void release();

private:
	/* Signal the current thread to stop. */
	void stop();

	std::thread m_thread;
};

