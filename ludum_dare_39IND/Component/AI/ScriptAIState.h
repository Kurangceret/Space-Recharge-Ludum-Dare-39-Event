#pragma once
#include <memory>
#include <SFML/System/Time.hpp>
#include "ScriptAITransition.h"
#include <vector>

class Entity;
class EntityManager;
class ComponentArranger;
class GridMap;

class ScriptAIState{
public:
	typedef std::unique_ptr<luabridge::LuaRef> LuaRefPtr;
	typedef std::shared_ptr<luabridge::LuaRef> LuaRefSharedPtr;
	typedef std::unique_ptr< ScriptAIState> Ptr;
	typedef  std::vector<ScriptAITransition::Ptr> TransitionsList;
	friend ComponentArranger;
public:
	ScriptAIState(LuaRefPtr statePtr);
	//ScriptAIState(luabridge::LuaRef& state);
	~ScriptAIState();

	void enterState(Entity* ownerEntity, EntityManager* entityManager, GridMap* gridMap, sf::Time dt);
	void updateState(Entity* ownerEntity, EntityManager* entityManager, GridMap* gridMap, sf::Time dt);
	void quitState(Entity* ownerEntity, EntityManager* entityManager, GridMap* gridMap, sf::Time dt);

	const TransitionsList& getTransitions() const;

	luabridge::LuaRef& getLuaReferenceToState(lua_State* luaState = nullptr) const;

	lua_State* getLuaStateRef() const;
	std::string getAIStateName() const;
private:
	void readTransitionsList();
	//void setNewAIState();
private:
	/*LuaRefPtr mEnterFunction;
	LuaRefPtr mUpdateFunction;
	LuaRefPtr mQuitFunction;*/

	LuaRefPtr mLuaReferenceToState;
	TransitionsList mTransitions;
	std::string mAIStateName;
};

