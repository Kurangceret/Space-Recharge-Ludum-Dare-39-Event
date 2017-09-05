#pragma once

#include "../Map/GridMap.h"

class RayCast{
public:
	RayCast() = default;
	~RayCast() = default;

	static bool CastRayCast(float initialX, float initialY, float destX, float destY, GridMap& gridMap);

private:
	static bool rayCastHorizontal(float initialX, float initialY, float dirX, float dirY,
		GridNode* destNode, float destX, float destY, float dir_tan_ratio, GridMap& gridMap);

	static bool rayCastVertical(float initialX, float initialY, float dirX, float dirY,
		GridNode* destNode, float destX, float destY, float dir_tan_ratio, GridMap& gridMap);
};

