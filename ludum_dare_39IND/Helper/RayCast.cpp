#include "RayCast.h"



bool RayCast::CastRayCast(float initialX, float initialY, float destX, float destY, GridMap& gridMap)
{
	GridNode* destNode = gridMap.toGrid(destX, destY);
	if (!destNode)
		return false;
	
	float xDiff = destX - initialX;
	float yDiff = destY - initialY;

	float diffLength = std::sqrt((xDiff * xDiff) + (yDiff * yDiff));

	float xDir = xDiff / diffLength;
	float yDir = yDiff / diffLength;

	
	float dir_tan_ratio = yDir / xDir;


	//float dir_tan_ratio = std::tan(std::atan2(yDir, xDir));
	return rayCastHorizontal(initialX, initialY, xDir, yDir, destNode, destX, destY, dir_tan_ratio, gridMap) &&
		rayCastVertical(initialX, initialY, xDir, yDir, destNode, destX, destY, dir_tan_ratio, gridMap);
}


bool RayCast::rayCastHorizontal(float initialX, float initialY, float dirX, float dirY,
	GridNode* destNode, float destX, float destY, float dir_tan_ratio, GridMap& gridMap)
{
	
	if (dirY == 0.f)
		return true;

	if (gridMap.doesNodeContainsObstacle(destNode))
		return false;

	sf::Vector2f gridSize = gridMap.getGridSize();

	float yA = dirY > 0.f ? gridSize.y : -gridSize.y;

	float startX = initialX;
	float startY = initialY;
		
	float xA = std::abs(yA / dir_tan_ratio);


	GridNode* curNode = gridMap.toGrid(startX, startY);

	if (dirY < 0.f){
		startY = (curNode->mGridPos.y - (gridSize.y / 2.f)) - 1;
	}
	else if (dirY > 0.f){
		startY = (curNode->mGridPos.y + (gridSize.y / 2.f));
	}

	if (dirX != 0.f){
		float yDiff = std::abs(startY - initialY);
		float ratioDiff = std::abs(yDiff / dir_tan_ratio);

		if (dirX < 0.f){
			ratioDiff *= -1.f;
			xA *= -1.f;
		}

		startX = initialX + ratioDiff;
	}
	

	bool flag = false;

	while (true)
	{
		curNode = gridMap.toGrid(startX, startY);
				
		if (!curNode){
			flag = true;
			break;
		}

		float curDirXToDest = destX - startX;
		float curDirYToDest = destY - startY;
		float dot_product = (dirX * curDirXToDest) + (dirY * curDirYToDest);

		if (curNode == destNode || dot_product < 0.f){
			flag = true;
			break;
		}

		if (gridMap.doesNodeContainsObstacle(curNode))
			break;

		startX += xA;
		startY += yA;
	}
	return flag;
}

bool RayCast::rayCastVertical(float initialX, float initialY, float dirX, float dirY,
	GridNode* destNode, float destX, float destY, float dir_tan_ratio, GridMap& gridMap)
{
	
	if (dirX == 0.f)
		return true;

	if (gridMap.doesNodeContainsObstacle(destNode))
		return false;

	sf::Vector2f gridSize = gridMap.getGridSize();

	float xA = dirX > 0.f ? gridSize.x : -gridSize.x;

	float startX = initialX;
	float startY = initialY;
		

	float yA = std::abs(xA * dir_tan_ratio);


	GridNode* curNode = gridMap.toGrid(startX, startY);

	if (dirX < 0.f){
		startX = (curNode->mGridPos.x - (gridSize.x / 2.f)) - 1;
	}
	else if (dirX > 0.f){
		startX = (curNode->mGridPos.x + (gridSize.x / 2.f));
	}

	if (dirY != 0.f){
		float xDiff = std::abs(startX - initialX);
		float ratioDiff = std::abs(xDiff * dir_tan_ratio);
		if (dirY < 0.f){
			ratioDiff *= -1.f;
			yA *= -1.f;
		}

		startY = initialY + ratioDiff;
	}

	bool flag = false;

	while (true)
	{
		curNode = gridMap.toGrid(startX, startY);

		if (!curNode){
			flag = true;
			break;
		}

		float curDirXToDest = destX - startX;
		float curDirYToDest = destY - startY;
		float dot_product = (dirX * curDirXToDest) + (dirY * curDirYToDest);

		if (curNode == destNode || dot_product < 0.f){
			flag = true;
			break;
		}

		if (gridMap.doesNodeContainsObstacle(curNode))
			break;

		startX += xA;
		startY += yA;
	}
	return flag;
}