#include "FileArtifact.h"

FileArtifact::FileArtifact(const std::string& name, const  std::string& data) : IArtifact(name, data)
{
	this->install();
}

FileArtifact::~FileArtifact()
{
	this->uninstall();
}

void FileArtifact::install()
{
	const std::string fileName = this->data;

	const AutoHandle hFile(CreateFileA(
		fileName.c_str(),		// Open file
		GENERIC_WRITE, 			// Open for writing
		0, 						// Do not share
		nullptr, 					// Default security
		OPEN_ALWAYS, 			// Open or create
		FILE_ATTRIBUTE_NORMAL, 	// Normal file
		nullptr
	));

	if (hFile.get() == INVALID_HANDLE_VALUE)
	{
		throw Win32ErrorCodeException("Failed to create a File!");
	}
}

void FileArtifact::uninstall()
{
	const std::string fileName = this->data;

	if (!DeleteFileA(fileName.c_str()))
	{
		throw Win32ErrorCodeException("Failed to delete a File!");
	}
}
