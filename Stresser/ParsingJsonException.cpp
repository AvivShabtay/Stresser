#include "ParsingJsonException.h"

ParsingJsonException::ParsingJsonException(char const* message)
	:std::exception(message)
{
}

char const* ParsingJsonException::what() const
{
	return std::exception::what();
}
