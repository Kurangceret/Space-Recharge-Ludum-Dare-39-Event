#pragma once
#include "../Component.h"
#include <memory>
#include <unordered_map>

#include <SFML/System/Time.hpp>
#include "ScriptAIState.h"
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

class ComponentArranger;
class EntityManager;
class GridMap;

class ScriptAIComponent : public Component{
public:
	friend ComponentArranger;
	
public:
	ScriptAIComponent(Entity* entity);
	virtual ~ScriptAIComponent();

	void update(EntityManager* entityManager, GridMap* gridMap, sf::Time dt);
	void insertNewStateToManage(const std::string& stateName,
		ScriptAIState::Ptr state);

	void setCurrentState(ScriptAIState* state);
	void setCurrentState(ScriptAIState::Ptr state);

	/*exclusive to script usage*/
	void setCurState(lua_State* luaState);
	/**/
	std::string getCurAIStateName() const;
	ScriptAIState* getCurAIState() const;
	//luabridge::LuaRef& getCurAIState() const;

	//float getRangeCheckingToPlayer() const;

	void set_new_state_script(const std::string& script_file_name, const std::string& states_table);
private:
	std::unordered_map<std::string, ScriptAIState::Ptr> mStates;
	ScriptAIState::Ptr mCurrentState;
	ScriptAIState::Ptr mQueueAIState;
	lua_State* mLuaState;

	bool mUsePlayerFoundSystem;
	bool mPlayerIsFound;
	float mRangeCheckingToTarget;
	//uses in conjunction with mUsePlayerFoundSystem
	std::unique_ptr<luabridge::LuaRef> mLuaTileChecker;


};

