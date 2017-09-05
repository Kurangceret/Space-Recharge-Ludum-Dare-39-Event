#pragma once
#include "../Entity/EntityManager.h"
#include "../Resource/ResourceHolder.h"
#include "../Map/TiledMapSystem.h"
//#include "GameObjectAdder.h"
#include "../Resource/TextureStringManager.h"
#include "../Component/ComponentArranger.h"
#include "../System/SystemManager.h"
#include "../Map/GridMap.h"
#include "../System/Camera.h"


#include <Box2D\Dynamics\b2World.h>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>


class GeneralData{
public:
	GeneralData();
	~GeneralData();

	/*bool mCalledLuaStateBinder;

	void setLuaState(lua_State* luaState)
	{
		mLuaState = luaState;
	}*/

	void setSystemManager(SystemManager* systemManager)
	{
		mSystemManager.reset(systemManager);
	}

	void setComponentArranger(ComponentArranger* componentArranger)
	{
		mComponentArranger.reset(componentArranger);
	}

	void setTexturesStringManager(TextureStringManager* manager)
	{
		mTexturesStringmanager.reset(manager);
	}

	void setEntityManager(EntityManager* entityManager)
	{
		mEntityManager.reset(entityManager);
	}

	void setTiledMap(TiledMapSystem *tiledMap)
	{
		mTiledMap.reset(tiledMap);
	}

	void setTextureHolderInt(TextureHolderInt *texturesInt)
	{
		mTexturesInt.reset(texturesInt);
	}

	void setTextureHolderStr(TextureHolderStr *texturesStr)
	{
		mTexturesStr.reset(texturesStr);
	}
	
	std::size_t m_player_health;
	bool mIsGameOver;
	std::string m_game_over_message;


	std::unique_ptr<Camera> mCamera;
	std::unique_ptr<GridMap> mGridMap;

	SystemManager* getSystemManager(){ return mSystemManager.get(); }
	TiledMapSystem* getTiledMap() { return mTiledMap.get(); }
	TextureHolderInt* getTextureHolderInt(){ return mTexturesInt.get(); }
	TextureHolderStr* getTextureHolderStr() { return mTexturesStr.get(); }
	//GameObjectAdder* getGameObjectAdder() { return mGameObjectAdder.get(); }
	EntityManager* getEntityManager() { return mEntityManager.get(); }
	//PathFinder* getPathFinder() { return mPathFinder.get(); }
	TextureStringManager* getTexturesStringManager(){ return mTexturesStringmanager.get(); }
	ComponentArranger* getComponentArranger(){ return mComponentArranger.get(); }
	//lua_State* getLuaState(){ return mLuaState; }

	Entity* mPlayerEntity;
	

	void clearAllData();

	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<TiledMapSystem> mTiledMap;
	std::unique_ptr<TextureHolderInt> mTexturesInt;
	std::unique_ptr<TextureHolderStr> mTexturesStr;
	std::unique_ptr<b2World> mB2World;
	//std::unique_ptr<GameObjectAdder> mGameObjectAdder;
	std::unique_ptr<TextureStringManager> mTexturesStringmanager;
	std::unique_ptr<ComponentArranger> mComponentArranger;
	std::unique_ptr<SystemManager> mSystemManager;
	
	//lua_State* mLuaState;
	
	
};

