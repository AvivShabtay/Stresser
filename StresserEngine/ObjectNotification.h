#pragma once

#include "HelperMacros.h"

#define STRESSER_OBJECT_NOTIFICATION_TYPE PsProcessType
#define STRESSER_OBJECT_CALLBACK_COUNT 1
#define STRESSER_PROCESS_OBJECT_TYPE 0

/*  */
OB_PREOP_CALLBACK_STATUS StresserEngineObPreOpCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation);

/*  */
void StresserEngineObPostOpCallback(PVOID RegistrationContext, POB_POST_OPERATION_INFORMATION OperationInformation);