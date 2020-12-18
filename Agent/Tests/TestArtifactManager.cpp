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

	TEST_CLASS(TestArtifactManager)
	{
	public:

		TEST_METHOD(AddArtifactsToArtifactManager)
		{
			/* Arrange */
			ArtifactManager manager;
			const int numOfArtifacts = 3;
			ArtifactArgs argsArray[numOfArtifacts];
			ArtifactType::Type typesArray[numOfArtifacts] = {
				ArtifactType::Type::File,
				ArtifactType::Type::Registry,
				ArtifactType::Type::File
			};

			/* Act */
			// Add artifacts to the manager:
			for (int i = 0; i < numOfArtifacts; ++i) {
				auto args = argsArray[i];
				auto type = typesArray[i];

				switch (type) {
				case ArtifactType::Type::File:
				{
					auto artifact = new FakeFile(type, args);
					manager.AddArtifact(artifact);
					break;
				}
				case ArtifactType::Type::Registry:
				{
					auto artifact = new FakeRegistry(type, args);
					manager.AddArtifact(artifact);
					break;
				}
				default:
					break;
				}
			}

			/* Assert */
			Assert::AreEqual(manager.Size(), numOfArtifacts);
		}

		TEST_METHOD(GetArtifactsFromArtifactManager)
		{
			/* Arrange */
			ArtifactManager manager;
			const int numOfArtifacts = 3;
			ArtifactArgs argsArray[numOfArtifacts];
			ArtifactType::Type typesArray[numOfArtifacts] = {
				ArtifactType::Type::File,
				ArtifactType::Type::Registry,
				ArtifactType::Type::File
			};

			// Use for assert latter:
			FakeArtifact* artifacts[numOfArtifacts];

			/* Act */
			// Add artifacts to the manager:
			for (int i = 0; i < numOfArtifacts; ++i) {
				auto args = argsArray[i];
				auto type = typesArray[i];

				switch (type) {
				case ArtifactType::Type::File:
				{
					auto artifact = new FakeFile(type, args);
					artifacts[i] = artifact;
					manager.AddArtifact(artifact);
					break;
				}
				case ArtifactType::Type::Registry:
				{
					auto artifact = new FakeRegistry(type, args);
					artifacts[i] = artifact;
					manager.AddArtifact(artifact);
					break;
				}
				default:
					break;
				}
			}

			/* Assert */
			auto artifact2 = manager.GetArtifactByIndex(2);
			Assert::IsNotNull(artifact2);
			Assert::IsTrue(artifact2 == artifacts[2]);
		}

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
