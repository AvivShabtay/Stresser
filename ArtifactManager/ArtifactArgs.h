#pragma once

#include <vector>
#include <string>

class ArtifactArgs
{

public:
	ArtifactArgs();
	ArtifactArgs(std::vector<std::wstring> args) : m_args(args) {}

	/* Add new argument. */
	ArtifactArgs* AddArg(std::wstring arg);

	/* Get all the arguments. */
	std::vector<std::wstring>& GetData();

private:
	std::vector<std::wstring> m_args;
};
