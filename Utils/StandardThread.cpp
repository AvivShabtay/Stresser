#include "pch.h"
#include "StandardThread.h"
#include "DebugPrint.h"

StandardThread::StandardThread(std::function<void()> threadFunction)
	: m_thread(threadFunction)
{
}

StandardThread::StandardThread(std::function<void(LPVOID params)> threadFunction, LPVOID params)
	: m_thread(threadFunction, params)
{
}

StandardThread::~StandardThread()
{
	try
	{
		this->stop();
	}
	catch (...)
	{
		DEBUG_PRINT("Exception was thrown in ThreadWrapper destructor");
	}
}

StandardThread::StandardThread(StandardThread&& other)
	: m_thread(std::move(other.m_thread))
{
}

StandardThread& StandardThread::operator=(StandardThread&& other)
{
	this->stop();

	this->m_thread = std::move(other.m_thread);
	return *this;
}

void StandardThread::reset(std::function<void(LPVOID params)> threadFunction, LPVOID params)
{
	this->stop();
	this->m_thread = std::move(std::thread(threadFunction, params));
}

void StandardThread::release()
{
	this->stop();
}

void StandardThread::stop()
{
	if (this->m_thread.joinable())
	{
		this->m_thread.join();
	}
}
