#include "System.h"





/*void System::update(sf::Time dt)
{
	for (auto& ptr : mEntities)
		processEntity(ptr.get(), dt);
}


bool System::isEntityEligible(Entity* entity) const
{
	auto& bitSet = entity->getComponentsIdentifiers();
	for (auto identifier : mRequiredComponents){
		if (!bitSet.test(identifier))
			return false;
	}
	return true;
}


void System::addEntity(Entity* entity)
{
	if (!isEntityEligible(entity))
		return;
	
	mEntities.push_back(std::make_shared<Entity>(entity));
}


void System::pushRequiredComponent(std::size_t getComponentIdentifier)
{
	mRequiredComponents.push_back(getComponentIdentifier);
}*/