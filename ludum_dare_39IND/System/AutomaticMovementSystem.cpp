#include "AutomaticMovementSystem.h"
#include "../Component/ComponentIdentifier.h"
#include "../Entity/Entity.h"
#include "../Helper/Utility.h"
#include "../Helper/Math.h"
#include "../Component/AutomaticPathComponent.h"
#include "../Component/Body2dComponent.h"
#include "../Component/AI/SteeringBehavior.h"
#include "../Helper/RayCast.h"
#include <Box2D\Collision\Shapes\b2PolygonShape.h>
#include <Box2D\Collision\Shapes\b2CircleShape.h>

AutomaticMovementSystem::AutomaticMovementSystem(GridMap& gridMap)
:mGridMap(gridMap)
{
	pushRequiredComponent(ComponentIdentifier::AutomaticPathComponent);
	
}


AutomaticMovementSystem::~AutomaticMovementSystem()
{
}


void AutomaticMovementSystem::processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt)
{

	AutomaticPathComponent* pathComponent = entity->getComp<AutomaticPathComponent>();
	auto body2dComp = entity->getComp<Body2dComponent>();
	/*if (pathComponent->isUncalculatedPosValid())
	mAStarPathFinder.moveSceneTo(entity, pathComponent->getUncalculatedDest(), mGridMap);*/


	if (pathComponent->isUncalculatedPosValid()){
		GridNode* reacheableNode = nullptr;

		b2AABB b2BoundingBox;

		b2CircleShape* circleShape = body2dComp->getMainCircleShape();
		b2PolygonShape* polyShape = body2dComp->getMainPolyShape();
		b2Transform transform;
		transform.SetIdentity();
		if (circleShape)
			circleShape->ComputeAABB(&b2BoundingBox, transform, 0);
		else if (polyShape)
			polyShape->ComputeAABB(&b2BoundingBox, transform, 0);

		sf::FloatRect rect = Utility::b2AABBToSfFloatRect(b2BoundingBox);
		//GridRect gridRect(rect.left, rect.top, rect.width, rect.height);

		auto path = mAStarPathFinder.constructPath(body2dComp->getSfWorldPosition(),
			pathComponent->getUncalculatedDest(), reacheableNode,
			mGridMap, &rect);

		pathComponent->setAutomaticPaths(path, reacheableNode);
	}

	AutomaticPathComponent::AutomaticPathList& pathList =
		pathComponent->getAutomaticPaths();


	if (pathList.empty())
		return;
	
	if (pathComponent->isPathSmooth())
		smoothAutomaticPath(entity, pathList);

	sf::Vector2f worldPos = body2dComp->getSfWorldPosition();

	MovingNode& movingNode = pathList.back();

	sf::Vector2f dirToFront = Math::unitVector(movingNode.mGridNode->mGridPos - worldPos);

	if ((Math::getDotProduct(movingNode.mDirection, dirToFront) <= 0.f) ||
		(pathList.size() == 1 && mGridMap.toGrid(worldPos) == movingNode.mGridNode))
	{
		pathList.erase(pathList.end() - 1);

		if (pathList.empty()) body2dComp->setVelocity(0.f, 0.f, false);
	}

	

	if (!pathList.empty())
		body2dComp->setVelocity(
			SteeringBehavior::seek(body2dComp, Math::unitVector(pathList.back().
			mGridNode->mGridPos - worldPos), dt));
}


void AutomaticMovementSystem::smoothAutomaticPath(Entity* entity,
	AutomaticPathComponent::AutomaticPathList& pathList)
{
	if (pathList.size() <= 1)
		return;

	Body2dComponent* body2dComp = entity->getComp<Body2dComponent>();
	b2Transform worldTransform = body2dComp->getWorldTransform();
	b2CircleShape* circleShape = body2dComp->getMainCircleShape();
	b2PolygonShape* polyShape = body2dComp->getMainPolyShape();

	b2AABB b2BoundingBox;

	if (circleShape)
		circleShape->ComputeAABB(&b2BoundingBox, worldTransform, 0);
	else if (polyShape)
		polyShape->ComputeAABB(&b2BoundingBox, worldTransform, 0);


	sf::FloatRect sfRect = Utility::b2AABBToSfFloatRect(b2BoundingBox);
	sf::Vector2f entWorldPos = body2dComp->getSfWorldPosition();

	sf::Vector2f tileSize = mGridMap.getGridSize();

	sf::Vector2f lastDir = pathList.back().mDirection;

	while (pathList.size() > 1){
		MovingNode& moveNode = pathList[pathList.size() - 2];
		float lengthDiff = Math::vectorLength(moveNode.mGridNode->mGridPos - entWorldPos);
		sf::Vector2f dir = Math::unitVector(moveNode.mGridNode->mGridPos - entWorldPos, lengthDiff);

		std::vector<sf::Vector2f> rayPoints =
			getListOfRayPoints(sfRect, dir, entWorldPos);

		sf::Vector2f diff(dir * lengthDiff);

		bool failed = false;

		for (auto& startPos : rayPoints){
			sf::Vector2f targetPos = diff + startPos;
			if (!RayCast::CastRayCast(startPos.x, startPos.y, targetPos.x, targetPos.y, mGridMap)){
				failed = true;
				pathList.back().mDirection = dir;
				break;
			}
		}
		if (failed)
			break;
		lastDir = dir;
		pathList.erase(pathList.end() - 1);

	}
}


