#pragma once
#include "SingleEntitySystem.h"
#include "../Map/AStarPathFinder.h"
#include "../Component/AutomaticPathComponent.h"

class AutomaticMovementSystem :	public SingleEntitySystem<AutomaticMovementSystem>{
public:
	AutomaticMovementSystem(GridMap& gridMap);
	virtual ~AutomaticMovementSystem();
	
private:
	virtual void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt);

private:
	void smoothAutomaticPath(Entity* entity,
		AutomaticPathComponent::AutomaticPathList& pathList);
	std::vector<sf::Vector2f> getListOfRayPoints(const sf::FloatRect& floatRect,
		const sf::Vector2f& direction, const sf::Vector2f& agentPos) const;
private:
	AStarPathFinder mAStarPathFinder;
	GridMap& mGridMap;
};

