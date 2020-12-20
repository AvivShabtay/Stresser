#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winhttp.h>
#include "nlohmann/json.hpp"
#include <iostream>
#include "../CommunicationManager/Request.h"
#include "Policy.h"

#pragma comment (lib, "winhttp.lib")

using json = nlohmann::json;

int __cdecl main(int argc, char** argv) {
	// TODO
}

void testPolicyFromServer() {
	Request getPolicy("127.0.0.1", 10000, "GET", "/policy");
	LPSTR responseData = getPolicy.GetData();
	json jsonData = json::parse(responseData);

	Policy policy(jsonData);
	std::cout << policy << std::endl;


	delete[] responseData;
}