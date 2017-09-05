#pragma once
#include "SingleEntitySystem.h"
#include "../Map/GridMap.h"
#include <Box2D\Collision\Shapes\b2Shape.h>
#include <SFML/Graphics/VertexArray.hpp>

class b2PolygonShape;
class Body2dComponent;

class ObstacleSystem : public SingleEntitySystem<ObstacleSystem>{
public:
	ObstacleSystem(GridMap& gridMap);
	virtual ~ObstacleSystem();

	void draw(sf::RenderTarget& target, sf::RenderStates states) override;
protected:
	void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt) override;

	bool initiatingTheUpdateCycle(EntityManager* entityManager, sf::Time dt) override;
	void finalizingTheUpdateCycle(EntityManager* entityManager, sf::Time dt) override;
private:
	//void checkPolyShapedObstacle(Entity* entity, Body2dComponent* body2dComp, b2PolygonShape* polygonShape);
	void checkAABBObstacle(Entity& entity, const b2AABB& b2BoundingBox);
private:
	GridMap& mGridMap;
	sf::VertexArray mObstacleVertices;
};
