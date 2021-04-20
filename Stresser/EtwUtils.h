#pragma once

#include <string>
#include <map>

#include <Windows.h>

class EtwUtils final
{
public:
	static std::map<UCHAR, std::wstring> getFileIoNameEventTypes();

	static std::map<UCHAR, std::wstring> getFileIoDirectoryEnumerationEventTypes();
};

