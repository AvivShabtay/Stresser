#include "ProcessArtifact.h"

ProcessArtifact::ProcessArtifact(const std::string& name, const std::string& data) : IArtifact(name, data)
{
    std::wstring tempPath = LocalPcUtils::getTempPath();
    std::wstring wideFileName = std::wstring(ATL::CA2W(name.c_str()));

    this->filePath = std::wstring(tempPath + wideFileName);
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

    STARTUPINFO info = { sizeof(info) };
    PROCESS_INFORMATION processInfo{ nullptr, nullptr, 0, 0 };
    if (!CreateProcess(
        this->filePath.c_str(),
        nullptr,
        nullptr,
        nullptr,
        TRUE,
        0,
        nullptr,
        nullptr,
        &info,
        &processInfo
    ))
    {
        throw Win32ErrorCodeException("Failed to create a process!");
    }

    AutoHandle autoProcessHandle(processInfo.hProcess);
    AutoHandle autoThreadHandle(processInfo.hThread);
}

void ProcessArtifact::uninstall() const
{
    Sleep(2000);
    if (!DeleteFile(this->filePath.c_str()))
    {
	    throw Win32ErrorCodeException("Failed to delete the fake process file!");
    }
}
