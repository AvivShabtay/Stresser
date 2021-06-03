#pragma once

#include <string>

//std::string TIMESTAMP_FORMAT("%d-%m-%Y %H:%M:%S");

/* https://www.boost.org/doc/libs/1_49_0/doc/html/date_time/date_time_io.html */
//std::string SHORT_TIMESTAMP_FORMAT("%x %X");

class TimeUtils final
{
public:
	static std::string GetCurrentDateTime();

	static std::string getCurrentTimestamp(std::string format = "%x %X");

	/*
		Convert system time value to local system time depending the given format.
		@note Formatting codes can be found here:
		https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strftime-wcsftime-strftime-l-wcsftime-l?view=msvc-160#remarks
	 */
	static std::wstring systemTimeToTimestamp(LARGE_INTEGER systemTimeValue, std::wstring format = L"%c");
};

