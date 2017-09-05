#pragma once
#include "SingleEntitySystem.h"

class ScriptUpdateSystem :	public SingleEntitySystem<ScriptUpdateSystem>{
public:
	ScriptUpdateSystem();
	virtual ~ScriptUpdateSystem();
protected:
	void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt);
};

