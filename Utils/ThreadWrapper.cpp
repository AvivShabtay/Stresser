#include "pch.h"
#include "ThreadWrapper.h"
#include "DebugPrint.h"

ThreadWrapper::ThreadWrapper(std::function<void(LPVOID params)> threadFunction, LPVOID params)
	: m_thread(threadFunction, params)
{
}

ThreadWrapper::~ThreadWrapper()
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

ThreadWrapper::ThreadWrapper(ThreadWrapper&& other)
	: m_thread(std::move(other.m_thread))
{
}

ThreadWrapper& ThreadWrapper::operator=(ThreadWrapper&& other)
{
	this->stop();

	this->m_thread = std::move(other.m_thread);
	return *this;
}

void ThreadWrapper::reset(std::function<void(LPVOID params)> threadFunction, LPVOID params)
{
	this->stop();
	this->m_thread = std::move(std::thread(threadFunction, params));
}

void ThreadWrapper::stop()
{
	if (this->m_thread.joinable())
	{
		this->m_thread.join();
	}
}
