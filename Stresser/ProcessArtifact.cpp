#include "ProcessArtifact.h"

ProcessArtifact::ProcessArtifact(std::string name, std::string data) : IArtifact(name, data)
{
    this->filePath = ProcessArtifactUtils::getFakeFilePath(data);
	this->install();
}

ProcessArtifact::~ProcessArtifact()
{
	this->uninstall();
}

void ProcessArtifact::install() const
{  
    PeResource fakeProcess(FAKE_PROCESS, L"BIN");

    fakeProcess.saveResourceToFileSystem(this->filePath);

    ProcessArtifactUtils::createFakeProcess(this->filePath);
}

void ProcessArtifact::uninstall() const
{
    if (!DeleteFile(this->filePath.c_str()))
        throw Win32ErrorCodeException("Failed to delete the fake process file!");
}
