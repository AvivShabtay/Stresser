#include "pch.h"
#include "FakeRegistry.h"

FakeRegistry::~FakeRegistry() { }

bool FakeRegistry::Install()
{
	// Get the arguments:
	auto args = this->GetArgs();

	// For each given argument, create registry key:
	for (auto& regValuePath : args.GetData()) {

		wil::unique_hkey hKey;

		// Create the fake key in the OS:
		::RegCreateKeyEx(
			HKEY_LOCAL_MACHINE, 	// Registry key
			regValuePath.c_str(), 	// Sub key
			0, 						// Reserved and must be 0
			NULL, 					// Class type of the key
			REG_OPTION_NON_VOLATILE,// Keep the key after reboot
			KEY_WRITE, 				// Registry key security and access right
			NULL, 					// Security attributes
			hKey.addressof(), 		// Handle to the opened key
			NULL 					// Determine either the key exists or not
		);

		// Save the key for latter usage:
		if (hKey) {
			this->registryPaths.push_back(regValuePath.c_str());
			std::wcout
				<< "[FAKE_REGISTRY] install registry: "
				<< regValuePath
				<< std::endl;
		}
		else {
			// Installation Failed:
			// TODO: Release previous handles.
			std::wcout
				<< "[FAKE_REGISTRY] Couldn't install registry: "
				<< regValuePath
				<< " Error: "
				<< GetLastError()
				<< std::endl;
			return false;
		}
	}
	// Installation successfully:
	return true;
}

bool FakeRegistry::Uninstall() {
	for (auto& regKeyPath : this->registryPaths) {
		auto result = ::RegDeleteKey(HKEY_LOCAL_MACHINE, regKeyPath.c_str());
		if (result != ERROR_SUCCESS) {
			std::wcout
				<< "[FAKE_REGISTRY] couldn't uninstall registry: "
				<< regKeyPath
				<< std::endl;
			return false;
		}
		std::wcout
			<< "[FAKE_REGISTRY] uninstall registry: "
			<< regKeyPath
			<< std::endl;
	}
	return true;
}