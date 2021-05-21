#pragma once

#include "IStresserDetector.h"

#include <memory>

enum class DetectorType
{
	Registry,
	File,
	Process
};

class DetectorFactory
{
public:
	static std::shared_ptr<IStresserDetector> buildDetector(DetectorType type, const EventController& eventController);

private:
	DetectorFactory() = default;
};

