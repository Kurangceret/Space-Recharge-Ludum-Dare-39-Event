#include "GridMap.h"
#include "../Entity/Entity.h"
#include "../Component/Body2dComponent.h"
#include <Box2D\Collision\Shapes\b2CircleShape.h>
#include <Box2D\Collision\Shapes\b2PolygonShape.h>
#include "../Helper/Utility.h"

GridMap::GridMap()
{
}


GridMap::~GridMap()
{
}

sf::Vector2f GridMap::coordToPosition(int x, int y) const
{
	return sf::Vector2f(x * mGridSize.x + mGridSize.x / 2,
		y * mGridSize.y + mGridSize.y / 2);
}

GridNode* GridMap::At(int x, int y) const
{
	if (x < 0 || y < 0 || x >= mMapSize.x || y >= mMapSize.y)
		return nullptr;


	int a = x + mMapSize.x * y;

	if (a >= mGridNodes.size())
		return nullptr;

	return mGridNodes[a].get();
}

GridNode* GridMap::At(const sf::Vector2i& coordinate) const
{
	return At(coordinate.x, coordinate.y);
}

GridNode* GridMap::AtScript(int x, int y) const
{
	return At(x, y);
}

const GridNode* GridMap::toGrid(Entity* entity) const
{
	if (!entity->hasComp<Body2dComponent>())
		return nullptr;

	return toGrid(entity->getComp<Body2dComponent>()->getSfWorldPosition());
}

const GridNode* GridMap::toGrid(const sf::Vector2f& pos) const
{
	return toGrid(pos.x, pos.y);
}

const GridNode* GridMap::toGrid(const float x, const float y) const
{
	float xs = x / mGridSize.x;
	float ys = y / mGridSize.y;

	if (xs < 0.f || ys < 0.f)
		return nullptr;

	return At(static_cast<int>(xs), static_cast<int>(ys));
}

GridNode* GridMap::toGrid(Entity *entity)
{
	if (!entity->hasComp<Body2dComponent>())
		return nullptr;

	return toGrid(entity->getComp<Body2dComponent>()->getSfWorldPosition());
}




GridNode* GridMap::toGrid(const sf::Vector2f& pos)
{
	return toGrid(pos.x, pos.y);
}

GridNode* GridMap::toGrid(const float x, const float y)
{
	float xs = x / mGridSize.x;
	float ys = y / mGridSize.y;

	if (xs < 0.f || ys < 0.f)
		return nullptr;

	return At(static_cast<int>(xs), static_cast<int>(ys));
}

GridNode* GridMap::toGridScript(const float x, const float y)
{
	return toGrid(x, y);
}

sf::Vector2i GridMap::toCoordinate(Entity *entity)
{
	if (!entity->hasComp<Body2dComponent>())
		return sf::Vector2i(-1, -1);

	return toCoordinate(entity->getComp<Body2dComponent>()->getSfWorldPosition());
}

sf::Vector2i GridMap::toCoordinate(const sf::Vector2f& pos)
{
	return toCoordinate(pos.x, pos.y);
}

sf::Vector2i GridMap::toCoordinate(const float x, const float y)
{
	float xs = x / mGridSize.x;
	float ys = y / mGridSize.y;

	return sf::Vector2i(static_cast<int>(xs), static_cast<int>(ys));
}

sf::Vector2i GridMap::toCoordinateScript(const float x, const float y)
{
	return toCoordinate(x, y);
}

sf::Vector2f GridMap::getGridSize() const
{
	return mGridSize;
}

sf::Vector2f GridMap::getMapSize() const
{
	return mMapSize;
}

void GridMap::initializeGridMap(const sf::Vector2f& gridSize,
	const sf::Vector2f& mapSize)
{
	mGridSize = gridSize;
	mMapSize = mapSize;
	//allocate grid node object
	for (int y = 0; y < mapSize.y; y++){
		for (int x = 0; x < mapSize.x; x++){
			GridNode::Ptr gridNode(new GridNode(gridSize, coordToPosition(x, y)));
			mGridNodes.push_back(std::move(gridNode));
		}
	}

	//then we begin initializng each node adjacent list
	int mapX = static_cast<int>(mMapSize.x);
	int mapY = static_cast<int>(mMapSize.y);
	for (int x = 0; x < mapX; x++){
		for (int y = 0; y < mapY; y++){
			if (At(x - 1, y - 1))	At(x, y)->mAdjacentNodes.push_back(At(x - 1, y - 1));
			if (At(x - 0, y - 1))	At(x, y)->mAdjacentNodes.push_back(At(x - 0, y - 1));
			if (At(x + 1, y - 1))	At(x, y)->mAdjacentNodes.push_back(At(x + 1, y - 1));
			if (At(x - 1, y - 0))	At(x, y)->mAdjacentNodes.push_back(At(x - 1, y - 0));
			if (At(x + 1, y - 0))	At(x, y)->mAdjacentNodes.push_back(At(x + 1, y - 0));
			if (At(x - 1, y + 1))	At(x, y)->mAdjacentNodes.push_back(At(x - 1, y + 1));
			if (At(x - 0, y + 1))	At(x, y)->mAdjacentNodes.push_back(At(x - 0, y + 1));
			if (At(x + 1, y + 1))	At(x, y)->mAdjacentNodes.push_back(At(x + 1, y + 1));
		}
	}
}

