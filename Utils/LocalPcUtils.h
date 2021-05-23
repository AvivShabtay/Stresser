#pragma once

#include <string>

/*
 * Include helper methods to query and preform actions on the local PC.
 */
class LocalPcUtils final
{
public:
	/* Return the local computer name in string (ASCII) format. */
	static std::wstring getLocalComputerName();

	static bool doesFileExists(const std::wstring& path);

	static std::wstring getTempPath();

	static std::wstring getDosNameFromNtName(const std::wstring& ntPath);
};

