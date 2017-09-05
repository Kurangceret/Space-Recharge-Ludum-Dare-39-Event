#include "ScriptAITransition.h"
#include <iostream>

ScriptAITransition::ScriptAITransition(LuaRefPtr callTargetStateFunc,
	LuaRefPtr isTriggeredFunc)
:mCallTargetStateFunction(std::move(callTargetStateFunc)),
mIsTriggeredFunction(std::move(isTriggeredFunc))
{
}


ScriptAITransition::~ScriptAITransition()
{
}


bool ScriptAITransition::isTriggered(const luabridge::LuaRef& state, 
	Entity* ownerEntity) const
{
	bool isTrigger = false; 
	try{
		isTrigger = (*mIsTriggeredFunction)(state, ownerEntity);
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what() <<"5"<< std::endl;
	}
	return isTrigger;
}

void ScriptAITransition::callTargetState(Entity* ownerEntity)
{
	try{
		(*mCallTargetStateFunction)(ownerEntity);
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what()<<"6" << std::endl;
	}
}