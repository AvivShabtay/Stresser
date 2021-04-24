#include "pch.h"
#include "StringUtils.h"

std::wstring StringUtils::RemoveQuotationMarks(std::wstring data) {
	static const std::wstring QUOTATION_MARKS(L"\"");

	if (data.size() <= 0) {
		return data;
	}

	auto newData = data;
	if (newData.substr(0, 1).compare(QUOTATION_MARKS) == 0) {
		newData = newData.substr(1, newData.size());
	}

	if (newData.substr(newData.size() - 1, 1).compare(QUOTATION_MARKS) == 0) {
		newData = newData.substr(0, newData.size() - 1);
	}

	return newData;
}

std::string StringUtils::RemoveQuotationMarks(std::string data) {
	static const std::string QUOTATION_MARKS("\"");

	if (data.size() <= 0) {
		return data;
	}

	auto newData = data;
	if (newData.substr(0, 1).compare(QUOTATION_MARKS) == 0) {
		newData = newData.substr(1, newData.size());
	}

	if (newData.substr(newData.size() - 1, 1).compare(QUOTATION_MARKS) == 0) {
		newData = newData.substr(0, newData.size() - 1);
	}

	return newData;
}

std::wstring StringUtils::RemoveNewLine(const std::wstring data)
{
	static const std::wstring NEW_LINE(L"\n");

	if (data.size() <= 0) {
		return data;
	}

	auto newData = data;
	if (newData.substr(newData.size() - 1, 1).compare(NEW_LINE) == 0) {
		newData = newData.substr(0, newData.size() - 1);
	}

	return newData;
}

std::string StringUtils::RemoveNewLine(const std::string data)
{
	static const std::string NEW_LINE("\n");

	if (data.size() <= 0) {
		return data;
	}

	auto newData = data;
	if (newData.substr(newData.size() - 1, 1).compare(NEW_LINE) == 0) {
		newData = newData.substr(0, newData.size() - 1);
	}

	return newData;
}

bool StringUtils::DoesEmptyJSON(const std::wstring data)
{
	static const std::wstring PARENTHESIS(L"{}");

	if (data.size() <= 0) {
		return false;
	}

	if (data.compare(PARENTHESIS) == 0)
		return true;

	return false;
}


bool StringUtils::DoesEmptyJSON(const std::string data)
{
	static const std::string PARENTHESIS("{}");

	if (data.size() <= 0) {
		return false;
	}

	if (data.compare(PARENTHESIS) == 0)
		return true;

	return false;
}

std::wstring StringUtils::ConvertToWideString(const std::string data)
{
	std::wstring wideString(data.length(), ' ');
	std::copy(data.begin(), data.end(), wideString.begin());
	
	return wideString;
}
