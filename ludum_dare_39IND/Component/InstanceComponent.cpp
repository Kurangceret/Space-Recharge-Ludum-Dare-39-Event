#include "InstanceComponent.h"
#include "ComponentIdentifier.h"
#include <iostream>

InstanceComponent::InstanceComponent(Entity* entity)
:Component(entity, ComponentIdentifier::InstanceComponent),
mInstanceLuaFunc(nullptr)
{
}


InstanceComponent::~InstanceComponent()
{
}


void InstanceComponent::callInstanceLuaFunc(Entity* createdEntity, const TiledMapSystem::ObjectData& objectData)
{
	if (!mInstanceLuaFunc.get())
		return;

	try{
		(*mInstanceLuaFunc)(createdEntity, objectData);
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what() << std::endl;
	}
}