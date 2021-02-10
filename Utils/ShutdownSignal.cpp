#include "pch.h"
#include "ShutdownSignal.h"

ShutdownSignal::ShutdownSignal(std::wstring signalName)
	:m_eventName(signalName) {

	AutoHandle hEvent(::CreateEvent(nullptr, TRUE, FALSE, this->m_eventName.c_str()));
	this->ahShutdownEvent = std::move(hEvent);
}

ShutdownSignal& ShutdownSignal::GetInstance(const std::wstring signalName) {
	// Keep static instance of this class in order to prevent
	// new creations of this class:
	static ShutdownSignal g_instance(signalName);
	return g_instance;
}

ShutdownSignal::~ShutdownSignal() {
	if (this->ahShutdownEvent.get()) {
		::SetEvent(this->ahShutdownEvent.get());
	}
}

const HANDLE ShutdownSignal::Get() {
	return const_cast<HANDLE>(this->ahShutdownEvent.get());
}
