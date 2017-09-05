#include "LoadingTask.h"
#include "../Helper/Constant.h"
#include "../Helper/Math.h"
#include "GeneralData.h"
#include "../Resource/ResourceHolder.h"
#include "../Entity/EntityManager.h"
#include "../Event/EventManager.h"
#include "../Helper/Utility.h"
#include "../WindowState/GuiGameState.h"
#include "../GUI/GuiButton.h"
#include "../GUI/GuiFlipableContainer.h"


#include "../Component/AllEntityComponentsHeader.h"
#include "../System/AllSystemsHeader.h"

#include "../Event/PrintEvent.h"
#include "../Event/BaseEvent.h"
#include "../CustomBox2D/SfDebugB2Draw.h"
#include "../Tile/TileSidesSensorGenerator.h"

ImportedTileData::ImportedTileData(GridMap* gridMap, sf::Texture* texture,
	const sf::IntRect& intRect, const sf::Vector2f& worldPos,
	const std::string& layerName, const int gameObjectType, TiledMapSystem* tiledMap)
:mGridMap(gridMap),
mTileTexture(texture),
mIntRect(intRect),
mWorldPos(worldPos),
mLayerName(layerName),
mGameObjectType(gameObjectType),
mTiledMapSystem(tiledMap),
mTextureManagerType(TexturesInt)
{}



bool ImportedTileData::isTexturesManagerInt() const
{
	return mTextureManagerType == TexturesInt;
}

bool ImportedTileData::isTexturesManagerStr() const
{
	return mTextureManagerType == TexturesStr;
}

void ImportedTileData::setTexturesManagerType(TextureManagerType type)
{
	mTextureManagerType = type;
}

LoadingTask::LoadingTask(GeneralData* generalData, GuiGameStateData* guiGameStateData, lua_State* luaState,
	sf::RenderWindow& renderWindow)
: mThread(&LoadingTask::runTask, this),
mFinished(false),
mGeneralData(generalData),
mCurrentPercent(0.f),
mCurrentProcess("Loading...."),
mIsThreadTerminated(false),
mLuaState(luaState),
mGuiGameStateData(guiGameStateData),
mRenderWindow(renderWindow)
{
}

LoadingTask::~LoadingTask()
{

}

void LoadingTask::execute()
{
	mFinished = false;
	mCurrentPercent = 0.f;
	mCurrentProcess = "Loading....";
	mThread.launch();
}

sf::RenderWindow& LoadingTask::getSafeThreadedWindow()
{
	sf::Lock lock(mMutex);
	return mRenderWindow;
}

bool LoadingTask::isFinished()
{
	sf::Lock lock(mMutex);
	return mFinished;
}

bool LoadingTask::isThreadTerminated()
{
	sf::Lock lock(mMutex);
	return mIsThreadTerminated;
}

float LoadingTask::getCompletion()
{
	sf::Lock lock(mMutex);
	return mCurrentPercent;
}

std::string LoadingTask::getCurrentText()
{
	sf::Lock lock(mMutex);
	return mCurrentProcess;
}

void LoadingTask::setGeneralData(GeneralData* generalData)
{
	mGeneralData = generalData;
}

void LoadingTask::setGuiGameStateData(GuiGameStateData* guiGameStateData)
{
	mGuiGameStateData = guiGameStateData;
}

