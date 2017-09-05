#pragma once
#include "SingleEntitySystem.h"
class JumpSystem :	public SingleEntitySystem<JumpSystem>{
public:
	JumpSystem();
	virtual ~JumpSystem();
protected:
	void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt) override;
};

