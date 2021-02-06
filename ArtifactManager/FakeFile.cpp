#include "pch.h"
#include "FakeFile.h"

FakeFile::~FakeFile() { }

bool FakeFile::Install() {
	auto args = this->GetArgs();

	for (auto& fileName : args.GetData()) {

		wil::unique_handle hFile(::CreateFile(
			fileName.c_str(),		// Open file
			GENERIC_WRITE, 			// Open for writing
			0, 						// Do not share
			NULL, 					// Default security
			OPEN_ALWAYS, 			// Open or create
			FILE_ATTRIBUTE_NORMAL, 	// Normal file
			NULL
		));

		// Save the filename for uninstall operation:
		if (hFile) {
			this->m_filePaths.push_back(fileName.c_str());
			std::wcout
				<< "[FAKE_FILE] install file: "
				<< fileName
				<< std::endl;
		}
		else {
			// Installation Failed:
			// TODO: Release previous handles.
			std::wcout
				<< "[FAKE_FILE] couldn't install file: "
				<< fileName
				<< " Error: "
				<< GetLastError()
				<< std::endl;
			return false;
		}
	}
	return true;
}

bool FakeFile::Uninstall() {

	for (auto& fakeFile : this->m_filePaths) {
		bool deleteResult = ::DeleteFile(fakeFile.c_str());
		if (!deleteResult) {
			std::wcout
				<< "[FAKE_FILE] couldn't uninstall file: "
				<< fakeFile
				<< std::endl;
			return false;
		}
		std::wcout
			<< "[FAKE_FILE] uninstall file: "
			<< fakeFile
			<< std::endl;
	}
	return true;
}