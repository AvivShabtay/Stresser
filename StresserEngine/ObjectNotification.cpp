#include "ObjectNotification.h"
#include "DriverConfiguration.h"
#include "ProcessUtils.h"

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

	auto* const process = static_cast<PEPROCESS>(OperationInformation->Object);
	auto processPid = ProcessUtils::getPidFromProcess(process);

	if (processPid.isError())
	{
		KdPrint((DRIVER_PREFIX "ObjectNotification invalid PID\n"));
		return;
	}

	auto processFileName = ProcessUtils::getProcessFileNameByPid(processPid.getValue());
	if (processFileName.isError())
	{
		KdPrint((DRIVER_PREFIX "ObjectNotification invalid filename\n"));
		return;
	}

	const StresserString imageFileName = processFileName.getValue();
	KdPrint((DRIVER_PREFIX "ObjectNotification PID=0x%08X, File name= %ws\n", processPid, imageFileName.get()));
}