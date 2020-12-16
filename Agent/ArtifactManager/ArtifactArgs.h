#pragma once
#ifndef __ARTIFACT_ARGS_H
#define __ARTIFACT_ARGS_H

#include <vector>
#include <string>

class ArtifactArgs
{
private:
	std::vector<std::wstring> args;

public:
	std::vector<std::wstring>& GetData();
};

#endif // !__ARTIFACT_ARGS_H


