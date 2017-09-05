#pragma once
#include "Component.h"
#include <memory>
#include "../Map/TiledMapSystem.h"
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>


class ComponentArranger;

class InstanceComponent :	public Component{
public:
	friend class ComponentArranger;
public:
	InstanceComponent(Entity* entity);
	virtual ~InstanceComponent();


	void callInstanceLuaFunc(Entity* createdEntity, const TiledMapSystem::ObjectData& objectData);
private:
	std::unique_ptr<luabridge::LuaRef> mInstanceLuaFunc;
};

