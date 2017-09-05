#pragma once

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>
class Entity;
#include "../Resource/TextureStringManager.h"
#include <unordered_map>

class GeneralData;
class SystemManager;
class EntityDetectorComponent;
class b2Shape;
class Body2dComponent;

//this class usually works with LUA script
//in order to create entity with different getComponents
//and data
class ComponentArranger{
public:
	ComponentArranger(GeneralData* generalData, luabridge::LuaRef entityNameToScriptDirMap,
		luabridge::LuaRef iteratedEntityNameToScriptDirMap, luabridge::LuaRef utilityTableFunc,
		lua_State* luaState);
	~ComponentArranger();
	

	

	//mostly uses for lua script only
	std::string entScriptDirToEntName(const std::string& entityScriptDir) const;

	void readFromLuaPremadeScript(std::shared_ptr<Entity>& entity, 
		const std::string& entityName);

	void readFromLuaScriptName(std::shared_ptr<Entity>& entity, const std::string& scriptFilename,
		const std::string& tableName);

	void readFromLuaScriptReturn(std::shared_ptr<Entity>& entity, 
		const std::string& scriptFilename);

	void readFromLuaScript(std::shared_ptr<Entity>& entity, luabridge::LuaRef& initialTable);

	//get the initializeData parto onyl
	luabridge::LuaRef getPremadeEntityTableData(const std::string& entityTableName);
private:
	void readOnlyScript(const std::string& scriptDir);
private:
	
	void readSpriteComponent(Entity* entity,
		luabridge::LuaRef& table);
	
	void readHealthComponent(Entity* entity,
		luabridge::LuaRef& table);
	
	void readAnimationComponent(Entity* entity,
		luabridge::LuaRef& table);
	void readScriptUpdateComponent(Entity* entity,
		luabridge::LuaRef& table);
	void readCategoryComponent(Entity* entity,
		luabridge::LuaRef& table);
	void readTextDisplayComponent(Entity* entity,
		luabridge::LuaRef& table);
	
	void readAutomaticPathComponent(Entity* entity,
		luabridge::LuaRef& table);
	
	void readTimerComponent(Entity* entity,
		luabridge::LuaRef& table);
	void readDestroyableComponent(Entity* entity,
		luabridge::LuaRef& table);
	
	void arrangeChildEntityList(Entity* entity,
		luabridge::LuaRef& table);

	void readPlayerComponent(Entity* entity,
		luabridge::LuaRef& table);
		

	void readBody2dComponent(Entity* entity,
		luabridge::LuaRef& table);

	void readObstacleComponent(Entity* entity, luabridge::LuaRef& table);

	void readAutoBody2dMovementComponent(Entity* entity,
		luabridge::LuaRef& table);
	void readJumpComponent(Entity* entity,
		luabridge::LuaRef& table);

	void readRealTimeInputComponent(Entity* entity, luabridge::LuaRef& table);

	void readInstanceComponent(Entity* entity, luabridge::LuaRef& table);

	void readScriptAIComponent(Entity* entity, luabridge::LuaRef& table);

	void readParticleEmitterComponent(Entity* entity, luabridge::LuaRef& table);

	void readSpaceShipComponent(Entity* entity, luabridge::LuaRef& table);

	void readPlanetSpawnComponent(Entity* entity, luabridge::LuaRef& table);
	
private:
	void readShapeTable(const luabridge::LuaRef& shapeTable,
		std::unique_ptr<b2Shape>& shapePtr, Body2dComponent* body2dComp = nullptr, 
		bool changeBodyOrigin = false);
private:
	TextureStringManager& mTexturesStringManager;
	GeneralData* mGeneralData;
	SystemManager* mSystemManager;
	//entity/table name to the script directory
	luabridge::LuaRef mEntityNameToScriptDir; 
	//script directory to the real table data
	std::unordered_map<std::string, luabridge::LuaRef> mEntityInitialTableMap;
	//script directory to entityName
	std::unordered_map<std::string, std::string> mEntityScriptDirToName;

	luabridge::LuaRef mLuaUtilityTableFunc;

	lua_State* mLuaState;
};

