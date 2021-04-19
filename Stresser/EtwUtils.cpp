#include "EtwUtils.h"

std::map<UCHAR, std::wstring> EtwUtils::getFileIoNameEventTypes()
{
	std::map<UCHAR, std::wstring> types;

	types.insert({ 0, L"Name" });
	types.insert({ 32, L"FileCreate" });
	types.insert({ 35, L"FileDelete" });
	types.insert({ 36, L"FileRundown" });

	return types;
}

std::map<UCHAR, std::wstring> EtwUtils::getFileIoDirectoryEnumerationEventTypes()
{
	std::map<UCHAR, std::wstring> types;

	types.insert({ 72, L"DirEnum" });
	types.insert({ 77, L"DirNotify" });

	return types;
}
