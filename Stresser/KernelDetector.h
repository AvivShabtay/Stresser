#pragma once

#include "IStresserDetector.h"
#include "IArtifactSubscriber.h"

class KernelDetector : public IStresserDetector, public IArtifactSubscriber
{
public:
	KernelDetector(const EventController& eventController);

	// Disable: copyable, assignable, movable:
	KernelDetector(KernelDetector&) = delete;
	KernelDetector& operator=(KernelDetector&) = delete;
	KernelDetector(KernelDetector&&) = delete;
	KernelDetector& operator=(KernelDetector&&) = delete;

	virtual ~KernelDetector();

	/* Start the detector */
	void start() override;

	/* Stop the detector */
	void stop() override;

private:

};

