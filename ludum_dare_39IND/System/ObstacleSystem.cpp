#include "ObstacleSystem.h"
#include "../Entity/Entity.h"
#include "../Component/Body2dComponent.h"
#include "../Component/ObstacleComponent.h"
#include "../Helper/Utility.h"
#include "../Helper/Math.h"
#include <Box2D\Collision\Shapes\b2PolygonShape.h>
#include <Box2D\Collision\Shapes\b2CircleShape.h>

ObstacleSystem::ObstacleSystem(GridMap& gridMap)
:mGridMap(gridMap)
{
	pushRequiredComponent(ComponentIdentifier::ObstacleComponent);
	pushRequiredComponent(ComponentIdentifier::Body2dComponent);
	mObstacleVertices.setPrimitiveType(sf::Quads);
}


ObstacleSystem::~ObstacleSystem()
{
}


bool ObstacleSystem::initiatingTheUpdateCycle(EntityManager* entityManager, sf::Time dt)
{
	mGridMap.resetObstacleMaps();
	return true;
}

void ObstacleSystem::draw(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(mObstacleVertices, states);
}

void ObstacleSystem::processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	Body2dComponent* body2dComp = entity->getComp<Body2dComponent>();
	b2Transform worldTransform = body2dComp->getWorldTransform();
	b2CircleShape* circleShape = body2dComp->getMainCircleShape();
	b2PolygonShape* polyShape = body2dComp->getMainPolyShape();

	b2AABB b2BoundingBox;

	if (circleShape)
		circleShape->ComputeAABB(&b2BoundingBox, worldTransform, 0);
	else if (polyShape)
		polyShape->ComputeAABB(&b2BoundingBox, worldTransform, 0);

	if (circleShape || polyShape)
		checkAABBObstacle(*entity, b2BoundingBox);
}

/*void ObstacleSystem::checkPolyShapedObstacle(Entity* entity, Body2dComponent* body2dComp,
b2PolygonShape* polyShape)
{
b2AABB polyAABB;


polyShape->ComputeAABB(&polyAABB, worldTransform, 0);
sf::FloatRect polySfRect = Utility::b2AABBToSfFloatRect(polyAABB);
std::vector<GridNode*> potentialNodes;

mGridMap.getListOfNodesBasedOnBoundingRect(polySfRect, potentialNodes);

for (auto& node : potentialNodes)
mGridMap.mObstacleMaps[node].push_back(entity);
}*/


void ObstacleSystem::checkAABBObstacle(Entity& entity, const b2AABB& b2BoundingBox)
{
	sf::FloatRect obstacleSfRect = Utility::b2AABBToSfFloatRect(b2BoundingBox);
	/*obstacleSfRect.top = std::ceil(obstacleSfRect.top);
	obstacleSfRect.left = std::ceil(obstacleSfRect.left);
	obstacleSfRect.width = std::floor(obstacleSfRect.width);
	obstacleSfRect.height = std::floor(obstacleSfRect.height);*/

	sf::Vector2f pos = entity.getComp<Body2dComponent>()->getSfWorldPosition();
	sf::Vector2f midOfRect = Math::getMidOfRect(obstacleSfRect);

	std::vector<GridNode*> potentialNodes;

	mGridMap.getListOfNodesBasedOnBoundingRect(obstacleSfRect, potentialNodes);

	std::size_t obstacleCategory = entity.getComp<ObstacleComponent>()->getObstacleCategory();
	for (auto& node : potentialNodes)
		mGridMap.markGridNodeObstacle(node, obstacleCategory);
}


void ObstacleSystem::finalizingTheUpdateCycle(EntityManager* entityManager, sf::Time dt)
{
	mObstacleVertices.clear();

	const sf::Vector2f halfTileSize(3.f, 3.f);
	sf::Color color(100, 25, 155, 155);
	for (auto& pair : mGridMap.mObstacleMaps){
		sf::Vector2f gridPos = pair.first->mGridPos;
		mObstacleVertices.append(sf::Vertex(
			sf::Vector2f(gridPos.x - halfTileSize.x, gridPos.y - halfTileSize.y),
			color));
		mObstacleVertices.append(sf::Vertex(
			sf::Vector2f(gridPos.x + halfTileSize.x, gridPos.y - halfTileSize.y),
			color));
		mObstacleVertices.append(sf::Vertex(
			sf::Vector2f(gridPos.x + halfTileSize.x, gridPos.y + halfTileSize.y),
			color));
		mObstacleVertices.append(sf::Vertex(
			sf::Vector2f(gridPos.x - halfTileSize.x, gridPos.y + halfTileSize.y),
			color));
	}
}