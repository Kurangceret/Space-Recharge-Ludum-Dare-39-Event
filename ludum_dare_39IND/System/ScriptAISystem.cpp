#include "ScriptAISystem.h"
#include "../Component/AI/ScriptAIComponent.h"

ScriptAISystem::ScriptAISystem(GridMap& gridMap)
:mGridMap(gridMap)
{
	pushRequiredComponent(ComponentIdentifier::ScriptAIComponent);
}


ScriptAISystem::~ScriptAISystem()
{
}


void ScriptAISystem::processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	entity->getComp<ScriptAIComponent>()->update(entityManager, &mGridMap, dt);
}