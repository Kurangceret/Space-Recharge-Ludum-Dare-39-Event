#ifndef MOVINGNODE_H
#define MOVINGNODE_H
#include "GridNode.h"

//this moving node will be exclusively used for entity

struct MovingNode{
	MovingNode(GridNode* gridNode, const sf::Vector2f& direction) :
	mGridNode(gridNode), mDirection(direction){}

	GridNode* mGridNode;
	sf::Vector2f mDirection;
	
};


#endif