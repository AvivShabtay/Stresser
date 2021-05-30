#pragma once
#include "IArtifact.h"
#include "IEtwEventHandler.h"

#include <memory>
#include <vector>

constexpr size_t FILE_CREATE_OPCODE = 64;

class FileEventHandler : public IEtwEventHandler
{
public:
	explicit FileEventHandler(std::vector<std::shared_ptr<IArtifact>>& artifacts);

	~FileEventHandler() override = default;

	std::optional<EventEntity> onEventRecord(PEVENT_RECORD record) override;

private:
	std::vector<std::shared_ptr<IArtifact>>& m_artifacts;
};
