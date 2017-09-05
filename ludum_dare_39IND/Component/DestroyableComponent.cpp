#include "DestroyableComponent.h"
#include <iostream>
#include "ComponentIdentifier.h"
#include "../Entity/Entity.h"

DestroyableComponent::DestroyableComponent(Entity* entity)
:Component(entity, ComponentIdentifier::DestroyableComponent)
{
}


DestroyableComponent::~DestroyableComponent()
{
}


bool DestroyableComponent::isDestroyed() const
{
	Entity* ownerEntity = getOwnerEntity();
	try{
		if (mLuaDestroyedFunc)
			return (*mLuaDestroyedFunc)(ownerEntity);
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what() <<"9"<< std::endl;
	}
	return ownerEntity->isEntityToBeRemoved();
}

bool DestroyableComponent::isRemoveable() const
{
	try{
		if (mLuaRemoveableFunc)
			return (*mLuaRemoveableFunc)(getOwnerEntity());
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what() <<"10" << std::endl;
	}
	return isDestroyed();
}