#include "FileArtifact.h"

FileArtifact::FileArtifact(std::string name, std::string data) : IArtifact(name, data)
{
	this->install();
}

FileArtifact::~FileArtifact()
{
	this->uninstall();
}

void FileArtifact::install() const
{
	std::string fileName = this->data;

	AutoHandle hFile(CreateFileA(
		fileName.c_str(),		// Open file
		GENERIC_WRITE, 			// Open for writing
		0, 						// Do not share
		NULL, 					// Default security
		OPEN_ALWAYS, 			// Open or create
		FILE_ATTRIBUTE_NORMAL, 	// Normal file
		NULL
	));

	if (!hFile.get())
		throw Win32ErrorCodeException("Failed to create a File!");
}

void FileArtifact::uninstall() const
{
	std::string fileName = this->data;

	bool deleteResult = DeleteFileA(fileName.c_str());

	if (!deleteResult)
		throw Win32ErrorCodeException("Failed to delete a File!");
}
