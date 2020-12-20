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
	ArtifactArgs();
	ArtifactArgs(std::vector<std::wstring> args) : args(args) {}
	ArtifactArgs* AddArg(std::wstring arg);
	std::vector<std::wstring>& GetData();
};

#endif // !__ARTIFACT_ARGS_H


