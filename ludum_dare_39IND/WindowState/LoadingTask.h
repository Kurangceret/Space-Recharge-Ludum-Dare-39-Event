#pragma once
#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class GeneralData;
struct GuiGameStateData;
struct lua_State;
class GridMap;
class TiledMapSystem;
class TileSidesSensorGenerator;

class ImportedTileData{
public:
	enum TextureManagerType{
		TexturesInt,
		TexturesStr,
		TextureManagerCount
	};
public:
	ImportedTileData(GridMap* gridMap, sf::Texture*, const sf::IntRect&, const sf::Vector2f&,
		const std::string&, const int, TiledMapSystem*);

	GridMap* mGridMap;
	int mGameObjectType;
	sf::Texture* mTileTexture;
	sf::IntRect mIntRect;
	sf::Vector2f mWorldPos;
	std::string mLayerName; //layer where the tile will be placed
	TiledMapSystem* mTiledMapSystem;
	int mTileSetId;
	int mTileId;

	bool isTexturesManagerInt() const;
	bool isTexturesManagerStr() const;

	void setTexturesManagerType(TextureManagerType type);

private:
	TextureManagerType mTextureManagerType;
};

struct EngineObjectInitializer{
	TileSidesSensorGenerator* mTileSidesSensorGen;
};

class LoadingTask{
public:
	LoadingTask(GeneralData* general, GuiGameStateData* guiGameStateData, lua_State* luaState,
		sf::RenderWindow& renderWindow);
	~LoadingTask();

	void setGeneralData(GeneralData* generalData);
	void setGuiGameStateData(GuiGameStateData* guiGameStateData);

	void execute();
	bool isFinished();
	float getCompletion();
	std::string getCurrentText();
	bool isThreadTerminated();
	sf::RenderWindow& getSafeThreadedWindow();
private:
	void runTask();
		
	void initializeGuiManager();

	//will setup the map and add entities and vertex to the mSceneGraph
	//also will add any rectangular object if there is any and will cam setupTmxObject
	//for every object in the layer
	void initializeMap();

	
	
	void loadAllManualTextures();

	//void loadManualTexture();
	void initializeSystemManager();
	void bindLogicsToLuaScript();
private:
	sf::Thread mThread;
	bool mFinished;
	float mCurrentPercent;
	std::string mCurrentProcess;
	sf::Mutex mMutex;

	lua_State* mLuaState;
	GeneralData* mGeneralData;
	GuiGameStateData* mGuiGameStateData;
	sf::RenderWindow& mRenderWindow;
	bool mIsThreadTerminated;
};

