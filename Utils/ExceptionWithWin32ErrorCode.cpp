#include "pch.h"
#include "ExceptionWithWin32ErrorCode.h"

ExceptionWithWin32ErrorCode::ExceptionWithWin32ErrorCode(std::string message)
{
	std::stringstream errorMessage;
	auto dwErrorCode = GetLastError();
	if (dwErrorCode) {
		errorMessage << message << ", Error code 0x" << std::hex << dwErrorCode;
	}
	this->m_errorMessage = errorMessage.str();
}

ExceptionWithWin32ErrorCode::~ExceptionWithWin32ErrorCode() { }

char const* ExceptionWithWin32ErrorCode::what() const throw()
{
	return this->m_errorMessage.c_str();
}
