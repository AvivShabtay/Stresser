#include "pch.h"
#include "AutoSignedImageVerifier.h"
#include "DebugPrint.h"
#include "Win32ErrorCodeException.h"

#include <stdexcept>

AutoSignedImageVerifier::AutoSignedImageVerifier(std::wstring filePath, GUID trustProvider)
	: m_filePath(filePath), m_trustProviderGuid(trustProvider), m_fileInfo{ 0 }, m_winTrustData{ 0 }
{
	if (m_filePath.empty())
	{
		throw std::runtime_error("Invalid filename");
	}

	this->m_fileInfo = this->createFileInfo();
	this->m_winTrustData = this->createWinTrustData();
}

AutoSignedImageVerifier::~AutoSignedImageVerifier()
{
	try
	{
		this->closeResources();
	}
	catch (const Win32ErrorCodeException& exception)
	{
		DEBUG_PRINT(exception.what());
	}
	catch (...)
	{
		DEBUG_PRINT("Exception was thrown in AutoSignedImageVerifier destructor");
	}
}

VerificationResult AutoSignedImageVerifier::verify()
{
	const LONG status = WinVerifyTrust(nullptr, &m_trustProviderGuid, &m_winTrustData);
	if (ERROR_SUCCESS == status)
	{
		return { true, status };
	}
	else
	{
		return { false, status };
	}
}

WINTRUST_FILE_INFO AutoSignedImageVerifier::createFileInfo() const
{
	const WINTRUST_FILE_INFO fileInfo
	{
		sizeof(WINTRUST_FILE_INFO), // Structure size
		this->m_filePath.c_str(),	// Full path to the file
		nullptr,					// Optional, handle to file
		nullptr						// Optional, GUID
	};

	return fileInfo;
}

WINTRUST_DATA AutoSignedImageVerifier::createWinTrustData() const
{
	WINTRUST_DATA winTrustData{ 0 };

	memset(&winTrustData, 0, sizeof(winTrustData));

	winTrustData.cbStruct = sizeof(winTrustData);
	winTrustData.pPolicyCallbackData = nullptr; // Use default code signing EKU.
	winTrustData.pSIPClientData = nullptr; // No data to pass to SIP.
	winTrustData.dwUIChoice = WTD_UI_NONE; // Disable WVT UI.
	winTrustData.fdwRevocationChecks = WTD_REVOKE_NONE; // No revocation checking.
	winTrustData.dwUnionChoice = WTD_CHOICE_FILE; // Verify an embedded signature on a file.
	winTrustData.dwStateAction = WTD_STATEACTION_VERIFY; // Verify action.
	winTrustData.hWVTStateData = nullptr; // Verification sets this value.
	winTrustData.pwszURLReference = nullptr; // Not used.
	winTrustData.dwUIContext = 0; // Not used if WVT UI disabled
	winTrustData.pFile = const_cast<WINTRUST_FILE_INFO_*>(&this->m_fileInfo); // Set pFile.

	return winTrustData;
}

void AutoSignedImageVerifier::closeResources()
{
	// Any hWVTStateData must be released by a call with close.
	m_winTrustData.dwStateAction = WTD_STATEACTION_CLOSE;

	const LONG status = WinVerifyTrust(nullptr, &m_trustProviderGuid, &m_winTrustData);

	if (ERROR_SUCCESS != status)
	{
		throw Win32ErrorCodeException("Could not release verification handle");
	}
}
