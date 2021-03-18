#pragma once

class ExceptionWithWin32ErrorCode : public std::exception
{
public:
	ExceptionWithWin32ErrorCode(std::string message);
	virtual ~ExceptionWithWin32ErrorCode();

	virtual char const* what() const throw();

protected:
	std::string m_errorMessage;
};

