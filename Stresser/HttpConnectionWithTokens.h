#pragma once

#include "../CommunicationManager/HttpConnection.h"

#include "nlohmann/json.hpp"

#include <iostream>

using Json = nlohmann::json;

//static const std::wstring HOSTNAME(L"stresser-project.herokuapp.com");
static const std::wstring HOSTNAME(L"localhost");
static const std::wstring AGENT(L"Stresser Client/1.0");
static const std::wstring HEADERS(L"Content-Type: application/json\r\n");

class HttpConnectionWithTokens
{
public:
	HttpConnectionWithTokens();

	Json SendRequest(const std::wstring& requestType, const std::wstring& path, const Json& jsData);

	void setToken(const std::string& token);

private:
	std::wstring getHeadersWithAuthorization();

	std::string m_token;
	std::unique_ptr<IHttpConnection> m_httpConnection;
};

