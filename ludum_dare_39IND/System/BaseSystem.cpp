#include "BaseSystem.h"
#include "../Entity/Entity.h"
#include "../Component/Body2dComponent.h"
#include "../Helper/Utility.h"
#include "../Helper/Math.h"
#include <Box2D\Collision\Shapes\b2PolygonShape.h>
#include <Box2D\Collision\Shapes\b2CircleShape.h>

BaseSystem::BaseSystem()
:mAdhereToBoundRect(false),
mAdhereToQuadTree(false),
mBoundAdjusterPos(96.f, 0.f)
{
}

BaseSystem::RefID BaseSystem::mRefIDCounter;

BaseSystem::~BaseSystem()
{
}


void BaseSystem::draw(sf::RenderTarget& target, sf::RenderStates states)
{

}

void BaseSystem::update(EntityManager* entityManager, sf::Time dt)
{
	finalizingTheUpdateCycle(entityManager, dt);
}

void BaseSystem::handleEvent(const sf::RenderWindow& renderWindow, const sf::Event& event)
{
}

bool BaseSystem::initiatingTheUpdateCycle(EntityManager* entityManager, sf::Time dt)
{
	return true;
}

void BaseSystem::finalizingTheUpdateCycle(EntityManager* entityManager, sf::Time dt)
{

}

void BaseSystem::setBoundRect(const sf::FloatRect& boundRect)
{
	mBoundRect = boundRect;
}


void BaseSystem::setAdhereToQuadTree(bool flag)
{
	mAdhereToQuadTree = flag;
}

void BaseSystem::setAdhereToBoundRect(bool flag)
{
	mAdhereToBoundRect = flag;
}

bool BaseSystem::isAdheringToQuadTree() const
{
	return mAdhereToQuadTree;
}

bool BaseSystem::isAdheringToBoundRect() const
{
	return mAdhereToBoundRect;
}

void BaseSystem::setBoundAdjuster(const sf::Vector2f& boundAdjusterPos)
{
	mBoundAdjusterPos = Math::absoluteVectorf(boundAdjusterPos);
}

bool BaseSystem::isEntityRestrictedByBoundRect(Entity* entity) const
{
	if (!isAdheringToBoundRect())
		return false;

	auto* body2dComp = entity->nonCreateComp<Body2dComponent>();
	if (!body2dComp)
		return false;

	auto* polyShape = body2dComp->getMainPolyShape();
	auto* circleShape = body2dComp->getMainCircleShape();

	b2AABB bodyAABB;
	b2Transform worldTransform = body2dComp->getWorldTransform();

	if (polyShape)
		polyShape->ComputeAABB(&bodyAABB, worldTransform, 0);
	if (circleShape)
		circleShape->ComputeAABB(&bodyAABB, worldTransform, 0);

	if (Utility::b2AABBToSfFloatRect(bodyAABB).intersects(getBoundRect()))
		return false;

	return true;
}


sf::FloatRect BaseSystem::getBoundRect(bool withBoundAdjuster) const
{
	if (withBoundAdjuster){
		float xAdjuster = mBoundAdjusterPos.x;
		float yAdjuster = mBoundAdjusterPos.y;
				
		return sf::FloatRect(mBoundRect.left - xAdjuster, mBoundRect.top - yAdjuster, 
			mBoundRect.width + (xAdjuster * 2.f), mBoundRect.height + (yAdjuster * 2.f));
	}
	return mBoundRect;
}