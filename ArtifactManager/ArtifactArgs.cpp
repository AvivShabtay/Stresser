#include "pch.h"
#include "ArtifactArgs.h"

ArtifactArgs::ArtifactArgs()
{
}

ArtifactArgs* ArtifactArgs::AddArg(std::wstring arg) {
	this->m_args.push_back(arg);
	return this;
}

std::vector<std::wstring>& ArtifactArgs::GetData()
{
	return this->m_args;
}
