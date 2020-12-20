#pragma once
#ifndef __FAKE_FILE_H
#define __FAKE_FILE_H

#include "FakeArtifact.h"

class FakeFile : public FakeArtifact
{
private:
	std::vector<std::wstring> filePaths;
public:
	FakeFile(ArtifactType::Type type, ArtifactArgs args) : FakeArtifact(type, args) {}
	~FakeFile();
	virtual bool Install() override;
	virtual bool Uninstall() override;
};


#endif // !__FAKE_FILE_H