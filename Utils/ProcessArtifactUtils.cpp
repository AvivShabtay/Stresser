#include "pch.h"
#include "ProcessArtifactUtils.h"

std::wstring ProcessArtifactUtils::getFakeFilePath(std::string fileName)
{
    wchar_t tempPath[MAX_PATH + 1];
    if (GetTempPath(MAX_PATH + 1, tempPath) == 0)
        throw Win32ErrorCodeException("Can't retrive the temp directory path!");

    std::wstring wideFileName = StringUtils::ConvertToWideString(fileName);

    return std::wstring(tempPath + wideFileName);
}

void ProcessArtifactUtils::createFakeProcess(std::wstring filePath)
{
    STARTUPINFO info = { sizeof(info) };
    PROCESS_INFORMATION processInfo;
    BOOL isProcessCreated = CreateProcess(
        filePath.c_str(),
        0,
        0,
        0,
        TRUE,
        0,
        0,
        0,
        &info,
        &processInfo
    );

    if (!isProcessCreated)
        throw Win32ErrorCodeException("Failed to create a process!");

    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
}
