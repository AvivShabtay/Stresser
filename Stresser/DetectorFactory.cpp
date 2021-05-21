#include "DetectorFactory.h"

std::shared_ptr<IStresserDetector> DetectorFactory::buildDetector(DetectorType type, const EventController& eventController)
{
	switch (type)
	{
	case DetectorType::File:
	{
		// TODO
		return {};
	}
	case DetectorType::Registry:
	{
		// TODO
		return {};
	}
	case DetectorType::Process:
	{
		// TODO
		return {};
	}
	}

	throw std::runtime_error("Invalid detector type");
}
