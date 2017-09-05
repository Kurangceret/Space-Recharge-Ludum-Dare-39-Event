#pragma once
#include <Box2D\Dynamics\b2WorldCallbacks.h>
#include "../Entity/EntityManager.h"
#include <unordered_map>


class FixtureLogic;

class EngineContactListener :	public b2ContactListener{
public:
	EngineContactListener(EntityManager& entityManager);
	virtual ~EngineContactListener();


	void BeginContact(b2Contact* contact) override;
	/// Called when two fixtures cease to touch.
	void EndContact(b2Contact* contact) override;

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
private:
	//bool isPairLogicCalled(const FixtureLogic* fixtureLogicA, const FixtureLogic* fixtureLogicB) const;
private:
	EntityManager& mEntityManager;
	std::vector<std::pair<const FixtureLogic*, const FixtureLogic*>> mLogicCalledFixturePairList;
};

