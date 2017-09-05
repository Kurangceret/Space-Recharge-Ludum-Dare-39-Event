#include "EngineContactListener.h"
#include "../Entity/Entity.h"
#include <Box2D\Dynamics\Contacts\b2Contact.h>
#include <Box2D\Dynamics\b2Fixture.h>
#include <Box2D\Dynamics\b2Body.h>
#include "../Component/Body2dComponent.h"
#include "../Component/PlayerComponent.h"
#include "../Helper/Utility.h"

EngineContactListener::EngineContactListener(EntityManager& entityManager)
:mEntityManager(entityManager)
{
}


EngineContactListener::~EngineContactListener()
{
}


void EngineContactListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	//fixtureA->GetBody()->getC
	Entity* entityA = (Entity*)(fixtureA->GetBody()->GetUserData());
	Entity* entityB = (Entity*)(fixtureB->GetBody()->GetUserData());

	const FixtureLogic& fixtureLogicA = (*(FixtureLogic*)(fixtureA->GetUserData()));
	const FixtureLogic& fixtureLogicB = (*(FixtureLogic*)(fixtureB->GetUserData()));



	//contact->SetEnabled(false);
	if (!fixtureLogicA.mIsActive || !fixtureLogicB.mIsActive)
		return;
	

	if (fixtureLogicA.mBeginContactLuaFunc.get())
		(*fixtureLogicA.mBeginContactLuaFunc)(entityA, entityB, &mEntityManager,
		&fixtureLogicA, &fixtureLogicB);
	if (fixtureLogicB.mBeginContactLuaFunc.get())
		(*fixtureLogicB.mBeginContactLuaFunc)(entityB, entityA, &mEntityManager,
		&fixtureLogicB, &fixtureLogicA);

	/*if (fixtureLogicA.mIsActive && fixtureLogicB.mIsActive &&
		!fixtureLogicA.mIsInteractionFixture && !fixtureLogicB.mIsInteractionFixture
		&& (fixtureLogicA.mCategoryToBeCollisionSolved
		& fixtureB->GetFilterData().categoryBits) &&
		(fixtureLogicB.mCategoryToBeCollisionSolved
		& fixtureA->GetFilterData().categoryBits))
	{
		contact->SetEnabled(true);
	}*/

	b2ContactListener::BeginContact(contact);
}

void EngineContactListener::EndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	//fixtureA->GetBody()->getC
	Entity* entityA = (Entity*)(fixtureA->GetBody()->GetUserData());
	Entity* entityB = (Entity*)(fixtureB->GetBody()->GetUserData());

	const FixtureLogic& fixtureLogicA = (*(FixtureLogic*)(fixtureA->GetUserData()));
	const FixtureLogic& fixtureLogicB = (*(FixtureLogic*)(fixtureB->GetUserData()));

	//contact->SetEnabled(false);
	/*if (fixtureLogicA.mIsActive && fixtureLogicB.mIsActive)
		contact->SetEnabled(true);*/
	


	if (fixtureLogicA.mEndContactLuaFunc.get())
		(*fixtureLogicA.mEndContactLuaFunc)(entityA, entityB, &mEntityManager,
		&fixtureLogicA, &fixtureLogicB);
	if (fixtureLogicB.mEndContactLuaFunc.get())
		(*fixtureLogicB.mEndContactLuaFunc)(entityB, entityA, &mEntityManager,
		&fixtureLogicB, &fixtureLogicA);

	

	/*if (!entityA->hasComp<PlayerComponent>())
		fixtureA->GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));
	//else
		//fixtureA->GetBody()->ApplyForceToCenter(b2Vec2(0.2f, 0.2f), true);
	if (!entityB->hasComp<PlayerComponent>()){
		fixtureB->GetBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		//fixtureB->GetBody()->ApplyForceToCenter(b2Vec2(0.2f, 0.2f), true);
	}*/

	b2ContactListener::EndContact(contact);
}

void EngineContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	Entity* entityA = (Entity*)(fixtureA->GetBody()->GetUserData());
	Entity* entityB = (Entity*)(fixtureB->GetBody()->GetUserData());

	const FixtureLogic& fixtureLogicA = (*(FixtureLogic*)(fixtureA->GetUserData()));
	const FixtureLogic& fixtureLogicB = (*(FixtureLogic*)(fixtureB->GetUserData()));

	contact->SetEnabled(false);
	if (!fixtureLogicA.mIsActive || !fixtureLogicB.mIsActive){
		contact->SetEnabled(false);
		return;
	}
	
	/*Entity* entityWithFixture = fixtureLogicA.mIsInteractionFixture ? entityA : entityB;

	bool entityAIsPlayer = entityA->hasComp<PlayerComponent>() ? true : false;
	PlayerComponent* playerComp;
	playerComp = entityAIsPlayer ?	entityA->getComp<PlayerComponent>() 
		: entityB->nonCreateComp<PlayerComponent>();*/

/*	if (playerComp && playerComp->isInteractingThisFrame()){
		const FixtureLogic* fixtureWithInteraction = entityAIsPlayer ? &fixtureLogicB : &fixtureLogicA;

		if (fixtureWithInteraction->mIsInteractionFixture
			&& fixtureWithInteraction->mInteractionLuaFunc.get())
		{
			Entity* opposingEntity = entityAIsPlayer ? entityB : entityA;
			(*fixtureWithInteraction->mInteractionLuaFunc)
				(opposingEntity, playerComp->getOwnerEntity(), &mEntityManager);
		}
		
	}*/
		

	if (fixtureLogicA.mIsActive && fixtureLogicB.mIsActive &&
		!fixtureLogicA.mIsInteractionFixture && !fixtureLogicB.mIsInteractionFixture
		&& (fixtureLogicA.mCategoryToBeCollisionSolved
		& fixtureB->GetFilterData().categoryBits) &&
		(fixtureLogicB.mCategoryToBeCollisionSolved
		& fixtureA->GetFilterData().categoryBits))
	{
		contact->SetEnabled(true);
	}

	if (fixtureLogicA.mPreSolveLuaFunc.get())
		(*fixtureLogicA.mPreSolveLuaFunc)(entityA, entityB, &mEntityManager, 
			&fixtureLogicA, &fixtureLogicB);
	if (fixtureLogicB.mPreSolveLuaFunc.get())
		(*fixtureLogicB.mPreSolveLuaFunc)(entityB, entityA, &mEntityManager, 
			&fixtureLogicB, &fixtureLogicA);

	b2ContactListener::PreSolve(contact, oldManifold);
}

void EngineContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	//fixtureA->GetBody()->getC
	Entity* entityA = (Entity*)(fixtureA->GetBody()->GetUserData());
	Entity* entityB = (Entity*)(fixtureB->GetBody()->GetUserData());

	const FixtureLogic& fixtureLogicA = (*(FixtureLogic*)(fixtureA->GetUserData()));
	const FixtureLogic& fixtureLogicB = (*(FixtureLogic*)(fixtureB->GetUserData()));

	if (!fixtureLogicA.mIsActive || !fixtureLogicB.mIsActive){
		contact->SetEnabled(false);
		return;
	}

	if (fixtureLogicA.mPostSolveLuaFunc.get())
		(*fixtureLogicA.mPostSolveLuaFunc)(entityA, entityB, &mEntityManager,
		&fixtureLogicA, &fixtureLogicB);
	if (fixtureLogicB.mPostSolveLuaFunc.get())
		(*fixtureLogicB.mPostSolveLuaFunc)(entityB, entityA, &mEntityManager,
		&fixtureLogicB, &fixtureLogicA);
	//fixtureA->GetBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	
	/*b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	
	Entity* entityA = (Entity*)(fixtureA->GetBody()->GetUserData());
	Entity* entityB = (Entity*)(fixtureB->GetBody()->GetUserData());
	contact->SetEnabled(false);
	contact->GetManifold()->pointCount = 0;

	if (contact->IsTouching())
		Utility::printVector(entityA->getComp<Body2dComponent>()->getSfPosition(), true);*/
	b2ContactListener::PostSolve(contact, impulse);
}