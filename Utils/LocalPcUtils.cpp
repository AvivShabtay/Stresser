#include "pch.h"
#include "LocalPcUtils.h"

#include "StringUtils.h"
#include "Win32ErrorCodeException.h"

#include <boost/asio/ip/udp.hpp>
#include <boost/asio/io_service.hpp>


#include <winternl.h>
#pragma comment(lib, "ntdll.lib")

namespace net = boost::asio;        // from <boost/asio.hpp>
using udp = net::ip::udp;

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
	if (deviceNames.empty())
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
	}

	for (auto& deviceName : deviceNames)
	{
		if (ntPath.find(deviceName.first) != std::wstring::npos)
		{
			return deviceName.second + (ntPath.c_str() + deviceName.first.size());
		}
	}

	throw std::runtime_error("could not find relevant DOS path");
}

bool LocalPcUtils::doesTestSigningEnabled()
{
	SYSTEM_CODEINTEGRITY_INFORMATION systemCodeIntegrityInformation = { 0 };
	systemCodeIntegrityInformation.Length = sizeof(systemCodeIntegrityInformation);

	ULONG returnLength = 0;
	const NTSTATUS status = NtQuerySystemInformation(
		SystemCodeIntegrityInformation,
		&systemCodeIntegrityInformation,
		sizeof(systemCodeIntegrityInformation),
		&returnLength
	);

	if (NT_SUCCESS(status) && returnLength == sizeof(systemCodeIntegrityInformation))
	{
		// Note that testsigning will play no role if bit CODEINTEGRITY_OPTION_ENABLED (or 0x1)
		// is not set in sci.CodeIntegrityOptions
		const bool doesTestSignEnabled = (systemCodeIntegrityInformation.CodeIntegrityOptions & CODEINTEGRITY_OPTION_TESTSIGN);
		return doesTestSignEnabled;
	}

	return false;
}
std::wstring LocalPcUtils::getLocalComputerIp()
{
	try {
		boost::asio::io_service netService;
		udp::endpoint ep(net::ip::address::from_string("8.8.8.8"), 53);
		udp::socket socket(netService);
		socket.connect(ep);
		boost::asio::ip::address addr = socket.local_endpoint().address();
		return StringUtils::stringToWString(addr.to_string());
	}
	catch (std::exception& e) {
		std::cerr << "Could not deal with socket. Exception: " << e.what() << std::endl;

	}

	throw std::exception("Cant get local computer ip");
}

