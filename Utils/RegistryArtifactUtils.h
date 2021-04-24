#pragma once
#include <map>
#include <Windows.h>

class RegistryArtifactUtils
{
public:
	static HKEY getRegistryPrefix(std::string regKey);

	static std::string getRegistrySubKey(std::string regKey);

private:
	static const std::map<std::string, HKEY> RegistryPrefixDict;
};

