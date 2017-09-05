#include "EntityManager.h"
#include "../Component/CategoryComponent.h"
#include "../Component/HealthComponent.h"

#include "../Component/DestroyableComponent.h"
#include "../Component/EntityParentComponent.h"
#include "../Component/EntityChildrenComponent.h"
#include "../Component/ComponentArranger.h"
#include "../Component/DestroyableComponent.h"

#include "../Component/PlayerComponent.h"
#include "../WindowState/GeneralData.h"
#include "../Component/Body2dComponent.h"
#include <Box2D\Dynamics\b2World.h>
#include <algorithm>

std::size_t EntityManager::mCurrentEntityIncremental = 0;

QueueEntityData::QueueEntityData(const std::string& layerName,
	const std::string& scriptDir, const std::string& tableName, bool isLocalVar,
	const luabridge::LuaRef* customInitializeData,
	const luabridge::LuaRef* customInitializeLuaFunc,
	const EngineInitializeFunc* customEngineInitializeFunc)
	:mIsLocalVar(isLocalVar),
	mLayer(layerName),
	mScriptDir(scriptDir),
	mTableName(tableName),
	mCustomInitializeData(nullptr),
	mCustomInitializeLuaFunc(nullptr),
	mEngineInitializeFunc()
{
	if (customInitializeData)
		mCustomInitializeData = std::make_shared<luabridge::LuaRef>(*customInitializeData);
	if (customInitializeLuaFunc)
		mCustomInitializeLuaFunc = std::make_shared<luabridge::LuaRef>(*customInitializeLuaFunc);
	if (customEngineInitializeFunc)
		mEngineInitializeFunc = *customEngineInitializeFunc;
	else
		mEngineInitializeFunc = [](Entity*){};
}



InteractionFuncData::InteractionFuncData(Entity* ownerEntity, const InteractionFunc& func, bool isSpecificZIndex,
	std::size_t zIndex)
	:mOwnerEntity(ownerEntity),
	mInteractionFunc(func),
	mIsSpecificZIndex(isSpecificZIndex),
	mTargetZIndex(zIndex)
{
}

InteractionFuncData::InteractionFuncData(Entity* ownerEntity,
	const luabridge::LuaRef& luaInteractionFunc,
	const luabridge::LuaRef& luaTableData,
	bool isSpecificZIndex,
	std::size_t zIndex)
	:mOwnerEntity(ownerEntity),
	mIsSpecificZIndex(isSpecificZIndex),
	mTargetZIndex(zIndex),
	mLuaInteractionFunc(nullptr),
	mLuaTableData(nullptr)
{
	mLuaInteractionFunc = std::make_unique<luabridge::LuaRef>(luaInteractionFunc);
	mLuaTableData = std::make_unique<luabridge::LuaRef>(luaTableData);
	mInteractionFunc = [&](Entity*){};
}

InteractionFuncData::InteractionFuncData(const InteractionFuncData& right)
:mOwnerEntity(right.mOwnerEntity),
mIsSpecificZIndex(right.mIsSpecificZIndex),
mTargetZIndex(right.mTargetZIndex),
mLuaInteractionFunc(std::move(right.mLuaInteractionFunc)),
mLuaTableData(std::move(right.mLuaTableData)),
mInteractionFunc(right.mInteractionFunc)
{
}

InteractionFuncData& InteractionFuncData::operator=(InteractionFuncData& right)
{
	mOwnerEntity = right.mOwnerEntity;
	mIsSpecificZIndex = right.mIsSpecificZIndex;
	mTargetZIndex = right.mTargetZIndex;
	mLuaInteractionFunc = std::move(right.mLuaInteractionFunc);
	mLuaTableData = std::move(right.mLuaTableData);
	mInteractionFunc = right.mInteractionFunc;
	return *this;
}


EntityManager::EntityManager(const std::vector<std::string>& listOfLayer)
{
	mCurrentEntityIncremental = 0;
	int i = 0;
	for (auto& layerName : listOfLayer){
		mLayerToZIndex[layerName] = i;
		mZIndexToLayer[i] = layerName;
		i++;
	}
}


EntityManager::~EntityManager()
{
}


std::shared_ptr<Entity> EntityManager::createEntity(const std::string& layer)
{
	auto iterLayer = mLayerToZIndex.find(layer);
	assert(iterLayer != mLayerToZIndex.end());

	Entity::Ptr entity(new Entity(mCurrentEntityIncremental++, iterLayer->second));

	Entity* entityPtr = entity.get();
	
	mEntities[layerToZIndex(layer)].push_back(entity);
	return entity;
}

