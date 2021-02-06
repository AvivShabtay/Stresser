#include "pch.h"
#include "UnexpectedHTTPStatusCodeException.h"

UnexpectedHTTPStatusCodeException::UnexpectedHTTPStatusCodeException(DWORD statusCode)
	: ExceptionWithWin32ErrorCode("Get unexpected HTTP status code"), m_httpStatusCode(statusCode) { }

UnexpectedHTTPStatusCodeException::~UnexpectedHTTPStatusCodeException()
{
}

DWORD UnexpectedHTTPStatusCodeException::GetHTTPStatusCode() {
	return this->m_httpStatusCode;
}
