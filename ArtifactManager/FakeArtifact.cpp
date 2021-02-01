#include "pch.h"
#include "FakeArtifact.h"

FakeArtifact::~FakeArtifact()
{
}

ArtifactType::Type FakeArtifact::GetType()
{
	return this->m_type;
}

ArtifactArgs FakeArtifact::GetArgs()
{
	return this->m_args;
}
