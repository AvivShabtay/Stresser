#pragma once

#include "../Utils/AutoHandle.h"

#include <Windows.h>

const std::wstring DEVICE_NAME = L"\\\\.\\StresserEngine";

class KernelDetector
{
public:
	KernelDetector();

	~KernelDetector();

	// Delete copyable, movable:
	KernelDetector(const KernelDetector&) = delete;
	KernelDetector& operator=(const KernelDetector&) = delete;
	KernelDetector(KernelDetector&&) = delete;
	KernelDetector& operator=(KernelDetector&&) = delete;

	void addFakeProcessId(ULONG processId) const;

	void removeFakeProcessId(ULONG processId) const;

	//void registerEvent(HANDLE eventObject) const;

private:
	AutoHandle stresserDevice;
};

