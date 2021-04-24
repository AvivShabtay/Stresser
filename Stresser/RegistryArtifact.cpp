#include "RegistryArtifact.h"

RegistryArtifact::RegistryArtifact(std::string name, std::string data) : IArtifact(name, data)
{
	this->registryPrefix = RegistryArtifactUtils::getRegistryPrefix(this->data);
	this->registrySubKey = RegistryArtifactUtils::getRegistrySubKey(this->data);
	this->install();
}

RegistryArtifact::~RegistryArtifact()
{
	this->uninstall();
}

void RegistryArtifact::install() const
{
	HKEY hKey;
	AutoRegistryKeyHandle autoHKey(hKey);

	auto createRegStatus = RegCreateKeyExA(
		this->registryPrefix, 			// Registry key
		this->registrySubKey.c_str(), 	// Sub key
		0, 								// Reserved and must be 0
		NULL, 							// Class type of the key
		REG_OPTION_NON_VOLATILE,		// Keep the key after reboot
		KEY_WRITE, 						// Registry key security and access right
		NULL, 							// Security attributes
		&hKey, 							// Handle to the opened key
		NULL 							// Determine either the key exists or not
	);

	if (createRegStatus != ERROR_SUCCESS) 
		throw Win32ErrorCodeException("Failed to create registry key!");
}

void RegistryArtifact::uninstall() const
{
	auto result = RegDeleteKeyA(this->registryPrefix, this->registrySubKey.c_str());

	if (result != ERROR_SUCCESS) 
		throw Win32ErrorCodeException("Failed to delete the registry key!");
}
