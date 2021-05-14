#pragma once
#include "EtwEventTypes.h"
#include "EventProvidersGuid.h"
#include <map>


const std::map<EtwEventTypes, GUID> EtwEventsGuid =
{
	{EtwEventTypes::DiskIo, DiskIoGuid},
	{EtwEventTypes::DiskFileIo, FileIoGuid},
	{EtwEventTypes::FileIo, FileIoGuid},
	{EtwEventTypes::FileIoInit, FileIoGuid},
	{EtwEventTypes::Registry, RegistryGuid},
	{EtwEventTypes::Process, ProcessGuid},
	{EtwEventTypes::ProcessCounters, ProcessGuid}
};
