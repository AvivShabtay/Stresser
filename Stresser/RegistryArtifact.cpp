#include "RegistryArtifact.h"

RegistryArtifact::RegistryArtifact(const std::string& name, const std::string& data) : IArtifact(name, data)
{
	this->m_registryPrefix = RegistryArtifactUtils::getRegistryPrefix(this->m_data);
	this->m_registrySubKey = RegistryArtifactUtils::getRegistrySubKey(this->m_data);
	this->RegistryArtifact::install();
}

RegistryArtifact::~RegistryArtifact()
{
	this->RegistryArtifact::uninstall();
}

void RegistryArtifact::install()
{
	HKEY hKey;

	LSTATUS createRegStatus = RegCreateKeyExA(
		this->m_registryPrefix, 			// Registry key
		this->m_registrySubKey.c_str(), 	// Sub key
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
	if (ERROR_SUCCESS != RegDeleteKeyA(this->m_registryPrefix, this->m_registrySubKey.c_str()))
	{
		throw Win32ErrorCodeException("Failed to delete the registry key!");
	}
}
