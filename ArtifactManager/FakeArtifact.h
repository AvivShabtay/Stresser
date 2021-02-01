#pragma once

/*
* Represent the base class of the Command pattern.
* Each concrete class provide logic to install fake artifact from specific type,
* while all the concrete class provide interface to the callee to do / redo the installation process.
*/
class FakeArtifact {

public:
	FakeArtifact(ArtifactType::Type type, ArtifactArgs args) : m_type(type), m_args(args) {}
	virtual ~FakeArtifact();

	/* Returns the type of the fake artifact. */
	ArtifactType::Type GetType();

	/* Returns the arguments of the fake artifact. */
	ArtifactArgs GetArgs();

	/* Creates the fake artifact in the local computer. */
	virtual bool Install() = 0;

	/* Removes the fake artifact from the local computer. */
	virtual bool Uninstall() = 0;

private:
	ArtifactType::Type m_type;
	ArtifactArgs m_args;
};