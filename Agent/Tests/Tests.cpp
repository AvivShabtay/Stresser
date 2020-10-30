#include "pch.h"
#include "CppUnitTest.h"

#include "../ArtifactManager/ArtifactManager.h"
#include "../ArtifactManager/FakeArtifact.h"
#include "../ArtifactManager/FakeRegistry.h"
#include "../ArtifactManager/FakeFile.h"
#include "../ArtifactManager/ArtifactArgs.h"
#include "../ArtifactManager/ArtifactType.h"

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
			ArtifactType typesArray[numOfArtifacts] = { ArtifactType::File, ArtifactType::Registry, ArtifactType::File };

			/* Act */
			// Add artifacts to the manager:
			for (int i = 0; i < numOfArtifacts; ++i) {
				auto args = argsArray[i];
				auto type = typesArray[i];

				switch (type) {
				case ArtifactType::File:
				{
					auto artifact = new FakeFile(type, args);
					manager.AddArtifact(artifact);
					break;
				}
				case ArtifactType::Registry:
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
			//auto artifact2 = manager.GetArtifactByIndex(2);
			//Assert::IsNotNull(artifact2);
		}

		TEST_METHOD(GetArtifactsFromArtifactManager)
		{
			/* Arrange */
			ArtifactManager manager;
			const int numOfArtifacts = 3;
			ArtifactArgs argsArray[numOfArtifacts];
			ArtifactType typesArray[numOfArtifacts] = { ArtifactType::File, ArtifactType::Registry, ArtifactType::File };

			// Use for assert latter:
			FakeArtifact* artifacts[numOfArtifacts];

			/* Act */
			// Add artifacts to the manager:
			for (int i = 0; i < numOfArtifacts; ++i) {
				auto args = argsArray[i];
				auto type = typesArray[i];

				switch (type) {
				case ArtifactType::File:
				{
					auto artifact = new FakeFile(type, args);
					artifacts[i] = artifact;
					manager.AddArtifact(artifact);
					break;
				}
				case ArtifactType::Registry:
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
