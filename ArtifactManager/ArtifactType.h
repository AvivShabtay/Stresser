#pragma once

class ArtifactType {
public:
	enum class Type {
		Registry,
		File,
		Process,
		Service
	};

	/* Convert from ArtifactType to WString represent the type. */
	static std::wstring ArtifactTypeToWString(Type type);
};