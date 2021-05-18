#pragma once
#include "IArtifact.h"
#include "resource.h"
#include "../Utils/PeResource.h"
#include "../Utils/LocalPcUtils.h"
#include "../Utils/StringUtils.h"

constexpr size_t PROCESS_TERMINATE_WAIT_TIME = 10000;
constexpr size_t INVALID_PID = 0;

class ProcessArtifact : public IArtifact
{
public:
	explicit ProcessArtifact(const std::string& name, const std::string& data);
	virtual ~ProcessArtifact();

	ArtifactTypes getType() const override;

	void install() override;
	void uninstall() override;

private:
	std::wstring m_filePath;
	DWORD m_pid;
};

