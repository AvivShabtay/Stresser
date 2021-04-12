#pragma once

#include <string>

class Win32ErrorCodeException : public std::exception
{
public:
	explicit Win32ErrorCodeException(const std::string& message);
	virtual ~Win32ErrorCodeException() = default;

	const char* what() const override;

	// delete copy ctor, move ctor, assignment
	Win32ErrorCodeException(const Win32ErrorCodeException&) = delete;
	Win32ErrorCodeException& operator=(const Win32ErrorCodeException&) = delete;
	Win32ErrorCodeException(Win32ErrorCodeException&&) = default;
	Win32ErrorCodeException& operator=(Win32ErrorCodeException&&) = default;

	/* Get Windows last error code. */
	DWORD getErrorCode() const;

	/* Get Windows message corresponding to the last error that occurred. */
	std::string getWinErrorMessage() const;

protected:
	std::string getLastErrorMessage() const;

	DWORD m_errorCode;
	std::string m_winErrorMessage;
	std::string m_errorMessage;
};

