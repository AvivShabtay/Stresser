#include "KernelDetector.h"

KernelDetector::KernelDetector(const EventController& eventController)
	: IStresserDetector(eventController), IArtifactSubscriber(ArtifactTypes::Process)
{

}

KernelDetector::~KernelDetector()
{
}

void KernelDetector::start()
{

}

void KernelDetector::stop()
{

}
