#include "Entity.h"


Entity::Entity(std::size_t id, std::size_t zIndex)
:mEntityId(id),
mZIndex(zIndex),
mComponentsIdentifiers(0),
mToBeRemoved(false)
{
}


Entity::~Entity()
{
}

std::size_t Entity::getId() const
{
	return mEntityId;
}

std::size_t Entity::getZIndex() const
{
	return mZIndex;
}

bool Entity::hasComp(ComponentIdentifier::ID componentIden)
{
	if (mComponentsIdentifiers == componentIden)
		return true;
	return false;
}

/*Entity::EntityBitSet Entity::getComponentsIdentifiers()
{
	return mComponentsIdentifiers;
}*/

const Entity::EntityBitSet& Entity::getComponentsIdentifiers() const
{
	return mComponentsIdentifiers;
}

/*std::size_t Entity::getComponentsIdentifiers() const
{
	return mComponentsIdentifiers;
}*/

void Entity::markEntityToBeRemoved()
{
	mToBeRemoved = true;
}

bool Entity::isEntityToBeRemoved() const
{
	return mToBeRemoved;
}