std::shared_ptr<Entity> EntityManager::createEntity(int zIndex)
{	
	auto iterIndex = mZIndexToLayer.find(zIndex);
	assert(iterIndex != mZIndexToLayer.end());

	Entity::Ptr entity(new Entity(mCurrentEntityIncremental++, iterIndex->first));

	Entity* entityPtr = entity.get();
	mEntities[zIndex].push_back(entity);
	return entity;
}

//EntityManager::EntitiesLayers::iterator;

bool EntityManager::isEntityRemoveable(Entity* entity) const
{
	if (!entity->hasComp<DestroyableComponent>())
		return false;

	DestroyableComponent* destroyableComp = entity->nonCreateComp<DestroyableComponent>();

	return (destroyableComp && destroyableComp->isRemoveable());
}

void EntityManager::queueEntityScript(const std::string& layerName,
	const std::string& scriptDir,
	const std::string& tableName, bool isLocalVar, lua_State* luaState)
{
	luabridge::LuaRef luaData = luabridge::LuaRef::fromStack(luaState, 6);
	luabridge::LuaRef luaFunc = luabridge::LuaRef::fromStack(luaState, 7);

	mEntitiesToBeCreated.push(QueueEntityData(layerName, scriptDir, tableName, isLocalVar,
		&luaData, &luaFunc, nullptr));
}

void EntityManager::queueEntityZScript(std::size_t zIndex, const std::string& scriptDir,
	const std::string& tableName, bool isLocalVar, lua_State* luaState)
{
	luabridge::LuaRef luaData = luabridge::LuaRef::fromStack(luaState, 6);
	luabridge::LuaRef luaFunc = luabridge::LuaRef::fromStack(luaState, 7);

	mEntitiesToBeCreated.push(QueueEntityData(zIndexToLayer(zIndex), scriptDir, tableName, isLocalVar,
		&luaData, &luaFunc, nullptr));
}

void EntityManager::queueEntityFromEngine(std::size_t zIndex, const std::string& scriptDir,
	const std::string& tableName, bool isLocalVar,
	const QueueEntityData::EngineInitializeFunc* engineFunc)
{
	queueEntityFromEngine(zIndexToLayer(zIndex), scriptDir, tableName, isLocalVar, engineFunc);
}

void EntityManager::queueEntityFromEngine(const std::string& layerName, const std::string& scriptDir,
	const std::string& tableName, bool isLocalVar,
	const QueueEntityData::EngineInitializeFunc* engineFunc)
{
	mEntitiesToBeCreated.push(QueueEntityData(layerName, scriptDir, tableName, isLocalVar, nullptr, nullptr,
		engineFunc));
}

void EntityManager::pushInteractionFuncDataScript(Entity* ownerEntity, bool isSpecificZIndex,
	std::size_t zIndex, lua_State* luaState)
{
	luabridge::LuaRef luaData = luabridge::LuaRef::fromStack(luaState, 5);
	luabridge::LuaRef luaFunc = luabridge::LuaRef::fromStack(luaState, 6);

	mInteractionFuncList.push_back(InteractionFuncData(ownerEntity, 
		[&](Entity* entity){}, isSpecificZIndex, zIndex));

	auto& ref = mInteractionFuncList.back();
	ref.mLuaInteractionFunc = std::make_unique<luabridge::LuaRef>(luaFunc);
	ref.mLuaTableData = std::make_unique<luabridge::LuaRef>(luaData);
}


void EntityManager::pushInteractionFuncData(const InteractionFuncData& funcData)
{
	mInteractionFuncList.push_back(funcData);
}

void EntityManager::pushInteractionFuncData(Entity* ownerEntity, const InteractionFuncData::InteractionFunc& func, 
	bool isSpecificZIndex,
	std::size_t zIndex)
{
	pushInteractionFuncData(InteractionFuncData(ownerEntity, func, isSpecificZIndex, zIndex));
}

void EntityManager::pushInteractionFuncData(Entity* ownerEntity, const InteractionFuncData::InteractionFunc& func,
	bool isSpecificZIndex,
	const std::string& layerName)
{
	pushInteractionFuncData(ownerEntity, func, isSpecificZIndex, layerToZIndex(layerName));
}

