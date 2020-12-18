#pragma once
#ifndef __REQUEST_H
#define __REQUEST_H
#include "pch.h"
#pragma comment (lib, "winhttp.lib")

class Connection
{
public:
	explicit Connection(std::string hostname);
	~Connection();

	std::string sendRequest(std::string requestType, std::string path, std::string data);

private:
	HINTERNET hSession;
	HINTERNET hConnect;
};


#endif // !__REQUEST_H


