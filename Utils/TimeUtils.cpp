#include "pch.h"
#include "TimeUtils.h"

#include <boost/date_time.hpp>

#include <ctime>

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

std::string TimeUtils::getCurrentTimestamp(std::string format)
{
	const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	const std::tm timestamp = boost::posix_time::to_tm(now);

	std::stringstream stream;
	stream << std::put_time(&timestamp, format.c_str());

	return stream.str();
}

std::wstring TimeUtils::systemTimeToTimestamp(LARGE_INTEGER systemTimeValue, std::wstring format)
{
	// QuadPart representing the LARGE_INTEGER data:
	auto* const systemTime = reinterpret_cast<FILETIME*>(&systemTimeValue.QuadPart);

	// Converting the
	const CTime time(*systemTime);
	return std::wstring(time.Format(format.c_str()));
}
