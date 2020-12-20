#include "pch.h"

int main() {

	ArtifactManager manager;

	ArtifactArgs vmwareArgs;
	vmwareArgs.AddArg(L"C:\\temp\\vmwareFakeFile1.txt");
	vmwareArgs.AddArg(L"C:\\temp\\vmwareFakeFile2.txt");

	ArtifactArgs vboxArgs;
	vboxArgs.AddArg(L"C:\\temp\\vboxFakeFile1.txt");
	vboxArgs.AddArg(L"C:\\temp\\vboxFakeFile2.txt");

	FakeArtifact* vmwareFakeFiles = new FakeFile(ArtifactType::Type::File, vmwareArgs);
	FakeArtifact* vboxFakeFiles = new FakeFile(ArtifactType::Type::File, vboxArgs);

	ArtifactArgs vmwareRegistryArgs;
	vmwareRegistryArgs.AddArg(L"SOFTWARE\\VMwareFakeRegistry1");
	vmwareRegistryArgs.AddArg(L"SOFTWARE\\VMwareFakeRegistry2");

	ArtifactArgs vboxRegistryArgs;
	vboxRegistryArgs.AddArg(L"SOFTWARE\\VBoxFakeRegistry1");
	vboxRegistryArgs.AddArg(L"SOFTWARE\\VBoxFakeRegistry2");

	FakeArtifact* vmwareFakeRegistry = new FakeRegistry(ArtifactType::Type::Registry, vmwareRegistryArgs);
	FakeArtifact* vboxFakeRegistry = new FakeRegistry(ArtifactType::Type::Registry, vboxRegistryArgs);

	// Add artifacts to be installed:
	manager.AddArtifact(vmwareFakeFiles);
	manager.AddArtifact(vboxFakeFiles);
	manager.AddArtifact(vmwareFakeRegistry);
	manager.AddArtifact(vboxFakeRegistry);

	bool result = manager.Install();
	result = manager.Uninstall();

	std::wcout << "Manager result: " << (result ? L"Success" : L"Failed") << std::endl;
}