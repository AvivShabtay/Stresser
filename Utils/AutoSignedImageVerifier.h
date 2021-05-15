#pragma once

#include <string>

#include <SoftPub.h>
#include <WinTrust.h>

#pragma comment (lib, "wintrust")

using VerificationResult = std::pair<bool, LONG>;

/*
	Verify if given path to PE file is signed by valid certificate.
	Signed file:
		- Hash that represents the subject is trusted.
		- Trusted publisher without any verification errors.
		- No publisher or time stamp chain errors
 */
class AutoSignedImageVerifier
{
public:
	explicit AutoSignedImageVerifier(std::wstring filePath, GUID trustProvider = WINTRUST_ACTION_GENERIC_VERIFY_V2);

	virtual ~AutoSignedImageVerifier();

	// Disable: copyable, assignable, movable:
	AutoSignedImageVerifier(AutoSignedImageVerifier&) = delete;
	AutoSignedImageVerifier& operator=(AutoSignedImageVerifier&) = delete;
	AutoSignedImageVerifier(AutoSignedImageVerifier&&) = delete;
	AutoSignedImageVerifier& operator=(AutoSignedImageVerifier&&) = delete;

	VerificationResult verify();

private:
	WINTRUST_FILE_INFO getDefaultFileInfo() const;
	WINTRUST_DATA getDefaultWinTrustData() const;

	void closeResources();

	std::wstring m_filePath;
	GUID m_trustProviderGuid;
	WINTRUST_FILE_INFO m_fileInfo;
	WINTRUST_DATA m_winTrustData;
};

