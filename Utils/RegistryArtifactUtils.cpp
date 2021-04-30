#include "pch.h"
#include "RegistryArtifactUtils.h"

const std::map<std::string, HKEY> RegistryArtifactUtils::REGISTRY_PREFIX_DICTIONARY = {
	{ "HKLM", HKEY_LOCAL_MACHINE },
	{ "HKCR", HKEY_CLASSES_ROOT },
	{ "HKU", HKEY_USERS },
	{ "HKCU", HKEY_CURRENT_USER }
};

HKEY RegistryArtifactUtils::getRegistryPrefix(std::string regKey)
{
    std::string regPrefix = regKey.substr(0, regKey.find_first_of("\\"));
    return REGISTRY_PREFIX_DICTIONARY.at(regPrefix);
}

std::string RegistryArtifactUtils::getRegistrySubKey(std::string regKey)
{
	return regKey.substr(regKey.find_first_of("\\") + 2);
}
