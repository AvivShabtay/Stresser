#include "pch.h"
#include "FakeArtifact.h"

ArtifactType FakeArtifact::GetType()
{
	return this->Type;
}

ArtifactArgs FakeArtifact::GetArgs()
{
	return this->Args;
}
