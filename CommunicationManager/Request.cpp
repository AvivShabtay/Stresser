#include "pch.h"
#include "Request.h"

Request::Request(std::string hostname, int port, std::string requestType, std::string requestURL)
{
	this->hostname = hostname;
	this->port = port;
	this->requestType = requestType;
	this->requestURL = requestURL;
}

Request::~Request()
{
	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
}

LPSTR Request::GetData() {
	BOOL bResults = FALSE;
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	std::wstring headers = L"Content-Type: application/json\r\n";

	// Use WinHttpOpen to obtain a session handle.
	hSession = ::WinHttpOpen(L"Stresser Client/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = ::WinHttpConnect(hSession, CA2W(hostname.c_str()), port, 0);

	// Create an HTTP request handle.
	if (hConnect)
		//hRequest = ::WinHttpOpenRequest(hConnect, L"GET", NULL, NULL, NULL, NULL, 0);
		hRequest = ::WinHttpOpenRequest(hConnect, CA2W(requestType.c_str()), CA2W(requestURL.c_str()), NULL, nullptr, NULL, 0);

	// Send a request.
	if (hRequest)
		//bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
		bResults = WinHttpSendRequest(hRequest, headers.c_str(), headers.length(), WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		do
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize = 0;
			}
			else
			{
				// Read the Data.
				ZeroMemory(pszOutBuffer, dwSize + 1);

				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
					dwSize, &dwDownloaded))
					//printf("Error %u in WinHttpReadData.\n", GetLastError());
					return nullptr;
				else
					//printf("%s\n", pszOutBuffer);
					return pszOutBuffer;

				// Free the memory allocated to the buffer.
				//delete[] pszOutBuffer;
			}

		} while (dwSize > 0);
	}

	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n", GetLastError());

	return nullptr;
}
