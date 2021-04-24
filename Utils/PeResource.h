#pragma once
#include "Win32ErrorCodeException.h"
#include "AutoHandle.h"

class PeResource
{
public:
	explicit PeResource(int resourceId, std::wstring resourceType);

	virtual ~PeResource() = default;	// According to MSDN no need to release the lock or the resource.

	// Delete copy constructor, assignment operator, move constructor, move operator:
	PeResource& operator=(const PeResource&) = delete;
	PeResource(const PeResource&) = delete;
	PeResource(PeResource&&) = delete;
	PeResource& operator=(PeResource&&) = delete;

	LPVOID getResourceData() const;

	void saveResourceToFileSystem(const std::wstring& path) const;

private:
	int m_resourceId;
	std::wstring m_resourceType;
	LPVOID m_resourceData;
	int m_resourceSize;
};