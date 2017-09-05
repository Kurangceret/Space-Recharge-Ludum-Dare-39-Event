#include "Box2DWorldSystem.h"
#include "../Entity/Entity.h"
#include "../Component/Body2dComponent.h"
#include "../Component/EntityChildrenComponent.h"

#include "../Component/PlayerComponent.h"
#include "../Helper/Math.h"
#include "../Helper/Utility.h"

Box2DWorldSystem::Box2DWorldSystem(b2World& box2DWorld)
:mBox2DWorld(box2DWorld)
{
	pushRequiredComponent(ComponentIdentifier::Body2dComponent);
	//pushRequiredComponent(ComponentIdentifier::PlayerComponent);
	pushRequiredComponent(ComponentIdentifier::EntityChildrenComponent);
}


Box2DWorldSystem::~Box2DWorldSystem()
{
}

void Box2DWorldSystem::processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	auto* body2dComp = entity->getComp<Body2dComponent>();
	//auto* playerComp = entity->nonCreateComp<PlayerComponent>();
	/*if (body2dComp->getSfLinearVelocity().y > 800.f){
		body2dComp->setVelocityY(800.f, false);
	}*/
	//if (playerComp->getCurStateEventName() == "RotationDashStateEvent")
		//Utility::printVector(body2dComp->getSfLinearVelocity(), true);
	//body2dComp->setVelocity(body2dComp->getSfLinearVelocity()), false);

	//auto* entityChildrenComp = entity->getComp<EntityChildrenComponent>();
	

	EntityChildrenComponent* entityChildComp = entity->nonCreateComp<EntityChildrenComponent>();
	if (!entityChildComp)
		return;

	b2Transform worldTransform = body2dComp->getTransform(false);
	Entity* parentEntity = entityChildComp->getParent();
	Body2dComponent* parentBody2dComp = parentEntity->getComp<Body2dComponent>();
	b2Vec2 parentMeterVelo = parentBody2dComp->getLinearVelocity();
	worldTransform.p += parentBody2dComp->getTransform().p;

	/*body2dComp->setLinearVelocity(parentMeterVelo + body2dComp->getLinearVelocity());
	if (parentMeterVelo == b2Vec2_zero)
		body2dComp->setLinearVelocity(b2Vec2_zero);*/

	worldTransform.p += b2Vec2(parentMeterVelo.x * dt.asSeconds(), parentMeterVelo.y * dt.asSeconds());

	b2Vec2 meterVelo = body2dComp->getLinearVelocity();
	ChildOriginalStatus childOriStatus;
	childOriStatus.mOriWorldTransform = worldTransform;
	childOriStatus.mOriLocalTransform = body2dComp->getTransform(false);
	childOriStatus.mOriLocalTransform.p += b2Vec2(meterVelo.x * dt.asSeconds(), meterVelo.y * dt.asSeconds());
	childOriStatus.mLastFrameDt = dt;
	childOriStatus.mParentOriLinearVelo = parentMeterVelo;

	mOriginalChildTransform.insert(std::make_pair(body2dComp, childOriStatus));
	/*mOriginalChildTransform.insert(std::make_pair(body2dComp, 
		std::make_pair(worldTransform, body2dComp->getTransform(false))));*/

	

	body2dComp->setTransform(worldTransform);
	//mOriginalChildTransform.insert(std::make_pair(body2dComp, body2dComp->getTransform(false)));
	//body2dComp->setTransform(body2dComp->getWorldTransform(false));
}

bool Box2DWorldSystem::initiatingTheUpdateCycle(EntityManager* entityManager, sf::Time dt)
{
	for (auto& pair : mOriginalChildTransform)
		pair.first->setTransform(pair.second.mOriLocalTransform);

	mOriginalChildTransform.clear();
	/*cheat code (ie: not according to the design of System/SystemManager
	but whatever, prototype is more important for now.s*/
	if (dt != sf::Time::Zero)
		return true;
	
	for (auto& pair : mOriginalChildTransform){
		b2Transform& originalWorldTransform = pair.second.mOriWorldTransform;
		b2Transform& originalLocalTransform = pair.second.mOriLocalTransform;
		b2Transform curMixedTransform = pair.first->getTransform(false);

		curMixedTransform.p -= originalWorldTransform.p;
		curMixedTransform.q.Set(curMixedTransform.q.GetAngle() - originalWorldTransform.q.GetAngle());

		curMixedTransform.p += originalLocalTransform.p;
		curMixedTransform.q.Set(curMixedTransform.q.GetAngle() + originalLocalTransform.q.GetAngle());

		//curMixedTransform.p -= pair.second.mLastFrameDt.asSeconds() * pair.second.mParentOriLinearVelo;
		pair.first->setTransform(curMixedTransform);
	}

	mOriginalChildTransform.clear();
	return false;
	/**/
}

void Box2DWorldSystem::finalizingTheUpdateCycle(EntityManager* entityManager, sf::Time dt)
{
	mBox2DWorld.Step(dt.asSeconds(), 8, 2);
	/*for (auto& pair : mOriginalChildTransform)
		pair.first->setTransform(pair.second.mOriLocalTransform);

	mOriginalChildTransform.clear();*/

	
}