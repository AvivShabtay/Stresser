#include "pch.h"
#include "ArtifactArgs.h"

// TODO

ArtifactArgs::ArtifactArgs()
{
}

ArtifactArgs* ArtifactArgs::AddArg(std::wstring arg) {
	this->args.push_back(arg);
	return this;
}

std::vector<std::wstring>& ArtifactArgs::GetData()
{
	return this->args;
}
