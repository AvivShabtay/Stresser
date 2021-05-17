#pragma once

#include <string>

class TimeUtils final
{
public:
	static std::string GetCurrentDateTime();

	/*
		Convert system time value to local system time depending the given format.
		@note Formatting codes can be found here:
		https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strftime-wcsftime-strftime-l-wcsftime-l?view=msvc-160#remarks
	 */
	static std::wstring systemTimeToTimestamp(LARGE_INTEGER systemTimeValue, std::wstring format = L"%c");
};

