#pragma once

#include <iostream>

class StringUtils final
{
public:
	static std::wstring RemoveQuotationMarks(const std::wstring data);
	static std::string RemoveQuotationMarks(const std::string data);
};

