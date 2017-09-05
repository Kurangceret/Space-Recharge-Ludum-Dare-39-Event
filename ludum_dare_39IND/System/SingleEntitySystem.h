#pragma once
#include "System.h"
#include <SFML/System/Time.hpp>
#include <vector>
#include <bitset>
#include <memory>

#include "../Entity/Entity.h"

template<class Derived>
class SingleEntitySystem :public System<Derived>{
public:
	virtual ~SingleEntitySystem() = default;

	virtual void update(EntityManager* entityManager, sf::Time dt) override;
	virtual void addEntity(std::shared_ptr<Entity>&);

protected:
	virtual void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt) = 0;
	void pushRequiredComponent(std::size_t componentIdentifier);
	void pushRequiredComponent(const std::vector<std::size_t> closelyRelatedCompIden);

	bool isEntityEligible(Entity* entity) const;

	SingleEntitySystem() = default;

private:
	//called before deleting this entity from system process cycle
	virtual void cleanupEntityToBeRemoved(EntityManager* entityManager, 
		Entity* entityToBeDeleted, sf::Time dt);

	
	
private:
	std::vector<Entity::Ptr> mEntities;
	//components the system wanted for it to fit inside
	std::vector<std::vector<std::size_t>> mRequiredComponents;
};


template<class T>
void SingleEntitySystem<T>::update(EntityManager* entityManager, sf::Time dt)
{
	if(!initiatingTheUpdateCycle(entityManager, dt))
		return;
		
	for (auto iter = mEntities.begin(); iter != mEntities.end();){
		auto& ptr = *iter;
		if (ptr->isEntityToBeRemoved()){
			cleanupEntityToBeRemoved(entityManager, ptr.get(), dt);
			iter = mEntities.erase(iter);
			continue;
		}
		
		if (BaseSystem::isEntityRestrictedByBoundRect(ptr.get())){
			iter++;
			continue;
		}

		processEntity(entityManager, ptr.get(), dt);
		iter++;
	}

	finalizingTheUpdateCycle(entityManager, dt);
}

template<class T>
void SingleEntitySystem<T>::cleanupEntityToBeRemoved(EntityManager* entityManager,
	Entity* entityToBeDeleted, sf::Time dt)
{

}


template<class T>
bool SingleEntitySystem<T>::isEntityEligible(Entity* entity) const
{
	auto& bitSet = entity->getComponentsIdentifiers();
	for (auto identifier : mRequiredComponents){
		bool badFlag = true;
		for (auto iden : identifier){
			if (bitSet.test(iden)){
				badFlag = false;
				break;
			}
		}
		if (badFlag)
			return false;
	}
	return true;
}

template<class T>
void SingleEntitySystem<T>::addEntity(std::shared_ptr<Entity>& entity)
{
	if (!isEntityEligible(entity.get()))
		return;

	mEntities.push_back(entity);
}

template<class T>
void SingleEntitySystem<T>::pushRequiredComponent(std::size_t componentIdentifier)
{
	std::vector<std::size_t> newIden = { componentIdentifier };
	mRequiredComponents.push_back(newIden);
}

template<class T>
void SingleEntitySystem<T>::pushRequiredComponent(const std::vector<std::size_t> closelyRelatedCompIden)
{
	mRequiredComponents.push_back(closelyRelatedCompIden);
}