#pragma once
#include "ExceptionWithWin32ErrorCode.h"
#include <Windows.h>

class UnexpectedHTTPStatusCodeException : public ExceptionWithWin32ErrorCode
{
public:
	UnexpectedHTTPStatusCodeException(DWORD statusCode);
	virtual ~UnexpectedHTTPStatusCodeException();

	DWORD GetHTTPStatusCode();

private:
	DWORD m_httpStatusCode;
};

