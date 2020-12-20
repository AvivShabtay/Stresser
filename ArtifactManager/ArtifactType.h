#pragma once
#ifndef __ARTIFACT_TYPE_H
#define __ARTIFACT_TYPE_H

class ArtifactType {
public:
	enum class Type {
		Registry,
		File,
		Process,
		Service
	};

	static std::wstring ArtifactTypeToWString(Type type);
};

#endif // !__ARTIFACT_TYPE_H
