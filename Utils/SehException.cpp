#include "pch.h"
#include "SehException.h"

#include <iomanip>
#include <sstream>

SehException::SehException(const std::uint32_t errorCode)
	: m_errorCode(errorCode)
{
	std::stringstream stringStream;
	stringStream << "Got SEH exception, error code: ";
	stringStream << std::hex << std::setw(8) << std::setfill('0') << this->m_errorCode;

	this->m_errorMessage = stringStream.str();
}

char const* SehException::what() const
{
	return this->m_errorMessage.c_str();
}

std::uint32_t SehException::getErrorCode() const
{
	return this->m_errorCode;
}