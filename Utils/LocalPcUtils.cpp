#include "pch.h"
#include "LocalPcUtils.h"

#include "Win32ErrorCodeException.h"

std::wstring LocalPcUtils::getLocalComputerName()
{
	std::vector<WCHAR> buffer(MAX_COMPUTERNAME_LENGTH + 1);
	DWORD dwNameLength = MAX_COMPUTERNAME_LENGTH + 1;

	if (!GetComputerName(reinterpret_cast<LPWSTR>(&buffer[0]), &dwNameLength)) {
		throw Win32ErrorCodeException("Could not get local computer name");
	}

	return std::wstring(&buffer[0]);
}

bool LocalPcUtils::doesFileExists(const std::wstring& path)
{
	const DWORD fileAttributes = GetFileAttributes(path.c_str());
	return (fileAttributes != INVALID_FILE_ATTRIBUTES) &&
		!(fileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

std::wstring LocalPcUtils::getTempPath()
{
	std::vector<WCHAR> tempPath(MAX_PATH + 1);
	if (GetTempPath(MAX_PATH + 1, reinterpret_cast<LPWSTR>(&tempPath[0])) == 0)
	{
		throw Win32ErrorCodeException("Can't retrieve the temp directory path!");
	}

	return std::wstring(reinterpret_cast<LPWSTR>(&tempPath[0]));
}

std::wstring LocalPcUtils::getDosNameFromNtName(const std::wstring& ntPath)
{
	static std::vector<std::pair<std::wstring, std::wstring>> deviceNames;
	static bool first = true;
	if (first)
	{
		auto drives = GetLogicalDrives();
		size_t drive = 0;
		while (drives)
		{
			if (drives & 1)
			{
				// drive exists
				WCHAR driveName[] = L"X:";
				driveName[0] = (WCHAR)(drive + 'A');
				WCHAR path[MAX_PATH];
				if (QueryDosDevice(driveName, path, MAX_PATH))
				{
					deviceNames.emplace_back(path, driveName);
				}
			}
			drive++;
			drives >>= 1;
		}
		first = false;
	}

	for (auto& deviceName : deviceNames)
	{
		if (ntPath.find(deviceName.first) != std::wstring::npos)
		{
			return deviceName.second + (ntPath.c_str() + deviceName.first.size());
		}
	}

	return L"";
}

