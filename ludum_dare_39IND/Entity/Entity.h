#pragma once
#include "../Component/Component.h"
#include "../Component/ComponentIdentifier.h"
#include <bitset>
#include <vector>
#include <memory>
#include <iostream>

class Entity{
public:
	typedef std::shared_ptr<Entity> Ptr;
	typedef std::bitset<100> EntityBitSet;
public:
	Entity(std::size_t id, std::size_t zIndex);
	~Entity();

	std::size_t getId() const;
	std::size_t getZIndex() const;

	//EntityBitSet getComponentsIdentifiers();
	const EntityBitSet& getComponentsIdentifiers() const;

	//check existence of T type in entity's getComponents list
	template <class T> bool hasComp();

	bool hasComp(ComponentIdentifier::ID getComponentIden);

	// Variadic template will allow users and systems to check if an entity has any number of specific entities
	//template <class T1, class T2, class... Other> bool hasComps();
	//call to remove getComp
	template <class T> bool removeComp();

	//call to create getComp, if return nullptr means getComp is already
	//attached to entity
	template <class T> T* createComp();
	//same as getComp<T>(), but it has additional
	//hasComp<T>() to check its existence faster
	//without iterating container
	template <class T> T* nonCreateComp();
	//get getComponent based on type T
	//call when it's 100% certain the caller has T getComponent
	template <class T> T* getComp();

	void markEntityToBeRemoved();
	bool isEntityToBeRemoved() const;
private:
	std::size_t mZIndex;
	std::size_t mEntityId;
	std::vector<Component::Ptr> mComponents;
	EntityBitSet mComponentsIdentifiers;
	bool mToBeRemoved;
	//std::size_t mComponentsIdentifiers;

	// Disambiguate Recursive Variadic Template
	template <class T> bool hasComps()
	{
		return this->hasComp<T>();
	}
};


template <class T>
bool Entity::hasComp()
{
	T getComp(this);

	//if (mComponentsIdentifiers.test(getComp.mIdentifier))
	if (mComponentsIdentifiers.test(getComp.mIdentifier))
	{
		return true;
	}

	return false;
}




/*template <class T1, class T2, class... Other> bool Entity::hasComps()
{
	T1 getComp(this);

	if (mComponentsIdentifiers.test(getComp.mIdentifier))
	{
		return (true && this->hasComps<T2, Other...>());
	}

	return false;
}*/

template <class T> bool Entity::removeComp()
{
	T getComp(this);

	for (unsigned int i = 0; i < mComponents.size(); ++i)
	{
		if (mComponents[i]->mIdentifier == getComp.mIdentifier)
		{
			//unregister getComponent from flags
			mComponentsIdentifiers.set(getComp.mIdentifier, false);
			//mComponentsIdentifiers = mComponentsIdentifiers ^ getComp.mIdentifier;

			mComponents.erase(mComponents.begin() + i);
			return true;
		}
	}

	return false;
}

template <class T> T* Entity::nonCreateComp()
{
	if (!hasComp<T>())
		return nullptr;

	T getComp(this);
	

	for (unsigned int i = 0; i < mComponents.size(); ++i)
	{
		if (mComponents[i]->mIdentifier == getComp.mIdentifier)
		{
			return dynamic_cast<T*>(mComponents[i].get());
		}
	}
	return nullptr;
}

template <class T> T* Entity::createComp()
{
	if (hasComp<T>())
		return nullptr;
	/*T getComp(this);

	for (unsigned int i = 0; i < mComponents.size(); ++i)
	{
		if (mComponents[i]->mIdentifier == getComp.mIdentifier)
		{
			return nullptr;
		}
	}*/

	T getComp(this);
	mComponentsIdentifiers.set(getComp.mIdentifier);
	//mComponentsIdentifiers = mComponentsIdentifiers | getComp.mIdentifier;
	std::unique_ptr<T> newPtr(new T(this));

	mComponents.push_back(std::move(newPtr));

	return dynamic_cast<T*>(mComponents.back().get());
}

template <class T> T* Entity::getComp()
{
	T getComp(this);

	for (unsigned int i = 0; i < mComponents.size(); ++i)
	{
		if (mComponents[i]->mIdentifier == getComp.mIdentifier)
		{
			return dynamic_cast<T*>(mComponents[i].get());
		}
	}
	return nullptr;
}