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
	/*
		Return if the system setting: testsigning enabled.
		Taken from here:
			https://social.msdn.microsoft.com/Forums/Windowsapps/en-US/e6c1be93-7003-4594-b8e4-18ab4a75d273/
			detecting-testsigning-onoff-via-api?forum=windowsgeneraldevelopmentissues
	*/
	static bool doesTestSigningEnabled();

	static std::wstring getLocalComputerIp();
};

