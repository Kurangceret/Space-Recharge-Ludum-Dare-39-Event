#pragma once

#include <unordered_map>
#include <map>
#include <queue>
#include <functional>
#include <string>
#include <SFML/Graphics/Rect.hpp>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>
#include "Entity.h"
#include <vector>

class ComponentArranger;

class GeneralData;
class b2World;


struct QueueEntityData{
	typedef std::function<void(Entity*)> EngineInitializeFunc;

	QueueEntityData(const std::string& layerName, const std::string& scriptDir,
		const std::string& tableName,
		bool isLocalVar,
		const luabridge::LuaRef* customInitializeData,
		const luabridge::LuaRef* customInitializeLuaFunc,
		const EngineInitializeFunc* customEngineInitializeFunc);

	std::string mLayer;
	std::string mScriptDir;
	//table name could also be referred as the entity name
	std::string mTableName;
	bool mIsLocalVar; // if set to true this will take the -1 stack from luabridge::luaref(means the table is local)

	std::shared_ptr<luabridge::LuaRef> mCustomInitializeData;
	std::shared_ptr<luabridge::LuaRef> mCustomInitializeLuaFunc;
	EngineInitializeFunc mEngineInitializeFunc;
};


struct InteractionFuncData{
	typedef std::function<void(Entity*)> InteractionFunc;
	
	//Engine Constructor
	InteractionFuncData(Entity* ownerEntity, const InteractionFunc& func, bool isSpecificZIndex,
		std::size_t zIndex);

	//Lua Script constructor
	InteractionFuncData(Entity* ownerEntity, 
		const luabridge::LuaRef& luaInteractionFunc, 
		const luabridge::LuaRef& luaTableData, 
		bool isSpecificZIndex,
		std::size_t zIndex);

	InteractionFuncData(const InteractionFuncData& right);

	InteractionFuncData& operator=(InteractionFuncData& right);

	std::shared_ptr<luabridge::LuaRef> mLuaInteractionFunc;
	std::shared_ptr<luabridge::LuaRef> mLuaTableData;

	Entity* mOwnerEntity;
	InteractionFunc mInteractionFunc;
	bool mIsSpecificZIndex;
	std::size_t mTargetZIndex;
};

class EntityManager{
public:
	typedef std::unique_ptr<EntityManager> Ptr;
	typedef std::map<std::size_t, std::vector<Entity::Ptr>> EntitiesMap;
	typedef std::vector<Entity::Ptr>::const_iterator EntityIter;
public:
	EntityManager(const std::vector<std::string>& listOfLayer);
	~EntityManager();

	void processQueuedEntityData(ComponentArranger* compArranger, lua_State* luaState,
		GeneralData* generalData = nullptr);

	std::shared_ptr<Entity> createEntity(const std::string& layer);
	std::shared_ptr<Entity> createEntity(int zIndex);
	
	//Entity* getEntityById(std::size_t id) const;

	bool isEntityRemoveable(Entity* entity) const;

	void queueEntityScript(const std::string& layerName, const std::string& scriptDir,
		const std::string& tableName, bool isLocalVar, lua_State* luaState);

	void queueEntityZScript(std::size_t zIndex, const std::string& scriptDir,
		const std::string& tableName, bool isLocalVar, lua_State* luaState);

	void queueEntityFromEngine(std::size_t zIndex, const std::string& scriptDir,
		const std::string& tableName, bool isLocalVar,
		const QueueEntityData::EngineInitializeFunc* engineFunc);

	void queueEntityFromEngine(const std::string& layerName, const std::string& scriptDir,
		const std::string& tableName, bool isLocalVar,
		const QueueEntityData::EngineInitializeFunc* engineFunc);

	void cleanEntityToBeRemoved(b2World* b2WorldArg = nullptr);
	
	std::string zIndexToLayer(std::size_t zIndex) const;
	std::size_t layerToZIndex(const std::string& layerName) const;

	void pushInteractionFuncData(Entity* ownerEntity, const InteractionFuncData::InteractionFunc& func, 
		bool isSpecificZIndex,
		std::size_t zIndex);

	void pushInteractionFuncData(Entity* ownerEntity, const InteractionFuncData::InteractionFunc& func,
		bool isSpecificZIndex,
		const std::string& layerName);

	void pushInteractionFuncData(const InteractionFuncData& funcData);

	void pushInteractionFuncDataScript(Entity* ownerEntity, bool isSpecificZIndex, 
		std::size_t zIndex, lua_State* luaState);

private:
	void matchEntityToInteractionFunc(Entity* entity, 
		const std::vector<InteractionFuncData*>& interactionList);
private:
	std::unordered_map<std::string, std::size_t> mLayerToZIndex;
	std::unordered_map<std::size_t, std::string> mZIndexToLayer;
	
	EntitiesMap mEntities;
	static std::size_t mCurrentEntityIncremental;
	std::queue<QueueEntityData> mEntitiesToBeCreated;
	std::vector<InteractionFuncData> mInteractionFuncList;
};

