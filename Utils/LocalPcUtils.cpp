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
