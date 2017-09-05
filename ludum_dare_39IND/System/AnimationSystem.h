#pragma once
#include "SingleEntitySystem.h"
class AnimationSystem :	public SingleEntitySystem<AnimationSystem>{
public:
	AnimationSystem();
	virtual ~AnimationSystem();

protected:
	void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt) override;
};

