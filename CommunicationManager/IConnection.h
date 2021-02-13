#pragma once
#include <iostream>

#include "nlohmann/json.hpp"

/* Used as the JSON data parser. */
using json = nlohmann::json;

class IConnection
{
public:
	IConnection() { }

	/*

	*/
	virtual void SetToken(std::string token) = 0;

	/*

	*/
	virtual json SendRequest(std::wstring requestType, std::wstring path, json data) = 0;

protected:
	std::wstring serverURL;
	std::string m_token;
};