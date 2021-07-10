#pragma once

#include <exception>

class InvalidResponseException : public std::exception
{
public:
	InvalidResponseException() = default;

	explicit InvalidResponseException(char const* message);

	char const* what() const override;
};

