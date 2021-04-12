#include "pch.h"
#include "Win32ErrorCodeException.h"

#include <vector>
#include <sstream>

#include <atlstr.h>

Win32ErrorCodeException::Win32ErrorCodeException(const std::string& errorMessage)
	: std::exception(errorMessage.c_str()), m_errorCode(0)
{
	this->m_errorCode = GetLastError();
	this->m_winErrorMessage = this->getLastErrorMessage();

	std::stringstream messageStream;
	messageStream << std::exception::what();
	messageStream << "\nWindows last error code: 0x";
	messageStream << std::hex << this->m_errorCode;
	messageStream << "\nWindows error message: ";
	messageStream << this->m_winErrorMessage;

	this->m_errorMessage = messageStream.str();
}

const char* Win32ErrorCodeException::what() const
{
	return this->m_errorMessage.c_str();
}

DWORD Win32ErrorCodeException::getErrorCode() const
{
	return this->m_errorCode;
}

std::string Win32ErrorCodeException::getWinErrorMessage() const
{
	return this->m_winErrorMessage;
}

std::string Win32ErrorCodeException::getLastErrorMessage() const
{
	if (m_errorCode == 0)
	{
		return "";
	}

	const int MESSAGE_SIZE = 512;
	std::vector<WCHAR> message(MESSAGE_SIZE);

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, this->m_errorCode,
		0, &message[0], MESSAGE_SIZE, nullptr);

	return std::string(CW2A(message.data()));
}
