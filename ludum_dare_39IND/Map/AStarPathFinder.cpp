#include "AStarPathFinder.h"
#include "../Helper/Utility.h"
#include "../Helper/Math.h"
#include "../Entity/Entity.h"

#include "../Component/AutomaticPathComponent.h"
#include "../Component/Body2dComponent.h"
#include <unordered_map>
#include <SFML/Graphics/VertexArray.hpp>




AStarPathFinder::AStarPathFinder()
{
}


AStarPathFinder::~AStarPathFinder()
{
}

std::vector<MovingNode> AStarPathFinder::constructPath(const sf::Vector2f& initialPos, const sf::Vector2f& destPos,
	GridNode*& reacheableNode, GridMap& gridMap, const sf::FloatRect* rect)
{
	GridNode *startNode = gridMap.toGrid(initialPos);
	GridNode *endNode = gridMap.toGrid(destPos);

	if (!startNode || !endNode){
		return std::vector<MovingNode>();
	}

	if (startNode == endNode){
		reacheableNode = endNode;
		return std::vector<MovingNode>();
	}

	GridNode *currentNode = startNode;

	//both these two variables semantic is as follow
	//costSoFar[current] = 'current' cost so far is, float var
	//cameFrom[past] = 'past' came from, GridNode* pointer

	std::unordered_map<GridNode*, float> costSoFar;
	std::unordered_map<GridNode*, GridNode*> cameFrom;


	//closed set is the one which all value has been calculated
	//and currentNode has already traversed, parent cannot be changed
	//open set is the one which all value has been calculated
	//however currentNode hasn't traversed it yet so the parent can still be changed
	std::unordered_map<GridNode*, bool> closedSet;


	std::priority_queue< std::pair<GridNode*, float>,
		std::vector<std::pair<GridNode*, float> >, ComparePair> openSet;


	openSet.push(std::make_pair(currentNode, 0.f));
	costSoFar[currentNode] = 0.f;
	cameFrom[currentNode] = nullptr;
	//isInsideQueue[currentNode] = true;

	std::vector<MovingNode> Path;

	float new_g;
	float new_f;


	std::vector<GridNode*>::const_iterator closedSetIter;


	std::vector<std::pair<GridNode*, float>> mFList;

	bool firstTime = true;


	sf::FloatRect transformRect;

	sf::Vector2f gridSize(gridMap.getGridSize());
	float sideLength = gridSize.x;
	float diagonalLength = Math::vectorLength(gridSize);

	while (!openSet.empty()){
		currentNode = openSet.top().first;
		openSet.pop();
		closedSet[currentNode] = true;

		if (currentNode != startNode)
			mFList.push_back(std::make_pair(currentNode, Math::vectorLength(currentNode->mGridPos - endNode->mGridPos)));


		if (currentNode == endNode)
			break;

		for (GridNode* n : currentNode->mAdjacentNodes){

			if (gridMap.doesNodeContainsObstacle(n) || closedSet[n])
			{
				if (n == endNode){
					endNode = currentNode;
					break;
				}
				continue;
			}
			/*if (rect){
				transformRect = *rect;
				transformRect.left += n->mGridPos.x;
				transformRect.top += n->mGridPos.y;



				std::vector<GridNode*> markedNodes;
				gridMap.getListOfNodesBasedOnBoundingRect(
					transformRect, markedNodes);

				bool nextLoop = false;
				for (auto m : markedNodes){
					if (n == m)
						continue;
					if (gridMap.doesNodeContainsObstacle(m)){
						nextLoop = true;
						break;
					}
				}
				if (nextLoop)
					continue;
			}*/

			auto costSoFarIter = costSoFar.find(n);
			float costToNNode = isDiagonallyDifferentNode(currentNode, n) ? diagonalLength : sideLength;

			new_g = costSoFar[currentNode] + costToNNode;
			if (costSoFarIter == costSoFar.end() || new_g < costSoFar[n]){
				cameFrom[n] = currentNode;
				costSoFar[n] = new_g;
				new_f = new_g + Math::vectorLength(n->mGridPos - endNode->mGridPos);
				openSet.push(std::make_pair(n, new_f));
				continue;
			}

		}
		//another test in case the final node is reached
		//but it is not movable to that loc because
		//of certain circumstances like width/height too tall
		if (currentNode == endNode)
			break;
	}

	//FLIST algorithm began
	if (currentNode != endNode && !mFList.empty()){
		std::sort(mFList.begin(), mFList.end(), [&]
			(std::pair<GridNode*, float>& a,
			std::pair<GridNode*, float>& b)
		{
			return a.second < b.second;
		});

		currentNode = mFList.front().first;
		endNode = currentNode;
	}

	reacheableNode = currentNode;

	if (currentNode == endNode){
		GridNode *n = endNode;
		GridNode *nextNode = nullptr;
		while (n != nullptr){
			MovingNode movingNode(n, sf::Vector2f());
			if (nextNode){
				MovingNode& rNode = Path.back();

				if (nextNode->mGridPos.x > n->mGridPos.x)
					rNode.mDirection.x = 1.f;
				else if (nextNode->mGridPos.x < n->mGridPos.x)
					rNode.mDirection.x = -1.f;

				if (nextNode->mGridPos.y > n->mGridPos.y)
					rNode.mDirection.y = 1.f;
				else if (nextNode->mGridPos.y < n->mGridPos.y)
					rNode.mDirection.y = -1.f;
			}

			if (n != startNode)
				Path.push_back(movingNode);
			nextNode = n;
			n = cameFrom[n];
		}
	}
	return Path;
}

bool AStarPathFinder::isDiagonallyDifferentNode(GridNode* gridNode1, GridNode* gridNode2) const
{
	return gridNode1->mGridPos.x != gridNode2->mGridPos.x && gridNode2->mGridPos.y != gridNode1->mGridPos.y;
}

std::vector<GridNode*>::const_iterator AStarPathFinder::getIter(
	const std::vector<GridNode*>& set, const GridNode *val)
{
	return (std::find(set.begin(), set.end(), val));
}