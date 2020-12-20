// ArtifactManager.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "ArtifactManager.h"

ArtifactManager::ArtifactManager() { }

ArtifactManager::~ArtifactManager() {
	if (artifacts.size() > 0) {
		for (int i = 0; i < artifacts.size(); ++i) {
			auto currentArtifact = artifacts.at(i);
			delete currentArtifact;
		}

		// Clear the entire vector:
		artifacts.erase(artifacts.begin(), artifacts.begin() + artifacts.size());
	}
}

/* Add new fake artifact object. */
bool ArtifactManager::AddArtifact(FakeArtifact* artifact)
{
	if (!artifact)
		return false;

	this->artifacts.push_back(artifact);
	return true;
}

/* Retrieve the index of given fake artifact. */
int ArtifactManager::GetArtifactIndex(FakeArtifact* artifact) {
	auto index = -1;

	if (!artifact) {
		return index;
	}
	else {
		for (int i = 0; i < artifacts.size(); ++i) {
			auto currentArtifact = artifacts.at(i);
			if (currentArtifact == artifact)
				index = i;
		}
	}

	return index;
}

/* Return the fake artifact object by it's index. */
FakeArtifact* ArtifactManager::GetArtifactByIndex(int artifactIndex) {
	if (artifactIndex < 0 || artifactIndex > artifacts.size()) {
		return nullptr;
	}
	else {
		return artifacts.at(artifactIndex);
	}
}

/*  */
bool ArtifactManager::RemoveArtifactByIndex(int artifactIndex) {
	auto result = false;

	if (artifactIndex < 0 || artifactIndex > artifacts.size()) {
		return false;
	}
	else {
		// Release the memory:
		delete artifacts.at(artifactIndex);

		// Remove the element from the data structure:
		artifacts.erase(artifacts.begin() + artifactIndex);
		return true;
	}
}

int ArtifactManager::Size() {
	return this->artifacts.size();
}

/*  */
bool ArtifactManager::Install() {
	if (0 < this->artifacts.size()) {
		for (auto& fakeArtifact : this->artifacts) {
			auto type = ArtifactType::ArtifactTypeToWString(fakeArtifact->GetType());
			if (!fakeArtifact->Install()) {
				return false;
			}
		}
		return true;
	}
	return false;
}

/*  */
bool ArtifactManager::Uninstall() {
	if (0 < this->artifacts.size()) {
		for (auto& fakeArtifact : this->artifacts) {
			auto type = ArtifactType::ArtifactTypeToWString(fakeArtifact->GetType());
			if (!fakeArtifact->Uninstall()) {
				return false;
			}
		}
		return true;
	}
	return false;
}