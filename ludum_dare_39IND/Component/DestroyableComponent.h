#pragma once
#include "Component.h"
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>


class ComponentArranger;
class DestroyableComponent :public Component{
public:
	friend ComponentArranger;
public:
	DestroyableComponent(Entity* entity);
	virtual ~DestroyableComponent();

	bool isDestroyed() const;
	bool isRemoveable() const;
private:
	std::unique_ptr<luabridge::LuaRef> mLuaDestroyedFunc;
	std::unique_ptr<luabridge::LuaRef> mLuaRemoveableFunc;
};

