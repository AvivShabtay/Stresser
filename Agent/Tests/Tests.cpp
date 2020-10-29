#include "pch.h"
#include "CppUnitTest.h"

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

		TEST_METHOD(TestMethod1)
		{
			const size_t numOfArtifacts = 2;
			const size_t zeroArtifacts = 0;

			std::vector<FakeArtifact*> fakeArtifacts;

			ArtifactType type1 = ArtifactType::Registry;
			ArtifactArgs args1;
			fakeArtifacts.push_back(new FakeRegistry(type1, args1));

			ArtifactType type2 = ArtifactType::File;
			ArtifactArgs args2;
			fakeArtifacts.push_back(new FakeFile(type2, args2));

			Assert::AreEqual(fakeArtifacts.size(), numOfArtifacts);

			// Cleanup:
			for (int i = 0; i < fakeArtifacts.size(); i++)
				delete fakeArtifacts.at(i);

		}
	};
}
