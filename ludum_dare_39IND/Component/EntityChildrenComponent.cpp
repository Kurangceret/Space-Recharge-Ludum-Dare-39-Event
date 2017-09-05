#include "EntityChildrenComponent.h"
#include <algorithm>
#include "../Entity/Entity.h"
#include "CategoryComponent.h"
#include "EntityParentComponent.h"
#include "../Entity/EntityManager.h"

#include "Body2dComponent.h"
#include "../Helper/Utility.h"
#include "../Helper/Math.h"



EntityChildrenComponent::EntityChildrenComponent(Entity* entity)
:Component(entity, ComponentIdentifier::EntityChildrenComponent)
{
}


EntityChildrenComponent::~EntityChildrenComponent()
{
}

Entity* EntityChildrenComponent::getParent() const
{
	return mParent;
}





/*void EntityChildrenComponent::removeChild(Entity* childEntity)
{
	auto iter = std::find(mChildren.begin(), mChildren.end(), childEntity);

	if (iter != mChildren.end())
		mChildren.erase(iter);
}

void EntityChildrenComponent::removeRemoveableChild(EntityManager* entityManager)
{
	typedef std::vector<Entity*>::const_iterator EntityIter;
	for (auto& iter : mChildren){
		for (EntityIter i = mChildren.begin(); i != mChildren.end();)
		{
			Entity *curEntity = *(i);
			if (entityManager->isEntityRemoveable(curEntity)){
				i = mChildren.erase(i);
				continue;
			}
						
			i++;
		}
	}
}

void EntityChildrenComponent::insertNewChild(Entity* childEntity)
{
	if (childEntity == mOwnerEntity || !childEntity->hasComp<EntityParentComponent>())
		return;

	childEntity->getComp<EntityParentComponent>()->mParent = mOwnerEntity;
	mChildren.push_back(childEntity);
}

bool EntityChildrenComponent::isCategoryInsideChildren(std::size_t category) const
{
	return std::find_if(mChildren.begin(), mChildren.end(), [&category](Entity* entity)-> bool
	{
		if (entity->hasComp<CategoryComponent>() && entity->
			getComp<CategoryComponent>()->getCategory() & category)
			return true;

		return false;

	}) != mChildren.end();
}*/