bool GridMap::is_entity_not_obstacled_on_node(Entity* entity, GridNode* selected_node)
{
	if (!selected_node)
		return false;

	Body2dComponent* body2d_comp = entity->getComp<Body2dComponent>();

	b2AABB b2BoundingBox;

	b2CircleShape* circleShape = body2d_comp->getMainCircleShape();
	b2PolygonShape* polyShape = body2d_comp->getMainPolyShape();
	b2Transform transform;
	transform.SetIdentity();
	if (circleShape)
		circleShape->ComputeAABB(&b2BoundingBox, transform, 0);
	else if (polyShape)
		polyShape->ComputeAABB(&b2BoundingBox, transform, 0);

	

	sf::FloatRect rect = Utility::b2AABBToSfFloatRect(b2BoundingBox);

	rect.left = selected_node->mGridPos.x;
	rect.top = selected_node->mGridPos.y;


	std::vector<GridNode*> marked_nodes;
	getListOfNodesBasedOnBoundingRect(rect, marked_nodes);

	for (auto node : marked_nodes)
	{
		if (!node || doesNodeContainsObstacle(node))
			return false;
	}

	return true;
	
}

void GridMap::getListOfNodesBasedOnBoundingRect(const sf::FloatRect& rect,
	std::vector<GridNode*>& markedNodes)
{
	float startX = rect.left;
	float startY = rect.top;

	float totalWidth = rect.width - 1;
	float totalHeight = rect.height - 1;

	float incrementX = 0.f;
	float incrementY = 0.f;
	

	sf::Vector2f tileSize = getGridSize();
	bool totalWidthIsFinished = false;
	bool totalHeightIsFinished = false;
	//if there are still height to be checked
	while (totalHeight > 0){
		//we minus it first, indicating a new loop is running
		//if (!firstY)
		if (totalHeightIsFinished){
			//totalHeightIsFinished = true;
			totalHeight = 0.f;
		}
		//if there are still width to be checked
		while (totalWidth > 0){
			if (totalWidthIsFinished){
				//totalWidthIsFinished = true;
				totalWidth = 0.f;
			}

			//if (!firstX)
			totalWidth -= tileSize.x;


			//we calculate the next node
			GridNode *newNode = toGrid(sf::Vector2f(startX + incrementX, startY + incrementY));

			//we increases the increment to pursue our test further
			//assuming the next loops is actually there
			incrementX += tileSize.x;
			//but if our current to be tested width is alreayd less
			//then the standard tile size, we have to change the added value
			//accordingly
			if (totalWidth < 0.f && !totalWidthIsFinished){
				incrementX -= tileSize.x;
				incrementX += std::abs(tileSize.x - (std::abs(totalWidth)));
				totalWidth = incrementX;
				totalWidthIsFinished = true;
			}
			//if this node is already included before, stop them from duplicating
			if (std::find(markedNodes.begin(),
				markedNodes.end(), newNode) != markedNodes.end())
				continue;
			if (!newNode)
				continue;
			//newNode->tile = entity;
			markedNodes.push_back(newNode);


		}
		totalHeight -= tileSize.y;

		totalWidth = rect.width - 1.f;
		incrementX = 0.f;

		incrementY += tileSize.y;

		if (totalHeight < 0.f && !totalHeightIsFinished){
			incrementY -= tileSize.y;
			incrementY += std::abs(tileSize.y - std::abs(totalHeight));
			totalHeight = incrementY;
			totalHeightIsFinished = true;
		}

		totalWidthIsFinished = false;
	}
}

void GridMap::resetObstacleMaps()
{
	mObstacleMaps.clear();
}

bool GridMap::doesNodeContainsObstacle(const GridNode* gridNode) const
{
	if (!gridNode)
		return false;

	auto find = mObstacleMaps.find(gridNode);
	if(find == mObstacleMaps.end())
		return false;

	//if (destroyerException && find->second.isDestroyer)
		//return false;

	return true;
}

void GridMap::markGridNodeObstacle(GridNode* gridNode, std::size_t obstacleCategory, bool replaceObstacleCategory)
{
	auto& obstacle = mObstacleMaps[gridNode];
	obstacle.mGridNode = gridNode;
	if (replaceObstacleCategory)
		obstacle.mObstacleCategory = obstacleCategory;
	else
		obstacle.mObstacleCategory |= obstacleCategory;
}

void GridMap::deleteGridNodeObstacle(GridNode* gridNode)
{
	auto findIter = mObstacleMaps.find(gridNode);
	if (findIter == mObstacleMaps.end())
		return;
	mObstacleMaps.erase(findIter);
}