#pragma once
#ifndef __FAKE_FILE_H
#define __FAKE_FILE_H

#include "FakeArtifact.h"

class FakeFile : public FakeArtifact
{
public:
	bool Install();
	bool Uninstall();
};


#endif // !__FAKE_FILE_H