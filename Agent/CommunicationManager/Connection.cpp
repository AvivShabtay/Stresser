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

std::string Connection::sendRequest(std::string requestType, std::string path, std::string data) {
	
	BOOL bResults = FALSE;
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
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

	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		do
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				std::cout << "Error " << GetLastError() << " in WinHttpQueryDataAvailable." << std::endl;

			// Allocate space for the buffer.
			std::unique_ptr<byte> pszOutBuffer(new byte[dwSize + 1]);
			
			// Read the Data.
			ZeroMemory(pszOutBuffer.get(), dwSize + 1);

			if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer.get(), dwSize, &dwDownloaded))
				return "";
			else
				return (char*)pszOutBuffer.get();

		} while (dwSize > 0);
	}

	if (hRequest) WinHttpCloseHandle(hRequest);

	// Report any errors.
	if (!bResults)
		std::cout << "Error " << GetLastError() << " has occurred." << std::endl;

	return "";
}
