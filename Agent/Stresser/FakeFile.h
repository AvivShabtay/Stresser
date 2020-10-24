#pragma once
#ifndef __FAKE_FILE_H
#define __FAKE_FILE_H

#include "FakeArtifact.h"

class FakeFile : public FakeArtifact
{
public:
	FakeFile(ArtifactType type, ArtifactArgs args) : FakeArtifact(type, args) {}
	virtual bool Install() const override;
	virtual bool Uninstall() const override;
};


#endif // !__FAKE_FILE_H