void LoadingTask::runTask()
{
	mGeneralData->setSystemManager(new SystemManager());
	

	mGeneralData->mGridMap.reset(new GridMap());
	mGeneralData->setTextureHolderInt(new TextureHolderInt());
	mGeneralData->setTextureHolderStr(new TextureHolderStr());
	
	mGeneralData->setTiledMap(new TiledMapSystem(*mGeneralData->getTextureHolderInt()));
	mGeneralData->setTexturesStringManager(new TextureStringManager());

	if (luaL_dofile(mLuaState, std::string(Constant::scriptDir + 
		"UtilityFunctionScript.lua").c_str()) != 0)
	{
		std::cout << lua_tostring(mLuaState, -1) << std::endl;
		assert(false == true);
	}

	luabridge::LuaRef utilityTableFunc = luabridge::LuaRef::getGlobal(
		mLuaState, "Utility");

	if (luaL_dofile(mLuaState, std::string(Constant::scriptDir 
		+ "EntityScriptDirectoryData.lua").c_str()) != 0)
	{
		std::cout << lua_tostring(mLuaState, -1) << std::endl;
		assert(false == true);
	}

	luabridge::LuaRef entityScriptDirData = luabridge::LuaRef::getGlobal(
		mLuaState, "EntityScriptDirData");
	luabridge::LuaRef iteratedEntityScriptDirData = luabridge::LuaRef::getGlobal(
		mLuaState, "IteratedEntityScriptDirData");

	
	mGeneralData->setComponentArranger(new ComponentArranger(mGeneralData, 
		entityScriptDirData, iteratedEntityScriptDirData, utilityTableFunc, mLuaState));
			
	TiledMapSystem& mTiledMap = *(mGeneralData->getTiledMap());
		
	TextureHolderInt& mTexturesInt = *(mGeneralData->getTextureHolderInt());
	TextureHolderStr& mTexturesStr = *(mGeneralData->getTextureHolderStr());
	
		
	{
		sf::Lock lock(mMutex);
		mCurrentProcess = "Loading Map...";
		mCurrentPercent = 0.1f;
	}
	bindLogicsToLuaScript();
	try{
		lua_State* luaState = mLuaState;
		if (luaL_dofile(luaState, std::string(Constant::scriptDir +
			"SettingScripts/StartupScript.lua").c_str()) != 0)
		{
			std::cout << lua_tostring(luaState, -1) << std::endl;
			assert(false);
		}

		//luabridge::LuaRef settingTable = 
			//luabridge::getGlobal(luaState, "InitialSetting");
		luabridge::LuaRef settingTable = luabridge::LuaRef::fromStack(luaState, -1);
		try{
			std::cout << settingTable["initialLevel"].cast<std::string>();
			mTiledMap.loadMap(settingTable["initialLevel"].cast<std::string>());
			float levelGravity = std::stof(mTiledMap.getMapCustomPropertyVal("Gravity"));
			mGeneralData->mB2World.reset(new b2World(b2Vec2(0.f, levelGravity)));
			mGeneralData->mB2World->SetAllowSleeping(false);
			
		}
		catch (luabridge::LuaException& e){
			std::cout << e.what() << std::endl;
			assert(false);
		}
	}
	catch (std::runtime_error message){
		//std::cout << message.what() << std::endl;
		mGeneralData->clearAllData();
		sf::Lock lock(mMutex);
		mIsThreadTerminated = true;
		mThread.terminate();
	}
	

	

	{
		sf::Lock lock(mMutex);
		mCurrentProcess = "Preparing Path Finding...";
		mCurrentPercent = 0.25f;
	}

	mGeneralData->mGridMap->initializeGridMap(sf::Vector2f(64.f, 64.f), mTiledMap.getMapSize());
	//mGeneralData->mGridMap->initializeGridMap(mTiledMap.getTileSize(), mTiledMap.getMapSize());

	mGeneralData->setEntityManager(new EntityManager(mTiledMap.getLayersList()));
	EntityManager& mEntityManager = *(mGeneralData->getEntityManager());
	

	{
		sf::Lock lock(mMutex);
		sf::Vector2f windowSize = mRenderWindow.getView().getSize();
		/*mGeneralData->mCamera.reset(new Camera(mRenderWindow, windowSize.x, windowSize.y, 1.f,
			mGeneralData->mGridMap->getMapSize() * mGeneralData->mGridMap->getGridSize().x));*/

		mGeneralData->mCamera.reset(new Camera(mRenderWindow, windowSize.x, windowSize.y, 1.f,
			mTiledMap.getMapSize() * mTiledMap.getTileSize().x));
		
	}

	float incrementPerLayer = 0.f;

	{
		sf::Lock lock(mMutex);
		mCurrentProcess = "Loading general info and textures...";
		mCurrentPercent = 0.50f;
		incrementPerLayer = (1.f - mCurrentPercent) / static_cast<float>(mTiledMap.getLayersList().size());
	}

	//loadManualTexture();
		
	initializeMap();
	initializeSystemManager();
	initializeGuiManager();
	mEntityManager.processQueuedEntityData(mGeneralData->getComponentArranger(), mLuaState, 
		mGeneralData);

	{
		sf::Lock lock(mMutex);
		mCurrentPercent = 1.f;
		mCurrentProcess = "Finished!";
		mFinished = true;
	}

	
}

