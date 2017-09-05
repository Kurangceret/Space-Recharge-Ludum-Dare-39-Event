#include "GeneralData.h"
//#include "EventManager.h"
//#include "GridMap.h"

GeneralData::GeneralData()
:mPlayerEntity(nullptr),
m_player_health(3)
{
}


GeneralData::~GeneralData()
{
}


void GeneralData::clearAllData()
{
	//mTextures.reset(nullptr);
	//mTexturesInt->destroyAllResources();
	mTexturesInt.reset(nullptr);
	mTexturesStr.reset(nullptr);
	mTiledMap.reset(nullptr);
	//mGameObjectAdder.reset(nullptr);
	//mGridMap.reset(nullptr);
	mTexturesStringmanager.reset(nullptr);
	mSystemManager.reset(nullptr);
	mEntityManager.reset(nullptr);
	//mComponentArranger.reset(nullptr);
	//mLightSystem.reset(nullptr);
	
	/*if (mLightSystem)
		mLightSystem->ClearLights();

	GridMap::getInstance()->resetAllMapData();
	EventManager::getInstance()->cleanEverything();*/
}