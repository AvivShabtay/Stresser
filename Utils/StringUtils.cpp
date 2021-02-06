#include "pch.h"
#include "StringUtils.h"

std::wstring StringUtils::RemoveQuotationMarks(std::wstring data) {
	static const std::wstring QUOTATION_MARKS(L"\?");

	if (data.size() <= 0) {
		return data;
	}

	auto newData = data;
	if (newData.substr(0, 1).compare(QUOTATION_MARKS)) {
		newData = newData.substr(1, newData.size());
	}

	if (newData.substr(newData.size() - 1, 1).compare(QUOTATION_MARKS)) {
		newData = newData.substr(0, newData.size() - 1);
	}

	return newData;
}

std::string StringUtils::RemoveQuotationMarks(std::string data) {
	static const std::string QUOTATION_MARKS("\?");

	if (data.size() <= 0) {
		return data;
	}

	auto newData = data;
	if (newData.substr(0, 1).compare(QUOTATION_MARKS)) {
		newData = newData.substr(1, newData.size());
	}

	if (newData.substr(newData.size() - 1, 1).compare(QUOTATION_MARKS)) {
		newData = newData.substr(0, newData.size() - 1);
	}

	return newData;
}
