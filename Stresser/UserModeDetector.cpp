#include "UserModeDetector.h"
#include "FileEventHandler.h"
#include "RegistryEventHandler.h"
#include "../Utils/DebugPrint.h"

UserModeDetector::UserModeDetector(const EventController& eventController)
	: IStresserDetector(eventController)
{
	DEBUG_WTRACE(UserModeDetector, "Initiating user mode detector");

	this->m_etwManager.reset(new EtwManager());
	this->m_etwManager->setCallback([this](EventEntity& eventEntity)
		{
			this->sendEvent(eventEntity);
		});

	const std::shared_ptr<RegistryEventHandler> registryEventHandler(new RegistryEventHandler(this->m_artifactsToReport));
	const std::shared_ptr<FileEventHandler> fileEventHandler(new FileEventHandler(this->m_artifactsToReport));

	this->m_etwManager->registerEventHandle(registryEventHandler);
	this->m_etwManager->registerEventHandle(fileEventHandler);
}

void UserModeDetector::start()
{
	DEBUG_WTRACE(UserModeDetector, "Start user mode detector");

	this->m_etwManager->start();
}

void UserModeDetector::stop()
{
	DEBUG_WTRACE(UserModeDetector, "Stop user mode detector");

	this->m_etwManager->stop();
}
