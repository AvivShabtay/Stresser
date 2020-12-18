#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
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
	};
}
