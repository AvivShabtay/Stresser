#include "RegistryArtifact.h"

RegistryArtifact::RegistryArtifact(const std::string& name, const std::string& data) : IArtifact(name, data)
{
	this->registryPrefix = RegistryArtifactUtils::getRegistryPrefix(this->data);
	this->registrySubKey = RegistryArtifactUtils::getRegistrySubKey(this->data);
	this->install();
}

RegistryArtifact::~RegistryArtifact()
{
	this->uninstall();
}

void RegistryArtifact::install()
{
	HKEY hKey;

	LSTATUS createRegStatus = RegCreateKeyExA(
		this->registryPrefix, 			// Registry key
		this->registrySubKey.c_str(), 	// Sub key
		0, 								// Reserved and must be 0
		nullptr, 							// Class type of the key
		REG_OPTION_NON_VOLATILE,		// Keep the key after reboot
		KEY_WRITE, 						// Registry key security and access right
		nullptr, 							// Security attributes
		&hKey, 							// Handle to the opened key
		nullptr 							// Determine either the key exists or not
	);

	if (ERROR_SUCCESS != createRegStatus)
	{
		throw Win32ErrorCodeException("Failed to create registry key!");
	}
	
	AutoRegistryKeyHandle autoHKey(hKey);
}

void RegistryArtifact::uninstall()
{
	LSTATUS result = RegDeleteKeyA(this->registryPrefix, this->registrySubKey.c_str());

	if (ERROR_SUCCESS != result)
	{
		throw Win32ErrorCodeException("Failed to delete the registry key!");
	}
}
