#include "ObjectNotification.h"
#include "DriverConfiguration.h"
#include "ProcessUtils.h"
#include "StresserEngine.h"
#include "AutoLock.h"

OB_PREOP_CALLBACK_STATUS StresserEngineObPreOpCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation)
{
	UNREFERENCED_PARAMETER(RegistrationContext);
	UNREFERENCED_PARAMETER(OperationInformation);

	return OB_PREOP_SUCCESS;
}

void StresserEngineObPostOpCallback(PVOID RegistrationContext, POB_POST_OPERATION_INFORMATION OperationInformation)
{
	UNREFERENCED_PARAMETER(RegistrationContext);

	// Doesn't interfere System operations:
	if (OperationInformation->KernelHandle)
	{
		return;
	}

	// Get driver's specific context:
	auto* notificationContext = static_cast<PNotificationContext>(RegistrationContext);
	if (nullptr == notificationContext)
	{
		return;
	}

	// Get data corresponding to the requested process:
	auto* const requestedProcess = static_cast<PEPROCESS>(OperationInformation->Object);
	auto requestedProcessPid = ProcessUtils::getPidFromProcess(requestedProcess);

	if (requestedProcessPid.isError())
	{
		KdPrint((DRIVER_PREFIX "ObjectNotification invalid PID\n"));
		return;
	}

	bool doesFakeProcess = false;

	{
		AutoLock lock(notificationContext->mutex);

		auto* fakeProcessIds = notificationContext->fakeProcessIds;
		if (nullptr == fakeProcessIds)
		{
			return;
		}

		for (ULONG i = 0; i < fakeProcessIds->size(); ++i)
		{
			const FakeProcessId* fakeProcessId = fakeProcessIds->getAt(i);
			if (requestedProcessPid.getValue() == fakeProcessId->processId)
			{
				doesFakeProcess = true;
			}
		}
	}

	// Current process doesn't fake process:
	if (!doesFakeProcess)
	{
		return;
	}

	// Create event item memory:
	auto* eventItem = new (PagedPool, DRIVER_TAG) EventItem<EventInfo>;
	if (nullptr == eventItem)
	{
		LOG_MESSAGE("Could not allocate memory for " STRINGIFY(EventItem));
		return;
	}

	// Set event info:
	auto* eventInfo = &eventItem->data;
	KeQuerySystemTimePrecise(&eventInfo->time);
	eventInfo->processId = HandleToULong(PsGetCurrentProcessId());
	eventInfo->fakeProcessId = requestedProcessPid.getValue();

	// Force null-terminated string:
	memset(eventInfo->processName, 0, MAX_PROCESS_NAME_LENGTH + 1);

	// Get filename path of the calling process:
	auto processFileName = ProcessUtils::getProcessFileNameByPid(eventInfo->processId);
	if (!processFileName.isError())
	{
		const StresserString filenamePath = processFileName.getValue();
		wcscpy_s(eventInfo->processName, MAX_PROCESS_NAME_LENGTH, filenamePath.get());
	}

	KdPrint((DRIVER_PREFIX "PID=0x%08X open handle to fake process PID=0x%08X\n", eventInfo->processId, eventInfo->fakeProcessId));

	{
		AutoLock lock(notificationContext->mutex);

		auto* fakeProcessEvents = notificationContext->fakeProcessEvents;
		if (nullptr == fakeProcessEvents)
		{
			return;
		}

		fakeProcessEvents->insertTail(eventItem);
	}
}