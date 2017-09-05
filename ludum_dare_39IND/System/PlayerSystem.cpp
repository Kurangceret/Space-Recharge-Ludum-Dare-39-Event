#include "PlayerSystem.h"
#include "../Entity/Entity.h"
#include "../Component/Body2dComponent.h"
#include "../Component/PlayerComponent.h"
#include "../Component/RealTimeInputComponent.h"
#include "../Component/AutoBody2dMovementComponent.h"
#include "../Component/EntityParentComponent.h"
#include "../Component/JumpComponent.h"
#include "../Helper/Math.h"
#include "../Helper/Constant.h"

PlayerSystem::PlayerSystem()
:mPlayerEntity(nullptr)
{
	pushRequiredComponent(ComponentIdentifier::PlayerComponent);
}


PlayerSystem::~PlayerSystem()
{
}


void PlayerSystem::handleEvent(const sf::RenderWindow& renderWindow, const sf::Event& event)
{
	if (!mPlayerEntity)
		return;

	

}


void PlayerSystem::processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	if (!mPlayerEntity)
		mPlayerEntity = entity;
	
}