//will setup the map and add entities and vertex to the mSceneGraph
//also will add any rectangular object if there is any and will cam setupTmxObject
//for every object in the layer
void LoadingTask::initializeMap()
{
	TiledMapSystem& tiledMap = *mGeneralData->getTiledMap();
	TextureHolderInt& texturesInt = *mGeneralData->getTextureHolderInt();
	
	TextureStringManager& texturesStringManager = *mGeneralData->getTexturesStringManager();
	
	lua_State* luaState = mLuaState;

	if (luaL_dofile(luaState, std::string(Constant::scriptDir 
		+ "GameObjectAdderScript.lua").c_str()) != 0)
	{
		std::cout << lua_tostring(luaState, -1) << std::endl;
		assert(false);
	}

	luabridge::LuaRef gameObjAdderTable = luabridge::LuaRef::fromStack(luaState, -1);
	luabridge::LuaRef checkObjTypeLuaFunc = gameObjAdderTable["checkGameObjectType"].cast<luabridge::LuaRef>();
	

	EntityManager* entityManager = mGeneralData->getEntityManager();
	std::unique_ptr<ImportedTileData> importedTileData;

	EngineObjectInitializer engineObjectInitializer;

	TileSidesSensorGenerator tileSidesSensorGen;

	engineObjectInitializer.mTileSidesSensorGen = &tileSidesSensorGen;
	
	const char* gameObjectPropertyKeyStr = "GameObjectType";

	sf::Vector2f tileSize = tiledMap.getTileSize();

	for (auto& layer : tiledMap.getLayersList()){
		for (std::size_t y = 0; y < tiledMap.getMapSize().y; y++){
			for (std::size_t x = 0; x < tiledMap.getMapSize().x; x++){
				unsigned int tileId = tiledMap.At(layer, x, y);
				if (tileId <= 0)
					continue;

				unsigned int tileSetId = tiledMap.tileToTileSet(tileId);

				int gameObjType = (std::stoi(
					tiledMap.propertyKeyToValue(tileSetId, tileId, gameObjectPropertyKeyStr).c_str()));

				importedTileData.reset(new ImportedTileData(mGeneralData->mGridMap.get(),
					&texturesInt.get(tileSetId),
					tiledMap.tileSetToRect(tileSetId, tileId),
					tiledMap.coordToPosition(x, y), layer, gameObjType, &tiledMap));

				importedTileData->mTileId = tileId;
				importedTileData->mTileSetId = tileSetId;
				try{
					checkObjTypeLuaFunc(gameObjAdderTable, entityManager, importedTileData.get(),
						engineObjectInitializer);
				}
				catch (luabridge::LuaException& e){
					std::cout << e.what() << std::endl;
				}
			}
		}
	}

	for (const auto& objLayerPair : tiledMap.getObjectLayerContainer()){
		if (Utility::transformStrToLowerCase(objLayerPair.first.layerType) != "tile")
			continue;

		std::string tileLayer = tiledMap.getObjectLayerCustomPropertyVal(
			objLayerPair.first.layerName, "TileLayer");

		if (tileLayer == "")
			continue;

		for (const auto& obj : objLayerPair.second){
			
			auto& tileData = *tiledMap.getTileData(obj.referenceGID, obj.id);
			auto findIter = tileData.properties.find(gameObjectPropertyKeyStr);

			if (findIter == tileData.properties.end())
				continue;
			
			int gameObjType = std::stoi(findIter->second);

			//object in Tiled origin pos is bot left, rather than top left
			

			auto& objData = tiledMap.getObjectData(obj.referenceGID, obj.id, tileLayer);
			

			float xDegreeOffset = 0.0f;
			float yDegreeOffset = 270.0f;

			if (obj.rotation != 0.f)
				int i = 0;

			xDegreeOffset = Math::plusDegree(xDegreeOffset, obj.rotation);
			yDegreeOffset = Math::plusDegree(yDegreeOffset, obj.rotation);

			sf::Vector2f xOffsetDir = Math::degreeToVector(xDegreeOffset);
			sf::Vector2f yOffsetDir = Math::degreeToVector(yDegreeOffset);

			sf::Vector2f objWorldPos(obj.pos);
			objWorldPos += (obj.objSize.x / 2.f * xOffsetDir);
			objWorldPos += (obj.objSize.y / 2.f * yOffsetDir);
			

			sf::Texture* texture = nullptr;
			if (tileData.imageSource != "")
				texture = texturesStringManager.loadTextures(Constant::levelDir + tileData.imageSource);

			if (!texture)
				texture = &texturesInt.get(tileData.tileFirstGID);

			if (tileData.tileRect == sf::IntRect())
				importedTileData->setTexturesManagerType(ImportedTileData::TexturesStr);			
			
			importedTileData.reset(new ImportedTileData(mGeneralData->mGridMap.get(),
				texture,
				tileData.tileRect,
				objWorldPos, tileLayer, gameObjType, &tiledMap));

			importedTileData->mTileId = obj.id;
			importedTileData->mTileSetId = obj.referenceGID;
			//importedTileData->setTexturesManagerType(ImportedTileData::TexturesStr);

			try{
				checkObjTypeLuaFunc(gameObjAdderTable, entityManager, importedTileData.get(),
					engineObjectInitializer);
			}
			catch (luabridge::LuaException& e){
				std::cout << e.what() << std::endl;
			}
		}
	}

	
}

