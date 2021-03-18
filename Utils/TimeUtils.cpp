#include "pch.h"
#include "TimeUtils.h"

std::string TimeUtils::GetCurrentDateTime() {
	std::time_t tt = std::time(nullptr);
	CHAR buffer[35];
	ctime_s(buffer, 35, &tt);
	return std::string(buffer);
}
