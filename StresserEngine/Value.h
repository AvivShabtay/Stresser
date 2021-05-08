#pragma once

#include "DriverConfiguration.h"
#include "Move.h"

#include <ntifs.h>

template<typename ReturnedValue, typename Error = ULONG>
class Value
{
public:
	// Disable default constructor:
	Value() = delete;

	// Allow conversion:
	Value(ReturnedValue& value) : m_value(value), m_isError(false) { }
	Value(ReturnedValue&& value) : m_value(move(value)), m_isError(false) { }

	// Allow conversion from ErrorValue:
	Value(Error& error) : m_isError(true), m_error(error) { }
	Value(Error&& error) : m_value(), m_isError(true), m_error(move(error)) { }

	/* Return the value data. */
	const ReturnedValue& getValue() { return this->m_value; }

	/* Return if there is an error. */
	bool isError() const { return this->m_isError; }

	/* If exists return the error value, otherwise throw. */
	const Error& getError()
	{
		return this->m_error;
	}

private:
	ReturnedValue m_value;
	bool m_isError;
	Error m_error;
};