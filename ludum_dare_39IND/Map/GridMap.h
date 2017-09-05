#pragma once
#include <vector>
#include "GridNode.h"
#include <SFML/Graphics/Rect.hpp>
#include <unordered_map>

class Entity;
class ObstacleSystem;
class AStarPathFinder;

/*struct GridData {
	std::vector<const Entity*> entityList;
	bool isDestroyer;
};*/


struct GridObstacle{
	std::size_t mObstacleCategory;
	GridNode* mGridNode;
};

class GridMap{
public:
	friend class ObstacleSystem;
	friend class AStarPathFinder;
public:
	GridMap();
	~GridMap();

	void initializeGridMap(const sf::Vector2f& gridSize, 
		const sf::Vector2f& mapSize);

	const GridNode* toGrid(Entity*) const;
	const GridNode* toGrid(const sf::Vector2f& pos) const;
	const GridNode* toGrid(const float x, const float y) const;


	GridNode* toGrid(Entity *);
	GridNode* toGrid(const sf::Vector2f& pos);
	GridNode* toGrid(const float x, const float y);

	/*exclusive for Lua script usage*/
	GridNode* toGridScript(const float x, const float y);
	GridNode* AtScript(int x, int y) const;
	sf::Vector2i toCoordinateScript(const float x, const float y);
	/**/

	sf::Vector2i toCoordinate(Entity *entity);
	sf::Vector2i toCoordinate(const sf::Vector2f& pos);
	sf::Vector2i toCoordinate(const float x, const float y);
	
	sf::Vector2f coordToPosition(int x, int y) const;
	GridNode* At(int x, int y) const;
	GridNode* At(const sf::Vector2i& coordinate) const;

	sf::Vector2f getGridSize() const;
	sf::Vector2f getMapSize() const;

	bool is_entity_not_obstacled_on_node(Entity* entity, GridNode* selected_node);

	void getListOfNodesBasedOnBoundingRect(const sf::FloatRect& rect,
		std::vector<GridNode*>& markedNodes);

	void markGridNodeObstacle(GridNode* gridNode, std::size_t obstacleCategory, bool replaceObstacleCategory = false);

	void resetObstacleMaps();
	bool doesNodeContainsObstacle(const GridNode* gridNode) const;

	void deleteGridNodeObstacle(GridNode* gridNode);
	
private:
	std::vector<GridNode::Ptr> mGridNodes;
	sf::Vector2f mGridSize;
	sf::Vector2f mMapSize;
	std::unordered_map<const GridNode*, GridObstacle> mObstacleMaps;
	//std::unordered_map<const GridNode*, GridData> mObstacleMaps;
};

