#include "NetworkConnectionException.h"

NetworkConnectionException::NetworkConnectionException(char const* message)
	: std::exception(message)
{
}

char const* NetworkConnectionException::what() const
{
	return std::exception::what();
}

