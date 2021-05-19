#pragma once

#include "Event.h"
#include "HelperMacros.h"

#define STRESSER_OBJECT_NOTIFICATION_TYPE PsProcessType
#define STRESSER_OBJECT_CALLBACK_COUNT 1
#define STRESSER_PROCESS_OBJECT_TYPE 0

/* Dummy callback function for object pre operations, process events. */
OB_PREOP_CALLBACK_STATUS StresserEngineObPreOpCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation);

/* Callback function for object post operations, process events. */
void StresserEngineObPostOpCallback(PVOID RegistrationContext, POB_POST_OPERATION_INFORMATION OperationInformation);

/* Signal notification event. */
void signalNotificationEvent(const Event& notificationEvent);