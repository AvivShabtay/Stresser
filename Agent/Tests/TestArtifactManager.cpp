#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	/* Check if given path of file exits in the file system. */
	BOOL FileExists(LPCTSTR szPath) {
		DWORD dwAttrib = GetFileAttributes(szPath);

		return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
			!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	/* Return mock FakeFile object. */
	FakeFile* CreateFakeFile(std::wstring path) {
		ArtifactArgs artifactArguments;
		ArtifactType::Type artifactType;
		FakeArtifact* fakeFile;

		artifactArguments.AddArg(path);
		artifactType = ArtifactType::Type::File;
		return new FakeFile(artifactType, artifactArguments);
	}

	TEST_CLASS(TestArtifactManager2)
	{
	public:

		TEST_METHOD(ArtifactManagerInstall)
		{
			// Arrange:
			bool managerInstall = false;
			bool isFakeFileExists = false;
			std::wstring fakeFilePath = L"C:\\temp\\testFile1.txt";
			ArtifactManager manager;

			// Act
			manager.AddArtifact(CreateFakeFile(fakeFilePath));
			managerInstall = managerInstall = manager.Install();
			isFakeFileExists = FileExists(fakeFilePath.c_str());

			// Assert:
			Assert::IsTrue(managerInstall);
			Assert::IsTrue(isFakeFileExists);

			// Cleanup:
			if (isFakeFileExists) {
				::DeleteFile(fakeFilePath.c_str());
			}
		}

		TEST_METHOD(ArtifactManagerUninstall)
		{
			// Arrange:
			bool managerInstall = false;
			bool managerUninstall = false;
			bool isFakeFileExists = false;
			std::wstring fakeFilePath = L"C:\\temp\\testFile1.txt";
			ArtifactManager manager;

			// Act
			manager.AddArtifact(CreateFakeFile(fakeFilePath));
			managerInstall = managerInstall = manager.Install();
			Assert::IsTrue(managerInstall);

			// If installation succeeded, uninstall:
			if (managerInstall) {
				managerUninstall = manager.Uninstall();
			}
			Assert::IsTrue(managerUninstall);
			isFakeFileExists = FileExists(fakeFilePath.c_str());

			// Assert:
			Assert::IsFalse(isFakeFileExists);

			// Cleanup:
			if (isFakeFileExists) {
				::DeleteFile(fakeFilePath.c_str());
			}
		}
	};
}
