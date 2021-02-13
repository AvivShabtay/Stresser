#pragma once

#include "pch.h"
#include "../Utils/AutoHandle.h"
#include "../Utils/StringUtils.h"
#include "IConnection.h"
#include "../StresserExceptions/ExceptionWithWin32ErrorCode.h"
#include "../StresserExceptions/UnexpectedHTTPStatusCodeException.h"

/* Force the linker to add WINHTTP library */
#pragma comment (lib, "winhttp.lib")

/* Used as the JSON data parser. */
using json = nlohmann::json;

class Connection : public IConnection
{
public:

	void SetServerURL(std::wstring serverURL);

	/*
	* Send HTTP request to specific route and return the data to the client.
	*
	* @param requestType - represent the HTTP method to be used.
	* @param path - represent the route name in the server.
	* @param data - represent the request body to be send.
	* @return JSON containing returned data from the request.
	*/
	json SendRequest(std::wstring requestType, std::wstring path, json data);

	/*
	*/
	void SetToken(std::string token);

	static Connection& GetInstance();

	~Connection() = default;

private:
	Connection(std::wstring hostname);
	Connection();

	/*
	* Returns HTTP status code of the HTTP request.
	* @param requestHandle - Handle for HTTP request.
	* @return Status code value of the HTTP request.
	*/
	DWORD GetStatusCode(const HINTERNET requestHandle);

	const std::wstring AGENT = L"Stresser Client/1.0";
	const std::wstring HEADERS = L"Content-Type: application/json\r\n";

	std::wstring m_hostname;
	AutoHttpHandle m_ahSession;
	AutoHttpHandle m_ahConnect;
};


