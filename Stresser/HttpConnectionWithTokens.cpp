#include "HttpConnectionWithTokens.h"

#include <atlstr.h>

#include "../Utils/AutoCriticalSection.h"

HttpConnectionWithTokens::HttpConnectionWithTokens()
{
	this->m_httpConnection.reset(
		new HttpConnection(HOSTNAME.c_str(), AGENT.c_str(), HEADERS.c_str()));
}

Json HttpConnectionWithTokens::SendRequest(const std::wstring& requestType, const std::wstring& path, const Json& jsData)
{
	// Updates HTTP headers to support authorization:
	const std::wstring headers = this->getHeadersWithAuthorization();
	this->m_httpConnection->setHeaders(headers);

	return this->m_httpConnection->SendRequest(requestType, path, jsData);
}

void HttpConnectionWithTokens::setToken(const std::string& token)
{
	AutoCriticalSection autoCriticalSection;
	this->m_token = token;
}

std::wstring HttpConnectionWithTokens::getHeadersWithAuthorization()
{
	std::wstring headers(this->m_httpConnection->getHeaders());

	if (!this->m_token.empty())
	{
		// Add the token to the HTTP headers:
		headers += L"Authorization:";
		headers += std::wstring(CA2W(this->m_token.c_str()));
		headers += L"\r\n";
	}

	return headers;
}