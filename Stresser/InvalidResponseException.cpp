#include "InvalidResponseException.h"

InvalidResponseException::InvalidResponseException(char const* message)
	: std::exception(message)
{
}

char const* InvalidResponseException::what() const
{
	return std::exception::what();
}