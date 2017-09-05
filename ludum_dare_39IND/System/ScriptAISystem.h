#pragma once
#include "SingleEntitySystem.h"
#include "../Map/GridMap.h"
class ScriptAISystem :	public SingleEntitySystem<ScriptAISystem>{
public:
	ScriptAISystem(GridMap& gridMap);
	~ScriptAISystem();
	

protected:
	void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt) override;

private:
	GridMap& mGridMap;
};

