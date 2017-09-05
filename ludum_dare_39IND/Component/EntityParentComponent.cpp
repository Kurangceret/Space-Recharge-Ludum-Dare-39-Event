#include "EntityParentComponent.h"
#include "ComponentIdentifier.h"
#include "EntityChildrenComponent.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityManager.h"
#include "CategoryComponent.h"
#include "Body2dComponent.h"
#include <algorithm>


/*EntityChildData::EntityChildData(Entity* childEntity)
:mAffectedByParentRotation(true),
mChildEntity(childEntity),
mAllowChildToAutoParent(true)
{}
EntityChildData::EntityChildData()
:mAffectedByParentRotation(true),
mChildEntity(nullptr),
mAllowChildToAutoParent(true)
{}*/

EntityParentComponent::EntityParentComponent(Entity* entity)
:Component(entity, ComponentIdentifier::EntityParentComponent)
{
}


EntityParentComponent::~EntityParentComponent()
{
}


void EntityParentComponent::attachChild(Entity* childEntity)
{
	Body2dComponent* body2dComp = childEntity->nonCreateComp<Body2dComponent>();
	if (body2dComp){
		body2dComp->setPosition(0.f, 0.f);
		body2dComp->setAngleDegree(0.f);
	}
	mChildren.push_back(childEntity);
}

void EntityParentComponent::removeChild(Entity* childEntity)
{
	auto iter = std::find_if(mChildren.begin(), mChildren.end(), 
		[&](const Entity* childData)-> bool{
			return childData == childEntity;
	});

	if (iter != mChildren.end())
		mChildren.erase(iter);
}

void EntityParentComponent::removeRemoveableChild(EntityManager* entityManager)
{
	typedef std::vector<Entity*>::const_iterator EntityIter;
	//for (auto& iter : mChildren){
	for (EntityIter i = mChildren.begin(); i != mChildren.end();)
	{
		Entity *curEntity = *i;
		if (entityManager->isEntityRemoveable(curEntity)){
			i = mChildren.erase(i);
			continue;
		}

		i++;
	}
	//}
}

void EntityParentComponent::insertNewChild(Entity* childEntity)
{
	Entity* ownerEntity = getOwnerEntity();
	if (childEntity == ownerEntity || !childEntity->hasComp<EntityChildrenComponent>())
		return;

	childEntity->getComp<EntityChildrenComponent>()->mParent = ownerEntity;
	mChildren.push_back(childEntity);
}

bool EntityParentComponent::isCategoryInsideChildren(std::size_t category) const
{
	return std::find_if(mChildren.begin(), mChildren.end(), [&category](Entity* entityChildData)-> bool
	{
		Entity* entity = entityChildData;
		if (entity->hasComp<CategoryComponent>() && entity->
			getComp<CategoryComponent>()->getCategory() & category)
			return true;

		return false;

	}) != mChildren.end();
}

Entity* EntityParentComponent::getChild(int index) const
{
	if (index < 0 || index >= mChildren.size())
		return nullptr;

	return mChildren[index];
}

Entity* EntityParentComponent::getChild(Entity* childToGet) const
{
	auto iter = std::find_if(mChildren.begin(), mChildren.end(),
		[&](const Entity* childData)-> bool{
		return childData == childToGet;
	});
	assert(iter != mChildren.end());
	return *iter;
	//return getChildData(childToGet).mChildEntity;
}

/*const EntityChildData& EntityParentComponent::getChildData(Entity* childToGet) const
{
	auto iter = std::find_if(mChildren.begin(), mChildren.end(),
		[&](const EntityChildData& childData)-> bool{
		return childData.mChildEntity == childToGet;
	});
	assert(iter != mChildren.end());
	return *iter;
}

EntityChildData& EntityParentComponent::getEditableChildData(Entity* childToGet)
{
	auto iter = std::find_if(mChildren.begin(), mChildren.end(),
		[&](const EntityChildData& childData)-> bool{
		return childData.mChildEntity == childToGet;
	});
	assert(iter != mChildren.end());
	return *iter;
}

void EntityParentComponent::setChildAffectedByParentRotation(Entity* child, bool flag)
{
	getEditableChildData(child).mAffectedByParentRotation = flag;
}*/