#pragma once

#include <iostream>

class ITokenListener
{
public:
	ITokenListener() = default;

	virtual void updateToken(const std::string& newToken) = 0;

	virtual ~ITokenListener() = default;
};

