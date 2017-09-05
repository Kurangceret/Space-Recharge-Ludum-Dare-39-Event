#pragma once
#include "Component.h"
#include <functional>
#include <SFML/System/Time.hpp>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

class Entity;
class ScriptUpdateSystem;
class ScriptUpdateComponent :	public Component{
public:
	friend ScriptUpdateSystem;
	typedef std::function < void(Entity*, sf::Time) > ScriptUpdateFunc;
public:
	ScriptUpdateComponent(Entity* entity);
	virtual ~ScriptUpdateComponent();

	//void setScriptUpdateFunc(const ScriptUpdateFunc& scriptUpdateFunc);
	void setScriptUpdateFunc(const luabridge::LuaRef& scriptUpdateFunc, lua_State* luaState);
	void setScriptMemberVariables(const luabridge::LuaRef& scriptMemberVariables);
	//void runScriptUpdateFunc(sf::Time dt);

	luabridge::LuaRef getScriptMemberVariables(lua_State* luaState);
	void popScriptStack();

	template<typename T>
	void setVarValue(const std::string& memberVarName, const T& val);

	template<typename T>
	T getVarValue(const std::string& memberVarName);

private:
	//ScriptUpdateFunc mScriptUpdateFunction;
	lua_State* mLuaState;
	std::unique_ptr<luabridge::LuaRef> mScriptUpdateFunction;
	std::unique_ptr<luabridge::LuaRef> mScriptMemberVariables;
};



template<typename T>
void ScriptUpdateComponent::setVarValue(const std::string& memberVarName, const T& val)
{
	if (!mScriptMemberVariables.get())
		return;

	auto& ref = *mScriptMemberVariables;

	if (!ref[memberVarName].isNil())
		ref[memberVarName] = val;
}

template<typename T>
T ScriptUpdateComponent::getVarValue(const std::string& memberVarName)
{
	if (!mScriptMemberVariables.get())
		return T();

	auto& ref = *mScriptMemberVariables;

	if (!ref[memberVarName].isNil())
		return ref[memberVarName].cast<T>();

	return T();
}