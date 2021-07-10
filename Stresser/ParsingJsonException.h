#pragma once

#include <exception>

class ParsingJsonException final : public std::exception
{
public:
	ParsingJsonException() = default;

	explicit ParsingJsonException(char const* message);

	char const* what() const override;
};

