#include "pch.h"
#include "Connection.h"

Connection::Connection(std::string hostname)
{
	this->hSession = ::WinHttpOpen(L"Stresser Client/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);
	if (!this->hSession) {
		throw;
	}

	this->hConnect = ::WinHttpConnect(hSession, CA2W(hostname.c_str()), INTERNET_DEFAULT_PORT, 0);
	if (!this->hConnect) {
		throw;
	}
}

Connection::~Connection()
{
	// Close any open handles.
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
}

json Connection::SendRequest(std::string requestType, std::string path, json jsonToSend) {
	
	BOOL bResults = FALSE;
	std::string data = jsonToSend.dump();
	std::wstring headers = L"Content-Type: application/json\r\n";

	HINTERNET hRequest = ::WinHttpOpenRequest(hConnect, 
											  CA2W(requestType.c_str()), 
											  CA2W(path.c_str()), 
											  NULL, 
											  nullptr,
											  NULL, 
											  WINHTTP_FLAG_SECURE);

	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
									  headers.c_str(), 
									  headers.length(), 
									  (LPVOID)data.c_str(), 
									  data.length(), 
									  data.length(), 
									  0);

	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	// Check the Response
	if (bResults) {

		DWORD dwDownloaded = 0;
		DWORD dwStatusCode = 0;
		DWORD dwSize = sizeof(dwStatusCode);

		// Check status code
		WinHttpQueryHeaders(hRequest,
			WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
			WINHTTP_HEADER_NAME_BY_INDEX,
			&dwStatusCode, &dwSize, WINHTTP_NO_HEADER_INDEX);

		if (dwStatusCode != 200) return "";

		// Check for available data.
		if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
			std::cout << "Error " << GetLastError() << " in WinHttpQueryDataAvailable." << std::endl;

		// Allocate space for the buffer.
		std::unique_ptr<byte> pszOutBuffer(new byte[dwSize + 1]);

		// Read the Data.
		ZeroMemory(pszOutBuffer.get(), dwSize + 1);

		if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer.get(), dwSize, &dwDownloaded)) {
			if (hRequest) WinHttpCloseHandle(hRequest);
			return "";
		}
		else {
			if (hRequest) WinHttpCloseHandle(hRequest);
			return json::parse(pszOutBuffer.get());
		}
	}

	// Report any errors.
	if (!bResults)
		std::cout << "Error " << GetLastError() << " has occurred." << std::endl;

	return "";
}
