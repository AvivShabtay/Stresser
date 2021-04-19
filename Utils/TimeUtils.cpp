#include "pch.h"
#include "TimeUtils.h"

// Add before any ATL header:
// https://social.msdn.microsoft.com/Forums/vstudio/en-US/371a069b-f428-47b5-8f7b-e9ad85964bce/guidnull-undeclared-identifier?forum=vclanguage
#include <cguid.h>

#include <atlbase.h>
#include <atltime.h>

std::string TimeUtils::GetCurrentDateTime() {
	std::time_t tt = std::time(nullptr);
	CHAR buffer[35];
	ctime_s(buffer, 35, &tt);
	return std::string(buffer);
}

std::wstring TimeUtils::systemTimeToTimestamp(LARGE_INTEGER systemTimeValue)
{
	// QuadPart representing the LARGE_INTEGER data:
	auto* const systemTime = reinterpret_cast<FILETIME*>(&systemTimeValue.QuadPart);

	// Converting the
	const CTime time(*systemTime);
	return std::wstring(time.Format(L"%c"));
}
