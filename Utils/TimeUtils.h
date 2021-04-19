#pragma once

#include <string>

class TimeUtils final
{
public:
	static std::string GetCurrentDateTime();

	static std::wstring systemTimeToTimestamp(LARGE_INTEGER systemTimeValue);
};

