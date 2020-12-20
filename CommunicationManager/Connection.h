#pragma once
#ifndef __REQUEST_H
#define __REQUEST_H
#include "pch.h"
#pragma comment (lib, "winhttp.lib")

using json = nlohmann::json;

class Connection
{
public:
	explicit Connection(std::string hostname);
	~Connection();

	json SendRequest(std::string requestType, std::string path, json data);

private:
	HINTERNET hSession;
	HINTERNET hConnect;
};


#endif // !__REQUEST_H


