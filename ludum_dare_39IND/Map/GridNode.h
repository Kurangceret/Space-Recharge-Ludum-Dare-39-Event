#ifndef GRIDNODE_H
#define GRIDNODE_H
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>

struct GridNode{
	typedef std::unique_ptr<GridNode> Ptr;
	GridNode(const sf::Vector2f& gridSize, const sf::Vector2f& gridPos)
	:mGridSize(gridSize), mGridPos(gridPos), mIsTraverseable(false){}
	

	bool mIsTraverseable;
	const sf::Vector2f mGridSize;
	const sf::Vector2f mGridPos;
	std::vector<GridNode*> mAdjacentNodes;
};

#endif