#pragma once
#include "System.h"
#include <SFML/System/Time.hpp>
#include <vector>
#include <bitset>


#include "../Entity/Entity.h"

//Dual Entity System will only run both of the containers
//there will be no seperate update, each update calls involves a pair
//from left container to right container
template<class Derived>
class DualEntitySystem :public System<Derived>{
public:
	
	virtual ~DualEntitySystem() = default;

	virtual void update(EntityManager* entityManager, sf::Time dt) override;
	virtual void addEntity(std::shared_ptr<Entity>& entity);

protected:
	DualEntitySystem() = default;

	virtual void processBothEntity(EntityManager* entityManager, Entity* leftEntity, 
		Entity* rightEntity, sf::Time dt) = 0;

	virtual bool beforeMainProcessLeftEntity(EntityManager* entityManager, 
		Entity* leftEntity,	sf::Time dt);
	virtual void afterMainProcessLeftEntity(EntityManager* entityManager, 
		Entity* leftEntity,	sf::Time dt);

	void pushLeftRequiredComponent(std::size_t componentIdentifier);
	void pushLeftRequiredComponent(const std::vector<std::size_t>& closelyRelatedCompIden);

	void pushRightRequiredComponent(std::size_t componentIdentifier);
	void pushRightRequiredComponent(const std::vector<std::size_t>& closelyRelatedCompIden);

	bool isEntityEligibleForLeft(Entity* entity) const;
	bool isEntityEligibleForRight(Entity* entity) const;


	
private:
	std::vector<Entity::Ptr> mEntitiesLeftList;
	std::vector<std::vector<std::size_t>> mLeftRequiredComponents;

	std::vector<Entity::Ptr> mEntitiesRightList;
	std::vector<std::vector<std::size_t>> mRightRequiredComponents;
};


template<class T>
void DualEntitySystem<T>::update(EntityManager* entityManager, sf::Time dt)
{
	if(!initiatingTheUpdateCycle(entityManager, dt))
		return;
	for (auto leftIter = mEntitiesLeftList.begin(); leftIter != mEntitiesLeftList.end();)
	{
		auto& leftPtr = *leftIter;
		if (leftPtr->isEntityToBeRemoved()){
			leftIter = mEntitiesLeftList.erase(leftIter);
			continue;
		}

		if (!beforeMainProcessLeftEntity(entityManager, leftPtr.get(), dt)){
			leftIter++;
			continue;
		}

		for (auto rightIter = mEntitiesRightList.begin(); rightIter != mEntitiesRightList.end();)
		{
			auto& rightPtr = *rightIter;
			if (rightPtr.get() == leftPtr.get()){
				rightIter++;
				continue;
			}
			if (rightPtr->isEntityToBeRemoved()){
				rightIter = mEntitiesRightList.erase(rightIter);
				continue;
			}

			
			processBothEntity(entityManager, leftPtr.get(), rightPtr.get(), dt);
			rightIter++;
		}
		afterMainProcessLeftEntity(entityManager, leftPtr.get(), dt);
		leftIter++;
	}
	finalizingTheUpdateCycle(entityManager, dt);
}



template<class T>
bool DualEntitySystem<T>::isEntityEligibleForLeft(Entity* entity) const
{
	auto& bitSet = entity->getComponentsIdentifiers();
	for (auto identifier : mLeftRequiredComponents){
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
bool DualEntitySystem<T>::isEntityEligibleForRight(Entity* entity) const
{
	auto& bitSet = entity->getComponentsIdentifiers();
	for (auto identifier : mRightRequiredComponents){
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
void DualEntitySystem<T>::addEntity(std::shared_ptr<Entity>& entity)
{
	if (isEntityEligibleForLeft(entity.get()))
		mEntitiesLeftList.push_back(entity);

	if (isEntityEligibleForRight(entity.get()))
		mEntitiesRightList.push_back(entity);
}

template<class T>
void DualEntitySystem<T>::pushLeftRequiredComponent(std::size_t componentIdentifier)
{
	std::vector<std::size_t> newIden = { componentIdentifier };
	mLeftRequiredComponents.push_back(newIden);
}

template<class T>
void DualEntitySystem<T>::pushLeftRequiredComponent(const std::vector<std::size_t>& closelyRelatedCompIden)
{
	mLeftRequiredComponents.push_back(closelyRelatedCompIden);
}

template<class T>
void DualEntitySystem<T>::pushRightRequiredComponent(const std::vector<std::size_t>& closelyRelatedCompIden)
{
	mRightRequiredComponents.push_back(closelyRelatedCompIden);
}

template<class T>
void DualEntitySystem<T>::pushRightRequiredComponent(std::size_t componentIdentifier)
{
	std::vector<std::size_t> newIden = {componentIdentifier};
	//newIden.push_back(componentIdentifier);
	mRightRequiredComponents.push_back(newIden);
}

template<class T>
bool DualEntitySystem<T>::beforeMainProcessLeftEntity(EntityManager* entityManager, Entity* leftEntity,
	sf::Time dt)
{
	return true;
}

template<class T>
void DualEntitySystem<T>::afterMainProcessLeftEntity(EntityManager* entityManager, Entity* leftEntity,
	sf::Time dt)
{

}