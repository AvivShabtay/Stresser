#include "ProcessArtifact.h"

ProcessArtifact::ProcessArtifact(const std::string& name, const std::string& data) : IArtifact(name, data)
{
    std::wstring tempPath = LocalPcUtils::getTempPath();
    std::wstring wideFileName = std::wstring(ATL::CA2W(data.c_str()));

    this->filePath = std::wstring(tempPath + wideFileName);
    this->pid = 0;
	this->install();
}

ProcessArtifact::~ProcessArtifact()
{
	this->uninstall();
}

void ProcessArtifact::install()
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

    AutoHandle autoHProcess(processInfo.hProcess); 
    AutoHandle autoThreadHandle(processInfo.hThread);
    this->pid = processInfo.dwProcessId;
}

void ProcessArtifact::uninstall()
{
    const AutoHandle autoProcess(OpenProcess(SYNCHRONIZE, FALSE, this->pid));
	if (nullptr != autoProcess.get())
	{
        WaitForSingleObject(autoProcess.get(), PROCESS_TERMINATE_WAIT_TIME);
	}
	
    if (!DeleteFile(this->filePath.c_str()))
    {
	    throw Win32ErrorCodeException("Failed to delete the fake process file!");
    }
}
