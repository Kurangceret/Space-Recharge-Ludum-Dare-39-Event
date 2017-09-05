#include "ScriptAIComponent.h"
#include "../Body2dComponent.h"

#include "../../Entity/Entity.h"
#include "../../Map/AStarPathFinder.h"
#include "../../Helper/Utility.h"
#include <iostream>
#include "../../Map/GridMap.h"

const std::string ai_script_dir = "Scripts/EntityScripts/EnemyEntityScripts/AIScripts/";

ScriptAIComponent::ScriptAIComponent(Entity* entity)
:Component(entity,	ComponentIdentifier::ScriptAIComponent),
mCurrentState(nullptr),
mUsePlayerFoundSystem(true),
mLuaState(nullptr),
mLuaTileChecker(nullptr)
{
	
}


ScriptAIComponent::~ScriptAIComponent()
{
}


void ScriptAIComponent::update(EntityManager* entityManager, GridMap* gridMap, sf::Time dt)
{
	if (!mCurrentState.get() && !mQueueAIState.get())
		return;
	
	Entity* ownerEntity = getOwnerEntity();

	if (!mCurrentState.get() && mQueueAIState.get()){
		mCurrentState.reset(mQueueAIState.release());
		mCurrentState->enterState(ownerEntity, entityManager, gridMap, dt);
		return;
	}

	//std::cout << mCurrentState->getAIStateName() << std::endl;

	ScriptAITransition* transition = nullptr;

	for (const auto& transitionPtr : mCurrentState->getTransitions()){
		if (transitionPtr->isTriggered(
			mCurrentState->getLuaReferenceToState(), ownerEntity))
			transition = transitionPtr.get();
	}
	
	if (transition){
		mCurrentState->quitState(ownerEntity, entityManager, gridMap, dt);
		transition->callTargetState(ownerEntity);
		transition = nullptr;
		mCurrentState.reset(mQueueAIState.release());
		mQueueAIState = nullptr;
		mCurrentState->enterState(ownerEntity, entityManager, gridMap, dt);
		return;
	}

	mCurrentState->updateState(ownerEntity, entityManager, gridMap, dt);
	
}

void ScriptAIComponent::insertNewStateToManage(const std::string& stateName,
	ScriptAIState::Ptr state)
{
	mStates.insert(std::make_pair(stateName, std::move(state)));
}

void ScriptAIComponent::setCurState(lua_State* luaState)
{
	try{
		luabridge::LuaRef param = luabridge::LuaRef::fromStack(luaState, 2);


		//std::cout << param.tostring() << std::endl;
		if (param.isTable())
			setCurrentState(new ScriptAIState(std::make_unique<luabridge::LuaRef>(param)));
	}
	catch (luabridge::LuaException e){
		std::cout << e.what() << "1" << std::endl;
	}
}

void ScriptAIComponent::setCurrentState(ScriptAIState* state)
{
	// we queue the generated state
	//and call it later, since the script is still going on this call
	mQueueAIState.reset(state); 
}

void ScriptAIComponent::setCurrentState(ScriptAIState::Ptr state)
{
	mCurrentState = std::move(state);
}

std::string ScriptAIComponent::getCurAIStateName() const
{
	if (mCurrentState.get())
		return mCurrentState->getAIStateName();

	return "";
}

/*const luabridge::LuaRef* ScriptAIComponent::getCurLuaState() const
{
	if (mCurrentState.get())
		return &mCurrentState->getLuaReferenceToState();

	return nullptr;
}*/

/*luabridge::LuaRef& ScriptAIComponent::getCurAIState() const
{
	if (mCurrentState.get())
		return mCurrentState.get();
	return nullptr;
	//return mCurrentState->getLuaReferenceToState(mLuaState);
}*/

ScriptAIState* ScriptAIComponent::getCurAIState() const
{
	if (mCurrentState.get())
		return mCurrentState.get();
	return nullptr;
	//return mCurrentState->getLuaReferenceToState(mLuaState);
}

void ScriptAIComponent::set_new_state_script(const std::string& script_file_name, const std::string& states_table)
{
	Utility::readLuaScriptFile(ai_script_dir + script_file_name, mLuaState);
	std::string states_t_str = states_table + "AIStates";

	luabridge::LuaRef states = luabridge::LuaRef::getGlobal(mLuaState, states_t_str.c_str());

	states["initialStateFunc"](getOwnerEntity());
}