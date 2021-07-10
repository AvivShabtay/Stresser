#pragma once

#include <exception>

class NetworkConnectionException : public std::exception
{
public:
	NetworkConnectionException() = default;

	explicit NetworkConnectionException(char const* message);

	char const* what() const override;
};

