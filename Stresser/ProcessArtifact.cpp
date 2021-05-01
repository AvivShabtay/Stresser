#include "ProcessArtifact.h"

ProcessArtifact::ProcessArtifact(const std::string& name, const std::string& data) : IArtifact(name, data)
{
	const std::wstring tempPath = LocalPcUtils::getTempPath();
    const std::wstring wideFileName = StringUtils::stringToWString(data);

    this->m_filePath = std::wstring(tempPath + wideFileName);
    this->m_pid = INVALID_PID;
	this->ProcessArtifact::install();
}

ProcessArtifact::~ProcessArtifact()
{
	this->ProcessArtifact::uninstall();
}

void ProcessArtifact::install()
{  
    const PeResource fakeProcess(FAKE_PROCESS, L"BIN");

    fakeProcess.saveResourceToFileSystem(this->m_filePath);

    STARTUPINFO info = { sizeof(info) };
    PROCESS_INFORMATION processInfo{ nullptr, nullptr, 0, 0 };
    if (!CreateProcess(
        this->m_filePath.c_str(),
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

    AutoHandle hProcess(processInfo.hProcess); 
    AutoHandle hThread(processInfo.hThread);
    this->m_pid = processInfo.dwProcessId;
}

void ProcessArtifact::uninstall()
{
	if (INVALID_PID == this->m_pid)
	{
        throw Win32ErrorCodeException("Invalid PID");
	}

    // SYNCHRONIZE for WaitForSingleObject - when the wait stop the process is terminated
    // The process need to be terminated before we try to delete his file from the disk
    const AutoHandle process(OpenProcess(SYNCHRONIZE, FALSE, this->m_pid));
	if (nullptr != process.get())
	{
        WaitForSingleObject(process.get(), PROCESS_TERMINATE_WAIT_TIME);
	}
	
    if (!DeleteFile(this->m_filePath.c_str()))
    {
	    throw Win32ErrorCodeException("Failed to delete the fake process file!");
    }
}
