#include "ScriptUpdateComponent.h"
#include "../Entity/Entity.h"

ScriptUpdateComponent::ScriptUpdateComponent(Entity* entity)
:Component(entity, ComponentIdentifier::ScriptUpdateComponent),
mScriptMemberVariables(nullptr),
mScriptUpdateFunction(nullptr),
mLuaState(nullptr)
{
}


ScriptUpdateComponent::~ScriptUpdateComponent()
{
}


/*void ScriptUpdateComponent::setScriptUpdateFunc(const ScriptUpdateFunc& scriptUpdateFunc)
{
	mScriptUpdateFunction = scriptUpdateFunc;
}*/

void ScriptUpdateComponent::setScriptUpdateFunc(const luabridge::LuaRef& scriptUpdateFunc, lua_State* luaState)
{
	mLuaState = luaState;
	mScriptUpdateFunction = std::make_unique<luabridge::LuaRef>(scriptUpdateFunc);
}

void ScriptUpdateComponent::setScriptMemberVariables(const luabridge::LuaRef& scriptMemberVariables)
{
	mScriptMemberVariables = std::make_unique<luabridge::LuaRef>(scriptMemberVariables);
}

luabridge::LuaRef ScriptUpdateComponent::getScriptMemberVariables(lua_State* luaState)
{
	luabridge::Stack<luabridge::LuaRef>::push(mLuaState, *mScriptMemberVariables);
	//return luabridge::Stack<luabridge::LuaRef>::get(mLuaState, -1);
	//luabridge::LuaRef(mLuaState, luabridge::FromStack());
	return luabridge::LuaRef(mLuaState, luabridge::LuaRef::fromStack(mLuaState, -1));
	//return *mScriptMemberVariables.get();
}


void ScriptUpdateComponent::popScriptStack()
{
	lua_settop(mLuaState, 0);
}

/*void ScriptUpdateComponent::runScriptUpdateFunc(sf::Time dt)
{
	Entity* ownerEntity = getOwnerEntity();
	//mScriptUpdateFunction(mOwnerEntity, dt);
	//luabridge::re
	try{
		if (mScriptUpdateFunction.get() && mScriptMemberVariables.get())
			(*mScriptUpdateFunction)(ownerEntity, dt.asSeconds(), *mScriptMemberVariables);
		else if (mScriptUpdateFunction.get() && !mScriptMemberVariables.get())
			(*mScriptUpdateFunction)(ownerEntity, dt.asSeconds());
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what() << std::endl;
	}
}*/