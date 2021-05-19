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
	// Doesn't interfere System operations:
	RETURN_ON_CONDITION(OperationInformation->KernelHandle);

	// Get driver's specific context:
	auto* notificationContext = static_cast<PNotificationContext>(RegistrationContext);
	RETURN_ON_CONDITION(nullptr == notificationContext);

	// Get data corresponding to the requested process:
	auto* const requestedProcess = static_cast<PEPROCESS>(OperationInformation->Object);
	auto requestedProcessPid = ProcessUtils::getPidFromProcess(requestedProcess);

	PRINT_MESSAGE_AND_RETURN_ON_CONDITION(requestedProcessPid.isError(), STRINGIFY(StresserEngineObPostOpCallback) " invalid PID");

	bool doesFakeProcess = false;

	{
		AutoLock lock(notificationContext->mutex);

		auto* fakeProcessIds = notificationContext->fakeProcessIds;
		RETURN_ON_CONDITION(nullptr == fakeProcessIds);

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
	RETURN_ON_CONDITION(false == doesFakeProcess);

	// Create event item memory:
	auto* eventItem = new (PagedPool, DRIVER_TAG) EventItem<EventInfo>;
	PRINT_MESSAGE_AND_RETURN_ON_CONDITION(nullptr == eventItem, "Could not allocate memory for " STRINGIFY(EventItem));

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

	// Add new event:
	{
		AutoLock lock(notificationContext->mutex);

		auto* fakeProcessEvents = notificationContext->fakeProcessEvents;
		RETURN_ON_CONDITION(nullptr == fakeProcessEvents);

		fakeProcessEvents->insertTail(eventItem);
	}

	auto* sharedEvent = notificationContext->onFakeProcessEvent;
	if (nullptr != sharedEvent->get())
	{
		Value<bool, NTSTATUS> result = sharedEvent->set();
		if (result.isError())
		{
			KdPrint((DRIVER_PREFIX STRINGIFY(StresserEngineObPostOpCallback) " could not signal UM, status: 0x%08X\n",
				result.getError()));
		}
	}
}

void signalNotificationEvent(const Event& notificationEvent)
{
	if (nullptr != notificationEvent.get())
	{
		Value<bool, NTSTATUS> result = notificationEvent.set();
		if (result.isError())
		{
			KdPrint((DRIVER_PREFIX STRINGIFY(StresserEngineObPostOpCallback) " could not signal UM, status: 0x%08X\n",
				result.getError()));
		}
	}
}