void EntityManager::processQueuedEntityData(ComponentArranger* compArranger, lua_State* luaState,
	GeneralData* generalData)
{
	assert(compArranger != nullptr && luaState != nullptr);
	
	while (!mEntitiesToBeCreated.empty()){
		auto& topRef = mEntitiesToBeCreated.front();
		
		Entity::Ptr newEntity = createEntity(topRef.mLayer);
		/*if (!topRef.mIsLocalVar)
			compArranger->readFromLuaScriptName(newEntity, topRef.mScriptDir,
				topRef.mTableName);
		else
			compArranger->readFromLuaScriptReturn(newEntity, topRef.mScriptDir);*/
		if (topRef.mTableName != "" && topRef.mScriptDir == "")
			compArranger->readFromLuaPremadeScript(newEntity, topRef.mTableName);
		else if (topRef.mScriptDir != "" && topRef.mTableName == "")
			compArranger->readFromLuaScriptReturn(newEntity, topRef.mScriptDir);
		else
			assert(false);
		
		if (topRef.mCustomInitializeData.get() && topRef.mCustomInitializeLuaFunc.get()){
			try{
				(*topRef.mCustomInitializeLuaFunc)(newEntity.get(), (*topRef.mCustomInitializeData));
			}
			catch (luabridge::LuaException e){
				std::cout << "Add Entity from script : " << e.what() << std::endl;
			}
		}

		/*if (topRef.mEngineInitializeFunc.get())
			(*topRef.mEngineInitializeFunc)(newEntity.get());*/
		topRef.mEngineInitializeFunc(newEntity.get());

		if (generalData && newEntity->hasComp<PlayerComponent>())
			generalData->mPlayerEntity = newEntity.get();
		mEntitiesToBeCreated.pop();
	}
}

std::string EntityManager::zIndexToLayer(std::size_t zIndex) const
{
	auto iter = mZIndexToLayer.find(zIndex);
	assert(iter != mZIndexToLayer.end());
	return iter->second;
}

std::size_t EntityManager::layerToZIndex(const std::string& layerName) const
{
	auto iter = mLayerToZIndex.find(layerName);
	assert(iter != mLayerToZIndex.end());
	return iter->second;
}

void EntityManager::cleanEntityToBeRemoved( b2World* b2WorldArg)
{
	std::vector<std::vector<InteractionFuncData*>> interactionFuncDataSorted;
	interactionFuncDataSorted.resize(mLayerToZIndex.size());

	for (auto& interData : mInteractionFuncList){
		if (interData.mIsSpecificZIndex){
			interactionFuncDataSorted[interData.mTargetZIndex].push_back(&interData);
			continue;
		}
		for (int i = 0; i < interactionFuncDataSorted.size(); i++){
			interactionFuncDataSorted[i].push_back(&interData);
			
		}
	}

	for (auto& pair : mEntities){
		auto& entityContainer = pair.second;
		std::size_t zIndex = pair.first;

		std::vector<InteractionFuncData*>& curInteractionFuncData = interactionFuncDataSorted[zIndex];

		auto startIterToBeRemoved = std::remove_if(entityContainer.begin(), entityContainer.end(),
			[&](Entity::Ptr& ptr) -> bool
		{
			bool removeEntityProcedure = ptr->isEntityToBeRemoved();

			if (!removeEntityProcedure){
				DestroyableComponent* destroyableComp = ptr->nonCreateComp<DestroyableComponent>();
				removeEntityProcedure = destroyableComp && destroyableComp->isRemoveable();
			}

			if (removeEntityProcedure){
				ptr->markEntityToBeRemoved();
				if (b2WorldArg && ptr->hasComp<Body2dComponent>()){
					Body2dComponent* bodyComp = ptr->getComp<Body2dComponent>();
					b2WorldArg->DestroyBody(bodyComp->mBody);
				}
				EntityChildrenComponent* childComp = ptr->nonCreateComp<EntityChildrenComponent>();

				if (childComp && childComp->getParent()){
					Entity* parent = childComp->getParent();
					EntityParentComponent* parentComp = parent->getComp<EntityParentComponent>();
					parentComp->removeChild(ptr.get());
				}
				return true;
			}
			if (!removeEntityProcedure)
				matchEntityToInteractionFunc(ptr.get(), curInteractionFuncData);
			
			return false;
		});
		entityContainer.erase(startIterToBeRemoved, entityContainer.end());
	}

	mInteractionFuncList.clear();
}

void EntityManager::matchEntityToInteractionFunc(Entity* entityToBeInteracted,
	const std::vector<InteractionFuncData*>& interactionList)
{
	for (auto interaction : interactionList){
		interaction->mInteractionFunc(entityToBeInteracted);
		if (interaction->mLuaInteractionFunc.get() 
			&& interaction->mLuaTableData.get())
		{
			(*interaction->mLuaInteractionFunc)(
				interaction->mOwnerEntity, entityToBeInteracted, (*interaction->mLuaTableData)
				);
		}
	}
}