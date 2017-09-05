#include "SystemManager.h"


SystemManager::SystemManager()
{
}


SystemManager::~SystemManager()
{
}


void SystemManager::putEntityInSystems(std::shared_ptr<Entity>& entity)
{
	for (auto& systemPair : mBaseSystems)
		systemPair.second->addEntity(entity);
}

void SystemManager::setSystemsBoundRect(const sf::FloatRect& boundRect)
{
	for (auto& systemPair : mBaseSystems)
		systemPair.second->setBoundRect(boundRect);
}