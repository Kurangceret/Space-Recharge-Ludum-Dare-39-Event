#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <stack>
#include <map>
#include <queue>
#include <functional>
#include <set>
#include <unordered_map>
#include <SFML/Graphics/RenderWindow.hpp>
#include "MovingNode.h"
#include "GridMap.h"

class Entity;
struct lua_State;

struct ComparePair{
	bool operator()(const std::pair<GridNode*, float>& left,
	const std::pair<GridNode*, float>& right) const
	{
		return left.second > right.second;
	}
};



class AStarPathFinder{
public:
	AStarPathFinder();
	~AStarPathFinder();

	//returns the achievable target node
	//in the case the original target was not reacheable
	//GridNode* moveSceneTo(Entity *entity, const sf::Vector2f& targetPos, GridMap& gridMap);

	std::vector<MovingNode> constructPath(const sf::Vector2f& initialPos, const sf::Vector2f& destinationPos,
		GridNode*& reacheableNode, GridMap& gridMap, const sf::FloatRect* rect);

private:
	/*std::vector<MovingNode> constructPath(Entity *entity, const sf::Vector2f& pos,
	GridNode*& reacheableNode, GridMap& gridMap);*/

	bool isDiagonallyDifferentNode(GridNode* gridNode1, GridNode* gridNode2) const;

	std::vector<GridNode*>::const_iterator getIter(const std::vector<GridNode*>& set, const GridNode *val);
private:




};

