#pragma once
#include "SingleEntitySystem.h"
#include <Box2D\Dynamics\b2World.h>
#include <map>

struct ChildOriginalStatus{
	b2Transform mOriWorldTransform;
	b2Transform mOriLocalTransform;

	b2Vec2 mParentOriLinearVelo;
	sf::Time mLastFrameDt;
};

class Body2dComponent;
class Box2DWorldSystem :public SingleEntitySystem<Box2DWorldSystem>{
public:
	Box2DWorldSystem(b2World& box2DWorld);
	virtual ~Box2DWorldSystem();
protected:
	void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt) override;
	bool initiatingTheUpdateCycle(EntityManager* entityManager, sf::Time dt) override;
	void finalizingTheUpdateCycle(EntityManager* entityManager, sf::Time dt) override;
private:
	b2World& mBox2DWorld;
	//first b2Transform is original world, second is original local
	std::map<Body2dComponent*, ChildOriginalStatus> mOriginalChildTransform;
};

