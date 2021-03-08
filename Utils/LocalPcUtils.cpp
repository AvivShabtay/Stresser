#include "pch.h"
#include "LocalPcUtils.h"

#include "ExceptionWithWin32ErrorCode.h"

std::wstring LocalPcUtils::getLocalComputerName()
{
	std::vector<WCHAR> buffer(MAX_COMPUTERNAME_LENGTH + 1);
	DWORD dwNameLength = MAX_COMPUTERNAME_LENGTH + 1;

	if (!GetComputerName(reinterpret_cast<LPWSTR>(&buffer[0]), &dwNameLength)) {
		throw ExceptionWithWin32ErrorCode("Could not get local computer name");
	}

	return std::wstring(&buffer[0]);
}
