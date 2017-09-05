#include "AnimationSystem.h"
#include "../Component/AnimationComponent.h"
#include "../Entity/Entity.h"

AnimationSystem::AnimationSystem()
{
	pushRequiredComponent(ComponentIdentifier::AnimationComponent);
}


AnimationSystem::~AnimationSystem()
{
}


void AnimationSystem::processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	entity->getComp<AnimationComponent>()->updateAnimation(dt);
}