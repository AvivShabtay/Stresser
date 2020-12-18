#pragma once
#ifndef __FAKE_ARTIFACT_H
#define __FAKE_ARTIFACT_H

/*
* Represent the base class of the Command pattern.
* Each concrete class provide logic to install fake artifact from specific type,
* while all the concrete class provide interface to the callee to do / redo the installation process.
*/
class FakeArtifact {
private:
	ArtifactType::Type Type;
	ArtifactArgs Args;

public:
	FakeArtifact(ArtifactType::Type type, ArtifactArgs args) : Type(type), Args(args) {}
	ArtifactType::Type GetType();
	ArtifactArgs GetArgs();

	// Interface functions:
	virtual bool Install() = 0;
	virtual bool Uninstall() = 0;
};

#endif // !__FAKE_ARTIFACT_H