#include "pch.h"
#include "Connection.h"

Connection::Connection(std::wstring hostname)
	: m_hostname(hostname)
{
	AutoHttpHandle hSession(::WinHttpOpen(this->AGENT.c_str(), WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0));

	if (!hSession.get()) {
		throw ExceptionWithWin32ErrorCode("Could not open HTTP session");
	}

	this->m_ahSession = std::move(hSession);

	AutoHttpHandle hConnect(::WinHttpConnect(this->m_ahSession.get(), this->m_hostname.c_str(),
		INTERNET_DEFAULT_PORT, 0));

	if (!hConnect.get()) {
		throw ExceptionWithWin32ErrorCode("Could not initiate HTTP connection");
	}

	this->m_ahConnect = std::move(hConnect);
}

json Connection::SendRequest(std::wstring requestType, std::wstring path, json jsonToSend) {

	if (!this->m_ahConnect.get()) {
		throw ExceptionWithWin32ErrorCode("Invalid HTTP connection");
	}

	AutoHttpHandle hRequest(::WinHttpOpenRequest(this->m_ahConnect.get(), requestType.c_str(), path.c_str(),
		nullptr, nullptr, nullptr, WINHTTP_FLAG_SECURE));

	if (!hRequest.get()) {
		throw ExceptionWithWin32ErrorCode("Could not open HTTP request");
	}

	std::string data = jsonToSend.dump();

	if (!(::WinHttpSendRequest(hRequest.get(), this->HEADERS.c_str(), this->HEADERS.length(),
		(LPVOID)data.c_str(), data.length(), data.length(), 0))) {
		throw ExceptionWithWin32ErrorCode("Could not send HTTP data");
	}

	if (!(::WinHttpReceiveResponse(hRequest.get(), nullptr))) {
		throw ExceptionWithWin32ErrorCode("Could not get response from HTTP request");
	}

	DWORD dwStatusCode = this->GetStatusCode(hRequest.get());
	if (dwStatusCode != 200) {
		throw UnexpectedHTTPStatusCodeException(dwStatusCode);
	}

	DWORD dwDataSize;
	if (!WinHttpQueryDataAvailable(hRequest.get(), &dwDataSize)) {
		throw ExceptionWithWin32ErrorCode("Could not query data size of the request");
	}

	dwDataSize += 1;

	// Allocate space for the buffer:
	std::unique_ptr<byte> pszOutBuffer(new byte[dwDataSize]);
	ZeroMemory(pszOutBuffer.get(), dwDataSize);

	DWORD dwDownloaded = 0;
	if (!WinHttpReadData(hRequest.get(), (LPVOID)pszOutBuffer.get(), dwDataSize, &dwDownloaded)) {
		return "";
	}
	else {
		return json::parse(pszOutBuffer.get());
	}

	return "";
}

Connection::~Connection() { }


DWORD Connection::GetStatusCode(const HINTERNET requestHandle) {
	DWORD dwStatusCode = 0;
	DWORD dwSize = sizeof(DWORD);

	::WinHttpQueryHeaders(
		requestHandle,					// Request handle
		WINHTTP_QUERY_STATUS_CODE		// Query status code
		| WINHTTP_QUERY_FLAG_NUMBER,	// Query for 32bit number (status code)
		WINHTTP_HEADER_NAME_BY_INDEX,	// For query which isn't WINHTTP_QUERY_CUSTOM
		&dwStatusCode,					// Buffer to status code
		&dwSize,						// 32bit size
		WINHTTP_NO_HEADER_INDEX	 		// Only first occurrence of a header should be returned
	);

	return dwStatusCode;
}

std::wstring ReadHeader(const HINTERNET requestHandle, std::wstring headerName, ULONG queryFlags) {
	DWORD bufferSize = 256;
	std::vector<WCHAR> headerBuffer(bufferSize);

	if (!WinHttpQueryHeaders(requestHandle, queryFlags, nullptr, &headerBuffer[0],
		&bufferSize, WINHTTP_NO_HEADER_INDEX)) {

		auto header = CStringA(headerName.c_str()).GetString();
		throw ExceptionWithWin32ErrorCode("Could not read header: " + std::string(header));
	}

	return std::wstring(&headerBuffer[0]);
}
