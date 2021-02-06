// ArtifactManager.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "ArtifactManager.h"

ArtifactManager::ArtifactManager() { }

ArtifactManager::~ArtifactManager() {
	if (this->m_artifacts.size() > 0) {
		for (int i = 0; i < this->m_artifacts.size(); ++i) {
			auto currentArtifact = this->m_artifacts.at(i);
			delete currentArtifact;
		}

		// Clear the entire vector:
		this->m_artifacts.erase(this->m_artifacts.begin(),
			this->m_artifacts.begin() + this->m_artifacts.size());
	}
}

bool ArtifactManager::AddArtifact(FakeArtifact* artifact)
{
	if (!artifact)
		return false;

	this->m_artifacts.push_back(artifact);
	return true;
}

int ArtifactManager::GetArtifactIndex(FakeArtifact* artifact) {
	auto index = -1;

	if (!artifact) {
		return index;
	}
	else {
		for (int i = 0; i < this->m_artifacts.size(); ++i) {
			auto currentArtifact = this->m_artifacts.at(i);
			if (currentArtifact == artifact)
				index = i;
		}
	}

	return index;
}

FakeArtifact* ArtifactManager::GetArtifactByIndex(int artifactIndex) {
	if (artifactIndex < 0 || artifactIndex > this->m_artifacts.size()) {
		return nullptr;
	}
	else {
		return this->m_artifacts.at(artifactIndex);
	}
}

bool ArtifactManager::RemoveArtifactByIndex(int artifactIndex) {
	auto result = false;

	if (artifactIndex < 0 || artifactIndex > this->m_artifacts.size()) {
		return false;
	}
	else {
		// Release the memory:
		delete this->m_artifacts.at(artifactIndex);

		// Remove the element from the data structure:
		this->m_artifacts.erase(this->m_artifacts.begin() + artifactIndex);
		return true;
	}
}

int ArtifactManager::Size() {
	return this->m_artifacts.size();
}

bool ArtifactManager::Install() {
	if (0 < this->m_artifacts.size()) {
		for (auto& fakeArtifact : this->m_artifacts) {
			auto type = ArtifactType::ArtifactTypeToWString(fakeArtifact->GetType());
			if (!fakeArtifact->Install()) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool ArtifactManager::Uninstall() {
	if (0 < this->m_artifacts.size()) {
		for (auto& fakeArtifact : this->m_artifacts) {
			auto type = ArtifactType::ArtifactTypeToWString(fakeArtifact->GetType());
			if (!fakeArtifact->Uninstall()) {
				return false;
			}
		}
		return true;
	}
	return false;
}