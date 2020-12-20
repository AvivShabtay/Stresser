#include "pch.h"
#include "FakeArtifact.h"

ArtifactType::Type FakeArtifact::GetType()
{
	return this->Type;
}

ArtifactArgs FakeArtifact::GetArgs()
{
	return this->Args;
}
