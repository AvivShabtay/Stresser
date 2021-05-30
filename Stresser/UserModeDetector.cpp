#include "UserModeDetector.h"
#include "FileEventHandler.h"
#include "RegistryEventHandler.h"

UserModeDetector::UserModeDetector(const EventController &eventController)
	: IStresserDetector(eventController)
{
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
	this->m_etwManager->start();
}

void UserModeDetector::stop()
{
	this->m_etwManager->stop();
}