std::vector<sf::Vector2f> AutomaticMovementSystem::getListOfRayPoints(const sf::FloatRect& rect,
	const sf::Vector2f& direction, const sf::Vector2f& agentPos) const
{
	/*std::vector<GridNode*> potentialNodes;

	mGridMap.getListOfNodesBasedOnBoundingRect(rect, potentialNodes);

	std::vector<sf::Vector2f> rayPoints;

	for (auto* gridNode : potentialNodes)
	rayPoints.push_back(gridNode->mGridPos);

	return rayPoints;*/

	sf::Vector2f tileSize = mGridMap.getGridSize();
	std::vector<sf::Vector2f> otherRayPoints;

	const sf::Vector2f topLeft = agentPos +
		sf::Vector2f((-rect.width / 2.f), (-rect.height / 2.f));
	const sf::Vector2f topRight = agentPos +
		sf::Vector2f((rect.width / 2.f), (-rect.height / 2.f));
	const sf::Vector2f bottomRight = agentPos +
		sf::Vector2f((rect.width / 2.f), (rect.height / 2.f) - 1.f);
	const sf::Vector2f bottomLeft = agentPos +
		sf::Vector2f((-rect.width / 2.f), (rect.height / 2.f) - 1.f);

	otherRayPoints.push_back(topLeft);
	otherRayPoints.push_back(topRight);
	otherRayPoints.push_back(bottomRight);
	otherRayPoints.push_back(bottomLeft);

	int widthRemainder = static_cast<int>(std::floor(((rect.width - 1.f) / tileSize.x)));
	int heightRemainder = static_cast<int>(std::floor(((rect.height - 1.f) / tileSize.y)));



	if (direction.x != 0.f && heightRemainder > 0){
		sf::Vector2f pointToBeUsed = direction.x > 0.f ? topRight : topLeft;
		float addY = rect.height / (heightRemainder + 1);

		int i = 1;
		while (i <= heightRemainder)
			otherRayPoints.push_back(pointToBeUsed + sf::Vector2f(0.f, addY * i++));

	}

	if (direction.y != 0.f && widthRemainder > 0){
		sf::Vector2f pointToBeUsed = direction.y > 0.f ? bottomLeft : topLeft;

		float addX = rect.width / (widthRemainder + 1);

		int i = 1;
		while (i <= widthRemainder)
			otherRayPoints.push_back(pointToBeUsed + sf::Vector2f(addX * i++, 0.f));
	}

	return otherRayPoints;
}

/*void AutomaticMovementSystem::smoothAutomaticPath(const sf::Vector2f& agentPos,
	const sf::FloatRect& boundingRect,
	AutomaticPathComponent::AutomaticPathList& pathList,
	EntityExpertiseComponent* entExpertiseComp)
{
	

	RayCast::TileChecker tileChecker = RayCast::mStandardTileChecker;
	if (entExpertiseComp){
		tileChecker = [&entExpertiseComp](AStarNode* curNode){
			if (!curNode || curNode->tile)
				return false;
			if (!entExpertiseComp->isAbleToFloat() && curNode->isFallable)
				return false;

			return true;
		};
	}

	while (pathList.size() > 1 && RayCast::castRayLinesFromRect(agentPos, boundingRect,
		(pathList.end() - 2)->starNode->pos, PathFinder::getInstance(), tileChecker))
	{
		pathList.erase(pathList.end() - 1);
	}
}*/