#pragma once
#include <tinyxml2.h>
#include <tmxparser.h>
#include <map>
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <memory>
#include "ObjectsType.h"
#include "../Resource/ResourceHolderType.h"

class SceneNode;




class TiledMapSystem{
public:
	typedef struct{
		//individual tile's rect
		std::string imageSource;
		sf::IntRect tileRect;
		//individual tile's properties
		tmxparser::TmxPropertyMap_t properties;
		std::size_t tileFirstGID;
	}TileData;



	//for rect object only
	typedef struct{

		std::string getObjectPropertyValue(const std::string& key) const;

		//individual rect
		sf::IntRect objectRect;
		//the type of object
		//not the type of the shape or individual tile
		ObjectsType::ID objectType;
		//name of the object
		std::string objectName;

		std::size_t referenceGID;
		std::size_t id;

		sf::Vector2f pos;
		sf::Vector2f objSize;
		//the rotation will be recoreded with the game coordinate format
		//not what the tiled coordinate format is
		float rotation;

		tmxparser::TmxPropertyMap_t properties;


	}ObjectData;

	struct ObjectLayerData{
		std::string layerName;
		std::string layerType;
	};

	typedef std::vector<ObjectData> ObjectCollection;


	typedef sf::Vector2u Coord;
	typedef std::pair<std::string, std::string> Property;
	typedef std::pair<ObjectLayerData, ObjectCollection> ObjectLayerPair;
	typedef std::vector <  ObjectLayerPair > ObjectLayerContainer;
public:
	TiledMapSystem(TextureHolderInt& texture);
	~TiledMapSystem();

	void loadMap(const std::string& filePath);

	sf::Vector2f getMapSize() const { return mSize; }

	//Example : if the tileId is 74 and it was located on tileSet with id
	//of 73 and size of 10 individual tile, the logic should be 
	//73 >= x <= 82 (10 + 73 - 1), since individual tile can also the same numberId with
	// the tileSetId
	unsigned int tileToTileSet(int tileGId);

	//don't forget that vector itself start from 0
	//(if size is 73, means its 0 - 72)
	//and example if the tileId is 74 and the tileSetId is 73 with the size
	//of 10 individual tile, if we are to put 74 onto a vector
	//with only a size of 10, it does not make sense, however if we did
	//tileId(74) - tileSetId(73), it will become 1 and can be accessed
	//from our vector
	sf::IntRect tileSetToRect(int tileSetId, int tileId);

	const TileData* getTileData(int tileGId, int tileId) const;

	std::string propertyValueWithTileSetId(int tileGId,  int tileId, const std::string& key) const;

	std::string objectPropertyValueWithTileSetId(
		int tileGId, 
		int tileId, 
		const std::string& objLayer,
		const std::string& key) const;

	const ObjectData& getObjectData(int tileGId, int tileId, 
		const std::string& objLayer) const;

	//used the same concept as tileSetToRect function for accessing individual tile
	std::string propertyKeyToValue(int tileSetId, int tileId, const std::string& key);

	unsigned int At(const std::string& layer, unsigned int x, unsigned int y) const;
	unsigned int At(const std::string& layer, Coord cord) const;

	const ObjectCollection& layerToObjectCollection(const std::string& layer) const;

	const ObjectLayerContainer& getObjectLayerContainer() const;

	std::string getObjectLayerCustomPropertyVal(const std::string& objGroupName,
		const std::string& objPropertyKey) const;

	sf::Vector2f coordToPosition(unsigned int x, unsigned int y);
	sf::Vector2f coordToPosition(Coord pos);

	std::vector<std::string>& getLayersList(){ return mLayers; }
	std::vector<std::string> getLayersList() const { return mLayers; }

	std::vector<std::string> getObjectGroupNameList() const;

	sf::Vector2f getTileSize() const { return mTileSize; }

	std::string getMapCustomPropertyVal(const std::string& mapPropertyKey) const;


	ObjectLayerContainer::const_iterator getObjLayerIter(const std::string& objLayerName) const;
private:
	void populateRect(unsigned int tileSetId, int totalTile, float imageWidth, float imageHeight,
		int margin, int spacing, const sf::Vector2f& tileSetTileSize);

	//call this function after populateRect was called
	void populateProperties(const tmxparser::TmxTileset& tileSet);

	void buildMap();
	void buildObject();
	

	sf::Color hexToColor(std::string hex);
	int charToNumber(char c);
private:
	std::unique_ptr<tmxparser::TmxMap> mTmxMap;
	//unsigned int will contain the ID for the texture which 
	//is being hold by TextureHolder, while the std::vector<IntRect>
	//will contains the individual tile in the tileset
	std::map<unsigned int, std::vector<TileData> > mTileList;
	//std::map<unsigned int, std::vector<sf::IntRect> > mTileList;
	//'std::vector<int>' will hold all the map data and the int will be the
	//identifier's id for mTileList
	//the std::string will hold the layer name
	std::map < std::string ,std::vector<unsigned int>  > mMap;
	//the key type of string will hold the layer name, so each object 
	//in this map correspond to the mMap container's layer
	ObjectLayerContainer mObjects;
	//size of the map
	sf::Vector2f mSize;
	//size of the tile on the map
	//not the size of the individual tile
	//since every tileset has their own inidividual tile
	sf::Vector2f mTileSize;
	//references to our textureholderint
	TextureHolderInt& mTextures;
	//will hold the name of the layer used for 'mMap' variable
	//and the mObjects variable
	std::vector<std::string> mLayers;
};

