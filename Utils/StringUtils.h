#pragma once

#include <iostream>

class StringUtils final
{
public:
	static std::wstring RemoveQuotationMarks(const std::wstring data);

	static std::string RemoveQuotationMarks(const std::string data);

	static std::wstring RemoveNewLine(const std::wstring data);

	static std::string RemoveNewLine(const std::string data);

	static bool DoesEmptyJSON(const std::wstring data);

	static bool DoesEmptyJSON(const std::string data);
};

