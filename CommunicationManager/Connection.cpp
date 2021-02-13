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

	// TODO: Remove latter
	auto dataForDebugOnly = jsonToSend.dump();

	if (!this->m_ahConnect.get()) {
		throw ExceptionWithWin32ErrorCode("Invalid HTTP connection");
	}

	AutoHttpHandle hRequest(::WinHttpOpenRequest(this->m_ahConnect.get(), requestType.c_str(), path.c_str(),
		//nullptr, nullptr, nullptr, WINHTTP_FLAG_SECURE));
		nullptr, nullptr, nullptr, 0));

	if (!hRequest.get()) {
		throw ExceptionWithWin32ErrorCode("Could not open HTTP request");
	}

	std::string data = jsonToSend.dump();

	std::wstring headers = this->HEADERS.c_str();
	if (!this->m_token.empty()) {
		headers += L"Authorization:";
		headers += std::wstring(CA2W(this->m_token.c_str()));
		headers += L"\r\n";
	}

	if (!(::WinHttpSendRequest(hRequest.get(), headers.c_str(), headers.length(),
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

	// Null terminator character:
	dwDataSize += 1;

	// Allocate space for the buffer:
	std::unique_ptr<byte> receivedData(new byte[dwDataSize]);
	ZeroMemory(receivedData.get(), dwDataSize);

	DWORD dwDownloaded = 0;
	if (!WinHttpReadData(hRequest.get(), (LPVOID)receivedData.get(), dwDataSize, &dwDownloaded)) {
		return "";
	}

	std::string receivedDataString(reinterpret_cast<PCHAR>(receivedData.get()));
	receivedDataString = StringUtils::RemoveNewLine(receivedDataString);
	if (StringUtils::DoesEmptyJSON(receivedDataString)) {
		return "";
	}

	else {
		return json::parse(receivedData.get());
	}

	return "";
}

void Connection::SetToken(std::string token)
{
	this->m_token = token;
}

Connection& Connection::GetInstance(std::wstring serverURL) {
	static Connection g_connection(serverURL);
	return g_connection;
}

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