void LoadingTask::loadAllManualTextures()
{
	auto textureStringManager = mGeneralData->mTexturesStringmanager.get();
	textureStringManager->loadTextures(Constant::textureDir + "wallpaper_3840.png");
	//textureStringManager->loadTextures(Constant::textureDir + "rotation_arrow.png");
}

void LoadingTask::initializeSystemManager()
{
	SystemManager* systemManager = mGeneralData->getSystemManager();
	

	{
		sf::Lock lock(mMutex);
		systemManager->addSystem<RenderingSpriteSystem>(mRenderWindow);
	}

	systemManager->addSystem<PlayerSystem>();
	systemManager->addSystem<Box2DWorldSystem>(*mGeneralData->mB2World);
	systemManager->addSystem<AutoBody2dMovementSystem>();
	systemManager->addSystem<ScriptUpdateSystem>();
	systemManager->addSystem<JumpSystem>();
	systemManager->addSystem<RealTimeInputSystem>();
	systemManager->addSystem<PlayerSystem>();
	systemManager->addSystem<ScriptAISystem>(*mGeneralData->mGridMap);
	systemManager->addSystem<AutomaticMovementSystem>(*mGeneralData->mGridMap);
	systemManager->addSystem<ObstacleSystem>(*mGeneralData->mGridMap);
	systemManager->addSystem<AnimationSystem>();
	systemManager->addSystem<ThorParticleSystem>(*mGeneralData->mTexturesStringmanager);
	systemManager->addSystem<PlanetSpawnSystem>();
	//systemManager->addSystem<EntityBuildingSystem>();
}



void LoadingTask::initializeGuiManager()
{
	
	mGuiGameStateData->mGuiManager.reset(new GuiManager());
	mGuiGameStateData->mLuaState = mLuaState;
	/*GuiFlipableContainer* guiFlipableContainer = new GuiFlipableContainer();

	guiFlipableContainer->setRightArrowTexture(*mGeneralData->getTexturesStringManager()->
		loadTextures("Media/Textures/gui_arrow.png"));
	guiFlipableContainer->setPositionOnWindow(sf::Vector2f(730.f, 550.f));

	BaseGuiComponent::Ptr testGuiRef(guiFlipableContainer);
	mGuiGameStateData->mGuiManager->addGuiComponent(testGuiRef);*/
}


void LoadingTask::bindLogicsToLuaScript()
{
	
}
