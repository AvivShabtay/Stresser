#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winhttp.h>
#include "nlohmann/json.hpp"
#include <iostream>
#include "Request.h"
#include "Policy.h"

#include "FakeArtifact.h"
#include "FakeRegistry.h"
#include "FakeFile.h"
#include "ArtifactArgs.h"
#include "ArtifactType.h"

#pragma comment (lib, "winhttp.lib")

using json = nlohmann::json;

int __cdecl main(int argc, char** argv) {

	std::vector<FakeArtifact*> fakeArtifacts;

	ArtifactType type1 = ArtifactType::Registry;
	ArtifactArgs args1;
	fakeArtifacts.push_back(new FakeRegistry(type1, args1));

	ArtifactType type2 = ArtifactType::File;
	ArtifactArgs args2;
	fakeArtifacts.push_back(new FakeFile(type2, args2));

	// Cleanup:
	for (int i = 0; i < fakeArtifacts.size(); i++)
		delete fakeArtifacts.at(i);
}

void testPolicyFromServer() {
	Request getPolicy("127.0.0.1", 10000, "GET", "/policy");
	LPSTR responseData = getPolicy.GetData();
	json jsonData = json::parse(responseData);

	Policy policy(jsonData);
	std::cout << policy << std::endl;


	delete[] responseData;
}