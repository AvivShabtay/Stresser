#include "pch.h"

/* Convert from ArtifactType to WString represent the type. */
std::wstring ArtifactType::ArtifactTypeToWString(Type type) {
	switch (type)
	{
	case Type::Registry:
		return L"REGISTRY";
	case Type::File:
		return L"FILE";
	case Type::Process:
		return L"PROCESS";
	case Type::Service:
		return L"SERVICE";
	default:
		return L"";
	}

}
