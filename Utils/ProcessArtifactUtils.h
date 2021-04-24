#pragma once
#include "Win32ErrorCodeException.h"
#include "StringUtils.h"

class ProcessArtifactUtils
{
public:
	static std::wstring getFakeFilePath(std::string fileName);

	static void createFakeProcess(std::wstring filePath);
};

