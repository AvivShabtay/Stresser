#pragma once

#include <exception>
#include <iostream>

class SehException : public std::exception
{
public:
	explicit SehException(std::uint32_t errorCode);

	virtual ~SehException() = default;

	// Delete copy constructor & assignment operator
	// @note std::exception using the move constructor when throwing exception.
	SehException(const SehException&) = delete;
	SehException& operator=(const SehException&) = delete;
	SehException(SehException&&) = default;
	SehException& operator=(SehException&&) = default;

	char const* what() const override;

	std::uint32_t getErrorCode() const;

private:
	std::uint32_t m_errorCode;
	std::string m_errorMessage;
};