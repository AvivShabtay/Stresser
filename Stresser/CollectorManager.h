#pragma once

#include <functional>

#include "IEventCollector.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

class CollectorManager
{
public:
	CollectorManager() = default;

	virtual ~CollectorManager() = default;

	void addCollector(IEventCollector* collector, std::string collectorName);

	void removeCollector(std::string collectorName);

private:
	std::map<std::unique_ptr<IEventCollector>, std::string> m_collectors;
};

